# Makefile pour 2048
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -Isrc -Iinclude `pkg-config --cflags sdl3`
LDFLAGS = `pkg-config --libs sdl3 sdl3-ttf`

# Fichiers sources
SOURCES = src/main.cpp src/Game.cpp src/Grid.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = 2048

# Règle par défaut
all: $(TARGET)

# Lier les objets
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

# Compiler chaque .cpp en .o
src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -f $(OBJECTS) $(TARGET)

# Exécuter
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run