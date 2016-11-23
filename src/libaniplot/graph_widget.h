/* vim: set tabstop=4 softtabstop=4 shiftwidth=4 noexpandtab nolist : */
#ifndef __GRAPH_WIDGET_H__
#define __GRAPH_WIDGET_H__

#include "graph_core.h"
#include "imgui_textwrap.h"

// Can contain many graphs (Graph objects).
// Handles user interaction (mouse dragging, keyboard movement)
// Animates movements smoothly
struct GraphWidget {

	ImVector<GraphVisual*> graph_visuals;
	bool draw_bottom_scrollbar;
	bool draw_top_scrollbar;
	// graph is moving from right to left? newest sample is anchored to the right window edge.
	bool anchored;
	bool m_dragging;
	ImguiTextwrap* m_textrend;

	GraphWidget();
	~GraphWidget();
	void add_graph(GraphVisual* graph_visual);
	void DoGraph(const ImVec2& size);
	void _render_minmax_background(const PortalRect& screen_value_portal, GraphVisual& graph_visual, const ImRect& canvas_bb);
	int _calculate_gridlines(double min_val, double max_val, double size_pixels, double min_pixels_per_div, double* out_val_begin, double* out_val_end, double* out_val_step);
	void _render_grid_horlines(const PortalRect& screen_value_portal, GraphVisual& graph_visual, const ImRect& canvas_bb);
	void _render_grid_horlegend(const PortalRect& screen_value_portal, GraphVisual& graph_visual, const ImRect& canvas_bb);
	void _render_grid_verlines(const PortalRect& screen_value_portal, GraphVisual& graph_visual, const ImRect& canvas_bb);
	void _render_grid_verlegend(const PortalRect& screen_value_portal, GraphVisual& graph_visual, const ImRect& canvas_bb);
	void _render_legend(const ImRect& canvas_bb);
	void _grid_timestr(double seconds, double step, char* outstr, int outstr_size);
	void _draw_graphlines(const PortalRect& screen_sample_portal, GraphVisual& graph_visual, const ImRect& canvas_bb);
};
#endif
