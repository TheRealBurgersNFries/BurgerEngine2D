
# C++ compiler
CXX             := g++

# Source and build directories
SRC_DIR         := src
WRAP_SRC_DIR    := src/wrappers
OBJ_DIR         := build/obj
DEBUG_OBJ_DIR   := build/debug/obj

# Include directory
INC_DIR         := inc
WRAP_DIR        := inc/wrappers

# Test directories
TEST_DIR            := test
TEST_OBJ_DIR        := build/obj/test
DEBUG_TEST_OBJ_DIR  := build/debug/obj/test
MOCK_DIR            := test/mocks

# Output directories
BIN_DIR         := build/bin
LIB_DIR         := build/lib

# Output targets
TARGET              := $(LIB_DIR)/libBurgerEngine2D.so
TEST_TARGET         := $(BIN_DIR)/test/testrunner
DEBUG_TARGET        := $(LIB_DIR)/debug/libBurgerEngine2D.so
DEBUG_TEST_TARGET   := $(BIN_DIR)/debug/testrunner

# Source files
SRCS            := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(WRAP_SRC_DIR)/*.cpp)
TEST_SRCS       := $(wildcard $(TEST_DIR)/*.cpp) $(wildcard $(MOCK_DIR)/*.cpp)

# Object files
OBJS            := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
TEST_OBJS       := $(patsubst $(TEST_DIR)/%.cpp,$(TEST_OBJ_DIR)/%.o,$(TEST_SRCS))

# Wrapper object files
WRAP_OBJS       := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(wildcard $(WRAP_SRC_DIR)/*.cpp))

# Library object files
LIB_OBJS        := $(filter-out $(OBJ_DIR)/main.o,$(OBJS))

# Test application object files
TEST_APP_OBJS   := $(filter-out $(OBJ_DIR)/main.o $(WRAP_OBJS),$(OBJS))

# Debug object files
DEBUG_OBJS      := $(patsubst $(SRC_DIR)/%.cpp,$(DEBUG_OBJ_DIR)/%.o,$(SRCS))
DEBUG_TEST_OBJS := $(patsubst $(TEST_DIR)/%.cpp,$(DEBUG_TEST_OBJ_DIR)/%.o,$(TEST_SRCS))

# Debug wrapper object files
DEBUG_WRAP_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(DEBUG_OBJ_DIR)/%.o,$(wildcard $(WRAP_SRC_DIR)/*.cpp))

# Debug library object files
DEBUG_LIB_OBJS  := $(filter-out $(DEBUG_OBJ_DIR)/main.o,$(DEBUG_OBJS))

# Debug test application object files
DEBUG_TEST_APP_OBJS := $(filter-out $(DEBUG_OBJ_DIR)/main.o $(DEBUG_WRAP_OBJS),$(DEBUG_OBJS))

# External libraries
CPPUTEST_HOME   := external/cpputest
RAYLIB_DIR      := external/raylib
RAYLIB_SRC      := $(RAYLIB_DIR)/src
RAYLIB_LIB      := $(RAYLIB_SRC)/libraylib.a

RAYLIB_CFLAGS   := -I$(RAYLIB_SRC)
RAYLIB_LIBS     := -lGL -lm -lpthread -ldl -lrt -lwayland-client -lwayland-cursor -lwayland-egl -lxkbcommon
TBB_LIBS        := -ltbb
LD_LIBS         := -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt

# Compiler flags
CXXFLAGS        := -std=c++23 -Wall -Wextra -Werror -pedantic -MP -MMD -fPIC
CXXFLAGS        += -I$(INC_DIR) -I$(WRAP_DIR) $(RAYLIB_CFLAGS)

CXXTESTFLAGS    := $(CXXFLAGS) -I$(MOCK_DIR) -I$(CPPUTEST_HOME)/include

# Debug flags
DEBUG_FLAGS     := -std=c++23 -Wall -Wextra -Werror -pedantic -g3 -O0 -DDEBUG -MP -MMD -fPIC
DEBUG_FLAGS     += -I$(INC_DIR) -I$(MOCK_DIR) -I$(WRAP_DIR) $(RAYLIB_CFLAGS)

DEBUG_TEST_FLAGS := $(DEBUG_FLAGS) -I$(CPPUTEST_HOME)/include

# Dependency files
DEPS            := $(OBJS:.o=.d) $(TEST_OBJS:.o=.d) \
                   $(DEBUG_OBJS:.o=.d) $(DEBUG_TEST_OBJS:.o=.d)

# Default target
.PHONY: all test debug clean run-debug raylib

all: $(TARGET) $(TEST_TARGET)

# Build raylib
raylib: $(RAYLIB_LIB)

$(RAYLIB_LIB):
	$(MAKE) -C $(RAYLIB_SRC) clean
	$(MAKE) -C $(RAYLIB_SRC) PLATFORM=PLATFORM_DESKTOP \
	    GLFW_LINUX_ENABLE_WAYLAND=TRUE GLFW_LINUX_ENABLE_X11=FALSE

# Build shared library
$(TARGET): $(LIB_OBJS)
	@mkdir -p $(dir $@)
	$(CXX) -shared -o $@ $(LIB_OBJS) $(TBB_LIBS)

# Build test runner
$(TEST_TARGET): $(TEST_APP_OBJS) $(TEST_OBJS) $(RAYLIB_LIB)
	@mkdir -p $(dir $@)
	$(CXX) $^ $(RAYLIB_LIBS) $(TBB_LIBS) $(LD_LIBS) -o $@

# Build debug shared library
$(DEBUG_TARGET): $(DEBUG_LIB_OBJS)
	@mkdir -p $(dir $@)
	$(CXX) -shared -o $@ $(DEBUG_LIB_OBJS) $(TBB_LIBS)

# Build debug test runner
$(DEBUG_TEST_TARGET): $(DEBUG_TEST_APP_OBJS) $(DEBUG_TEST_OBJS) $(RAYLIB_LIB)
	@mkdir -p $(dir $@)
	$(CXX) $(DEBUG_TEST_APP_OBJS) $(DEBUG_TEST_OBJS) \
	    $(RAYLIB_LIB) $(RAYLIB_LIBS) $(TBB_LIBS) $(LD_LIBS) -o $@

# Compile application objects
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile test objects
$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXTESTFLAGS) -c $< -o $@

# Compile debug application objects
$(DEBUG_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(DEBUG_FLAGS) -c $< -o $@

# Compile debug test objects
$(DEBUG_TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(DEBUG_TEST_FLAGS) -c $< -o $@

# Run tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Build debug targets
debug: $(DEBUG_TARGET) $(DEBUG_TEST_TARGET)

# Run debug tests
run-debug: $(DEBUG_TEST_TARGET)
	./$(DEBUG_TEST_TARGET)

# Clean build files
clean:
	rm -rf build

# Include generated dependencies
-include $(DEPS)
