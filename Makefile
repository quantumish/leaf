test_read_rapl:
	gcc src/rapl.c tests/test_read_rapl.c -I./src -o test_read_rapl

profile:
	g++ src/profiler.cpp src/main.cpp src/rapl.cpp -lunwind -lunwind-generic -lunwind-ptrace -o leaf
