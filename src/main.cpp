/* vim: set tabstop=4 softtabstop=4 shiftwidth=4 noexpandtab nolist : */
// Copyright (c) 2015, Elmo Trolla
// LICENSE: https://opensource.org/licenses/ISC

#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include <math.h>

#ifdef _WIN32
	#include <direct.h> // getcwd
	#define getcwd _getcwd
#else
	#include <unistd.h> // getcwd
#endif

#include "udp_listener.h"
#include "graph_core.h"
#include "graph_world.h"

#include <SDL.h>
#include <GL/gl3w.h>
#include "imgui_impl_sdl_gl3.h"


using namespace std;

SDL_Window* window = NULL;

#define INITIAL_SCREEN_WIDTH 1200
#define INITIAL_SCREEN_HEIGHT 700



ImRect get_window_coords() {
	return ImRect(ImGui::GetWindowPos(), ImGui::GetWindowPos()+ImGui::GetWindowSize());
	//const ImRect bb(window->DC.CursorPos, window->DC.CursorPos+size);
	//const ImRect bb(window->DC.CursorPos, window->DC.CursorPos+ImGui::GetContentRegionAvail());
}


int main(int, char**)
{
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
	ImGui_ImplSdlGL3_Init(window);


	// --------------------------------------------------------------------

	ImVec4 clear_color = ImColor(114, 144, 154);
	ImguiTextwrap textrend;

	GraphWorld graph_world;
	// creates one channel. without it, nothing would be rendered until someone updated some channels.
	GraphChannel* graph_channel = graph_world.get_graph_channel(0, 0);
	graph_channel->name = "default";

	UdpListener windows_udp_listener;

#if 0

	if (0) {
		GraphChannel* graph_channel = graph_world.get_graph_channel(0, 0);

		// TODO: 500 million samples crash on windows. 1.8GB memory gets allocated, then crash.
		for (int i = 0; i < 500000; i++) {
			float v = 0.9*sinf(i * 1000. / 60. / 3. * M_PI / 180.) +
					0.5*sinf(i * 1000. / 60. / 3.333 * M_PI / 180.) +
					0.3*sinf(i * 1000. / 60. / 31. * M_PI / 180.) +
					sinf(i * 1000. / 60. / 321. * M_PI / 180.) +
					sinf(i * 1000. / 60. / 12345. * M_PI / 180.) +
					2.*sinf(i * 1000. / 60. / 567890. * M_PI / 180.) +
					sinf(i * 1000. / 60. / 58890000. * M_PI / 180.);
			graph_channel->append_sample(v);
		}
	}

	if (0) {
		//char* filename = "tek0005both channels.csv-ch1.samples-f32";
		char* filename = "test.samples-f32";
		//char* filename = "tek0005both channels.csv-ch1.samples-f32";
		SDL_Log("opening file '%s'", filename);
		MemFile f(filename);
		if (f.buf) {
			float* buf = (float*)f.buf;
			SDL_Log("file size %i bytes. loading data to the channel object.", f.file_size);
			for (int i = 0; i < f.file_size/4; i++) {
				float v = buf[i]; // TODO: does compiler optimize this to the while-not-last-ptr method?
				graph_channel.data_channel.append_minmaxavg(v, v, v);
			}
			SDL_Log("import done");
		} else {
			SDL_Log("file open failed");
		}
	}
#endif

	// Main loop
	bool done = false;
	while (!done) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSdlGL3_ProcessEvent(&event);
			if (event.type == SDL_QUIT) done = true;
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE) done = true;
			}
		}
		ImGui_ImplSdlGL3_NewFrame();

		ImGuiIO &io = ImGui::GetIO();

		// make the window always fullsize.
		// disable window background rendering.
		// remove borders, titlebar, menus, ..
		// TODO: change window margins?


		//ImGui::ShowTestWindow();

		if (1) {
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
		}


		if (0) {

			// testing ground

			ImGui::Begin("deb", NULL);

			ImFont *f = ImGui::GetWindowFont();

			float x = floor(1.);
			float y = floor(1.);

			const char* txt = "AB";
			ImVec2 str_size = f->CalcTextSizeA(f->FontSize, FLT_MAX, 0, txt, NULL, NULL);

			float up_left_x = x;
			float up_left_y = y;
			float down_right_x = x + str_size.x + 1;
			float down_right_y = y + f->FontSize;
			// TODO: ascent + descent == size?


			// 1.0 ,1.0  2.0 ,2.0  - this draws 1x1 square one pixel away from the corner
			// 1.5 ,1.5  2.0 ,2.0  - this draws 1x1 square one pixel away from the corner
			// 1.51,1.51 2.0 ,2.0  - this draws nothing
			// 1.0 ,1.0  1.51,1.51 - this draws 1x1 square one pixel away from the corner
			// 1.5 ,1.5  1.51,1.51 - this draws 1x1 square one pixel away from the corner

			// 0.51,0.51 2.50,2.50 - this draws 1x1 square one pixel away from the corner

			//   /
			// |  .  |  .  |  .  |  .  |  .  |

			// rect : pixel coordinates start from pixel corner, not from pixel center.
			//        (0.5 - 1.5] - rendering starts from second pixel. smaller-equal than 0.5 is first pixel.
			//ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(0.51, 0.51), ImVec2(2.50, 2.50), ImColor(128,128,128,255));

			ImGui::GetWindowDrawList()->AddRectFilled(
				ImVec2(up_left_x, up_left_y),
				ImVec2(down_right_x, down_right_y),
				ImColor(128, 128, 128, 255));

			// text : pixel coordinates start from pixel corner, not from pixel center.
			// [0., 1.) - render starts inside first pixel. 1. is start of the second pixel.
			//ImGui::GetWindowDrawList()->AddText(ImGui::GetWindowFont(), ImGui::GetWindowFont()->FontSize, ImVec2(1., 1.), ImColor(255,255,255,255), txt, NULL, 0, NULL);

			ImGui::GetWindowDrawList()->AddText(
				ImGui::GetWindowFont(),
				ImGui::GetWindowFont()->FontSize,
				ImVec2(x, y),
				ImColor(255, 255, 255, 255),
				txt, NULL, 0, NULL);

			// textile saab floor teha ok.
			// aga rectile? round(x-0.5)
			// tekstil on pixel 1, x coord [1..2)

			ImGui::End();
		}

		//static bool show_test_window;
		//ImGui::ShowTestWindow(&show_test_window);

		// Rendering
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();
		SDL_GL_SwapWindow(window);
	}

	// Cleanup
	ImGui_ImplSdlGL3_Shutdown();
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
