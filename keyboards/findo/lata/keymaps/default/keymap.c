#include "keycode.h"
#include QMK_KEYBOARD_H

enum layer_number {
  U_COLEMAKDH,
  U_NAV,
  U_MEDIA,
  U_NUM,
  U_SYM,
  U_FUN,
  U_GAME
};

#define XXX   KC_NO
#define U_NP  KC_NO // key is not present
#define U_NA  KC_NO // present but not available for use
#define U_NU  KC_NO // available but not used

#define U_RDO (LCTL(KC_Y)) // redo
#define U_PST (LCTL(KC_V)) // paste
#define U_CPY (LCTL(KC_C)) // copy
#define U_CUT (LCTL(KC_X)) // cut
#define U_UND (LCTL(KC_Z)) // undo


#define LAYOUT_miryoku( \
     K00, K01, K02, K03, K04,                K05, K06, K07, K08, K09, \
     K10, K11, K12, K13, K14,                K15, K16, K17, K18, K19, \
     K20, K21, K22, K23, K24,                K25, K26, K27, K28, K29, \
     N30, N31, K32, K33, K34,                K35, K36, K37, N38, N39  \
) \
LAYOUT_split_4x6_3( \
KC_SCLN,  KC_1, KC_2, KC_3, KC_4, KC_5,               KC_6, KC_7, KC_8, KC_9, KC_0, KC_COLN,  \
KC_TAB,   K00,  K01,  K02,  K03,  K04,                K05,  K06,  K07,  K08,  K09,  KC_GRV,  \
KC_MINS,  K10,  K11,  K12,  K13,  K14,                K15,  K16,  K17,  K18,  K19,  XXX,  \
KC_PIPE,  K20,  K21,  K22,  K23,  K24,                K25,  K26,  K27,  K28,  K29,  KC_UNDS,  \
                      K32,  K33,  K34,                K35,  K36,  K37                 \
)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

   [U_COLEMAKDH] = LAYOUT_miryoku(
    KC_Q,         KC_W,         KC_F,               KC_P,               KC_B,                             KC_J,               KC_L,               KC_U,             KC_Y,               KC_QUOT,
    LGUI_T(KC_A), LALT_T(KC_R), LCTL_T(KC_S),       LSFT_T(KC_T),       KC_G,                             KC_M,               RSFT_T(KC_N),               RCTL_T(KC_E),     RALT_T(KC_I),       RGUI_T(KC_O),
    KC_Z,         KC_X,         KC_C,               KC_D,               KC_V,                             KC_K,               KC_H,               KC_COMM,          KC_DOT,             KC_SLSH,
    U_NP,         U_NP,         LT(U_MEDIA,KC_ESC), LT(U_NAV,KC_BSPC),  KC_LSFT,                          LT(U_SYM,KC_ENT),   LT(U_NUM,KC_SPC),   LT(U_FUN,KC_DEL), U_NP,               U_NP
  ),

  [U_NAV] = LAYOUT_miryoku(         
    RESET,             U_NA,              DF(U_GAME),         DF(U_COLEMAKDH),  U_NA,                     U_RDO,              U_PST,              U_CPY,             U_CUT,             U_UND,  
    KC_LGUI,           KC_LALT,           KC_LCTL,            KC_LSFT,          U_NA,                     KC_LEFT,            KC_DOWN,            KC_UP,             KC_RGHT,           KC_CAPS,
    U_UND,             U_CUT,             U_CPY,              U_PST,            U_RDO,                    KC_HOME,            KC_PGDN,            KC_PGUP,           KC_END,            KC_INS, 
    U_NP,              U_NP,              U_NA,               U_NA,             U_NA,                     KC_ENT,             KC_BSPC,            KC_DEL,            U_NP,              U_NP
  ),

  [U_MEDIA] = LAYOUT_miryoku(         
    RESET,             U_NA,              DF(U_GAME),        DF(U_COLEMAKDH),   U_NA,                     U_NU,               U_NU,               U_NU,               U_NU,              U_NU,
    KC_LGUI,           KC_LALT,           KC_LCTL,           KC_LSFT,           U_NA,                     KC_MPRV,            KC_VOLD,            KC_VOLU,           KC_MNXT,              U_NU,              
    U_NA,              U_NA,              U_NA,              U_NA,              U_NA,                     OUT_AUTO,           U_NU,               U_NU,              U_NU,                 U_NU,   
    U_NP,              U_NP,              U_NA,              U_NA,              U_NA,                     KC_MSTP,            KC_MPLY,            KC_MUTE,           U_NP,                 U_NP
  ),

  [U_NUM] = LAYOUT_miryoku(         
    KC_PLUS,           KC_7,              KC_8,               KC_9,              KC_EQL,                  U_NA,               DF(U_COLEMAKDH),    DF(U_GAME),         U_NA,             RESET,  
    KC_MINS,           KC_4,              KC_5,               KC_6,              U_NU,                    U_NA,               KC_LSFT,            KC_LCTL,            KC_LALT,          KC_LGUI,
    KC_DOT,            KC_1,              KC_2,               KC_3,              U_NU,                    U_NA,               U_NA,               U_NA,               U_NA,             U_NA,   
    U_NP,              U_NP,              KC_0,               KC_SPC,            KC_SLSH,                 U_NA,               U_NA,               U_NA,               U_NP,             U_NP
  ),

  [U_SYM] = LAYOUT_miryoku(         
    KC_HASH,           KC_CIRC,           KC_ASTR,           KC_DLR,           KC_BSLS,                  U_NA,               DF(U_COLEMAKDH),    DF(U_GAME),         U_NA,             RESET,             
    KC_AMPR,           KC_AT,             KC_LPRN,           KC_RPRN,          KC_COLN,                  U_NA,               KC_LSFT,            KC_LCTL,            KC_LALT,          KC_LGUI,           
    KC_PERC,           KC_EXLM,           KC_LBRC,           KC_RBRC,          KC_GRV,                   U_NA,               U_NA,               U_NA,               U_NA,             U_NA,              
    U_NP,              U_NP,              KC_MINS,           KC_UNDS,          KC_PLUS,                  U_NA,               U_NA,               U_NA,               U_NP,             U_NP
  ),

  [U_FUN] = LAYOUT_miryoku(         
    KC_F12,            KC_F7,             KC_F8,             KC_F9,             KC_PSCR,                  U_NA,               DF(U_COLEMAKDH),    DF(U_GAME),         U_NA,             RESET,  
    KC_F11,            KC_F4,             KC_F5,             KC_F6,             KC_SLCK,                  U_NA,               KC_LSFT,            KC_LCTL,            KC_LALT,          KC_LGUI,
    KC_F10,            KC_F1,             KC_F2,             KC_F3,             KC_PAUS,                  U_NA,               U_NA,               U_NA,               U_NA,             U_NA,   
    U_NP,              U_NP,              KC_APP,            KC_SPC,            KC_TAB,                   U_NA,               U_NA,               U_NA,               U_NP,             U_NP
  ),

  [U_GAME] = LAYOUT_miryoku(
    KC_TAB,   KC_Q,     KC_W,     KC_E,       KC_R,                                                       KC_T,               KC_Y,               KC_U,               KC_I,             KC_O,            
    KC_LSFT,  KC_A,     KC_S,     KC_D,       KC_F,                                                       KC_G,               KC_H,               KC_J,               KC_K,             KC_L,   
    KC_LCTL,  KC_Z,     KC_X,     KC_C,       KC_V,                                                       KC_B,               KC_N,               KC_M,               KC_COMM,          KC_DOT,
    U_NP,     U_NP,     KC_ESC,   KC_SPC,     KC_LALT,                                                    KC_F9,              KC_F10,             DF(U_COLEMAKDH),    U_NP,             U_NP
  )
};
