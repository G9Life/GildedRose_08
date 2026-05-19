# Gilded Rose 테스트 계획서

## 1. 목적과 범위

본 문서는 C++17, Google Test, CMake 기반의 `GildedRose::updateQuality()` 동작을 검증하기 위한 테스트 계획을 정의한다. 테스트의 1차 목표는 요구사항에 명시된 아이템별 품질 변화 규칙을 회귀 테스트로 고정하고, 신규 `Conjured` 요구사항 적용 시 기존 동작이 깨지지 않도록 하는 것이다.

테스트 대상은 다음 공개 동작이다.

- `GildedRose::updateQuality()` 호출 후 각 아이템의 `sellIn` 변화
- `GildedRose::updateQuality()` 호출 후 각 아이템의 `quality` 변화
- 아이템 타입별 예외 규칙
- `quality` 하한 `0`, 상한 `50` 불변 조건
- `Sulfuras`의 전설 아이템 예외

`Item` 클래스와 `items` 멤버 구조는 요구사항상 변경하지 않는 영역이므로 테스트에서는 입력 데이터 생성과 결과 검증에만 사용한다.

## 2. 테스트 구조

기존 샘플 테스트는 `TEST(GildedRoseTest, Foo)` 형태이나, 본 계획에서는 반복되는 준비 과정을 줄이고 케이스 가독성을 높이기 위해 `TEST_F` 기반 단위 테스트로 전환한다.

권장 Fixture 구조:

```cpp
class GildedRoseTest : public ::testing::Test {
protected:
    std::vector<Item> items;

    void update() {
        GildedRose app(items);
        app.updateQuality();
    }

    void addItem(const std::string& name, int sellIn, int quality) {
        items.emplace_back(name, sellIn, quality);
    }
};
```

우선순위는 비즈니스 영향도와 회귀 위험을 기준으로 P0, P1, P2로 구분한다.

| 우선순위 | 범위 | 목적 |
|---|---|---|
| P0 | Normal, Aged Brie, Backstage Pass, Sulfuras 핵심 규칙 | 기존 요구사항 회귀 방지 |
| P0 | `quality` 0/50 경계, `sellIn` 0/-1 경계 | 치명적인 품질 범위 위반 방지 |
| P0 | Conjured 신규 규칙 | 신규 요구사항 완료 여부 검증 |
| P1 | 문자열 식별 정확성, `quality` 1/49 인접 경계 | 오프바이원 및 잘못된 분기 방지 |
| P1 | 다중 아이템 일괄 업데이트 | 반복 처리 중 일부 아이템만 오염되는 문제 방지 |
| P2 | 빈 아이템 목록, 비정상 초기 품질 입력 관찰 테스트 | 특이 입력에 대한 현재 정책 기록 |

## 3. TEST_F 단위 테스트 범위와 우선순위

### P0: Normal Item

- 판매 기한 전 일반 아이템은 하루 뒤 `sellIn`이 `1` 감소하고 `quality`가 `1` 감소한다.
- `sellIn == 0`에서 업데이트하면 업데이트 후 만료 상태가 되며 `quality`가 총 `2` 감소한다.
- `sellIn == -1`에서 업데이트하면 이미 만료된 상태로 처리되어 `quality`가 `2` 감소한다.
- `quality == 0`인 일반 아이템은 업데이트 후에도 `quality`가 `0` 미만으로 내려가지 않는다.
- `quality == 1`이고 만료된 일반 아이템은 업데이트 후 `quality == 0`이 되어야 한다.

### P0: Aged Brie

- 판매 기한 전 `Aged Brie`는 하루 뒤 `sellIn`이 `1` 감소하고 `quality`가 `1` 증가한다.
- `sellIn == 0`에서 업데이트하면 만료 후 증가 규칙까지 적용되어 `quality`가 총 `2` 증가한다.
- `sellIn == -1`에서 업데이트하면 `quality`가 `2` 증가한다.
- `quality == 49`인 경우 업데이트 후 `50`을 초과하지 않는다.
- `quality == 50`인 경우 업데이트 후에도 `50`을 유지한다.

### P0: Backstage Pass

- `sellIn > 10`이면 `quality`가 `1` 증가한다.
- `sellIn == 10`이면 `quality`가 `2` 증가한다.
- `sellIn == 5`이면 `quality`가 `3` 증가한다.
- `sellIn == 0`이면 업데이트 후 콘서트가 지난 상태이므로 `quality`가 `0`이 된다.
- `sellIn == -1`이면 이미 콘서트가 지난 상태이므로 `quality`가 `0`이 된다.
- `quality == 49` 또는 `50` 근처에서도 증가 결과가 `50`을 초과하지 않는다.

### P0: Sulfuras

- `Sulfuras, Hand of Ragnaros`는 업데이트 후 `sellIn`이 변하지 않는다.
- `Sulfuras, Hand of Ragnaros`는 업데이트 후 `quality`가 변하지 않는다.
- `quality == 80`은 전설 아이템 예외로 유지되며 일반 상한 `50`을 적용하지 않는다.
- `sellIn == 0` 또는 `sellIn == -1`에서도 변화가 없어야 한다.

### P0: Conjured

- 판매 기한 전 `Conjured` 아이템은 하루 뒤 `sellIn`이 `1` 감소하고 `quality`가 `2` 감소한다.
- `sellIn == 0`에서 업데이트하면 만료 후 규칙까지 적용되어 `quality`가 `4` 감소한다.
- `sellIn == -1`에서 업데이트하면 `quality`가 `4` 감소한다.
- `quality == 0`인 경우 업데이트 후에도 `0` 미만으로 내려가지 않는다.
- `quality == 1` 또는 `quality == 2`인 경우 감소량이 품질 하한을 넘더라도 결과는 `0`이어야 한다.

### P1: 문자열 식별과 다중 아이템

- `"Aged Brie"`와 정확히 일치하지 않는 이름은 일반 아이템으로 처리된다.
- `"Backstage passes to a TAFKAL80ETC concert"`와 정확히 일치하지 않는 이름은 일반 아이템으로 처리된다.
- `"Sulfuras, Hand of Ragnaros"`와 정확히 일치하지 않는 이름은 일반 아이템으로 처리된다.
- `Conjured` 식별 정책은 구현과 테스트에서 하나로 고정한다. 권장안은 이름이 `"Conjured"`로 시작하는 경우만 Conjured로 처리하는 것이다.
- 여러 아이템이 한 벡터에 들어 있어도 각 아이템의 규칙이 독립적으로 적용된다.

### P2: 특이 입력

- 빈 `items` 벡터에 대해 `updateQuality()`를 호출해도 예외 없이 종료된다.
- `quality > 50`으로 시작하는 비전설 아이템의 처리 정책을 명시한다. 현재 요구사항은 업데이트 후 품질 증가가 `50`을 넘지 않아야 한다는 규칙을 중심으로 하므로, 비정상 초기값 정규화 여부는 별도 합의가 필요하다.
- `quality < 0`으로 시작하는 아이템의 처리 정책을 명시한다. 요구사항은 품질이 음수가 되지 않아야 한다는 결과 불변 조건을 요구하므로, 방어 로직 추가 시 별도 테스트가 필요하다.

## 4. 경계값 케이스 목록

아래 경계값은 각 아이템 타입별로 최소 1회 이상 조합해 테스트한다. P0에서는 핵심 타입에 직접 테스트를 두고, P1에서는 파라미터화 테스트 또는 보조 테스트로 중복을 줄인다.

| 축 | 값 | 검증 포인트 |
|---|---:|---|
| `quality` | `0` | 감소 아이템이 음수가 되지 않는다. |
| `quality` | `1` | 만료 후 이중 감소 또는 Conjured 감소가 하한 `0`에서 멈춘다. |
| `quality` | `49` | 증가 아이템이 `50`까지만 증가한다. |
| `quality` | `50` | 증가 아이템이 `50`을 초과하지 않는다. |
| `sellIn` | `0` | 업데이트 후 만료 상태가 되는 날의 동작을 검증한다. |
| `sellIn` | `-1` | 이미 만료된 상태의 동작을 검증한다. |

아이템별 필수 경계 조합:

| 아이템 | 입력 | 기대 결과 |
|---|---|---|
| Normal | `sellIn=0, quality=1` | `sellIn=-1, quality=0` |
| Normal | `sellIn=-1, quality=1` | `sellIn=-2, quality=0` |
| Normal | `sellIn=0, quality=50` | `sellIn=-1, quality=48` |
| Aged Brie | `sellIn=0, quality=49` | `sellIn=-1, quality=50` |
| Aged Brie | `sellIn=-1, quality=49` | `sellIn=-2, quality=50` |
| Aged Brie | `sellIn=0, quality=50` | `sellIn=-1, quality=50` |
| Backstage Pass | `sellIn=0, quality=49` | `sellIn=-1, quality=0` |
| Backstage Pass | `sellIn=-1, quality=50` | `sellIn=-2, quality=0` |
| Backstage Pass | `sellIn=5, quality=49` | `sellIn=4, quality=50` |
| Backstage Pass | `sellIn=10, quality=49` | `sellIn=9, quality=50` |
| Sulfuras | `sellIn=0, quality=80` | `sellIn=0, quality=80` |
| Sulfuras | `sellIn=-1, quality=80` | `sellIn=-1, quality=80` |
| Conjured | `sellIn=0, quality=1` | `sellIn=-1, quality=0` |
| Conjured | `sellIn=-1, quality=2` | `sellIn=-2, quality=0` |
| Conjured | `sellIn=1, quality=50` | `sellIn=0, quality=48` |

## 5. 예외 및 특이 케이스 목록

### 예외 규칙

- `Sulfuras, Hand of Ragnaros`는 `sellIn`과 `quality`가 모두 변하지 않는다.
- `Sulfuras`의 `quality == 80`은 일반 품질 상한 `50`의 예외이다.
- `Backstage Pass`는 콘서트 이후 `quality`가 무조건 `0`이 된다.
- `Aged Brie`는 일반 아이템과 반대로 시간이 지날수록 `quality`가 증가한다.
- `Conjured`는 일반 아이템보다 두 배 빠르게 감소한다.

### 특이 케이스

- 빈 벡터 입력은 아무 변경 없이 정상 종료되어야 한다.
- 한 번의 `updateQuality()` 호출에서 여러 아이템이 순서대로 처리될 때, 앞 아이템의 결과가 뒤 아이템에 영향을 주지 않아야 한다.
- 특수 아이템 이름은 현재 구현의 정확한 문자열과 요구사항의 문자열이 일치해야 한다.
- `Conjured` 문자열 식별은 `std::string::find()` 사용 시 `npos` 비교를 누락하면 `"Conjured"`가 문자열 맨 앞에 있는 케이스를 놓칠 수 있다.
- 요구사항 밖의 초기값인 `quality < 0`, 비전설 `quality > 50`, `Sulfuras quality != 80`은 정책을 먼저 합의한 뒤 테스트로 고정한다.

## 6. 커버리지 목표

목표 커버리지는 다음과 같이 설정한다.

| 지표 | 목표 | 비고 |
|---|---:|---|
| Line coverage | 90% 이상 | `updateQuality()` 전체 분기 실행 보장 |
| Branch coverage | 85% 이상 | 아이템 타입, 품질 경계, 판매 기한 경계 분기 검증 |
| Function coverage | 100% | `GildedRose` 공개 함수 기준 |

최소 통과 기준은 라인 커버리지 90% 이상으로 두되, 본 코드의 핵심 위험은 중첩 조건과 경계 분기에 있으므로 브랜치 커버리지를 함께 추적한다. 특히 `Aged Brie`, `Backstage Pass`, `Sulfuras`, `Conjured`, Normal의 모든 주요 분기가 커버리지 리포트에서 실행된 것으로 확인되어야 한다.

## 7. gcov/lcov 측정 전략

GCC 또는 MinGW 환경에서 커버리지를 측정할 때는 테스트 전용 빌드 타입을 사용한다.

권장 CMake 옵션:

```cmake
option(ENABLE_COVERAGE "Enable coverage flags" OFF)

if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(gilded_rose PRIVATE --coverage -O0 -g)
    target_link_options(gilded_rose PRIVATE --coverage)
    target_compile_options(gilded_rose_test PRIVATE --coverage -O0 -g)
    target_link_options(gilded_rose_test PRIVATE --coverage)
endif()
```

측정 절차:

```powershell
cmake -S . -B build-coverage -DENABLE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build-coverage
ctest --test-dir build-coverage --output-on-failure
lcov --capture --directory build-coverage --output-file build-coverage/coverage.info
lcov --remove build-coverage/coverage.info "*/_deps/*" "*/test/*" --output-file build-coverage/coverage.filtered.info
genhtml build-coverage/coverage.filtered.info --output-directory build-coverage/coverage-html
```

Windows에서 `lcov` 사용이 어렵다면 `gcovr`를 대안으로 사용한다.

```powershell
gcovr -r . build-coverage --filter ".*src.*" --filter ".*include.*" --exclude ".*test.*" --html --html-details -o build-coverage/coverage.html
gcovr -r . build-coverage --filter ".*src.*" --filter ".*include.*" --exclude ".*test.*" --txt
```

## 8. 커버리지 개선 전략

1. 먼저 P0 테스트를 모두 구현하고 라인 커버리지 90% 이상 달성 여부를 확인한다.
2. 커버리지 리포트에서 미실행 분기가 남으면 해당 분기를 특정하는 최소 테스트를 추가한다.
3. `quality == 0`, `1`, `49`, `50` 및 `sellIn == 0`, `-1` 조합을 우선 보강한다.
4. `Backstage Pass`의 `sellIn > 10`, `<= 10`, `<= 5`, `< 0` 분기는 별도 테스트명으로 분리해 누락을 쉽게 확인한다.
5. `Conjured` 구현 후에는 판매 기한 전/후, 하한 클램프, 문자열 식별 테스트를 함께 추가한다.
6. 테스트명은 실패 원인이 바로 드러나도록 `NormalItem_DegradesTwiceAsFastAfterSellDate` 같은 동작 중심 이름을 사용한다.
7. 커버리지 목표를 맞추기 위한 무의미한 테스트보다 요구사항 분기와 경계값을 직접 설명하는 테스트를 우선한다.

## 9. 완료 기준

- 모든 P0 테스트가 통과한다.
- Conjured 신규 요구사항 테스트가 통과한다.
- `ctest --output-on-failure`가 성공한다.
- 라인 커버리지 90% 이상, 브랜치 커버리지 85% 이상을 달성한다.
- 커버리지 리포트에서 `GildedRose::updateQuality()`의 아이템 타입별 핵심 분기가 모두 실행된 것으로 확인된다.
- 요구사항 밖 입력에 대한 정책은 문서화되었거나 별도 이슈로 분리된다.
