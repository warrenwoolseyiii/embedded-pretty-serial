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
#include "emb_pretty_serial.h"
#include "../deps/embedded-ring-buffer/src/emb_rb.h"
#include <stdio.h>
#include <string.h>

uint8_t _pb_src[PRINT_RB_SIZE];
emb_rb_t _rb;
bool _bypass = false;

void init_pretty_serial(void)
{
    emb_rb_init(&_rb, _pb_src, PRINT_RB_SIZE);
}

void bypass_debug(bool bypass)
{
    _bypass = bypass;
}

#if DEBUG_COLORS_EN
// Define color codes for log levels
// Normal/Reset pattern:
// { 0x1b, '[', '0', ';', '3', '9', 'm', 0}
static uint8_t color_codes[8][3] =
{
    { '0', '0', '1' }, /* bold white - 0 */
    { '0', '3', '2' }, /* green      - 1 */
    { '1', '3', '3' }, /* yellow     - 2 */
    { '1', '3', '1' }, /* red        - 3 */
    { '1', '3', '4' }, /* blue       - 4 */
    { '0', '3', '9' }, /* normal/rst - 5 */
    { '0', '3', '6' }, /* cyan       - 6 */
    { '1', '3', '0' }  /* black      - 7 */
};
#else
// colors disabled, print level string instead
static const char *log_level_str(uint16_t lvl)
{
    switch (lvl) {
        case LL_INFO: return "INFO";
        case LL_OKAY: return "OKAY";
        case LL_WARN: return "WARN";
        case LL_ERROR: return "ERROR";
        case LL_DEBUG: return "DEBUG";
    }
    return "INVALID";
}
#endif /* DEBUG_COLORS_EN */

static const char *log_type_str(uint16_t prio)
{
    uint16_t type = prio & 0xFFF8;

    // printf("type=%hu\n", type);
    switch (type) {
        case LT_SYS << 3: return "SYS";
        // TODO: Add more log modules here
    }
    return "???";
}

int make_pretty_header(uint16_t prio, char *header, const char *location)
{
    if (_bypass) {
        return 0;
    }

    uint16_t lvl = (prio & 0x0007); // determine log level from first 3 bits
    if (!(DEBUG_LVL_MASK & (1 << lvl))) {
        return 0;
    }

    int  i = 0;

    header[i++] = ('['); // write left bracket

#if DEBUG_COLORS_EN
    // begin color encoding based on level
    char color_pbuf[] = { 0x1b, '[', '0', ';', '3', '9', 'm', 0x00 };
    color_pbuf[2] = color_codes[lvl][0];
    color_pbuf[4] = color_codes[lvl][1];
    color_pbuf[5] = color_codes[lvl][2];
    for (int j = 0; j < 7; j++) {
        header[i++] = (color_pbuf[j]);
    }
#else
    // write level string instead of coloring
    const char *level = log_level_str(lvl);
    while (*level) {
        header[i++] = (*level++);
    }
    // enclose level string and write new left bracket
    header[i++] = (']');
    header[i++] = ('[');
#endif /* DEBUG_COLORS_EN */

    // write log facility
    const char *faciltiy = log_type_str(prio);
    while (*faciltiy) {
        header[i++] = (*faciltiy++);
    }

#if DEBUG_COLORS_EN
    // terminate color encoding
    color_pbuf[2] = 'm';
    color_pbuf[3] = 0;
    color_pbuf[4] = 0;
    for (int j = 0; j < 4; j++) {
        header[i++] = (color_pbuf[j]);
    }
#endif /* DEBUG_COLORS_EN */

    // right bracket + spacer
    header[i++] = (']');
    header[i++] = (':');

#if DEBUG_LOCATIONS_EN
    // write location + spacer
    while (*location) {
        header[i++] = (*location++);
    }
    header[i++] = (':');
    header[i++] = (' ');
#endif /* DEBUG_LOCATIONS_EN */

    return i;


    // // write log message, the header must be done at the character level because
    // // the color codes contain null terminators.
    // for (int j = 0; j < i; j++) {
    //     printf("%c", header[j]);
    // }
    // printf("%s", str);
}

int queue_log_message(const uint8_t *buf, int len)
{
    if (_bypass) {
        return 0;
    }

    int err;
    emb_rb_queue(&_rb, buf, len, &err);
    return err;
    
}

int get_log_message(uint8_t *buf, int len)
{
    if (_bypass) {
        return 0;
    }

    int err;
    int l = emb_rb_dequeue(&_rb, buf, len, &err);
    return l;
}