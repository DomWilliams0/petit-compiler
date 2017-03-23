OBJ = obj
BIN = bin
SRC = src
INC = $(SRC)

FLEX_SRC = $(SRC)/c.l
BISON_SRC = $(SRC)/c.y

FLEX_OUT = $(OBJ)/lex.yy.c
BISON_OUT = $(OBJ)/c.tab.c

SRCS := $(shell find $(SRC) -type f -name '*.cpp')
OBJS := $(addprefix $(OBJ)/,$(notdir $(SRCS:%.cpp=%.o)))

TARGET = c
CC = g++
CFLAGS = -std=c++11 -Wall -g -I$(INC) -O0

VPATH=%.cpp $(SRC)

.PHONY: default
default: $(TARGET)

$(OBJ)/%.o : %.cpp | build_dirs
	$(CC) $(CFLAGS) -c $< -o $@ -c

$(TARGET): $(OBJS) bison flex
	$(CC) $(CFLAGS) $(OBJS) $(FLEX_OUT) $(BISON_OUT) -o $@

.PHONY: bison
bison:
	bison --defines=$(OBJ)/c.tab.h -o $(BISON_OUT) --report=state --report-file=$(OBJ)/c.output $(BISON_SRC)

.PHONY: flex
flex:
	flex -o $(FLEX_OUT) $(FLEX_SRC)

.PHONY: clean
clean:
	@rm -rf $(OBJ) $(BIN) $(TARGET)

.PHONY: build_dirs
build_dirs:
	@mkdir -p $(BIN) $(OBJ)
