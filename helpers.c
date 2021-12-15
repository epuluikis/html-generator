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
char *str_replace(char *orig, char *rep, char *with) {
    char *result;
    char *ins;
    char *tmp;
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
void console_text_color(char color) {
    switch (color) {
        case 'r':
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (FOREGROUND_RED));
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
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                    (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN));
    }
}

/**
 * \brief           Waits for user input, validates, asks again validation if failed
 * \param[in]       min: Input number min value
 * \param[in]       max: Input number max value
 * \param[out]      number: Validated int input
 */
int get_number(int min, int max) {
    int number;

    while (1) {
        printf("Please input integer number [%d, %d]: ", min, max);

        if (scanf("%d", &number) != 1 || getchar() != '\n') {
            printf("Input does contain not number characters. ");

            char c = getchar();

            while (c != '\n') {
                c = getchar();
            };

            continue;
        }

        if (number < min || number > max) {
            printf("Input value doesn't fit in range. ");

            continue;
        }

        return (int) number;
    }
}

/**
 * \brief           Replaces "<",">" symbols in string with their html entity number
 * \param[in]       input: Original string 
 * \param[out]      input: Adjusted string
 */
char *sanitize_input(char *input) {
    char less_sign[] = "<";
    char greater_sign[] = ">";
    char less_sign_html[] = "&#60";
    char greater_sign_html[] = "&#62";

    input = str_replace(input, less_sign, less_sign_html);
    input = str_replace(input, greater_sign, greater_sign_html);

    return input;
}

/**
 * \brief           Clear terminal
 * \note            This function doesn't have return value
 */
void clear_terminal() {
//    system("cls");
}

/**
 * \brief           Read and sanitize unlimited size string from user input
 * \param[out]      input: Pointer to read string
 */
char *read_sanitized_string() {
    char *input;

    input = read_string();
    input = sanitize_input(input);

    return input;
}

/**
 * \brief           Returns pointer to current year in string
 * \note            You should free allocated memory after calling this function
 * \param[out]      year: Pointer to string
 */
char *get_current_year() {
    time_t seconds = time(NULL);
    struct tm *current_time = localtime(&seconds);
    char *yearString = (char *) malloc(sizeof(char) * 4);

    sprintf(yearString, "%d", current_time->tm_year+1900);

    return yearString;

}

/**
 * \brief           Read unlimited size string from user input
 * \param[out]      big: Pointer to read string
 */
char *read_string() {
    char *string;
    size_t counter = 0;
    size_t allocated = 16;
    int c;

    string = malloc(allocated);

    do {
        c = getchar();
        if (c == EOF) {
            break;
        }
        if (counter + 2 <= allocated) {
            size_t new_size = allocated * 2;
            char *new_buffer = realloc(string, new_size);
            if (!new_buffer) {
                new_size = allocated + 16;
                new_buffer = realloc(string, new_size);
                if (!new_buffer) {
                    free(string);
                    console_text_color('r');
                    printf("\nSorry, but we are out of memory\n");
                    console_text_color('w');
                    exit(0);
                }
            }
            allocated = new_size;
            string = new_buffer;
        }
        string[counter++] = c;
    } while (c != '\n');

    string[counter - 1] = '\0';
    return string;
}

/**
 * \brief           Write string to file
 * \param[in]       string: Pointer to pointer to string
 * \param[in]       file: Pointer to file
 */
void write_to_file(char **string, FILE *file) {
    fprintf(file, "%s", *string);
    free(*string);
    *string = NULL;
}
