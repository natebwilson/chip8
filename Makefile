CC = cc
CFLAGS = -Wall -Wextra -pedantic 
LDFLAGS = -lncursesw -lncurses

SRC = chip8.c video.c file.c instructions.c global.c
OBJ = $(SRC:.c=.o)

TARGET = chip8

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

.PHONY: all clean