#include <boost/asio/steady_timer.hpp>
#include <boost/system/detail/error_code.hpp>
#include <iostream>
#include <thread>

#include "http.h"
#include "network_request.h"
#include "server.h"

#if not defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
#error Local sockets not available on this platform.
#endif

boost::asio::io_context io_context;
boost::asio::steady_timer timer(io_context, boost::asio::chrono::seconds(5));

void tick(const boost::system::error_code &) {
    std::cout << "tick :: " << request::requests_count() << std::endl;
    timer.expires_at(timer.expires_at() + boost::asio::chrono::seconds(5));
    timer.async_wait(tick);
}

int main() {
    ::unlink("/tmp/scheduler.sock");
    server::DatagramServer s(io_context);
    s.do_receive();

    timer.async_wait(tick);

    std::thread api_thread(http::check_requests);

    io_context.run();

    return 0;
}
