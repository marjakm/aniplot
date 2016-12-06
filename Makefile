#!/usr/bin/make

CC = gcc -O2
CXX = g++ -O2 -std=gnu++11

BUILD_DIR=./build
DIST_DIR=./dist
DIRS=$(BUILD_DIR) $(DIST_DIR)


UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux) #LINUX
	PLT_INC=$(shell pkg-config sdl2 --cflags)
	PLT_LNK=$(shell pkg-config sdl2 --libs) -ldl -lGL -lm
	CIMGUI_DYLIB=$(DIST_DIR)/cimgui.so
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	PLT_INC=-I/usr/local/include/ \
			-I/usr/local/include/SDL2/
	PLT_LNK=-lSDL2 \
			-lSDL2main \
			-framework OpenGL \
			-framework CoreFoundation
	CIMGUI_DYLIB=$(DIST_DIR)/cimgui.dylib
endif


LIB=$(DIST_DIR)/libaniplot.a
ANLIB_DIR=./src/libaniplot
ANLIB_SRCS=$(ANLIB_DIR)/imgui_textwrap.cpp \
		   $(ANLIB_DIR)/graph_widget.cpp \
		   $(ANLIB_DIR)/graph_world.cpp
ANLIB_OBJS=$(ANLIB_SRCS:$(ANLIB_DIR)%.cpp=$(BUILD_DIR)%.o)

GL3W_DIR=./lib/gl3w
IMGUI_DIR=./lib/cimgui/imgui
CIMGUI_DIR=./lib/cimgui/cimgui

COM_INC=-I$(GL3W_DIR) \
		-I$(CIMGUI_DIR) \
		-I$(IMGUI_DIR)/examples/sdl_opengl3_example \
		-I$(IMGUI_DIR) \
		-I$(ANLIB_DIR) \
		$(PLT_INC)

BIN=$(DIST_DIR)/aniplot
BIN_LNK=-lstdc++ $(PLT_LNK)
BIN_SRCS=./src/main.cpp \
		 ./src/udp_listener.cpp \
		 $(IMGUI_DIR)/examples/sdl_opengl3_example/imgui_impl_sdl_gl3.cpp \
		 $(IMGUI_DIR)/imgui.cpp \
		 $(IMGUI_DIR)/imgui_demo.cpp \
		 $(IMGUI_DIR)/imgui_draw.cpp \
		 $(GL3W_DIR)/GL/gl3w.c

default: bin
lib: clean $(LIB)

print-%:
	@echo $*=$($*)

clean:
	rm -rf $(DIRS)
	make -i -C $(CIMGUI_DIR) clean

$(DIRS):
	mkdir $@

bin: clean $(DIRS)
	$(CXX) \
		$(COM_INC) \
		$(ANLIB_SRCS) \
		$(BIN_SRCS) \
		$(BIN_LNK) \
		-o $(BIN)

bin-linked: clean $(LIB)
	$(CXX) \
		$(COM_INC) \
		$(BIN_SRCS) \
		$(LIB) \
		$(BIN_LNK) \
		-o $(BIN)

$(LIB): $(DIRS) $(ANLIB_OBJS)
	ar rcs $(LIB) $(ANLIB_OBJS)

$(BUILD_DIR)/%.o: $(ANLIB_DIR)/%.cpp
	$(CXX) -c $(COM_INC) -o $@ $<



CBIN=$(DIST_DIR)/caniplot
IMGUI_SDL2_GL3_OBJ=$(BUILD_DIR)/imgui_impl_sdl_gl3.o
CIMGUI_SDL2_GL3_OBJ=$(BUILD_DIR)/cimgui_impl_sdl_gl3.o
CBIN_CPP_OBJS=$(BUILD_DIR)/cimgui_helper.o \
			  $(BUILD_DIR)/libcaniplot.o \
			  $(IMGUI_SDL2_GL3_OBJ) \
			  $(CIMGUI_SDL2_GL3_OBJ) \
			  $(LIB) \
			  $(CIMGUI_DYLIB)

cbin: clean $(DIRS) $(CBIN_CPP_OBJS)
	$(CXX) \
		$(COM_INC) \
		-L$(DIST_DIR) \
		src/main.c \
		$(GL3W_DIR)/GL/gl3w.c \
		$(CBIN_CPP_OBJS) \
		$(PLT_LNK) \
		-o $(CBIN)

$(BUILD_DIR)/%.o: src/%.cpp
	$(CXX) -c $(COM_INC) -o $@ $<

$(CIMGUI_DYLIB):
	make -C $(CIMGUI_DIR)
	cp $(CIMGUI_DYLIB:$(DIST_DIR)%=$(CIMGUI_DIR)%) $@

$(IMGUI_SDL2_GL3_OBJ): $(IMGUI_DIR)/examples/sdl_opengl3_example/imgui_impl_sdl_gl3.cpp
	$(CXX) -c $(COM_INC) -o $@ $<

$(CIMGUI_SDL2_GL3_OBJ): src/cimgui_impl_sdl_gl3.cpp
	$(CXX) -c $(COM_INC) -o $@ $<

