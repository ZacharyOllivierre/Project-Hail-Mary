file(GLOB_RECURSE GAMEPLAY ./  *.cpp)
add_library(gameplay_lib ${GAMEPLAY} "combat/aoe.h" "combat/aoe.cpp")
