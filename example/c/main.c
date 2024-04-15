#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "../../src/emb_pretty_serial.h"

void * test_thread1(void *arg)
{
   while (1)
   {
      SYS_INFO("This is a message from a thread\n");
      sleep(1);
   }
   return(NULL);
}

void * test_thread2(void *arg)
{
   while (1)
   {
      SYS_WARN("This is a message from a second thread\n");
      sleep(2);
   }
   return(NULL);
}

void * test_thread3(void *arg)
{
   while (1)
   {
      SYS_ERROR("This is a message from a third thread\n");
      usleep((rand() % 4990 + 10) * 1000);
   }
   return(NULL);
}

void * print_thread(void *arg)
{
   while (1)
   {
      uint8_t buf[128];
      int     len = get_log_message(buf, 128);
      for (int i = 0; i < len; i++)
      {
         printf("%c", buf[i]);
      }
      usleep(10000);
   }
   return(NULL);
}

int main()
{
   init_pretty_serial();

   pthread_t pthread;
   pthread_create(&pthread, NULL, print_thread, NULL);

   SYS_DBG("This is a debug message\n");
   SYS_INFO("This is an info message\n");
   SYS_OK("This is an okay message\n");
   SYS_WARN("This is a warning message\n");
   SYS_ERROR("This is an error message\n");

   SYS_INFO("Here is a number: %d\n", 42);
   SYS_INFO("Here is a string: %s\n", "Hello, world!");
   SYS_WARN("Uh oh a float: %f\n", 3.14);
   SYS_ERROR("And a double: %lf\n", 2.71828);

   pthread_t t1, t2, t3;
   pthread_create(&t1, NULL, test_thread1, NULL);
   pthread_create(&t2, NULL, test_thread2, NULL);
   pthread_create(&t3, NULL, test_thread3, NULL);

   pthread_join(t1, NULL);
   pthread_join(t2, NULL);
   pthread_join(t3, NULL);
   pthread_join(pthread, NULL);

   return(0);
}
