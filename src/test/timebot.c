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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "timebot.h"

/**
 * @brief Creates a timebot object for a module
 * 
 * @param module_name specified module name
 * @param n number of preallocated functions 
 * @return timebot* 
 */
timebot *timebot_create(size_t n)
{
    timebot *bot = (timebot *) calloc(1, sizeof *bot);
    if(bot == NULL) {
        perror("timebot_create");  /* this must not happen */
        abort();
    }

    bot->exec_pool = (excecute_bundle *) calloc(n, sizeof *bot->exec_pool);
    if(bot->exec_pool == NULL) {
        perror("timebot_create execution pool");
        abort();
    }

    return bot;
}

void timebot_destroy(timebot *tb)
{
    free(tb->exec_pool);
    free(tb);
}

/* runs function with a argumenet and returns runtime */
void _get_runtime(excecute_bundle *bundle)
{
    clock_t start = clock();
    bundle->run(bundle->arg);
    bundle->runtime =  (double) (clock() - start) / CLOCKS_PER_SEC;
}

void _tb_append(timebot *tb, void (*func), void *arg, const char *name, const char *description)
{
    excecute_bundle *current = &tb->exec_pool[tb->n];
    size_t fname_len = strlen(name);

    if(fname_len > tb->longest_function_name_length) {
        tb->longest_function_name_length = fname_len;
    }

    current->function_name = name;
    current->run = func;
    current->arg = arg;
    current->description = description;

    tb->n++;
}

/* runs all appended functions */
void timebot_run(timebot *tb) {
    excecute_bundle *current;
    for(size_t i = 0; i < tb->n; i++) {

        current = &tb->exec_pool[i];

        if(current->run != NULL) {
            printf("Running `%*s`: ", -tb->longest_function_name_length, current->function_name);
            _get_runtime(current);
            printf("COMPLETED\n");
            if(current->runtime > tb->longest_rtime) {
                tb->longest_rtime = current->runtime;
                tb->longest_rtime_fname = current->function_name;
            }
        }
    }

    timebot_compare(tb);
}

/* compare functions execution time */
void timebot_compare(timebot *tb)
{

    for(size_t n = tb->n; n > 1; --n) {
        for(size_t i = 0; i < (n - 1); ++i) {
            if(tb->exec_pool[i].runtime > tb->exec_pool[i+1].runtime) {
                excecute_bundle tmp = tb->exec_pool[i];
                tb->exec_pool[i] = tb->exec_pool[i+1];
                tb->exec_pool[i+1] = tmp;
            }
        }
    }


    excecute_bundle *current;
    printf("\nComparison:\n");
    for(size_t i = 0; i < tb->n; i++) {

        current = &tb->exec_pool[i];
        printf("%zu. [ %*s ] with %fs\n", i + 1,  -tb->longest_function_name_length, current->function_name, current->runtime);
    }
}


/* append to timebot's execution list */
#define timebot_append(tb, func, arg, desc) _tb_append(tb, func, arg, #func, desc)
#define timebot_standalone(tb, func, arg, desc) _tb_standalone(tb, func, arg, #func, desc)

