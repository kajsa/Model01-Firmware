// -*- mode: c++ -*-
// Copyright 2016 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details

#define DEBUG_SERIAL false

#ifndef BUILD_INFORMATION
#define BUILD_INFORMATION "locally built"
#endif


#include "Kaleidoscope-MouseKeys.h"
#include "Kaleidoscope-Macros.h"
#include "Kaleidoscope-LEDControl.h"
//#include "Kaleidoscope-Numlock.h"
#include "Kaleidoscope.h"

#include "LED-Off.h"
#include "Kaleidoscope-LEDEffect-BootGreeting.h"
#include "Kaleidoscope-LEDEffect-SolidColor.h"
#include "Kaleidoscope-LEDEffect-Breathe.h"
#include "Kaleidoscope-LEDEffect-Rainbow.h"
#include "Kaleidoscope-LED-Stalker.h"
#include "Kaleidoscope-LED-AlphaSquare.h"
#include "Kaleidoscope-Model01-TestMode.h"

#define MACRO_VERSION_INFO 1
#define Macro_VersionInfo M(MACRO_VERSION_INFO)
#define MACRO_ANY 2
#define Macro_Any M(MACRO_ANY)
#define NUMPAD_KEYMAP 2

#define GENERIC_FN2  KEYMAP_STACKED ( \
___,     Key_F1,          Key_F2,        Key_F3,        Key_F4,        Key_F5,           Key_LEDEffectNext, \
Key_Tab, Key_PrintScreen, Key_Insert,    Key_mouseUp,   ___,           Key_mouseWarpEnd, Key_mouseWarpNE,   \
Key_PageUp, Key_ScrollLock, Key_mouseL,  Key_mouseDn,   Key_mouseR,    Key_mouseWarpNW,                     \
Key_PageDown, Key_Pause,  Key_mouseBtnL, Key_mouseBtnM, Key_mouseBtnR, Key_mouseWarpSW,  Key_mouseWarpSE,   \
Key_Delete, ___, ___, ___,  \
            ___,   \
\
Consumer_ScanPreviousTrack, Key_F6,                 Key_F7,                   Key_F8,                  Key_F9,          Key_F10,          Key_F11, \
Consumer_PlaySlashPause,    Consumer_ScanNextTrack, Key_LeftCurlyBracket,     Key_RightCurlyBracket,   Key_LeftBracket, Key_RightBracket, Key_F12, \
                            Key_LeftArrow,          Key_DownArrow,            Key_UpArrow,             Key_RightArrow,  ___,              ___, \
Key_PcApplication,          Key_Mute,               Consumer_VolumeDecrement, Consumer_VolumeIncrement, ___,            Key_Backslash,    Key_Pipe, \
___, ___, ___, Key_Enter, \
___ \
)


#define NUMPAD KEYMAP_STACKED  (\
    ___, ___, ___, ___, ___, ___, ___,  \
    ___, ___, ___, ___, ___, ___, ___,  \
    ___, ___, ___, ___, ___, ___,       \
    ___, ___, ___, ___, ___, ___, ___,  \
               ___, ___, ___, ___,  \
                 Key_Keymap1_Momentary, \
\
\
    Macro_VersionInfo,  ___, Key_Keypad7, Key_Keypad8,   Key_Keypad9,        Key_KeypadSubtract, ___, \
    ___, ___, Key_Keypad4, Key_Keypad5,   Key_Keypad6,        Key_KeypadAdd,      ___, \
         ___, Key_Keypad1, Key_Keypad2,   Key_Keypad3,        Key_Equals,         Key_Quote, \
    ___, ___, Key_Keypad0, Key_KeypadDot, Key_KeypadMultiply, Key_KeypadDivide,   Key_Enter, \
    ___, ___, ___, ___, \
    Key_Keymap1_Momentary \
)

#define QWERTY KEYMAP_STACKED ( \
    ___,          Key_1, Key_2, Key_3, Key_4, Key_5, Key_6, \
    Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,           \
    Key_Home,   Key_A, Key_S, Key_D, Key_F, Key_G,                    \
    Key_End, Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,        \
    Key_Backspace, Key_LeftShift, Key_LeftGui, Key_LeftControl,         \
                          Key_Keymap1_Momentary,     \
\
    Macro_Any,     Key_6, Key_7, Key_8,     Key_9,      Key_0,         Key_CapsLock, \
    Key_Enter,     Key_Y, Key_U, Key_I,     Key_O,      Key_P,         Key_LeftBracket,    \
                   Key_H, Key_J, Key_K,     Key_L,      Key_Semicolon, Key_Quote,     \
    Key_RightAlt,  Key_N, Key_M, Key_Comma, Key_Period, Key_Slash,     Key_RightBracket,  \
    Key_RightControl, Key_LeftAlt, Key_RightShift, Key_Spacebar,                      \
    Key_Keymap1_Momentary \
)

const Key keymaps[][ROWS][COLS] PROGMEM = {
  QWERTY,
  GENERIC_FN2,
  NUMPAD
};

static kaleidoscope::LEDSolidColor solidRed(160, 0, 0);
static kaleidoscope::LEDSolidColor solidOrange(140, 70, 0);
static kaleidoscope::LEDSolidColor solidYellow(130, 100, 0);
static kaleidoscope::LEDSolidColor solidGreen(0, 160, 0);
static kaleidoscope::LEDSolidColor solidBlue(0, 70, 130);
static kaleidoscope::LEDSolidColor solidIndigo(0, 0, 170);
static kaleidoscope::LEDSolidColor solidViolet(130, 0, 120);

const macro_t *macroAction(uint8_t macroIndex, uint8_t keyState) {
  if (macroIndex == 1 && keyToggledOn(keyState)) {
    Macros.type(PSTR("Keyboardio Model 01 - Kaleidoscope "));
    Macros.type(PSTR(BUILD_INFORMATION));
  } else if (macroIndex == MACRO_ANY) {
    static Key lastKey;
    if (keyToggledOn(keyState))
      lastKey.keyCode = Key_A.keyCode + (uint8_t)(millis() % 36);

    if (keyIsPressed(keyState))
      kaleidoscope::hid::pressKey(lastKey);
  }
  return MACRO_NONE;
}

void setup() {
  Kaleidoscope.setup(KEYMAP_SIZE);
  BootKeyboard.begin();
  Kaleidoscope.use(
    &BootGreetingEffect,
    &TestMode,
    &LEDControl, &LEDOff,
    &LEDRainbowEffect, &LEDRainbowWaveEffect, 
    &solidRed, &solidOrange, &solidYellow, &solidGreen, &solidBlue, &solidIndigo, &solidViolet,
    &LEDBreatheEffect,
    &AlphaSquareEffect,
    &StalkerEffect,
//    &NumLock,
    &Macros,
    &MouseKeys,
    NULL);

//  NumLock.numPadLayer = NUMPAD_KEYMAP;
  AlphaSquare.color = { 255, 0, 0 };
  LEDRainbowEffect.brightness(150);
  LEDRainbowWaveEffect.brightness(150);
  StalkerEffect.variant = STALKER(BlazingTrail);
  LEDOff.activate();
}


void loop() {
  Kaleidoscope.loop();
}
