# Gilded Rose 코드 품질 분석 보고서

## 분석 대상

- `include/GildedRose.h`
- `src/GildedRose.cpp`
- 핵심 메서드: `GildedRose::updateQuality()`

`updateQuality()`는 모든 아이템의 하루 경과 규칙을 처리하지만, 현재 구현은 아이템 식별, 품질 증감, 품질 한계값 보정, 판매 기한 감소, 판매 기한 경과 후 예외 규칙을 하나의 중첩 조건문 안에서 동시에 수행한다. 이 구조는 요구사항 추가 시 기존 코드를 반복적으로 수정하게 만들어 SOLID 관점에서 위험하다.

## 품질 이슈 분석

| 문제점 | 위반 원칙/스멜 | 영향 | 개선 방향 | 우선순위 |
|---|---|---|---|---:|
| `updateQuality()`가 모든 아이템 규칙을 직접 알고 처리한다. 일반 아이템, `Aged Brie`, `Backstage passes`, `Sulfuras`의 정책이 한 메서드에 섞여 있다. | SRP 위반, Long Method | 메서드가 여러 변경 이유를 가진다. 신규 아이템(`Conjured` 등) 추가 시 기존 메서드 전체를 이해하고 수정해야 하므로 회귀 위험이 커진다. | 아이템별 업데이트 정책을 분리한다. C++17에서는 `ItemUpdater` 인터페이스 기반 전략 패턴, 함수 객체 테이블, 또는 `std::variant` 기반 규칙 타입으로 분리할 수 있다. | 1 |
| 아이템 종류가 문자열 비교 조건문으로 하드코딩되어 있다. | OCP 위반, 조건문 복잡도 | 새로운 아이템 타입을 추가할 때 `updateQuality()`의 조건 분기를 수정해야 한다. 기존 정책과 신규 정책이 충돌하거나 분기 순서 버그가 생기기 쉽다. | 이름에서 정책을 선택하는 팩토리/레지스트리를 두고, `updateQuality()`는 선택된 정책 실행만 담당하게 한다. | 2 |
| `quality` 증가/감소 전후의 경계 검사(`> 0`, `< 50`)가 여러 위치에 반복된다. | Duplicated Code, Primitive Obsession | 품질 상한/하한 규칙이 흩어져 있어 한 곳을 고치면 다른 곳을 놓칠 수 있다. 특히 Backstage Pass처럼 여러 단계 증가가 있는 규칙에서 중복이 늘어난다. | `increaseQuality(Item&, int amount)`, `decreaseQuality(Item&, int amount)`, `clampQuality()` 같은 보조 함수로 `0..50` 불변식을 한 곳에서 보장한다. 단, `Sulfuras`는 품질 상한 예외임을 정책 레벨에서 분리한다. | 3 |
| `0`, `50`, `11`, `6`, `1` 같은 숫자가 의미 없이 직접 사용된다. | Magic Number | `50`이 품질 최대값인지, `11`과 `6`이 Backstage Pass 임계값인지 코드만 보고 즉시 알기 어렵다. 요구사항 변경 시 검색/수정 오류가 발생할 수 있다. | `constexpr int MinQuality = 0;`, `MaxQuality = 50;`, `BackstageMediumThreshold = 10;`, `BackstageHighThreshold = 5;`, `DailySellInDecrease = 1;`처럼 도메인 이름을 가진 상수로 치환한다. 기존 조건이 `< 11`, `< 6`이므로 상수명은 요구사항 기준(`10일 이하`, `5일 이하`)으로 맞추는 것이 좋다. | 4 |
| 중첩 `if`가 깊고 부정 조건이 많다. 예: `name != A && name != B`, 내부에서 다시 `name != Sulfuras`를 검사한다. | Long Method, Complex Conditional, Negative Logic | 정상 흐름과 예외 흐름을 추적하기 어렵다. `sellIn` 감소 전/후 어느 시점의 값을 기준으로 판단하는지도 분기 안에 숨어 있다. | 조기 분기 또는 정책 다형성으로 중첩을 제거한다. 최소 리팩토링 단계에서는 `Item& item = items[i];`로 반복 인덱싱을 줄이고, `isAgedBrie()`, `isBackstagePass()`, `isSulfuras()` 같은 판별 함수를 도입한다. | 5 |
| `items[i]` 인덱싱이 메서드 전체에 반복된다. | Readability Smell, Duplicated Expression | 조건식과 대입식이 길어지고, 핵심 비즈니스 규칙보다 컨테이너 접근 표현이 더 많이 보인다. | 루프를 범위 기반 for로 바꾸고 `for (Item& item : items)` 형태로 지역 참조를 사용한다. | 5 |
| `items`가 `GildedRose`의 public 참조 멤버로 노출되어 있다. | Encapsulation 약화 | 외부에서 언제든 아이템 목록을 직접 변경할 수 있어 `GildedRose`가 품질 갱신 불변식을 책임지기 어렵다. | 공개 API 호환성이 허용된다면 `private`로 이동하고 필요한 접근자만 제공한다. 레거시 카타 제약상 큰 변경이 어렵다면 우선순위는 낮게 둔다. | 5 |

## SRP/OCP 위반 근거

### SRP

`updateQuality()`는 다음 책임을 동시에 수행한다.

1. 문자열 기반 아이템 타입 판별
2. 일반 아이템 품질 감소
3. `Aged Brie` 품질 증가
4. `Backstage passes` 단계별 품질 증가 및 만료 후 0 처리
5. `Sulfuras` 불변 처리
6. `sellIn` 감소
7. `quality` 하한/상한 보정

이 중 하나의 규칙만 바뀌어도 같은 메서드를 수정해야 하므로 변경 이유가 하나가 아니다. 따라서 SRP를 위반한다.

### OCP

현재 구조에서 신규 아이템 정책을 추가하려면 `updateQuality()` 내부 조건문을 수정해야 한다. 예를 들어 `Conjured` 규칙을 추가하려면 기존의 일반 아이템 감소 분기, 만료 후 감소 분기, 특수 아이템 제외 조건을 모두 함께 바꿔야 한다. 확장에는 열려 있지만 기존 코드 수정에는 닫혀 있어야 한다는 OCP와 반대 방향이다.

## Magic Number 상수화 방향

| 현재 값 | 의미 | 권장 상수명 예시 | 비고 |
|---:|---|---|---|
| `0` | 최소 품질, 만료 후 Backstage Pass 품질 | `MinQuality`, `ExpiredBackstageQuality` | 같은 값이라도 의미가 다르면 별도 이름을 줄 수 있다. |
| `50` | 일반 아이템 최대 품질 | `MaxQuality` | `Sulfuras`는 예외 정책으로 분리한다. |
| `11` | Backstage Pass가 10일 이하인지 검사하기 위한 경계 | `BackstageMediumThreshold = 10` | 조건은 `sellIn <= BackstageMediumThreshold`가 더 읽기 쉽다. |
| `6` | Backstage Pass가 5일 이하인지 검사하기 위한 경계 | `BackstageHighThreshold = 5` | 조건은 `sellIn <= BackstageHighThreshold`가 요구사항과 일치한다. |
| `1` | 하루 경과, 기본 품질 변화량 | `DailySellInDecrease`, `DefaultQualityDelta` | 판매 기한 변화와 품질 변화는 의미가 다르므로 이름을 분리한다. |

## C++17 스타일 개선 방향

### 1. 전략 패턴 기반

아이템별 정책 클래스를 두고 공통 인터페이스를 통해 업데이트한다.

```cpp
struct UpdatePolicy {
    virtual ~UpdatePolicy() = default;
    virtual void update(Item& item) const = 0;
};
```

장점은 정책별 테스트가 쉽고 OCP를 가장 명확하게 만족한다는 점이다. 단점은 카타 규모에 비해 클래스 수가 늘어날 수 있다는 점이다.

### 2. 테이블 기반 함수 객체

`std::unordered_map<std::string, std::function<void(Item&)>>` 또는 문자열 판별 함수와 업데이트 람다를 묶은 테이블을 사용한다.

이 방식은 작은 코드베이스에서 과도한 클래스 계층 없이 분기를 데이터화할 수 있다. 다만 문자열 매칭 우선순위와 기본 정책 fallback을 명확히 관리해야 한다.

### 3. `std::variant` 기반 규칙 모델

아이템 타입을 `Normal`, `AgedBrie`, `BackstagePass`, `Sulfuras` 같은 타입으로 모델링하고 `std::visit`으로 업데이트한다.

컴파일타임 타입 안정성을 얻을 수 있지만, 현재 `Item`이 문자열 이름 중심의 레거시 구조라면 변환 계층이 필요하다. 도메인 모델을 더 크게 개선할 때 적합하다.

## 리팩토링 우선순위 요약

| 우선순위 | 작업 | 이유 |
|---:|---|---|
| 1 | 현재 동작을 고정하는 테스트 보강 | 레거시 코드는 먼저 동작을 보호해야 한다. 특히 만료 전후, 품질 경계값, 특수 아이템의 회귀를 막아야 한다. |
| 2 | 품질 증감/경계 보정 helper 도입 | 가장 반복이 많고 버그 가능성이 높은 부분을 작게 줄일 수 있다. |
| 3 | Magic Number와 아이템명 상수화 | 의미를 드러내고 이후 구조 변경 시 실수를 줄인다. |
| 4 | 아이템별 정책 함수 또는 전략 클래스로 분리 | SRP/OCP 위반의 핵심을 해결한다. 신규 규칙 추가 비용이 크게 낮아진다. |
| 5 | `GildedRose` 캡슐화 개선 및 public 멤버 정리 | 설계 품질은 좋아지지만 테스트/호환성 영향이 있을 수 있어 정책 분리 이후에 진행하는 편이 안전하다. |

## 개선 방향 요약

가장 먼저 현재 동작을 테스트로 고정한 뒤, `quality` 경계값 처리와 상수를 추출해 작은 안전망을 만든다. 이후 `updateQuality()`를 아이템별 정책 선택과 정책 실행으로 나누면 SRP/OCP 위반을 크게 줄일 수 있다. C++17 기준에서는 작은 코드베이스에는 함수 객체 테이블이 현실적이고, 장기적으로 신규 아이템이 계속 늘어나는 구조라면 전략 패턴이 더 적합하다. `std::variant`는 문자열 중심 레거시 모델을 타입 중심 모델로 바꾸는 더 큰 리팩토링 단계에서 고려하는 것이 좋다.
