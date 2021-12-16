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
void
create_list(data_t** root)
{
    *root = NULL;
}

/*
 * \brief           Delete singly linked list
 * \note            This function does not return value, it stores it to pointer instead
 * \param[in]       root: First list node
 */
void
delete_list(data_t** root)
{
    data_t* temp;

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
void add_node(data_t** root)
{
    data_t *temp;

    temp = (data_t*)malloc(sizeof(data_t));

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
}

/*
 * \brief           Remove node from singly linked list
 * \note            This function does not return value, it stores it to pointer instead
 * \param[in]       root: First list node
 * \param[in]       value: Value of node to remove from list
 * \param[out]      root: First list node
 */
//void
//remove_nodes(node_t** root, int8_t value)
//{
//    node_t *temp = *root, *prev = NULL;
//
//    if (temp != NULL && temp->value == value) {
//        *root = temp->next;
//        free(temp);
//        temp = *root;
//    }
//
//    while (temp != NULL) {
//        while (temp != NULL && temp->value != value) {
//            prev = temp;
//            temp = temp->next;
//        }
//
//        if (temp == NULL) {
//            break;
//        }
//
//        prev->next = temp->next;
//        free(temp);
//        temp = prev->next;
//    }
//}