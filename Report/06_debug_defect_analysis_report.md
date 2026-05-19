# Gilded Rose C++17 디버깅 및 결함 분석 보고서

## 1. 작성 목적

본 보고서는 `GildedRoseTest.cpp`의 Google Test 검증 결과와 `GildedRose.cpp`의 `updateQuality()` 구현을 기준으로 결함을 분석하고, 최소 변경 수정 방안 및 Green 확인 절차를 정리한 문서이다.

분석 대상은 다음 파일이다.

- `test/GildedRoseTest.cpp`
- `src/GildedRose.cpp`

## 2. 테스트 실패 요약

현재 로컬 환경에서 다음 명령을 실행한 결과, 전체 테스트는 Green 상태이다.

```powershell
cmake --build build
ctest --test-dir build --output-on-failure
```

실행 결과는 다음과 같다.

```text
100% tests passed, 0 tests failed out of 26
```

따라서 현재 코드 기준으로 재현되는 `EXPECT_EQ` 실패는 없다.

다만 `Conjured` 아이템 처리 분기가 누락된 이전 구현을 기준으로 하면, 다음과 같은 실패가 발생할 수 있다.

| 테스트 케이스 | 기대값 | 실제값 예시 | 차이 |
| --- | ---: | ---: | --- |
| `ConjuredItemDegradesByTwoBeforeSellDate` | `quality == 18` | `quality == 19` | 판매 전 품질 감소량이 2가 아니라 1로 적용됨 |
| `ConjuredItemDegradesByFourWhenSellInIsZero` | `quality == 16` | `quality == 18` | 판매일 경과 후 감소량이 4가 아니라 2로 적용됨 |
| `ConjuredItemDegradesByFourAfterSellDate` | `quality == 16` | `quality == 18` | 판매일 경과 후 감소량이 4가 아니라 2로 적용됨 |

## 3. 결함 위치

결함 위치는 `src/GildedRose.cpp`의 `GildedRose::updateQuality()` 내부 아이템 종류 분기이다.

현재 구현에서는 다음 위치에 `Conjured` 전용 분기가 포함되어 있어 결함이 수정된 상태이다.

```cpp
} else if (isConjured(item)) {
    decreaseQuality(item, item.sellIn < 0 ? 4 : 2);
} else {
    decreaseQuality(item, item.sellIn < 0 ? 2 : 1);
}
```

버그가 있는 상태라면 `Conjured` 아이템이 일반 아이템 분기로 처리되어 품질 감소량이 요구사항보다 작게 적용된다.

## 4. 원인 분석

Gilded Rose 요구사항에서 `Conjured` 아이템은 일반 아이템보다 두 배 빠르게 품질이 감소해야 한다.

- 일반 아이템: 판매 전 `quality - 1`, 판매일 이후 `quality - 2`
- `Conjured` 아이템: 판매 전 `quality - 2`, 판매일 이후 `quality - 4`

`Conjured` 분기가 없으면 일반 아이템의 기본 감소 로직이 적용되어 품질이 실제 요구사항보다 높게 유지된다. 이로 인해 재고 품질 계산 결과가 비즈니스 규칙과 불일치한다.

## 5. 심각도 분류

심각도는 `Major`로 분류한다.

근거는 다음과 같다.

- 특정 상품군의 핵심 비즈니스 규칙이 잘못 적용된다.
- `updateQuality()` 실행 시 잘못된 품질 값이 반복적으로 누적될 수 있다.
- 프로그램 크래시, 메모리 손상, 데이터 유실은 아니므로 `Critical`은 아니다.
- 단순 메시지나 문서 오류가 아니라 실제 계산 결과 오류이므로 `Minor` 또는 `Info`보다 높다.

## 6. 최소 변경 수정 방안

`Item` 구조체는 수정하지 않는다. `src/GildedRose.cpp` 내부에 `Conjured` 판별 함수를 추가하고, `updateQuality()`의 기존 분기에 최소 변경으로 연결한다.

제안 diff는 다음과 같다.

```diff
 bool isSulfuras(const Item& item) {
     return item.name == "Sulfuras" || item.name == "Sulfuras, Hand of Ragnaros";
 }
 
+bool isConjured(const Item& item) {
+    return item.name.rfind("Conjured", 0) == 0;
+}
+
 void decreaseQuality(Item& item, int amount = 1) {
     item.quality -= amount;
     if (item.quality < MinQuality) {
         item.quality = MinQuality;
     }
@@
         } else if (isBackstagePass(item)) {
             if (item.sellIn < 0) {
                 item.quality = MinQuality;
             } else if (initialSellIn <= 5) {
                 increaseQuality(item, 3);
@@
-        } else {
+        } else if (isConjured(item)) {
+            decreaseQuality(item, item.sellIn < 0 ? 4 : 2);
+        } else {
             decreaseQuality(item, item.sellIn < 0 ? 2 : 1);
         }
     }
 }
```

현재 워크스페이스의 `src/GildedRose.cpp`에는 위 수정이 이미 반영되어 있다.

## 7. Green 확인 절차

수정 후 다음 절차로 빌드와 테스트를 확인한다.

```powershell
cmake --build build
ctest --test-dir build --output-on-failure
```

기대 결과는 다음과 같다.

```text
100% tests passed, 0 tests failed out of 26
```

테스트가 실패할 경우 `--output-on-failure` 출력에서 실패한 `EXPECT_EQ`의 기대값과 실제값을 확인하고, 해당 아이템 종류의 `updateQuality()` 분기와 품질 상한/하한 보정 로직을 우선 점검한다.

## 8. 결론

현재 코드 기준으로 `Conjured` 아이템 결함은 수정되어 있으며, 전체 26개 테스트가 Green 상태임을 확인했다.

최소 변경 수정 방향은 `Item` 구조체를 변경하지 않고 `GildedRose.cpp` 내부의 아이템 판별 및 품질 감소 분기만 보완하는 방식이다. 이 접근은 C++17 스타일을 유지하면서 기존 공개 인터페이스와 레거시 도메인 제약을 침해하지 않는다.
