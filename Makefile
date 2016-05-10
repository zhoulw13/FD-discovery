Main : FDD.o Readfile.o Main.o
	@g++ FDD.o Readfile.o Main.o -o Main
	@echo ok
Main.o : Main.cpp FDD.h Readfile.h
	@g++ -c Main.cpp
FDD.o : FDD.cpp FDD.h
	@g++ -c FDD.cpp
Readfile.o : Readfile.cpp Readfile.h
	@g++ -c Readfile.cpp

clean : 
	@rm -f *.o
	@rm -f Main
	@rm -f *~
	@echo clean

