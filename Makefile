all : build
	

build :  421_Shvetsov.cpp
	g++ 421_Shvetsov.cpp -o421_Shvetsov -std=c++0x -O2 -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self \
	-Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body \
	-Wlogical-op \
	-Wmissing-field-initializers -g

graph : build
	./421_Shvetsov 1 2 3 4 -lts lts_m12.dot
	dot lts_m12.dot -Tpng -olts_m12.png
