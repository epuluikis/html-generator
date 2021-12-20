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
#include <time.h>

char *str_replace(char *orig, char *rep, char *with);

void console_text_color(char color);

int get_number(int min, int max);

char *sanitize_input(char *input);

void clear_terminal();

char *read_sanitized_string();

char *read_string();

char *get_current_year();

void print_error(char *string);

void print_ofm();

void write_to_file(char *string, FILE *file);

void print_generating_message();

void print_generated_message();

void print_outro_message();

void print_introduction_message();

char *concat(const char *s1, const char *s2);

#endif /* HELPERS_H_INCLUDED */
