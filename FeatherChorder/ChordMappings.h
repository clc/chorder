// ChordMappings.h
// Chordmappings for 7 button chorder, split out from FeatherChorder.ino
// Mappings moved here so they can be changed without risk
// of modifying the rest of the code.
// - Greg

typedef uint8_t keymap_t;


/**********************************************************
 *  order is  Far Thumb, Center Thumb, Near Thumb button  *
 *  Index Finger, Middle Finger, Ring Finger, Pinky       *
 *  FCN IMRP                                              *
 **********************************************************/
const keymap_t keymap_default[128] = {
  ENUMKEY__,                        // --- ----  0x00 no keys pressed
  ENUMKEY_W,                        // --- ---P  0x01
  ENUMKEY_Y,                        // --- --R-  0x02
  ENUMKEY_U,                        // --- --RP  0x03
  ENUMKEY_R,                        // --- -M--  0x04
  MACRO_openparen,                  // --- -M-P  0x05
  ENUMKEY_H,                        // --- -MR-  0x06
  ENUMKEY_S,                        // --- -MRP  0x07

  ENUMKEY_I,                        // --- I---  0x08
  ENUMKEY_B,                        // --- I--P  0x09
  ENUMKEY_K,                        // --- I-R-  0x0A
  ENUMKEY_Z,                        // --- I-RP  0x0B
  ENUMKEY_D,                        // --- IM--  0x0C
  MACRO_closeparen,                 // --- IM-P  0x0D
  ENUMKEY_E,                        // --- IMR-  0x0E
  ENUMKEY_T,                        // --- IMRP  0x0F

  MODE_NUM,                         // --N ----  0x10
  MODE_FUNC,                        // --N ---P  0x11
  ENUMKEY_esc,                      // --N --R-  0x12
  ENUMKEY_smcol,                    // --N --RP  0x13
  ENUMKEY_comma,                    // --N -M--  0x14
  MACRO_closecurly,                 // --N -M-P  0x15
  ENUMKEY_dot,                      // --N -MR-  0x16
  MOD_LALT,                         // --N -MRP  0x17

  ENUMKEY__,                        // --N I---  0x18
  ENUMKEY_ins,                      // --N I--P  0x19
  MOD_LGUI,                         // --N I-R-  0x1A
  MOD_LCTRL,                        // --N I-RP  0x1B
  ENUMKEY_F9,                       // --N IM--  0x1C
  MACRO_opencurly,                  // --N IM-P  0x1D
  ENUMKEY_ping,                     // --N IMR-  0x1E
  MODE_NUMLCK,                      // --N IMRP  0x1F

  ENUMKEY_spc,                      // -C- ----  0x20
  ENUMKEY_F,                        // -C- ---P  0x21
  ENUMKEY_G,                        // -C- --R-  0x22
  ENUMKEY_V,                        // -C- --RP  0x23
  ENUMKEY_C,                        // -C- -M--  0x24
  ENUMKEY_rbr,                      // -C- -M-P  0x25
  ENUMKEY_P,                        // -C- -MR-  0x26
  ENUMKEY_N,                        // -C- -MRP  0x27

  ENUMKEY_L,                        // -C- I---  0x28
  ENUMKEY_X,                        // -C- I--P  0x29
  ENUMKEY_J,                        // -C- I-R-  0x2A
  ENUMKEY_Q,                        // -C- I-RP  0x2B
  ENUMKEY_M,                        // -C- IM--  0x2C
  ENUMKEY_lbr,                      // -C- IM-P  0x2D
  ENUMKEY_A,                        // -C- IMR-  0x2E
  ENUMKEY_O,                        // -C- IMRP  0x2F

  MULTI_NumShift,                   // -CN ----  0x30
  ENUMKEY__,                        // -CN ---P  0x31
  ENUMKEY__,                        // -CN --R-  0x32
  ENUMKEY__,                        // -CN --RP  0x33
  ANDROID_dpadcenter,               // -CN -M--  0x34
  ENUMKEY__,                        // -CN -M-P  0x35
  ANDROID_home,                     // -CN -MR-  0x36
  MOD_RALT,                         // -CN -MRP  0x37

  ENUMKEY__,                        // -CN I---  0x38
  ANDROID_back,                     // -CN I--P  0x39
  MOD_RGUI,                         // -CN I-R-  0x3A
  MOD_RCTRL,                        // -CN I-RP  0x3B
  ANDROID_menu,                     // -CN IM--  0x3C
  ENUMKEY__,                        // -CN IM-P  0x3D
  ANDROID_search,                   // -CN IMR-  0x3E
  ENUMKEY_numlock,                  // -CN IMRP  0x3F

  MOD_LSHIFT,                       // F-- ----  0x40
  ENUMKEY_enter,                    // F-- ---P  0x41
  ENUMKEY_rarr,                     // F-- --R-  0x42
  ENUMKEY_darr,                     // F-- --RP  0x43
  ENUMKEY_bckspc,                   // F-- -M--  0x44
  ENUMKEY_PrtScr,                   // F-- -M-P  0x45
  ENUMKEY_del,                      // F-- -MR-  0x46
  ENUMKEY_pgdn,                     // F-- -MRP  0x47

  ENUMKEY_larr,                     // F-- I---  0x48
  ENUMKEY_end,                      // F-- I--P  0x49
  ENUMKEY_tab,                      // F-- I-R-  0x4A
  ENUMKEY_home,                     // F-- I-RP  0x4B
  ENUMKEY_uarr,                     // F-- IM--  0x4C
  ENUMKEY_scrlck,                   // F-- IM-P  0x4D
  ENUMKEY_pgup,                     // F-- IMR-  0x4E
  ENUMKEY_cpslck,                   // F-- IMRP  0x4F

  ENUMKEY_break,                    // F-N ----  0x50
  ENUMKEY__,                        // F-N ---P  0x51
  ENUMKEY__,                        // F-N --R-  0x52
  ENUMKEY__,                        // F-N --RP  0x53
  ENUMKEY__,                        // F-N -M--  0x54
  ENUMKEY__,                        // F-N -M-P  0x55
  ENUMKEY__,                        // F-N -MR-  0x56
  ENUMKEY__,                        // F-N -MRP  0x57

  ENUMKEY__,                        // F-N I---  0x58
  ENUMKEY__,                        // F-N I--P  0x59
  ENUMKEY__,                        // F-N I-R-  0x5A
  ENUMKEY__,                        // F-N I-RP  0x5B
  ENUMKEY__,                        // F-N IM--  0x5C
  ENUMKEY__,                        // F-N IM-P  0x5D
  ENUMKEY__,                        // F-N IMR-  0x5E
  ENUMKEY__,                        // F-N IMRP  0x5F

  MOD_RSHIFT,                       // FC- ----  0x60
  ENUMKEY_KPenter,                  // FC- ---P  0x61
  ENUMKEY_KP6,                      // FC- --R-  0x62
  MEDIA_volup, // was ENUMKEY_KP2,  //  FC- --RP  0x63
  MEDIA_stop, // was ENUMKEY_KP5,   // FC- -M--  0x64
  ENUMKEY_KPast,                    // FC- -M-P  0x65
  MEDIA_playpause,  // was ENUMKEY_KPcomma,   // FC- -MR-  0x66
  MEDIA_next, // was ENUMKEY_KP3,             // FC- -MRP  0x67

  ENUMKEY_KP4,                      // FC- I---  0x68
  MEDIA_previous, // was ENUMKEY_KP1,         // FC- I--P  0x69
  ENUMKEY_KPminus,                  // FC- I-R-  0x6A
  ENUMKEY_KP7,                      // FC- I-RP  0x6B
  MEDIA_voldn, // was ENUMKEY_KP8,            // FC- IM--  0x6C
  ENUMKEY_KPslash,                  // FC- IM-P  0x6D
  MEDIA_previous, // was ENUMKEY_KP9,         // FC- IMR-  0x6E
  ENUMKEY_KP0,                      // FC- IMRP  0x6F

  MODE_MRESET,                      // FCN ----  0x70
  ENUMKEY__,                        // FCN ---P  0x71
  ENUMKEY__,                        // FCN --R-  0x72
  ENUMKEY__,                        // FCN --RP  0x73
  ENUMKEY__,                        // FCN -M--  0x74
  ENUMKEY__,                        // FCN -M-P  0x75
  ENUMKEY__,                        // FCN -MR-  0x76
  ENUMKEY__,                        // FCN -MRP  0x77

  ENUMKEY__,                        // FCN I---  0x78
  ENUMKEY__,                        // FCN I--P  0x79
  ENUMKEY__,                        // FCN I-R-  0x7A
  ENUMKEY__,                        // FCN I-RP  0x7B
  ENUMKEY__,                        // FCN IM--  0x7C
  ENUMKEY__,                        // FCN IM-P  0x7D
  ENUMKEY__,                        // FCN IMR-  0x7E
  ENUMKEY__                         // FCN IMRP  0x7F
};

/**************************************
 * number/symbols mode                *
 **************************************/
const keymap_t keymap_numsym[128] = {
  ENUMKEY__,                        // --- ----  0x00
  ENUMKEY_5,                        // --- ---P  0x01
  ENUMKEY_4,                        // --- --R-  0x02
  MACRO_quotes,                     // --- --RP  0x03   "" and a back arrow
  ENUMKEY_3,                        // --- -M--  0x04
  ENUMKEY__,                        // --- -M-P  0x05
  MACRO_00,                         // --- -MR-  0x06   00
  ENUMKEY_minus,                    // --- -MRP  0x07

  ENUMKEY_2,                        // --- I---  0x08
  ENUMKEY_bckslsh,                  // --- I--P  0x09
  MACRO_dollar,                     // --- I-R-  0x0A   $
  ENUMKEY_grave,                    // --- I-RP  0x0B
  ENUMKEY_slash,                    // --- IM--  0x0C
  ENUMKEY__,                        // --- IM-P  0x0D
  ENUMKEY_equal,                    // --- IMR-  0x0E
  MACRO_000,                        // --- IMRP  0x0F   000

  ENUMKEY_spc,                      // --N ----  0x10
  MODE_FUNC,                        // --N ---P  0x11
  ENUMKEY_esc,                      // --N --R-  0x12
  ENUMKEY_smcol,                    // --N --RP  0x13
  ENUMKEY_comma,                    // --N -M--  0x14
  ENUMKEY__,                        // --N -M-P  0x15
  ENUMKEY_dot,                      // --N -MR-  0x16
  MOD_LALT,                         // --N -MRP  0x17

  ENUMKEY__,                        // --N I---  0x18
  ENUMKEY_ins,                      // --N I--P  0x19
  MOD_LGUI,                         // --N I-R-  0x1A
  MOD_LCTRL,                        // --N I-RP  0x1B
  ENUMKEY__,                        // --N IM--  0x1C
  ENUMKEY__,                        // --N IM-P  0x1D
  ENUMKEY_ping,                     // --N IMR-  0x1E
  MODE_RESET,                       // --N IMRP  0x1F

  ENUMKEY_1,                        // -C- ----  0x20
  ENUMKEY_9,                        // -C- ---P  0x21
  ENUMKEY_8,                        // -C- --R-  0x22
  ENUMKEY_rbr,                      // -C- --RP  0x23
  ENUMKEY_7,                        // -C- -M--  0x24
  ENUMKEY_rbr,                      // -C- -M-P  0x25
  MACRO_percent,                    // -C- -MR-  0x26   %
  ENUMKEY_lbr,                      // -C- -MRP  0x27

  ENUMKEY_6,                        // -C- I---  0x28
  MACRO_ampersand,                  // -C- I--P  0x29       &
  MACRO_parens,                     // -C- I-R-  0x2A       () and a back arrow
  MACRO_question,                   // -C- I-RP  0x2B       ?
  MACRO_asterisk,                   // -C- IM--  0x2C
  ENUMKEY_lbr,                      // -C- IM-P  0x2D
  MACRO_plus,                       // -C- IMR-  0x2E
  ENUMKEY_0,                        // -C- IMRP  0x2F

  MULTI_NumShift,                   // -CN ----  0x30
  ENUMKEY__,                        // -CN ---P  0x31
  ENUMKEY__,                        // -CN --R-  0x32
  ENUMKEY__,                        // -CN --RP  0x33
  ANDROID_dpadcenter,               // -CN -M--  0x34
  ENUMKEY__,                        // -CN -M-P  0x35
  ANDROID_home,                     // -CN -MR-  0x36
  MOD_RALT,                         // -CN -MRP  0x37

  ENUMKEY__,                        // -CN I---  0x38
  ANDROID_back,                     // -CN I--P  0x39
  MOD_RGUI,                         // -CN I-R-  0x3A
  MOD_RCTRL,                        // -CN I-RP  0x3B
  ANDROID_menu,                     // -CN IM--  0x3C
  ENUMKEY__,                        // -CN IM-P  0x3D
  ANDROID_search,                   // -CN IMR-  0x3E
  ENUMKEY_numlock,                  // -CN IMRP  0x3F

  MOD_LSHIFT,                       // F-- ----  0x40
  ENUMKEY_enter,                    // F-- ---P  0x41
  ENUMKEY_rarr,                     // F-- --R-  0x42
  ENUMKEY_darr,                     // F-- --RP  0x43
  ENUMKEY_bckspc,                   // F-- -M--  0x44
  ENUMKEY_PrtScr,                   // F-- -M-P  0x45
  ENUMKEY_del,                      // F-- -MR-  0x46
  ENUMKEY_pgdn,                     // F-- -MRP  0x47

  ENUMKEY_larr,                     // F-- I---  0x48
  ENUMKEY_end,                      // F-- I--P  0x49
  ENUMKEY_tab,                      // F-- I-R-  0x4A
  ENUMKEY_home,                     // F-- I-RP  0x4B
  ENUMKEY_uarr,                     // F-- IM--  0x4C
  ENUMKEY_scrlck,                   // F-- IM-P  0x4D
  ENUMKEY_pgup,                     // F-- IMR-  0x4E
  ENUMKEY_cpslck,                   // F-- IMRP  0x4F

  ENUMKEY_break,                    // F-N ----  0x50
  ENUMKEY__,                        // F-N ---P  0x51
  ENUMKEY__,                        // F-N --R-  0x52
  ENUMKEY__,                        // F-N --RP  0x53
  ENUMKEY__,                        // F-N -M--  0x54
  ENUMKEY__,                        // F-N -M-P  0x55
  ENUMKEY__,                        // F-N -MR-  0x56
  ENUMKEY__,                        // F-N -MRP  0x57

  ENUMKEY__,                        // F-N I---  0x58
  ENUMKEY__,                        // F-N I--P  0x59
  ENUMKEY__,                        // F-N I-R-  0x5A
  ENUMKEY__,                        // F-N I-RP  0x5B
  ENUMKEY__,                        // F-N IM--  0x5C
  ENUMKEY__,                        // F-N IM-P  0x5D
  ENUMKEY__,                        // F-N IMR-  0x5E
  ENUMKEY__,                        // F-N IMRP  0x5F

  MOD_RSHIFT,                       // FC- ----  0x60
  ENUMKEY_KPenter,                  // FC- ---P  0x61
  ENUMKEY_KP6,                      // FC- --R-  0x62
  ENUMKEY_KP2,                      // FC- --RP  0x63
  ENUMKEY_KP5,                      // FC- -M--  0x64
  ENUMKEY_KPast,                    // FC- -M-P  0x65
  ENUMKEY_KPcomma,                  // FC- -MR-  0x66
  ENUMKEY_KP3,                      // FC- -MRP  0x67

  ENUMKEY_KP4,                      // FC- I---  0x68
  ENUMKEY_KP1,                      // FC- I--P  0x69
  ENUMKEY_KPminus,                  // FC- I-R-  0x6A
  ENUMKEY_KP7,                      // FC- I-RP  0x6B
  ENUMKEY_KP8,                      // FC- IM--  0x6C
  ENUMKEY_KPslash,                  // FC- IM-P  0x6D
  ENUMKEY_KP9,                      // FC- IMR-  0x6E
  ENUMKEY_KP0,                      // FC- IMRP  0x6F

  MODE_RESET,                      // FCN ----  0x70
  ENUMKEY__,                        // FCN ---P  0x71
  ENUMKEY__,                        // FCN --R-  0x72
  ENUMKEY__,                        // FCN --RP  0x73
  ENUMKEY__,                        // FCN -M--  0x74
  ENUMKEY__,                        // FCN -M-P  0x75
  ENUMKEY__,                        // FCN -MR-  0x76
  ENUMKEY__,                        // FCN -MRP  0x77

  ENUMKEY__,                        // FCN I---  0x78
  ENUMKEY__,                        // FCN I--P  0x79
  ENUMKEY__,                        // FCN I-R-  0x7A
  ENUMKEY__,                        // FCN I-RP  0x7B
  ENUMKEY__,                        // FCN IM--  0x7C
  ENUMKEY__,                        // FCN IM-P  0x7D
  ENUMKEY__,                        // FCN IMR-  0x7E
  ENUMKEY__                         // FCN IMRP  0x7F
};

/**************************************
 * function key mode                  *
 **************************************/
const keymap_t keymap_function[128] = {
  ENUMKEY__,                        // --- ----  0x00
  ENUMKEY_F5,                       // --- ---P  0x01
  ENUMKEY_F4,                       // --- --R-  0x02
  MEDIA_volup,                      // --- --RP  0x03
  ENUMKEY_F3,                       // --- -M--  0x04
  ENUMKEY__,                        // --- -M-P  0x05
  ENUMKEY__,                        // --- -MR-  0x06
  MEDIA_stop,                       // --- -MRP  0x07

  ENUMKEY_F2,                       // --- I---  0x08
  MEDIA_previous,                   // --- I--P  0x09
  ENUMKEY__,                        // --- I-R-  0x0A
  ENUMKEY__,                        // --- I-RP  0x0B
  MEDIA_voldn,                      // --- IM--  0x0C
  ENUMKEY__,                        // --- IM-P  0x0D
  ENUMKEY__,                        // --- IMR-  0x0E
  ENUMKEY__,                        // --- IMRP  0x0F

  ENUMKEY__,                        // --N ----  0x10
  MODE_RESET,                       // --N ---P  0x11
  ENUMKEY__,                        // --N --R-  0x12
  ENUMKEY__,                        // --N --RP  0x13
  ENUMKEY__,                        // --N -M--  0x14
  ENUMKEY__,                        // --N -M-P  0x15
  ENUMKEY__,                        // --N -MR-  0x16
  MOD_LALT,                         // --N -MRP  0x17

  ENUMKEY__,                        // --N I---  0x18
  ENUMKEY__,                        // --N I--P  0x19
  MOD_LGUI,                         // --N I-R-  0x1A
  MOD_LCTRL,                        // --N I-RP  0x1B
  ENUMKEY__,                        // --N IM--  0x1C
  ENUMKEY__,                        // --N IM-P  0x1D
  ENUMKEY__,                        // --N IMR-  0x1E
  ENUMKEY__,                        // --N IMRP  0x1F

  ENUMKEY_F1,                       // -C- ----  0x20
  ENUMKEY_F9,                       // -C- ---P  0x21
  ENUMKEY_F8,                       // -C- --R-  0x22
  ENUMKEY_F12,                      // -C- --RP  0x23
  ENUMKEY_F7,                       // -C- -M--  0x24
  ENUMKEY__,                        // -C- -M-P  0x25
  ENUMKEY_F11,                      // -C- -MR-  0x26
  ENUMKEY__,                        // -C- -MRP  0x27

  ENUMKEY_F6,                       // -C- I---  0x28
  ENUMKEY__,                        // -C- I--P  0x29
  ENUMKEY__,                        // -C- I-R-  0x2A
  ENUMKEY__,                        // -C- I-RP  0x2B
  ENUMKEY_F10,                      // -C- IM--  0x2C
  ENUMKEY__,                        // -C- IM-P  0x2D
  ENUMKEY__,                        // -C- IMR-  0x2E
  ENUMKEY__,                        // -C- IMRP  0x2F

  MULTI_NumShift,                   // -CN ----  0x30
  ENUMKEY__,                        // -CN ---P  0x31
  ENUMKEY__,                        // -CN --R-  0x32
  ENUMKEY__,                        // -CN --RP  0x33
  ANDROID_dpadcenter,               // -CN -M--  0x34
  ENUMKEY__,                        // -CN -M-P  0x35
  ANDROID_home,                     // -CN -MR-  0x36
  MOD_RALT,                         // -CN -MRP  0x37

  ENUMKEY__,                        // -CN I---  0x38
  ANDROID_back,                     // -CN I--P  0x39
  MOD_RGUI,                         // -CN I-R-  0x3A
  MOD_RCTRL,                        // -CN I-RP  0x3B
  ANDROID_menu,                     // -CN IM--  0x3C
  ENUMKEY__,                        // -CN IM-P  0x3D
  ANDROID_search,                   // -CN IMR-  0x3E
  ENUMKEY__,                        // -CN IMRP  0x3F

  MOD_LSHIFT,                       // F-- ----  0x40
  ENUMKEY__,                        // F-- ---P  0x41
  ENUMKEY__,                        // F-- --R-  0x42
  ENUMKEY__,                        // F-- --RP  0x43
  ENUMKEY__,                        // F-- -M--  0x44
  ENUMKEY__,                        // F-- -M-P  0x45
  MEDIA_playpause,                  // F-- -MR-  0x46
  MEDIA_next,                        // F-- -MRP  0x47

  MEDIA_previous,                   // F-- I---  0x48
  ENUMKEY__,                        // F-- I--P  0x49
  ENUMKEY__,                        // F-- I-R-  0x4A
  ENUMKEY__,                        // F-- I-RP  0x4B
  ENUMKEY__,                        // F-- IM--  0x4C
  ENUMKEY__,                        // F-- IM-P  0x4D
  ENUMKEY__,                        // F-- IMR-  0x4E
  ENUMKEY__,                        // F-- IMRP  0x4F

  ENUMKEY__,                        // F-N ----  0x50
  ENUMKEY__,                        // F-N ---P  0x51
  ENUMKEY__,                        // F-N --R-  0x52
  ENUMKEY__,                        // F-N --RP  0x53
  ENUMKEY__,                        // F-N -M--  0x54
  ENUMKEY__,                        // F-N -M-P  0x55
  ENUMKEY__,                        // F-N -MR-  0x56
  ENUMKEY__,                        // F-N -MRP  0x57

  ENUMKEY__,                        // F-N I---  0x58
  ENUMKEY__,                        // F-N I--P  0x59
  ENUMKEY__,                        // F-N I-R-  0x5A
  ENUMKEY__,                        // F-N I-RP  0x5B
  ENUMKEY__,                        // F-N IM--  0x5C
  ENUMKEY__,                        // F-N IM-P  0x5D
  ENUMKEY__,                        // F-N IMR-  0x5E
  ENUMKEY__,                        // F-N IMRP  0x5F

  MOD_RSHIFT,                       // FC- ----  0x60
  ENUMKEY__,                        // FC- ---P  0x61
  ENUMKEY__,                        // FC- --R-  0x62
  ENUMKEY__,                        // FC- --RP  0x63
  ENUMKEY__,                        // FC- -M--  0x64
  ENUMKEY__,                        // FC- -M-P  0x65
  ENUMKEY__,                        // FC- -MR-  0x66
  ENUMKEY__,                        // FC- -MRP  0x67

  ENUMKEY__,                        // FC- I---  0x68
  ENUMKEY__,                        // FC- I--P  0x69
  ENUMKEY__,                        // FC- I-R-  0x6A
  ENUMKEY__,                        // FC- I-RP  0x6B
  ENUMKEY__,                        // FC- IM--  0x6C
  ENUMKEY__,                        // FC- IM-P  0x6D
  ENUMKEY__,                        // FC- IMR-  0x6E
  ENUMKEY__,                        // FC- IMRP  0x6F

  MODE_RESET,                       // FCN ----  0x70
  ENUMKEY__,                        // FCN ---P  0x71
  ENUMKEY__,                        // FCN --R-  0x72
  ENUMKEY__,                        // FCN --RP  0x73
  ENUMKEY__,                        // FCN -M--  0x74
  ENUMKEY__,                        // FCN -M-P  0x75
  ENUMKEY__,                        // FCN -MR-  0x76
  ENUMKEY__,                        // FCN -MRP  0x77

  ENUMKEY__,                        // FCN I---  0x78
  ENUMKEY__,                        // FCN I--P  0x79
  ENUMKEY__,                        // FCN I-R-  0x7A
  ENUMKEY__,                        // FCN I-RP  0x7B
  ENUMKEY__,                        // FCN IM--  0x7C
  ENUMKEY__,                        // FCN IM-P  0x7D
  ENUMKEY__,                        // FCN IMR-  0x7E
  ENUMKEY__                         // FCN IMRP  0x7F
};

// end ChordMappings.h
