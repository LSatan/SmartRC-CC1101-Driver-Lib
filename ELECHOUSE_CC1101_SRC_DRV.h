/*
ELECHOUSE_CC1101_SRC_DRV.h
==============================================================
BACKWARD COMPATIBILITY WRAPPER

This header exists only so that old Arduino scripts
that use #include <ELECHOUSE_CC1101_SRC_DRV.h>
do not fail.

It automatically directs the compiler to the new SmartRC file.
Users do not have to touch their old code!
==============================================================
*/

#ifndef ELECHOUSE_CC1101_SRC_DRV_h
#define ELECHOUSE_CC1101_SRC_DRV_h

// Redirect to the new, tidy header
#include "SmartRC_CC1101.h"

#endif