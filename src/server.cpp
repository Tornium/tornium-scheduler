#include "server.h"

#include <algorithm>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/local/datagram_protocol.hpp>
#include <boost/system/detail/error_code.hpp>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <string>

#include "bucket.h"
#include "network_request.h"

#if not defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
#error Local sockets not available on this platform.
#endif

using namespace boost;
using namespace request;

const size_t server::DatagramServer::max_length = 128;

namespace server {
DatagramServer::DatagramServer(boost::asio::io_context &io_context)
    : socket_(io_context, boost::asio::local::datagram_protocol::endpoint("/tmp/scheduler.sock")){};
void DatagramServer::do_receive() {
    socket_.async_receive_from(boost::asio::buffer(data_, server::DatagramServer::max_length), sender_endpoint_,
                               [this](boost::system::error_code error, std::size_t bytes_received) {
                                   if (!error and bytes_received > 0) {
                                       request::NetworkRequest r = request::parse_request(data_, bytes_received);

                                       std::cout << "Received request... " << r.endpoint << " :: " << (int)r.nice
                                                 << " :: " << r.user << std::endl;

                                       if (request::enqueue_request(r)) {
                                           std::cout << "Request enqueued" << std::endl;
                                           bucket::insert_request(r);
                                       }

                                       do_send();
                                   } else {
                                       do_receive();
                                   }
                               });
}
void DatagramServer::do_send() {
    socket_.async_send_to(boost::asio::buffer("ACK", 3), sender_endpoint_,
                          [this](boost::system::error_code, std::size_t) { do_receive(); });
}
}  // namespace server
