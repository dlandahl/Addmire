
addtest.exe: AddmireCore.cpp AddmireAlgorithms.cpp Sandbox.cpp
	g++ $^ -o $@ -std=c++17