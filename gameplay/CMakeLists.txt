file(GLOB_RECURSE GAMEPLAY ./  *.cpp)

if(GAMEPLAY)
    add_library(gameplay_lib ${GAMEPLAY})
else()
    add_library(gameplay_lib INTERFACE)
endif()
