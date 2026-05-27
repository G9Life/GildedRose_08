#pragma once

#include "GildedRoseItem.h"

class NormalItem : public GildedRoseItem {
public:
  using GildedRoseItem::GildedRoseItem;

  void updateQuality() override {
    static constexpr int MIN_QUALITY = 0;

    if (item_.quality > MIN_QUALITY) {
      item_.quality--;
    }

    if (item_.sellIn < 1 && item_.quality > MIN_QUALITY) {
      item_.quality--;
    }
  }
};