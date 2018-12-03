
addtest.exe: AddmireCore.cpp AddmireAlgorithms.cpp Sandbox.cpp
	g++ -Wall -Wextra -Wpedantic $^ -o $@ -std=c++17

.PHONY: mac
mac: AddmireCore.cpp AddmireAlgorithms.cpp Sandbox.cpp
	g++ -Wall -Wextra -Wpedantic $^ -o addtest.out -std=c++17

.PHONY: clean
clean:
	rm addtest.exe
