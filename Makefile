test_read_rapl:
	gcc src/rapl.c tests/test_read_rapl.c -I./src -o test_read_rapl

profile:
	gcc src/*.c -lunwind -lunwind-generic -lunwind-ptrace -o leaf
