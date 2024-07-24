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
 *    <range_slider x="44" y="51" name="range_slider" value1="50" value2="150" min="1" max="300" w="468" h="24" step="1" bar_size="2" dragger_style="ocean" no_use_second_dragger="false" dragger_adapt_to_icon="true" dragger_size="0" style="ocean" style:normal:border_width="0" style:normal:round_radius="0" focusable="true" value_label_visible="true" range_label_visible="true" value_label_style="ocean" range_label_style="ocean_no_border" sensitive="true">
    </range_slider>
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
  * <style name="ocean" bg_color="#00C2FF" border_color="#00000000" fg_color="#4C728E" font_size="32" icon="icons_slider_2" selected_bg_color="#00C2FF" selected_fg_color="#4C728E" text_color="#000000">
  *     <normal/>
  *     <focused/>
  *     <over bg_color="#00E1FF" border_color="#E7E6E6"/>
  *   </style>
 * </range_slider>
 * <radio_button>
 *   <style name="no_dragger" bg_color="#00000000" border_color="#00FFFF" round_radius="4" text_color="#00000000">
 *     <normal border_color="#00000000"/>
 *     <focused/>
 *     <focused_of_checked/>
 *   </style>
 *   <style name="ocean" bg_color="#00000000" bg_image="icons_slider_3" border_color="#00000000" round_radius="4" text_color="#00000000">
 *     <normal bg_image="icons_slider_2" icon="" icon_at="auto"/>
 *     <disable_of_checked icon="" icon_at="auto"/>
 *     <focused bg_image="icons_slider_2" border_color="#00FFFF"/>
 *     <focused_of_checked border_color="#00FFFF" icon="" icon_at="auto"/>
 *     <normal_of_checked icon="" icon_at="auto"/>
 *     <over_of_checked icon="" icon_at="auto"/>
 *     <pressed/>
 *     <pressed_of_checked icon="" icon_at="auto"/>
 *   </style>
 * </radio_button>
 * <label>
 *   <style name="ocean">
 *     <normal border_color="#000000" text_color="#444444"/>
 *   </style>
 * </label>
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
   * @property {uint32_t} dragger_size
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 滑块的宽度或高度（单位：像素），缺省为 bar_size * 1.5。
   */
  uint32_t dragger_size;
  
  /**
   * @property {char*} dragger_style
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 渲染dragger的style名称, 目前使用radio_button的style, 默认为default, 可通过自定义radio_button的style来定制。
   */
  char* dragger_style;

  /**
   * @property {char*} range_label_style
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 设置标识min和max的label的样式, 默认为default, 注意：text_align_h样式默认定死为right和left,无法修改。
   */
  char* range_label_style;

  
  /**
   * @property {char*} value_label_style
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 设置标识value1和value2的label的样式, 默认为default
   */
  char* value_label_style;


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
  bool_t range_label_visible;

  /**
   * @property {bool_t} value_label_visible
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否在控件widget下方显示value1和value2的label，缺省为true
   */
  bool_t value_label_visible;
  /**
   * @property {uint32_t} range_label_width
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 标识min和max的label的宽度，默认为50
   */
  uint32_t range_label_width;

  /**
   * @property {uint32_t} range_label_gap
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 标识value1和value2的label与旁侧range_slider的间距，默认为10
   */
  uint32_t range_label_gap;
  /**
   * @property {uint32_t} value_label_width
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 标识value1和value2的label的宽度，默认为50
   */
  uint32_t value_label_width;
  /**
   * @property {uint32_t} value_label_height
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 标识value1和value2的label的高度，默认为25
   */
  uint32_t value_label_height;
  /**
   * @property {uint32_t} value_label_gap
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 标识value1和value2的label与上方range_slider的间距，默认为0
   */
  uint32_t value_label_gap;

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
#define RANGE_SLIDER_PROP_RANGE_LABEL_VISIBLE "range_label_visible"
#define RANGE_SLIDER_PROP_VALUE_LABEL_VISIBLE "value_label_visible"
#define RANGE_SLIDER_PROP_RANGE_LABEL_STYLE "range_label_style"
#define RANGE_SLIDER_PROP_VALUE_LABEL_STYLE "value_label_style"

#define RANGE_SLIDER_SUB_WIDGET_DRAGGER1 "dragger1"
#define RANGE_SLIDER_SUB_WIDGET_DRAGGER2 "dragger2"
#define RANGE_SLIDER_SUB_WIDGET_LABEL_MIN "label_min"
#define RANGE_SLIDER_SUB_WIDGET_LABEL_MAX "label_max"
#define RANGE_SLIDER_SUB_WIDGET_LABEL_VALUE1 "label_value1"
#define RANGE_SLIDER_SUB_WIDGET_LABEL_VALUE2 "label_value2"

#define RANGE_SLIDER_SUB_WIDGET_RANGE_LABEL_WIDTH "range_label_width"
#define RANGE_SLIDER_SUB_WIDGET_RANGE_LABEL_GAP "range_label_gap"
#define RANGE_SLIDER_SUB_WIDGET_VALUE_LABEL_WIDTH "value_label_width"
#define RANGE_SLIDER_SUB_WIDGET_VALUE_LABEL_HEIGHT "value_label_height"
#define RANGE_SLIDER_SUB_WIDGET_VALUE_LABEL_GAP "value_label_gap"

#define RANGE_SLIDER_SUB_WIDGET_VALUE_LABEL_WIDTH_ 50

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
