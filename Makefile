CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I include

SRC = \
src/main.cpp \
src/allocator/list_allocator.cpp \
src/allocator/buddy_allocator.cpp \
src/cache/cache_level.cpp \
src/cache/cache_simulator.cpp

TEST_SRC = \
tests/allocator_tests.cpp \
src/allocator/list_allocator.cpp

CACHE_TEST_SRC = \
tests/cache_tests.cpp \
src/cache/cache_level.cpp \
src/cache/cache_simulator.cpp

RANDOM_SRC = \
tests/random_test.cpp \
src/allocator/list_allocator.cpp \
src/allocator/buddy_allocator.cpp

CACHE_RANDOM_SRC = \
tests/cache_random_test.cpp \
src/cache/cache_level.cpp \
src/cache/cache_simulator.cpp

TARGET = memsim
TEST_TARGET = allocator_tests
CACHE_TEST_TARGET = cache_tests
RANDOM_TARGET = random_test
CACHE_RANDOM_TARGET = cache_random_test

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

test:
	$(CXX) $(CXXFLAGS) $(TEST_SRC) -o $(TEST_TARGET)
	./$(TEST_TARGET)

cache_test:
	$(CXX) $(CXXFLAGS) $(CACHE_TEST_SRC) -o $(CACHE_TEST_TARGET)
	./$(CACHE_TEST_TARGET)

random:
	$(CXX) $(CXXFLAGS) $(RANDOM_SRC) -o $(RANDOM_TARGET)
	./$(RANDOM_TARGET)

cache_random:
	$(CXX) $(CXXFLAGS) $(CACHE_RANDOM_SRC) -o $(CACHE_RANDOM_TARGET)
	./$(CACHE_RANDOM_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET) $(CACHE_TEST_TARGET) $(RANDOM_TARGET) $(CACHE_RANDOM_TARGET)
