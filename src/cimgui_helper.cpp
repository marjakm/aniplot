#include "imgui.h"
#include "imgui_internal.h" // ImFormatStringV
#include "cimgui.h" // CIMGUI_API


CIMGUI_API ImVec2 cih_get_io_DisplaySize() {
    ImGuiIO& io = ImGui::GetIO();
    return io.DisplaySize;
}

CIMGUI_API float cih_get_io_Framerate() {
    ImGuiIO& io = ImGui::GetIO();
    return io.Framerate;
}

CIMGUI_API ImVec4 cih_ImColor(int r, int g, int b, int a) {
    return ImColor(r,g,b,a);
}

CIMGUI_API int cih_ImFormatString(char* buf, int buf_size, const char* fmt, ...) {
    va_list argp;
    va_start(argp, fmt);
    ImFormatStringV(buf, buf_size, fmt, argp);
    va_end(argp);
}
