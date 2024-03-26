#include <boost/asio/steady_timer.hpp>
#include <boost/system/detail/error_code.hpp>
#include <iostream>

#include "datagram_server.cpp"

#if not defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
# error Local sockets not available on this platform.
#endif

using namespace boost;

boost::asio::io_context io_context;
boost::asio::steady_timer timer(io_context, boost::asio::chrono::seconds(5));

void tick(const boost::system::error_code&) {
    std::cout << "tick" << std::endl;
    timer.expires_at(timer.expires_at() + boost::asio::chrono::seconds(5));
    timer.async_wait(tick);
}

int main() {
    ::unlink("/tmp/scheduler.sock");
    datagram_server server(io_context);

    timer.async_wait(tick);

    io_context.run();

    return 0;
}
