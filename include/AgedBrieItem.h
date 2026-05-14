#pragma once

#include "Item.h"

class AgedBrieItem {
private:
  Item &item_;

public:
  explicit AgedBrieItem(Item &item) : item_(item) {}

  void updateQuality() {
    static constexpr int MAX_QUALITY = 50;

    if (item_.quality < MAX_QUALITY) {
      item_.quality++;
    }

    if (item_.sellIn < 1) {
      if (item_.quality < MAX_QUALITY) {
        item_.quality++;
      }
    }
  }
};