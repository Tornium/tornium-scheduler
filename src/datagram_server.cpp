#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/local/datagram_protocol.hpp>
#include <boost/system/detail/error_code.hpp>
#include <iostream>

#if not defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
# error Local sockets not available on this platform.
#endif

using namespace boost;

class datagram_server {
public:
    datagram_server(boost::asio::io_context& io_context) : socket_(io_context, boost::asio::local::datagram_protocol::endpoint("/tmp/scheduler.sock")) {
        do_receive();
    }
    
    void do_receive() {
        socket_.async_receive_from(
            boost::asio::buffer(data_, 3), sender_endpoint_,
            [this](boost::system::error_code error, std::size_t bytes_received) {
                if (!error and bytes_received > 0) {
                    std::cout << data_ << std::endl;
                    do_send(bytes_received);
                } else {
                    do_receive();
                }
        });
    }

    void do_send(std::size_t length) {
        socket_.async_send_to(
            boost::asio::buffer("ACK", 3), sender_endpoint_,
            [this](boost::system::error_code, std::size_t) {
                do_receive();
            });
    }

private:
    boost::asio::local::datagram_protocol::socket socket_;
    boost::asio::local::datagram_protocol::endpoint sender_endpoint_;
    enum { max_length = 128 };
    char data_[max_length];
};

