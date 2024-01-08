#include "interfaces/bms.h"

extern State state;

BMS::BMS()
{
    FS_CAN0.subscribe_to_message(&packStateMessage);
    // TODO: Uncomment this when the BMS is actually sending this message
    // FS_CAN0.subscribe_to_message(&packInfoMessage);
    FS_CAN0.subscribe_to_message(&packFaultsMessage);
}

BMS::BMS_RELAY_STATES BMS::getRelayState()
{
    BMS::BMS_RELAY_STATES relayStates;

    // Extract individual states from the float using bitwise operations
    uint32_t relayStateBits = 0;
    std::memcpy(&relayStateBits, &packRelayState, sizeof(float));

    relayStates.DISCHARGE_RELAY_ENABLED = (relayStateBits & 0x01) >> 0;
    relayStates.CHARGE_RELAY_ENABLED = (relayStateBits & 0x02) >> 1;
    relayStates.CHARGER_SAFETY_ENABLED = (relayStateBits & 0x04) >> 2;
    relayStates.MALFUNCTION_INDICATOR_ACTIVE = (relayStateBits & 0x08) >> 3;
    relayStates.MULTI_PURPOSE_INPUT_SIGNAL_STATUS = (relayStateBits & 0x10) >> 4;
    relayStates.ALWAYS_ON_SIGNAL_STATUS = (relayStateBits & 0x20) >> 5;
    relayStates.IS_READY_INPUT_STATUS = (relayStateBits & 0x40) >> 6;
    relayStates.IS_CHARGING_INPUT_STATUS = (relayStateBits & 0x80) >> 7;
    relayStates.CHARGE_MODE_STATUS = (relayStateBits & 0x400) >> 10;

    return relayStates;
}

BMS::BMS_FAULTS BMS::getFaults()
{
    BMS::BMS_FAULTS faults;

    uint32_t faultBits1 = 0;
    std::memcpy(&faultBits1, &packFaults1, sizeof(float));

    uint32_t faultBits2 = 0;
    std::memcpy(&faultBits2, &packFaults2, sizeof(float));

    faults.P0A07 = (faultBits1 & 0x0001) >> 0;
    faults.P0A08 = (faultBits1 & 0x0002) >> 1;
    faults.P0A09 = (faultBits1 & 0x0004) >> 2;
    faults.P0A0A = (faultBits1 & 0x0008) >> 3;
    faults.P0A0B = (faultBits1 & 0x0010) >> 4;
    faults.P0A0C = (faultBits1 & 0x0020) >> 5;
    faults.P0A0E = (faultBits1 & 0x0040) >> 6;
    faults.P0A10 = (faultBits1 & 0x0080) >> 7;
    faults.P0A95 = (faultBits1 & 0x0100) >> 8;
    faults.P0AA1 = (faultBits1 & 0x0200) >> 9;
    faults.P0A11 = (faultBits1 & 0x0400) >> 10;

    faults.P0A1F = (faultBits2 & 0x0001) >> 0;
    faults.P0A12 = (faultBits2 & 0x0002) >> 1;
    faults.P0A80 = (faultBits2 & 0x0004) >> 2;
    faults.P0AFA = (faultBits2 & 0x0008) >> 3;
    faults.P0A04 = (faultBits2 & 0x0010) >> 4;
    faults.P0AC0 = (faultBits2 & 0x0020) >> 5;
    faults.P0A0D = (faultBits2 & 0x0040) >> 6;
    faults.P0A0F = (faultBits2 & 0x0080) >> 7;
    faults.P0A02 = (faultBits2 & 0x0100) >> 8;
    faults.P0A81 = (faultBits2 & 0x0200) >> 9;
    faults.P0A9C = (faultBits2 & 0x0400) >> 10;
    faults.U0100 = (faultBits2 & 0x0800) >> 11;
    faults.P0560 = (faultBits2 & 0x1000) >> 12;
    faults.P0AA6 = (faultBits2 & 0x2000) >> 13;
    faults.P0A05 = (faultBits2 & 0x4000) >> 14;
    faults.P0A06 = (faultBits2 & 0x8000) >> 15;

    return faults;
}