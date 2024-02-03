test_read_rapl:
	gcc src/rapl.c tests/test_read_rapl.c -I./src -o test_read_rapl

profile:
	g++ -std=c++20 src/profiler.cpp src/main.cpp src/rapl.cpp src/leaffunction.cpp src/flametree.cpp src/json.cpp -lunwind -lunwind-generic -lunwind-ptrace -ludis86 -o leaf
