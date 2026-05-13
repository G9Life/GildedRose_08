# Gilded Rose

Gilded Rose 는 게임 "World of Warcraft"에 나오는 여관 이름입니다.  
아이템은 퀄리티 속성을 가지고 있으며, 예외 사항이 있는 아이템들이 있습니다.  
- 아이템은 [아이템 이름, 퀄리티, 유통기한]의 속성을 가집니다.  
- 아이템의 퀄리티는 0 이상이고 아이템의 퀄리티는 하루가 지날 때마다 1씩 줄어듭니다.  
- 유통 기한이 지난 아이템의 퀄리티는 2배의 속도로 떨어집니다.  
- 퀄리티는 최대값이 50입니다.  

### Aged Brie, Backstage Pass, Sulfuras 의 예외적 규칙  

- Aged Brie는 하루가 지날 때마다 퀄리티가 1씩 증가합니다.  
- 유통기한이 지나면 퀄리티가 2씩 증가합니다.  
- Backstage Pass 는 유통기한(콘서트일)이 다가올수록 퀄리티가 증가합니다.  
  유통기한이 11일 이상일 때는 1, 10일 이하일 때는 2, 5일 이하일 때는 3씩 증가하지만  
  콘서트 날이 지나면 퀄리티는 0이 됩니다.  
- Sulfuras의 퀄리티는 변화가 없습니다.  
  
---------------------------------------------------------  
## 실습 목적 :  테스트 코드 작성을 통한 legacy code 이해

### 프로젝트 구조 (C++)

```
cpp/
├── CMakeLists.txt          # CMake 빌드 설정 (Google Test 자동 다운로드)
├── include/
│   ├── Item.h              # Item 클래스 (수정 불가)
│   └── GildedRose.h        # GildedRose 클래스 헤더
├── src/
│   └── GildedRose.cpp      # updateQuality() 구현
└── test/
    └── GildedRoseTest.cpp  # Google Test 기반 테스트
```

### 빌드 및 테스트 실행

```bash
cd cpp
cmake -B build
cmake --build build
ctest --test-dir build
```

### GildedRoseTest.cpp : failed test 수정
- updateQuality() 의 test case를 추가 작성해 보세요.
- unit test 내용 : 문서상에 나타난 동작들 확인

### unit test VS golden-master test 비교

### To-Do list (첫번째 커밋)

- 2번째 커밋
 -. CMake 사용을 위한 폴더 정리
 -. CPP 폴더 내 Include, src, test, CMakeLists.txt 파일 등을 GildedRose_08 프로젝트의 Root Directory 로 이동
 -. CMake 의 Configure 실행

- 3번째 커밋
 -. CMake 에서 빌드해보기, 결과는 성공
  : CMakeList 에서 GoogleTest 수정작업 없었지만 Build 됨...이유는?
 
 - 4번째 커밋
  -. 2번째 커밋할때 dev Branch 에 New Pull Request 버튼 활성화 안됨
     강제로 New Pull Request 했더니 Main Branch 에 코드 머지 됨
     vscode 에서 원격지에 코드 제출하면 왜 Pull Request 가 안되고 Code 머지가 바로 되지?
  -. GildedRoseTest.cpp 에서 Test 실행 시 실패
     EXPECT_EQ("fixme", app.items[0].name); -> EXPECT_EQ("foo", app.items[0].name); 수정
 - 5번째 커밋    
  -. 전체 Test Case 추가