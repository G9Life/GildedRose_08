# Test plan for C++ project
_Exported on 5/19/2026 at 10:14:55 GMT+9 from Cursor (3.4.17)_

---

**User**

@GildedRose.h @GildedRose.cpp @GildedRoseRequirements.txt @requirements_analysis.md

[P] 시니어 QA 리드입니다.
[C] C++17, Google Test, CMake, (가능하면 gcov/lcov)
[T] 테스트 계획서를 작성해줘.
    - TEST_F 기반 단위 테스트 범위/우선순위
    - 경계값 케이스 목록 (quality 0,1,49,50 / sellIn 0,-1)
    - 예외/특이 케이스 목록
    - 커버리지 목표 (예: 90%+)와 gcov/lcov 측정·개선 전략
[F] Markdown 문서. docs 폴더에 test_plan.md로 저장


---

**Cursor**

요청하신 `GildedRose` 테스트 계획서를 작성하기 위해 먼저 현재 요구사항과 구현 코드를 읽고, 그 기준으로 우선순위와 경계값/특이 케이스를 정리하겠습니다.

일부 참조 파일은 루트에 없어서 실제 위치를 다시 찾고 있습니다. 요구사항 파일은 확인했고, 이제 소스와 분석 문서 위치를 좁혀서 읽겠습니다.

구현은 현재 원문 레거시 분기 구조이며, 테스트는 `TEST` 샘플 1개만 있는 상태입니다. 이제 `TEST_F`로 확장할 수 있는 계획 문서를 `docs/test_plan.md`에 작성하겠습니다.

문서를 생성했습니다. 마지막으로 파일이 의도한 경로와 내용으로 저장되었는지 간단히 확인하겠습니다.

`docs/test_plan.md`로 테스트 계획서를 작성했습니다.

포함한 내용:
- `TEST_F` 기반 단위 테스트 구조와 P0/P1/P2 우선순위
- `quality` 0, 1, 49, 50 및 `sellIn` 0, -1 경계값 케이스
- Normal, Aged Brie, Backstage Pass, Sulfuras, Conjured별 예외/특이 케이스
- 90%+ 커버리지 목표와 `gcov/lcov`, `gcovr` 측정 전략
- 커버리지 개선 및 완료 기준

`ReadLints` 확인 결과 문서에 linter 오류는 없습니다.

---

**User**

Report 폴더에 보고서로 작성해줘

---

**Cursor**

요청하신 테스트 계획서를 `Report` 폴더용 보고서 형태로 옮기겠습니다. 먼저 기존 보고서 파일의 형식과 `Report` 폴더 구성을 확인한 뒤 같은 톤으로 저장하겠습니다.

`Report` 폴더는 번호가 붙은 보고서 형식을 쓰고 있어, 이어지는 번호인 `04_test_plan_report.md`로 작성하겠습니다. 기존 `docs/test_plan.md` 내용을 보고서 제목과 적용 결과 중심으로 정리해 저장합니다.

보고서 파일을 생성했습니다. 저장 상태와 문서 진단을 확인해 마무리하겠습니다.

`Report/04_test_plan_report.md`로 보고서 형식의 테스트 계획서를 작성했습니다.

기존 `docs/test_plan.md`의 내용을 바탕으로 `Report` 폴더의 기존 번호 체계에 맞춰 정리했고, `TEST_F` 범위/우선순위, 경계값, 예외/특이 케이스, 커버리지 목표와 gcov/lcov 전략까지 포함했습니다. 문서 linter 오류도 없습니다.

