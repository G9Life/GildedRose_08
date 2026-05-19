#include "GildedRose.h"

GildedRose::GildedRose(std::vector<Item>& items) : items(items) {}

namespace {
constexpr int MinQuality = 0;
constexpr int MaxQuality = 50;

bool isAgedBrie(const Item& item) {
    return item.name == "Aged Brie";
}

bool isBackstagePass(const Item& item) {
    return item.name == "Backstage passes to a TAFKAL80ETC concert";
}

bool isSulfuras(const Item& item) {
    return item.name == "Sulfuras" || item.name == "Sulfuras, Hand of Ragnaros";
}

bool isConjured(const Item& item) {
    return item.name.rfind("Conjured", 0) == 0;
}

void increaseQuality(Item& item, int amount = 1) {
    item.quality += amount;
    if (item.quality > MaxQuality) {
        item.quality = MaxQuality;
    }
}

void decreaseQuality(Item& item, int amount = 1) {
    item.quality -= amount;
    if (item.quality < MinQuality) {
        item.quality = MinQuality;
    }
}
}

void GildedRose::updateQuality() {
    for (Item& item : items) {
        if (isSulfuras(item)) {
            continue;
        }

        const int initialSellIn = item.sellIn;
        item.sellIn -= 1;

        if (isAgedBrie(item)) {
            increaseQuality(item, item.sellIn < 0 ? 2 : 1);
        } else if (isBackstagePass(item)) {
            if (item.sellIn < 0) {
                item.quality = MinQuality;
            } else if (initialSellIn <= 5) {
                increaseQuality(item, 3);
            } else if (initialSellIn <= 10) {
                increaseQuality(item, 2);
            } else {
                increaseQuality(item);
            }
        } else if (isConjured(item)) {
            decreaseQuality(item, item.sellIn < 0 ? 4 : 2);
        } else {
            decreaseQuality(item, item.sellIn < 0 ? 2 : 1);
        }
    }
}
