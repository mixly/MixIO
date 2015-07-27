/*
 * This file is part of Espruino, a JavaScript interpreter for Microcontrollers
 *
 * Copyright (C) 2013 Gordon Williams <gw@pur3.co.uk>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * ----------------------------------------------------------------------------
 * This file is designed to be parsed during the build process
 *
 * JavaScript 'process' object - for information about the Espruino board
 * ----------------------------------------------------------------------------
 */
#include "jsvar.h"
#include "jsparse.h"
#include "jswrap_process.h"
#include "jswrap_interactive.h"
#include "jsinteractive.h"


/*JSON{
  "type" : "class",
  "class" : "process"
}
This class contains information about Espruino itself
*/

/*JSON{
  "type" : "staticproperty",
  "class" : "process",
  "name" : "version",
  "generate_full" : "jsvNewFromString(JS_VERSION)",
  "return" : ["JsVar","The version of Espruino"]
}
Returns the version of Espruino as a String
*/

const void *exportPtrs[] = {
  jsvLock,jsvLockAgainSafe,jsvUnLock,jsvSkipName,jsvMathsOp,jsvMathsOpSkipNames,
  jsvNewFromFloat,jsvNewFromInteger,jsvNewFromString,jsvNewFromBool,
  jsvGetFloat,jsvGetInteger,jsvGetBool,
  jspeiFindInScopes,jspGetVarNamedField,jspReplaceWith,jspeFunctionCall,
  jsvNewWithFlags,
};
const char *exportNames = 
"jsvLock,jsvLockAgainSafe,jsvUnLock,jsvSkipName,jsvMathsOp,jsvMathsOpSkipNames,"
"jsvNewFromFloat,jsvNewFromInteger,jsvNewFromString,jsvNewFromBool,"
"jsvGetFloat,jsvGetInteger,jsvGetBool,"
"jspeiFindInScopes,jspGetVarNamedField,jspReplaceWith,jspeFunctionCall,"
"jsvNewWithFlags,";

/*JSON{
  "type" : "staticproperty",
  "class" : "process",
  "name" : "env",
  "generate" : "jswrap_process_env",
  "return" : ["JsVar","An object"]
}
Returns an Object containing various pre-defined variables. standard ones are BOARD, VERSION
*/
JsVar *jswrap_process_env() {
  JsVar *obj = jsvNewWithFlags(JSV_OBJECT);
/*
  jsvUnLock(jsvObjectSetChild(obj, "VERSION", jsvNewFromString(JS_VERSION)));
  jsvUnLock(jsvObjectSetChild(obj, "BUILD_DATE", jsvNewFromString(__DATE__)));
  jsvUnLock(jsvObjectSetChild(obj, "BUILD_TIME", jsvNewFromString(__TIME__)));
#ifdef GIT_COMMIT
  jsvUnLock(jsvObjectSetChild(obj, "GIT_COMMIT", jsvNewFromString(STRINGIFY(GIT_COMMIT))));
#endif
  jsvUnLock(jsvObjectSetChild(obj, "BOARD", jsvNewFromString(PC_BOARD_ID)));
  jsvUnLock(jsvObjectSetChild(obj, "CHIP", jsvNewFromString(PC_BOARD_CHIP)));
  jsvUnLock(jsvObjectSetChild(obj, "CHIP_FAMILY", jsvNewFromString(PC_BOARD_CHIP_FAMILY)));
  jsvUnLock(jsvObjectSetChild(obj, "FLASH", jsvNewFromInteger(FLASH_TOTAL)));
  jsvUnLock(jsvObjectSetChild(obj, "RAM", jsvNewFromInteger(RAM_TOTAL)));
  jsvUnLock(jsvObjectSetChild(obj, "SERIAL", jswrap_interface_getSerial()));
  jsvUnLock(jsvObjectSetChild(obj, "CONSOLE", jsvNewFromString(jshGetDeviceString(jsiGetConsoleDevice()))));
  */
  JsVar *arr = jsvNewWithFlags(JSV_ARRAY);
  if (arr) {
    jsvArrayPushAndUnLock(arr, jsvNewFromString(exportNames));
    jsvArrayPushAndUnLock(arr, jsvNewFromInteger((JsVarInt)exportPtrs));
    jsvUnLock(jsvObjectSetChild(obj, "EXPORT", arr));
  }  

  return obj;
}


/*JSON{
  "type" : "staticmethod",
  "class" : "process",
  "name" : "memory",
  "generate" : "jswrap_process_memory",
  "return" : ["JsVar","Information about memory usage"]
}
Run a Garbage Collection pass, and return an object containing information on memory usage.

free : Memory that is available to be used

usage : Memory that has been used

total : Total memory

history : Memory used for command history - that is freed if memory is low. Note that this is INCLUDED in the figure for 'free'

stackEndAddress : (on ARM) the address (that can be used with peek/poke/etc) of the END of the stack. The stack grows down, so unless you do a lot of recursion the bytes above this can be used.

Memory units are specified in 'blocks', which are around 16 bytes each (depending on your device). See http://www.espruino.com/Performance for more information.
*/

/*
#ifdef ARM
extern int _end; // end of ram used (variables)
extern int _etext; // end of flash text (binary) section
#endif
*/
JsVar *jswrap_process_memory() {
  jsvGarbageCollect();
  JsVar *obj = jsvNewWithFlags(JSV_OBJECT);
  if (obj) {
    unsigned int history = 0;
    JsVar *historyVar = jsvObjectGetChild(execInfo.hiddenRoot, JSI_HISTORY_NAME, 0);
    if (historyVar) {
      history = (unsigned int)jsvCountJsVarsUsed(historyVar); // vars used to store history
      jsvUnLock(historyVar);
    }
    unsigned int usage = jsvGetMemoryUsage() - history;
    unsigned int total = jsvGetMemoryTotal();
    jsvUnLock(jsvObjectSetChild(obj, "free", jsvNewFromInteger((JsVarInt)(total-usage))));
    jsvUnLock(jsvObjectSetChild(obj, "usage", jsvNewFromInteger((JsVarInt)usage)));
    jsvUnLock(jsvObjectSetChild(obj, "total", jsvNewFromInteger((JsVarInt)total)));
    jsvUnLock(jsvObjectSetChild(obj, "history", jsvNewFromInteger((JsVarInt)history)));

/*
#ifdef ARM
    jsvUnLock(jsvObjectSetChild(obj, "stackEndAddress", jsvNewFromInteger((JsVarInt)(unsigned int)&_end)));
    jsvUnLock(jsvObjectSetChild(obj, "flash_start", jsvNewFromInteger((JsVarInt)FLASH_START)));
    jsvUnLock(jsvObjectSetChild(obj, "flash_binary_end", jsvNewFromInteger((JsVarInt)(unsigned int)&_etext)));
    jsvUnLock(jsvObjectSetChild(obj, "flash_code_start", jsvNewFromInteger((JsVarInt)FLASH_SAVED_CODE_START)));
    jsvUnLock(jsvObjectSetChild(obj, "flash_length", jsvNewFromInteger((JsVarInt)FLASH_TOTAL)));
#endif
*/
  }
  return obj;
}
