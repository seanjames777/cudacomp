#
# Makefile
#
# This is a pure Makefile for 411/Autolab compatability. For general builds,
# CMake according the README.txt.
#
# Targets: make, make l4c, make clean
#
# Author: Sean James <seanjames777@gmail.com>
#

all: l4c

CPP := g++
FLEX := flex
BISON := bison

CPPFLAGS := \
	-I./ \
	-I./include/ \
	-std=c++11 \
	$(shell llvm-config --cxxflags) \
	-frtti \
	-fexceptions \
	-O3 \
	-Wno-deprecated-register

LDFLAGS := \
	$(shell llvm-config --ldflags) \
	$(shell llvm-config --libs) \
	$(shell llvm-config --system-libs)

OBJ := obj
BIN := bin

SOURCES := \
	src/ast/astnode.cpp \
	src/ast/astprint.cpp \
	src/ast/astseqnode.cpp \
	src/ast/astvisitor.cpp \
	src/ast/expr/astallocarrayexp.cpp \
	src/ast/expr/astallocexp.cpp \
	src/ast/expr/astbinopexp.cpp \
	src/ast/expr/astbooleanexp.cpp \
	src/ast/expr/astcallexp.cpp \
	src/ast/expr/astderefexp.cpp \
	src/ast/expr/astexpnode.cpp \
	src/ast/expr/astfloatexp.cpp \
	src/ast/expr/astidentifierexp.cpp \
	src/ast/expr/astindexexp.cpp \
	src/ast/expr/astintegerexp.cpp \
	src/ast/expr/astnullexp.cpp \
	src/ast/expr/astrecordaccessexp.cpp \
	src/ast/expr/astternopexp.cpp \
	src/ast/expr/astunopexp.cpp \
	src/ast/stmt/astassertstmt.cpp \
	src/ast/stmt/astassignstmt.cpp \
	src/ast/stmt/astexprstmt.cpp \
	src/ast/stmt/astifstmt.cpp \
	src/ast/stmt/astforstmt.cpp \
	src/ast/stmt/astreturnstmt.cpp \
	src/ast/stmt/astscopestmt.cpp \
	src/ast/stmt/aststmtnode.cpp \
	src/ast/stmt/astvardeclstmt.cpp \
	src/ast/stmt/astwhilestmt.cpp \
	src/ast/decl/astfundecl.cpp \
	src/ast/decl/astdeclnode.cpp \
	src/ast/decl/astrecorddecl.cpp \
	src/ast/decl/asttypedecl.cpp \
	src/ast/type/astargnode.cpp \
	src/ast/type/astarrtype.cpp \
	src/ast/type/astbooleantype.cpp \
	src/ast/type/astfloattype.cpp \
	src/ast/type/astfuntype.cpp \
	src/ast/type/astidtype.cpp \
	src/ast/type/astintegertype.cpp \
	src/ast/type/astptrtype.cpp \
	src/ast/type/astrecordtype.cpp \
	src/ast/type/asttypenode.cpp \
	src/ast/type/astvoidtype.cpp \
	src/codegen/codegen.cpp \
	src/codegen/codegenctx.cpp \
	src/codegen/converttype.cpp \
	src/main.cpp \
	src/options.cpp \
	src/parser/parse.cpp \
	src/statics/functioninfo.cpp \
	src/statics/moduleinfo.cpp \
	src/statics/returncheck.cpp \
	src/statics/statics.cpp \
	src/statics/symbolcheck.cpp \
	src/statics/symboltable.cpp \
	src/statics/typecheck.cpp \
	src/statics/typeresolve.cpp \
	src/statics/funcheck.cpp \
	src/statics/exceptions.cpp \
	src/statics/symbolmangle.cpp \
	src/statics/cudapartition.cpp

OBJS := \
	$(SOURCES:src/%.cpp=$(OBJ)/%.o) \
	$(OBJ)/parser.o \
	$(OBJ)/lexer.o

.PHONY: DIRECTORIES
DIRECTORIES:
	mkdir -p $(OBJ)
	mkdir -p $(OBJ)/ast
	mkdir -p $(OBJ)/ast/decl
	mkdir -p $(OBJ)/ast/expr
	mkdir -p $(OBJ)/ast/sched
	mkdir -p $(OBJ)/ast/stmt
	mkdir -p $(OBJ)/ast/type
	mkdir -p $(OBJ)/codegen
	mkdir -p $(OBJ)/parser
	mkdir -p $(OBJ)/statics
	mkdir -p $(BIN)

$(OBJ)/%.o: src/%.cpp
	$(CPP) $(CPPFLAGS) -o $@ -c $<

$(OBJ)/lexer.o: src/parser/lexer.l
	$(FLEX) -o lexer.cpp src/parser/lexer.l
	$(CPP) $(CPPFLAGS) -o $@ -c lexer.cpp

$(OBJ)/parser.o: src/parser/parser.y
	$(BISON) -v --defines=parser.hpp --output=parser.cpp src/parser/parser.y;
	$(CPP) $(CPPFLAGS) -o $@ -c parser.cpp

$(BIN)/cc: $(OBJS)
	$(CPP) $(LDFLAGS) -o $@ $(OBJS)

l4c: DIRECTORIES $(BIN)/cc
	llvm-config --version
	clang -S -emit-llvm -o l4lib.ll -c l4lib.c
	cp 411_wrapper.py $(BIN)/l4c

clean:
	rm -rf bin obj parser.cpp parser.hpp lexer.cpp parser.output l4lib.ll
