/* vim: set tabstop=4 softtabstop=4 shiftwidth=4 noexpandtab nolist : */
#include "graph_world.h"

void fill_graph_with_testdata(GraphChannel& graph)
{
	for(int i = 0; i < 10000; i++) {
		float v = sin(i*M_PI/180.);
		graph.data_channel.append_minmaxavg(v, v, v);
	}
}

// Holds streams.
// A stream can consist of multiple channels.
// Channels can be mapped to different graph_widgets. (well not yet)


GraphWorld::GraphWorld() { graph_widgets.push_back(GraphWidgetPtr(new GraphWidget)); }

GraphChannel* GraphWorld::get_graph_channel(uint8_t stream_id, uint8_t channel_index_in_stream) {
	// ensure that the wanted channel exists
	_fill_stream_with_channels(stream_id, channel_index_in_stream + 1);
	return streams[stream_id].graph_channels[channel_index_in_stream].get();
}

//void clear_stream(uint8_t stream_id) {
//	// TODO: how??
//}

// stream_id : 0..255
// channel_id : 0..255
// datatype not implemented
void GraphWorld::update_channel_info(uint8_t stream_id, uint8_t channel_index_in_stream,
		uint8_t* channel_name, uint8_t* channel_unit, uint8_t datatype,
		uint32_t rgba, float line_width,
		float value_min, float value_max,
		float portal_x1, float portal_y1, float portal_x2, float portal_y2) {

	_fill_stream_with_channels(stream_id, channel_index_in_stream + 1);

	GraphWorldStream* stream = &streams[stream_id];

	// TODO: replace the whole channel if datatypes are different

	GraphChannel* channel = stream->graph_channels[channel_index_in_stream].get();
	GraphVisual*  visual  = stream->graph_visuals[channel_index_in_stream].get();

	//visual->set_visual_valuespace_mapping(ImRect(0., 1., 1., -1));
	visual->line_color = ImColor(rgba);
	visual->line_width = line_width < 0.001f ? 0.001f : line_width;
	// How samplevalues should be mapped to a unit-box in valuespace.
	// ImRect(1, 10,  2, 255) maps samplenum 1 to 0s, sampleval 10 to 0V, samplenum 2 to 1s, sampleval 255 to 1V.
	channel->set_value_samplespace_mapping(ImRect(portal_x1, portal_y1, portal_x2, portal_y2));
	channel->set_value_limits(value_min, value_max);
	channel->name = (char*)channel_name;
	channel->unit = (char*)channel_unit;
}

// make sure stream contains at least num_channels
void GraphWorld::_fill_stream_with_channels(uint8_t stream_id, uint8_t num_channels) {
	while (streams[stream_id].graph_channels.size() < num_channels) {
		GraphChannelPtr channel = GraphChannelPtr(new GraphChannel);
		GraphVisualPtr  visual  = GraphVisualPtr(new GraphVisual(channel.get()));
		streams[stream_id].graph_channels.push_back(channel);
		streams[stream_id].graph_visuals.push_back(visual);
		graph_widgets[0]->add_graph(visual.get());
	}
}
