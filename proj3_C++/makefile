driver.exe: proj4_driver.o bet.o
	g++ proj4_driver.o bet.o	-o driver.exe
	
proj4_driver.o: proj4_driver.cpp	
	g++ -c proj4_driver.cpp 	-o proj4_driver.o
	
bet.o: bet.cpp
	g++ -c bet.cpp 	-o bet.o

clean:
	rm *.o
	rm *.exe