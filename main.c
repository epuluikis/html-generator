#include "headers/template.h"
#include "headers/contents.h"
#include "headers/helpers.h"
#include <stdio.h>
#include <string.h>

FILE *output_file;

void write_to_HTML(char *section) {
    fprintf(output_file, "%s", section);
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

//This should be all in main and not a function.
void user_interface(t_contents *contents) {
    char *name, *site_title, *site_description, *input;
    int choice;

    clear_terminal();
    console_text_color('b');
    printf("================================================\n"
           "Create beautiful personalised portfolio website.\n"
           "================================================\n\n");
    console_text_color('w');

    printf("Please enter your full name: ");
    name = read_string();

    printf("Please enter website title: ");
    site_title = read_string();

    printf("Please enter website description: ");
    site_description = read_string();

    /**
     *  TODO: send info to add landing section which is required
     *  Don't free name until footer is added. It will be used again.
     */

    free(site_title);
    free(site_description);

    while (1) {
        clear_terminal();
        printf("Choose which section to add: \n");

        for (int i = 1; i < SECTION_COUNT; i++) {
            console_text_color('g');
            printf("[%d]", i);
            console_text_color('w');
            printf(" to add %s \n", contents->section_titles[i]);
        }

        printf("\n\n");

        choice = get_number(1, SECTION_COUNT - 1);

        clear_terminal();

        console_text_color('b');
        printf("> %s selected!\n\n", contents->section_titles[choice]);
        console_text_color('w');

        for (int j = 0; contents->section_ids[choice][j] != -1; ++j) {
            printf("Please enter preferred %s: ", contents->interface_text[contents->section_ids[choice][j]]);

            input = read_string();

            /**
             * TODO: allocate more input memory, because overflow can happen if 1 char is replaced with 4
             */

            input = sanitize_input(input); //

            /*
             * TODO: store input into variables for later use
             */

            free(input);
        }

        /**
         * TODO: send inputs and section index to corresponding function
         */

        console_text_color('b');
        printf("\n> %s added!\n\n", contents->section_titles[choice]);
        console_text_color('w');

        printf("Choose what to do next:\n");
        console_text_color('g');
        printf("[1]");
        console_text_color('w');
        printf(" to add another section\n");
        console_text_color('g');
        printf("[2]");
        console_text_color('w');
        printf(" to finish generating website\n\n");

        if (get_number(1, 2) == 2) {
            break;
        }

        console_text_color('w');
    }

    /**
     * TODO: send info to add footer
     * Year variable is required. We need to get it somehow to string.
     */

    clear_terminal();
    console_text_color('b');
    printf("Your generated website should be in output folder.\n"
            "Thank you for using this tool!\n");
    console_text_color('w');

    free(name);
}

void create_section(t_contents *contents, int section_number) {
    printf("\nworking with %s: \n", contents->section_titles[section_number]);
    for (int j = 0; contents->section_ids[section_number][j] >= 0; ++j) {
        char user_input[256];
        // basic interface
        printf("Enter a %s: ", contents->interface_text[contents->section_ids[section_number][j]]);
        scanf("%s", user_input);

        strcpy(section_pointers[section_number], str_replace(section_pointers[section_number],
                                                             contents->hashes_to_change[contents->section_ids[section_number][j]],
                                                             user_input));
    }
    write_to_HTML(section_pointers[section_number]);
}

int main() {

    t_contents contents;

    init_contents(&contents);

    user_interface(&contents);

    return 0;

    printf("The rest is not part of interface \n");

    output_file = fopen("index.html", "w");

    char user_name[256], description[256];

    // basic interface, need to change
    printf("user_name = ");
    scanf("%s", user_name);

    printf("description = ");
    scanf("%s", description);

    strcpy(header_section, str_replace(header_section, "{{user_name}}", user_name));
    strcpy(header_section, str_replace(header_section, "{{description}}", description));
    strcpy(footer_section, str_replace(footer_section, "{{user_name}}", user_name));

    write_to_HTML(header_section);

    printf("\ninput a section number or -1 if you are done: ");
    int input = -1;
    scanf("%d", &input);
    while (input >= 0 && input <= 4) {
        create_section(&contents, input);
        // basic interface
        printf("\ninput a section number or -1 if you are done: ");
        scanf("%d", &input);
    }

    write_to_HTML(footer_section);

    fclose(output_file);

    return 0;
}
