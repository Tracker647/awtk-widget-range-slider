﻿/**
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
#include "base/enums.h"
#include "base/widget_vtable.h"
#include "base/canvas.h"
#include "base/vgcanvas.h"
#include "widgets/view.h"
#include "widgets/label.h"
#include "widgets/check_button.h"
#include "range_slider.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
  kDragger1 = 0,
  kDragger2 = 1,
} dragger_index;

#define RANGE_SLIDER_ALL_WIDGET_INIT()                                                        \
  range_slider_t* range_slider = RANGE_SLIDER(widget);                                        \
  return_value_if_fail(range_slider != NULL, RET_BAD_PARAMS);                                 \
  widget_t* dragger1 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_DRAGGER1, TRUE);         \
  widget_t* dragger2 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_DRAGGER2, TRUE);         \
  return_value_if_fail(dragger1 != NULL && dragger2 != NULL, RET_BAD_PARAMS);                 \
  widget_t* label_value1 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_LABEL_VALUE1, TRUE); \
  widget_t* label_value2 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_LABEL_VALUE2, TRUE); \
  return_value_if_fail(label_value1 != NULL && label_value2 != NULL, RET_BAD_PARAMS)

static ret_t range_slider_load_bg_image(widget_t* widget, bitmap_t* img) {
  style_t* style = widget->astyle;
  return_value_if_fail(style != NULL, RET_BAD_PARAMS);
  const char* image_name = style_get_str(style, STYLE_ID_BG_IMAGE, NULL);
  if (image_name && widget_load_image(widget, image_name, img) == RET_OK) {
    return RET_OK;
  } else {
    return RET_FAIL;
  }
}

static uint32_t inline range_slider_view_get_height(widget_t* widget) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL, 0);
  return range_slider->range_slider_view_height_ratio * widget->h;
}

static uint32_t inline range_slider_view_get_width(widget_t* widget) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL, 0);
  return range_slider->range_slider_view_width_ratio * widget->w;
}

static uint32_t inline range_slider_view_get_x(widget_t* widget) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL, 0);
  return widget->w / 2 - range_slider_view_get_width(widget) / 2;
}
static uint32_t inline range_slider_view_get_y(widget_t* widget) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL, 0);
  return 0;
}

static uint32_t inline range_slider_get_bar_size(widget_t* widget) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL, 0);

  return range_slider->bar_size ? range_slider->bar_size
                                : (range_slider_view_get_height(widget) >> 1);
}

static ret_t range_slider_invalidate(widget_t* widget) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(widget != NULL && range_slider != NULL, RET_BAD_PARAMS);

  return widget_invalidate(widget, NULL);
}

static ret_t range_slider_set_bar_size(widget_t* widget, uint32_t bar_size) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL, RET_BAD_PARAMS);
  range_slider->bar_size = bar_size;
  return range_slider_invalidate(widget);
}

static ret_t range_slider_set_dragger_size(widget_t* widget, uint32_t dragger_size) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL, RET_BAD_PARAMS);
  range_slider->dragger_size = dragger_size;
  range_slider->auto_get_dragger_size = range_slider->dragger_size == 0;
  return range_slider_invalidate(widget);
}

static uint32_t range_slider_get_dragger_size(widget_t* widget) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL, RET_BAD_PARAMS);
  widget_t* dragger1 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_DRAGGER1, TRUE);
  widget_t* dragger2 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_DRAGGER2, TRUE);
  return_value_if_fail(dragger1 != NULL && dragger2 != NULL, RET_BAD_PARAMS);

  bitmap_t img;
  uint32_t dragger_size = range_slider->dragger_size;
  if (range_slider->auto_get_dragger_size) {
    dragger_size = range_slider_get_bar_size(widget) * 1.5f;
  }
  if (range_slider->dragger_adapt_to_icon) {
    float_t ratio = system_info()->device_pixel_ratio;
    if (range_slider_load_bg_image(dragger1, &img) == RET_OK) {
      dragger_size = img.w / ratio;
    }
  }
  return dragger_size;
}

static ret_t range_slider_set_range_label_style(widget_t* widget, const char* style) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL, RET_BAD_PARAMS);
  widget_t* label_min = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_LABEL_MIN, TRUE);
  widget_t* label_max = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_LABEL_MAX, TRUE);
  return_value_if_fail(label_min != NULL && label_max != NULL, RET_BAD_PARAMS);
  range_slider->range_label_style = style;
  widget_use_style(label_min, style);
  widget_use_style(label_max, style);
  return RET_OK;
}

static ret_t range_slider_set_value_label_style(widget_t* widget, const char* style) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL, RET_BAD_PARAMS);
  widget_t* label_value1 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_LABEL_VALUE1, TRUE);
  widget_t* label_value2 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_LABEL_VALUE2, TRUE);
  return_value_if_fail(label_value1 != NULL && label_value2 != NULL, RET_BAD_PARAMS);
  range_slider->value_label_style = style;
  widget_use_style(label_value1, style);
  widget_use_style(label_value2, style);
  return RET_OK;
}

static ret_t range_slider_set_dragger_style(widget_t* widget, const char* style) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL, RET_BAD_PARAMS);
  widget_t* dragger1 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_DRAGGER1, TRUE);
  widget_t* dragger2 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_DRAGGER2, TRUE);
  return_value_if_fail(dragger1 != NULL && dragger2 != NULL, RET_BAD_PARAMS);

  range_slider->dragger_style = style;
  widget_use_style(dragger1, range_slider->dragger_style);
  widget_use_style(dragger2, range_slider->dragger_style);
  return RET_OK;
}

static ret_t range_slider_dragger_no_collision_assure(widget_t* widget, dragger_index dr_idx,
                                                      double* value_of_this_dragger) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(widget != NULL && range_slider != NULL, RET_BAD_PARAMS);
  double step = range_slider->step;
  if (dr_idx == kDragger1) {
    if (range_slider->no_use_second_dragger == FALSE &&
        *value_of_this_dragger >= range_slider->value2) {
      *value_of_this_dragger = range_slider->value2 - step;
    }
  } else if (dr_idx == kDragger2) {
    if (range_slider->no_use_second_dragger) return RET_STOP;
    if (*value_of_this_dragger <= range_slider->value1) {
      *value_of_this_dragger = range_slider->value1 + step;
    }
  } else {
    printf("invaild dragger!\r\n");
    return RET_FAIL;
  }
  return RET_OK;
}

static ret_t range_slider_set_value_internal(widget_t* widget, double value, event_type_t etype,
                                             dragger_index dr_idx) {
  double step = 0;
  double offset = 0;
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(widget != NULL && range_slider != NULL, RET_BAD_PARAMS);

  step = range_slider->step;
  value = tk_clamp(value, range_slider->min, range_slider->max);
  if (step > 0) {
    offset = value - range_slider->min;
    offset = tk_roundi(offset / step) * step;
    value = range_slider->min + offset;
  }

  range_slider_dragger_no_collision_assure(widget, dr_idx, &value);

  if (dr_idx == kDragger1) {
    if (range_slider->value1 != value) {
      value_change_event_t evt;
      value_change_event_init(&evt, etype, widget);
      value_set_double(&(evt.old_value), range_slider->value1);
      value_set_double(&(evt.new_value), value);
      range_slider->value1 = value;
      widget_dispatch(widget, (event_t*)&evt);
    }
  } else if (dr_idx == kDragger2) {
    if (range_slider->value2 != value) {
      value_change_event_t evt;
      value_change_event_init(&evt, etype, widget);
      value_set_double(&(evt.old_value), range_slider->value2);
      value_set_double(&(evt.new_value), value);
      range_slider->value2 = value;
      widget_dispatch(widget, (event_t*)&evt);
    }
  } else {
    printf("invaild dragger!\r\n");
    return RET_STOP;
  }
  range_slider_invalidate(widget);
  return RET_OK;
}

ret_t range_slider_set_value(widget_t* widget, dragger_index dr_idx, double value) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL, RET_BAD_PARAMS);

  if (dr_idx == kDragger1 && range_slider->dragger1_dragging) {
    return RET_BUSY;
  }
  if (dr_idx == kDragger2 && range_slider->dragger2_dragging) {
    return RET_BUSY;
  }

  double* pvalue = (dr_idx == kDragger1) ? &range_slider->value1 : &range_slider->value2;
  event_type_t evt_value_will_change =
      (dr_idx == kDragger1) ? EVT_VALUE1_WILL_CHANGE : EVT_VALUE2_WILL_CHANGE;
  event_type_t evt_value_changed = (dr_idx == kDragger1) ? EVT_VALUE1_CHANGED : EVT_VALUE2_CHANGED;

  if (*pvalue != value) {
    value_change_event_t evt;
    value_change_event_init(&evt, evt_value_will_change, widget);
    value_set_uint32(&(evt.old_value), *pvalue);
    value_set_uint32(&(evt.new_value), value);
    if (widget_dispatch(widget, (event_t*)&evt) == RET_STOP) {
      return RET_OK;
    }
    return range_slider_set_value_internal(widget, value, evt_value_changed, dr_idx);
  }
  return RET_OK;
}

ret_t range_slider_inc(widget_t* widget, dragger_index dr_idx) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL, RET_BAD_PARAMS);
  double delta = range_slider->step ? range_slider->step : 1;
  double* pvalue = (dr_idx == kDragger1) ? &(range_slider->value1) : &(range_slider->value2);

  double new_value = *pvalue + delta;
  if (new_value < range_slider->min) {
    new_value = range_slider->min;
  }
  if (new_value > range_slider->max) {
    new_value = range_slider->max;
  }
  return range_slider_set_value(widget, dr_idx, new_value);
}

ret_t range_slider_dec(widget_t* widget, dragger_index dr_idx) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL, RET_BAD_PARAMS);
  double delta = range_slider->step ? range_slider->step : 1;
  double* pvalue = (dr_idx == kDragger1) ? &(range_slider->value1) : &(range_slider->value2);
  double new_value = *pvalue - delta;
  if (new_value < range_slider->min) {
    new_value = range_slider->min;
  }
  if (new_value > range_slider->max) {
    new_value = range_slider->max;
  }
  return range_slider_set_value(widget, dr_idx, new_value);
}

static ret_t range_slider_dispatch_value_change_event(widget_t* widget, event_type_t evt_type,
                                                      dragger_index dr_idx, double value) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL, RET_BAD_PARAMS);

  double* pvalue = (dr_idx == kDragger1) ? &range_slider->value1 : &range_slider->value2;
  value_change_event_t evt;
  value_change_event_init(&evt, evt_type, widget);
  value_set_uint32(&(evt.old_value), *pvalue);
  value_set_uint32(&(evt.new_value), value);
  if (widget_dispatch(widget, (event_t*)&evt) == RET_STOP) {
    return RET_OK;
  }
  return RET_OK;
}

static ret_t range_slider_set_no_use_second_dragger(widget_t* widget,
                                                    bool_t no_use_second_dragger) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL, RET_BAD_PARAMS);
  widget_t* dragger1 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_DRAGGER1, TRUE);
  widget_t* dragger2 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_DRAGGER2, TRUE);
  return_value_if_fail(dragger1 != NULL && dragger2 != NULL, RET_BAD_PARAMS);
  widget_t* label_value2 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_LABEL_VALUE2, TRUE);
  return_value_if_fail(label_value2 != NULL, RET_BAD_PARAMS);

  range_slider->no_use_second_dragger = no_use_second_dragger;
  if (range_slider->no_use_second_dragger) {
    widget_set_visible(dragger2, FALSE);
    if (range_slider->value_label_visible) {
      widget_set_visible(label_value2, FALSE);
    }
  } else {
    widget_set_visible(dragger2, TRUE);
    if (range_slider->value_label_visible) {
      widget_set_visible(label_value2, TRUE);
    }
    range_slider_set_value(widget, kDragger2, range_slider->value2);
  }

  return range_slider_invalidate(widget);
}

static ret_t range_slider_set_range_label_visible(widget_t* widget, bool_t visible) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(widget != NULL && range_slider != NULL, RET_BAD_PARAMS);
  widget_t* label_min = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_LABEL_MIN, TRUE);
  widget_t* label_max = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_LABEL_MAX, TRUE);
  return_value_if_fail(label_min != NULL && label_max != NULL, RET_BAD_PARAMS);
  range_slider->range_label_visible = visible;
  widget_set_visible(label_min, visible);
  widget_set_visible(label_max, visible);
  return range_slider_invalidate(widget);
}

static ret_t range_slider_set_value_label_visible(widget_t* widget, bool_t visible) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(widget != NULL && range_slider != NULL, RET_BAD_PARAMS);
  widget_t* label_value1 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_LABEL_VALUE1, TRUE);
  widget_t* label_value2 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_LABEL_VALUE2, TRUE);
  return_value_if_fail(label_value1 != NULL && label_value2 != NULL, RET_BAD_PARAMS);
  range_slider->value_label_visible = visible;
  widget_set_visible(label_value1, visible);
  if (range_slider->no_use_second_dragger) {
    widget_set_visible(label_value2, FALSE);
  } else {
    widget_set_visible(label_value2, visible);
  }
  return range_slider_invalidate(widget);
}

static ret_t range_slider_set_focusable(widget_t* widget, bool_t focusable) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL, RET_BAD_PARAMS);
  widget_t* dragger1 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_DRAGGER1, TRUE);
  widget_t* dragger2 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_DRAGGER2, TRUE);
  return_value_if_fail(dragger1 != NULL && dragger2 != NULL, RET_BAD_PARAMS);
  widget_set_focusable(dragger1, focusable);
  widget_set_focusable(dragger2, focusable);
  return RET_OK;
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
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_DRAGGER_STYLE)) {
    value_set_str(v, range_slider->dragger_style);
    return RET_OK;
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_NO_USE_SECOND_DRAGGER)) {
    value_set_bool(v, range_slider->no_use_second_dragger);
    return RET_OK;
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_RANGE_LABEL_STYLE)) {
    value_set_str(v, range_slider->range_label_style);
    return RET_OK;
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_VALUE_LABEL_STYLE)) {
    value_set_str(v, range_slider->value_label_style);
    return RET_OK;
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_RANGE_LABEL_VISIBLE)) {
    value_set_bool(v, range_slider->range_label_visible);
    return RET_OK;
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_VALUE_LABEL_VISIBLE)) {
    value_set_bool(v, range_slider->value_label_visible);
    return RET_OK;
  } else if (tk_str_eq(name, RANGE_SLIDER_SUB_WIDGET_RANGE_LABEL_GAP)) {
    value_set_uint32(v, range_slider->range_label_gap);
    return RET_OK;
  } else if (tk_str_eq(name, RANGE_SLIDER_SUB_WIDGET_VALUE_LABEL_GAP)) {
    value_set_uint32(v, range_slider->value_label_gap);
    return RET_OK;
  } else if (tk_str_eq(name, RANGE_SLIDER_SUB_WIDGET_VALUE_LABEL_WIDTH)) {
    value_set_uint32(v, range_slider->value_label_width);
    return RET_OK;
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_FOCUSABLE)) {
    value_set_bool(v, range_slider->range_slider_focusable);
    return RET_OK;
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_VIEW_WIDTH_RATIO)) {
    value_set_double(v, range_slider->range_slider_view_width_ratio);
    return RET_OK;
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_VIEW_HEIGHT_RATIO)) {
    value_set_double(v, range_slider->range_slider_view_height_ratio);
    return RET_OK;
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_VALUE_MIN_SHOW_ONE_POINT)) {
    value_set_bool(v, range_slider->value_min_show_one_point);
  }
  return RET_NOT_FOUND;
}

static ret_t range_slider_set_prop(widget_t* widget, const char* name, const value_t* v) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);
  widget_t* dragger1 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_DRAGGER1, TRUE);
  widget_t* dragger2 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_DRAGGER2, TRUE);
  return_value_if_fail(dragger1 != NULL && dragger2 != NULL, RET_BAD_PARAMS);
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
    range_slider_set_value(widget, kDragger1, value_double(v));
    return RET_OK;
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_VALUE2)) {
    range_slider_set_value(widget, kDragger2, value_double(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_BAR_SIZE)) {
    return range_slider_set_bar_size(widget, value_uint32(v));
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_DRAGGER_SIZE)) {
    return range_slider_set_dragger_size(widget, value_uint32(v));
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_DRAGGER_ADAPT_TO_ICON)) {
    range_slider->dragger_adapt_to_icon = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_DRAGGER_STYLE)) {
    return range_slider_set_dragger_style(widget, value_str(v));
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_NO_USE_SECOND_DRAGGER)) {
    return range_slider_set_no_use_second_dragger(widget, value_bool(v));
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_RANGE_LABEL_VISIBLE)) {
    return range_slider_set_range_label_visible(widget, value_bool(v));
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_VALUE_LABEL_VISIBLE)) {
    return range_slider_set_value_label_visible(widget, value_bool(v));
    return RET_OK;
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_RANGE_LABEL_STYLE)) {
    return range_slider_set_range_label_style(widget, value_str(v));
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_VALUE_LABEL_STYLE)) {
    return range_slider_set_value_label_style(widget, value_str(v));
  } else if (tk_str_eq(name, RANGE_SLIDER_SUB_WIDGET_RANGE_LABEL_GAP)) {
    range_slider->range_label_gap = value_uint32(v);
    return RET_OK;
  } else if (tk_str_eq(name, RANGE_SLIDER_SUB_WIDGET_VALUE_LABEL_GAP)) {
    range_slider->value_label_gap = value_uint32(v);
    return RET_OK;
  } else if (tk_str_eq(name, RANGE_SLIDER_SUB_WIDGET_VALUE_LABEL_WIDTH)) {
    range_slider->value_label_width = value_uint32(v);
    return RET_OK;
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_FOCUSABLE)) {
    bool_t flag = value_bool(v);
    range_slider->range_slider_focusable = flag;
    range_slider_set_focusable(widget, flag);
    return RET_OK;
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_VIEW_WIDTH_RATIO)) {
    range_slider->range_slider_view_width_ratio = value_double(v);
    return RET_OK;
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_VIEW_HEIGHT_RATIO)) {
    range_slider->range_slider_view_height_ratio = value_double(v);
    return RET_OK;
  } else if (tk_str_eq(name, RANGE_SLIDER_PROP_VALUE_MIN_SHOW_ONE_POINT)) {
    range_slider->value_min_show_one_point = value_bool(v);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t on_dragger_key_down(void* ctx, event_t* e) {
  range_slider_t* range_slider = RANGE_SLIDER(ctx);
  return_value_if_fail(ctx != NULL && e != NULL && e->target != NULL, RET_BAD_PARAMS);
  widget_t* dragger = WIDGET(e->target);
  return_value_if_fail(dragger != NULL, RET_BAD_PARAMS);
  bool dragger_selected = widget_get_prop_bool(dragger, "value", false);

  key_event_t* evt = (key_event_t*)e;
  ret_t ret = RET_OK;
  /* 按钮enter默认不会冒泡到父控件，需手动处理 */
  widget_dispatch((widget_t*)range_slider, e);
  if (dragger_selected && (evt->key == TK_KEY_LEFT || evt->key == TK_KEY_RIGHT)) {
    ret = RET_STOP;  //用于阻止后继的焦点切换操作
  }
  if (dragger_selected && evt->key == TK_KEY_ESCAPE) {
    ret = RET_STOP;
  }
  return ret;
}

static ret_t range_slider_on_destroy(widget_t* widget) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(widget != NULL && range_slider != NULL, RET_BAD_PARAMS);

  return RET_OK;
}

static xy_t range_slider_value_to_dragger_x(widget_t* widget, double value) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL, RET_BAD_PARAMS);
  uint32_t dragger_size = range_slider_get_dragger_size(widget);
  double fvalue = (value - range_slider->min) / (range_slider->max - range_slider->min);
  int32_t margin =
      range_slider->no_dragger_icon ? 0 : style_get_int(widget->astyle, STYLE_ID_MARGIN, 0);

  xy_t res_x = 0;
  if (range_slider->no_dragger_icon) {
    res_x = range_slider_view_get_width(widget) * fvalue - (dragger_size >> 1);
  } else {
    res_x = margin + (range_slider_view_get_width(widget) - dragger_size - (margin << 1)) * fvalue;
  }

  return res_x;
}

static ret_t range_slider_update_dragger_rect(widget_t* widget, widget_t* dragger) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL && dragger != NULL, RET_BAD_PARAMS);
  widget_t* dragger1 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_DRAGGER1, TRUE);
  widget_t* dragger2 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_DRAGGER2, TRUE);
  return_value_if_fail(dragger1 != NULL && dragger2 != NULL, RET_BAD_PARAMS);
  uint32_t dragger_size = range_slider_get_dragger_size(widget);

  if (dragger == dragger1) {
    dragger->x = range_slider_value_to_dragger_x(widget, range_slider->value1);
  } else if (dragger == dragger2) {
    dragger->x = range_slider_value_to_dragger_x(widget, range_slider->value2);
  }

  dragger->y = 0;
  dragger->w = dragger_size;
  dragger->h = range_slider_view_get_height(widget);
  return RET_OK;
}

static ret_t range_slider_get_bar_rect(widget_t* widget, rect_t* br, rect_t* fr1, rect_t* fr2) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL && br != NULL && fr1 != NULL && fr2 != NULL,
                       RET_BAD_PARAMS);
  widget_t* dragger1 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_DRAGGER1, TRUE);
  widget_t* dragger2 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_DRAGGER2, TRUE);
  return_value_if_fail(dragger1 != NULL && dragger2 != NULL, RET_BAD_PARAMS);
  widget_t* range_slider_view = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_VIEW, TRUE);
  return_value_if_fail(range_slider_view != NULL, RET_BAD_PARAMS);

  uint32_t bar_size = range_slider_get_bar_size(widget);
  rect_t* dr1 = (rect_t*)dragger1;
  rect_t* dr2 = (rect_t*)dragger2;

  /* fill background */
  bar_size = tk_min(bar_size, range_slider_view->h);
  br->x = 0;
  br->h = bar_size;
  br->w = range_slider_view->w;
  br->y = (range_slider_view->h - bar_size) / 2;

  /* fill foreground of value1 */
  fr1->x = br->x;
  fr1->y = br->y;
  fr1->w = dr1->x - br->x + (dr1->w >> 1);
  fr1->h = br->h;
  /* fill foreground of value2 */
  fr2->x = dr2->x + (dr2->w >> 1);
  fr2->y = br->y;
  fr2->w = range_slider_view->w - dr2->x - (dr2->w >> 1);
  fr2->h = br->h;

  br->x += range_slider_view_get_x(widget);
  fr1->x += range_slider_view_get_x(widget);
  fr2->x += range_slider_view_get_x(widget);
  return RET_OK;
}

static ret_t range_slider_fill_rect(widget_t* widget, canvas_t* c, rect_t* rect, rect_t* br,
                                    bool is_fg) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL && c != NULL, RET_BAD_PARAMS);
  const char* color_key = is_fg ? STYLE_ID_FG_COLOR : STYLE_ID_BG_COLOR;

  style_t* style = widget->astyle;
  color_t trans = color_init(0, 0, 0, 0);
  uint32_t radius = style_get_int(style, STYLE_ID_ROUND_RADIUS, 0);

  color_t color = style_get_color(style, color_key, trans);
  if (color.rgba.a && rect->w > 0 && rect->h > 0) {
    canvas_set_fill_color(c, color);
    if (radius > 3) {
      ret_t ret = canvas_fill_rounded_rect(c, rect, br, &color, radius);
      // 对于底层背景bar, 若圆角直径大于 rect 矩形的宽高, 会返回RET_FAIL, 此时需要按无圆角矩形的方法填充
      if (ret != RET_OK) {
        canvas_fill_rect(c, rect->x, rect->y, rect->w, rect->h);
      }
    } else {
      canvas_fill_rect(c, rect->x, rect->y, rect->w, rect->h);
    }
  }
  return RET_OK;
}

static ret_t range_slider_check_on_dragger_icon(widget_t* widget) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);
  widget_t* dragger = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_DRAGGER1, TRUE);
  return_value_if_fail(dragger != NULL, RET_BAD_PARAMS);
  widget_t* range_slider_view = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_VIEW, TRUE);
  return_value_if_fail(range_slider_view != NULL, RET_BAD_PARAMS);

  bitmap_t img;
  style_t* style = dragger->astyle;
  color_t trans = color_init(0, 0, 0, 0);
  ret_t ret = range_slider_load_bg_image(dragger, &img);
  color_t color = style_get_color(style, STYLE_ID_BG_COLOR, trans);
  if (color.rgba.a == 0 && ret != RET_OK) {
    range_slider->no_dragger_icon = TRUE;
  } else {
    range_slider->no_dragger_icon = FALSE;
  }
  return RET_OK;
}

static ret_t range_slider_update_label(widget_t* widget, rect_t* fr1, rect_t* fr2) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(widget != NULL && range_slider != NULL, RET_BAD_PARAMS);
  widget_t* label_min = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_LABEL_MIN, TRUE);
  widget_t* label_max = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_LABEL_MAX, TRUE);
  widget_t* label_value1 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_LABEL_VALUE1, TRUE);
  widget_t* label_value2 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_LABEL_VALUE2, TRUE);
  return_value_if_fail(
      label_min != NULL && label_max != NULL && label_value1 != NULL && label_value2 != NULL,
      RET_BAD_PARAMS);
  widget_t* range_slider_view = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_VIEW, TRUE);
  return_value_if_fail(range_slider_view != NULL, RET_BAD_PARAMS);

  char textbuf[10] = {0};
  sprintf(textbuf, "%.f", range_slider->value1);
  widget_set_text_utf8(label_value1, textbuf);
  memset(textbuf, 0, sizeof(textbuf));

  sprintf(textbuf, "%.f", range_slider->value2);
  widget_set_text_utf8(label_value2, textbuf);
  memset(textbuf, 0, sizeof(textbuf));

  uint32_t range_label_width = (widget->w - range_slider_view_get_width(widget)) / 2 - range_slider->range_label_gap * 2;
  uint32_t range_label_height = range_slider_view_get_height(widget);
  uint32_t value_label_width = range_slider->value_label_width;
  uint32_t value_label_gap = range_slider->value_label_gap;
  uint32_t value_label_height = widget->h - range_slider_view->h - value_label_gap;

  widget_move_resize(label_min, 0, 0, range_label_width, range_label_height);
  widget_move_resize(label_max, widget->w - range_label_width, 0, range_label_width,
                     range_label_height);
  widget_move_resize(label_value1, fr1->x + fr1->w - value_label_width / 2,
                     widget->h - value_label_height, value_label_width, value_label_height);
  widget_move_resize(label_value2, fr2->x - value_label_width / 2, widget->h - value_label_height,
                     value_label_width, value_label_height);

  return RET_OK;
}

static ret_t range_slider_on_paint_self(widget_t* widget, canvas_t* c) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL, RET_BAD_PARAMS);
  rect_t br, fr1, fr2;
  widget_t* dragger1 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_DRAGGER1, TRUE);
  widget_t* dragger2 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_DRAGGER2, TRUE);
  return_value_if_fail(dragger1 != NULL && dragger2 != NULL, RET_BAD_PARAMS);
  widget_t* range_slider_view = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_VIEW, TRUE);
  return_value_if_fail(range_slider_view != NULL, RET_BAD_PARAMS);

  range_slider_check_on_dragger_icon(widget);
  range_slider_update_dragger_rect(widget, dragger1);
  range_slider_update_dragger_rect(widget, dragger2);

  return_value_if_fail(RET_OK == range_slider_get_bar_rect(widget, &br, &fr1, &fr2),
                       RET_BAD_PARAMS);
  range_slider_fill_rect(widget, c, &br, NULL, false);
  range_slider_fill_rect(widget, c, &fr1, &br, true);
  if (range_slider->no_use_second_dragger == FALSE)
    range_slider_fill_rect(widget, c, &fr2, &br, true);

  range_slider_update_label(widget, &fr1, &fr2);
  return RET_OK;
}

static ret_t range_slider_change_value_by_pointer_event(widget_t* widget, pointer_event_t* evt,
                                                        dragger_index dr_idx) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(widget != NULL && range_slider != NULL, RET_BAD_PARAMS);
  widget_t* range_slider_view = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_VIEW, TRUE);
  return_value_if_fail(range_slider_view != NULL, RET_BAD_PARAMS);

  double value = 0;
  point_t p = {evt->x, evt->y};
  widget_to_local(range_slider_view, &p);
  double range = range_slider->max - range_slider->min;
  uint32_t dragger_size = range_slider_get_dragger_size(widget);
  int32_t margin =
      range_slider->no_dragger_icon ? 0 : style_get_int(widget->astyle, STYLE_ID_MARGIN, 0);
  if (range_slider->no_dragger_icon) {
    value = range * p.x / range_slider_view_get_width(widget);
  } else {
    int32_t half_dragger_size = dragger_size >> 1;
    //align mouse to the middle of the dragger
    value =
        tk_clamp(range * (p.x - half_dragger_size - margin) /
                     (int32_t)(range_slider_view_get_width(widget) - dragger_size - (margin << 1)),
                 0.0, range);
  }
  value += range_slider->min;
  value = tk_clamp(value, range_slider->min, range_slider->max);
  event_type_t evt_value_changing =
      (dr_idx == kDragger1) ? EVT_VALUE1_CHANGING : EVT_VALUE2_CHANGING;
  return range_slider_set_value_internal(widget, value, evt_value_changing, dr_idx);
}

static ret_t range_slider_on_event(widget_t* widget, event_t* e) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL, RET_BAD_PARAMS);
  widget_t* dragger1 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_DRAGGER1, TRUE);
  widget_t* dragger2 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_DRAGGER2, TRUE);
  widget_t* range_slider_view = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_VIEW, TRUE);
  if (!(dragger1 != NULL && dragger2 != NULL && range_slider_view != NULL)) {
    /* 子控件还未初始化完毕，先返回 */
    return RET_BAD_PARAMS;
  }

  pointer_event_t* evt = (pointer_event_t*)e;
  point_t p = {evt->x, evt->y};
  ret_t ret = RET_OK;
  switch (e->type) {
    case EVT_POINTER_DOWN: {
      rect_t *dr1 = (rect_t*)dragger1, *dr2 = (rect_t*)dragger2;
      widget_to_local(range_slider_view, &p);
      if (rect_contains(dr1, p.x, p.y)) {
        range_slider_dispatch_value_change_event(widget, EVT_VALUE1_WILL_CHANGE, kDragger1, 0);
        range_slider->dragger1_dragging = TRUE;
        range_slider->dragger2_dragging = FALSE;
        widget_grab(range_slider_view, dragger1);
      } else if (rect_contains(dr2, p.x, p.y)) {
        range_slider_dispatch_value_change_event(widget, EVT_VALUE2_WILL_CHANGE, kDragger2, 0);
        range_slider->dragger2_dragging = TRUE;
        range_slider->dragger1_dragging = FALSE;
        widget_grab(range_slider_view, dragger2);
      }
      break;
    }
    case EVT_POINTER_MOVE: {
      if (range_slider->dragger1_dragging) {
        range_slider_change_value_by_pointer_event(widget, evt, kDragger1);
      } else if (range_slider->dragger2_dragging) {
        range_slider_change_value_by_pointer_event(widget, evt, kDragger2);
      }
      break;
    }
    case EVT_POINTER_UP: {
      pointer_event_t* evt = (pointer_event_t*)e;
      if (range_slider->dragger1_dragging) {
        range_slider->dragger1_dragging = FALSE;
        range_slider_change_value_by_pointer_event(widget, evt, kDragger1);
        range_slider_dispatch_value_change_event(widget, EVT_VALUE1_CHANGED, kDragger1, 0);
        widget_ungrab(range_slider_view, dragger1);
      } else if (range_slider->dragger2_dragging) {
        range_slider->dragger2_dragging = FALSE;
        range_slider_change_value_by_pointer_event(widget, evt, kDragger2);
        range_slider_dispatch_value_change_event(widget, EVT_VALUE2_CHANGED, kDragger2, 0);
        widget_ungrab(range_slider_view, dragger2);
      }
      break;
    }
    case EVT_POINTER_LEAVE:
      widget_set_prop_bool(dragger1, "value", FALSE);
      widget_set_prop_bool(dragger2, "value", FALSE);
      break;
    case EVT_KEY_DOWN:{
      key_event_t* evt = (key_event_t*)e;
      const key_type_value_t* kvt = keys_type_find_by_value(evt->key);
      bool dragger1_selected = widget_get_prop_bool(dragger1, "value", false);
      bool dragger2_selected = widget_get_prop_bool(dragger2, "value", false);

      if (dragger1_selected) {
        if (evt->key == TK_KEY_LEFT) {
          range_slider_dec(widget, kDragger1);
          ret = RET_STOP;  //用于阻止后继的焦点切换操作，确保按键控制正常执行,下面ret = RET_STOP同理
        } else if (evt->key == TK_KEY_RIGHT) {
          range_slider_inc(widget, kDragger1);
          ret = RET_STOP;
        } else if (evt->key == TK_KEY_ESCAPE) {
          widget_set_prop_bool(dragger1, "value", false);
        }
      } else if (dragger2_selected) {
        if (evt->key == TK_KEY_LEFT) {
          range_slider_dec(widget, kDragger2);
          ret = RET_STOP;
        } else if (evt->key == TK_KEY_RIGHT) {
          range_slider_inc(widget, kDragger2);
          ret = RET_STOP;
        } else if (evt->key == TK_KEY_ESCAPE) {
          widget_set_prop_bool(dragger2, "value", false);
        }
      }
      break;
    }
    case EVT_KEY_UP:{
      break;
    }
    default:
      break;
  }
  return ret;
}

static ret_t range_slider_on_layout_children(widget_t* widget) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(widget != NULL && range_slider != NULL, RET_BAD_PARAMS);
  widget_t* label_min = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_LABEL_MIN, TRUE);
  widget_t* label_max = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_LABEL_MAX, TRUE);
  return_value_if_fail(label_min != NULL && label_max != NULL,RET_BAD_PARAMS);
  widget_t* dragger1 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_DRAGGER1, TRUE);
  widget_t* dragger2 = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_DRAGGER2, TRUE);
  return_value_if_fail(dragger1 != NULL && dragger2 != NULL, RET_BAD_PARAMS);
  widget_t* range_slider_view = widget_lookup(widget, RANGE_SLIDER_SUB_WIDGET_VIEW, TRUE);
  return_value_if_fail(range_slider_view != NULL, RET_BAD_PARAMS);

  widget_set_style_str(label_min, "normal:text_align_h", "right");
  widget_set_style_str(label_min, "focused:text_align_h", "right");
  widget_set_style_str(label_min, "disabled:text_align_h", "right");
  widget_set_style_str(label_max, "normal:text_align_h", "left");
  widget_set_style_str(label_max, "focused:text_align_h", "left");
  widget_set_style_str(label_max, "disabled:text_align_h", "left");

  char textbuf[10] = {0};
  if (range_slider->value_min_show_one_point) {
    sprintf(textbuf, "%.1f", range_slider->min);
  } else {
    sprintf(textbuf, "%.f", range_slider->min);
  }
  widget_set_text_utf8(label_min, textbuf);
  memset(textbuf, 0, sizeof(textbuf));

  sprintf(textbuf, "%.f", range_slider->max);
  widget_set_text_utf8(label_max, textbuf);
  memset(textbuf, 0, sizeof(textbuf));

  widget_move_resize(range_slider_view, widget->w / 2 - range_slider_view_get_width(widget) / 2, 0,
                     range_slider_view_get_width(widget), range_slider_view_get_height(widget));

  uint32_t range_label_width =
      (widget->w - range_slider_view_get_width(widget)) / 2 - range_slider->range_label_gap * 2;
  uint32_t range_label_height = range_slider_view_get_height(widget);

  widget_move_resize(label_min, 0, 0, range_label_width, range_label_height);
  widget_move_resize(label_max, widget->w - range_label_width, 0, range_label_width,
                     range_label_height);

  return RET_OK;
}

const char* s_range_slider_properties[] = {NULL};

TK_DECL_VTABLE(range_slider) = {.size = sizeof(range_slider_t),
                                .type = WIDGET_TYPE_RANGE_SLIDER,
                                .clone_properties = s_range_slider_properties,
                                .persistent_properties = s_range_slider_properties,
                                .get_parent_vt = TK_GET_PARENT_VTABLE(widget),
                                .create = range_slider_create,
                                .on_layout_children = range_slider_on_layout_children,
                                .on_paint_self = range_slider_on_paint_self,
                                .on_paint_background = widget_on_paint_null,
                                .set_prop = range_slider_set_prop,
                                .get_prop = range_slider_get_prop,
                                .on_event = range_slider_on_event,
                                .on_destroy = range_slider_on_destroy};

ret_t range_slider_init_default_style_when_reload(widget_t* widget) {
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(widget != NULL && range_slider != NULL, RET_BAD_PARAMS);

  /* create时刻没有触发set_value更新，必须先初始化一个值 */
  range_slider->min = 0;
  range_slider->max = 100;
  range_slider->value1 = 20;
  range_slider->value2 = 50;

  return RET_OK;
}

widget_t* range_slider_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(range_slider), x, y, w, h);
  range_slider_t* range_slider = RANGE_SLIDER(widget);
  return_value_if_fail(range_slider != NULL, NULL);
  widget_t* range_slider_view = view_create(widget, 0, 0, 0, 0);
  return_value_if_fail(range_slider_view != NULL, NULL);

  widget_t* dragger1 = check_button_create_radio(range_slider_view, 0, 0, 0, 0);
  widget_t* dragger2 = check_button_create_radio(range_slider_view, 0, 0, 0, 0);
  return_value_if_fail(dragger1 != NULL && dragger2 != NULL, NULL);

  widget_t* label_min = label_create(widget, 0, 0, 0, 0);
  widget_t* label_max = label_create(widget, 0, 0, 0, 0);
  widget_t* label_value1 = label_create(widget, 0, 0, 0, 0);
  widget_t* label_value2 = label_create(widget, 0, 0, 0, 0);
  return_value_if_fail(
      label_min != NULL && label_max != NULL && label_value1 != NULL && label_value2 != NULL, NULL);

  widget_set_name(range_slider_view, RANGE_SLIDER_SUB_WIDGET_VIEW);
  widget_set_name(dragger1, RANGE_SLIDER_SUB_WIDGET_DRAGGER1);
  widget_set_name(dragger2, RANGE_SLIDER_SUB_WIDGET_DRAGGER2);
  widget_set_name(label_min, RANGE_SLIDER_SUB_WIDGET_LABEL_MIN);
  widget_set_name(label_max, RANGE_SLIDER_SUB_WIDGET_LABEL_MAX);
  widget_set_name(label_value1, RANGE_SLIDER_SUB_WIDGET_LABEL_VALUE1);
  widget_set_name(label_value2, RANGE_SLIDER_SUB_WIDGET_LABEL_VALUE2);

  widget_on(dragger1, EVT_KEY_DOWN, on_dragger_key_down, range_slider);
  widget_on(dragger2, EVT_KEY_DOWN, on_dragger_key_down, range_slider);
  range_slider_init_default_style_when_reload(widget);
  return widget;
}

widget_t* range_slider_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, range_slider), NULL);

  return widget;
}
