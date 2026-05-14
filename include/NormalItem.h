#pragma once

#include "Item.h"

class NormalItem {
private:
  Item &item_;

public:
  explicit NormalItem(Item &item) : item_(item) {}

  void updateQuality() {
    static constexpr int MIN_QUALITY = 0;

    if (item_.quality > MIN_QUALITY) {
      item_.quality--;
    }

    if (item_.sellIn < 1 && item_.quality > MIN_QUALITY) {
      item_.quality--;
    }
  }
};