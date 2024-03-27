#include "network_request.h"
#include "server.h"
#include <iostream>
#include <map>

using namespace request;
using namespace server;

static std::multimap<std::string, request::NetworkRequest> requests_map = {};


namespace request {
request::NetworkRequest parse_request(
    char *data_,
    const size_t& bytes_received
) {
    uint8_t parse_step = 0;
    std::string nice_string = "";
    std::string endpoint = "";

    for(int i=0; i < std::min(bytes_received, server::DatagramServer::max_length); i++) {
        if (data_[i] == '\n') {
            parse_step++;
            continue;
        }

        switch (parse_step) {
            case 0:
                nice_string += data_[i];
                break;
            case 1:
                endpoint += data_[i];
                break;
        }
    }

    request::NetworkRequest request_ = {
        (int8_t) std::stoi(nice_string),              // niceness
        endpoint,                                     // endpoint
        endpoint.substr(1, endpoint.find('?') - 1),   // endpoint_id
        std::vector<NetworkRequest> {},               // linked requests
        std::time(0),                                 // timestamp_started
    };

    return request_;
}

void enqueue_request(request::NetworkRequest request_) {
    if (requests_map.count(request_.endpoint_id) > 0) {
        auto existing_keys_range = requests_map.equal_range(request_.endpoint_id);

        for (auto i = existing_keys_range.first; i != existing_keys_range.second; i++) {
            // TODO: Implement this to check the endpoint used to determine if the request should be linked to the existing request
            return;
        }
    }

    requests_map.insert({request_.endpoint_id, request_});
}

size_t requests_count() {
    return requests_map.size();
}
};
