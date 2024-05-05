CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SRC = tempstat.c temp_function.c
OBJ = $(SRC:.c=.o)
EXEC = report.exe

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
