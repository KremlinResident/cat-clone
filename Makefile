CC = clang
CFLAGS = -std=c17 -D_POSIX_C_SOURCE=200809L -fsanitize=address,leak,undefined -fno-omit-frame-pointer \
	-Weverything -Werror -Wno-c++-compat -Wno-pre-c11-compat -Wno-pessimizing-move -Wno-move -Wno-nrvo \
	-Wno-unsafe-buffer-usage
INCL = ./src/
SRC = $(wildcard ./src/*.c)
OBJS = $(patsubst ./src/%.c, ./build/%.o, $(SRC))

build: $(OBJS)
	$(CC) -g $(CFLAGS) -I$(INCL) -O0 $(OBJS) -o cat-clone.out

./build/%.o: ./src/%.c
	mkdir -p ./build/
	$(CC) -c -g $(CFLAGS) -I$(INCL) -O0 $< -o $@

clean:
	rm -rf ./build/ cat-clone.out

.PHONY: clean build