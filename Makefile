#!/usr/bin/make
# TODO: make it work under linux

GL3W_DIR=./lib/gl3w
IMGUI_DIR=./lib/cimgui/imgui
CIMGUI_DIR=./lib/cimgui/cimgui

BUILD_DIR=./build
DIST_DIR=./dist
DIRS=$(BUILD_DIR) $(DIST_DIR)

LIB=$(DIST_DIR)/libaniplot.a
ANLIB_DIR=./src/libaniplot
ANLIB_SRCS=$(ANLIB_DIR)/imgui_textwrap.cpp \
		   $(ANLIB_DIR)/graph_widget.cpp \
		   $(ANLIB_DIR)/graph_world.cpp
ANLIB_OBJS=$(ANLIB_SRCS:$(ANLIB_DIR)%.cpp=$(BUILD_DIR)%.o)

COM_INC=-I/usr/local/include/ \
		-I/usr/local/include/SDL2/ \
		-I$(GL3W_DIR) \
		-I$(IMGUI_DIR)/examples/sdl_opengl3_example \
		-lSDL2 \
		-lSDL2main \
		-lstdc++ \
		-framework OpenGL \
		-framework CoreFoundation \

BIN=$(DIST_DIR)/aniplot
BIN_INC=$(COM_INC) \
		-I$(IMGUI_DIR) \
		-I$(ANLIB_DIR) \
BIN_SRCS=src/main.cpp \
		 src/udp_listener.cpp \
		 $(IMGUI_DIR)/examples/sdl_opengl3_example/imgui_impl_sdl_gl3.cpp \
		 $(IMGUI_DIR)/imgui.cpp \
		 $(IMGUI_DIR)/imgui_demo.cpp \
		 $(IMGUI_DIR)/imgui_draw.cpp \
		 $(GL3W_DIR)/GL/gl3w.c

default: bin

print-%:
	@echo $*=$($*)

clean:
	rm -rf $(DIRS)
	make -i -C $(CIMGUI_DIR) clean

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



CBIN=$(DIST_DIR)/caniplot
IMGUI_SDL2_GL3_OBJ=$(BUILD_DIR)/imgui_impl_sdl_gl3.o
CIMGUI_SDL2_GL3_OBJ=$(BUILD_DIR)/cimgui_impl_sdl_gl3.o
CIMGUI_DYLIB=$(DIST_DIR)/cimgui.dylib
CBIN_CPP_OBJS=$(BUILD_DIR)/cimgui_helper.o \
			  $(BUILD_DIR)/libcaniplot.o

cbin: clean $(DIRS) $(IMGUI_SDL2_GL3_OBJ) $(CIMGUI_SDL2_GL3_OBJ) $(CIMGUI_DYLIB) $(CBIN_CPP_OBJS) $(ANLIB_OBJS)
	cc -O2 \
		$(COM_INC) \
		-L$(DIST_DIR) \
		-I$(CIMGUI_DIR) \
		$(CIMGUI_DYLIB) \
		$(CBIN_CPP_OBJS) \
		$(ANLIB_OBJS) \
		$(GL3W_DIR)/GL/gl3w.c \
		$(IMGUI_SDL2_GL3_OBJ) \
		$(CIMGUI_SDL2_GL3_OBJ) \
		src/main.c \
		-o $(CBIN)

$(BUILD_DIR)/%.o: src/%.cpp
	g++ -O2 -c -I$(IMGUI_DIR) -I$(CIMGUI_DIR) -I$(ANLIB_DIR) -o $@ $<

$(IMGUI_SDL2_GL3_OBJ): $(IMGUI_DIR)/examples/sdl_opengl3_example/imgui_impl_sdl_gl3.cpp
	g++ -O2 -c -I$(IMGUI_DIR) -I$(GL3W_DIR) -I/usr/local/include/SDL2 -o $@ $<

$(CIMGUI_SDL2_GL3_OBJ): src/cimgui_impl_sdl_gl3.cpp
	g++ -O2 -c -I$(IMGUI_DIR) -I$(IMGUI_DIR)/examples/sdl_opengl3_example/ -o $@ $<

$(CIMGUI_DYLIB):
	make -C $(CIMGUI_DIR)
	cp $(CIMGUI_DIR)/cimgui.dylib $@
