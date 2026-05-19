# Cursor rules for Gilded Rose project
_Exported on 5/19/2026 at 09:42:53 GMT+9 from Cursor (3.4.17)_

---

**User**

[P] 당신은 레거시 코드 QA/리팩토링을 돕는 시니어 C++ 엔지니어입니다.
[C] Gilded Rose C++17 프로젝트에서 Cursor AI가 항상 따라야 하는 규칙을
    프로젝트 루트의 .cursorrules로 작성하려고 합니다.
[T] 아래 요구를 만족하는 .cursorrules 내용을 작성해줘.
    - 기술 스택: C++17 + CMake + Google Test (+ gcov/lcov)
    - 절대 규칙: Item 구조체 수정 금지, quality 0~50 (Sulfuras 제외), sellIn 음수 허용
    - 테스트 규칙: Given-When-Then, TEST_F, 경계값(quality 0/50, sellIn 0/-1) 포함
    - 리팩토링 규칙: 테스트 Green 상태에서만 진행, 매직 넘버 상수화 권장
[F] .cursorrules 파일에 그대로 붙여넣을 수 있는 텍스트


---

**Cursor**

아래 내용을 `.cursorrules`에 그대로 붙여넣으면 됩니다.

```text
# Gilded Rose C++17 Project Rules

You are a senior C++ engineer helping with legacy-code QA and refactoring.

## Tech Stack

- Use C++17.
- Build with CMake.
- Write tests with Google Test.
- Use gcov/lcov when checking coverage.

## Absolute Domain Rules

- Do not modify the `Item` struct.
- Do not change the public contract of existing legacy interfaces unless explicitly requested.
- `quality` must always stay between 0 and 50.
- Exception: `Sulfuras` is legendary and may keep quality 80.
- `sellIn` may become negative. Do not clamp `sellIn` to 0.
- Preserve existing Gilded Rose behavior unless tests and requirements explicitly define a change.

## Testing Rules

- Add or update tests before refactoring production code.
- Use Google Test `TEST_F` for fixture-based tests.
- Structure test intent with Given-When-Then naming, comments, or clear arrangement.
- Include boundary cases:
  - `quality == 0`
  - `quality == 50`
  - `sellIn == 0`
  - `sellIn == -1`
- Cover normal items and special items:
  - `Aged Brie`
  - `Backstage passes`
  - `Sulfuras`
  - `Conjured` items if supported by the current requirements
- Prefer focused tests that describe one behavior at a time.

## Refactoring Rules

- Refactor only when the test suite is Green.
- After every meaningful refactoring step, run the relevant tests again.
- Keep behavior unchanged during refactoring unless the task explicitly asks for behavior changes.
- Prefer small, reversible changes.
- Replace magic numbers with named constants where it improves clarity.
- Avoid broad rewrites of legacy code without test coverage.
- Prefer simple C++17 constructs over unnecessary abstractions.
- Do not introduce new dependencies unless explicitly approved.

## Code Style Guidance

- Keep code readable for legacy maintainers.
- Use descriptive names for item categories, quality limits, and sell-in thresholds.
- Avoid duplicating business rules across multiple places.
- Add comments only when they clarify non-obvious business rules.
```

---

**User**

파일 만들어줘

---

**Cursor**

요청하신 대로 프로젝트 루트에 `.cursorrules` 파일을 만들겠습니다. 먼저 기존 파일이 있는지 확인한 뒤, 없거나 비어 있으면 바로 내용을 추가하겠습니다.

기존 `.cursorrules` 파일은 없어서 새로 생성하겠습니다. 내용은 앞서 정리한 Gilded Rose C++17 규칙 그대로 넣습니다.

`.cursorrules` 파일을 프로젝트 루트에 생성했습니다.

경로: `C:\DEV\GildedRose_08\.cursorrules`

---

**User**

Report 폴더에 보고서로 작성해줘

---

**Cursor**

`.cursorrules` 내용을 바탕으로 `Report` 폴더 안에 읽기 좋은 Markdown 보고서로 정리하겠습니다. 먼저 `Report` 폴더가 있는지 확인하고, 없으면 생성한 뒤 보고서 파일을 추가하겠습니다.

`Report` 폴더에는 아직 파일이 없어서, 폴더를 준비한 다음 보고서 문서를 생성하겠습니다. 보고서는 규칙의 목적, 핵심 제약, 테스트/리팩토링 체크리스트 형태로 정리하겠습니다.

이제 `Report` 폴더 안에 `.cursorrules` 작성 보고서를 Markdown 파일로 추가하겠습니다. 한국어 보고서 형식으로, 바로 검토하거나 제출 자료로 쓸 수 있게 구성하겠습니다.

`Report` 폴더에 보고서를 작성했습니다.

파일 경로: `Report\cursorrules_report.md`

`.cursorrules`의 목적, 기술 스택, 절대 규칙, 테스트 규칙, 리팩토링 규칙, 적용 결과까지 보고서 형식으로 정리했습니다.

