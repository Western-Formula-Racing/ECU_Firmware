#ifndef _FS_CAN_
#define _FS_CAN_

#include <FlexCAN_T4.h>
#include "arduino_freertos.h"
#include "avr/pgmspace.h"
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <iostream>
#include "can_helpers.hpp"
#include <format>
#include "black_box.h"
class FS_CAN
{
public:
    enum CAN_TX_FREQUENCY
    {
        ONE_MS = 1,
        TEN_MS = 10,
        HUNDRED_MS = 100,
        THOUSAND_MS = 1000
    };
    
    struct CAN_SIGNAL
    {
        float* data;
        uint8_t start_bit;
        uint8_t data_length;
        bool isIntel;
        float factor;
        float offset;
    };

    struct CAN_MSG
    {
        u_int16_t id;
        std::vector<CAN_SIGNAL *> signals;
    };
    uint16_t txCallBackCounter;
    std::vector<CAN_MSG *> CAN_TX_1000ms;
    std::vector<CAN_MSG *> CAN_TX_100ms;
    std::vector<CAN_MSG *> CAN_TX_10ms;
    std::vector<CAN_MSG *> CAN_TX_1ms;
    FS_CAN(FlexCAN_T4_Base *canHandle);
    void subscribe_to_message(CAN_MSG *msg); // need to figure out the parameters here
    void publish_CAN_msg(CAN_MSG *msg, CAN_TX_FREQUENCY frequency);
    void add_signal_to_message(CAN_MSG *msg, CAN_SIGNAL *signal);
    void CAN_RX_ISR(const CAN_message_t &msg);
    uint8_t reverseByte(uint8_t byte);
    void printBits(size_t const size, void const *const ptr);
    uint64_t extractCANSignal(const uint8_t *data, int startBit, int endBit, bool isLittleEndian, bool isLeastSignificantBitFirst);

private:
    FlexCAN_T4_Base *can;
    void CAN_Tx(CAN_MSG *msg);
    uint32_t getSignal(uint8_t *buffer, uint8_t startBit, uint8_t length);
    void txCallBack();
    std::unordered_map<uint16_t, CAN_MSG *> CAN_msg_lookup; //(id, msg)
};

#endif