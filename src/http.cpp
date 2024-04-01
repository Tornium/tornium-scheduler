#include <cpr/cpr.h>
#include <unistd.h>

#include <boost/system/result.hpp>
#include <boost/url.hpp>
#include <boost/url/urls.hpp>
#include <chrono>
#include <cstddef>
#include <ctime>
#include <iostream>
#include <memory>

#include "cpr/session.h"
#include "network_request.h"

std::list<std::unique_ptr<cpr::AsyncResponse>> pending_requests = {};

// Pre-allocate memory that doesn't need to be deleted
std::list<std::unique_ptr<cpr::AsyncResponse>>::iterator request_iterator;
boost::url_view request_url;
const std::chrono::seconds request_wait_period = std::chrono::seconds{0};

namespace http {
void emplace_request(request::NetworkRequest &request_) {
    // cpr::Url url = cpr::Url("https://api.torn.com" + request_.endpoint);
    const cpr::Url url = cpr::Url("https://api.ipify.org" + request_.endpoint);

    pending_requests.push_back(std::make_unique<cpr::AsyncResponse>(cpr::GetAsync(url)));

    std::cout << "Pending request count: " << pending_requests.size() << std::endl;
    return;
}

void check_requests() {
    std::cout << "Starting request checks" << std::endl;

    do {
        // FIXME: For some reason, without a usleep here, the while loop has issues checking values
        // TODO: Re-use sessions and connections
        usleep(5);

        request_iterator = pending_requests.begin();

        while (request_iterator != pending_requests.end()) {
            const auto pending_response = request_iterator->get();
            if (pending_response->valid() and
                pending_response->wait_for(request_wait_period) == std::future_status::ready) {
                const cpr::Response response = pending_response->get();

                if (not response.error.message.empty()) {
                    // TODO: Make sure that error checking is valid
                    // TODO: Handle errors properly
                    // TODO: Implement max retries into request class and retries into logic
                    std::cout << response.status_code << std::endl;
                    std::cout << response.error.message << std::endl;
                }
                request_iterator = pending_requests.erase(request_iterator);
                boost::url_view request_url = boost::url_view(
                    boost::system::result<boost::url_view>(boost::urls::parse_uri(response.url.str())).value());
                request::remove_request(request_url.path());
            } else {
                request_iterator++;
            }
        };
    } while (true);
}
}  // namespace http
