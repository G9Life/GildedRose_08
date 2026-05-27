#pragma once

#include "GildedRoseItem.h"

class SulfurasItem : public GildedRoseItem {
public:
  using GildedRoseItem::GildedRoseItem;

  void updateQuality() override {
    // Sulfuras does not change.
  }
};