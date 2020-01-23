SHELL := bash
.ONESHELL:
.SHELLFLAGS := -eu -o pipefail -c
.DELETE_ON_ERROR:

# get the real compiler
ifeq ($(CC),cc)
CC := $(shell readlink -f $(shell which cc))
endif

OPTIMIZING ?= -O2
CSTANDARD ?= -std=c18
CWARNINGS ?= -Wall -Wextra -Wpedantic -Werror

# GCC thinks that _Generic does not use the argument variable
# so we have to trick him
ifneq ($(findstring gcc,$(CC)),)
CWARNINGS += -Wno-unused-but-set-variable
endif


sources := $(shell find . -name "*.c")
binaries := $(patsubst %.c,%,$(sources))


default: $(binaries)

%: %.c
	$(CC) $< -o $@ $(CSTANDARD) $(CWARNINGS) $(OPTIMIZING)

clean:
	rm -rf $(binaries)
