#ifndef GAMEUI
#define GAMEUI

#include "entity.h"
#include "gamestate.h"
#include "vendor/raylib/raylib.h"
#include <inttypes.h>

#define MAX_BUTTONS 32
#define MAX_LABELS	64
#define MAX_IMAGES	16

#define PAD_MASK  0xFFF
#define MUTE(c)	  ((c > 30) ? c - 30 : c + 30)
#define UNMUTE(c) ((c > 30) ? c + 30 : c - 30)

/*
 * Macro to pad out componets anchor pos
 */
#define PAD_COMP(anchor, offset) (anchor | ((offset > PAD_MASK) ? 0 : offset))

/*
 * ENUM to set x/y value to a anchored pos i.e. center, left, top, bottom, right
 * not best way to impl it but cba to do a hole thing for it so its just a big number
 * Also could be defines but meh zig brain
 *
 * This also lets us pad/add offset to button e.g. POS_AXIS | 25
 * Max is 4095 or 0xFFF
 *
 * Also before anyone says "i might put this number in by accident"
 * Please ask yourself why you are putting a button at x|y 268431360
 */
typedef enum
{
	CENTER_AXIS = 0xFFFF000,
	NEG_AXIS	= 0xFF00000,
	POS_AXIS	= 0x00FF000,
} ANCHOR_POS;

/*
 * button struct font size and font are set by def atm
 */
typedef struct button_t
{
	unsigned int x, y, w, h;
	UI_STATE	 owner;
	Color		 bg, fg;
	float		 font_size;
	Rectangle	 rect;
	Vector2		 textPos;
	Font		 font;
	const char*	 text;
	void (*click)(gamestate*);
	bool button_muted;
} button_t;

typedef struct label_t
{
	unsigned int x, y;
	UI_STATE	 owner;
	Color		 fg;
	float		 font_size;
	Font		 font;
	const char*	 text;
} label_t;

typedef struct image_t
{
	UI_STATE	 owner;
	unsigned int rot;
	Rectangle	 pos;
	Rectangle	 texture_pos;
	Texture2D	 texture;
} image_t;

/*
 * UI Struct for building a ui
 */
typedef struct ui_t
{
	UI_STATE owner;
	button_t buttons[MAX_BUTTONS];
	label_t	 labels[MAX_LABELS];
	image_t	 images[MAX_IMAGES];
} ui_t;

/*
 * Draws a button to the screen
 */
inline static void draw_button(button_t* button)
{
	DrawRectangleRounded(button->rect, 0.5f, 24, button->bg);
	DrawTextEx(button->font, button->text, button->textPos, button->font_size, button->font_size * .1f, button->fg);
}

inline static void draw_label(label_t* label)
{
	DrawTextEx(label->font, label->text, (Vector2){(float)label->x, (float)label->y}, label->font_size,
			   label->font_size * .1f, label->fg);
}

inline static void draw_image(image_t* image)
{
	DrawTexturePro(image->texture, image->texture_pos, image->pos, (Vector2){0, 0}, (f32)image->rot, RAYWHITE);
}

/*
 * Creates a button struct
 *
 * x - the x position, can be anchored with ANCHOR_POS enum and you can add padding to the anchor with
 * PAD_BUTTON(ANCHOR_POS, PADDING) or just ANCHOR_POS | PADDING
 *
 * y - the y position, can be anchored with ANCHOR_POS enum and you can add padding to the anchor with
 * PAD_BUTTON(ANCHOR_POS, PADDING) or just ANCHOR_POS | PADDING
 *
 * w - Width is calculated from the text width, so this is added onto that width
 *
 * h - Height is calculated from the text Height, so this is added onto that height
 *
 * bg - Background color
 *
 * fg - Text color
 *
 * text - the button text
 *
 * click - onclick function pointer, takes (gamestate*)
 *
 * Font size and font is set by default
 */
button_t create_button(unsigned int x, unsigned int y, int w, int h, Color bg, Color fg, const char* text,
					   void (*click)(gamestate*));

void reframe_button(button_t* button);

// todo impl add functions, i.e. add button add label

label_t create_label(unsigned int x, unsigned int y, const char* text, Color fg, float fontSize, Font font);

void update_label(label_t* label);

image_t create_image(Rectangle rect, Rectangle texture_pos, Texture2D texture, unsigned int rot);

Vector2 calculate_axis_pos(unsigned int x, unsigned int y, Vector2 widthHeight);
Vector2 calculate_axis_pos_rec(Rectangle rect);

/* -- Function def  -- */

void unload_ui(ui_t* ui);
void render_current_ui(ui_t* ui);
void init_ui_components(ui_t* ui);

ui_t* init_game_ui(void);

/* Start UI function */
ui_t* init_start_ui(void);
void  start_click_start(gamestate* state);
void  options_click_start(gamestate* state);
/* Start UI function */

ui_t* init_options_ui(void);
void  back_click_options(gamestate* state);

ui_t* init_comabat_ui(entity_t* entity, gamestate* state);
void  attack_click_combat(gamestate* state);
void  skills_click_combat(gamestate* state);

#endif
