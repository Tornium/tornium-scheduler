#ifndef BUCKET_H
#define BUCKET_H

#include <ctime>

#include "network_request.h"

namespace bucket {
const static size_t requests_per_bucket = 6;
const static size_t bucket_interval = 10;
enum class insertion_status { queued, lazy_insert, immediate_insert };

class RequestBucket {
   public:
    RequestBucket();
    insertion_status try_emplace(request::NetworkRequest& request_);

   private:
    const std::time_t start_timestamp;
    std::vector<request::NetworkRequest> bucket_requests;
};

bucket::RequestBucket insert_request(request::NetworkRequest& request_);
}  // namespace bucket

#endif
