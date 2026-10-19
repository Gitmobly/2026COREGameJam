#ifndef GAMEUI
#define GAMEUI

#include "gamestate.h"
#include "graphics.h"
#include "vendor/raylib/raylib.h"
#include "vendor/raylib/raymath.h"

#define MAX_BUTTONS 32

/*
ENUM to set x/y value to a anchored pos i.e. center, left, top, bottom, right
not best way to impl it but cba to do a hole thing for it so it just a neg number
Also could be defs but meh zig brain
 */
typedef enum {
  CENTER_AXIS = -999,
  NEG_AXIS = -998,
  POS_AXIS = -997,
} ANCHOR_POS;

/*
button struct font size and font are set by def atm
*/
typedef struct button_t {
  // could remove these too but again cba
  int x,y,w,h;
  UI_STATE owner;
  float font_size;
  Rectangle rect;
  Vector2 textPos;
  Color bg, fg;
  Font font;
  const char* text;
  void(*click)(void);
  bool button_muted;
} button_t;


typedef struct ui_t {
  UI_STATE owner;
  button_t buttons[MAX_BUTTONS];
} ui_t;

// probs doesnt need to be pointer 
inline static void draw_button(button_t* button){
  DrawRectangleRounded(button->rect, 0.5f, 24, button->bg);
  DrawTextEx(button->font, button->text, button->textPos, button->font_size, button->font_size * .1f, button->fg);
}

inline static button_t create_button(int x, int y, int w, int h, Color bg, Color fg, const char* text, void (*click)(void)) {
  button_t button = {
    .x = x,
    .y = y,
    .w = w,
    .h = h,
    .text = text,
    .click = click,
    .bg = bg,
    .fg = fg,
    .owner = START_UI,
    .font = GetFontDefault(),
    .font_size = 40.0,
    .button_muted = false,
  };

  Vector2 textSize = MeasureTextEx(button.font, button.text, button.font_size, button.font_size*.1f);

  
  Vector2 buttonSize = (Vector2) {
    button.w+textSize.x,
    button.h+textSize.y,
  };


  switch (button.x) {
    case CENTER_AXIS:
      x = COMP_CENTER_X(buttonSize.x);
      break;
    case POS_AXIS:
      x = SCREEN_WD - buttonSize.x;
      break;
    case NEG_AXIS:
      x = 0;
      break;
    default:
      break;
  }

  switch (button.y) {
    case CENTER_AXIS:
      y = COMP_CENTER_Y(buttonSize.y);
      break;
    case POS_AXIS:
      y = 0;
      break;
    case NEG_AXIS:
      y = SCREEN_HT - buttonSize.y;
      break;
    default:
      break;
  }


  Vector2 textPos = (Vector2) {
    x + Lerp(0.0f, buttonSize.x - textSize.x, (float)0.5),
    y + Lerp(0.0f, buttonSize.y - textSize.y, (float)0.5),
  };

  Rectangle rect = {(float)x, (float)y, buttonSize.x, buttonSize.y};

  button.rect = rect;
  button.textPos = textPos;

  return button;
}



/* -- Function def  -- */

ui_t init_start_ui( void );

void start_click( void );

void render_current_ui ( ui_t* ui );



#endif
