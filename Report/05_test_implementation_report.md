# Gilded Rose 테스트 구현 결과 보고서

## 1. 작성 목적

본 보고서는 Gilded Rose C++17 프로젝트에서 아이템 타입별 Google Test 단위 테스트를 구현하고, 테스트가 요구사항을 검증하도록 `GildedRose::updateQuality()` 동작을 보정한 결과를 정리한 문서이다.

작업의 핵심 목적은 Normal, Aged Brie, Backstage Pass, Sulfuras, Conjured 아이템의 주요 규칙과 경계값을 테스트로 고정하여 이후 리팩토링 또는 기능 변경 시 회귀를 빠르게 탐지할 수 있게 하는 것이다.

## 2. 작업 대상

- 테스트 파일: `test/GildedRoseTest.cpp`
- 구현 파일: `src/GildedRose.cpp`
- 요구사항 분석 문서: `docs/requirements_analysis.md`
- 테스트 프레임워크: Google Test
- 언어 표준: C++17

## 3. 테스트 구현 범위

기존 `GildedRoseTest.cpp`의 placeholder 테스트를 제거하고 `TEST_F` 기반 fixture 테스트로 재작성하였다.

테스트 fixture에는 다음 요소를 포함하였다.

- 아이템 타입별 대표 이름 상수
- 단일 아이템을 생성하고 `updateQuality()`를 실행하는 `UpdateOne()` helper
- 각 테스트가 `sellIn`과 `quality` 결과를 함께 검증하는 구조

구현된 테스트는 총 26개이며, 아이템 타입별 최소 5개 이상의 테스트를 만족한다.

- Normal: 5개
- Aged Brie: 5개
- Backstage Pass: 6개
- Sulfuras: 5개
- Conjured: 5개

모든 테스트는 Given-When-Then 주석 구조를 사용하여 준비, 실행, 검증 단계를 명확히 구분하였다.

## 4. 아이템 타입별 검증 내용

### Normal

Normal 아이템은 판매 기한 전에는 `quality`가 1 감소하고, 판매 기한이 지난 뒤에는 2 감소해야 한다.

테스트에서는 다음 동작을 검증하였다.

- 판매 기한 전 하루 경과 시 `sellIn -1`, `quality -1`
- `sellIn == 0`에서 업데이트 시 만료 후 감소 규칙 적용
- `sellIn == -1`에서 업데이트 시 이미 만료된 상태로 처리
- `quality == 0`에서 음수로 내려가지 않음
- 만료 후 감소량이 현재 품질보다 커도 `quality == 0`에서 멈춤

### Aged Brie

Aged Brie는 시간이 지날수록 `quality`가 증가하며, 판매 기한 이후에는 더 빠르게 증가한다.

테스트에서는 다음 동작을 검증하였다.

- 판매 기한 전 하루 경과 시 `quality +1`
- `sellIn == 0`에서 업데이트 시 `quality +2`
- `sellIn == -1`에서 업데이트 시 `quality +2`
- `quality == 50`에서 더 증가하지 않음
- `quality == 49`에서 만료 후 증가해도 최대 `50`으로 제한

### Backstage Pass

Backstage Pass는 콘서트 날짜가 가까워질수록 증가량이 커지고, 콘서트 이후에는 `quality`가 0이 된다.

테스트에서는 다음 동작을 검증하였다.

- `sellIn > 10` 구간에서 `quality +1`
- `sellIn == 10` 구간에서 `quality +2`
- `sellIn == 5` 구간에서 `quality +3`
- `sellIn == 0`에서 업데이트 후 `quality == 0`
- `sellIn == -1`에서 업데이트 후 `quality == 0`
- 증가 중에도 `quality == 50`을 초과하지 않음

### Sulfuras

Sulfuras는 전설 아이템이므로 `sellIn`과 `quality`가 변경되지 않는다.

테스트에서는 다음 동작을 검증하였다.

- 일반 날짜에서 `sellIn` 불변
- 일반 날짜에서 `quality` 불변
- `quality == 80` 예외 유지
- `sellIn == 0`에서도 불변
- `sellIn == -1`에서도 불변

### Conjured

Conjured 아이템은 Normal 아이템보다 두 배 빠르게 `quality`가 감소한다.

테스트에서는 다음 동작을 검증하였다.

- 판매 기한 전 하루 경과 시 `quality -2`
- `sellIn == 0`에서 업데이트 시 `quality -4`
- `sellIn == -1`에서 업데이트 시 `quality -4`
- `quality == 0`에서 음수로 내려가지 않음
- 감소량이 현재 품질보다 커도 `quality == 0`에서 멈춤

## 5. 경계값 검증

이번 테스트 구현에는 요구된 핵심 경계값을 포함하였다.

- `quality == 0`: Normal, Conjured 하한 검증
- `quality == 50`: Aged Brie, Backstage Pass 상한 검증
- `sellIn == 0`: 업데이트 후 만료 상태 진입 검증
- `sellIn == -1`: 이미 만료된 상태의 규칙 검증
- Sulfuras `quality == 80`: 전설 아이템 예외 검증

경계값 테스트는 단순히 값 하나만 확인하지 않고, 각 아이템 타입의 비즈니스 규칙과 결합하여 `sellIn`과 `quality`를 함께 검증하도록 작성하였다.

## 6. 구현 보정 내용

테스트를 Green 상태로 만들기 위해 `src/GildedRose.cpp`의 `updateQuality()` 구현을 요구사항 기준으로 보정하였다.

주요 변경 사항은 다음과 같다.

- 범위 기반 `for` 루프를 사용하여 `items[i]` 반복 접근을 제거하였다.
- `isAgedBrie()`, `isBackstagePass()`, `isSulfuras()`, `isConjured()` helper를 추가하였다.
- `increaseQuality()`, `decreaseQuality()` helper로 품질 상한과 하한을 한 곳에서 처리하였다.
- `Conjured` 아이템은 이름이 `"Conjured"`로 시작하는 경우로 식별하도록 하였다.
- Sulfuras는 `"Sulfuras"`와 `"Sulfuras, Hand of Ragnaros"`를 모두 전설 아이템으로 처리하도록 하였다.
- Backstage Pass는 업데이트 후 `sellIn < 0`이면 `quality`를 0으로 설정하도록 명확히 하였다.

이 변경은 `Item` 구조체와 `GildedRose` 공개 인터페이스를 변경하지 않고, 기존 CMake/Google Test 구조 안에서 수행하였다.

## 7. 검증 결과

다음 명령으로 빌드와 테스트를 수행하였다.

```powershell
cmake --build build
ctest --test-dir build --output-on-failure
```

검증 결과는 다음과 같다.

- 전체 테스트 수: 26개
- 통과 테스트 수: 26개
- 실패 테스트 수: 0개
- 결과: 100% tests passed

추가로 `test/GildedRoseTest.cpp`와 `src/GildedRose.cpp`에 대해 IDE linter 진단을 확인했으며, 신규 linter 오류는 발견되지 않았다.

## 8. 빌드 환경 특이사항

검증 중 기존 `build` 디렉터리의 일부 중간 산출물 디렉터리가 누락되어 MinGW가 `.obj.d` 의존성 파일과 정적 라이브러리 출력 파일을 생성하지 못하는 문제가 있었다.

해당 문제는 소스 코드 또는 테스트 실패가 아니라 빌드 산출물 폴더 상태 문제로 확인되었으며, 필요한 중간 디렉터리를 생성한 뒤 동일한 빌드 및 테스트 명령이 정상 통과하였다.

## 9. 결론

이번 작업으로 Gilded Rose 핵심 아이템 5종에 대한 Google Test 기반 회귀 테스트가 마련되었다.

특히 `quality` 하한/상한, `sellIn` 만료 경계, Sulfuras 예외, Conjured 신규 규칙을 테스트로 고정했기 때문에 이후 `updateQuality()` 리팩토링을 진행하더라도 기존 비즈니스 동작을 안정적으로 검증할 수 있다.
