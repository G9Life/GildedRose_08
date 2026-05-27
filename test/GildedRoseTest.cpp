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

// ①일반아이템—매일quality -1, sellIn -1
TEST(GildedRoseTest, NormalItemDegrades) {
  std::vector<Item> items = {Item("+5 Dexterity Vest", 10, 20)};

  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ(9, app.items[0].sellIn);
  EXPECT_EQ(19, app.items[0].quality);
}

// ②sellIn 지나면quality 2배감소
TEST(GildedRoseTest, NormalItemDegradesTwiceAfterSellIn) {
  std::vector<Item> items = {Item("Normal", 0, 10)};

  GildedRose app(items);
  app.updateQuality();

  EXPECT_EQ(-1, app.items[0].sellIn);
  EXPECT_EQ(8, app.items[0].quality);
}

// ③quality는절대음수가되지않는다
TEST(GildedRoseTest, QualityNeverNegative) {
  std::vector<Item> items = {Item("Normal", 5, 0)};
  GildedRose app(items);
  app.updateQuality();
  EXPECT_EQ(0, items[0].quality); // 0 유지
}

// ④Aged Brie —오래될수록quality 증가
TEST(GildedRoseTest, AgedBrieIncreasesQuality) {
  std::vector<Item> items = {Item("Aged Brie", 5, 10)};
  GildedRose app(items);
  app.updateQuality();
  EXPECT_EQ(11, items[0].quality);
}

// ⑤quality는50을초과하지않는다
TEST(GildedRoseTest, QualityNeverMoreThan50) {
  std::vector<Item> items = {Item("Aged Brie", 5, 50)};
  GildedRose app(items);
  app.updateQuality();
  EXPECT_EQ(50, items[0].quality); // 50 유지
}

// ⑥Sulfuras —quality/sellIn 변화없음
TEST(GildedRoseTest, SulfurasNeverChanges) {
  std::vector<Item> items = {Item("Sulfuras, Hand of Ragnaros", 0, 80)};
  GildedRose app(items);
  app.updateQuality();
  EXPECT_EQ(0, items[0].sellIn);
  EXPECT_EQ(80, items[0].quality);
}