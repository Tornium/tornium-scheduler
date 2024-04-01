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
enum class nice_type { user_request, high_priority_request, generic_request };

struct NetworkRequest {
    int8_t nice;
    std::string endpoint;
    std::string endpoint_id;
    uint32_t user;
    std::vector<NetworkRequest> linkedRequests;

    std::time_t requestReceived;
    std::optional<std::time_t> requestScheduled;
    nice_type request_type;
};

NetworkRequest parse_request(char* data_, const size_t& bytes_received);
bool enqueue_request(request::NetworkRequest& request_);

size_t requests_count();
void remove_request(std::string request_key);
}  // namespace request

#endif
