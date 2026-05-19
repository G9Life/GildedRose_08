#include <gtest/gtest.h>
#include "GildedRose.h"

class GildedRoseTest : public ::testing::Test {
protected:
    static constexpr const char* NormalItem = "Elixir of the Mongoose";
    static constexpr const char* AgedBrie = "Aged Brie";
    static constexpr const char* BackstagePass = "Backstage passes to a TAFKAL80ETC concert";
    static constexpr const char* Sulfuras = "Sulfuras, Hand of Ragnaros";
    static constexpr const char* Conjured = "Conjured Mana Cake";

    static Item UpdateOne(const std::string& name, int sellIn, int quality) {
        std::vector<Item> items = {Item(name, sellIn, quality)};
        GildedRose app(items);
        app.updateQuality();
        return app.items[0];
    }
};

TEST_F(GildedRoseTest, NormalItemDegradesByOneBeforeSellDate) {
    // Given
    const int sellIn = 10;
    const int quality = 20;

    // When
    const Item item = UpdateOne(NormalItem, sellIn, quality);

    // Then
    EXPECT_EQ(9, item.sellIn);
    EXPECT_EQ(19, item.quality);
}

TEST_F(GildedRoseTest, NormalItemDegradesTwiceWhenSellInIsZero) {
    // Given
    const int sellIn = 0;
    const int quality = 20;

    // When
    const Item item = UpdateOne(NormalItem, sellIn, quality);

    // Then
    EXPECT_EQ(-1, item.sellIn);
    EXPECT_EQ(18, item.quality);
}

TEST_F(GildedRoseTest, NormalItemDegradesTwiceAfterSellDate) {
    // Given
    const int sellIn = -1;
    const int quality = 20;

    // When
    const Item item = UpdateOne(NormalItem, sellIn, quality);

    // Then
    EXPECT_EQ(-2, item.sellIn);
    EXPECT_EQ(18, item.quality);
}

TEST_F(GildedRoseTest, NormalItemQualityNeverDropsBelowZeroAtBoundary) {
    // Given
    const int sellIn = 5;
    const int quality = 0;

    // When
    const Item item = UpdateOne(NormalItem, sellIn, quality);

    // Then
    EXPECT_EQ(4, item.sellIn);
    EXPECT_EQ(0, item.quality);
}

TEST_F(GildedRoseTest, NormalItemQualityStopsAtZeroAfterSellDate) {
    // Given
    const int sellIn = -1;
    const int quality = 1;

    // When
    const Item item = UpdateOne(NormalItem, sellIn, quality);

    // Then
    EXPECT_EQ(-2, item.sellIn);
    EXPECT_EQ(0, item.quality);
}

TEST_F(GildedRoseTest, AgedBrieIncreasesByOneBeforeSellDate) {
    // Given
    const int sellIn = 10;
    const int quality = 20;

    // When
    const Item item = UpdateOne(AgedBrie, sellIn, quality);

    // Then
    EXPECT_EQ(9, item.sellIn);
    EXPECT_EQ(21, item.quality);
}

TEST_F(GildedRoseTest, AgedBrieIncreasesTwiceWhenSellInIsZero) {
    // Given
    const int sellIn = 0;
    const int quality = 20;

    // When
    const Item item = UpdateOne(AgedBrie, sellIn, quality);

    // Then
    EXPECT_EQ(-1, item.sellIn);
    EXPECT_EQ(22, item.quality);
}

TEST_F(GildedRoseTest, AgedBrieIncreasesTwiceAfterSellDate) {
    // Given
    const int sellIn = -1;
    const int quality = 20;

    // When
    const Item item = UpdateOne(AgedBrie, sellIn, quality);

    // Then
    EXPECT_EQ(-2, item.sellIn);
    EXPECT_EQ(22, item.quality);
}

TEST_F(GildedRoseTest, AgedBrieQualityDoesNotExceedFiftyAtBoundary) {
    // Given
    const int sellIn = 5;
    const int quality = 50;

    // When
    const Item item = UpdateOne(AgedBrie, sellIn, quality);

    // Then
    EXPECT_EQ(4, item.sellIn);
    EXPECT_EQ(50, item.quality);
}

TEST_F(GildedRoseTest, AgedBrieQualityCapsAtFiftyAfterSellDate) {
    // Given
    const int sellIn = -1;
    const int quality = 49;

    // When
    const Item item = UpdateOne(AgedBrie, sellIn, quality);

    // Then
    EXPECT_EQ(-2, item.sellIn);
    EXPECT_EQ(50, item.quality);
}

TEST_F(GildedRoseTest, BackstagePassIncreasesByOneMoreThanTenDaysOut) {
    // Given
    const int sellIn = 11;
    const int quality = 20;

    // When
    const Item item = UpdateOne(BackstagePass, sellIn, quality);

    // Then
    EXPECT_EQ(10, item.sellIn);
    EXPECT_EQ(21, item.quality);
}

TEST_F(GildedRoseTest, BackstagePassIncreasesByTwoAtTenDays) {
    // Given
    const int sellIn = 10;
    const int quality = 20;

    // When
    const Item item = UpdateOne(BackstagePass, sellIn, quality);

    // Then
    EXPECT_EQ(9, item.sellIn);
    EXPECT_EQ(22, item.quality);
}

TEST_F(GildedRoseTest, BackstagePassIncreasesByThreeAtFiveDays) {
    // Given
    const int sellIn = 5;
    const int quality = 20;

    // When
    const Item item = UpdateOne(BackstagePass, sellIn, quality);

    // Then
    EXPECT_EQ(4, item.sellIn);
    EXPECT_EQ(23, item.quality);
}

TEST_F(GildedRoseTest, BackstagePassDropsToZeroWhenSellInIsZero) {
    // Given
    const int sellIn = 0;
    const int quality = 20;

    // When
    const Item item = UpdateOne(BackstagePass, sellIn, quality);

    // Then
    EXPECT_EQ(-1, item.sellIn);
    EXPECT_EQ(0, item.quality);
}

TEST_F(GildedRoseTest, BackstagePassQualityDoesNotExceedFiftyAtBoundary) {
    // Given
    const int sellIn = 5;
    const int quality = 50;

    // When
    const Item item = UpdateOne(BackstagePass, sellIn, quality);

    // Then
    EXPECT_EQ(4, item.sellIn);
    EXPECT_EQ(50, item.quality);
}

TEST_F(GildedRoseTest, BackstagePassRemainsZeroAfterSellDate) {
    // Given
    const int sellIn = -1;
    const int quality = 20;

    // When
    const Item item = UpdateOne(BackstagePass, sellIn, quality);

    // Then
    EXPECT_EQ(-2, item.sellIn);
    EXPECT_EQ(0, item.quality);
}

TEST_F(GildedRoseTest, SulfurasSellInDoesNotChange) {
    // Given
    const int sellIn = 10;
    const int quality = 80;

    // When
    const Item item = UpdateOne(Sulfuras, sellIn, quality);

    // Then
    EXPECT_EQ(10, item.sellIn);
    EXPECT_EQ(80, item.quality);
}

TEST_F(GildedRoseTest, SulfurasQualityDoesNotChange) {
    // Given
    const int sellIn = 5;
    const int quality = 80;

    // When
    const Item item = UpdateOne(Sulfuras, sellIn, quality);

    // Then
    EXPECT_EQ(5, item.sellIn);
    EXPECT_EQ(80, item.quality);
}

TEST_F(GildedRoseTest, SulfurasAllowsQualityAboveNormalMaximum) {
    // Given
    const int sellIn = 0;
    const int quality = 80;

    // When
    const Item item = UpdateOne(Sulfuras, sellIn, quality);

    // Then
    EXPECT_EQ(0, item.sellIn);
    EXPECT_EQ(80, item.quality);
}

TEST_F(GildedRoseTest, SulfurasDoesNotChangeWhenSellInIsZero) {
    // Given
    const int sellIn = 0;
    const int quality = 80;

    // When
    const Item item = UpdateOne(Sulfuras, sellIn, quality);

    // Then
    EXPECT_EQ(0, item.sellIn);
    EXPECT_EQ(80, item.quality);
}

TEST_F(GildedRoseTest, SulfurasDoesNotChangeAfterSellDate) {
    // Given
    const int sellIn = -1;
    const int quality = 80;

    // When
    const Item item = UpdateOne(Sulfuras, sellIn, quality);

    // Then
    EXPECT_EQ(-1, item.sellIn);
    EXPECT_EQ(80, item.quality);
}

TEST_F(GildedRoseTest, ConjuredItemDegradesByTwoBeforeSellDate) {
    // Given
    const int sellIn = 10;
    const int quality = 20;

    // When
    const Item item = UpdateOne(Conjured, sellIn, quality);

    // Then
    EXPECT_EQ(9, item.sellIn);
    EXPECT_EQ(18, item.quality);
}

TEST_F(GildedRoseTest, ConjuredItemDegradesByFourWhenSellInIsZero) {
    // Given
    const int sellIn = 0;
    const int quality = 20;

    // When
    const Item item = UpdateOne(Conjured, sellIn, quality);

    // Then
    EXPECT_EQ(-1, item.sellIn);
    EXPECT_EQ(16, item.quality);
}

TEST_F(GildedRoseTest, ConjuredItemDegradesByFourAfterSellDate) {
    // Given
    const int sellIn = -1;
    const int quality = 20;

    // When
    const Item item = UpdateOne(Conjured, sellIn, quality);

    // Then
    EXPECT_EQ(-2, item.sellIn);
    EXPECT_EQ(16, item.quality);
}

TEST_F(GildedRoseTest, ConjuredItemQualityNeverDropsBelowZeroAtBoundary) {
    // Given
    const int sellIn = 5;
    const int quality = 0;

    // When
    const Item item = UpdateOne(Conjured, sellIn, quality);

    // Then
    EXPECT_EQ(4, item.sellIn);
    EXPECT_EQ(0, item.quality);
}

TEST_F(GildedRoseTest, ConjuredItemQualityStopsAtZeroAfterSellDate) {
    // Given
    const int sellIn = -1;
    const int quality = 3;

    // When
    const Item item = UpdateOne(Conjured, sellIn, quality);

    // Then
    EXPECT_EQ(-2, item.sellIn);
    EXPECT_EQ(0, item.quality);
}
