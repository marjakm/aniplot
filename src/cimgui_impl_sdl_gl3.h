//
// Copied from imgui version 1.49 imgui/examples/sdl_opengl3_example
//
// ImGui SDL2 binding with OpenGL3
// In this binding, ImTextureID is used to store an OpenGL 'GLuint' texture identifier. Read the FAQ about ImTextureID in imgui.cpp.

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you use this binding you'll need to call 4 functions: ImGui_ImplXXXX_Init(), ImGui_ImplXXXX_NewFrame(), ImGui::Render() and ImGui_ImplXXXX_Shutdown().
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui

#if defined _WIN32 || defined __CYGWIN__
#define API __declspec(dllexport)
#else
#define API
#endif

#if defined __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

#define CIMGUI_API EXTERN API

struct SDL_Window;
typedef union SDL_Event SDL_Event;

CIMGUI_API bool        cImGui_ImplSdlGL3_Init(SDL_Window* window);
CIMGUI_API void        cImGui_ImplSdlGL3_Shutdown();
CIMGUI_API void        cImGui_ImplSdlGL3_NewFrame(SDL_Window* window);
CIMGUI_API bool        cImGui_ImplSdlGL3_ProcessEvent(SDL_Event* event);

// Use if you want to reset your rendering device without losing ImGui state.
CIMGUI_API void        cImGui_ImplSdlGL3_InvalidateDeviceObjects();
CIMGUI_API bool        cImGui_ImplSdlGL3_CreateDeviceObjects();
