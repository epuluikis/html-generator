/**
 * \file            list.c
 * \brief           List source file
 */

#include "headers/list.h"
#include "headers/helpers.h"

/*
 * \brief           Create singly linked list
 * \note            This function does not return value, it stores it to pointer instead
 * \param[in]       root: First list node
 */
void create_list(data_t **root) {
    *root = NULL;
}

/*
 * \brief           Delete singly linked list
 * \note            This function does not return value, it stores it to pointer instead
 * \param[in]       root: First list node
 */
void delete_list(data_t **root) {
    data_t *temp;

    while (*root != NULL) {
        temp = *root;
        *root = (data_t *) (*root)->next;
        free(temp);
    }
}

/*
 * \brief           Add node to singly linked list
 * \note            This function does not return value, it stores it to pointer instead
 * \param[in]       root: First list node
 */
void add_node(data_t **root) {
    data_t *temp;

    temp = (data_t *) malloc(sizeof(data_t));

    if (temp == NULL) {
        delete_list(root);
        print_ofm();
        exit(0);
    }

    if (*root == NULL) {
        *root = temp;
        temp->next = NULL;
    } else {
        temp->next = *root;
        *root = temp;
    }

    temp->section_count = 0;
}