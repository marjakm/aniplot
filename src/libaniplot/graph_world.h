/* vim: set tabstop=4 softtabstop=4 shiftwidth=4 noexpandtab nolist : */
#ifndef __GRAPH_WORLD_H__
#define __GRAPH_WORLD_H__

#include "graph_core.h"
#include "graph_widget.h"

typedef std::shared_ptr<GraphWidget>  GraphWidgetPtr;
typedef std::shared_ptr<GraphChannel> GraphChannelPtr;
typedef std::shared_ptr<GraphVisual>  GraphVisualPtr;

struct GraphWorldStream {
	uint8_t stream_id;
	std::vector<GraphChannelPtr> graph_channels;
	std::vector<GraphVisualPtr>  graph_visuals;
	GraphWorldStream(): stream_id(0) {}
};

class GraphWorld {
public:
	GraphWorldStream streams[255];
	std::vector<GraphWidgetPtr> graph_widgets;
	GraphWorld();
	GraphChannel* get_graph_channel(uint8_t stream_id, uint8_t channel_index_in_stream);
	void update_channel_info(uint8_t stream_id, uint8_t channel_index_in_stream,
			uint8_t* channel_name, uint8_t* channel_unit, uint8_t datatype,
			uint32_t rgba, float line_width,
			float value_min, float value_max,
			float portal_x1, float portal_y1, float portal_x2, float portal_y2);
	void _fill_stream_with_channels(uint8_t stream_id, uint8_t num_channels);
};

#endif
