/*
 * Arduino code for a Bluetooth version of the Chorder.
 * @author: clc@clcworld.net
 * additional code by: priestdo@budgardr.org
 * This version tested on/with the Adafruit Feather 32u4 Bluefruit LE
 *
 * This is new arduino code based on/inspired by the SpiffChorder 
 * which can be found at http://symlink.dk/projects/spiffchorder/ 
 *
 * changes Feb. 2025
 * - Added many comments to remind me of what the code is doing.
 * - Work to add proper key down/key up so key repeat works started 2025-02
 * - Fixed key send, it needed Hexidecimal digits padded to be 2 char. - existing
 *   code regularly send one digit when a leading zero seems to be required by 
 *   BLE firmware greater than 0.6.7, It also partially fixed need to forget and
 *   re-pair every time. However, this still creeps back in now and then. SO...
 * - Added factory reset keyboard macro bound to Function modifyer then all 3 thumbs.
 * - Added ability to suppress all output to the serial console with VERBOSE_MODE
 *   set to false, moved this setting from BluefruitConfig.h
 * - Added Battery Voltage macro that returns the current voltage of the LiPo by
 *   printing that as keyboard output. (Not to serial console)
 * - Have not impimented key repeat yet.
 * - Added printed comment to device when doing a factory reset.
 * - Added 4 macros for the 4 unassigned chords in the default keyset
 * - Changed caps lock to be handled on the host rather than by the keyboard
 * - Added mod key latching. If LATCH is pressed, any current modKeys are  sent
 * - with each key until LATCH is pressed again.
 *   Last mucked with on: 2025/03/02
 */

#include <Arduino.h>
#include <SPI.h>

// slight change to below to match adafruit example GPD 2025-02-09
// #if not defined (_VARIANT_ARDUINO_DUE_X_)
// #include <SoftwareSerial.h>
// #endif

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

#if SOFTWARE_SERIAL_AVAILABLE
#include <SoftwareSerial.h>
#endif


#include "KeyCodes.h"
#include "ChordMappings.h"

/*=============================================================
	APPLICATION SETTINGS
	
	FACTORYRESET_ENABLE       Perform a factory reset when running this sketch
	
	Enabling this will put your Bluefruit LE module
	in a 'known good' state and clear any config
	data set in previous sketches or projects, so
	running this at least once is a good idea.
	
	When deploying your project, however, you will
	want to disable factory reset by setting this
	value to 0.  If you are making changes to your
	Bluefruit LE device via AT commands, and those
	changes aren't persisting across resets, this
	is the reason why.  Factory reset will erase
	the non-volatile memory where config data is
	stored, setting it back to factory default
	values.
	
	Some sketches that require you to bond to a
	central device (HID mouse, keyboard, etc.)
	won't work at all with this feature enabled
	since the factory reset will clear all of the
	bonding data stored on the chip, meaning the
	central device won't be able to reconnect.
	MINIMUM_FIRMWARE_VERSION  Minimum firmware version to have some new features
	VERBOSE_MODE	      If set to 'true' enables debug output, 'false'
	attempts to suppresses most serial output.
	-----------------------------------------------------------------------*/
#define FACTORYRESET_ENABLE         0
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define VERBOSE_MODE                   true

#define DEVICENAME       "FeatherChorder+"
//=============================================================
#define VBATPIN A9  // used by gAsBattLvl()
//=============================================================

// Create the bluefruit object, either software serial...uncomment these lines
/*
	SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);
	
	Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
	BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);
*/

/* ...or hardware serial, which does not need the RTS/CTS pins. Uncomment this line */
// Adafruit_BluefruitLE_UART ble(BLUEFRUIT_HWSERIAL_NAME, BLUEFRUIT_UART_MODE_PIN);

/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

/* ...software SPI, using SCK/MOSI/MISO user-defined SPI pins and then user selected CS/IRQ/RST */
//Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_SCK, BLUEFRUIT_SPI_MISO,
//                             BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS,
//                             BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

//=============================================================
// a small helper used in setup()
// ctb
void error(const __FlashStringHelper*err) {
  if ( VERBOSE_MODE ) Serial.println(err);
  while (1); 
}
//=============================================================
class Button {
  byte _pin;  // The button's I/O pin, as an Arduino pin number.
	
 public:
 Button(byte pin) : _pin(pin) {
    pinMode(pin, INPUT_PULLUP);      // Make pin an input and activate pullup.
  }
	
  bool isDown() const {
    // TODO: this assumes we're using analog pins!
    //return analogRead(_pin) < 0x100;
    return (digitalRead(_pin) == LOW);
  }
};
//=============================================================
// board and connectivity specific
// power regulator enable control pin

const int EnPin =  5;


// Pin numbers for the chording keyboard switches, using the Arduino numbering.
static const Button switch_pins[7] = {
  Button(6),  // Pinky
  Button(A5),  // Ring
  Button(A4),  // Middle
  Button(A3),  // Index 
  Button(A2),  // Near Thumb
  Button(A1),  // Center Thumb
  Button(A0),  // Far Thumb
};

//==================================================
// ctb
// A few timing constants
const int HalfSec = 500;  // for a half second delay

// It seems it can happen that there are times when sending 3 raw
// keys in a macro, the keys can arrive in the wrong order.  So this was added
// 2025-02-23 to see if it prevents that.  Adjust as needed.
// A long name for a short thing. The time between raw key sends in a macro  .
const int InterstitialDelay = 50; // for a 5/100 sec. delay (4/100 was not enough)

// note - key debounce timing constants are not in this section.

//====END=CONSTANTS=====================END=CONSTANTS=============

//=====SETUP============================SETUP====================
// board specific messages
void setup(void)
{
  // Ensure software power reset pin in high
	pinMode(EnPin, OUTPUT);      // Make pin an output,
	digitalWrite(EnPin, HIGH);  // and activate pullup.
	// while (!Serial);  // Required for Flora & Micro (and usb output)
  delay(HalfSec);
	
  if ( VERBOSE_MODE ) Serial.begin(115200);
  if ( VERBOSE_MODE ) Serial.println(F("Adafruit Bluefruit HID Chorder"));
  if ( VERBOSE_MODE ) Serial.println(F("---------------------------------------"));
	
  /* Initialise the module */
  if ( VERBOSE_MODE ) Serial.print(F("Initialising the Bluefruit LE module: "));
	
  if ( !ble.begin(VERBOSE_MODE) )
		{
			error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
		}
  if ( VERBOSE_MODE ) Serial.println( F("OK!") );
	
  if ( FACTORYRESET_ENABLE )
		{
			
			/* Perform a factory reset to make sure everything is in a known state */
			if ( VERBOSE_MODE ) Serial.println(F("Performing a factory reset: "));
			if ( ! ble.factoryReset() ){
				error(F("Factory reset failed!"));
			}
		}
	
  /* Disable command echo from Bluefruit */
  ble.echo(false);
	
  if ( VERBOSE_MODE ) Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  if ( VERBOSE_MODE )  ble.info();
	
  /* Change the device name to make it easier to find */
  if ( VERBOSE_MODE ) Serial.println(F("Setting device name to " DEVICENAME ": "));
  if (! ble.sendCommandCheckOK(F( "AT+GAPDEVNAME="DEVICENAME )) ) {
    error(F("Could not set device name?"));
  }
	
	// GPD 2025-02-09 replaced this section with section from latest hid keyboiard example
	
  /* Enable HID Service */
  if ( VERBOSE_MODE ) Serial.println(F("Enable HID Service (including Keyboard): "));
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
		{
			if ( !ble.sendCommandCheckOK(F( "AT+BleHIDEn=On" ))) {
				error(F("Could not enable Keyboard"));
			}
		}else
		{
			if (! ble.sendCommandCheckOK(F( "AT+BleKeyboardEn=On"  ))) {
				error(F("Could not enable Keyboard"));
			}
		}
	
  /* Add or remove service requires a reset */
  if ( VERBOSE_MODE ) Serial.println(F("Performing a SW reset (service changes require a reset): "));
  if (! ble.reset() ) {
    error(F("Couldn't reset??"));
  }
	
	// GPD end of replaced section
	
	
  String stringOne =  String(0x45, HEX);
	
  if ( VERBOSE_MODE ) Serial.println(stringOne);
}

// used by processReading()
// ctb
enum State {
  PRESSING,
  RELEASING,
};

State state = RELEASING;
byte lastKeyState = 0;

// used by sendKey()
// ctb
enum Mode {
  ALPHA,
  NUMSYM,
  FUNCTION
};

bool isNumsymLocked = false;
keymap_t latchMods = 0x00;  // currently latched modKeys
keymap_t modKeys = 0x00;  // current modifyers ( L/Rshift,L/Ralt, L/Rctrl, L/Rgui )

Mode mode = ALPHA;

// used by processREADING and loop
byte previousStableReading = 0;
byte currentStableReading = 0;
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 10;    // the debounce time; increase if the output flickers
//=====RESET=====================RESET==========================
void reset(){
	mode = ALPHA;
	latchMods=0x00;
	modKeys = 0x00;
	isNumsymLocked = false;
	sendRawKeyUp();
}
//=====SEND KEY====================SEND KEY========================
// used by processReading()
// ctb
void sendKey(byte keyState){
  keymap_t theKey;  
  // Determine the key based on the current mode's keymap
  if (mode == ALPHA) {
    theKey = keymap_default[keyState];
  } else if (mode == NUMSYM) {
    theKey = keymap_numsym[keyState];
  } else {
    theKey = keymap_function[keyState];
  }
	
  switch (theKey)  {
		// Handle mode switching - return immediately after the mode has changed
		// Handle basic mode switching
  case LATCH:
		if ( latchMods == 0x00 ) {  // latch was not set, so set it  current modKeys 
			latchMods = modKeys;
		} else {
      latchMods = 0x00;   // latch was set, so clear it.   
		}
		return;
  case MODE_NUM:
    if (mode == NUMSYM) {
      mode = ALPHA;
    } else {
      mode = NUMSYM;
    }
    return;
  case MODE_FUNC:
    if (mode == FUNCTION) {
      mode = ALPHA;
    } else {
      mode = FUNCTION;
    }
    return;
  case MODE_RESET:
		reset();
    return;
  case MODE_MRESET:
		reset();
    digitalWrite(EnPin, LOW);  // turn off 3.3v regulator enable.
    return;
		// something with a battery only		
	case BAT_LVL:
		// get and send the battedy level, then
		// do a mode_reset
    gAsBattLvl();
		reset();
    return;
	case MODE_FRESET:
    sendFactoryReset();
    return;
		// back to common code
		// Handle mode locks
  case MODE_NUMLCK:
    if (isNumsymLocked){
      isNumsymLocked = false;
      mode = ALPHA;
    } else {
      isNumsymLocked = true;
      mode = NUMSYM;
    }
    return;
		// Handle modifier keys toggling
  case MOD_LCTRL:
    modKeys = modKeys ^ 0x01;
    return;
  case MOD_LSHIFT:
    modKeys = modKeys ^ 0x02;
    return;
  case MOD_LALT:
    modKeys = modKeys ^ 0x04;
    return;
  case MOD_LGUI:
    modKeys = modKeys ^ 0x08;
    return;
  case MOD_RCTRL:
    modKeys = modKeys ^ 0x10;
    return;
  case MOD_RSHIFT:
    modKeys = modKeys ^ 0x20;
    return;
  case MOD_RALT:
    modKeys = modKeys ^ 0x40;
    return;
  case MOD_RGUI:
    modKeys = modKeys ^ 0x80;
    return;
		// Handle special keys
  case MULTI_NumShift:
    if (mode == NUMSYM) {
      mode = ALPHA;
    } else {
      mode = NUMSYM;
    }
    modKeys = modKeys ^ 0x02;
    return;
  case MULTI_CtlAlt:
    modKeys = modKeys ^ 0x01;
    modKeys = modKeys ^ 0x04;
    return;
		/* Everything after this sends actual keys to the system; break rather than
 			 return since we want to reset the modifiers after these keys are sent. */
  case MACRO_000:
	  sendRawKey(0x00, ENUMKEY_0);
    sendRawKey(0x00, ENUMKEY_0);
    sendRawKey(0x00, ENUMKEY_0);
    break;
  case MACRO_00:
    sendRawKey(0x00, ENUMKEY_0);
    sendRawKey(0x00, ENUMKEY_0);
    break;
  case MACRO_quotes:
    sendRawKey(0x02, 0x34);
    delay(InterstitialDelay);
    sendRawKey(0x02, 0x34);
    delay(InterstitialDelay);
    sendRawKey(0x00, 0x50);
    break;
  case MACRO_parens:
    sendRawKey(0x02, 0x26);
    delay(InterstitialDelay);
    sendRawKey(0x02, 0x27);
    delay(InterstitialDelay);
    sendRawKey(0x00, 0x50);
    break;
  case MACRO_dollar:
    sendRawKey(0x02, 0x21);
    break;
  case MACRO_percent:
    sendRawKey(0x02, 0x22);
    break;
  case MACRO_ampersand:
    sendRawKey(0x02, 0x24);
    break;
  case MACRO_asterisk:
    sendRawKey(0x02, 0x25);
    break;
  case MACRO_question:
    sendRawKey(0x02, 0x38);
    break;
  case MACRO_plus:
    sendRawKey(0x02, 0x2E);
    break;
  case MACRO_openparen:
    sendRawKey(0x02, 0x26);
    break;
  case MACRO_closeparen:
    sendRawKey(0x02, 0x27);
    break;
  case MACRO_opencurly:
    sendRawKey(0x02, 0x2F);
    break;
  case MACRO_closecurly:
    sendRawKey(0x02, 0x30);
    break;
  case MACRO_1 :
		// er
		sendRawKey(modKeys, ENUMKEY_E);
		delay(InterstitialDelay);
		sendRawKey(modKeys, ENUMKEY_R);
		break;
	case MACRO_2:
		// th
		sendRawKey(modKeys, ENUMKEY_T);
		delay(InterstitialDelay);
		sendRawKey(modKeys, ENUMKEY_H);
		break;
	case MACRO_3:
		// an
		sendRawKey(modKeys, ENUMKEY_A);
		delay(InterstitialDelay);
		sendRawKey(modKeys, ENUMKEY_N);
		break;
	case MACRO_4:
		// in
		sendRawKey(modKeys, ENUMKEY_I);
		delay(InterstitialDelay);
		sendRawKey(modKeys, ENUMKEY_N);
		break;
		// macro test is a long string to confirm length of interstitial delay
	case MACRO_TEST:
		sendRawKey (modKeys, ENUMKEY_A);
		delay(InterstitialDelay);
		sendRawKey (modKeys, ENUMKEY_B);
		delay(InterstitialDelay);
		sendRawKey (modKeys, ENUMKEY_C);
		delay(InterstitialDelay);
		sendRawKey (modKeys, ENUMKEY_D);
		delay(InterstitialDelay);
		sendRawKey (modKeys, ENUMKEY_E);
		delay(InterstitialDelay);
		sendRawKey (modKeys, ENUMKEY_F);
		delay(InterstitialDelay);
		sendRawKey (modKeys, ENUMKEY_G);
		delay(InterstitialDelay);
		sendRawKey (modKeys, ENUMKEY_H);
		break;
	case MACRO_SHIFTDN:
		modKeys = MOD_LSHIFT;
   	sendRawKeyDn (MOD_LSHIFT, 0x00);
		break;
		// Handle Android specific keys
  case ANDROID_search:
    sendRawKey(0x04, 0x2C);
    break;
  case ANDROID_home:
    sendRawKey(0x04, 0x29);
    break;
  case ANDROID_menu:
    sendRawKey(0x10, 0x29);
    break;
  case ANDROID_back:
    sendRawKey(0x00, 0x29);
    break;
  case ANDROID_dpadcenter:
    sendRawKey(0x00, 0x5D);
    break;
  case MEDIA_playpause:
    sendControlKey("PLAYPAUSE");
    break;
  case MEDIA_stop:
    sendControlKey("MEDIASTOP");
    break;
  case MEDIA_next:
    sendControlKey("MEDIANEXT");
    break;
  case MEDIA_previous:
    sendControlKey("MEDIAPREVIOUS");
    break;
  case MEDIA_volup:
    sendControlKey("VOLUME+,500");
    break;
  case MEDIA_voldn:
    sendControlKey("VOLUME-,500");
    break;
		// Send the key
  default:
    sendRawKey(modKeys, theKey);
    break;
  }
	
  modKeys = latchMods; //sets modKeys to any currently latched mods, or 0x00 if none
  mode = ALPHA;
	// Reset the modKeys and mode based on
	if (isNumsymLocked){
    mode = NUMSYM;
  }
}

//======SEND RAW KEY====================SEND RAW KEY================
// ctb
// used in sendKey()
//
// new sendRawKey to make sure all is working as expected
// before trying to impliment seporate watching for key down and key up
// to allow host side key repeat
// 


void sendRawKey(char modKey, char rawKey){
	sendRawKeyDn(modKey, rawKey);
	sendRawKeyUp();
}

//======SEND RAW KEY DOWN===============SEND RAW KEY DOWN============
// connectivity specific - This is for BT/BLE
// used in sendRawKey()
//

void sendRawKeyDn(char modKey, char rawKey){
	// Format for Bluefruit Feather is MOD-00-KEY.
	// Plan: use print to only print the last 2 ch so that we get 2 char
	// String keys = String(modKey, HEX) + "-00-" + String(rawKey, HEX);
	
	
	// pad & trim modKey to ensure 2 digit hexidecimal is sent
	String tmpModKey = "00" + String(modKey, HEX);
	int modKeyLen = tmpModKey.length() - 2;
	// pad & trim rawKey to ensure 2 digit hexidecimal is sent
	String tmpRawKey = "00" + String(rawKey, HEX);
	int rawKeyLen = tmpRawKey.length() - 2;
	
	ble.print("AT+BLEKEYBOARDCODE=");
	ble.println(String(&tmpModKey[modKeyLen]) + "-00-" + String(&tmpRawKey[rawKeyLen]));
	
	
}

//======SEND RAW KEY UP==============SEND RAW KEY UP==================
// connectivity specific - This is for BT/BLE
// used in sendRawKey()  and sendString()
//
void sendRawKeyUp(){
	ble.println("AT+BLEKEYBOARDCODE=00-00");
}  
//======SEND STRING============SEND STRING==========================
// connectivity specific - This is for BT/BLE
// Currently this is only for testing, it was temporarily added to MRESET
//
void sendString(String StringOut){
  // 
  ble.print("AT+BleKeyboard=");
  ble.println(StringOut);
  sendRawKeyUp(); // just in case as there have been some odd key repeats happening.
}  

//======SEND MOUSE KEY=====SEND MOUSE KEY===========================
// connectivity specific - This is for BT/BLE
// 
void sendMouseKey(String MouseKey){
	ble.print("AT+BleHidMouseButton=");
	ble.println(MouseKey);
	delay(HalfSec);
	ble.println("AT+BleHidMouseButton=0");
}
//======SEND CONTROL KEY============SEND CONTROL KEY==================
// connectivity specific -  This is for BT/BLE
// used in sendKey()
//
void sendControlKey(String cntrlName){
  // note: for Volume +/- and the few other keys that take a time to hold, simply add it into the string
  // for example:
  //    sendControlKey("VOLUME+,500")
  // will send Volume up and hold it for half a second
  ble.print("AT+BleHidControlKey=");
  ble.println(cntrlName);
}
//======GET AND SEND BATTERY LEVEL==================================
// board specific this is for BLE feather
// note needs #define VBATPIN A9 which is up top with other defines.
void gAsBattLvl() {   
	float measuredvbat = analogRead(VBATPIN);
	measuredvbat *= 2;    // we divided by 2, so multiply back
	measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
	measuredvbat /= 1024; // convert to voltage
	sendString( " Kbd Batt: " );
	sendString( String(measuredvbat) );
	sendString(  "volts. " );
}
//======SEND FACTORY RESET============SEND FACTORY RESET===============
//  board specific - This is for BLE feather
// Factory Reset will clear the Bluetooth known hosts table
void sendFactoryReset(){
  // Perform a factory reset to make sure everything is in a known state 
    if ( VERBOSE_MODE ) Serial.println(F("Performing a factory reset: "));
	sendString( " Performing Factory Reset, You will need to pair the device after..."); 
	if ( ! ble.factoryReset() ){
		error(F("Factory reset failed!"));
	}
}
//=====PROCESS READING==================PROCESS READING===============
// ctb
// used in loop()
//
// check if was pressing chord and now releasing, change to releasing,
//                                                 then send the key
//         if chord was not pressing and now is, change to pressing
//
void processReading(){
	switch (state) {
	case PRESSING:
		if (previousStableReading & ~currentStableReading) {
			state = RELEASING;
			sendKey(previousStableReading);
		} 
		break;
		
	case RELEASING:
		if (currentStableReading & ~previousStableReading) {
			state = PRESSING;
		}
		break;
	}
}

//========LOOP=========================LOOP==================
// ctb
void loop() {
  // Build the current key state.
  byte keyState = 0, mask = 1;
  for (int i = 0; i < 7; i++) {
    if (switch_pins[i].isDown()) keyState |= mask;
    mask <<= 1;
  }
	
  if (lastKeyState != keyState) {
    lastDebounceTime = millis();
  }
	
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
    currentStableReading = keyState;
  }
	
  if (previousStableReading != currentStableReading) {
    processReading();
    previousStableReading = currentStableReading;
  }
	
  lastKeyState = keyState;
}
