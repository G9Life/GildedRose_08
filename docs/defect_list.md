# Defect List

## Summary

- Project: Gilded Rose C++17
- Test command: `cmake --build build`, `ctest --test-dir build --output-on-failure`
- Current result: `100% tests passed, 0 tests failed out of 26`
- Note: 현재 코드 기준으로 재현되는 테스트 실패는 없으며, 아래 항목은 테스트 구현 및 디버깅 과정에서 발견되어 수정 또는 조치된 결함이다.

## DEF-001

- ID: `DEF-001`
- Severity: `Major`
- ItemType: `Functional Defect`
- Steps:
  1. `Conjured Mana Cake` 아이템을 `sellIn = 10`, `quality = 20`으로 생성한다.
  2. `GildedRose::updateQuality()`를 1회 실행한다.
  3. `ConjuredItemDegradesByTwoBeforeSellDate` 테스트의 `quality` 결과를 확인한다.
  4. 추가로 `sellIn = 0` 또는 `sellIn = -1` 상태에서 `ConjuredItemDegradesByFourWhenSellInIsZero`, `ConjuredItemDegradesByFourAfterSellDate` 테스트를 실행한다.
- Expected:
  - 판매 기한 전 `Conjured` 아이템은 하루에 `quality`가 2 감소해야 한다.
  - 판매 기한 당일 또는 이후에는 하루에 `quality`가 4 감소해야 한다.
  - `quality`는 0 아래로 내려가지 않아야 한다.
- Actual:
  - `Conjured` 전용 분기가 없던 구현에서는 일반 아이템 분기가 적용되어 판매 기한 전 `quality`가 1만 감소했다.
  - 판매 기한 당일 또는 이후에도 `quality`가 4가 아니라 2만 감소했다.
  - 예시 실패: 기대 `quality == 18`, 실제 `quality == 19`; 기대 `quality == 16`, 실제 `quality == 18`.
- Root Cause:
  - `GildedRose::updateQuality()`의 아이템 타입 분기에 `Conjured` 식별 및 전용 품질 감소 규칙이 누락되어 있었다.
  - 요구사항의 `"Conjured" items degrade in Quality twice as fast as normal items` 규칙이 일반 아이템 기본 감소 로직으로 처리되었다.
- Fix Summary:
  - `src/GildedRose.cpp`에 `isConjured()` helper를 추가해 이름이 `"Conjured"`로 시작하는 아이템을 식별하도록 했다.
  - `updateQuality()`에 `Conjured` 분기를 추가하여 판매 기한 전에는 2, 판매 기한 이후에는 4만큼 `quality`를 감소시키도록 수정했다.
  - `decreaseQuality()`를 통해 `quality` 하한 0을 유지하도록 했다.
  - 관련 Google Test 5개를 추가하고 전체 26개 테스트 통과를 확인했다.

## DEF-002

- ID: `DEF-002`
- Severity: `Minor`
- ItemType: `Build Environment Defect`
- Steps:
  1. 기존 `build` 디렉터리를 사용해 `cmake --build build`를 실행한다.
  2. MinGW 빌드가 중간 산출물 의존성 파일 또는 정적 라이브러리 출력 파일을 생성하는 단계를 확인한다.
  3. 이어서 `ctest --test-dir build --output-on-failure` 실행 가능 여부를 확인한다.
- Expected:
  - 빌드 산출물에 필요한 중간 디렉터리가 존재해야 한다.
  - 빌드가 정상 완료되고 Google Test 실행 파일이 생성되어야 한다.
  - 이후 `ctest`에서 전체 테스트가 실행되어야 한다.
- Actual:
  - 기존 `build` 디렉터리의 일부 중간 산출물 디렉터리가 누락되어 MinGW가 `.obj.d` 의존성 파일 또는 정적 라이브러리 출력 파일을 생성하지 못했다.
  - 이 문제는 테스트 assertion 실패가 아니라 로컬 빌드 산출물 상태 문제로 확인되었다.
- Root Cause:
  - 소스 코드 결함이 아니라 기존 `build` 디렉터리 내부의 생성 산출물 구조가 불완전한 상태였다.
  - 필요한 CMake/MinGW 중간 출력 디렉터리가 누락되어 빌드 도구가 파일을 기록하지 못했다.
- Fix Summary:
  - 누락된 중간 산출물 디렉터리를 생성한 뒤 동일한 빌드 및 테스트 명령을 재실행했다.
  - 이후 `cmake --build build`와 `ctest --test-dir build --output-on-failure`가 정상 통과했다.
  - 장기적으로는 빌드 산출물은 소스 결함과 분리해 관리하고, 필요 시 `build` 디렉터리를 재생성하는 방식으로 조치한다.
