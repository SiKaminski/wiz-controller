#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "wiz.hpp"
#include "globals.hpp"
#include "skutils/skutilflagparser.hpp"

using namespace SKUTIL;

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

SK_VEC<Flag> gFlags {
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

void initFlags(int argc, char** argv)
{
    FlagParser parser(&gFlags);
    parser.ParseFlags(argc, argv);
}

int main(int argc, char** argv)
{
    #if TRACING
    Global::logger.EnableTracing();
    #endif

    Global::wizController = new Wiz::Controller();

    initFlags(argc, argv);


    // while (1) {
    //     for (int i = 0; i < 255; i+=10) {
    //         bulbs[0].SetRGB(i, 0, 0);
    //     }

    //     for (int i = 0; i < 255; i+=10) {
    //         bulbs[0].SetRGB(0, i, 0);
    //     }

    //     for (int i = 0; i < 255; i+=10) {
    //         bulbs[0].SetRGB(0, 0, i);
    //     }
    // }

    return 0;
}
