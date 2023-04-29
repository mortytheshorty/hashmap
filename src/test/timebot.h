/* Copyright (c) [2023] [Florian Giest]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. */

#ifndef _TIMEBOT_H
#define _TIMEBOT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

typedef struct excecute_bundle {
    const char *description;
    const char *function_name;
    int (*run)(void*);
    void *arg;
    double runtime;
} excecute_bundle;

typedef struct timebot {
    excecute_bundle *exec_pool;
    int n;
    double capacity;
    double total_time;
    double longest_rtime;
    const char *longest_rtime_fname;
    int longest_function_name_length;
} timebot;

timebot *timebot_create(size_t n);
void timebot_destroy(timebot *tb);



/* runs function with a argumenet and returns runtime */
void _get_runtime(excecute_bundle *bundle);

void _tb_append(timebot *tb, void (*func), void *arg, const char *name, const char *description);




/* runs all appended functions */
void timebot_run(timebot *tb);

/* compare functions execution time */
void timebot_compare(timebot *tb);

/* prints a summery of all executions */
void timebot_print_summery(timebot *tb);

/* append to timebot's execution list */
#define timebot_append(tb, func, arg, desc) _tb_append(tb, func, arg, #func, desc)
#define timebot_standalone(tb, func, arg, desc) _tb_standalone(tb, func, arg, #func, desc)

#endif