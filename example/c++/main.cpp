#include <iostream>
#include <thread>
#include "../../src/emb_pretty_serial.h"

void test_thread1()
{
   while (true)
   {
      SYS_INFO("This is a message from a thread\n");
      std::this_thread::sleep_for(std::chrono::seconds(1));
   }
}

void test_thread2()
{
   while (true)
   {
      SYS_WARN("This is a message from a second thread\n");
      std::this_thread::sleep_for(std::chrono::seconds(2));
   }
}

void test_thread3()
{
   while (true)
   {
      SYS_ERROR("This is a message from a third thread\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 4990 + 10));
   }
}

void print_thread()
{
   while (true)
   {
      uint8_t buf[128];
      int     len = get_log_message(buf, 128);
      for (int i = 0; i < len; i++)
      {
         std::cout << buf[i];
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
   }
}

int main()
{
   init_pretty_serial();

   std::thread pthread(print_thread);

   SYS_DBG("This is a debug message\n");
   SYS_INFO("This is an info message\n");
   SYS_OK("This is an okay message\n");
   SYS_WARN("This is a warning message\n");
   SYS_ERROR("This is an error message\n");

   SYS_INFO("Here is a number: %d\n", 42);
   SYS_INFO("Here is a string: %s\n", "Hello, world!");
   SYS_WARN("Uh oh a float: %f\n", 3.14);
   SYS_ERROR("And a double: %lf\n", 2.71828);

   std::thread t1(test_thread1);
   std::thread t2(test_thread2);
   std::thread t3(test_thread3);

   t1.join();
   t2.join();
   t3.join();
   pthread.join();
   return(0);
}
