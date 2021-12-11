/**
 * \file            helpers.h
 * \brief           Helps to deal with stuff
 */

#ifndef HELPERS_H_INCLUDED
#define HELPERS_H_INCLUDED

#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>

char* str_replace(char* orig, char* rep, char* with);

void console_text_color(char color);

int get_number(int limit);

char *validation(char* input);

#endif /* HELPERS_H_INCLUDED */