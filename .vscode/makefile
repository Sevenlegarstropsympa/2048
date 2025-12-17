# Makefile pour 2048
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -Isrc -Iinclude -I/mingw64/include -I/mingw64/include/SDL3
LDFLAGS = -L/mingw64/lib -lSDL3 -lSDL3_ttf -lSDL3_image -Wl,-subsystem,windows

# Fichiers sources
SOURCES = src/main.cpp src/Game.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = 2048.exe

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
	rm -f $(OBJECTS) $(TARGET) *.dll

# Copier DLLs
dll:
	-cp /mingw64/bin/SDL3.dll .
	-cp /mingw64/bin/SDL3_ttf.dll .
	-cp /mingw64/bin/SDL3_image.dll .

# Exécuter
run: $(TARGET) dll
	./$(TARGET)

.PHONY: all clean run dll