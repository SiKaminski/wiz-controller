#include "cliflags.hpp"

#include "globals.hpp"

using namespace SKUTIL;

SK_VEC<Flag> flags {
    {
        'p',
        "prefix",
        "Device prefix to scan for",
        1,
        InitWizControllerPrefixed,
    },
    {
        'i',
        "home-id",
        "Home id for the lights, if provided the controller will contain only lights from that home id",
        1,
        FilterDevicesWithHomeID,
    },
    {
        'r',
        "room-id",
        "Room id for the lights, if provided the controller will contain only lights from that room id",
        1,
        FilterDevicesWithRoomID,
    },
    {
        'c',
        "confirm-bulbs",
        "Gives the choice to confirm which scanned bulbs to communicate with",
        0,
        ConfirmBulbSelection,
    },
};


void InitWizControllerPrefixed(OPT int inputCount, OPT char** inputVals)
{
    Global::wizController->FilterDevicesByModulePrefix(inputVals[0]);
}

void FilterDevicesWithHomeID(OPT int inputCount, OPT char** inputVals)
{
    Global::wizController->FilterDevicesByHomeID(inputVals[0]);
}

void FilterDevicesWithRoomID(OPT int inputCount, OPT char** inputVals)
{
    Global::wizController->FilterDevicesByRoomID(inputVals[0]);
}

void ConfirmBulbSelection(OPT int inputCount, OPT char** inputVals)
{
    Global::wizController->ConfirmBulbChoices();
}

void InitFlags(int argc, char** argv)
{
    FlagParser parser(&flags);
    parser.ParseFlags(argc, argv);
}
