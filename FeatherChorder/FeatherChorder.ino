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
 * 
 * Last mucked with on: 2025/02/14 21:18 UTC
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
//
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

//====END=CONSTANTS=====================END=CONSTANTS=============

//=====SETUP============================SETUP====================
void setup(void)
{
  // Ensure software power reset pin in high
   pinMode(EnPin, OUTPUT);      // Make pin an output,
   digitalWrite(EnPin, HIGH);  // and activate pullup.
   // while (!Serial);  // Required for Flora & Micro (and usb output)
  delay(500);

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
enum State {
  PRESSING,
  RELEASING,
};

State state = RELEASING;
byte lastKeyState = 0;

// used by sendKey()
enum Mode {
  ALPHA,
  NUMSYM,
  FUNCTION
};

bool isCapsLocked = false;
bool isNumsymLocked = false;
keymap_t modKeys = 0x00;

Mode mode = ALPHA;

// used by processREADING and loop
byte previousStableReading = 0;
byte currentStableReading = 0;
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 10;    // the debounce time; increase if the output flickers

//=====send()======================send()==========================
// ??? where is this used ??? 
// ??? trying with this commented out as I see no place it is
// ??? used - GPD 2025-02
//GPD 
//GPDvoid send(char* character) {
//GPD//  Uart.print(character);
//GPD}

//=====SEND KEY====================SEND KEY========================
// used by processReading()
//
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
    mode = ALPHA;
    modKeys = 0x00;
    isCapsLocked = false;
    isNumsymLocked = false;
    return;
  case MODE_MRESET:
    mode = ALPHA;
    modKeys = 0x00;
    isCapsLocked = false;
    isNumsymLocked = false;
//    digitalWrite(EnPin, LOW);  // turn off 3.3v regulator enable.
//    digitalWrite(EnPin, HIGH);  // turn on 3.3v regulator enable.
//    sendMouseKey("L,press");  // added for testing
//    sendString(" -Keyboard State Reset- ");  // added for testing
//    
    return;
 case BAT_LVL:
 // get and send the battedy level, then
 // do a mode_reset
    gAsBattLvl();
    mode = ALPHA;
    modKeys = 0x00;
    isCapsLocked = false;
    isNumsymLocked = false;
    return;
 case FACT_RESET:
    sendFactoryReset();
    return;
// Handle mode locks
  case ENUMKEY_cpslck:
    if (isCapsLocked){
      isCapsLocked = false;
      modKeys = 0x00;
    } else {
      isCapsLocked = true;
      modKeys = 0x02;
    }
    return;
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
    sendRawKey(0x00, 0x27);
    sendRawKey(0x00, 0x27);
    sendRawKey(0x00, 0x27);
    break;
  case MACRO_00:
    sendRawKey(0x00, 0x27);
    sendRawKey(0x00, 0x27);
    break;
  case MACRO_quotes:
    sendRawKey(0x02, 0x34);
    sendRawKey(0x02, 0x34);
    sendRawKey(0x00, 0x50);
    break;
  case MACRO_parens:
    sendRawKey(0x02, 0x26);
    sendRawKey(0x02, 0x27);
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

  modKeys = 0x00;
  mode = ALPHA;
// Reset the modKeys and mode based on locks
  if (isCapsLocked){
    modKeys = 0x02;
  }
  if (isNumsymLocked){
    mode = NUMSYM;
  }
}

/* NEW CODE 2025-02 */

//======SEND RAW KEY====================SEND RAW KEY================
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
// used in sendRawKey()  and sendString()
//
void sendRawKeyUp(){
   ble.println("AT+BLEKEYBOARDCODE=00-00");
}
//======SEND STRING============SEND STRING==========================
// Currently this is only for testing, it was temporarily added to MRESET
//
void sendString(String StringOut){
  // 
  ble.print("AT+BleKeyboard=");
  ble.println(StringOut);
  sendRawKeyUp(); // just in case as there have been some odd key repeats happening.
}  
//======SEND MOUSE KEY=====SEND MOUSE KEY===========================
// Currently this is only for testing, it was temporarily added to MRESET
//
void sendMouseKey(String MouseKey){
    ble.print("AT+BleHidMouseButton=");
    ble.println(MouseKey);
    delay(500);
    ble.println("AT+BleHidMouseButton=0");
}

//======SEND CONTROL KEY============SEND CONTROL KEY==================
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
// Currently this is only for testing, it was temporarily added to MRESET
//
void sendFactoryReset(){
  // Perform a factory reset to make sure everything is in a known state 
    if ( VERBOSE_MODE ) Serial.println(F("Performing a factory reset: "));
    sendString( " Performing Factory Reset, You will need to pair the device after..."); 
    if ( ! ble.factoryReset() ){
      error(F("Factory reset failed!"));
    }
}
//=====PROCESS READING==================PROCESS READING===============
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
 
