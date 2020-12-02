CC = gcc
CCFLAGS = -g -O2 -Werror -Wall -Wextra -Wshadow

LIB_TARGET = perceptron.a
TST_TARGET = perceptron_test

SRC = $(shell find ./src -name "*.c")
OBJ = $(patsubst ./src/%.c, ./obj/%.o, $(SRC))

.phony: all
all: lib tests

.phony: dirs
dirs:
	@mkdir -p obj lib bin
	
.phony: lib
lib: dirs $(OBJ)
	@mkdir -p ./lib
	@echo [AR] ./lib/$(LIB_TARGET)
	@ar -crs ./lib/$(LIB_TARGET) $(OBJ)



obj/%.o: src/%.c
	@echo [CC] $@
	@$(CC) -c $< -o $@ $(CCFLAGS) -Iinclude 


TESTSRC = $(shell find ./tests -name "*.c")
TESTEXE = $(patsubst ./tests/%.c, ./bin/%, $(TESTSRC))
CSVSRC = rainy-csv/src/csv.c
TESTINC = -Iinclude -Irainy-csv/src

.phony: tests
tests: dirs $(TESTEXE)
	
bin/%: tests/%.c
	@echo [CC] $@
	@$(CC) $< ./lib/$(LIB_TARGET) $(CSVSRC) -o $@ $(CCFLAGS) $(TESTINC) -lm
	
.phony: clean
clean:
	@rm -r obj
	
.phony: clean_all
clean_all:
	@rm -r obj lib bin
