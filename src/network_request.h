#ifndef NETWORK_REQUEST_H
#define NETWORK_REQUEST_H

#include <array>
#include <cstdint>
#include <ctime>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace request {
struct NetworkRequest {
    const int8_t nice;
    const std::string endpoint;
    const std::string endpoint_id;
    std::vector<NetworkRequest> linkedRequests;

    const std::time_t requestReceived;
    const std::optional<std::time_t> requestScheduled;
    const std::optional<std::time_t> requestFinished;
};

NetworkRequest parse_request(
    char *data_,
    const size_t& bytes_received
);
void enqueue_request(request::NetworkRequest request_);

size_t requests_count();
}

#endif
