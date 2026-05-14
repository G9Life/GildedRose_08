#pragma once

#include "Item.h"

class BackstagePassItem {
private:
  Item &item_;

public:
  explicit BackstagePassItem(Item &item) : item_(item) {}

  void updateQuality() {
    static constexpr int MAX_QUALITY = 50;
    static constexpr int MIN_QUALITY = 0;

    if (item_.quality < MAX_QUALITY) {
      item_.quality++;
    }

    if (item_.sellIn < 11 && item_.quality < MAX_QUALITY) {
      item_.quality++;
    }

    if (item_.sellIn < 6 && item_.quality < MAX_QUALITY) {
      item_.quality++;
    }

    if (item_.sellIn < 1) {
      item_.quality = MIN_QUALITY;
    }
  }
};