#include "GildedRose.h"

GildedRose::GildedRose(std::vector<Item> &items) : items(items) {}

void GildedRose::updateQuality() {
  for (size_t i = 0; i < items.size(); i++) {
    Item &item = items[i];

    if (item.name == AGED_BRIE) {
      if (item.quality < MAX_QUALITY) {
        item.quality++;
      }

      if (item.sellIn < 1) {
        if (item.quality < MAX_QUALITY) {
          item.quality++;
        }
      }
    } else if (item.name == BACKSTAGE_PASS) {
      if (item.quality < MAX_QUALITY) {
        item.quality++;

        if (item.sellIn < 11) {
          if (item.quality < MAX_QUALITY) {
            item.quality++;
          }
        }

        if (item.sellIn < 6) {
          if (item.quality < MAX_QUALITY) {
            item.quality++;
          }
        }
      }

      if (item.sellIn < 1) {
        item.quality = MIN_QUALITY;
      }
    } else if (item.name == SULFURAS) {
      // Sulfuras does not change.
    } else {
      if (item.quality > MIN_QUALITY) {
        item.quality--;
      }

      if (item.sellIn < 1) {
        if (item.quality > MIN_QUALITY) {
          item.quality--;
        }
      }
    }

    if (item.name != SULFURAS) {
      item.sellIn--;
    }
  }
}
