SRCS := $(shell find -name main.c)
BINS := $(SRCS:main.c=main.out)

all: $(BINS)

./%/main.out: ./%/main.c
	@echo "Compiling $< into $@"
	@$(CC) $(CFLAGS) $< -o $@

clean:
	@echo "Cleaning up..."
	@find -name main.out -exec rm -f {} +

test: all
	@echo "Running tests..."
	@total=0; correct=0; \
	for bin in $(BINS); do \
		dir=$$(dirname $$bin); \
		formatted_dir=$$(echo $$dir | sed 's|^\./||' | sed 's|/|-|g'); \
		answer="$$dir/answer.txt"; \
		if [ -f $$answer ]; then \
			output=$$(cd $$dir && ./main.out); \
			if echo -n "$$output" | diff -q - $$answer > /dev/null; then \
				echo "$$formatted_dir: PASS"; \
				correct=$$((correct + 1)); \
			else \
				echo "$$formatted_dir: FAIL"; \
				echo "Expected:"; cat $$answer; echo " ";\
				echo "Got:"; echo "$$output"; echo " ";\
			fi; \
			total=$$((total + 1)); \
		else \
			echo "$$answer not found for $$bin"; \
		fi; \
	done; \
	echo "$$correct/$$total tests passed."; \
	if [ $$total -ne $$correct ]; then \
		$(error Some tests failed!)


.PHONY: all clean test