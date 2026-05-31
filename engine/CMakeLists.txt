file(GLOB_RECURSE ENGINE CONFIGURE_DEPENDS "*.cpp")
list(FILTER ENGINE EXCLUDE REGEX [[/animation/.*\.cpp$]])

add_library(engine_lib ${ENGINE})
