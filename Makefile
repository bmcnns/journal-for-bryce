CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lncurses

TARGET = journal_for_bryce

SRC = journal.c

$(TARGET):$(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(TARGET)
