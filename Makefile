#!/usr/bin/make
# TODO: make it work under linux

GL3W_DIR=./lib/gl3w
IMGUI_DIR=./lib/imgui

BUILD_DIR=./build
DIST_DIR=./dist
DIRS=$(BUILD_DIR) $(DIST_DIR)

LIB=$(DIST_DIR)/libaniplot.a
ANLIB_DIR=./src/libaniplot
ANLIB_SRCS=$(ANLIB_DIR)/imgui_textwrap.cpp \
		   $(ANLIB_DIR)/graph_widget.cpp \
		   $(ANLIB_DIR)/graph_world.cpp
ANLIB_OBJS=$(ANLIB_SRCS:$(ANLIB_DIR)%.cpp=$(BUILD_DIR)%.o)

BIN=$(DIST_DIR)/aniplot
BIN_SRCS=src/main.cpp \
		 src/udp_listener.cpp \
		 $(IMGUI_DIR)/examples/sdl_opengl3_example/imgui_impl_sdl_gl3.cpp \
		 $(IMGUI_DIR)/imgui.cpp \
		 $(IMGUI_DIR)/imgui_demo.cpp \
		 $(IMGUI_DIR)/imgui_draw.cpp \
		 $(GL3W_DIR)/GL/gl3w.c
BIN_INC=-I/usr/local/include/ \
		-I/usr/local/include/SDL2/ \
		-I$(GL3W_DIR) \
		-I$(IMGUI_DIR) \
		-I$(IMGUI_DIR)/examples/sdl_opengl3_example \
		-I$(ANLIB_DIR) \
		-lSDL2 \
		-lSDL2main \
		-framework OpenGL \
		-framework CoreFoundation \
		-lstdc++


default: bin

print-%:
	@echo $*=$($*)

clean:
	rm -rf $(DIRS)

$(DIRS):
	mkdir $@

bin: clean $(DIRS)
	g++ -O2 \
		$(BIN_INC) \
		$(ANLIB_SRCS) \
		$(BIN_SRCS) \
		-o $(BIN)

bin-linked: lib
	g++ -O2 \
		$(BIN_INC) \
		$(LIB) \
		$(BIN_SRCS) \
		-o $(BIN)

lib: clean $(DIRS) $(ANLIB_OBJS)
	ar rcs $(LIB) $(ANLIB_OBJS)

$(BUILD_DIR)/%.o: $(ANLIB_DIR)/%.cpp
	g++ -O2 -c -I$(IMGUI_DIR) -o $@ $<
