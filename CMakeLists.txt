cmake_minimum_required(VERSION 2.6)
project(cuda_comp)

# RPath
set(CMAKE_INSTALL_RPATH "\$ORIGIN/")
set(CMAKE_INSTALL_NAME_DIR "@executable_path/")

execute_process(COMMAND llvm-config-mp-3.5 --cxxflags OUTPUT_VARIABLE LLVM_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND llvm-config-mp-3.5 --ldflags OUTPUT_VARIABLE LLVM_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND llvm-config-mp-3.5 --libs core native OUTPUT_VARIABLE LLVM_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND llvm-config-mp-3.5 --system-libs core native OUTPUT_VARIABLE LLVM_SYS_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)

include_directories(include)

find_package(FLEX)
find_package(BISON)

BISON_TARGET(parser src/parser/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
FLEX_TARGET(lexer src/parser/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(lexer parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories("/Developer/NVIDIA/CUDA-6.5/include/")

add_executable(cc
    src/main.cpp
    src/ast/astnode.cpp
    src/ast/astinteger.cpp
    src/ast/astidentifier.cpp
    src/ast/astbinop.cpp
    src/ast/astexpnode.cpp
    src/ast/aststmtnode.cpp
    src/ast/astseqnode.cpp
    src/ast/astreturnstmt.cpp
    src/parser/parse.cpp
    src/codegen/codegenctx.cpp
    ${BISON_parser_OUTPUTS}
    ${FLEX_lexer_OUTPUTS}
)

set_target_properties(cc PROPERTIES COMPILE_FLAGS "-std=c++11 -Wno-deprecated-register ${LLVM_CXX_FLAGS}")

target_link_libraries(cc
    ${FLEX_LIBRARIES}
    ${BISON_LIBRARIES}
    ${LLVM_LD_FLAGS}
    ${LLVM_LIBS}
    ${LLVM_SYS_LIBS}
)

add_library(host_rt
    driver/host_rt.c
)

add_library(device_rt
    driver/device_rt.c
)

install(TARGETS cc DESTINATION bin/)

install(TARGETS host_rt DESTINATION bin/)
install(TARGETS device_rt DESTINATION bin/)
