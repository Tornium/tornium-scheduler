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
    void do_send();

    // TODO: Make data_ use max_length in its constructor
    const static size_t max_length;
    char data_[128];

   private:
    boost::asio::local::datagram_protocol::socket socket_;
    boost::asio::local::datagram_protocol::endpoint sender_endpoint_;
};
}  // namespace server

#endif
