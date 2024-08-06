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
/**
 * @class range_slider_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * 
 * 在xml中使用"range\_slider"标签创建控件。如：
 *
 * ```xml
 * <!-- ui -->
 *   <range_slider name="range_slider" value1="20" value2="50" max="100" step="1" bar_size="2" dragger_style="range_slider_default" style="range_slider_default" value_label_visible="true" range_label_visible="true" value_label_style="range_slider_default" range_label_style="range_slider_default_no_border" w="591" h="47" range_slider_view_width_ratio="0.85" range_slider_view_height_ratio="0.5" dragger_adapt_to_icon="true" value_label_width="50" range_label_gap="5" value_min_show_one_point="false"/>
 * ```
 *
 * 可用通过style来设置控件的显示风格，如字体的大小和颜色等等。如：
 * 
 * ```xml
 * <!-- style -->
 * <range_slider>
 *   <style name="range_slider_default">
 *     <normal bg_color="#A5A5A5" fg_color="#338FFF" font_size="32" selected_bg_color="#A5A5A5" selected_fg_color="#00B0F0" text_color="#000000"/>
 *   </style>
  * <style name="range_slider_default" bg_color="#00C2FF" fg_color="#4C728E" font_size="32" icon="range_slider_icon_2" selected_bg_color="#00C2FF" selected_fg_color="#4C728E" text_color="#000000">
  *     <normal/>
  *     <focused/>
  *   </style>
 * </range_slider>
 * <radio_button>
 *   <style name="no_dragger" bg_color="#00000000"  round_radius="4" text_color="#00000000">
 *     <normal/>
 *     <focused/>
 *     <focused_of_checked/>
 *   </style>
 *   <style name="range_slider_default" bg_color="#00000000" bg_image="range_slider_icon_3" round_radius="4" text_color="#00000000">
      <normal bg_image="range_slider_icon_2" icon="" icon_at="auto"/>
      <disable_of_checked icon="" icon_at="auto"/>
      <focused bg_image="range_slider_icon_2" border_color="#00FFFF"/>
      <focused_of_checked border_color="#00FFFF" icon="" icon_at="auto"/>
      <normal_of_checked icon="" icon_at="auto"/>
      <over_of_checked icon="" icon_at="auto"/>
      <pressed/>
      <pressed_of_checked icon="" icon_at="auto"/>
    </style>
 * </radio_button>
 * <label>
    <style name="default">
      <normal text_color="#444444"/>
      <disable text_color="#44444466"/>
    </style>
    <style name="range_slider_default">
      <normal border_color="#113554" text_color="#FFFFFF"/>
    </style>
    <style name="range_slider_default_no_border">
      <normal text_color="#527894"/>
    </style>
  </label>
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
  const char* dragger_style;

  /**
   * @property {char*} range_label_style
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 设置标识min和max的label的样式, 默认为default, 注意：text_align_h样式默认定死为right和left,无法修改。
   */
  const char* range_label_style;

  
  /**
   * @property {char*} value_label_style
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 设置标识value1和value2的label的样式, 默认为default
   */
  const char* value_label_style;


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
   * @property {uint32_t} range_label_gap
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 标识value1和value2的label与旁侧range_slider的间距，默认为10
   */
  uint32_t range_label_gap;

  /**
   * @property {uint32_t} value_label_gap
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 标识value1和value2的label与上方range_slider的间距，默认为0
   */
  uint32_t value_label_gap;

   /**
   * @property {uint32_t} value_label_width
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 标识value1和value2的label宽度，默认为50
   */
  uint32_t value_label_width;

  /**
   * @property {double} range_slider_view_width_ratio
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * range_slider主view的高度占widget比重，间接控制min和max的label的宽度
   */
  double range_slider_view_width_ratio;

  /**
   * @property {double} range_slider_view_height_ratio
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * range_slider主view的高度占widget比重，间接控制min和max的label的高度
   */
  double range_slider_view_height_ratio;


  /**
   * @property {bool_t} range_slider_focusable
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否允许控件range_slider获得焦点，由于某些原因widget的focusable无法得到预想的效果，因此focusable应以这个为准，缺省为false
   */
  bool_t range_slider_focusable;
  
  /**
   * @property {bool_t} value_min_show_one_point
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * value_min值显示一位小数点, 默认为false
   */
  bool_t value_min_show_one_point;

  /* private */
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
#define RANGE_SLIDER_PROP_FOCUSABLE "range_slider_focusable"
#define RANGE_SLIDER_PROP_VIEW_WIDTH_RATIO "range_slider_view_width_ratio"
#define RANGE_SLIDER_PROP_VIEW_HEIGHT_RATIO "range_slider_view_height_ratio"
#define RANGE_SLIDER_PROP_VALUE_MIN_SHOW_ONE_POINT "value_min_show_one_point"

#define RANGE_SLIDER_SUB_WIDGET_VIEW "range_slider_view"
#define RANGE_SLIDER_SUB_WIDGET_DRAGGER1 "dragger1"
#define RANGE_SLIDER_SUB_WIDGET_DRAGGER2 "dragger2"
#define RANGE_SLIDER_SUB_WIDGET_LABEL_MIN "label_min"
#define RANGE_SLIDER_SUB_WIDGET_LABEL_MAX "label_max"
#define RANGE_SLIDER_SUB_WIDGET_LABEL_VALUE1 "label_value1"
#define RANGE_SLIDER_SUB_WIDGET_LABEL_VALUE2 "label_value2"


#define RANGE_SLIDER_SUB_WIDGET_RANGE_LABEL_GAP "range_label_gap"
#define RANGE_SLIDER_SUB_WIDGET_VALUE_LABEL_GAP "value_label_gap"
#define RANGE_SLIDER_SUB_WIDGET_VALUE_LABEL_WIDTH "value_label_width"


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
