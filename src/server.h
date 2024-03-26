#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>

using namespace boost;

namespace server {
class DatagramServer {
public:
    DatagramServer(boost::asio::io_context& io_context);

    void do_receive();
    void do_send(std::size_t length);

private:
    boost::asio::local::datagram_protocol::socket socket_;
    boost::asio::local::datagram_protocol::endpoint sender_endpoint_;
    enum { max_length = 128 };
    char data_[max_length];
};
}

#endif
