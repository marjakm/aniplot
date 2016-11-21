/* vim: set tabstop=4 softtabstop=4 shiftwidth=4 noexpandtab : */
#ifndef __UDP_LISTENER_H__
#define __UDP_LISTENER_H__

#ifdef _WIN32
	//#include <iostream>
	#include <winsock2.h>
#else
	#include <sys/socket.h>
	#include <netinet/in.h> // sockaddr_in
	#include <arpa/inet.h>  // inet_addr
#endif

#include "graph_world.h"

/* packet version 1
#pragma pack(push,1)
struct p_channel_info {
	uint8_t  packet_type;
	uint8_t  packet_version; // 1
	uint8_t  stream_id;

	uint8_t  channel_index; // channel index in stream. starts from 0.
	uint8_t  channel_name[51]; // zero-terminated
	uint8_t  unit[51];  // zero-terminated
	uint8_t  datatype; // "b", "f", "B", "d", "i", "u", "I", "U", "h", "H"; // only f is supported
	uint8_t  reserved;
	float    frequency;
	uint32_t rgba;
	// used to draw visual limits. if you know your signal is for example 0..5V, use 0 as min and 5 as max here.
	float    value_min;
	float    value_max;
	// used to translate and scale the samples to value-space
	// x1 and y1 is mapped to 0 in value space, x2 and y2 is mapped to 1 in value space.
	// for example using (0, 5,  1000, 0) maps sample num 1000 to 1s and sampleval 1 to 5V.
	float    portal_x1;
	float    portal_y1;
	float    portal_x2;
	float    portal_y2;
};
#pragma pack(pop)*/

// packet version 2
#pragma pack(push,1)
struct p_channel_info {
	uint8_t  packet_type;
	uint8_t  packet_version; // 2
	uint8_t  stream_id;

	uint8_t  channel_index; // channel index in stream. starts from 0.
	uint8_t  channel_name[51]; // zero-terminated
	uint8_t  unit[51];  // zero-terminated
	uint8_t  datatype; // "b", "f", "B", "d", "i", "u", "I", "U", "h", "H"; // only f is supported
	uint8_t  reserved;
	uint32_t line_color_rgba;
	float    line_width;
	// used to draw visual limits. if you know your signal is for example 0..5V, use 0 as min and 5 as max here.
	float    value_min;
	float    value_max;
	// used to translate and scale the samples to value-space
	// x1 and y1 is mapped to 0 in value space, x2 and y2 is mapped to 1 in value space.
	// for example using (0, 5,  1000, 0) maps sample num 1000 to 1s and sampleval 1 to 5V.
	float    portal_x1;
	float    portal_y1;
	float    portal_x2;
	float    portal_y2;
};
#pragma pack(pop)

#pragma pack(push,1)
struct p_channel_samples {
	uint8_t  packet_type;
	uint8_t  packet_version;
	uint8_t  stream_id;
	uint8_t  channel_index; // channel index in stream. starts from 0.
	uint8_t  channel_packet_sn;
	uint8_t  datatype; // "b", "f", "B", "d", "i", "u", "I", "U", "h", "H"; // only f is supported
	uint16_t samples_bytes;
	float    samples[];
};
#pragma pack(pop)

class UdpListener {
public:
	#ifdef _WIN32
		WSADATA _wsa_data;
		SOCKET _socket;
		sockaddr_in _address;
	#else
		int _socket;
		sockaddr_in _address;
	#endif // _WIN32

	bool _initialized;

	UdpListener();
	int _recvfrom(uint8_t* rxbuf, size_t maxlen);
	void tick(GraphWorld& graph_world);
};
#endif
