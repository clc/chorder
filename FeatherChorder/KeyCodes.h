// KeyCodes.h
// Split out from main code for FeatherChorder.ino
// If using the feather 32u4 bluefruit then see
// https://learn.adafruit.com/adafruit-feather-32u4-bluefruit-le/ble-services
// for all posible codes (in case something has been added).
// - Greg

/*****************************************
 * Possible keycodes in the chord tables *
 *****************************************/
enum keycodes {
/* first part is simply an enumeration of the allowed scan-codes
   used for USB HID devices */
  ENUMKEY__,                // 0x00  Unused key-binding
  ENUMKEY_errorRollOver,
  ENUMKEY_POSTfail,
  ENUMKEY_errorUndefined,
  ENUMKEY_A,                // 0x04
  ENUMKEY_B,
  ENUMKEY_C,
  ENUMKEY_D,
  ENUMKEY_E,
  ENUMKEY_F,
  ENUMKEY_G,
  ENUMKEY_H,
  ENUMKEY_I,
  ENUMKEY_J,
  ENUMKEY_K,
  ENUMKEY_L,
  ENUMKEY_M,                // 0x10
  ENUMKEY_N,
  ENUMKEY_O,
  ENUMKEY_P,
  ENUMKEY_Q,
  ENUMKEY_R,
  ENUMKEY_S,
  ENUMKEY_T,
  ENUMKEY_U,
  ENUMKEY_V,
  ENUMKEY_W,
  ENUMKEY_X,
  ENUMKEY_Y,
  ENUMKEY_Z,
  ENUMKEY_1,
  ENUMKEY_2,
  ENUMKEY_3,                // 0x20
  ENUMKEY_4,
  ENUMKEY_5,
  ENUMKEY_6,
  ENUMKEY_7,
  ENUMKEY_8,
  ENUMKEY_9,
  ENUMKEY_0,                // 0x27
  ENUMKEY_enter,            // 0x28
  ENUMKEY_esc,              // 0x29
  ENUMKEY_bckspc,           // 0x2A  backspace
  ENUMKEY_tab,              // 0x2B
  ENUMKEY_spc,              // 0x2C  space
  ENUMKEY_minus,            // 0x2D  - (and _)
  ENUMKEY_equal,            // 0x2E  = (and +)
  ENUMKEY_lbr,              // 0x2F  [
  ENUMKEY_rbr,              // 0x30  ]
  ENUMKEY_bckslsh,          // 0x31  \ (and |)
  ENUMKEY_hash,             // 0x32  Non-US # and ~
  ENUMKEY_smcol,            // 0x33  ; (and :)
  ENUMKEY_ping,             // 0x34  ' and "
  ENUMKEY_grave,            // 0x35  Grave accent and tilde
  ENUMKEY_comma,            // 0x36  , (and <)
  ENUMKEY_dot,              // 0x37  . (and >)
  ENUMKEY_slash,            // 0x38  / (and ?)
  ENUMKEY_cpslck,           // 0x39  capslock
  ENUMKEY_F1,
  ENUMKEY_F2,
  ENUMKEY_F3,
  ENUMKEY_F4,
  ENUMKEY_F5,
  ENUMKEY_F6,
  ENUMKEY_F7,               // 0x40
  ENUMKEY_F8,
  ENUMKEY_F9,
  ENUMKEY_F10,
  ENUMKEY_F11,
  ENUMKEY_F12,
  ENUMKEY_PrtScr,
  ENUMKEY_scrlck,
  ENUMKEY_break,
  ENUMKEY_ins,
  ENUMKEY_home,
  ENUMKEY_pgup,
  ENUMKEY_del,
  ENUMKEY_end,
  ENUMKEY_pgdn,
  ENUMKEY_rarr,
  ENUMKEY_larr,             // 0x50
  ENUMKEY_darr,
  ENUMKEY_uarr,
  ENUMKEY_numlock,
  ENUMKEY_KPslash,
  ENUMKEY_KPast,
  ENUMKEY_KPminus,
  ENUMKEY_KPplus,
  ENUMKEY_KPenter,
  ENUMKEY_KP1,
  ENUMKEY_KP2,
  ENUMKEY_KP3,
  ENUMKEY_KP4,
  ENUMKEY_KP5,              // 0x5E
  ENUMKEY_KP6,
  ENUMKEY_KP7,
  ENUMKEY_KP8,              // 0x60
  ENUMKEY_KP9,
  ENUMKEY_KP0,
  ENUMKEY_KPcomma,
  ENUMKEY_Euro2,

/* The following are not standard USB HID, but are the modifier keys,
   handeled specially in decoding and mapped to the modifier byte in
   the USB report */
  DIV_Mods,
  MOD_LCTRL=DIV_Mods,   // 0x01
  MOD_LSHIFT,           // 0x02
  MOD_LALT,             // 0x04
  MOD_LGUI,             // 0x08
  MOD_RCTRL,            // 0x10
  MOD_RSHIFT,           // 0x20
  MOD_RALT,             // 0x40
  MOD_RGUI,             // 0x80

/* Next comes the mode change codes */
  DIV_Modes,
  MODE_RESET=DIV_Modes, // Reset (Default mode)
  MODE_MRESET,          // Master Reset and ID
  MODE_NUM,             // Number/symbols mode
  MODE_NUMLCK,          // Number/symbols lock
  MODE_FUNC,            // Function key mode
  MODE_FUNCLCK,         // Function key lock

/* Then a special mode for both num/sym and shift */
  DIV_Multi,
  MULTI_NumShift=DIV_Multi,
  MULTI_CtlAlt,

/* media keys for testing */
  MEDIA_playpause,     // PLAYPAUSE
  MEDIA_next,     // MEDIANEXT
  MEDIA_previous, // MEDIAPREVIOUS
  MEDIA_stop,     // MEDIASTOP
  MEDIA_volup,     // Volume up
  MEDIA_voldn,     // Volume dn

/* And finally macros, that generate multiple key presses */
  DIV_Macro,
  MACRO_000=DIV_Macro,  // 000
  MACRO_00,             // 00
  MACRO_quotes,         // "" and left arrow
  MACRO_parens,         // () and left arrow
  MACRO_dollar,         // aka, FORCE_LSHIFT|KEY_4
  MACRO_percent,        // aka, FORCE_LSHIFT|KEY_5
  MACRO_ampersand,      // aka, FORCE_LSHIFT|KEY_7
  MACRO_asterisk,       // aka, FORCE_LSHIFT|KEY_8
  MACRO_question,       // aka, FORCE_LSHIFT|KEY_slash
  MACRO_plus,           // aka, FORCE_LSHIFT|KEY_equal
  MACRO_openparen,      // aka, FORCE_LSHIFT|KEY_9
  MACRO_closeparen,     // aka, FORCE_LSHIFT|KEY_0
  MACRO_opencurly,      // aka, FORCE_LSHIFT|lbr
  MACRO_closecurly,     // aka, FORCE_LSHIFT|rbr
  ANDROID_search,       // aka, ALT|KEY_spc
  ANDROID_home,         // aka, ALT|KEY_esc
  ANDROID_menu,         // aka, CTRL|KEY_esc
  ANDROID_back,         // aka, KEY_esc with NO MODS
  ANDROID_dpadcenter,   // aka, KEY_KP5 with NO MODS

  /* Some new macros for a few BT functions */
  BAT_LVL,  // print the batter level of the  LiPo
  FACT_RESET, // do a factory reset (clearing  all BT  paring info)
  DIV_Last
};


// end of KeyCodes.h
