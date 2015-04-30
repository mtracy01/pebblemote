#include <pebble.h>

static Window *window;
static TextLayer *text_forward;   /* Top    Layer   */
static TextLayer *text_play;     /*  Middle Layer  */
static TextLayer *text_back;    /*   Bottom Layer */

//Handle play request
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  //text_layer_set_text(text_layer, "Select");
}

//Handle forward request
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  //text_layer_set_text(text_layer, "Up");
}

//Handle back request
static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  //text_layer_set_text(text_layer, "Down");
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  text_forward =  text_layer_create((GRect){.origin = {0,72}, .size = { bounds.size.w, 20}});
  text_play    =  text_layer_create((GRect){.origin = {0,72}, .size = { bounds.size.w, 20}});
  text_back    =  text_layer_create((GRect){.origin = {0,72}, .size = { bounds.size.w, 20}});
 
  text_layer_set_text(text_forward, "Forward");
  text_layer_set_text(text_play, "Play/Pause");
  text_layer_set_text(text_back, "Back");
  
  text_layer_set_text_alignment(text_forward, GAlignTop);
  text_layer_set_text_alignment(text_play, GTextAlignmentCenter);
  text_layer_set_text_alignment(text_back, GAlignTop);
  /*text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer, "Press a button");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  */
}

static void window_unload(Window *window) {
  text_layer_destroy(text_play);
  text_layer_destroy(text_back);
  text_layer_destroy(text_forward);
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
	.load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}