.PHONY: all clean
CXX_FLAGS := -std=c++11 -ggdb -Wall -Werror

BIN := build
SRC := src
INCLUDE := include

EXECUTABLE := JackCompiler

all:${BIN}/${EXECUTABLE}

${BIN}/${EXECUTABLE}:${SRC}/*.cpp $(INCLUDE)/*.h
	g++ $(CXX_FLAGS) ${SRC}/*.cpp -I$(INCLUDE) -o $@

clean:
	rm ${EXECUTABLE}/*