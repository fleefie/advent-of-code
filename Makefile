SRCS := $(shell find -name main.c)
BINS := $(SRCS:main.c=main.elf)

all: $(BINS)

./%/bin: ./%/main.c
	@echo "Compiling $< into $@"
	@$(CC) $(CFLAGS) $< -o $@

clean:
	@echo "Cleaning up..."
	@find -name bin -exec rm -f {} +

.PHONY: all clean