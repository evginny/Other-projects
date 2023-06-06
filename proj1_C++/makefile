all: proj2.exe test_list.exe

proj2.exe: mem_manager.o MyMemory.o MyMemoryBlock.o
	g++ -std=c++11 mem_manager.o MyMemory.o MyMemoryBlock.o		-o proj2.exe

mem_manager.o: mem_manager.cpp
	g++ -std=c++11 -c mem_manager.cpp	-o mem_manager.o

MyMemory.o: MyMemory.cpp
	g++ -std=c++11 -c MyMemory.cpp		-o MyMemory.o

MyMemoryBlock.o: MyMemoryBlock.cpp
	g++ -std=c++11 -c MyMemoryBlock.cpp 	-o MyMemoryBlock.o

test_list.exe: test_list.cpp
	g++ -std=c++11 test_list.cpp		-o test_list.exe


clean:
	rm *.o
	rm *.x