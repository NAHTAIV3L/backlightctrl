CC=clang
BIN=backlightctrl

all:$(BIN)

$(BIN): $(BIN).o
	$(CC) $^ -o $(BIN)

%.o: %.c %.h
	$(CC) -c $^

install: $(BIN)
	cp $(BIN) /usr/local/bin/

clean:
	$(RM) *.o $(BIN) /usr/local/bin/$(BIN)

.PHONY: install clean all
