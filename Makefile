OBJ = obj
BIN = bin
SRC = src
TESTS = tests
INC = $(SRC)

FLEX_SRC = $(SRC)/c.l
BISON_SRC = $(SRC)/c.y

FLEX_OUT = $(OBJ)/lex.yy.c
BISON_OUT = $(OBJ)/c.tab.c

SRCS := $(shell find $(SRC) -type f -name '*.cpp')
OBJS := $(addprefix $(OBJ)/,$(notdir $(SRCS:%.cpp=%.o)))

SRCS_TEST := $(SRCS) $(shell find $(TESTS) -type f -name '*.cpp')
OBJS_TEST := $(filter-out $(OBJ)/compiler.o,$(addprefix $(OBJ)/,$(notdir $(SRCS_TEST:%.cpp=%.o))))


TARGET = $(BIN)/c
TARGET_TESTS = $(BIN)/c_tests
CC = g++
CFLAGS = -std=c++11 -Wall -I$(INC) -I$(OBJ) -O1

VPATH=%.cpp $(SRC) $(TESTS)

.PHONY: default
default: compiler

$(OBJ)/%.o : %.cpp | build_dirs
	$(CC) $(CFLAGS) -c $< -o $@ -c

$(TARGET): $(OBJS) bison flex
	$(CC) $(CFLAGS) $(OBJS) $(FLEX_OUT) $(BISON_OUT) -o $@

$(TARGET_TESTS): bison flex $(OBJS_TEST)
	@echo $(OBJS_TEST)
	$(CC) $(CFLAGS) $(OBJS_TEST) $(FLEX_OUT) $(BISON_OUT) -o $@

.PHONY: compiler
compiler: $(TARGET)

.PHONY: tests
tests: $(TARGET_TESTS)
	./$(TARGET_TESTS)

.PHONY: ast
ast: $(TARGET)
	./$(TARGET) 2> $(OBJ)/ast.dot
	dot -Tpng $(OBJ)/ast.dot > $(OBJ)/ast.png

.PHONY: bison
bison: | build_dirs
	bison --defines=$(OBJ)/c.tab.h -o $(BISON_OUT) --report=state --report-file=$(OBJ)/c.output $(BISON_SRC)

.PHONY: flex
flex: | build_dirs
	flex -o $(FLEX_OUT) $(FLEX_SRC)

.PHONY: clean
clean:
	@rm -rf $(OBJ) $(BIN)

.PHONY: build_dirs
build_dirs:
	@mkdir -p $(BIN) $(OBJ)
