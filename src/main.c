/* vim: set tabstop=4 softtabstop=4 shiftwidth=4 noexpandtab nolist : */
// Copyright (c) 2015, Elmo Trolla
// LICENSE: https://opensource.org/licenses/ISC

#include <math.h>
#include <SDL.h>
#include <GL/gl3w.h>
/* #include "imgui_impl_sdl_gl3.h" */

#ifdef _WIN32
	#include <direct.h> // getcwd
	#define getcwd _getcwd
#else
	#include <unistd.h> // getcwd
#endif

/* #include "graph_core.h" */
/* #include "graph_world.h" */



SDL_Window* window = NULL;

#define INITIAL_SCREEN_WIDTH 1200
#define INITIAL_SCREEN_HEIGHT 700



/* ImRect get_window_coords() { */
/* 	return ImRect(ImGui::GetWindowPos(), ImGui::GetWindowPos()+ImGui::GetWindowSize()); */
/* 	//const ImRect bb(window->DC.CursorPos, window->DC.CursorPos+size); */
/* 	//const ImRect bb(window->DC.CursorPos, window->DC.CursorPos+ImGui::GetContentRegionAvail()); */
/* } */


int main(int argc, char *argv[]) {
	SDL_Log("initializing SDL\n");

	char dirbuf[10000];
	SDL_Log("wd: %s\n", getcwd(dirbuf, sizeof(dirbuf)));

	//if (SDL_Init(SDL_INIT_VIDEO) != 0) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("SDL_Init error: %s\n", SDL_GetError());
		return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	// opengl version 4.1 is max for yosemite. 2015-07-14
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_Log("SDL_CreateWindow\n");

	window = SDL_CreateWindow(
			"Telemetry",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			INITIAL_SCREEN_WIDTH,
			INITIAL_SCREEN_HEIGHT,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window == NULL) {
		SDL_Log("SDL_CreateWindow error: %s\n", SDL_GetError());
		return -1;
	}

	SDL_Log("SDL_GL_CreateContext\n");

	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	if (gl_context == NULL) {
		SDL_Log("SDL_GL_CreateContext error: %s\n", SDL_GetError());
		return -1;
	}

	// Initialize GL3W
	int r = gl3wInit();
	if (r != 0) {
		SDL_Log("Error initializing GL3W! %i\n", r);
		return -1;
	}

	SDL_Log("Supported GLSL version is %s.\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Use Vsync, set for late swap tearing.
	if (SDL_GL_SetSwapInterval(-1) < 0) {
		SDL_Log("Warning: Unable to set VSync for late swap tearing! SDL Error: %s\n", SDL_GetError());
	}

	// Setup ImGui binding
	/* ImGui_ImplSdlGL3_Init(window); */

	// --------------------------------------------------------------------


	// Cleanup
	/* ImGui_ImplSdlGL3_Shutdown(); */
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}


/*void main_loop() {

	ImVec4 clear_color = ImColor(114, 144, 154);
	ImguiTextwrap textrend;

	GraphWorld graph_world;
	// creates one channel. without it, nothing would be rendered until someone updated some channels.
	GraphChannel* graph_channel = graph_world.get_graph_channel(0, 0);
	graph_channel->name = "default";

	UdpListener windows_udp_listener;


	// Main loop
	bool done = false;
	while (!done) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSdlGL3_ProcessEvent(&event);
			if (event.type == SDL_QUIT) {
				done = true;
			}
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					done = true;
				}
			}
		}

		ImGui_ImplSdlGL3_NewFrame(window);
		ImGuiIO &io = ImGui::GetIO();

		// make the window always fullsize.
		// disable window background rendering.
		// remove borders, titlebar, menus, ..
		// TODO: change window margins?


		//ImGui::ShowTestWindow();

		float prev_bgalpha = GImGui->Style.WindowFillAlphaDefault; // PushStyleVar() doesn't seem to support this yet
		GImGui->Style.WindowFillAlphaDefault = 0.;
		ImGui::SetNextWindowSize(io.DisplaySize, ImGuiSetCond_Always);
		ImGui::Begin("Robot", NULL,
					 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
					 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings |
					 ImGuiWindowFlags_ShowBorders);
		GImGui->Style.WindowFillAlphaDefault = prev_bgalpha;

		uint32_t milliseconds = SDL_GetTicks();

		//float v = sin(milliseconds / 3. * M_PI / 180.);
		//graph_channel.data_channel.append_minmaxavg(v, v, v);

		windows_udp_listener.tick(graph_world);

		//for (auto graph : graph_world) {}
		graph_world.graph_widgets[0]->DoGraph(ImVec2(1000, 600));

		if (!textrend.font) textrend.init(ImGui::GetWindowFont());

		char txt[50];
		ImFormatString(txt, sizeof(txt), "fps: %.1f (%.3f ms)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
		ImRect windim = get_window_coords();
		textrend.set_bgcolor(ImColor(80,80,80,100));
		textrend.set_fgcolor(ImColor(255,255,255,100));
		textrend.drawtr(txt, windim.Max.x - 14, windim.Min.y + 14);

		ImGui::End();


		//static bool show_test_window;
		//ImGui::ShowTestWindow(&show_test_window);

		// Rendering
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();
		SDL_GL_SwapWindow(window);
	}
}
*/
