CXX      := g++
CXXFLAGS := -Wall -Wextra -std=c++11 -Iinclude
TARGET   := passgen

SRCS     := main.cpp src/passgen.cpp src/key.cpp
OBJS     := $(SRCS:.cpp=.o)

.PHONY: all compile run clean

all: compile

compile: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: compile
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
