file(GLOB_RECURSE GAMEPLAY ./  *.cpp)

if(GAMEPLAY)
    add_library(gameplay_lib ${GAMEPLAY})
else()
    add_library(gameplay_lib INTERFACE)
endif()

target_include_directories(gameplay_lib PRIVATE
    ${CMAKE_SOURCE_DIR}/engine
)