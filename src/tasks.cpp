#include "tasks.h"
#include "config/devices.h"
#include "mpu6x00.h"
extern State state;
extern std::array<state_function_t, 8> states;
float stateS;
float rtdButton;
extern FS_CAN dataCAN;
extern FS_CAN controlCAN;
float amsOK = 0;
float imdOK = 0;
float precharge_enable = 0;
float precharge_ok = 0;
static Mpu6500 imu = Mpu6500(37);//cs pin for mpu
float accelX;
float accelY;
float accelZ;
float gyroX;
float gyroY;
float gyroZ;
float rearHeave;
float rearRoll;

FS_CAN::CAN_SIGNAL precharge_enable_signal{&precharge_enable, 0, 1, true, 1, 0};
FS_CAN::CAN_SIGNAL precharge_ok_signal{&precharge_ok, 1, 1, true, 1, 0};
FS_CAN::CAN_MSG VCU_Precharge{2003, {&precharge_enable_signal, &precharge_ok_signal}};
FS_CAN::CAN_SIGNAL amsOKSignal{&amsOK, 40, 8, true, 1, 0};
FS_CAN::CAN_SIGNAL imdOKSignal{&imdOK, 48, 8, true, 1, 0};
FS_CAN::CAN_MSG AccMB_Info{2010, {&amsOKSignal, &imdOKSignal}};


 
#ifndef REAR
FS_CAN::CAN_SIGNAL stateSignal{&stateS, 8, 8, true, 1, 0};
FS_CAN::CAN_SIGNAL rtdButtonSignal{&rtdButton, 0, 1, true, 1, 0};
FS_CAN::CAN_MSG VCU_StateInfo{2002, {&stateSignal, &rtdButtonSignal}};
FS_CAN::CAN_SIGNAL rearHeaveSignal{&rearHeave, 0, 16, true, 1.0f, 0};
FS_CAN::CAN_SIGNAL rearRollSignal{&rearRoll, 0, 16, true, 1.0f, 0};
#endif

#ifdef REAR
    float HSDEnable[8];
    FS_CAN::CAN_SIGNAL HSD1_Enable_Signal{&HSDEnable[0], 0, 8, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL HSD2_Enable_Signal{&HSDEnable[1], 8, 8, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL HSD3_Enable_Signal{&HSDEnable[2], 16, 8, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL HSD4_Enable_Signal{&HSDEnable[3], 24, 8, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL HSD5_Enable_Signal{&HSDEnable[4], 32, 8, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL HSD6_Enable_Signal{&HSDEnable[5], 40, 8, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL HSD7_Enable_Signal{&HSDEnable[6], 48, 8, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL HSD8_Enable_Signal{&HSDEnable[7], 56, 8, true, 1.0f, 0};
    FS_CAN::CAN_MSG VCU_rearECU_command{2012, 
    {
    &HSD1_Enable_Signal,
    &HSD2_Enable_Signal,
    &HSD3_Enable_Signal,
    &HSD4_Enable_Signal,
    &HSD5_Enable_Signal,
    &HSD6_Enable_Signal,
    &HSD7_Enable_Signal,
    &HSD8_Enable_Signal,
    }};
    FS_CAN::CAN_SIGNAL rearHeaveSignal{&Devices::Get().GetSensors()[5]->value, 0, 16, true, 1.0f, 0};
    FS_CAN::CAN_SIGNAL rearRollSignal{&Devices::Get().GetSensors()[6]->value, 0, 16, true, 1.0f, 0};
    
#endif
FS_CAN::CAN_MSG VCU_rearLinPots{2013,{&rearHeaveSignal, &rearRollSignal}};


void setup_task(void *)
{
    BlackBox::begin(300, tskIDLE_PRIORITY + 1);

    // Delay to allow the serial port to be opened and queue to be created (for some reason it needs this)
    vTaskDelay(pdMS_TO_TICKS(5000));

    if (timeStatus() != timeSet)
    {
        while (!Serial)
            ;
        Serial.printf("my god this sh borked\n");
    }
    SPI.begin();
    if(!imu.begin()){
        Serial.printf("imu failed to innit");
    }

    xTaskCreate(CLI_Tool::task, "CLI_Task", 2048, nullptr, tskIDLE_PRIORITY + 1, nullptr);

#ifndef REAR
    controlCAN.publish_CAN_msg(&VCU_StateInfo, FS_CAN::HUNDRED_MS);
    controlCAN.publish_CAN_msg(&VCU_Precharge, FS_CAN::HUNDRED_MS);
    controlCAN.subscribe_to_message(&AccMB_Info);
    controlCAN.subscribe_to_message(&VCU_rearLinPots);
    xTaskCreate(frontDAQ, "frontDAQ", 5028, nullptr, tskIDLE_PRIORITY + 1, nullptr);
    xTaskCreate(VCU_stateMachine, "VCU_stateMachine", 1028, nullptr, tskIDLE_PRIORITY + 2, nullptr);
    Devices::Get().GetPDM().setPin(HSDIN3, 1);

#endif
#ifdef REAR
    controlCAN.subscribe_to_message(&VCU_Precharge);
    controlCAN.subscribe_to_message(&VCU_rearECU_command);
    controlCAN.publish_CAN_msg(&AccMB_Info, FS_CAN::HUNDRED_MS);
    controlCAN.subscribe_to_message(&VCU_rearLinPots);
    xTaskCreate(rearECU_task, "rearECU_task", 5028, nullptr, tskIDLE_PRIORITY + 2, nullptr);
#endif
    vTaskDelete(nullptr);
}

void frontDAQ(void *)
{
    while (true)
    {
        digitalWriteFast(LED_BUILTIN, HIGH);
        // BlackBox::log(LOG_INFO, std::format("DCBus: {:.1f} TorqueCmd: {:.1f}, requested torque: {:.1f}", inverter.dcBusVoltage, inverter.commandedTorque, inverter.getTorqueRequest()).c_str());
        // BlackBox::log(LOG_INFO, std::format("inverter state: {}, run mode {:.1f} enable state {:.1f}", static_cast<int> (inverter.getInverterState()), inverter.runMode, inverter.enableState).c_str());
        int i = 0;
        for (auto *sensor : Devices::Get().GetSensors())
        {
            sensor->read();
            Serial.printf(">sensor%draw:", i);
            Serial.print(sensor->voltage);
            Serial.println();
            i++;
        }
        Devices::Get().GetRTDButton().read();
        float pedalPos = Devices::Get().GetPedal().getPedalPosition();
        imu.readSensor();
        accelX = imu.getAccelX();
        accelY = imu.getAccelY();
        accelZ = imu.getAccelZ();
        gyroX = imu.getGyroX() - 341;
        gyroY = imu.getGyroY() - 79.5;
        gyroZ = imu.getGyroZ() + 204.5;


        Serial.printf(">pedal_postion:%f\n", pedalPos);
        Serial.printf(">sensor1:%f\n>sensor2:%f\n", Devices::Get().GetPedal().appsSensor1Position, Devices::Get().GetPedal().appsSensor2Position);
        Serial.printf(">RTDButton:%f\n", Devices::Get().GetRTDButton().value);
        rtdButton = static_cast<float>(Devices::Get().GetRTDButton().value);
        Serial.printf(">brake:%f\n", Devices::Get().GetPedal().getFrontBreakPressure());
        Serial.printf(">state:%d\n", static_cast<int>(state));
        Serial.printf(">torqueRequest:%f\n", Devices::Get().GetInverter().getTorqueRequest());
        Serial.printf(">MotorSpeed:%f\n", Devices::Get().GetInverter().motorSpeed);
        Serial.printf(">TorqueLimit:%f\n", Devices::Get().GetInverter().torqueLimit);
        Serial.printf(">packVoltage:%f\n", Devices::Get().GetBMS().packVoltage);
        Serial.printf(">inverterVoltage:%f\n", Devices::Get().GetInverter().dcBusVoltage);
        Serial.printf(">precharge thresh:%f\n", Devices::Get().GetBMS().packVoltage * 0.90);
        Serial.printf(">inv_commandedTorque:%f\n", Devices::Get().GetInverter().commandedTorque);
        Serial.printf(">INV_DC_Bus_Current:%f\n", Devices::Get().GetInverter().INV_DC_Bus_Current);
        Serial.printf(">INV_Coolant_Temp:%f\n", Devices::Get().GetInverter().INV_Coolant_Temp);
        Serial.printf(">INV_glv_voltage:%f\n", Devices::Get().GetInverter().INV_glv_voltage);
        // Serial.printf(">gyroX:%f\n",gyroX);
        // Serial.printf(">gyroY:%f\n",gyroY);
        // Serial.printf(">gyroZ:%f\n",gyroZ);
        // Serial.printf(">accelX:%f\n",accelX);
        // Serial.printf(">accelY:%f\n",accelY);
        // Serial.printf(">accelZ:%f\n",accelZ);


        BlackBox::logSensor("state",static_cast<int>(state));
        BlackBox::logSensor("torqueRequest",Devices::Get().GetInverter().getTorqueRequest());
        BlackBox::logSensor("inv_torque_request", Devices::Get().GetInverter().commandedTorque);
        BlackBox::logSensor("inv_torque feedback", Devices::Get().GetInverter().torqueFeedback);
        BlackBox::logSensor("pedal_position",pedalPos);
        BlackBox::logSensor("apps1",Devices::Get().GetPedal().appsSensor1Position);
        BlackBox::logSensor("apps2",Devices::Get().GetPedal().appsSensor2Position);
        BlackBox::logSensor("brake_avg", Devices::Get().GetPedal().avgbrakePressure);
        BlackBox::logSensor("packVoltage", Devices::Get().GetBMS().packVoltage);   
        BlackBox::logSensor("iq", Devices::Get().GetInverter().iq);
        BlackBox::logSensor("id", Devices::Get().GetInverter().id);
        BlackBox::logSensor("INV_DC_Bus_Current", Devices::Get().GetInverter().INV_DC_Bus_Current);
        BlackBox::logSensor("INV_Coolant_Temp", Devices::Get().GetInverter().INV_Coolant_Temp);
        // BlackBox::logSensor("ThermModule1_lowTemp", Devices::Get().GetBMS().lowTemp1);
        BlackBox::logSensor("ThermModule1_highTemp", Devices::Get().GetBMS().highTemp1);
        BlackBox::logSensor("motorSpeed", Devices::Get().GetInverter().motorSpeed);
        BlackBox::logSensor("INV_glv_voltage",  Devices::Get().GetInverter().INV_glv_voltage);
        // BlackBox::logSensor("INV_BMS_Active",  Devices::Get().GetInverter().INV_BMS_Active);
        BlackBox::logSensor("INV_BMS_Torque_Limiting",  Devices::Get().GetInverter().INV_BMS_Torque_Limiting);
        BlackBox::logSensor("INV_Limit_Max_Speed",  Devices::Get().GetInverter().INV_Limit_Max_Speed);
        BlackBox::logSensor("INV_Limit_Coolant_Derating",  Devices::Get().GetInverter().INV_Limit_Coolant_Derating);
        BlackBox::logSensor("accelX", accelX);
        BlackBox::logSensor("accelY", accelY);
        BlackBox::logSensor("accelZ", accelZ);
        // BlackBox::logSensor("gyroX", gyroX);
        // BlackBox::logSensor("gyroY", gyroY);
        // BlackBox::logSensor("gyroZ", gyroZ);
        BlackBox::logSensor("imdOK", imdOK);
        BlackBox::logSensor("amsOK", amsOK);
        // BlackBox::logSensor("rearHeave", rearHeave);
        // BlackBox::logSensor("rearRoll", rearRoll);
        // BlackBox::logSensor("frontHeave", Devices::Get().GetSensors()[5]->value);
        // BlackBox::logSensor("frontRoll", Devices::Get().GetSensors()[5]->value);
        BlackBox::logSensor("INV_Torque_Capability", Devices::Get().GetInverter().INV_torque_capability);
        BlackBox::logSensor("INV_Internal_State_full", Devices::Get().GetInverter().INV_Internal_State_full);
        BlackBox::logSensor("BMS_DCL", Devices::Get().GetBMS().dcl);
        BlackBox::logSensor("BMS_CCL", Devices::Get().GetBMS().ccl);
        vTaskDelay(pdMS_TO_TICKS(100));
        digitalWriteFast(LED_BUILTIN, LOW);
    }
}
#ifdef REAR
void rearECU_task(void *)
{
    while (1)
    {
        int i = 0;
        for (auto *sensor : Devices::Get().GetSensors())
        {
            sensor->read();
            i++;
        }
        Devices::Get().GetPDM().setPin(HSDIN2, precharge_enable);
        Devices::Get().GetPDM().setPin(HSDIN5, precharge_ok);
        Devices::Get().GetPDM().setPin(HSDIN1, HSDEnable[0]);
        Devices::Get().GetPDM().setPin(HSDIN4, HSDEnable[3]);
        Devices::Get().GetPDM().setPin(HSDIN3, HSDEnable[4]);
        Devices::Get().GetPDM().setPin(HSDIN6, HSDEnable[5]);
        Devices::Get().GetPDM().setPin(HSDIN7, HSDEnable[6]);
        Devices::Get().GetPDM().setPin(HSDIN8, HSDEnable[7]);
        imdOK = digitalRead(A0);
        amsOK = digitalRead(A4);
        Serial.printf(">imdOK:%f\n", imdOK);
        Serial.printf(">amsOK:%f\n", amsOK);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
#endif
void VCU_stateMachine(void *)
{
    state = START;
    while (true)
    {
        stateS = static_cast<float>(state);
        state = states[state]();
        Devices::Get().GetPDM().setPin(HSDIN7, !amsOK); // first light 
        Devices::Get().GetPDM().setPin(HSDIN5, !imdOK); // second light
        if (state == DRIVE){
            Devices::Get().GetPDM().setPin(HSDIN6, 1); // RTD light 
        }
        else{
            Devices::Get().GetPDM().setPin(HSDIN6, 0);
        }
         if (Devices::Get().GetInverter().dcBusVoltage >= 60){
            Devices::Get().GetPDM().setPin(HSDIN8, 1); // HVP light 
        }
        else{
            Devices::Get().GetPDM().setPin(HSDIN8, 0);
        }


        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
