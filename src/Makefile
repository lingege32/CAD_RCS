CC=g++
FLAGS=-O3 -Wall -Wextra
SOURCES=main.cpp parser.cpp heuristic.cpp algorithm.cpp validate.cpp
OBJECTS=$(SOURCES:.cpp=.o)
HEADERS=arg_parser.hpp parser.hpp heuristic.hpp algorithm.hpp validate.hpp
EXECUTABLE=main


all: $(SOURCES) $(EXECUTABLE)


$(EXECUTABLE): $(OBJECTS)
	$(CC) $(FLAGS) $(OBJECTS) -o $@

.cpp.o: $(HEADERS)
	$(CC) $(FLAGS) $< -c -o $@

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)