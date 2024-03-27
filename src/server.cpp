#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/local/datagram_protocol.hpp>
#include <boost/system/detail/error_code.hpp>
#include <cstddef>
#include <iostream>

#include "server.h"

#if not defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
# error Local sockets not available on this platform.
#endif

using namespace boost;

namespace server {
DatagramServer::DatagramServer(boost::asio::io_context& io_context) : socket_(io_context, boost::asio::local::datagram_protocol::endpoint("/tmp//scheduler.sock")) {};
void DatagramServer::do_receive() {
    socket_.async_receive_from(
        boost::asio::buffer(data_, 3), sender_endpoint_, [this](boost::system::error_code error, std::size_t bytes_received) {
            if (!error and bytes_received > 0) {
                std::cout << data_ << std::endl;
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
