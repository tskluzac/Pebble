#include <pebble.h>
/*
 * main.c
 * Constructs a Window housing an output TextLayer to show data from 
 * either modes of operation of the accelerometer.
 */

//Required to program for all versions of the Pebble Watch. 
#include <pebble.h>
#define TAP_NOT_DATA true

// Static variable declaration and pointer.
static Window *s_main_window;
static TextLayer *s_output_layer;

static void data_handler(AccelData *data, uint32_t num_samples) {
  // Long lived buffer
  static char s_buffer[128];

  // Compose string of all data
  
  snprintf(s_buffer, sizeof(s_buffer), 
    "N X,Y,Z\n0 %d,%d,%d\n1 %d,%d,%d\n2 %d,%d,%d", 
    data[0].x, data[0].y, data[0].z, 
    data[1].x, data[1].y, data[1].z, 
    data[2].x, data[2].y, data[2].z
  );
  
  DataLoggingSessionRef logging_session = data_logging_create(0x1234, DATA_LOGGING_INT, num_samples, false);
  data_logging_log(logging_session, &data, num_samples);
  data_logging_finish(logging_session);
  
  //DataLoggingResult r = data_logging_log(my_data_log, data, num_samples);

  
  //Show the data
  text_layer_set_text(s_output_layer, s_buffer);
}

static void tap_handler(AccelAxisType axis, int32_t direction) {
  switch (axis) {
  case ACCEL_AXIS_X:
    if (direction > 0) {
      text_layer_set_font(s_output_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
      text_layer_set_text(s_output_layer, "Rest Well.\nWe'll see you tomorrow");
    } else {
      text_layer_set_font(s_output_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
      text_layer_set_text(s_output_layer, "Rest Well.\nWe'll see you tomorrow!");
    }
    break;
  case ACCEL_AXIS_Y:
    if (direction > 0) {
      text_layer_set_font(s_output_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
      text_layer_set_text(s_output_layer, "Rest Well.\nWe'll see you tomorrow!");
    } else {
       text_layer_set_font(s_output_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
      text_layer_set_text(s_output_layer, "Rest Well.\nWe'll see you tomorrow!");
    }
    break;
  case ACCEL_AXIS_Z:
    if (direction > 0) {
      text_layer_set_font(s_output_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
      text_layer_set_text(s_output_layer, "Rest Well.\nWe'll see you tomorrow!");
    } else {
      text_layer_set_font(s_output_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
      text_layer_set_text(s_output_layer, "Rest Well.\nWe'll see you tomorrow!");
    
    }
    break;
  }
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Create output TextLayer
  s_output_layer = text_layer_create(GRect(5, 0, window_bounds.size.w - 10, window_bounds.size.h));
  text_layer_set_font(s_output_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text(s_output_layer, "Welcome to myBivy! Please shake watch to calibrate.");
  text_layer_set_overflow_mode(s_output_layer, GTextOverflowModeWordWrap);
  layer_add_child(window_layer, text_layer_get_layer(s_output_layer));
}

static void main_window_unload(Window *window) {
  // Destroy output TextLayer
  text_layer_destroy(s_output_layer);
}


///***** Init, Deinit, and Main Clauses*******/////
static void init() {
  // Create main Window
    //DataLoggingSessionRef logging_session = data_logging_create(0x1234, DATA_LOGGING_UINT, 4, false);
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_stack_push(s_main_window, true);

  // Use tap service? If not, use data service
  if (TAP_NOT_DATA) {
    // Subscribe to the accelerometer tap service
    accel_tap_service_subscribe(tap_handler);
  } else {
    // Subscribe to the accelerometer data service
    int num_samples = 3;
    
    accel_data_service_subscribe(num_samples, data_handler);

    // Choose update rate
    accel_service_set_sampling_rate(ACCEL_SAMPLING_50HZ);
  }
}


//Denit terminates and unsubscribes from process and data from init. 
static void deinit() {
  // Destroy main Window
    text_layer_set_text(s_output_layer, "");
  window_destroy(s_main_window);

  if (TAP_NOT_DATA) {
    accel_tap_service_unsubscribe();
  } else {
    accel_data_service_unsubscribe();
  }
}

//Main function - looks simple, but these 5 lines run the whole pebble watch program. 
int main(void) {
  init();
  app_event_loop();
  deinit();
}