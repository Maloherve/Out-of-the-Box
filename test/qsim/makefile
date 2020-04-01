FLAGS := 
FLAGS +=-pedantic -Wall 
FLAGS += -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused
FLAGS += -Wnoexcept 
#FLAGS += -Werror 
all: clean math_test

SRC:= ../../quantumsim/grid_wave.cpp ../../quantumsim/qsystem1D.cpp

math_test:
	g++ -o qsim_test ${FLAGS} -std=c++17 qsim_test.cpp ${SRC} -I../../quantumsim

.PHONY: clean

clean:
	rm -f qsim_test

