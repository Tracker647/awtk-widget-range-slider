/**
 * File:   range_slider.c
 * Author: 
 * Brief:  
 *
 * Copyright (c) 2024 - 2024 
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2024-7-4  created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/widget_vtable.h"
#include "range_slider.h"
#include <stdbool.h>

#define DEBUG_RANGE_SLIDER(_slider_, fmt, ...)                         \
  printf("range_slider %s(%#x) |> ", _slider_->widget.name, _slider_); \
  printf(fmt, ##__VA_ARGS__);                                          \
  printf("\r\n");

typedef enum {
  kDragger1 = 0,
  kDragger2 = 1,
} dragger_index;

static ret_t range_slider_load_icon(widget_t* widget, bitmap_t* img) {
  style_t* style = widget->astyle;
  const char* image_name = style_get_str(style, STYLE_ID_ICON, NULL);
  if (image_name && widget_load_image(widget, image_name, img) == RET_OK) {
    printf("image_name: %s\r\n", image_name);
    return RET_OK;
  } else {
    return RET_FAIL;
  }
}

static uint32_t range_slider_get_bar_size(widget_t* widget) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL, 0);
  return range_slider->bar_size ? range_slider->bar_size : (widget->h >> 1);
}

static ret_t range_slider_set_bar_size(widget_t* widget, uint32_t bar_size) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL, RET_BAD_PARAMS);
  range_slider->bar_size = bar_size;
  return widget_invalidate(widget, NULL);
}

static ret_t range_slider_set_dragger_size(widget_t* widget, uint32_t dragger_size) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL, RET_BAD_PARAMS);
  range_slider->dragger_size = dragger_size;
  return widget_invalidate(widget, NULL);
}

static uint32_t range_slider_get_dragger_size(widget_t* widget) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL, RET_BAD_PARAMS);
  bitmap_t img;
  uint32_t dragger_size = range_slider->dragger_size;
  if (range_slider->auto_get_dragger_size) {
    dragger_size = range_slider_get_bar_size(widget) * 1.5f;
  }
  if (range_slider->dragger_adapt_to_icon) {
    float_t ratio = system_info()->device_pixel_ratio;
    if (range_slider_load_icon(widget, &img) == RET_OK) {
      dragger_size = img.w / ratio;
      DEBUG_RANGE_SLIDER(range_slider, "imgw: %d ratio: %.2lf dragger_size: %d\r\n", img.w, ratio,
                         dragger_size);
    }
  }
  return dragger_size;
}

static ret_t range_slider_get_prop(widget_t* widget, const char* name, value_t* v) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);
  if (tk_str_eq(name, WIDGET_PROP_MIN)) {
    value_set_double(v, range_slider->min);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    value_set_double(v, range_slider->max);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_STEP)) {
    value_set_double(v, range_slider->step);
    return RET_OK;
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_VALUE1)) {
    value_set_double(v, range_slider->value1);
    return RET_OK;
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_VALUE2)) {
    value_set_double(v, range_slider->value2);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_BAR_SIZE)) {
    value_set_uint32(v, range_slider->bar_size);
    return RET_OK;
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_DRAGGER_SIZE)) {
    value_set_uint32(v, range_slider->dragger_size);
    return RET_OK;
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_DRAGGER_ADAPT_TO_ICON)) {
    value_set_bool(v, range_slider->dragger_adapt_to_icon);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t range_slider_set_prop(widget_t* widget, const char* name, const value_t* v) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_MIN)) {
    range_slider->min = value_double(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    range_slider->max = value_double(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_STEP)) {
    range_slider->step = value_double(v);
    return RET_OK;
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_VALUE1)) {
    range_slider->value1 = value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_VALUE2)) {
    range_slider->value2 = value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_BAR_SIZE)) {
    return range_slider_set_bar_size(widget, value_uint32(v));
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_DRAGGER_SIZE)) {
    range_slider->dragger_size = value_uint32(v);
    range_slider->auto_get_dragger_size = range_slider->dragger_size == 0;
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_DRAGGER_ADAPT_TO_ICON)) {
    range_slider->dragger_adapt_to_icon = value_bool(v);
  }

  return RET_NOT_FOUND;
}

static ret_t range_slider_on_destroy(widget_t* widget) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(widget != NULL && range_slider != NULL, RET_BAD_PARAMS);

  return RET_OK;
}

static ret_t range_slider_update_dragger_rect(widget_t* widget, rect_t* dragger_rect, canvas_t* c) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL && dragger_rect != NULL && c != NULL, RET_BAD_PARAMS);
  uint32_t dragger_size = range_slider_get_dragger_size(widget);
  DEBUG_RANGE_SLIDER(range_slider, "dragger size: %d\r\n", range_slider->dragger_size);
  uint32_t range_width =
      widget->w -
      dragger_size;  // widget width substract dragger left half at the left and dragger right half at the right
  if (dragger_rect == &range_slider->dragger_rect1) {
    double fvalue =
        (range_slider->value1 - range_slider->min) / (range_slider->max - range_slider->min);
    dragger_rect->x = range_width * fvalue;
    dragger_rect->y = 0;
    dragger_rect->w = dragger_size;
    dragger_rect->h = widget->h;
  } else if (dragger_rect == &range_slider->dragger_rect2) {
    double fvalue =
        (range_slider->value2 - range_slider->min) / (range_slider->max - range_slider->min);
    dragger_rect->x = range_width * fvalue;
    dragger_rect->y = 0;
    dragger_rect->w = dragger_size;
    dragger_rect->h = widget->h;
  }

  return RET_OK;
}

static ret_t range_slider_get_bar_rect(widget_t* widget, rect_t* br, rect_t* fr1, rect_t* fr2) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL && br != NULL && fr1 != NULL && fr2 != NULL,
                       RET_BAD_PARAMS);
  uint32_t bar_size = range_slider_get_bar_size(widget);
  rect_t* dr1 = &(range_slider->dragger_rect1);
  rect_t* dr2 = &(range_slider->dragger_rect2);

  /* fill background */
  bar_size = tk_min(bar_size, widget->h);
  br->x = 0;
  br->y = (widget->h - bar_size) / 2;
  br->w = widget->w;
  br->h = bar_size;
  /* fill foreground of value1 */
  fr1->x = br->x;
  fr1->y = br->y;
  fr1->w = dr1->x - br->x;
  fr1->h = br->h;
  /* fill foreground of value2 */
  fr2->x = dr2->x;
  fr2->y = br->y;
  fr2->w = widget->w - dr2->x;
  fr2->h = br->h;
  return RET_OK;
}

static ret_t range_slider_fill_rect(widget_t* widget, canvas_t* c, rect_t* rect, bool is_fg) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL && c != NULL, RET_BAD_PARAMS);
  const char* color_key = is_fg ? STYLE_ID_FG_COLOR : STYLE_ID_BG_COLOR;
  const char* image_key = is_fg ? STYLE_ID_FG_IMAGE : STYLE_ID_BG_IMAGE;
  const char* draw_type_key = is_fg ? STYLE_ID_FG_IMAGE_DRAW_TYPE : STYLE_ID_BG_IMAGE_DRAW_TYPE;

  color_t trans = color_init(0, 0, 0, 0);

  style_t* style = widget->astyle;
  color_t color = style_get_color(style, color_key, trans);
  canvas_set_fill_color(c, color);
  canvas_fill_rect(c, rect->x, rect->y, rect->w, rect->h);
  return RET_OK;
}

static ret_t range_slider_paint_dragger(widget_t* widget, rect_t* dragger, canvas_t* c) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL && c != NULL, RET_BAD_PARAMS);
  style_t* style = widget->astyle;
  color_t trans = color_init(0, 0, 0, 0);
  if (range_slider->no_dragger_icon) {
    return RET_OK;
  }
  color_t color = style_get_color(style, STYLE_ID_BORDER_COLOR, trans);
  canvas_set_fill_color(c, color);
  canvas_fill_rect(c, dragger->x, dragger->y, dragger->w, dragger->h);

  bitmap_t img;
  if (range_slider_load_icon(widget, &img) == RET_OK) {
    canvas_draw_image_ex(c, &img, IMAGE_DRAW_ICON, dragger);
  }
  return RET_OK;
}

static ret_t range_slider_check_on_dragger_icon(widget_t* widget) {
  bitmap_t img;
  color_t color;
  style_t* style = widget->astyle;
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  color_t trans = color_init(0, 0, 0, 0);

  ret_t ret = range_slider_load_icon(widget, &img);
  color = style_get_color(style, STYLE_ID_BORDER_COLOR, trans);
  if (color.rgba.a == 0 && ret != RET_OK) {
    range_slider->no_dragger_icon = TRUE;
  } else {
    range_slider->no_dragger_icon = FALSE;
  }
  return RET_OK;
}

static ret_t range_slider_on_paint_self(widget_t* widget, canvas_t* c) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  rect_t br, fr1, fr2;

  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);
  range_slider_check_on_dragger_icon(widget);
  DEBUG_RANGE_SLIDER(range_slider, "no dragger icon: %d\r\n", range_slider->no_dragger_icon);
  range_slider_update_dragger_rect(widget, &range_slider->dragger_rect1, c);
  range_slider_update_dragger_rect(widget, &range_slider->dragger_rect2, c);

  return_value_if_fail(RET_OK == range_slider_get_bar_rect(widget, &br, &fr1, &fr2),
                       RET_BAD_PARAMS);

  range_slider_fill_rect(widget, c, &br, false);
  range_slider_fill_rect(widget, c, &fr1, true);
  range_slider_fill_rect(widget, c, &fr2, true);
  range_slider_paint_dragger(widget, &range_slider->dragger_rect1, c);
  range_slider_paint_dragger(widget, &range_slider->dragger_rect2, c);
  return RET_OK;
}

static ret_t range_slider_set_value_internal(widget_t* widget, double value, event_type_t etype,
                                             dragger_index dr_idx) {
  double step = 0;
  double offset = 0;
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(widget != NULL && range_slider != NULL, RET_BAD_PARAMS);

  step = range_slider->step;
  printf("step: %lf\r\n", step);
  value = tk_clamp(value, range_slider->min, range_slider->max);
  if (step > 0) {
    offset = value - range_slider->min;
    offset = tk_roundi(offset / step) * step;
    value = range_slider->min + offset;
  }

  if (dr_idx == kDragger1) {
    if (range_slider->value1 != value) {
      if (value >= range_slider->value2) {
        value = range_slider->value2 - step;
      }
      range_slider->value1 = value;
    }
  } else if (dr_idx == kDragger2) {
    if (range_slider->value2 != value) {
      if (value <= range_slider->value1) {
        value = range_slider->value1 + step;
      }
      range_slider->value2 = value;
    }
  } else {
    printf("invaild dragger!\r\n");
    return RET_STOP;
  }
  widget_invalidate(widget, NULL);
  return RET_OK;
}

static ret_t range_slider_change_value_by_pointer_event(widget_t* widget, pointer_event_t* evt,
                                                        dragger_index dr_idx) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(widget != NULL && range_slider != NULL, RET_BAD_PARAMS);
  double value = 0;
  point_t p = {evt->x, evt->y};
  widget_to_local(widget, &p);
  double range = range_slider->max - range_slider->min;
  value = range * p.x / widget->w;
  value += range_slider->min;
  value = tk_clamp(value, range_slider->min, range_slider->max);

  return range_slider_set_value_internal(widget, value, EVT_VALUE_CHANGING, dr_idx);
}

static ret_t range_slider_on_event(widget_t* widget, event_t* e) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(widget != NULL && range_slider != NULL, RET_BAD_PARAMS);
  pointer_event_t* evt = (pointer_event_t*)e;
  point_t p = {evt->x, evt->y};
  switch (e->type) {
    case EVT_POINTER_DOWN: {
      rect_t *dr1 = &range_slider->dragger_rect1, *dr2 = &range_slider->dragger_rect2;
      widget_to_local(widget, &p);
      if (rect_contains(dr1, p.x, p.y)) {
        DEBUG_RANGE_SLIDER(range_slider, "dragger1 down");
        range_slider->dragger1_dragging = TRUE;
        range_slider->dragger2_dragging = FALSE;
      } else if (rect_contains(dr2, p.x, p.y)) {
        DEBUG_RANGE_SLIDER(range_slider, "dragger2 down");
        range_slider->dragger2_dragging = TRUE;
        range_slider->dragger1_dragging = FALSE;
      }
      widget_grab(widget->parent, widget);
      break;
    }
    case EVT_POINTER_MOVE: {
      if (range_slider->dragger1_dragging) {
        DEBUG_RANGE_SLIDER(range_slider, "dragger1 moving");
        range_slider_change_value_by_pointer_event(widget, evt, kDragger1);
      } else if (range_slider->dragger2_dragging) {
        DEBUG_RANGE_SLIDER(range_slider, "dragger2 moving");
        range_slider_change_value_by_pointer_event(widget, evt, kDragger2);
      }
      break;
    }
    case EVT_POINTER_UP: {
      double value = 0;
      pointer_event_t* evt = (pointer_event_t*)e;
      if (range_slider->dragger1_dragging) {
        range_slider->dragger1_dragging = FALSE;
        DEBUG_RANGE_SLIDER(range_slider, "dragger1 up");
        range_slider_change_value_by_pointer_event(widget, evt, kDragger1);
      } else if (range_slider->dragger2_dragging) {
        range_slider->dragger2_dragging = FALSE;
        DEBUG_RANGE_SLIDER(range_slider, "dragger2 up");
        range_slider_change_value_by_pointer_event(widget, evt, kDragger2);
      }
      widget_ungrab(widget->parent, widget);
      break;
    }
    default:
      break;
  }
  return RET_OK;
}

const char* s_range_slider_properties[] = {NULL};

TK_DECL_VTABLE(range_slider) = {.size = sizeof(range_slider_t),
                                .type = WIDGET_TYPE_RANGE_SLIDER,
                                .clone_properties = s_range_slider_properties,
                                .persistent_properties = s_range_slider_properties,
                                .get_parent_vt = TK_GET_PARENT_VTABLE(widget),
                                .create = range_slider_create,
                                .on_paint_self = range_slider_on_paint_self,
                                .on_paint_border = widget_on_paint_null,
                                .on_paint_background = widget_on_paint_null,
                                .set_prop = range_slider_set_prop,
                                .get_prop = range_slider_get_prop,
                                .on_event = range_slider_on_event,
                                .on_destroy = range_slider_on_destroy};

widget_t* range_slider_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(range_slider), x, y, w, h);
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL, NULL);
  range_slider->min = 0;
  range_slider->max = 100;
  range_slider->value1 = 0;
  range_slider->value2 = 100;
  range_slider->no_dragger_icon = FALSE;
  range_slider->dragger1_dragging = FALSE;
  range_slider->dragger2_dragging = FALSE;
  range_slider->auto_get_dragger_size = TRUE;
  range_slider->dragger_adapt_to_icon = TRUE;
  return widget;
}

widget_t* range_slider_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, range_slider), NULL);

  return widget;
}
