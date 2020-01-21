SHELL := bash
.ONESHELL:
.SHELLFLAGS := -eu -o pipefail -c
.DELETE_ON_ERROR:

CSTANDARD := -std=c18
CWARNINGS := -Wall -Wextra -Wpedantic -Werror

# GCC think that _Generic does not use the argument variable
# so we have to trick him
CWARNINGS += -Wno-unused-but-set-variable

OPTIMIZING := -O2

sources := $(shell find . -name "*.c")
binaries := $(patsubst %.c,%,$(sources))


default: $(binaries)

%: %.c
	$(CC) $< -o $@ $(CSTANDARD) $(CWARNINGS) $(OPTIMIZING)

clean:
	rm -rf $(binaries)
