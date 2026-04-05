#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "wiz.hpp"
#include "globals.hpp"
#include "skutils/skutilflagparser.hpp"

using namespace SKUTIL;

void InitWizControllerPrefixed(OPT int inputCount, OPT char** inputVals)
{
    Global::controller = new Wiz::Controller(inputVals[0]);
}

SK_VEC<Flag> gFlags {
    {
        'p',
        "prefix",
        "Device prefix to scan for",
        2,
        InitWizControllerPrefixed,
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

    initFlags(argc, argv);

    if (Global::controller == nullptr) {
        Global::controller = new Wiz::Controller();
    }

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
