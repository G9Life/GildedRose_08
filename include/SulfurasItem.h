#pragma once

#include "Item.h"

class SulfurasItem {
private:
  Item &item_;

public:
  explicit SulfurasItem(Item &item) : item_(item) {}

  void updateQuality() {
    // Sulfuras does not change.
  }
};