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
 * Utilities and definitions for handling Pins
 * ----------------------------------------------------------------------------
 */

#include "jspin.h"
#include "jshardware.h"
#include "jsinteractive.h"
#include "jspininfo.h"
bool jshIsPinValid(Pin pin) {
	return pin != NC;
}


Pin jshGetPinFromString(const char *s){

	if(isStrInt(s))  return (Pin)stringToInt(s);

	int searchMin = 0;
	int searchMax = PINNUM - 1;

	while(searchMin <= searchMax){
		int idx = (searchMin + searchMax) >> 1;
		int cmp = strcmp(s,pinsInfo[idx].pinName);
		if(cmp == 0) return pinsInfo[idx].pin;
		else if(cmp < 0) searchMax = idx - 1;
		else searchMin = idx + 1;
	}
	return NC;
}
/*  //nucleo411
Pin jshGetPinFromString(const char *s) {
	if(!s || !s[0])   return NC;
	if(isStrInt(s))  return (Pin)stringToInt(s);

	if(s[0] == 'A'){
		if(!s[2]){
			switch(s[1]){
			case '0' : return A0;
			case '1' : return A1;
			case '2' : return A2;
			case '3' : return A3;
			case '4' : return A4;
			case '5' : return A5;
			default: return NC;

			}
		}
	}else if(s[0] == 'D'){
		if(s[1] && !s[2]){
			switch(s[1]){
			case '0' : return D0;
			case '1' : return D1;
			case '2' : return D2;
			case '3' : return D3;
			case '4' : return D4;
			case '5' : return D5;
			case '6' : return D6;
			case '7' : return D7;
			case '8' : return D8;
			case '9' : return D9;

			default:return NC;
			}
		}else if(s[1] && s[2] && !s[3]){
			if(s[1] == '1'){
				switch(s[2]){
				case '0' : return D10;
				case '1' : return D11;
				case '2' : return D12;
				case '3' : return D13;
				case '4' : return D14;
				case '5' : return D15;
				default:return NC;
				}

			}
		}
	}
	if(s[0] == 'L' && s[1] == 'E' && s[2] == 'D' && s[3] == '1' && !s[4])  return LED1;
	if(s[0] == 'U' && s[1] == 'S' && s[2] == 'B' && s[3] && s[4] == 'X' && !s[5]){
		if(s[3] == 'T')  return USBTX;
		else if(s[3] == 'R')  return USBRX;
	}
	if(s[0] == 'S' && s[1] == 'E' && s[2] == 'R' && s[3] && s[4] == 'I' && s[5] == 'A' && s[6] == '_' && s[7] && s[8] == 'X' && !s[9]){
		if(s[3] == 'T')  return SERIAL_TX;
		else if(s[3] == 'R')  return SERIAL_TX;
	}
	if(s[0] == 'U' && s[1] == 'S' && s[2] == 'E' && s[3] == 'R' && s[4] == '_' && s[5] == 'B' && s[6] == 'U' && s[7] == 'T' && s[8] == 'T' && s[9] == 'O' && s[10] == 'N' && !s[11])
		return USER_BUTTON;


	return NC;

}

*/
//xbed
/*
Pin jshGetPinFromString(const char *s) {
	if(isStrInt(s))  return (Pin)stringToInt(s);
	// built in constants
	if(!strcmp(s,"p5"))
		return p5;
	else if(!strcmp(s,"p6"))
		return p6;
	else if(!strcmp(s,"p7"))
		return p7;
	else if(!strcmp(s,"p8"))
		return p8;
	else if(!strcmp(s,"p9"))
		return p9;
	else if(!strcmp(s,"p10"))
		return p10;
	else if(!strcmp(s,"p11"))
		return p11;
	else if(!strcmp(s,"p12"))
		return p12;
	else if(!strcmp(s,"p13"))
		return p13;
	else if(!strcmp(s,"p14"))
		return p14;
	else if(!strcmp(s,"p15"))
		return p15;
	else if(!strcmp(s,"p16"))
		return p16;
	else if(!strcmp(s,"p17"))
		return p17;
	else if(!strcmp(s,"p18"))
		return p18;
	else if(!strcmp(s,"p19"))
		return p19;
	else if(!strcmp(s,"p20"))
		return p20;
	else if(!strcmp(s,"p21"))
		return p21;
	else if(!strcmp(s,"p22"))
		return p22;
	else if(!strcmp(s,"p23"))
		return p23;
	else if(!strcmp(s,"p24"))
		return p24;
	else if(!strcmp(s,"p25"))
		return p25;
	else if(!strcmp(s,"p26"))
		return p26;
	else if(!strcmp(s,"p27"))
		return p27;
	else if(!strcmp(s,"p28"))
		return p28;
	else if(!strcmp(s,"p29"))
		return p29;
	else if(!strcmp(s,"p30"))
		return p30;
	else if(!strcmp(s,"LED1"))
		return LED1;
	else if(!strcmp(s,"LED2"))
		return LED2;
	else if(!strcmp(s,"LED3"))
		return LED3;
	else if(!strcmp(s,"LED4"))
		return LED4;
	else if(!strcmp(s,"USBTX"))
		return USBTX;
	else if(!strcmp(s,"USBRX"))
		return USBRX;
	else if(!strcmp(s,"I2C_SCL"))
		return I2C_SCL;
	else if(!strcmp(s,"I2C_SDA"))
		return I2C_SDA;
	else
		return NC;

}
*/
/** Write the pin name to a string. String must have at least 8 characters (to be safe)
 * now it just return 'Integer' String*/
void jshGetPinString(char *result, Pin pin) {
	result[0] = 0; // just in case

	if (jshIsPinValid(pin)) {
		itostr(pin,&result[0],10);
	}

}

/// Given a var, convert it to a pin ID (or -1 if it doesn't exist). safe for undefined!
Pin jshGetPinFromVar(JsVar *pinv) {
	if (jsvIsString(pinv) && pinv->varData.str[5]==0/*should never be more than 4 chars!*/) {
		return jshGetPinFromString(&pinv->varData.str[0]);
	} else if (jsvIsInt(pinv) /* This also tests for the Pin datatype */) {
		return (Pin)jsvGetInteger(pinv);
	} else return NC;
}

Pin jshGetPinFromVarAndUnLock(JsVar *pinv) {
	Pin pin = jshGetPinFromVar(pinv);
	jsvUnLock(pinv);
	return pin;
}

// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------

bool jshPinInput(Pin pin) {
	bool value = false;
	if (jshIsPinValid(pin)) {
		if (!jshGetPinStateIsManual(pin))
			jshPinSetState(pin, JSHPINSTATE_GPIO_IN);
		value = jshPinGetValue(pin);
	} else jsExceptionHere(JSET_ERROR, "Invalid pin!");
	return value;
}


void jshPinOutput(Pin pin, bool value) {
	if (jshIsPinValid(pin)) {
		if (!jshGetPinStateIsManual(pin))
			jshPinSetState(pin, JSHPINSTATE_GPIO_OUT);
		jshPinSetValue(pin, value);
	} else jsExceptionHere(JSET_ERROR, "Invalid pin!");
}

