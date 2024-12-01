SRCS := $(shell find -name main.c)
BINS := $(SRCS:main.c=main.elf)

all: $(BINS)

./%/main.elf: ./%/main.c
	@echo "Compiling $< into $@"
	@$(CC) $(CFLAGS) $< -o $@

clean:
	@echo "Cleaning up..."
	@find -name main.elf -exec rm -f {} +

.PHONY: all clean