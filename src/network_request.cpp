#include "network_request.h"

#include <iostream>
#include <map>
#include <optional>
#include <ostream>

#include "bucket.h"
#include "server.h"

static std::multimap<std::string, request::NetworkRequest &> requests_map = {};

namespace request {
request::NetworkRequest parse_request(char *data_, const size_t &bytes_received) {
    // TODO: Add type and value checkign to this
    uint8_t parse_step = 0;
    std::string nice_string = "";
    std::string endpoint = "";
    std::string user_string = "";

    for (size_t i = 0; i < std::min(bytes_received, server::DatagramServer::max_length); i++) {
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
            case 2:
                user_string += data_[i];
                break;
        }
    }

    int8_t niceness = static_cast<int8_t>(std::stoi(nice_string));
    uint32_t user = static_cast<uint32_t>(std::stoul(user_string));
    request::nice_type parsed_request_type;

    if (niceness <= -10) {
        parsed_request_type = request::nice_type::user_request;
    } else if (niceness <= 0) {
        parsed_request_type = request::nice_type::high_priority_request;
    } else {
        parsed_request_type = request::nice_type::generic_request;
    }

    request::NetworkRequest request_ = {
        niceness,                                // niceness
        endpoint,                                // endpoint
        endpoint.substr(0, endpoint.find('?')),  // endpoint_id
        user,                                    // user ID
        std::vector<NetworkRequest>{},           // linked requests
        std::time(0),                            // requestReceived
        std::nullopt,                            // requestScheduled
        parsed_request_type,                     // request_type
    };

    return request_;
}

bool enqueue_request(request::NetworkRequest &request_) {
    if (requests_map.count(request_.endpoint_id) > 0) {
        auto existing_keys_range = requests_map.equal_range(request_.endpoint_id);

        for (auto i = existing_keys_range.first; i != existing_keys_range.second; i++) {
            // TODO: Implement this to check the endpoint used to determine if the
            // request should be linked to the existing request
            return false;
        }
    }

    requests_map.insert({request_.endpoint_id, request_});
    return true;
}

size_t requests_count() { return requests_map.size(); }

void remove_request(std::string request_key) { requests_map.erase(request_key); }
};  // namespace request
