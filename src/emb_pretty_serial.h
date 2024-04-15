//MIT License
//
//Copyright (c) 2023 budgettsfrog
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
#ifndef EMB_PRETTY_SERIAL_H_
#define EMB_PRETTY_SERIAL_H_

// C++ guard
#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#define S1(x)    #x
#define S2(x)    S1(x)

#define GLOBAL_PBUF_SZ        512
#define PRINT_RB_SIZE         4096

// Comment this out to remove file locations
#define DEBUG_LOCATIONS_EN    1

// Comment this out to disable colored logs
#define DEBUG_COLORS_EN       1

// Change this to allow levels
// DEBUG | ERROR | WARN | OKAY | INFO
#define DEBUG_LVL_MASK    0b11111

#if DEBUG_LOCATIONS_EN
#define LOCATION          __FILE__ ":" S2(__LINE__)
#else
#define LOCATION          NULL
#endif /* DEBUG_LOCATIONS_EN */

/**
 * @brief Initialize the pretty serial module.
 */
void init_pretty_serial(void);

/**
 * @brief Enable or dsiable run time debug print bypass.
 * @param bypass set to true to bypass debug printing, false otherwise.
 */
void bypass_debug(bool bypass);

/**
 * @brief Logging output redirect function, this is called internally, don't call this directly.
 * @param prio Priority of the log message.
 * @param header Pointer to the header of the log message.
 * @param location Location of the log message.
 */
int make_pretty_header(uint16_t prio, char *header, const char *location);

/**
 * @brief Queue the log message to be printed.
 * @param buf Pointer to the buffer containing the log message.
 * @param len Length of the log message.
 * @return 0 on success, -1 on failure.
 */
int queue_log_message(const uint8_t *buf, int len);

/**
 * @brief Get log message bytes to you can print them.
 * @param buf Pointer to the buffer to store the log message.
 * @param len Length of the buffer.
 * @return Number of bytes copied to the buffer.
 */
int get_log_message(uint8_t *buf, int len);

/**
 * @brief Get the version of the pretty serial module.
 * @return Pointer to the version string.
 */
const char *pretty_ser_get_ver();

#define log_printf(prio, ...)                                                    \
        {                                                                        \
           do {                                                                  \
              char global_print_buf[GLOBAL_PBUF_SZ];                             \
              int  i = make_pretty_header(prio, &global_print_buf[0], LOCATION); \
              i += sprintf((char *)&global_print_buf[i], __VA_ARGS__);           \
              queue_log_message((uint8_t *)global_print_buf, i);                 \
           } while (0);                                                          \
        }

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

#define _MKPRI(type, level)    (((LT_ ## type) << 3) + LL_ ## level)

#if SERIAL_EN && SYS_DEBUG_EN
#define DBG_ASSERT(a, b)       if (!(a)) {                                  \
           SYS_ERR("\n--------------------------------------------------\n" \
                   "Assert failed: %s\n%s\nLine %u in %s\n"                 \
                   "--------------------------------------------------\n",  \
                   #a, (b), __LINE__, __FILE__);                            \
           while (true);                                                    \
}
#define SYS_DBG(...)           log_printf(_MKPRI(SYS, DEBUG), __VA_ARGS__)
#define SYS_INFO(...)          log_printf(_MKPRI(SYS, INFO), __VA_ARGS__)
#define SYS_OK(...)            log_printf(_MKPRI(SYS, OKAY), __VA_ARGS__)
#define SYS_WARN(...)          log_printf(_MKPRI(SYS, WARN), __VA_ARGS__)
#define SYS_ERROR(...)         log_printf(_MKPRI(SYS, ERROR), __VA_ARGS__)
#else
#define DBG_ASSERT(a, b)
#define SYS_DBG(...)
#define SYS_INFO(...)
#define SYS_OK(...)
#define SYS_WARN(...)
#define SYS_ERROR(...)
#endif /* SERIAL_EN && SYS_DEBUG_EN */

// TODO: You can add more macro types here if you'd like to have more logging modules

// C++ guard
#ifdef __cplusplus
}
#endif

#endif /* EMB_PRETTY_SERIAL_H_ */
