#include <pebble.h>
  

static Window *window;
static TextLayer *text_layer;
static TextLayer *text_layer2;

void vibes_short_pulse(void);
void vibes_long_pulse(void);
void vibes_double_pulse(void);
void vibes_cancel(void);


static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Click to Test Sleep");
  vibes_long_pulse();
  //Direct out to Accelerometer application. 
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  
  //GRect bounds = layer_get_bounds(window_layer);
  //Get this to display three lines of text. 
  //text_layer_set_text(text_layer, "Sleep Statistics");
  text_layer_set_text(text_layer2, "Total Perturbs: ");
  //text_layer_set_text(text_layer2, "Total Pert. per Minute: ");


  vibes_short_pulse();
  //Un-cache data from phone --- put to watch. 
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Settings");
  vibes_double_pulse();
  //Allow user to turn on/off buzz-settings.  
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer, "Sleep Tight 5.0");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
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