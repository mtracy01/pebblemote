#include <pebble.h>

#define KEY_DATA 5

static Window *window;
//static TextLayer *text_forward;   /* Top    Layer   */
static TextLayer *text_play;     /*  Middle Layer  */
//static TextLayer *text_back;    /*   Bottom Layer */
static TextLayer *text_layer;


//Handle play request

static void send_int(int key, int value);

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Play/Pause");
  send_int(1,1);
  
}

//Handle forward request
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Forward");
  //sendMessage(0);
  //send_int(1,1);
}

//Handle back request
static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Back");
  //sendMessage(2);
  //send_int(1,1);
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  //text_forward =  text_layer_create((GRect){.origin = {0,72}, .size = { bounds.size.w, 20}});
  text_play    =  text_layer_create((GRect){.origin = { 0, 72}, .size = { bounds.size.w, 20 } });
  //text_back    =  text_layer_create((GRect){.origin = {0,72}, .size = { bounds.size.w, 20}});
 
  //text_layer_set_text(text_forward, "Forward");
  text_layer_set_text(text_play, "Play/Pause");
  //text_layer_set_text(text_back, "Back");
  
 // text_layer_set_text_alignment(text_forward, GAlignTop);
  text_layer_set_text_alignment(text_play, GTextAlignmentCenter);
 // text_layer_set_text_alignment(text_back, GAlignTop);
  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer, "Press a button");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_play);
  //text_layer_destroy(text_back);
  //text_layer_destroy(text_forward);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Get the first pair
  Tuple *t = dict_read_first(iterator);

  // Process all pairs present
  while (t != NULL) {
    // Long lived buffer
    static char s_buffer[64];

    // Process this pair's key
    switch (t->key) {
      case KEY_DATA:
        // Copy value and display
        snprintf(s_buffer, sizeof(s_buffer), "Received '%s'", t->value->cstring);
        //text_layer_set_text(s_output_layer, s_buffer);
        break;
    }

    // Get next pair, if any
    t = dict_read_next(iterator);
  }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}


static void init(void) {
  //Set appropriate callback handlers
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
   // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  
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

static void send_int(int key, int value) {
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  dict_write_int(iter, key, &value, sizeof(int), true);
  app_message_outbox_send();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}