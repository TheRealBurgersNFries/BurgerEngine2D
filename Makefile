# C++ compiler
CXX 			:=  g++


# Source and build directories
SRC_DIR 		:= src
OBJ_DIR 		:= build/obj

# Separate object directory for the debug build
DEBUG_OBJ_DIR 	:= build/debug/obj

# Test source and object directories
TEST_DIR		:= test
TEST_OBJ_DIR	:= build/obj/test

# Output executable names
TARGET			:= minerpirates
TEST_TARGET		:= test_runner

# Output path for the debug executable
DEBUG_TARGET 	:= build/debug/minerpirates


# Find all C++ source files in the application and test directories
SRCS 			:= $(wildcard $(SRC_DIR)/*.cpp)
TEST_SRCS		:= $(wildcard $(TEST_DIR)/*.cpp)

# Convert source file paths into their corresponding object file paths
OBJS 			:= $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TEST_OBJS		:= $(TEST_SRCS:$(TEST_DIR)/%.cpp=$(TEST_OBJ_DIR)/%.o)

# Exclude the application's main() entry point from the test executable
APP_OBJS 		:= $(filter-out $(OBJ_DIR)/main.o,$(OBJS))

# Generate object file paths for the debug build
DEBUG_OBJS 		:= $(SRCS:$(SRC_DIR)/%.cpp=$(DEBUG_OBJ_DIR)/%.o)

# Debug-build objects excluding the application's main() entry point
DEBUG_APP_OBJS 	:= $(filter-out $(DEBUG_OBJ_DIR)/main.o,$(DEBUG_OBJS))


# Path to the CppUTest dependency
CPPUTEST_HOME 	:= external/cpputest


# Raylib source directory, static library, and compiler include path
RAYLIB_DIR 		:= external/raylib
RAYLIB_SRC 		:= $(RAYLIB_DIR)/src
RAYLIB_LIB 		:= $(RAYLIB_SRC)/libraylib.a
RAYLIB_CFLAGS 	:= -I$(RAYLIB_SRC)

# System libraries required by the raylib Wayland desktop build
RAYLIB_LIBS 	:= -lGL -lm -lpthread -ldl -lrt 
					-lwayland-client -lwayland-cursor 
					-lwayland-egl -lxkbcommon


# Standard compiler flags:
# C++23, common warnings, warnings as errors, strict conformance,
# dependency-file generation, and suppression of separate .o/.d targets
CXXFLAGS    	:= -std=c++23 -Wall -Wextra -Werror -pedantic -MP -MMD

# Add raylib's header directory to the compiler's include search path
CXXFLAGS 		+= $(RAYLIB_CFLAGS)

# Force-include CppUTest's memory leak detector macros in each translation unit
CXXFLAGS		+= -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorNewMacros.h

# Compiler flags for test sources, including CppUTest's headers
CXXTESTFLAGS 	:= $(CXXFLAGS) -I$(CPPUTEST_HOME)/include

# CppUTest library search path and linker flag
LD_LIBS 		:= -L$(CPPUTEST_HOME)/lib -lCppUTest


# Debug compiler flags:
# -g3 includes detailed debugging information
# -O0 disables optimizations to make stepping through code easier
# -DDEBUG defines the DEBUG preprocessor macro
DEBUG_FLAGS 	:= -std=c++23 -Wall -Wextra -Werror -pedantic  
					-g3 -O0 -DDEBUG -MP -MMD $(RAYLIB_CFLAGS)


# Include generated dependency files so header changes trigger recompilation.
# The leading '-' tells Make to tolerate dependency files that don't exist yet.
-include $(OBJS:.o=.d) $(TEST_OBJS:.o=.d) $(DEBUG_OBJS:.o=.d)


# Declare user-facing targets that don't represent files
.PHONY: all test clean raylib debug run-debug


# Default application build
all: $(TARGET) raylib

# Build the test executable and run the tests
test: $(TEST_TARGET) raylib
	@echo "Running Tests:"
	./$(TEST_TARGET)


# Build raylib from its source directory using its Makefile.
# Select the desktop platform, enable Wayland, and disable X11.
raylib:
	$(MAKE) -C $(RAYLIB_SRC) 
	    PLATFORM=PLATFORM_DESKTOP 
	    GLFW_LINUX_ENABLE_WAYLAND=TRUE 
	    GLFW_LINUX_ENABLE_X11=FALSE


# Build the debug executable
debug: $(DEBUG_TARGET)

# Build the debug executable and immediately launch it under GDB
run-debug: $(DEBUG_TARGET)
	gdb -ex run --args ./$(DEBUG_TARGET)


# Link the test executable from application objects (excluding main.o)
# and test objects, then link against CppUTest.
$(TEST_TARGET): $(APP_OBJS) $(TEST_OBJS)
	@echo "Linking Tests..."
	$(CXX) $^ $(LD_LIBS) -o $@


# Link the normal application executable from its object files and raylib
$(TARGET): $(OBJS)
	@echo "Linking..."
	$(CXX) $(CXXFLAGS) $^ $(RAYLIB_LIB) -o $@
	

# Compile an application source file into a normal-build object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling..."
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@


# Compile a test source file into a test object file
$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	@echo "Compiling Tests..."
	@mkdir -p $(TEST_OBJ_DIR)
	$(CXX) $(CXXTESTFLAGS) -c $< -o $@


# Compile an application source file with debug flags into the debug object directory
$(DEBUG_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling Debug build..."
	@mkdir -p $(dir $@)
	$(CXX) $(DEBUG_FLAGS) -c $< -o $@


# Link the debug executable from debug objects and raylib.
# Create the output directory if it doesn't exist.
$(DEBUG_TARGET): $(DEBUG_OBJS) | raylib
	@echo "Linking Debug Build..."
	@mkdir -p $(dir $@)
	@echo "Linking debug application..."
	$(CXX) $(DEBUG_OBJS) $(RAYLIB_LIB) $(RAYLIB_LIBS) -o $@


# Remove generated build artifacts and executables
clean:
	@echo "Cleaning..."
	rm -rf build $(TARGET) $(TEST_TARGET)