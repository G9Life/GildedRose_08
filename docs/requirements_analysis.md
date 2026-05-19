# Gilded Rose 요구사항 분석

## 1. 아이템 타입별 비즈니스 규칙

| 아이템 타입 | 식별 기준 | sellIn 변화 | quality 변화 | 경계/예외 조건 |
|---|---|---:|---|---|
| Normal | 특수 아이템 이름에 해당하지 않는 일반 아이템 | 매일 `-1` | 판매 기한 전에는 매일 `-1`, 판매 기한이 지난 뒤에는 매일 `-2` | `quality`는 `0` 미만으로 내려가지 않는다. |
| Aged Brie | 이름이 정확히 `"Aged Brie"` | 매일 `-1` | 오래될수록 증가한다. 판매 기한 전에는 매일 `+1`, 판매 기한이 지난 뒤에는 매일 `+2` | `quality`는 `50`을 초과하지 않는다. |
| Backstage Pass | 이름이 `"Backstage passes"` 계열 요구사항 문자열과 일치 | 매일 `-1` | `sellIn > 10`이면 `+1`, `6 <= sellIn <= 10`이면 `+2`, `1 <= sellIn <= 5`이면 `+3` | 콘서트 이후, 즉 업데이트 후 판매 기한이 지난 상태에서는 `quality`가 `0`이 된다. `quality`는 증가 중에도 `50`을 초과하지 않는다. |
| Sulfuras | 이름이 정확히 `"Sulfuras"` 또는 레거시 요구사항의 전체 명칭과 일치 | 변화 없음 | 변화 없음 | 전설 아이템 예외로 `quality`는 보통 `80`이며, 일반 `0~50` 제한을 적용하지 않는다. |
| Conjured | 이름에 `"Conjured"` 신규 카테고리를 명확히 식별할 수 있는 문자열 | 매일 `-1` | Normal 아이템보다 두 배 빠르게 감소한다. 판매 기한 전에는 매일 `-2`, 판매 기한이 지난 뒤에는 매일 `-4` | `quality`는 `0` 미만으로 내려가지 않는다. |

## 2. 문자열 비교/분기 시 주의점

1. 정확한 아이템은 `std::string::operator==`로 비교하는 것이 가장 안전하다.
   예: `"Aged Brie"`, `"Sulfuras"`처럼 고정된 이름을 요구하는 경우.

2. `std::string::find()`를 사용할 경우 반환값은 위치 인덱스이며, 찾지 못하면 `std::string::npos`를 반환한다.
   `if (name.find("Conjured"))`처럼 작성하면 위치가 `0`일 때 `false`로 평가되어 버그가 된다.

3. `find()` 기반 분기는 반드시 `name.find("Conjured") != std::string::npos` 형태로 작성한다.

4. 부분 문자열 검색은 의도하지 않은 아이템까지 매칭할 수 있다.
   예: `"Not Conjured Item"`도 `"Conjured"`를 포함하므로 Conjured로 처리될 수 있다.

5. Backstage Pass와 Sulfuras는 카타 원문에서 긴 이름을 사용하는 경우가 많다.
   구현과 테스트는 실제 코드에서 사용하는 정확한 문자열을 기준으로 통일해야 한다.

6. 문자열 분기 순서는 특수 아이템을 먼저 처리하고, 마지막에 Normal 기본 규칙을 적용하는 방식이 안전하다.

## 3. 예외/경계값 조건

1. `quality == 0`
   Normal과 Conjured는 더 감소하지 않아야 한다.

2. `quality == 49`
   Aged Brie와 Backstage Pass 증가 시 `50`까지만 증가해야 한다.

3. `quality == 50`
   Aged Brie와 Backstage Pass는 더 증가하지 않아야 한다.

4. `sellIn > 0`
   판매 기한 전 규칙을 적용한 뒤 `sellIn`은 하루 감소한다.

5. `sellIn == 0`
   업데이트 후 판매 기한이 지난 상태가 되므로, 다음 규칙 적용에서 만료 후 동작을 명확히 검증해야 한다.
   기존 구현 방식에 따라 감소 순서가 결과에 영향을 줄 수 있으므로 테스트로 고정해야 한다.

6. `sellIn == -1` 및 그보다 작은 음수
   이미 판매 기한이 지난 아이템으로 취급한다.
   Normal은 `-2`, Conjured는 `-4`, Aged Brie는 `+2` 효과가 기대된다.

7. Sulfuras
   `sellIn`과 `quality`가 모두 변하지 않아야 한다.
   `quality == 80`은 전설 아이템 예외로 허용하며, 일반 `quality <= 50` 제한을 적용하지 않는다.

8. 모든 일반 품질 제한
   Sulfuras를 제외한 모든 아이템의 `quality`는 업데이트 후 `0 <= quality <= 50` 범위를 유지해야 한다.

## 4. Conjured 신규 요구사항 명세

1. Conjured 아이템은 Normal 아이템과 동일하게 매일 `sellIn`이 `1` 감소한다.

2. Conjured 아이템은 Normal 아이템보다 `quality`가 두 배 빠르게 감소한다.

3. 판매 기한 전 Conjured 아이템은 하루 업데이트 시 `quality`가 `2` 감소한다.

4. 판매 기한이 지난 Conjured 아이템은 하루 업데이트 시 `quality`가 `4` 감소한다.

5. Conjured 아이템의 `quality`는 `0` 미만으로 내려가지 않는다.

6. Conjured 아이템에는 Aged Brie, Backstage Pass, Sulfuras의 증가/불변 예외 규칙을 적용하지 않는다.

7. 구현에서는 Conjured 식별 기준을 테스트와 동일하게 고정해야 한다.
   권장 기준은 이름이 `"Conjured"`로 시작하거나, 프로젝트에서 합의한 정확한 문자열과 일치하는 방식이다.

## 5. Google Test 기준 테스트 시나리오 목록

1. Normal 아이템은 판매 기한 전 하루가 지나면 `sellIn`이 `1` 감소하고 `quality`가 `1` 감소한다.

2. Normal 아이템은 `sellIn == 0`에서 업데이트하면 판매 기한 경과 규칙에 따라 `quality`가 더 빠르게 감소한다.

3. Normal 아이템은 `sellIn < 0`에서 업데이트하면 `quality`가 `2` 감소한다.

4. Normal 아이템은 `quality == 0`일 때 업데이트해도 음수가 되지 않는다.

5. Aged Brie는 판매 기한 전 하루가 지나면 `quality`가 `1` 증가한다.

6. Aged Brie는 판매 기한이 지난 뒤 하루가 지나면 `quality`가 `2` 증가한다.

7. Aged Brie는 `quality == 50`일 때 업데이트해도 `50`을 초과하지 않는다.

8. Backstage Pass는 `sellIn > 10`일 때 `quality`가 `1` 증가한다.

9. Backstage Pass는 `sellIn == 10`일 때 `quality`가 `2` 증가한다.

10. Backstage Pass는 `sellIn == 5`일 때 `quality`가 `3` 증가한다.

11. Backstage Pass는 콘서트 이후 업데이트 결과 `quality`가 `0`이 된다.

12. Backstage Pass는 증가하더라도 `quality`가 `50`을 초과하지 않는다.

13. Sulfuras는 업데이트 후에도 `sellIn`이 변하지 않는다.

14. Sulfuras는 업데이트 후에도 `quality`가 변하지 않는다.

15. Sulfuras는 `quality == 80`을 유지하며 일반 최대값 `50` 제한을 받지 않는다.

16. Conjured 아이템은 판매 기한 전 하루가 지나면 `quality`가 `2` 감소한다.

17. Conjured 아이템은 `sellIn == 0` 또는 음수 상태에서 업데이트하면 `quality`가 `4` 감소한다.

18. Conjured 아이템은 `quality`가 낮을 때 업데이트해도 `0` 미만으로 내려가지 않는다.

19. Conjured 아이템도 Normal 아이템처럼 매일 `sellIn`이 `1` 감소한다.

20. 특수 아이템 문자열이 정확히 매칭되지 않는 아이템은 Normal 규칙으로 처리된다.
