
addtest.exe: AddmireCore.cpp AddmireAlgorithms.cpp Sandbox.cpp
	g++ -Wall -Wextra -Wpedantic $^ -o $@ -std=c++17

clean:
	rm addtest.exe