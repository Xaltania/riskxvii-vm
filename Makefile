TARGET = vm_riskxvii

CC = gcc

CFLAGS     = -c -Wall -Wvla -Werror -O0 -g -std=c11
ASAN_FLAGS = -fsanitize=address
SRC        = vm_riskxvii.c cpu.c vm.c memory.c instructions.c
OBJ        = $(SRC:.c=.o)

all:$(TARGET)

$(TARGET):$(OBJ)
	$(CC) $(ASAN_FLAGS) -o $@ $(OBJ)

.SUFFIXES: .c .o

.c.o:
	 $(CC) $(CFLAGS) $(ASAN_FLAGS) $<

run:
	./$(TARGET)

test:
	test_count=0; \
	for program in tests/*/; \
	do \
		program=$$(basename $$program);\
		for test in tests/$$program/*.in; \
		do \
		echo Test: $$test;\
			test_name=$$(basename $$test .in); \
			expected_output=tests/$$program/$$test_name.out; \
			./$(TARGET) examples/$$program/$$program.mi < $$test | diff - $$expected_output || echo "$$test_name: FAILED\n"; \
			test_count=$$((test_count+1)); \
		done; \
	done; \
	echo "$$test_count tests ran"

clean:
	rm -f *.o *.obj $(TARGET)
