add_executable(logger_tests
    logger_tests.cpp
    ${CMAKE_SOURCE_DIR}/engine/io/path_manager.cpp
    ${CMAKE_SOURCE_DIR}/engine/tools/logger.cpp
)

target_include_directories(logger_tests PRIVATE ${CMAKE_SOURCE_DIR})

add_test(NAME logger_tests COMMAND logger_tests)
set_tests_properties(logger_tests PROPERTIES WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
