#include "CAN/FS_CAN.h"
#define SERIAL_PRINT 0
#if SERIAL_PRINT == 1
#define debug(x) \
    if (Serial)  \
    Serial.print(x)
#define debugf(x, ...) \
    if (Serial)        \
    Serial.printf((x), ##__VA_ARGS__)
#define debugln(x) \
    if (Serial)    \
    Serial.println(x)
#else
#define debug(x)
#define debugf(...)
#define debugln(x)
#endif

FS_CAN::FS_CAN(FlexCAN_T4_Base *canHandle)
{
    debugln("FS_CAN Constructor called");
    FS_CAN::can = canHandle;
    txCallBackCounter = 0;
    TimerHandle_t txTimer = xTimerCreate("txTaskTimer", pdMS_TO_TICKS(1), pdTRUE, (void *)this, [](TimerHandle_t xTimer)
                                         { static_cast<FS_CAN *>(pvTimerGetTimerID(xTimer))->txCallBack(); });
    if (txTimer == NULL)
    {
        debugln("failed to start CAN TxTimer\n");
    }
    else
    {
        if (xTimerStart(txTimer, 0) != pdPASS)
        {
            debugln("failed to start CAN TxTimer\n");
        }
    }
}

void FS_CAN::CAN_Tx(CAN_MSG *msg)
{
    CAN_message_t flex_msg; // message format for FlexCAN library

    flex_msg.id = msg->id;
    debugf("transmitting message ID: %d \n", msg->id);
    for (const auto &signal : msg->signals)
    {
        debugf("signal: %f, address:%#08x factor: %f, offset:%f \n", *signal->data, signal->data,signal->factor, signal->offset);
        can_setSignal<int16_t>(flex_msg.buf, *signal->data, signal->start_bit, signal->data_length, signal->isIntel, signal->factor, signal->offset);
    }
    can->write(flex_msg);
}

void FS_CAN::txCallBack()
{
    // reset the counter
    if (txCallBackCounter >= 1000)
    {
        txCallBackCounter = 0;
    }
    for (const auto &msg : CAN_TX_1ms)
    {
        // transmit 1ms messages (i hope we don't need these)
        CAN_Tx(msg);
    }
    // debugf("%dms\n",txCallBackCounter);
    if (txCallBackCounter % 10 == 0)
    {
        for (const auto &msg : CAN_TX_10ms)
        {
            CAN_Tx(msg);
        }
    }
    if (txCallBackCounter % 100 == 0)
    {
        for (const auto &msg : CAN_TX_100ms)
        {
            CAN_Tx(msg);
        }
    }
    if (txCallBackCounter % 1000 == 0)
    {
        for (const auto &msg : CAN_TX_1000ms)
        {
            CAN_Tx(msg);
        }
    }
    txCallBackCounter++;
}

uint8_t FS_CAN::reverseByte(uint8_t byte)
{
    byte = (byte & 0xF0) >> 4 | (byte & 0x0F) << 4;
    byte = (byte & 0xCC) >> 2 | (byte & 0x33) << 2;
    byte = (byte & 0xAA) >> 1 | (byte & 0x55) << 1;
    return byte;
}

void FS_CAN::subscribe_to_message(CAN_MSG *msg)
{
    CAN_msg_lookup[msg->id] = msg;
    // debugf("Subcribed to ID: %d with a Signal from %d to %d \n", msg->id);
}
void FS_CAN::publish_CAN_msg(CAN_MSG *msg, CAN_TX_FREQUENCY frequency)
{
    Serial.printf("message with ID %d published at address\n ", msg->id);
    if (frequency == THOUSAND_MS)
    {
        CAN_TX_1000ms.push_back(msg);
    }
    else if (frequency == HUNDRED_MS)
    {
        CAN_TX_100ms.push_back(msg);
    }
    else if (frequency == TEN_MS)
    {
        CAN_TX_10ms.push_back(msg);
    }
    else if (frequency == ONE_MS)
    {
        CAN_TX_1ms.push_back(msg);
    }
}

void FS_CAN::add_signal_to_message(CAN_MSG *msg, CAN_SIGNAL *signal)
{
    //@todo probably wanna use this to check if a message is valid before adding it to message struct
}

void FS_CAN::CAN_RX_ISR(const CAN_message_t &msg)
{
    debug(msg.id);
    debug(" ");
    for (uint8_t i = 0; i < msg.len; i++)
    {
        debug(msg.buf[i]);
        debug(" ");
    }
    debugln();
    if (CAN_msg_lookup.find(msg.id) == CAN_msg_lookup.end())
    {                                     // check if CAN msg was subscribed to
        debugf("unregistered message\n"); // later this will be handled by hardware filter just ignoring
    }
    else
    {
        debugf("message found\n");
        CAN_MSG *CAN_msg = CAN_msg_lookup[msg.id];
        int signalCount = 0;
        for (const auto &signal : CAN_msg->signals)
        {
            debugf("signal#%d\n", signalCount);
            float data = 0;
            data = can_getSignal<int16_t>(msg.buf, signal->start_bit, signal->data_length, signal->isIntel, signal->factor, signal->offset);
            debugf("data: %.2f\n", data);
            *signal->data = data;
            signalCount++;
        }
    }
}
