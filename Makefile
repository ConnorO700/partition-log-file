all: stdin_boost.o
	g++ stdin_boost.o -o ps7a -lboost_regex -lboost_date_time

stdin_boost.o: stdin_boost.cpp
	g++ -c stdin_boost.cpp -Wall -Werror -ansi -pedantic

clean: 
	rm *.o ps7a
