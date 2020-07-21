CC ?= gcc
CXX ?= g++
AR ?= ar

MKDIR_P = mkdir -p

BINARY_NAME = editor
LIB_NAME = libshadereditor.a
OBJ_DIR = ./obj
INC_DIR = ./include
LIB_DIR = ./lib
LIB_PATH = $(LIB_DIR)/$(LIB_NAME)

CPPFLAGS := -MMD -MP
CXXFLAGS := -Wall -Wextra -Wpedantic -std=c++14
LDFLAGS = -lglfw -lpthread -lGL -lm

SRC_ED_DIR = ./src/editor
OBJ_ED_DIR = ./obj/editor
CPP_ED_PATHS := $(shell find $(SRC_ED_DIR) -name *.cpp)
CPP_ED_FILES := $(notdir ${CPP_ED_PATHS})
OBJ_ED_PATHS := $(CPP_ED_FILES:%=$(OBJ_ED_DIR)/%.o)
GCC_ED_MAKEFILES := $(OBJ_ED_PATHS:.o=.d)
INC_ED_DIR := $(INC_DIR)/shader_editor
INC_ED_FILES := shader_editor.h
INC_ED_PATHS := $(addprefix $(INC_ED_DIR)/,$(INC_ED_FILES))

SRC_GR_DIR = ./src/graph
OBJ_GR_DIR = ./obj/graph
CPP_GR_PATHS := $(shell find $(SRC_GR_DIR) -name *.cpp)
CPP_GR_FILES := $(notdir ${CPP_GR_PATHS})
OBJ_GR_PATHS := $(CPP_GR_FILES:%=$(OBJ_GR_DIR)/%.o)
GCC_GR_MAKEFILES := $(OBJ_GR_PATHS:.o=.d)
INC_GR_DIR := $(INC_DIR)/shader_graph
INC_GR_FILES := curves.h graph.h node.h node_enums.h node_id.h node_type.h ramp.h slot.h slot_id.h
INC_GR_PATHS := $(addprefix $(INC_GR_DIR)/,$(INC_GR_FILES))

SRC_CO_DIR = ./src/core
OBJ_CO_DIR = ./obj/core
CPP_CO_PATHS := $(shell find $(SRC_CO_DIR) -name *.cpp)
CPP_CO_FILES := $(notdir ${CPP_CO_PATHS})
OBJ_CO_PATHS := $(CPP_CO_FILES:%=$(OBJ_CO_DIR)/%.o)
GCC_CO_MAKEFILES := $(OBJ_CO_PATHS:.o=.d)
INC_CO_DIR := $(INC_DIR)/shader_core
INC_CO_FILES := config.h lerp.h rect.h util_enum.h vector.h
INC_CO_PATHS := $(addprefix $(INC_CO_DIR)/,$(INC_CO_FILES))

SRC_IM_DIR = ./third_party/imgui
OBJ_IM_DIR = ./obj/imgui
CPP_IM_PATHS := $(shell find $(SRC_IM_DIR) -name *.cpp)
CPP_IM_FILES := $(notdir ${CPP_IM_PATHS})
OBJ_IM_PATHS := $(CPP_IM_FILES:%=$(OBJ_IM_DIR)/%.o)
INC_IM_DIR := $(INC_DIR)
INC_IM_FILES := imconfig.h imgui.h imgui_impl_glfw.h imgui_impl_opengl2.h
INC_IM_PATHS := $(addprefix $(INC_IM_DIR)/,$(INC_IM_FILES))

$(BINARY_NAME): $(LIB_PATH) $(INC_ED_PATHS)
	$(CXX) ./extra/main.cpp $(LIB_PATH) $(CXXFLAGS) $(LDFLAGS) -I$(INC_DIR) -o $@

$(LIB_PATH): copy_headers $(OBJ_IM_PATHS) $(OBJ_CO_PATHS) $(OBJ_GR_PATHS) $(OBJ_ED_PATHS)
	$(MKDIR_P) $(dir $@)
	$(AR) rcs $(LIB_PATH) $(OBJ_IM_PATHS) $(OBJ_CO_PATHS) $(OBJ_GR_PATHS) $(OBJ_ED_PATHS)

copy_headers: $(INC_IM_PATHS) $(INC_CO_PATHS) $(INC_GR_PATHS) $(INC_ED_PATHS)

# shader_editor targets
$(INC_ED_DIR)/%.h: $(SRC_ED_DIR)/%.h
	$(MKDIR_P) $(dir $@)
	cp $< $@

$(OBJ_ED_DIR)/%.cpp.o: $(SRC_ED_DIR)/%.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) -I./include $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# shader_graph targets
$(INC_GR_DIR)/%.h: $(SRC_GR_DIR)/%.h
	$(MKDIR_P) $(dir $@)
	cp $< $@

$(OBJ_GR_DIR)/%.cpp.o: $(SRC_GR_DIR)/%.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) -I./include $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# shader_core targets
$(INC_CO_DIR)/%.h: $(SRC_CO_DIR)/%.h
	$(MKDIR_P) $(dir $@)
	cp $< $@

$(OBJ_CO_DIR)/%.cpp.o: $(SRC_CO_DIR)/%.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# imgui targets
$(INC_IM_DIR)/%.h: $(SRC_IM_DIR)/%.h
	$(MKDIR_P) $(dir $@)
	cp $< $@

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
	rm -rf ./$(INC_DIR)
	rm -rf ./$(LIB_DIR)
	rm -rf ./$(BINARY_NAME)
