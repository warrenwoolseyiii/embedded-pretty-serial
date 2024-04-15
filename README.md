# embedded-pretty-serial
Lightweight serial printing library for embedded systems. Compatible with both C, and C++ systems. Major features of this library include:
1. Thread safe operation
2. ISR safe operation
3. File / module location included in the print log
4. Color / level scheme, with maskable lavels
5. Easy implementation and customization

# Demo the library
To demo the library, simply navigate to the `example` directory. There are two demos that run the same logic, a c, and c++ demo. Navigate to either directory and use the make file to build the application:
```
make clean
make
./main
```
The demo will run until SIGINT is recieved from the user.
Below is some example output:
```
[SYS]:main.c:59: This is a debug message
[SYS]:main.c:60: This is an info message
[SYS]:main.c:61: This is an okay message
[SYS]:main.c:62: This is a warning message
[SYS]:main.c:63: This is an error message
[SYS]:main.c:65: Here is a number: 42
[SYS]:main.c:66: Here is a string: Hello, world!
[SYS]:main.c:67: Uh oh a float: 3.140000
[SYS]:main.c:68: And a double: 2.718280
[SYS]:main.c:11: This is a message from a thread
[SYS]:main.c:21: This is a message from a second thread
[SYS]:main.c:31: This is a message from a third thread
[SYS]:main.c:11: This is a message from a thread
[SYS]:main.c:31: This is a message from a third thread
[SYS]:main.c:21: This is a message from a second thread
[SYS]:main.c:11: This is a message from a thread
[SYS]:main.c:11: This is a message from a thread
[SYS]:main.c:31: This is a message from a third thread
[SYS]:main.c:21: This is a message from a second thread
```

# Dependencies
This library depends on the standard c and c++ libraries. There is also a submodule dependency on https://github.com/warrenwoolseyiii/embedded-ring-buffer. This is a thread safe ring buffer module, it is easily swappable if you choose to use a different ring buffer module. All dependencies are located in the `deps` directory.

# Source & usage
All source files are located in the `src` directory. There are several configurable options in `emb_pretty_serial.h` listed below:
1. The compiler must define `SERIAL_EN && SYS_DEBUG_EN` to be true. You can define these in the header or as a compile time symbol. If you wish to disable pretty serial output, simply define `SERIAL_EN=0` which will remove all of the log macros from your source code. 
2. log levels and types are assigned in these enums:
```
// NOTE: these will correspond to color codes in lib/Printf/debug.cpp
enum log_levels
{
   LL_INFO  = 0,
   LL_OKAY  = 1,
   LL_WARN  = 2,
   LL_ERROR = 3,
   LL_DEBUG = 4
};

enum log_types
{
   LT_SYS = 0
};
```
You can change the levels and add log types other than `LT_SYS` if you choose to do so.
3. Finally, there are several defines at the top of the header file which can help you configure the module to your needs:
```
// Change this to change the maximum allowable string size
#define MAX_ALLOWABLE_STR_SIZE    512

// Change this to change the global print ring buffer size
#define PRINT_RB_SIZE             4096

// Comment this out to remove file locations
#define DEBUG_LOCATIONS_EN        1

// Comment this out to disable colored logs
#define DEBUG_COLORS_EN           1

// Change this to allow levels
// DEBUG | ERROR | WARN | OKAY | INFO
#define DEBUG_LVL_MASK    0b11111
```

