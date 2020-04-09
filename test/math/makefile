FLAGS := 
FLAGS +=-pedantic -Wall 
FLAGS += -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused
FLAGS += -Wnoexcept 
#FLAGS += -Werror 
all: clean math_test

math_test:
	g++ -o math_test ${FLAGS} -std=c++17 math_test.cpp ../../quantumsim/grid/wave.cpp -I../../quantumsim

.PHONY: clean

clean:
	rm -f math_test

