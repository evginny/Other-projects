proj5.exe: passserver.o proj5.o
	g++ -std=c++11 -lcrypt passserver.o proj5.o		-o proj5.exe

proj5.o: proj5.cpp
	g++ -std=c++11 -c proj5.cpp		-o proj5.o

passserver.o: passserver.cpp
	g++ -std=c++11 -lcrypt -c passserver.cpp	-o passserver.o

clean:
	rm *.exe
	rm *.o