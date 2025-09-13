TARGET = bin/final
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))
CFLAGS= -Iinclude -Ideps -g -Wall -Wextra
LIBS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm

default: $(TARGET)

clean:
	rm -f obj/*.o
	rm -f bin/*

$(TARGET): $(OBJ)
	gcc -o $@ $^ $(LIBS)

obj/%.o : src/%.c
	gcc -c $< -o $@ ${CFLAGS} 

compile_flags.txt: force
	@echo "Generating compile_flags.txt for IDE support"
	@echo $(CFLAGS) | tr ' ' '\n' > $@

force:

