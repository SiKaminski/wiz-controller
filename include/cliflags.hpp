#pragma once
#ifndef _CLIFLAGS_HPP_
#define _CLIFLAGS_HPP_

#include "skutils/skutilflagparser.hpp"

void InitWizControllerPrefixed(OPT int inputCount, OPT char** inputVals);
void FilterDevicesWithHomeID(OPT int inputCount, OPT char** inputVals);
void FilterDevicesWithRoomID(OPT int inputCount, OPT char** inputVals);
void ConfirmBulbSelection(OPT int inputCount, OPT char** inputVals);
void InitFlags(int argc, char** argv);

#endif // _CLIFLAGS_HPP_
