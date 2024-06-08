#ifndef _DEVICES_
#define _DEVICES_
#include <ADC.h>
#include "CAN/FS_CAN.h"
#include "sensor.h"
#include "interfaces/pedal.h"
#include "interfaces/bms.h"
#include "interfaces/inverter.h"
#include "interfaces/pdm.h"
#include "digitalSensor.h"
#include "interfaces/rearECU.h"
//this is kinda jank but it's the end of the year:
#ifndef REAR
    #define rollPin  26
    #define heavePin  22
#endif
#ifdef REAR
    #define rollPin  25
    #define heavePin 21
#endif

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
    std::array<Sensor *, 7> &GetSensors()
    {
        return sensors;
    }

    DigitalSensor &GetRTDButton()
    {
        return rtdButton;
    }
    rearECU &GetRearECU()
    {
        return rearEcu;
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
    Sensor sense3;
    Sensor sense4;
    Sensor sense5;//steering angle
    Sensor sense6;//front heave
    Sensor sense7;//front roll
    DigitalSensor rtdButton;
    Pedal pedal;
    std::array<Sensor *, 7> sensors;
    PDM pdm;
    rearECU rearEcu;
private:
    Devices()
        : inverter(), bms(), adc(), 
        sense1(&adc, A9, 0, 1, 2, 2, 1e3, true),
        sense2(&adc, A4, 0, 1, 2, 2, 1e3, true), 
        sense3(&adc, A7, 0, 1, 2, 2, 1e3, true), 
        sense4(&adc, A10, 0, 1, 2, 2, 1e3, true),
        sense5(&adc, A3, 0, 1, 2, 2, 1e3, true),
        sense6(&adc, heavePin, 0, 1, 2, 100, 1e3, true),
        sense7(&adc, rollPin, 0, 1, 2, 100, 1e3, true),
        rtdButton(&adc,36), pedal(&sense1, &sense2, &sense3, &sense4), sensors({&sense1, &sense2, &sense3, &sense4, &sense5, &sense6, &sense7}),
        pdm(), rearEcu()
    {
        Serial.println("Devices:Devices() constructor called");
    }
};

#endif
