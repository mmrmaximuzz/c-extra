SHELL := bash
.ONESHELL:
.SHELLFLAGS := -eu -o pipefail -c
.DELETE_ON_ERROR:

OPTIMIZING ?= -O2
CSTANDARD ?= -std=c18
CWARNINGS ?= -Wall -Wextra -Wpedantic -Werror


sources := $(shell find . -name "*.c")
binaries := $(patsubst %.c,%,$(sources))


default: $(binaries)

%: %.c
	$(CC) $< -o $@ $(CSTANDARD) $(CWARNINGS) $(OPTIMIZING)

clean:
	rm -rf $(binaries)
