add_executable(libcompressortest
               libcompressortest.cpp)

target_link_libraries(libcompressortest
                      PRIVATE
                      libcompressor
                      GTest::gtest
                      GTest::gtest_main
                      project_warnings)

add_test(NAME libcompressortest
         COMMAND libcompressortest
                 --gtest_output=xml:${CMAKE_BINARY_DIR}/test_results.xml)