#include "ui.h"
#include "entity.h"
#include "gamestate.h"
#include "graphics.h"
#include "vendor/raylib/raylib.h"
#include "vendor/raylib/raymath.h"

// Should move this to a ui manager or something but for the sake of time just keeping them here
ui_t start_ui;
ui_t game_ui;
ui_t options_ui;
ui_t combat_ui;


button_t create_button(unsigned int x, unsigned int y, int w, int h, Color bg, Color fg, const char *text, void (*click)(gamestate*)) {

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

  Vector2 axisPos = calculate_axis_pos(button.x, button.y, buttonSize);


  Vector2 textPos = (Vector2) {
    axisPos.x + Lerp(0.0f, buttonSize.x - textSize.x, (float)0.5),
    axisPos.y + Lerp(0.0f, buttonSize.y - textSize.y, (float)0.5),
  };

  Rectangle rect = {axisPos.x, axisPos.y, buttonSize.x, buttonSize.y};

  button.rect = rect;
  button.textPos = textPos;
  button.x = (int) axisPos.x;
  button.y = (int) axisPos.y;

  return button;
}

lable_t create_lable(unsigned int x, unsigned int y, const char *text, Color fg, float fontSize, Font font) {
  lable_t lable = {
    .x = x,
    .y = y,
    .fg = fg,
    .font_size = fontSize,
    .font = font,
    .text = text,
  };
  
  Vector2 textSize = MeasureTextEx(lable.font, lable.text, lable.font_size, lable.font_size*.1f);

  Vector2 axisPos = calculate_axis_pos(lable.x, lable.y, textSize);
  lable.x = axisPos.x;
  lable.y = axisPos.y;

  return lable;
}

image_t create_image(Rectangle rect, Rectangle texture_pos, Texture2D texture, unsigned int rot){
  image_t image = {
    .pos = rect,
    .texture_pos = texture_pos,
    .rot = rot,
    .texture = texture,
  };

  Vector2 axisPos = calculate_axis_pos_rec(rect);

  image.pos.x = axisPos.x;
  image.pos.y = axisPos.y;

  return image;
}



Vector2 calculate_axis_pos(unsigned int x, unsigned int y, Vector2 widthHeight){
  Vector2 ret = {(float)x, (float) y};
  unsigned int xv = x;
  unsigned int yv = y;

  xv &= ~PAD_MASK;
  yv &= ~PAD_MASK;

  switch (xv) {
    case CENTER_AXIS:
      ret.x = (float) COMP_CENTER_X(widthHeight.x);
      break;
    case POS_AXIS:
      // padding 
      xv = (x & PAD_MASK);
      ret.x = (float)(SCREEN_WD - (widthHeight.x + xv));
      break;
    case NEG_AXIS:
      // padding
      xv = (x & PAD_MASK);
      ret.x = (float) (0 + xv);
      break;
    default:
      break;
  }
  switch (yv) {
    case CENTER_AXIS:
      ret.y = COMP_CENTER_Y(widthHeight.y);
      break;
    case POS_AXIS:
      // padding 
      yv = (y & PAD_MASK);
      ret.y = (float)(0 + yv);
      break;
    case NEG_AXIS:
      // padding 
      yv = (y & PAD_MASK);
      ret.y = (float)(SCREEN_HT - (widthHeight.y + yv));
      break;
    default:
      break;
  }


  return ret;
}

Vector2 calculate_axis_pos_rec(Rectangle rect){
  return calculate_axis_pos((unsigned int) rect.x, (unsigned int) rect.y, (Vector2) {rect.width, rect.height});
}

void render_current_ui(ui_t* ui) {
  for (int i = 0; i < MAX_BUTTONS; i++) {
    button_t b = ui->buttons[i];
    if(b.owner == NO_OWNER) continue;
    draw_button(&b);
  }
  for (int i = 0; i < MAX_LABLES; i++) {
    lable_t l = ui->lables[i];
    if(l.owner == NO_OWNER) continue;
    draw_lable(&l);
  }
  for(int i = 0; i < MAX_IMAGES; i++) {
    image_t im = ui->images[i];
    if(im.owner == NO_OWNER) continue;
    draw_image(&im);
  }
}

void unload_ui(ui_t* ui) {
  for (int i = 0; i < MAX_IMAGES; i++) {
    image_t img = ui->images[i];
    if(img.owner == NO_OWNER) continue;
    UnloadTexture(img.texture);
  }
}

void init_ui_components(ui_t *ui) {
  // set all button owner to -1 (no owner)
  for (int i=0; i<MAX_BUTTONS; i++) ui->buttons[i].owner = NO_OWNER;
  for (int i=0; i<MAX_LABLES; i++) ui->lables[i].owner = NO_OWNER;
  for (int i=0; i<MAX_IMAGES; i++) ui->images[i].owner = NO_OWNER;
}



ui_t* init_start_ui(void){
  ui_t ui = {
    .owner = START_UI,
  };

  init_ui_components(&ui);

  button_t start_button =
    create_button(PAD_COMP(NEG_AXIS, 38), SCREEN_HT - 85, 10, 5, RAYWHITE, BLACK, "START GAME", start_click_start);
  button_t options_button =
    create_button(PAD_COMP(POS_AXIS, 38), SCREEN_HT - 85, 10, 5, RAYWHITE, BLACK, "OPTIONS", options_click_start);

  start_button.owner = START_UI;
  options_button.owner = START_UI;

  lable_t lable = create_lable(CENTER_AXIS, CENTER_AXIS, "2026 GameJam", RAYWHITE, 32.0f, GetFontDefault());
  lable.owner = START_UI;

  Texture2D texture = LoadTexture("./assets/death_start_game.png");
  Rectangle pos = {CENTER_AXIS, 100, 150, 150};
  Rectangle text_pos = {0,0,texture.width,texture.height};

  image_t image = create_image(pos, text_pos, texture, 0);
  image.owner = START_UI;

  

  ui.buttons[0] = start_button;
  ui.buttons[1] = options_button;
  ui.lables[0] = lable;
  ui.images[0] = image;

  start_ui = ui;

  return &start_ui;
}


ui_t* init_game_ui(void) {
  ui_t ui = {
    .owner = GAME,
  };

  init_ui_components(&ui);
  game_ui = ui;
  return &game_ui;
}

ui_t* init_options_ui(void) {
  ui_t ui = {
    .owner = OPTIONS_UI,
  };

  init_ui_components(&ui);
  button_t back_button =
    create_button(PAD_COMP(NEG_AXIS, 38), SCREEN_HT - 85, 10, 5, RAYWHITE, BLACK, "BACK", back_click_options);
  back_button.owner = OPTIONS_UI;

  ui.buttons[0] = back_button;

  options_ui = ui;
  return &options_ui;
}


ui_t* init_combat_ui(entity_t* entity) {
  ui_t ui = {
    .owner = COMBAT_UI,
  };

  init_ui_components(&ui);

  combat_ui = ui;

  return &combat_ui;
}


void start_click_start( gamestate* state ) {
  unload_ui(state->active_ui);
  state->ui_state = GAME;
  state->active_ui = init_game_ui();
}

// Not impl yet
void options_click_start(gamestate* state) {
  unload_ui(state->active_ui);
  state->ui_state=OPTIONS_UI;
  state->active_ui = init_options_ui();
 }

void back_click_options(gamestate* state) {
   state->ui_state = START_UI;
   // blat textures blat unload ui too 
   state->active_ui = init_start_ui();
 }


