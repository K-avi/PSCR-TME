
TARGET:=test

# Flags to compiler
CPPFLAGS = -std=c++20 -g -Wall -Wextra -Werror -pedantic 
CXX = g++

CLEAN:=clean

$(TARGET): 
	$(CXX) $(CPPFLAGS) src/*.cpp -o $(TARGET)


$(CLEAN): 
	rm src/*.o test
