#include "awtk.h"
#include "base/assets_manager.h"
#if !defined(WITH_FS_RES) || defined(AWTK_WEB)
#include "default/inc/strings/en_US.data"
#include "default/inc/strings/zh_CN.data"
#include "default/inc/styles/default.data"
#include "default/inc/styles/keyboard.data"
#include "default/inc/styles/main.data"
#include "default/inc/ui/kb_ufloat.data"
#include "default/inc/ui/kb_default.data"
#include "default/inc/ui/kb_ascii.data"
#include "default/inc/ui/kb_int.data"
#include "default/inc/ui/kb_uint.data"
#include "default/inc/ui/main.data"
#include "default/inc/ui/kb_hex.data"
#include "default/inc/ui/kb_float.data"
#include "default/inc/ui/kb_phone.data"
#ifndef AWTK_WEB
#ifdef WITH_STB_IMAGE
#include "default/inc/images/arrow_right_n.res"
#include "default/inc/images/arrow_up_n.res"
#include "default/inc/images/arrow_right_d.res"
#include "default/inc/images/radio_unchecked_d.res"
#include "default/inc/images/arrow_left_f.res"
#include "default/inc/images/arrow_right_o.res"
#include "default/inc/images/arrow_right_p.res"
#include "default/inc/images/arrow_left_n.res"
#include "default/inc/images/arrow_left_o.res"
#include "default/inc/images/arrow_down_o.res"
#include "default/inc/images/arrow_left_d.res"
#include "default/inc/images/check_unchecked_d.res"
#include "default/inc/images/check_checked.res"
#include "default/inc/images/range_slider_icon_2.res"
#include "default/inc/images/check_checked_d.res"
#include "default/inc/images/radio_checked.res"
#include "default/inc/images/arrow_down_f.res"
#include "default/inc/images/range_slider_icon_1.res"
#include "default/inc/images/range_slider_icon_3.res"
#include "default/inc/images/arrow_down_p.res"
#include "default/inc/images/arrow_up_d.res"
#include "default/inc/images/arrow_left_p.res"
#include "default/inc/images/arrow_down_d.res"
#include "default/inc/images/arrow_right_f.res"
#include "default/inc/images/arrow_up_o.res"
#include "default/inc/images/check_unchecked.res"
#include "default/inc/images/arrow_up_f.res"
#include "default/inc/images/radio_checked_d.res"
#include "default/inc/images/arrow_up_p.res"
#include "default/inc/images/arrow_down_n.res"
#include "default/inc/images/radio_unchecked.res"
#else /*WITH_STB_IMAGE*/
#endif /*WITH_STB_IMAGE*/
#ifdef WITH_TRUETYPE_FONT
#include "default/inc/fonts/default.res"
#else /*WITH_TRUETYPE_FONT*/
#endif /*WITH_TRUETYPE_FONT*/
#endif /*AWTK_WEB*/
#ifdef WITH_VGCANVAS
#endif /*WITH_VGCANVAS*/
#endif /*!defined(WITH_FS_RES) || defined(AWTK_WEB)*/

ret_t assets_init_default(void) {
  assets_manager_t* am = assets_manager();
  assets_manager_set_theme(am, "default");

#if defined(WITH_FS_RES) && !defined(AWTK_WEB)
  assets_manager_preload(am, ASSET_TYPE_STYLE, "default");
#else /*defined(WITH_FS_RES) && !defined(AWTK_WEB)*/
  assets_manager_add(am, strings_en_US_default);
  assets_manager_add(am, strings_zh_CN_default);
  assets_manager_add(am, style_default_default);
  assets_manager_add(am, style_keyboard_default);
  assets_manager_add(am, style_main_default);
  assets_manager_add(am, ui_kb_ufloat_default);
  assets_manager_add(am, ui_kb_default_default);
  assets_manager_add(am, ui_kb_ascii_default);
  assets_manager_add(am, ui_kb_int_default);
  assets_manager_add(am, ui_kb_uint_default);
  assets_manager_add(am, ui_main_default);
  assets_manager_add(am, ui_kb_hex_default);
  assets_manager_add(am, ui_kb_float_default);
  assets_manager_add(am, ui_kb_phone_default);
#ifndef AWTK_WEB
  assets_manager_add(am, image_arrow_right_n_default);
  assets_manager_add(am, image_arrow_up_n_default);
  assets_manager_add(am, image_arrow_right_d_default);
  assets_manager_add(am, image_radio_unchecked_d_default);
  assets_manager_add(am, image_arrow_left_f_default);
  assets_manager_add(am, image_arrow_right_o_default);
  assets_manager_add(am, image_arrow_right_p_default);
  assets_manager_add(am, image_arrow_left_n_default);
  assets_manager_add(am, image_arrow_left_o_default);
  assets_manager_add(am, image_arrow_down_o_default);
  assets_manager_add(am, image_arrow_left_d_default);
  assets_manager_add(am, image_check_unchecked_d_default);
  assets_manager_add(am, image_check_checked_default);
  assets_manager_add(am, image_range_slider_icon_2_default);
  assets_manager_add(am, image_check_checked_d_default);
  assets_manager_add(am, image_radio_checked_default);
  assets_manager_add(am, image_arrow_down_f_default);
  assets_manager_add(am, image_range_slider_icon_1_default);
  assets_manager_add(am, image_range_slider_icon_3_default);
  assets_manager_add(am, image_arrow_down_p_default);
  assets_manager_add(am, image_arrow_up_d_default);
  assets_manager_add(am, image_arrow_left_p_default);
  assets_manager_add(am, image_arrow_down_d_default);
  assets_manager_add(am, image_arrow_right_f_default);
  assets_manager_add(am, image_arrow_up_o_default);
  assets_manager_add(am, image_check_unchecked_default);
  assets_manager_add(am, image_arrow_up_f_default);
  assets_manager_add(am, image_radio_checked_d_default);
  assets_manager_add(am, image_arrow_up_p_default);
  assets_manager_add(am, image_arrow_down_n_default);
  assets_manager_add(am, image_radio_unchecked_default);
#ifdef WITH_TRUETYPE_FONT
  assets_manager_add(am, font_default_default);
#else /*WITH_TRUETYPE_FONT*/
#endif /*WITH_TRUETYPE_FONT*/
#endif /*AWTK_WEB*/
#ifdef WITH_VGCANVAS
#endif /*WITH_VGCANVAS*/
#endif /*defined(WITH_FS_RES) && !defined(AWTK_WEB)*/

  tk_init_assets();
  return RET_OK;
}
