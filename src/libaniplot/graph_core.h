/* vim: set tabstop=4 softtabstop=4 shiftwidth=4 noexpandtab nolist : */
#ifndef __GRAPH_CORE_H__
#define __GRAPH_CORE_H__

#undef min
#undef max
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))
#define EPSILON 0.0000001f

extern void ANIPLOT_Log(const char* fmt, ...);
#define LOG_IMVEC2(vec) ANIPLOT_Log(#vec" %.5f %.5f\n", vec.x, vec.y)
#define LOG_IMRECT(rc)  ANIPLOT_Log(#rc" min %.5f %.5f max %.5f %.5f\n", rc.Min.x, rc.Min.y, rc.Max.x, rc.Max.y)
#define LOG_PORTAL(rc)  ANIPLOT_Log(#rc" min %.5f %.5f max %.5f %.5f\n", rc.min.x, rc.min.y, rc.max.x, rc.max.y)
#define LOG_AACS(cs)    ANIPLOT_Log(#cs" pos %.5f %.5f axis %.5f %.5f\n", cs.pos.x, cs.pos.y, cs.xyaxis.x, cs.xyaxis.y)

#include <string>
#include <vector>
#include <memory>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include "imgui_internal.h" // for custom graph renderer

#include "imvec2d_portalrect.h"
#include "mip_buf_t.h"
//#include "memfile.h"


enum GraphVisualFlags_ {
	GraphVisualFlags_DrawLeftGridLegend       = 1 << 0, // value axis legend
	GraphVisualFlags_DrawRightGridLegend      = 1 << 1, // value axis legend
	GraphVisualFlags_DrawTopGridLegend        = 1 << 2, // time axis legend
	GraphVisualFlags_DrawBottomGridLegend     = 1 << 3, // time axis legend
	GraphVisualFlags_DrawHorizontalGrid       = 1 << 4,
	GraphVisualFlags_DrawVerticalGrid         = 1 << 5,
	GraphVisualFlags_DrawMinMaxBackground     = 1 << 6,
	//GraphVisualFlags_DrawTopScrollbar         = 1 << 7, // scrollbar flags are the only things that shrink the visible graph window size. all other legends are rendered on the graph.
	//GraphVisualFlags_DrawBottomScrollbar      = 1 << 8,
	GraphVisualFlags_GridLegendIsAbsoluteTime = 1 << 9,
	GraphVisualFlags_GridLegendIsRelativeTime = 1 << 10,
};


// Contains data about the graph:
//      MipBuf, name, unit, description
//      value_sample_portal : how to project samples from sample-space to value-space. this object resides in valuespace.
struct GraphChannel {
	MipBuf_t<float> data_channel;
	std::string name; // for the legend
	std::string unit; // displayed on the legend after value
	std::string description;

	// we HAVE to know this to calculate timepoints from sample numbers.
	//float sample_freq;
	// Useful to show data limits.
	float value_min;
	float value_max;
	PortalRect portal; // How to project samples from sample-space to value-space. Resides in sample-space.

	//GraphChannel(float _sample_freq=1000) { set_value_samplespace_mapping(ImRect(0,0, _sample_freq,1)); }
	GraphChannel() { portal.max = ImVec2d(1000., 1.);} // { set_value_samplespace_mapping(ImRect(0,0, 1000,1)); }

	// convenience functions
	inline void append_sample(float v) { data_channel.append(v); }
	inline void append_sample_minmaxavg(float min, float max, float avg) { data_channel.append_minmaxavg(min, max, avg); }

	//void set_portal(PortalRect& _portal) { portal = _portal; } // sample_freq = fabs(_portal.max.x - _portal.min.x);
	void set_value_limits(float min, float max) { value_min = min; value_max = max; }
	// How samplevalues should be mapped to a unit-box in valuespace.
	// ImRect(1, 10,  2, 255) maps samplenum 1 to 0s, sampleval 10 to 0V, samplenum 2 to 1s, sampleval 255 to 1V.
	void set_value_samplespace_mapping(const ImRect& innerspace) { portal = PortalRect(innerspace); }

	// convert coordinates between spaces. for example 1V to raw sample val 255, time 0s to samplenum 0, ..

	// input: (time, value), return: (samplenum, sampleval)
	inline ImVec2d value_to_samplespace(const ImVec2d& value)  const { return portal.proj_vin(value); }
	// input: (samplenum, sampleval), return: (time, value)
	inline ImVec2d sample_to_valuespace(const ImVec2d& sample) const { return portal.proj_vout(sample); }
};


// Contains info about how to draw the data (color, linewidth, background color between min-max values..)
// There can be many GraphVisual objects per GraphChannel, so one GraphChannel can be drawn to many places with many different ways.
struct GraphVisual {
	GraphChannel* graph_channel;  // GraphVisual does NOT take ownership of the graph_channel.
	ImVec4        line_color;
	ImVec4        line_color_minmax;
	ImVec4        minmax_bgcolor; // background that is filled outside of the value_min/value_max area given in GraphChannel. but is unused if value_min == value_max.
	ImVec4        bg_color;
	ImVec4        hor_grid_color;
	ImVec4        hor_grid_text_color;
	ImVec4        hor_grid_text_bgcolor;
	ImVec4        ver_grid_color;
	ImVec4        ver_grid_text_color;
	ImVec4        ver_grid_text_bgcolor;
	ImVec4        grid_legend_color;
	float         grid_min_div_horizontal_pix;
	float         grid_min_div_vertical_pix;
	float         line_width;     // thickness of the data-line in pixels
	bool          anchored;       // newest sample is anchored to the right window edge.
	bool          visible;        // graph line rendered or not.
	uint32_t      flags;          // GraphVisualFlags_
	PortalRect    portal;         // Everything coming out of this portal between 0..1 is visible. Resides in value-space.

	GraphVisual(GraphChannel* _graph_channel, uint32_t flags=0) {
		IM_ASSERT(_graph_channel);
		graph_channel = _graph_channel;
		line_color = ImColor(200, 200, 200);
		line_color_minmax = ImColor(200, 150, 150, 150);
		minmax_bgcolor = ImColor(20, 20, 20);
		bg_color = ImColor(0, 0, 0);
		hor_grid_color = ImVec4(0.15, 0.15, 0.15, 1.0);
		hor_grid_text_color = ImVec4(1., 1., 1., 0.8);
		hor_grid_text_bgcolor = ImVec4(0.2, 0.2, 0.2, 0.8);
		ver_grid_color = ImVec4(0.15, 0.15, 0.15, 1.0);
		ver_grid_text_color = ImVec4(1., 1., 1., 0.8);
		ver_grid_text_bgcolor = ImVec4(0.2, 0.2, 0.2, 0.8);
		grid_legend_color = ImVec4(0.65, 0.15, 0.15, 1.0);
		grid_min_div_horizontal_pix = 50.0;
		grid_min_div_vertical_pix = 100.0;
		flags = 0;
		line_width = 1;
		anchored = true;
		visible = true;
		// mirror horizontally. pixel coordinates start from top, but we want values start from bottom.
		//portal = PortalRect(0,1, 1,0);
		// also set y zero to center of the window
		portal = PortalRect(0,1, 1,-1);
	}

	// // zoom: visualcenterpos: coordinates on the graph window around which everything changes. usually the coordinates under the mouse. 0..1
	void resize_by_ratio(const ImVec2d& center, const ImVec2d& ratio) { portal.resize_by_ratio(portal.proj_vin(center), ratio); }

	// How should values be mapped to a box with origin 0,0 and size 1,1. 0,0 is top-left corner.
	// innerspace is a rect of values and time.
	// innerspace ex: ((5.6V, 0s), (1.1V, 60s)) - this shows 1 minute worth of data from start of the recording,
	//                bottom of the window starts from 1.1V and top of the window ends with 5.6V).
	void set_visual_valuespace_mapping(const ImRect& innerspace) { portal = PortalRect(innerspace); }
	//ImRect get_visual_valuespace_mapping() { return portal.get_imrect(); }

	// input: (x, y) where visible x and y are 0..1, return: (time, value)
	inline ImVec2d visual_to_valuespace(const ImVec2d& visual)  { return portal.proj_vin(visual); }
	// input: (time, value), return: (x, y) where visible x and y are 0..1
	inline ImVec2d value_to_visualspace(const ImVec2d& value)   { return portal.proj_vout(value); }

	// convenience functions

	// input: (x, y) where visible x and y are 0..1, return: (samplenum, sampleval)
	inline ImVec2d visual_to_samplespace(const ImVec2d& visual) { return graph_channel->portal.proj_vin( portal.proj_vin(visual) ); }
	// input: (samplenum, sampleval), return: (x, y) where visible x and y are 0..1
	inline ImVec2d sample_to_visualspace(const ImVec2d& sample) { return portal.proj_vout( graph_channel->portal.proj_vout(sample) ); }
};
#endif
