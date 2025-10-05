# ==========================================
#   HW1_P76144265 Makefile
# ==========================================

SRC_DIR := ./src/
OBJ_DIR := ./obj/
TARGET  := HW1_P76144265

CXX      := g++
CXXFLAGS := -O3 -Wall -std=c++11
LIB      :=
INCLUDE  :=

# ==========================================

SRCS := $(wildcard $(SRC_DIR)*.cpp)
OBJS := $(patsubst $(SRC_DIR)%.cpp, $(OBJ_DIR)%.o, $(SRCS))

# ==========================================

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIB) -o $(TARGET)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJ_DIR)*.o core *~
