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
#include "range_slider_register.h"
#include "base/widget_factory.h"
#include "range_slider/range_slider.h"

ret_t range_slider_register(void) {
  return widget_factory_register(widget_factory(), WIDGET_TYPE_RANGE_SLIDER, range_slider_create);
}

const char* range_slider_supported_render_mode(void) {
  return "OpenGL|AGGE-BGR565|AGGE-BGRA8888|AGGE-MONO";
}
