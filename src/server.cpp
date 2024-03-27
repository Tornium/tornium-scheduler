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

#include "network_request.h"
#include "server.h"

#if not defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
# error Local sockets not available on this platform.
#endif

using namespace boost;
using namespace request;

const size_t server::DatagramServer::max_length = 128;

namespace server {
DatagramServer::DatagramServer(boost::asio::io_context& io_context) : socket_(io_context, boost::asio::local::datagram_protocol::endpoint("/tmp/scheduler.sock")) {};
void DatagramServer::do_receive() {
    socket_.async_receive_from(
        boost::asio::buffer(data_, server::DatagramServer::max_length), sender_endpoint_, [this](boost::system::error_code error, std::size_t bytes_received) {
            if (!error and bytes_received > 0) {
                request::NetworkRequest r = request::parse_request(data_, bytes_received);
                request::enqueue_request(r);
                do_send(bytes_received);
            } else {
                do_receive();
            }
        }
    );
}
void DatagramServer::do_send(std::size_t length) {
    socket_.async_send_to(
        boost::asio::buffer("ACK", 3), sender_endpoint_, [this](boost::system::error_code, std::size_t) {
            do_receive();
        }
    );
}
}
