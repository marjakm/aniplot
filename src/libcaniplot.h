#include "cimgui.h" // CIMGUI_API
#include "cimgui_helper.h"

// -------------- ImguiTextwrap --------------------------------------
typedef struct ImguiTextwrap ImguiTextwrap;

CIMGUI_API ImguiTextwrap* anp_ImguiTextwrap();
CIMGUI_API void anp_del_ImguiTextwrap();

CIMGUI_API void anp_ImguiTextwrap_init(ImguiTextwrap* tw, ImFont* font);

CIMGUI_API void anp_ImguiTextwrap_set_window_font(ImguiTextwrap* tw);
// return string size in pixels
CIMGUI_API ImVec2 anp_ImguiTextwrap_get_size(ImguiTextwrap* tw, const char* text);

CIMGUI_API void anp_ImguiTextwrap_set_fgcolor(ImguiTextwrap* tw, const ImVec4* color);
CIMGUI_API void anp_ImguiTextwrap_set_bgcolor(ImguiTextwrap* tw, const ImVec4* color);

// // top
//
// CIMGUI_API void anp_ImguiTextwrap_drawtl(ImguiTextwrap* tw, const char* text, float x, float y);
CIMGUI_API void anp_ImguiTextwrap_drawtr(ImguiTextwrap* tw, const char* text, float x, float y);
// CIMGUI_API void anp_ImguiTextwrap_drawtm(ImguiTextwrap* tw, const char* text, float x, float y);
//
// // bottom
//
// CIMGUI_API void anp_ImguiTextwrap_drawbl(ImguiTextwrap* tw, const char* text, float x, float y);
// CIMGUI_API void anp_ImguiTextwrap_drawbr(ImguiTextwrap* tw, const char* text, float x, float y);
// CIMGUI_API void anp_ImguiTextwrap_drawbm(ImguiTextwrap* tw, const char* text, float x, float y);
//
// // middle
//
// CIMGUI_API void anp_ImguiTextwrap_drawml(ImguiTextwrap* tw, const char* text, float x, float y);
// CIMGUI_API void anp_ImguiTextwrap_drawmr(ImguiTextwrap* tw, const char* text, float x, float y);
// CIMGUI_API void anp_ImguiTextwrap_drawmm(ImguiTextwrap* tw, const char* text, float x, float y);
//
// // baseline
//
// CIMGUI_API void anp_ImguiTextwrap_drawbll(ImguiTextwrap* tw, const char* text, float x, float y);
// CIMGUI_API void anp_ImguiTextwrap_drawblr(ImguiTextwrap* tw, const char* text, float x, float y);
// CIMGUI_API void anp_ImguiTextwrap_drawblm(ImguiTextwrap* tw, const char* text, float x, float y);
//
// // if fgcolor or bgcolor or z are None, previous values are used.
// // assumes some kind of pixel-projection..
// // you almost never want to use this method directly.
// CIMGUI_API void draw(ImguiTextwrap* tw, const char* text, float x, float y, int positioning);


// -------------- GraphChannel --------------------------------------
typedef struct GraphChannel GraphChannel;

CIMGUI_API GraphChannel* anp_GraphChannel();
CIMGUI_API void anp_del_GraphChannel();

CIMGUI_API void anp_GraphChannel_set_name(GraphChannel* gc, const char* str);


// // convenience functions
// inline void append_sample(float v) { data_channel.append(v); }
// inline void append_sample_minmaxavg(float min, float max, float avg) { data_channel.append_minmaxavg(min, max, avg); }
//
// //void set_portal(PortalRect& _portal) { portal = _portal; } // sample_freq = fabs(_portal.max.x - _portal.min.x);
// void set_value_limits(float min, float max) { value_min = min; value_max = max; }
// // How samplevalues should be mapped to a unit-box in valuespace.
// // ImRect(1, 10,  2, 255) maps samplenum 1 to 0s, sampleval 10 to 0V, samplenum 2 to 1s, sampleval 255 to 1V.
// void set_value_samplespace_mapping(const ImRect& innerspace) { portal = PortalRect(innerspace); }
//
// // convert coordinates between spaces. for example 1V to raw sample val 255, time 0s to samplenum 0, ..
//
// // input: (time, value), return: (samplenum, sampleval)
// inline ImVec2d value_to_samplespace(const ImVec2d& value)  const { return portal.proj_vin(value); }
// // input: (samplenum, sampleval), return: (time, value)
// inline ImVec2d sample_to_valuespace(const ImVec2d& sample) const { return portal.proj_vout(sample); }


// -------------- GraphWidget --------------------------------------
typedef struct GraphWidget GraphWidget;

CIMGUI_API GraphWidget* anp_GraphWidget();
CIMGUI_API void anp_del_GraphWidget(GraphWidget* gw);

// CIMGUI_API void anp_GraphWidget_add_graph(GraphWidget* cw, GraphVisual* graph_visual);
CIMGUI_API void anp_GraphWidget_DoGraph(GraphWidget* cw, const ImVec2* size);


// -------------- GraphWorld --------------------------------------
typedef struct GraphWorld GraphWorld;

CIMGUI_API GraphWorld* anp_GraphWorld();
CIMGUI_API void anp_del_GraphWorld();

CIMGUI_API GraphChannel* anp_GraphWorld_get_graph_channel(GraphWorld* gw, uint8_t stream_id, uint8_t channel_index_in_stream);
CIMGUI_API void anp_GraphWorld_update_channel_info(GraphWorld* cw, uint8_t stream_id, uint8_t channel_index_in_stream,
                         uint8_t* channel_name, uint8_t* channel_unit, uint8_t datatype,
                         uint32_t rgba, float line_width,
                         float value_min, float value_max,
                         float portal_x1, float portal_y1, float portal_x2, float portal_y2);

CIMGUI_API GraphWidget* anp_GraphWorld_get_graph_widget(GraphWorld* gw, uint8_t widget_id);
