#ifndef _DEVICES_
#define _DEVICES_
#include <ADC.h>
#include "CAN/FS_CAN.h"
#include "sensor.h"
#include "interfaces/pedal.h"
#include "interfaces/bms.h"
#include "interfaces/inverter.h"
#include "interfaces/pdm.h"

class Devices
{
public:
    static Devices &Get()
    {
        static Devices d;
        return d;
    }

public:
    Inverter &GetInverter()
    {
        return inverter;
    }

    BMS &GetBMS()
    {
        return bms;
    }

    Pedal &GetPedal()
    {
        return pedal;
    }
    PDM &GetPDM(){
        return pdm;
    }
    std::array<Sensor *, 2> &GetSensors()
    {
        return sensors;
    }

    Devices(const Devices &) = delete;
    Devices &operator=(const Devices &) = delete;

    Devices(Devices &&) = delete;
    Devices &operator=(Devices &&) = delete;

private:
    static Devices *s_instance;
    Inverter inverter;
    BMS bms;
    ADC adc;
    Sensor sense1;
    Sensor sense2;
    Pedal pedal;
    PDM pdm;
    std::array<Sensor *, 2> sensors;

private:
    Devices()
        : inverter(), bms(), adc(), sense1(&adc, A0, 0, 1, 2, 2, 1e3, true),
          sense2(&adc, A16, 0, 1, 2, 2, 1e3, true), pedal(&sense1, &sense2), sensors({&sense1, &sense2})
    {
        Serial.println("Devices:Devices() constructor called");
    }
};

#endif
