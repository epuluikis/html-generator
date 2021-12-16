/**
 * \file            main.c
 * \brief           Generate HTML website
 */

#include "headers/template.h"
#include "headers/contents.h"
#include "headers/helpers.h"

/**
 * \brief           Inputs user input to selected section
 */
void write_input_to_section(t_contents *contents, int section_number) {
    for (int j = 0; contents->section_ids[section_number][j] != -1; ++j) {

        if(section_pointers_input[section_number]==NULL) {
            section_pointers_input[section_number] = strdup( str_replace(section_pointers[section_number],
                                                        contents->hashes_to_change[contents->section_ids[section_number][j]],
                                                        contents->user_input[section_number][contents->section_ids[section_number][j]]));
        }
        else {
            section_pointers_input[section_number] = str_replace(section_pointers_input[section_number],
                                                        contents->hashes_to_change[contents->section_ids[section_number][j]],
                                                        contents->user_input[section_number][contents->section_ids[section_number][j]]);
        }
    }
}

/**
 * \brief           Initial Questions
 * \note            Generate html wouldn't work without initial questions.
 * \param[in]       contents: pointer to t_contents populated structure
 * \param[in]       output_file: pointer to output file
 * \param[out]      name: pointer to name
 */
char *initial_questions(t_contents *contents, FILE *output_file) {
    char *site_description, *name, *color;

    printf("Please enter your full name: ");
    name = read_sanitized_string();

    printf("Please enter website description: ");
    site_description = read_sanitized_string();

    printf("Please choose preferred color scheme:\n");

    for (int i = 0; i < COLOR_COUNT; i++) {
        console_text_color('g');
        printf("[%d] ", i + 1);
        console_text_color('w');
        printf("%c%s\n", toupper(colors[i][0]), colors[i] + 1);
    }

    color = colors[get_number(1, COLOR_COUNT) - 1];

    for (int i = 5; i < 8; i++) {
        strcpy(contents->user_input[i][contents->section_ids[i][0]], name);
    }

    strcpy(contents->user_input[5][contents->section_ids[5][1]], site_description);
    strcpy(contents->user_input[5][contents->section_ids[5][2]], color);
    strcpy(contents->user_input[7][contents->section_ids[7][1]], get_current_year());

    for (int i = 5; i < 8; i++) {
        write_input_to_section(contents, i);

        if (i != 7) {
            write_to_file(&section_pointers_input[i], output_file);
        }
    }

    free(site_description);

    return name;
}

/**
 * \brief           User Interface
 * \param[in]       contents: pointer to t_contents populated structure
 * \param[in]       output_file: pointer to output file
 */
void user_interface(t_contents *contents, FILE *output_file) {
    char *name, *input;
    int choice;

    clear_terminal();
    console_text_color('b');
    printf("================================================\n"
           "Create beautiful personalised portfolio website.\n"
           "================================================\n\n");
    console_text_color('w');

    name = initial_questions(contents, output_file);

    while (1) {
        clear_terminal();
        printf("Choose which section to add: \n");
        for (int i = 1; i < SECTION_COUNT - 3; i++) {
            console_text_color('g');
            printf("[%d]", i);
            console_text_color('w');
            printf(" to add %s \n", contents->section_titles[i]);
        }

        printf("\n\n");

        choice = get_number(1, SECTION_COUNT - 4);

        clear_terminal();

        console_text_color('b');
        printf("> %s selected!\n\n", contents->section_titles[choice]);
        console_text_color('w');

        for (int j = 0; contents->section_ids[choice][j] != -1; ++j) {
            printf("Please enter preferred %s: ", contents->interface_text[contents->section_ids[choice][j]]);

            input = read_sanitized_string();

            strcpy(contents->user_input[choice][contents->section_ids[choice][j]], input);

            free(input);
        }

        write_input_to_section(contents, choice);
        write_to_file(&section_pointers_input[choice], output_file);
        printf("write to full should be null %s",section_pointers_input[choice]);

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

    write_to_file(&section_pointers_input[7], output_file);

    clear_terminal();
    console_text_color('b');
    printf("Your generated website should be in output folder.\n"
           "Thank you for using this tool!\n");
    console_text_color('w');

    free(name);
}

int main() {
    FILE *output_file;

    t_contents contents;

    init_contents(&contents);

    output_file = fopen("output/index.html", "w");

    user_interface(&contents, output_file);

    fclose(output_file);

    return 0;
}
