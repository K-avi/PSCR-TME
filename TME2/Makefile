DEFAULT:=all
TARGET:=tme2
CC:=g++ 
CFLAGS:=-Wall -Wextra -Werror -std=c++20 -pedantic -g


SRC:=$(wildcard *.cpp)

$(TARGET): 
	$(CC) $(SRC) $(CFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)


