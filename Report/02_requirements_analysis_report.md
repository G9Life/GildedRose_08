# Gilded Rose 요구사항 분석 보고서

## 1. 작성 목적

본 보고서는 Gilded Rose C++17 프로젝트의 요구사항을 시니어 C++ QA 엔지니어 관점에서 구현 및 테스트 가능한 형태로 재정리한 문서이다.

레거시 코드의 동작을 보존하면서 `Conjured` 신규 요구사항을 안전하게 추가하기 위해, 아이템 타입별 비즈니스 규칙, 문자열 분기 주의점, 경계값 조건, Google Test 기준 테스트 시나리오를 명확히 정의하는 것을 목적으로 한다.

## 2. 분석 대상

- 요구사항 원문: `GildedRoseRequirements.txt`
- 프로젝트 개요: `README.md`
- 분석 산출물: `docs/requirements_analysis.md`
- 기술 스택: C++17, CMake, Google Test

## 3. 핵심 도메인 규칙 요약

| 구분 | 핵심 규칙 | 테스트 관점 |
|---|---|---|
| Normal | 하루마다 `sellIn`과 `quality`가 감소하며, 판매 기한 이후 `quality` 감소율이 2배가 된다. | `sellIn > 0`, `sellIn == 0`, `sellIn < 0`, `quality == 0`을 검증한다. |
| Aged Brie | 시간이 지날수록 `quality`가 증가하며, 판매 기한 이후 더 빠르게 증가한다. | `quality == 50` 상한을 반드시 검증한다. |
| Backstage Pass | 콘서트 날짜가 가까워질수록 `quality` 증가량이 커지고, 콘서트 이후 `0`이 된다. | `sellIn == 10`, `sellIn == 5`, 콘서트 이후 조건을 경계값으로 검증한다. |
| Sulfuras | 전설 아이템으로 `sellIn`과 `quality`가 변하지 않는다. | `quality == 80` 예외와 불변성을 검증한다. |
| Conjured | Normal 아이템보다 `quality`가 2배 빠르게 감소한다. | 판매 기한 전 `-2`, 판매 기한 후 `-4`, `quality >= 0`을 검증한다. |

## 4. C++ 구현 시 주요 리스크

1. `std::string::find()`의 반환값을 불리언처럼 사용하는 오류를 주의해야 한다.
   `"Conjured"`가 문자열 맨 앞에 있을 때 반환값 `0`이 `false`로 평가될 수 있다.

2. 정확한 이름 비교가 필요한 아이템은 `operator==`를 우선 사용해야 한다.
   `Aged Brie`, `Sulfuras`처럼 고정 명칭을 가진 아이템은 부분 문자열 검색보다 정확 비교가 안전하다.

3. 부분 문자열 검색은 의도하지 않은 매칭을 만들 수 있다.
   예를 들어 `"Not Conjured Item"`도 `"Conjured"`를 포함하므로 Conjured로 처리될 수 있다.

4. `sellIn` 감소 시점과 품질 변경 시점은 테스트로 고정해야 한다.
   특히 `sellIn == 0`은 판매 기한 경계이므로 구현 순서에 따라 결과가 달라질 수 있다.

5. Sulfuras는 일반 품질 제한의 예외이다.
   일반 아이템의 `quality`는 `0~50` 범위에 머물러야 하지만, Sulfuras는 `80`을 유지할 수 있다.

## 5. 경계값 테스트 기준

| 경계값 | 검증 의도 |
|---|---|
| `quality == 0` | 감소형 아이템의 품질이 음수가 되지 않는지 확인한다. |
| `quality == 49` | 증가형 아이템이 `50`까지만 증가하는지 확인한다. |
| `quality == 50` | 증가형 아이템이 상한을 초과하지 않는지 확인한다. |
| `sellIn == 10` | Backstage Pass의 `+2` 증가 구간 진입을 확인한다. |
| `sellIn == 5` | Backstage Pass의 `+3` 증가 구간 진입을 확인한다. |
| `sellIn == 0` | 판매 기한 경계에서 만료 후 규칙이 올바르게 적용되는지 확인한다. |
| `sellIn == -1` | 이미 판매 기한이 지난 아이템의 규칙을 확인한다. |
| Sulfuras `quality == 80` | 전설 아이템 예외 품질을 확인한다. |

## 6. Google Test 시나리오 설계 방향

테스트는 하나의 테스트가 하나의 비즈니스 규칙을 검증하도록 작고 명확하게 작성하는 것이 적절하다.

권장 시나리오는 다음 범주로 나눈다.

1. Normal 아이템 기본 감소 및 판매 기한 이후 2배 감소
2. Aged Brie 증가 및 `quality` 상한
3. Backstage Pass의 `10일`, `5일`, 콘서트 이후 경계값
4. Sulfuras의 `sellIn`, `quality` 불변성
5. Conjured의 판매 기한 전후 2배 감소율
6. 모든 비전설 아이템의 `quality` 하한/상한
7. 문자열 식별 실패 시 Normal 규칙 적용

## 7. 결론

Gilded Rose 프로젝트에서 가장 중요한 QA 관점은 레거시 동작 보존과 경계값 명확화이다.

`Conjured` 요구사항은 단순히 감소량을 2배로 늘리는 변경처럼 보이지만, 판매 기한 이후에는 기존 Normal의 2배 감소 규칙과 결합되어 하루 `4` 감소가 되어야 한다. 따라서 Conjured 구현 전에는 Normal, Aged Brie, Backstage Pass, Sulfuras의 기존 동작을 Google Test로 고정하고, 이후 Conjured 전용 테스트를 추가하는 순서가 안전하다.
