.PHONY: all clean

SRC := src
INCLUDE := include

EXECUTABLE := JackCompiler

all:${EXECUTABLE}

${EXECUTABLE}:${SRC}/*.cpp $(INCLUDE)/*.h
	g++ ${SRC}/*.cpp -I$(INCLUDE) -o $@

clean:
	rm ${EXECUTABLE}/*