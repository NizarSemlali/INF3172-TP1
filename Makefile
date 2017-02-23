CC = gcc
OBJECTS := $(patsubst %.c,%.o,$(wildcard *.c))
EXEC = tp1

$(EXEC): $(OBJECTS)
	$(CC) -o $(EXEC) $(OBJECTS)

%.o: %.c
	$(CC) -Wall -o $@ -c $<

.PHONY: clean

clean:
	rm -f $(EXEC) $(OBJECTS)
