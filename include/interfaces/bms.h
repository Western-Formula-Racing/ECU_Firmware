#ifndef _BMS_
#define _BMS_

// Includes

#include "required_includes.h"
#include "state_machine.h"

extern FS_CAN FS_CAN0;

class BMS
{
private:
    // PREFACE: The BMS uses Moterola byte ordering, which is big endian. This is the opposite of what the inverter uses.
    FS_CAN::CAN_SIGNAL packCurrentSignal{&packCurrent, 8, 16, false, 0.1f, 0};
    FS_CAN::CAN_SIGNAL packInstantaneousVoltageSignal{&packVoltage, 24, 16, false, 0.1f, 0};
    FS_CAN::CAN_SIGNAL packSOCSignal{&packSOC, 32, 8, false, 0.5f, 0};
    FS_CAN::CAN_SIGNAL packRelayStateSignal{&packRelayState, 48, 16, false, 1.0f, 0};
    FS_CAN::CAN_MSG packStateMessage{1712, {&packCurrentSignal, &packInstantaneousVoltageSignal, &packSOCSignal, &packRelayStateSignal}};

    FS_CAN::CAN_MSG packInfoMessage{1713, {}};

    FS_CAN::CAN_SIGNAL packFlags1Signal{&packFaults1, 8, 16, false, 1.0f, 0};
    FS_CAN::CAN_SIGNAL packFlags2Signal{&packFaults2, 24, 16, false, 1.0f, 0};
    FS_CAN::CAN_MSG packFaultsMessage{1714, {&packFlags1Signal, &packFlags2Signal}};

public:
    // BMS Status Variables:
    float packCurrent;    // amps
    float packVoltage;    // volts
    float packSOC;        // percent
    float packRelayState; // bitfield

    // BMS Fault Variables:
    float packFaults1; // bitfield
    float packFaults2; // bitfield

    struct BMS_RELAY_STATES
    {
        bool DISCHARGE_RELAY_ENABLED;           // 0x01
        bool CHARGE_RELAY_ENABLED;              // 0x02
        bool CHARGER_SAFETY_ENABLED;            // 0x04
        bool MALFUNCTION_INDICATOR_ACTIVE;      // 0x08
        bool MULTI_PURPOSE_INPUT_SIGNAL_STATUS; // 0x10
        bool ALWAYS_ON_SIGNAL_STATUS;           // 0x20
        bool IS_READY_INPUT_STATUS;             // 0x40
        bool IS_CHARGING_INPUT_STATUS;          // 0x80
        bool CHARGE_MODE_STATUS;                // 0x400
    };

    struct BMS_FAULTS
    {
        // DTC Status #1
        bool P0A07; // Discharge Limit Enforcement Fault (0x0001)
        bool P0A08; // Charger Safety Relay Fault (0x0002)
        bool P0A09; // Internal Hardware Fault (0x0004)
        bool P0A0A; // Internal Heatsink Thermistor Fault (0x0008)
        bool P0A0B; // Internal Software Fault (0x0010)
        bool P0A0C; // Highest Cell Voltage Too High Fault (0x0020)
        bool P0A0E; // Lowest Cell Voltage Too Low Fault (0x0040)
        bool P0A10; // Pack Too Hot Fault (0x0080)
        bool P0A95; // High Voltage Interlock Signal Fault (0x0100)
        bool P0AA1; // Pre-charge Circuit Malfunction (0x0200)
        bool P0A11; // Abnormal State of Charge Behavior Fault (0x0400)

        // DTC Status #2
        bool P0A1F; // Internal Communication Fault (0x0001)
        bool P0A12; // Cell Balancing Stuck Off Fault (0x0002)
        bool P0A80; // Weak Cell Fault (0x0004)
        bool P0AFA; // Low Cell Voltage Fault (0x0008)
        bool P0A04; // Open Wiring Fault (0x0010)
        bool P0AC0; // Current Sensor Fault (0x0020)
        bool P0A0D; // Highest Cell Voltage Over 5V Fault (0x0040)
        bool P0A0F; // Cell ASIC Fault (0x0080)
        bool P0A02; // Weak Pack Fault (0x0100)
        bool P0A81; // Fan Monitor Fault (0x0200)
        bool P0A9C; // Thermistor Fault (0x0400)
        bool U0100; // External Communication Fault (0x0800)
        bool P0560; // Redundant Power Supply Fault (0x1000)
        bool P0AA6; // High Voltage Isolation Fault (0x2000)
        bool P0A05; // Input Power Supply Fault (0x4000)
        bool P0A06; // Charge Limit Enforcement Fault (0x8000)
    };

    // public function declarations
    BMS();
    BMS_RELAY_STATES getRelayState();
    BMS_FAULTS getFaults();
};
#endif