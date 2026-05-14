#pragma once

#include "GildedRoseItem.h"

class FoodBeverageItem : public GildedRoseItem {
public:
  explicit FoodBeverageItem(Item &item) : GildedRoseItem(item) {}

  void updateQuality() override {
    decreaseQualityTwice();

    if (item_.sellIn < 1) {
      decreaseQualityTwice();
    }
  }

private:
  void decreaseQualityTwice() {
    decreaseQuality();
    decreaseQuality();
  }

  void decreaseQuality() {
    static constexpr int MIN_QUALITY = 0;

    if (item_.quality > MIN_QUALITY) {
      item_.quality--;
    }
  }
};