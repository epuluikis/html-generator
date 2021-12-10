/**
 * \file            helpers.h
 * \brief           Helps to deal with stuff
 */

#include "headers/helpers.h"

/**
 * \brief           Replace part of string with other string
 * \param[in]       orig: Original string
 * \param[in]       rep: Replace string
 * \param[in]       with: Replace with string
 * \param[out]      result: Pointer to new string
 */
char* str_replace(char* orig, char* rep, char* with)
{
    char* result;
    char* ins;
    char* tmp;
    int len_rep;
    int len_with;
    int len_front;
    int count;

    /* sanity checks and initialization */
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL;
    if (!with)
        with = "";
    len_with = strlen(with);

    /* count the number of replacements needed */
    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep;
    }

    strcpy(tmp, orig);

    return result;
}