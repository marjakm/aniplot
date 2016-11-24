#include "cimgui.h" // CIMGUI_API
#include "imgui.h"
#include "imgui_textwrap.h"
#include "graph_core.h"
#include "graph_widget.h"
#include "graph_world.h"


// -------------- ImguiTextwrap --------------------------------------
CIMGUI_API ImguiTextwrap* anp_ImguiTextwrap() {
    return new ImguiTextwrap();
}

CIMGUI_API void anp_del_ImguiTextwrap(ImguiTextwrap* tw) {
    delete tw;
}

CIMGUI_API void anp_ImguiTextwrap_init(ImguiTextwrap* tw, ImFont* font) {
    tw->init(font);
}


CIMGUI_API void anp_ImguiTextwrap_set_window_font(ImguiTextwrap* tw) {
    tw->init(ImGui::GetWindowFont());
}

// return string size in pixels
CIMGUI_API ImVec2 anp_ImguiTextwrap_get_size(ImguiTextwrap* tw, const char* text) {
    return tw->size(text);
}


CIMGUI_API void anp_ImguiTextwrap_set_fgcolor(ImguiTextwrap* tw, const ImVec4* color) {
    tw->set_fgcolor(*color);
}


CIMGUI_API void anp_ImguiTextwrap_set_bgcolor(ImguiTextwrap* tw, const ImVec4* color) {
    tw->set_bgcolor(*color);
}


CIMGUI_API void anp_ImguiTextwrap_drawtr(ImguiTextwrap* tw, const char* text, float x, float y) {
    tw->drawtr(text, x, y);
}



// -------------- GraphChannel --------------------------------------
CIMGUI_API GraphChannel* anp_GraphChannel() {
    return new GraphChannel();
}

CIMGUI_API void anp_del_GraphChannel(GraphChannel* gc) {
    delete gc;
}


CIMGUI_API void anp_GraphChannel_set_name(GraphChannel* gc, const char* str) {
    gc->name = str;
}


// -------------- GraphWidget --------------------------------------
CIMGUI_API GraphWidget* anp_GraphWidget() {
    return new GraphWidget();
}

CIMGUI_API void anp_del_GraphWidget(GraphWidget* gw) {
    delete gw;
}

CIMGUI_API void anp_GraphWidget_DoGraph(GraphWidget* cw, const ImVec2* size) {
    cw->DoGraph(*size);
}


// -------------- GraphWorld --------------------------------------
CIMGUI_API GraphWorld* anp_GraphWorld() {
    return new GraphWorld();
}

CIMGUI_API void anp_del_GraphWorld(GraphWorld* gw) {
    delete gw;
}

CIMGUI_API GraphChannel* anp_GraphWorld_get_graph_channel(GraphWorld* gw, uint8_t stream_id, uint8_t channel_index_in_stream) {
    return gw->get_graph_channel(stream_id, channel_index_in_stream);
}


CIMGUI_API void anp_GraphWorld_update_channel_info(GraphWorld* cw, uint8_t stream_id, uint8_t channel_index_in_stream,
                         uint8_t* channel_name, uint8_t* channel_unit, uint8_t datatype,
                         uint32_t rgba, float line_width,
                         float value_min, float value_max,
                         float portal_x1, float portal_y1, float portal_x2, float portal_y2) {
    cw->update_channel_info(stream_id, channel_index_in_stream,
                         channel_name, channel_unit, datatype,
                         rgba, line_width,
                         value_min, value_max,
                         portal_x1, portal_y1, portal_x2, portal_y2);
}

CIMGUI_API GraphWidget* anp_GraphWorld_get_graph_widget(GraphWorld* gw, uint8_t widget_id) {
    return gw->graph_widgets[widget_id].get();
}
