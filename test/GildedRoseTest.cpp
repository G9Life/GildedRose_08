#include "GildedRose.h"
#include <gtest/gtest.h>


TEST(GildedRoseTest, Foo) {
  std::vector<Item> items = {Item("foo", 0, 0)};
  GildedRose app(items);
  app.updateQuality();
  // EXPECT_EQ("fixme", app.items[0].name);
  EXPECT_EQ("foo", app.items[0].name);
}
