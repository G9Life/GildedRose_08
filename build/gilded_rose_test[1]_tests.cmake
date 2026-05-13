add_test([=[GildedRoseTest.Foo]=]  C:/DEV/GildedRose_08/build/gilded_rose_test.exe [==[--gtest_filter=GildedRoseTest.Foo]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[GildedRoseTest.Foo]=]  PROPERTIES DEF_SOURCE_LINE [==[C:\DEV\GildedRose_08\test\GildedRoseTest.cpp:4]==] WORKING_DIRECTORY C:/DEV/GildedRose_08/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  gilded_rose_test_TESTS GildedRoseTest.Foo)
