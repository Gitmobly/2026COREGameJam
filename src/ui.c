#include "ui.h"
#include "gamestate.h"
#include "graphics.h"
#include "vendor/raylib/raylib.h"
#include <stdio.h>


void render_current_ui(ui_t* ui) {
  for (int i = 0; i < MAX_BUTTONS; i++) {
    button_t b = ui->buttons[i];
    if(b.owner == NO_OWNER) continue;
    draw_button(&b);
  }
}


ui_t init_start_ui(){
  ui_t ui = {
    .owner = START_UI,
    .buttons = {},
  };

  // set all button owner to -1 (no owner)
  for (int i=0; i<MAX_BUTTONS; i++) ui.buttons[i].owner = NO_OWNER;

  button_t start_button =
    create_button(PAD_BUTTON(NEG_AXIS, 25), SCREEN_HT - 100, 10, 5, RAYWHITE, BLACK, "START GAME", start_click);

  ui.buttons[0] = start_button;

  return ui;
}

void start_click() {
  printf("Blayt Suka, kakaye zeh topiya suka\n");
}



