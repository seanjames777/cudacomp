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

# Configure main compiler executable

add_executable(cc
    src/ast/astnode.cpp
    src/ast/astseqnode.cpp
    src/ast/expr/astbinop.cpp
    src/ast/expr/astboolean.cpp
    src/ast/expr/astexpnode.cpp
    src/ast/expr/astidentifier.cpp
    src/ast/expr/astinteger.cpp
    src/ast/expr/astunop.cpp
    src/ast/expr/astcall.cpp
    src/ast/stmt/astifstmt.cpp
    src/ast/stmt/astreturnstmt.cpp
    src/ast/stmt/astexprstmt.cpp
    src/ast/stmt/astscope.cpp
    src/ast/stmt/aststmtnode.cpp
    src/ast/stmt/astvardeclstmt.cpp
    src/ast/stmt/astvardefnstmt.cpp
    src/ast/top/asttopnode.cpp
    src/ast/top/astfundefn.cpp
    src/ast/type/astbooleantype.cpp
    src/ast/type/astvoidtype.cpp
    src/ast/type/astptrtype.cpp
    src/ast/type/astarg.cpp
    src/ast/type/astfuntype.cpp
    src/ast/type/astintegertype.cpp
    src/ast/type/asttype.cpp
    src/codegen/codegen.cpp
    src/codegen/codegenctx.cpp
    src/codegen/converttype.cpp
    src/main.cpp
    src/parser/parse.cpp
    src/statics/statics.cpp
    src/statics/typecheck.cpp
    src/statics/functioninfo.cpp
    src/statics/moduleinfo.cpp
    src/statics/symboltable.cpp
    src/statics/returncheck.cpp
    ${BISON_parser_OUTPUTS}
    ${FLEX_lexer_OUTPUTS}
)

# TODO: -fexceptions hack might break exceptions
set_target_properties(cc PROPERTIES COMPILE_FLAGS "-std=c++11 -Wno-deprecated-register ${LLVM_CXX_FLAGS} -fexceptions")

target_link_libraries(cc
    ${FLEX_LIBRARIES}
    ${BISON_LIBRARIES}
    ${LLVM_LD_FLAGS}
    ${LLVM_LIBS}
    ${LLVM_SYS_LIBS}
)

# Configure host and device runtime libraries

add_library(host_rt
    driver/host_rt.c
)

add_library(device_rt
    driver/device_rt.c
)

# Configure documentation

find_package(Doxygen)

if (DOXYGEN_FOUND)
# Substitutes macros into Doxyfile to set up the build paths
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_Configured)

# Creates a 'make doc' target
add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile_Configured
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Building API documentation" VERBATIM)
endif (DOXYGEN_FOUND)

# Configure test driver
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/driver/driver.py
    ${CMAKE_CURRENT_BINARY_DIR}/driver.py)

# Install everything

install(TARGETS cc DESTINATION bin/)
install(TARGETS host_rt DESTINATION lib/)
install(TARGETS device_rt DESTINATION lib/)
