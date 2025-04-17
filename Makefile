TARGET = bin/calculator
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))
CFLAGS = `pkg-config --cflags gtk+-3.0` -Iinclude
LDFLAGS = `pkg-config --libs gtk+-3.0`

default: $(TARGET)

clean:
	rm -f obj/*.o
	rm -f bin/*
	rm -f *.db

$(TARGET): $(OBJ)
	gcc -o $@ $^ $(LDFLAGS)

obj/%.o: src/%.c
	gcc $(CFLAGS) -c $< -o $@
