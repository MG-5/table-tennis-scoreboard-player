#pragma once

#include <avr/interrupt.h>
#include <avr/io.h>

#include "settings.h"

#include "common/A1001Display.h"
#include "common/timer0.h"
#include "common/uart.h"
#include "common/utils.h"

bool checkNewInfos();
