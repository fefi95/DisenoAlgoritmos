CXX = g++
OPT = -g -Wall -O3

main: main.cpp
	$(CXX) $(OPT) main.cpp -o $@
	rm -f $*.c

runProject: runProject.cpp
	$(CXX) $(OPT) runProject.cpp -o $@
	rm -f $*.c

.PHONY: clean
clean:
	rm -f main runProject *.o *~
