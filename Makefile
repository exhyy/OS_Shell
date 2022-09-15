TARGET := myshell
SRC := ${wildcard src/*.c}
OBJ := ${patsubst %.c,%.o,${SRC}}
OBJ := ${patsubst src/%,build/%,${OBJ}}
C_FLAGS := -Iinclude -g -Wall

all: ${OBJ}
	gcc $^ -o ${TARGET} ${C_FLAGS}

build/%.o: src/%.c dir
	gcc -o $@ -c $< ${C_FLAGS}

dir:
	@mkdir -p build

clean:
	rm -rf build
	rm -rf ${TARGET}