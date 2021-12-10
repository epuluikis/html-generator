/**
 * \file            contents.h
 * \brief           Initialize contents structure & dependencies
 */

#ifndef CONTENTS_H_INCLUDED
#define CONTENTS_H_INCLUDED

#include <string.h>
#include <windows.h>
#include <fileapi.h>

#define STR_MAX_LENGTH 256
#define FIELD_COUNT 14
#define SECTION_COUNT 5
#define SECTION_MAX_SIZE 7

/**
 * \brief           Holds all data required to generate template
 *
 */
typedef struct contents {
    char hashes_to_change[FIELD_COUNT][STR_MAX_LENGTH];     /*!< Hashes to replace */
    char interface_text[FIELD_COUNT][STR_MAX_LENGTH];       /*!< Interface text */
    char section_titles[SECTION_COUNT][STR_MAX_LENGTH];     /*!< Section titles */
    char user_input[FIELD_COUNT][STR_MAX_LENGTH];           /*!< User input */
    int section_ids[SECTION_COUNT][SECTION_MAX_SIZE];       /*!< Section - hash relation */
} t_contents;

int init_contents(t_contents* contents);

#endif /* CONTENTS_H_INCLUDED */