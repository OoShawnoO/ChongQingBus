cmake_minimum_required(VERSION 3.21)
project(DSCD)

#MATH(EXPR stack_size "500*1024*1024")
#set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${stack_size}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /STACK:12000000")

set(CMAKE_CXX_STANDARD 14)
set(EGE_INC_DIR D:/EGE20.08/include)
#set(EGE_LIB_DIR D:/EGE20.08/lib/mingw64)
set(EGE_LIB_DIR D:/EGE20.08/lib/vs2017/x64)

include_directories(${EGE_INC_DIR})
link_directories(${EGE_LIB_DIR})


add_executable(DSCD main.cpp)

#target_link_libraries(DSCD libgraphics64.a
#        libgdi32.a libimm32.a libmsimg32.a libole32.a
#        liboleaut32.a libwinmm.a libuuid.a libgdiplus.a)
