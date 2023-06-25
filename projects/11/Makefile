.PHONY:clean, cleanXML, run

CXX_FLAGS := -std=c++11 -ggdb -Wall -Werror
BIN := project11/build
SRC := project11/src
INCLUDE := project11/include

EXECUTABLE := JackCompiler

ARG ?= Seven
all:${BIN}/${EXECUTABLE}

${BIN}/${EXECUTABLE}:${SRC}/*.cpp $(INCLUDE)/*.h
	g++ $(CXX_FLAGS) ${SRC}/*.cpp -I$(INCLUDE) -o $@ 

run:
	./${BIN}/${EXECUTABLE} ${ARG}

clean:
	rm ${EXECUTABLE}/*
cleanXML:
	rm -f */*.xml */*.vm 
