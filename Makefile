CC=clang
BIN=backlightctrl

all:debug

debug: $(BIN).o
	$(CC) $^ -o $(BIN)-debug

%.o: %.c %.h
	$(CC) -c $^

release: backlightctrl.o
	$(CC) $^ -o $(BIN)
	cp $(BIN) /usr/local/bin/

clean:
	rm *.o $(BIN) $(BIN)-debug /usr/local/bin/$(BIN)
