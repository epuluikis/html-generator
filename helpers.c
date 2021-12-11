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

/**
 * \brief           Change console color
 * \param[in]       color: Chosen color
 */
void console_text_color(char color)
{
    switch(color)
    {
    case 'r':
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (FOREGROUND_RED ));
        break;
    case 'y':
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (FOREGROUND_GREEN | FOREGROUND_RED));
        break;
    case 'g':
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (FOREGROUND_GREEN));
        break;
    case 'b':
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (FOREGROUND_BLUE | FOREGROUND_GREEN));
        break;
    case 'f':
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (0));
        break;
    default:
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN));
    }
}

/**
 * \brief           Waits for user input, validates, asks again validation if failed
 * \param[in]       limit: Input number max value
 * \param[out]      number: Validated int input
 */
int get_number(int limit)
{
    int number;

    while(1)
    {
        ask_again:
        if(scanf("%d", &number) != 1 || getchar() != '\n')
        {
            console_text_color('r');
            printf("Please enter number between 0 and %d:", limit);
            console_text_color('b');

            char c = getchar();

            while (c != '\n')
            {
                c = getchar();
            };

            continue;
        }
        if(number>limit||number<0)
        {
            console_text_color('r');
            printf("Input validation error. Please enter number between 0 and %d:", limit);
            console_text_color('b');

            goto ask_again;
        }

        return number;
    }
}

/**
 * \brief           Replaces "<",">" symbols in string with their html entity number
 * \param[in]       input: Original string 
 * \param[out]      input: Adjusted string
 */
char *validation(char* input){
    //TODO move to contents.c ?
    char *less_sign;
    char *greater_sign;
    char *less_sign_html;
    char *greater_sign_html;

    less_sign="<";
    greater_sign=">";
    less_sign_html="&#60";
    greater_sign_html="&#62";

    input = str_replace(input, less_sign, less_sign_html);
    input = str_replace(input, greater_sign, greater_sign_html);

    return input;
 }