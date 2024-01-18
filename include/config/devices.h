#pragma once
#include <ADC.h>
#include "sensor.h"
#include "interfaces/pedal.h"
#include "interfaces/bms.h"
#include "interfaces/inverter.h"


class Devices
{
public:
    static Devices& Get() // Edit this needs to be static, my bad
    {
        Serial.println("get devices called");
        static Devices d;
        return d;
    }
public:
    FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16>& GetCan0()
    {
        return Can0;
    }
    FS_CAN& GetFS_Can0()
    {
        return FS_CAN0;
    }

    Inverter& GetInverter()
    {
        return inverter;
    }

    BMS& GetBMS(){
        return bms;
    }
    std::array <Sensor*, 2>& GetSensors(){
        return sensors;
    }
    Devices(const Devices&) = delete;
    Devices& operator=(const Devices&) = delete;

    Devices(Devices&&) = delete;
    Devices& operator=(Devices&&) = delete;


private:
    FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> Can0;

    FS_CAN FS_CAN0;
    Inverter inverter;
    BMS bms;
    ADC adc;
    Sensor sense1;
    Sensor sense2;
    Pedal pedal;
    std::array <Sensor*, 2>sensors = {&sense1, &sense2};
private:
    Devices()
        : Can0(), FS_CAN0(&Can0), inverter(), bms(), adc(), sense1(&adc, A0, 0, 1, 2, 2, 1e3, true),
        sense2(&adc, A16, 0, 1, 2, 2, 1e3, true), pedal(&sense1, &sense2)
    {
        Serial.println("help, i got past innit");
    }

};