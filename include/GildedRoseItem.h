#pragma once

#include "Item.h"

class GildedRoseItem {
protected:
  Item &item_;

public:
  explicit GildedRoseItem(Item &item) : item_(item) {}

  virtual void updateQuality() = 0;

  virtual ~GildedRoseItem() = default;
};