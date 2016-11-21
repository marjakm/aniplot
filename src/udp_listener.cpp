/* vim: set tabstop=4 softtabstop=4 shiftwidth=4 noexpandtab nolist : */

using namespace std;

#define P_CHANNEL_SAMPLES 10
#define P_CHANNEL_INFO 11

#include "udp_listener.h"


UdpListener::UdpListener() {
	uint16_t PORT = 59100;

	_initialized = false;

	#ifdef _WIN32

		if (WSAStartup(MAKEWORD(2, 2), &_wsa_data) != NO_ERROR) {
			SDL_Log("UdpListener: error in WSAStartup\n");
			WSACleanup();
			return;
		}

		_socket = socket(AF_INET, SOCK_DGRAM, 0);
		if (_socket == INVALID_SOCKET) {
			SDL_Log("UdpListener: error creating socket\n");
			WSACleanup();
			return;
		}

		// TODO: clean to zero
		_address.sin_family = AF_INET;
		_address.sin_addr.s_addr = inet_addr("0.0.0.0");
		_address.sin_port = htons(PORT);

		// If iMode!=0, non-blocking mode is enabled.
		u_long iMode = 1;
		ioctlsocket(_socket, FIONBIO, &iMode);

		if (bind(_socket, (SOCKADDR*)&_address, sizeof(_address)) == SOCKET_ERROR) {
			SDL_Log("UdpListener: failed to bind socket\n");
			WSACleanup();
			return;
		}

	#else

		_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (_socket < 0) {
			SDL_Log("UdpListener: error creating socket\n");
			return;
		}

		// zero out the structure
		memset((char*)&_address, 0, sizeof(_address));

		_address.sin_family = AF_INET;
		_address.sin_addr.s_addr = inet_addr("0.0.0.0");
		_address.sin_port = htons(PORT);

		if (bind(_socket, (struct sockaddr*)&_address, sizeof(_address)) < 0) {
			SDL_Log("UdpListener: failed to bind socket\n");
			return;
		}

	#endif // _WIN32

	_initialized = true;
}

int UdpListener::_recvfrom(uint8_t* rxbuf, size_t maxlen) {
	sockaddr_in remote_addr;
	// TODO: need to differentiate between win and others? and the server_length stuff - what's that?
	#ifdef _WIN32
		int server_length = sizeof(struct sockaddr_in);
		return recvfrom(_socket, (char*)rxbuf, maxlen, 0, (SOCKADDR*)&remote_addr, &server_length);
	#else
		socklen_t server_length = sizeof(struct sockaddr_in);
		return recvfrom(_socket, (char*)rxbuf, maxlen, MSG_DONTWAIT, (struct sockaddr*)&remote_addr, &server_length);
	#endif
}

void UdpListener::tick(GraphWorld& graph_world) {
	if (!_initialized) return;

	uint8_t rxbuf[65535];

	int n = 1;
	while (n > 0) {
		n = _recvfrom(rxbuf, sizeof(rxbuf));
		if (n <= 0) break;

		uint8_t packet_type = rxbuf[0];

		if (packet_type == P_CHANNEL_SAMPLES && n > sizeof(p_channel_samples)) {

			p_channel_samples* p = (p_channel_samples*)rxbuf;
			if (p->packet_version != 1) continue;

			GraphChannel* graph_channel = graph_world.get_graph_channel(p->stream_id, p->channel_index);
			float* samples = &p->samples[0];
			for (int i = 0; i < p->samples_bytes / 4; i++) {
				float v = samples[i];
				graph_channel->append_sample_minmaxavg(v, v, v);
			}

		} else if (packet_type == P_CHANNEL_INFO) {

			p_channel_info* p = (p_channel_info*)rxbuf;
			if (p->packet_version != 2) {
				SDL_Log("got packet P_CHANNEL_INFO version %i but waited for version 2\n", p->packet_version);
				continue;
			}

			graph_world.update_channel_info(p->stream_id, p->channel_index,
				p->channel_name, p->unit, p->datatype, p->line_color_rgba, p->line_width,
				p->value_min, p->value_max,
				p->portal_x1, p->portal_y1, p->portal_x2, p->portal_y2);
		}
	}

	//sendto(_socket, txbuf, strlen(txbuf), 0, (SOCKADDR*)&remote_Addr, server_length);
}
