#include "imgui.h" //IMGUI_API
#include "cimgui_impl_sdl_gl3.h"
#include "imgui_impl_sdl_gl3.h"

CIMGUI_API bool        cImGui_ImplSdlGL3_Init(SDL_Window* window) {
    return ImGui_ImplSdlGL3_Init(window);
}
CIMGUI_API void        cImGui_ImplSdlGL3_Shutdown() {
    return ImGui_ImplSdlGL3_Shutdown();
}
CIMGUI_API void        cImGui_ImplSdlGL3_NewFrame(SDL_Window* window) {
    return ImGui_ImplSdlGL3_NewFrame(window);
}
CIMGUI_API bool        cImGui_ImplSdlGL3_ProcessEvent(SDL_Event* event) {
    return ImGui_ImplSdlGL3_ProcessEvent(event);
}

// Use if you want to reset your rendering device without losing ImGui state.
CIMGUI_API void        cImGui_ImplSdlGL3_InvalidateDeviceObjects() {
    return ImGui_ImplSdlGL3_InvalidateDeviceObjects();
}
CIMGUI_API bool        cImGui_ImplSdlGL3_CreateDeviceObjects() {
    return ImGui_ImplSdlGL3_CreateDeviceObjects();
}
