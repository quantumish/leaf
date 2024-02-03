.PHONY: test_baseline_stats profile test_read_rapl

clean:
	rm test_read_rapl test_baseline_stats test_delta_rapl
	
test_read_rapl:
	g++ src/rapl.cpp tests/test_read_rapl.c -I./src -o test_read_rapl

test_baseline_stats:
	g++ src/rapl.cpp src/statistics.cpp tests/stats_tests/stest_baseline.cpp -I./src -o test_baseline_stats

test_delta_rapl:
	g++ src/rapl.cpp tests/energy_tests/src/etest_delta_rapl.cpp -I./src -o test_delta_rapl

profile:
	g++ -std=c++20 src/profiler.cpp src/main.cpp src/rapl.cpp src/leaffunction.cpp src/flametree.cpp src/json.cpp -lunwind -lunwind-generic -lunwind-ptrace -lZydis -o leaf -I /usr/include/zydis/dependencies/zycore/include
