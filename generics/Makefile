CSTANDARD := -std=c18
CWARNINGS := -Wall -Wextra -Wpedantic -Werror
OPTIMIZING := -O2

sources := $(shell find . -name *.c)
binaries := $(patsubst %.c,%,$(sources))


default: $(binaries)

%: %.c
	$(CC) $< -o $@ $(CSTANDARD) $(CWARNINGS) $(OPTIMIZING)

clean:
	rm -rf $(binaries)
