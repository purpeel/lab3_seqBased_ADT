add_executable(run_all_tests run_all_tests.cpp ${CMAKE_SOURCE_DIR}/src/tmpl/Hanoi.cpp)
target_link_libraries(run_all_tests ${GTEST_LIBRARIES} pthread)
add_test(NAME run_all_tests COMMAND run_all_tests)