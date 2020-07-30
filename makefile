CC ?= gcc
CXX ?= g++
AR ?= ar

MKDIR_P = mkdir -p

BINARY_NAME = editor
LIB_NAME = libshadereditor.a
OBJ_DIR = ./obj
LIB_DIR = ./lib
LIB_PATH = $(LIB_DIR)/$(LIB_NAME)

CPPFLAGS := -MMD -MP
CXXFLAGS := -Wall -Wextra -Wpedantic -std=c++14 -I./src/
LDFLAGS = -lglfw -lpthread -lGL -lm

SRC_ED_DIR = ./src/shader_editor
OBJ_ED_DIR = ./obj/shader_editor
CPP_ED_PATHS := $(shell find $(SRC_ED_DIR) -name *.cpp)
CPP_ED_FILES := $(notdir ${CPP_ED_PATHS})
OBJ_ED_PATHS := $(CPP_ED_FILES:%=$(OBJ_ED_DIR)/%.o)
GCC_ED_MAKEFILES := $(OBJ_ED_PATHS:.o=.d)

SRC_GR_DIR = ./src/shader_graph
OBJ_GR_DIR = ./obj/shader_graph
CPP_GR_PATHS := $(shell find $(SRC_GR_DIR) -name *.cpp)
CPP_GR_FILES := $(notdir ${CPP_GR_PATHS})
OBJ_GR_PATHS := $(CPP_GR_FILES:%=$(OBJ_GR_DIR)/%.o)
GCC_GR_MAKEFILES := $(OBJ_GR_PATHS:.o=.d)

SRC_CO_DIR = ./src/shader_core
OBJ_CO_DIR = ./obj/shader_core
CPP_CO_PATHS := $(shell find $(SRC_CO_DIR) -name *.cpp)
CPP_CO_FILES := $(notdir ${CPP_CO_PATHS})
OBJ_CO_PATHS := $(CPP_CO_FILES:%=$(OBJ_CO_DIR)/%.o)
GCC_CO_MAKEFILES := $(OBJ_CO_PATHS:.o=.d)

SRC_IM_DIR = ./third_party/imgui
OBJ_IM_DIR = ./obj/imgui
CPP_IM_PATHS := $(shell find $(SRC_IM_DIR) -name *.cpp)
CPP_IM_FILES := $(notdir ${CPP_IM_PATHS})
OBJ_IM_PATHS := $(CPP_IM_FILES:%=$(OBJ_IM_DIR)/%.o)

$(BINARY_NAME): $(LIB_PATH)
	$(CXX) ./extra/main.cpp $(LIB_PATH) $(CXXFLAGS) $(LDFLAGS) -o $@

$(LIB_PATH): $(OBJ_IM_PATHS) $(OBJ_CO_PATHS) $(OBJ_GR_PATHS) $(OBJ_ED_PATHS)
	$(MKDIR_P) $(dir $@)
	$(AR) rcs $(LIB_PATH) $(OBJ_IM_PATHS) $(OBJ_CO_PATHS) $(OBJ_GR_PATHS) $(OBJ_ED_PATHS)


# shader_editor targets
$(OBJ_ED_DIR)/%.cpp.o: $(SRC_ED_DIR)/%.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I./third_party/imgui/ -c $< -o $@

# shader_graph targets
$(OBJ_GR_DIR)/%.cpp.o: $(SRC_GR_DIR)/%.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# shader_core targets
$(OBJ_CO_DIR)/%.cpp.o: $(SRC_CO_DIR)/%.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# imgui targets
$(OBJ_IM_DIR)/%.cpp.o: $(SRC_IM_DIR)/%.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) -std=c++14 -c $< -o $@

-include $(GCC_ED_MAKEFILES)
-include $(GCC_GR_MAKEFILES)

# iwyu

iwyu: CXX := /opt/iwyu-0.13/bin/include-what-you-use
iwyu: CXXFLAGS += -Xiwyu --mapping_file=./extra/iwyu.imp
iwyu: copy_headers $(OBJ_CO_PATHS) $(OBJ_GR_PATHS) $(OBJ_ED_PATHS)

clean:
	rm -rf ./$(OBJ_DIR)
	rm -rf ./$(LIB_DIR)
	rm -rf ./$(BINARY_NAME)
