#include <boost/asio/io_service.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <array>
#include <string>
#include <iostream>

using namespace boost::asio;
using namespace boost::asio::ip;

io_service ioservice;
tcp::resolver resolv{ioservice};
tcp::socket tcp_socket{ioservice};
std::array<char, 4096> bytes;

void read_handler(const boost::system::error_code &ec,
  std::size_t bytes_transferred)
{
  if (!ec)
  {
    std::cout << "Received : " << bytes_transferred << " bytes" << std::endl;
    std::cout.write(bytes.data(), bytes_transferred);
    tcp_socket.async_read_some(buffer(bytes), read_handler);
  }
      else
  {
      std::cout << "Read failed with error code :" << ec.message() << std::endl;
  }
}

void connect_handler(const boost::system::error_code &ec)
{
  if (!ec)
  {
    std::string r =
      "GET / HTTP/1.1\r\nHost: example.com\r\n\r\n";
    write(tcp_socket, buffer(r));
    tcp_socket.async_read_some(buffer(bytes), read_handler);
  }
    else
  {
      std::cout << "Connect failed with error code :" << ec.message() << std::endl;
  }
}

void resolve_handler(const boost::system::error_code &ec,
  tcp::resolver::iterator it)
{
  if (!ec)
    tcp_socket.async_connect(*it, connect_handler);
  else
  {
      std::cout << "Resolve failed with error code :" << ec.message() << std::endl;
  }
  
}

/* Route resover through proxy if proxy is enabled */
void GET(std::string url, std::string port)
{
  std::string http_proxy = getenv("http_proxy");
  if(!http_proxy.empty())
  {
      size_t pos = http_proxy.find_last_of(':');
      std::string proxy_url, proxy_port;
      if( pos != std::string::npos)
      {
          proxy_url = http_proxy.substr(0,pos);
          if(proxy_url.find("http://") == 0)
            proxy_url = proxy_url.substr(7);
          proxy_port = http_proxy.substr(pos+1);
          if(proxy_port[proxy_port.length() - 1] == '/')
            proxy_port = proxy_port.substr(0, proxy_port.length() - 1);
          std::cout << "Proxy url : " << proxy_url.c_str() << std::endl;
          std::cout << "Proxy port : " << proxy_port.c_str() << std::endl;
      }
      url = proxy_url;
      port = proxy_port;
  }
  tcp::resolver::query q{url,port};
  resolv.async_resolve(q, resolve_handler);
}

int main()
{
  GET("www.example.com", "80");
  ioservice.run();
}