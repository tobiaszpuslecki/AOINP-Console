Menu:	*.cpp
	g++ -Wall -std=c++17 *.cpp -o Menu -lncurses && ./Menu


force:
	touch menu.cpp
	make
