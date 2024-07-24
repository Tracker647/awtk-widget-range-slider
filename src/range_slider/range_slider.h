/**
 * File:   range_slider.h
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


#ifndef TK_RANGE_SLIDER_H
#define TK_RANGE_SLIDER_H

#include "base/widget.h"
#include "widgets/dragger.h"

BEGIN_C_DECLS
typedef enum {
  kRangeSilderFocus_view = 0,
  kRangeSilderFocus_dragger1,
  kRangeSilderFocus_dragger2,
  kRangeSilderWillMove_dragger1,
  kRangeSilderWillMove_dragger2
} enFocusState;

/**
 * @class range_slider_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 
 * 在xml中使用"range\_slider"标签创建控件。如：
 *
 * ```xml
 * <!-- ui -->
 * <range_slider name="range_slider1" value1="25" value2="75" min="1" max="100" w="268" h="28"/>
 * ```
 *
 * 可用通过style来设置控件的显示风格，如字体的大小和颜色等等。如：
 * 
 * ```xml
 * <!-- style -->
 * <range_slider>
 *   <style name="default">
 *     <normal bg_color="#A5A5A5" border_color="#E7E6E6" fg_color="#338FFF" font_size="32" selected_bg_color="#A5A5A5" selected_fg_color="#00B0F0" text_color="#000000"/>
 *   </style>
 * </range_slider>
 * ```
 */
typedef struct _range_slider_t {
  widget_t widget;
  /**
   * @property {double} value1
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 值1，最左为起点, 注意：value1和value2设值不能相跨！
   */
  double value1;
  /**
   * @property {double} value2
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 值2, 最右为起点, 注意：value1和value2设值不能相跨！
   */
  double value2;
  /**
   * @property {double} min
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 最小值。
   */
  double min;

  /**
   * @property {double} max
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 最大值。
   */
  double max;

  /**
   * @property {double} step
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 拖动的最小单位。
   */
  double step;

  /**
   * @property {uint32_t} bar_size
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 轴的宽度或高度（单位：像素），为0表示为控件的宽度或高度的一半，缺省为0。
   */
  uint32_t bar_size;

  /**
   * @property {char*} dragger_style
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 渲染dragger的style名称,默认为default。
   */
  char* dragger_style;

  /**
   * @property {uint32_t} dragger_size
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 滑块的宽度或高度（单位：像素），缺省为 bar_size * 1.5。
   */
  uint32_t dragger_size;

  /**
   * @property {bool_t} dragger_adapt_to_icon
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 滑块的宽度或高度是否与icon适应，缺省为true。
   */
  bool_t dragger_adapt_to_icon;

  /**
   * @property {bool_t} no_use_second_dragger
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 禁止使用第二个dragger，缺省为false。
   */
  bool_t no_use_second_dragger;

  /**
   * @property {bool_t} range_label_visible
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否在控件widget左右显示min和max的label，缺省为true
   */
  bool_t range_slider_visible;

  /**
   * @property {bool_t} value_label_visible
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否在控件widget下方显示value1和value2的label，缺省为true
   */
  bool_t value_label_visible;


  /* private */
  enFocusState prev_key_state; 
  enFocusState curr_key_state; /* 键盘控制模式下的状态 */ 
  bool_t no_dragger_icon;
  bool_t dragger1_dragging;
  bool_t dragger2_dragging;
  bool_t auto_get_dragger_size;
} range_slider_t;

/**
 * @event {value_change_event_t} EVT_VALUE1_WILL_CHANGE
 * 值1即将改变事件。
 */

/**
 * @event {value_change_event_t} EVT_VALUE1_CHANGING
 * 值1正在改变事件(拖动中)。
 */

/**
 * @event {value_change_event_t} EVT_VALUE1_CHANGED
 * 值1改变事件。
 */

/**
 * @event {value_change_event_t} EVT_VALUE2_WILL_CHANGE
 * 值2即将改变事件。
 */

/**
 * @event {value_change_event_t} EVT_VALUE2_CHANGING
 * 值2正在改变事件(拖动中)。
 */

/**
 * @event {value_change_event_t} EVT_VALUE2_CHANGED
 * 值2改变事件。
 */

#define EVT_VALUE1_WILL_CHANGE (EVT_USER_START + 1)
#define EVT_VALUE1_CHANGING (EVT_USER_START + 2)
#define EVT_VALUE1_CHANGED (EVT_USER_START + 3)
#define EVT_VALUE2_WILL_CHANGE (EVT_USER_START + 4)
#define EVT_VALUE2_CHANGING (EVT_USER_START + 5)
#define EVT_VALUE2_CHANGED (EVT_USER_START + 6)


#define RANGE_SLIDER(widget) ((range_slider_t*)(range_slider_cast(WIDGET(widget))))
#define RANGE_SLIDER_PROP_VALUE1 "value1"
#define RANGE_SLIDER_PROP_VALUE2 "value2"
#define RANGE_SLIDER_PROP_DRAGGER_SIZE "dragger_size"
#define RANGE_SLIDER_PROP_DRAGGER_ADAPT_TO_ICON "dragger_adapt_to_icon"
#define RANGE_SLIDER_PROP_DRAGGER_STYLE "dragger_style"
#define RANGE_SLIDER_PROP_NO_USE_SECOND_DRAGGER "no_use_second_dragger"
#define RANGE_SLIDER_SUB_WIDGET_DRAGGER1 "dragger1"
#define RANGE_SLIDER_SUB_WIDGET_DRAGGER2 "dragger2"
#define RANGE_SLIDER_SUB_WIDGET_LABEL_MIN "label_min"
#define RANGE_SLIDER_SUB_WIDGET_LABEL_MAX "label_max"
#define RANGE_SLIDER_SUB_WIDGET_LABEL_VALUE1 "label_value1"
#define RANGE_SLIDER_SUB_WIDGET_LABEL_VALUE2 "label_value2"
#define RANGE_SLIDER_SUB_VIEW_RANGE_SLIDER_VIEW "range_slider_view"

/**
 * @method range_slider_create
 * @annotation ["constructor", "scriptable"]
 * 创建range_slider对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} range_slider对象。
 */
widget_t* range_slider_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method range_slider_cast
 * 转换为range_slider对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget range_slider对象。
 *
 * @return {widget_t*} range_slider对象。
 */
widget_t* range_slider_cast(widget_t* widget);





#define WIDGET_TYPE_RANGE_SLIDER "range_slider"

#define RANGE_SLIDER(widget) ((range_slider_t*)(range_slider_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(range_slider);

END_C_DECLS

#endif /*TK_RANGE_SLIDER_H*/
