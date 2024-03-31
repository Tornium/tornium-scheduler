#include <cpr/cpr.h>
#include <unistd.h>

#include <ctime>
#include <iostream>

#include "network_request.h"

std::list<std::unique_ptr<cpr::AsyncResponse>> pending_requests = {};

namespace http {
void emplace_request(request::NetworkRequest &request_) {
    // cpr::Url url = cpr::Url("https://api.torn.com" + request_.endpoint);
    cpr::Url url = cpr::Url("https://api.ipify.org?format=json" + request_.endpoint);
    pending_requests.push_back(std::make_unique<cpr::AsyncResponse>(cpr::GetAsync(url)));
    std::cout << "Pending request count: " << pending_requests.size() << std::endl;
}

void check_requests() {
    std::cout << "Starting request checks" << std::endl;

    do {
        std::list<std::unique_ptr<cpr::AsyncResponse>>::iterator request_iterator = pending_requests.begin();

        // FIXME: For some reason, without a usleep here, the while loop has issues checking values
        usleep(5);

        while (request_iterator != pending_requests.end()) {
            auto pending_response = request_iterator->get();
            if (pending_response->valid() &&
                pending_response->wait_for(std::chrono::seconds{0}) == std::future_status::ready) {
                const cpr::Response response = pending_response->get();
                request_iterator = pending_requests.erase(request_iterator);
            } else {
                request_iterator++;
            }
        };
    } while (true);
}
}  // namespace http
