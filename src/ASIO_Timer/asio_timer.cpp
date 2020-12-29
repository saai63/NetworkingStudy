/*
 * It is pretty interesting to note in the below example that most of the times,
 * the async jobs are executed by the same thread.
 */

#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include <sys/syscall.h> /* For SYS_xxx definitions */

using namespace boost::asio;

void dummy_func(const boost::system::error_code &ec)
{
   std::cout <<"[Tid:" << std::this_thread::get_id() << "][" << syscall(SYS_gettid) << "]" << __LINE__ << std::endl;
}

int main()
{
   io_service ioservice;

   /* Async task 1 */
   steady_timer timer{ioservice, std::chrono::seconds{3}};
   timer.async_wait([](const boost::system::error_code &ec)
      { std::cout << "[Tid:" << std::this_thread::get_id() << "][" << syscall(SYS_gettid) << "] 3 sec\n"; });
   
   /* Async task 2 */
   steady_timer timer1{ioservice, std::chrono::seconds{3}};
   timer1.async_wait([](const boost::system::error_code &ec)
      { std::cout << "[Tid:" << std::this_thread::get_id() << "][" << syscall(SYS_gettid) << "] 5 sec\n"; });
   
   /* Async task 3 - Non lambda callback */
   steady_timer timer2{ioservice, std::chrono::seconds{3}};
   timer2.async_wait(dummy_func);

   /* Threads to execute the async tasks*/
   std::thread t1{ [&ioservice](){ioservice.run();} };
   std::thread t2{ [&ioservice](){ioservice.run();} };
   std::thread t3{ [] (){
   for (auto idx=0; idx<=5; idx++)
   {
      std::cout << "[Tid:" << std::this_thread::get_id() << "][" << syscall(SYS_gettid) << "] Time : " << idx << " seconds\n"; 
      sleep(1);
   }}
   };

   /* Wait for threads to complete their jobs*/   
   t1.join();
   t2.join();
   t3.join();
   
}
