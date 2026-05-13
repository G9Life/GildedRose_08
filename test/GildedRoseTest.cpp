#include "GildedRose.h"
#include <gtest/gtest.h>

TEST(GildedRoseTest, Foo) {
  std::vector<Item> items = {Item("foo", 0, 0)};
  GildedRose app(items);
  app.updateQuality();
  // EXPECT_EQ("fixme", app.items[0].name);
  EXPECT_EQ("foo", app.items[0].name);
}

TEST(GildedRoseTest, NormalItemWithZeroQuality) {
  std::vector<Item> items = {Item("noname", 0, 0)};

  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ("noname", app.items[0].name);
  EXPECT_EQ(-1, app.items[0].sellIn);
  EXPECT_EQ(0, app.items[0].quality);
}

TEST(GildedRoseTest, ExpiredNormalItemDegradesTwiceAsFast) {
  std::vector<Item> items = {Item("noname", 0, 5)};

  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ("noname", app.items[0].name);
  EXPECT_EQ(-1, app.items[0].sellIn);
  EXPECT_EQ(3, app.items[0].quality);
}

TEST(GildedRoseTest, LegendaryItemDoesNotChange) {
  std::vector<Item> items = {Item("Sulfuras, Hand of Ragnaros", 0, 5)};

  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ("Sulfuras, Hand of Ragnaros", app.items[0].name);
  EXPECT_EQ(0, app.items[0].sellIn);
  EXPECT_EQ(5, app.items[0].quality);
}

TEST(GildedRoseTest, LegendaryItemDoesNotChangeAfterSellDate) {
  std::vector<Item> items = {Item("Sulfuras, Hand of Ragnaros", -1, 5)};

  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ("Sulfuras, Hand of Ragnaros", app.items[0].name);
  EXPECT_EQ(-1, app.items[0].sellIn);
  EXPECT_EQ(5, app.items[0].quality);
}

TEST(GildedRoseTest, ExpiredAgedBrieIncreasesByTwo) {
  std::vector<Item> items = {Item("Aged Brie", 0, 0)};

  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ("Aged Brie", app.items[0].name);
  EXPECT_EQ(-1, app.items[0].sellIn);
  EXPECT_EQ(2, app.items[0].quality);
}

TEST(GildedRoseTest, QualityNeverExceedsFifty) {
  std::vector<Item> items = {Item("Aged Brie", 0, 50)};

  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ("Aged Brie", app.items[0].name);
  EXPECT_EQ(-1, app.items[0].sellIn);
  EXPECT_EQ(50, app.items[0].quality);
}

TEST(GildedRoseTest, BackstagePassIncreasesByOneAboveTenDays) {
  std::vector<Item> items = {
      Item("Backstage passes to a TAFKAL80ETC concert", 15, 0)};

  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ("Backstage passes to a TAFKAL80ETC concert", app.items[0].name);
  EXPECT_EQ(14, app.items[0].sellIn);
  EXPECT_EQ(1, app.items[0].quality);
}

TEST(GildedRoseTest, BackstagePassDropsToZeroAfterConcert) {
  std::vector<Item> items = {
      Item("Backstage passes to a TAFKAL80ETC concert", 0, 0)};

  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ("Backstage passes to a TAFKAL80ETC concert", app.items[0].name);
  EXPECT_EQ(-1, app.items[0].sellIn);
  EXPECT_EQ(0, app.items[0].quality);
}

TEST(GildedRoseTest, BackstagePassDropsToZeroAfterConcertFromAboveFifty) {
  std::vector<Item> items = {
      Item("Backstage passes to a TAFKAL80ETC concert", 0, 51)};

  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ("Backstage passes to a TAFKAL80ETC concert", app.items[0].name);
  EXPECT_EQ(-1, app.items[0].sellIn);
  EXPECT_EQ(0, app.items[0].quality);
}
