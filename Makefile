all: gen sort check

gen: generate_big_file.cpp
	g++ --std=c++17 -Wall -Werror -o gen generate_big_file.cpp

sort: sort_big_file.cpp
	g++ --std=c++17 -Wall -Werror -o sort sort_big_file.cpp

check: check_sorting.cpp
	g++ --std=c++17 -Wall -Werror -o check check_sorting.cpp

start-tests: test_gen_and_sort.cpp
	g++ --std=c++17 -Wall -Werror -o tests test_gen_and_sort.cpp
	./tests

clean:
	rm -rf *.o gen sort check tests
