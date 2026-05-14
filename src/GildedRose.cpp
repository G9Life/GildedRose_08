#include "GildedRose.h"

#include "AgedBrieItem.h"
#include "BackstagePassItem.h"
#include "GildedRoseItem.h"
#include "NormalItem.h"
#include "SulfurasItem.h"

#include <memory>

namespace {
constexpr const char *AGED_BRIE = "Aged Brie";
constexpr const char *BACKSTAGE_PASS =
    "Backstage passes to a TAFKAL80ETC concert";
constexpr const char *SULFURAS = "Sulfuras, Hand of Ragnaros";

std::unique_ptr<GildedRoseItem> createItem(Item &item) {
  if (item.name == AGED_BRIE) {
    return std::make_unique<AgedBrieItem>(item);
  }

  if (item.name == BACKSTAGE_PASS) {
    return std::make_unique<BackstagePassItem>(item);
  }

  if (item.name == SULFURAS) {
    return std::make_unique<SulfurasItem>(item);
  }

  return std::make_unique<NormalItem>(item);
}
} // namespace

GildedRose::GildedRose(std::vector<Item> &items) : items(items) {}

void GildedRose::updateQuality() {
  for (auto &item : items) {
    auto gildedItem = createItem(item);
    gildedItem->updateQuality();

    updateSellIn(item);
  }
}

void GildedRose::updateSellIn(Item &item) {
  if (item.name != SULFURAS) {
    item.sellIn--;
  }
}