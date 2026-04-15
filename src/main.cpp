#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "controller.hpp"
#include "globals.hpp"
#include "cliflags.hpp"

int main(int argc, char** argv)
{
    #if TRACING
    Global::logger.EnableTracing();
    #endif

    Global::wizController = new Wiz::Controller();

    initFlags(argc, argv);

    Global::wizController->SetColor({255, 255, 255});
    return 0;
}
