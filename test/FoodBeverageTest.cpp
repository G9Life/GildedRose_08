#include <gtest/gtest.h>
#include <vector>

#include "GildedRose.h"
#include "Item.h"

TEST(FoodBeverageTest, DegradesTwiceAsNormal) {
  std::vector<Item> items = {Item("[F&B] Bread", 5, 20)};

  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ(4, items[0].sellIn);
  EXPECT_EQ(18, items[0].quality);
}

TEST(FoodBeverageTest, DegradesFourTimesAfterSellIn) {
  std::vector<Item> items = {Item("[F&B] Milk", 0, 20)};

  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ(-1, items[0].sellIn);
  EXPECT_EQ(16, items[0].quality);
}

TEST(FoodBeverageTest, QualityNeverBelowZero) {
  std::vector<Item> items = {Item("[F&B] Water", 0, 1)};

  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ(-1, items[0].sellIn);
  EXPECT_EQ(0, items[0].quality);
}