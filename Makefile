CXX      := g++
CXXFLAGS := -Wall -Wextra -std=c++11 -Iinclude
TARGET   := passgen

SRCS     := main.cpp src/passgen.cpp src/key.cpp src/file_manager.cpp
OBJS     := $(SRCS:.cpp=.o)

TEST_SRCS := tests/test_file_manager.cpp src/file_manager.cpp
TEST_OBJS := $(TEST_SRCS:.cpp=.o)
TEST_BIN  := test_file_manager

.PHONY: all compile run test clean

all: compile

compile: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: compile
	./$(TARGET)

$(TEST_BIN): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -f $(OBJS) $(TARGET) $(TEST_OBJS) $(TEST_BIN) test_passwords.json
