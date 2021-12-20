/**
 * \file            list.h
 * \brief           List include file
 */

#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "defines.h"

/*
 * \brief           Store single section data
 *
 */
typedef struct {
    int section_index;                             /*!< Filename / URL */
    int input_count;                               /*!< Input count */
    char *input[SECTION_MAX_SIZE];                 /*!< Point data for sections */
} section_t;

/* 
 * \brief           Singly linked list to store each page information
 *                  
 */
typedef struct data {
    int section_count;                           /*!< Point added section count */
    char *url;                                   /*!< Filename / URL */
    section_t section_ptr[SECTION_MAX_COUNT];    /*!< Point added sections */

    struct data *next;                           /*!< Point next node */
} data_t;

void create_list(data_t **root);

void delete_list(data_t **root);

void add_node(data_t **root);

#endif /* LIST_H_INCLUDED */