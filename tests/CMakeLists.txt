add_executable(logger_tests
    logger_tests.cpp
    ${CMAKE_SOURCE_DIR}/engine/io/path_manager.cpp
    ${CMAKE_SOURCE_DIR}/engine/tools/logger.cpp
)

target_include_directories(logger_tests PRIVATE ${CMAKE_SOURCE_DIR})

add_test(NAME logger_tests COMMAND logger_tests)
set_tests_properties(logger_tests PROPERTIES WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(ui_widget_tests ui_widget_tests.cpp)
target_include_directories(ui_widget_tests PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(ui_widget_tests PRIVATE engine_lib ${SDL2_LIBRARIES})

add_test(NAME ui_widget_tests COMMAND ui_widget_tests)
set_tests_properties(ui_widget_tests PROPERTIES WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
