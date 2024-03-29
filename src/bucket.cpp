#include "bucket.h"

#include <algorithm>
#include <boost/beast/http/message.hpp>
#include <ctime>
#include <iostream>
#include <map>
#include <vector>

#include "http.h"
#include "network_request.h"

std::map<uint32_t, bucket::RequestBucket> requestBuckets = {};

namespace bucket {
RequestBucket::RequestBucket() : start_timestamp(std::time(0)), bucket_requests({}) {}
bucket::insertion_status RequestBucket::try_emplace(request::NetworkRequest &request_) {
    const bool max_requests = bucket_requests.size() > bucket::requests_per_bucket;

    if (request_.request_type == request::nice_type::user_request or
        request_.request_type == request::nice_type::high_priority_request) {
        if (max_requests) {
            // TODO: Switch sort to priority from niceness
            std::sort(bucket_requests.begin(), bucket_requests.end(),
                      [](const request::NetworkRequest first, const request::NetworkRequest second) {
                          return first.nice < second.nice;
                      });

            if (bucket_requests.back().request_type == request::nice_type::user_request) {
                bucket_requests.pop_back();
            }
        }

        bucket_requests.push_back(request_);
        return bucket::insertion_status::immediate_insert;
    }

    return bucket::insertion_status::queued;
}

bucket::RequestBucket insert_request(request::NetworkRequest &request_) {
    requestBuckets.try_emplace(request_.user, RequestBucket());

    RequestBucket bucket_ = requestBuckets[request_.user];
    bucket::insertion_status status = bucket_.try_emplace(request_);

    switch (status) {
        case bucket::insertion_status::immediate_insert:
            std::cout << "immediate" << std::endl;
            break;
        case bucket::insertion_status::queued:
            std::cout << "queued" << std::endl;
            break;
    }

    if (status == bucket::insertion_status::immediate_insert) {
        std::cout << "HTTP request enqueued" << std::endl;
        http::emplace_request(request_);
        return bucket_;
    }

    // TODO: Push request to request queue

    return bucket_;
}
};  // namespace bucket
