test_read_rapl:
	gcc src/rapl.c tests/test_read_rapl.c -I./src -o test_read_rapl

profile:
	gcc src/profiler.c src/uba.c src/main.c src/rapl.c -lunwind -lunwind-generic -lunwind-ptrace -o leaf
