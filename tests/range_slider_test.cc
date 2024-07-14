#include "range_slider/range_slider.h"
#include "gtest/gtest.h"

TEST(range_slider, basic) {
  value_t v;
  widget_t* w = range_slider_create(NULL, 10, 20, 30, 40);

  widget_destroy(w);
}
