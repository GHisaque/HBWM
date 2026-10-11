EXAMPLE_DIRS := $(patsubst %/,%,$(dir $(wildcard examples/*/Makefile)))

.PHONY: examples $(EXAMPLE_DIRS)

examples: $(EXAMPLE_DIRS)

$(EXAMPLE_DIRS):
	$(MAKE) -C $@