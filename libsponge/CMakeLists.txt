file (GLOB LIB_SOURCES "*.cc" "util/*.cc" "tcp_helpers/*.cc")
add_library (sponge STATIC ${LIB_SOURCES})
IF(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
ENDIF(CMAKE_COMPILER_IS_GNUCC)