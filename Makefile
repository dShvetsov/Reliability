all : task1.cpp
	g++ 421_Shvetsov.cpp -o421_Shvetsov -std=c++0x -O2 -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self \
	-Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body \
	-Wlogical-op \
	-Wmissing-field-initializers 
