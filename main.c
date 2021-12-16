/**
 * \file            main.c
 * \brief           Generate HTML website
 */

#include "headers/template.h"
#include "headers/contents.h"
#include "headers/helpers.h"
#include "headers/list.h"

///**
// * \brief           Inputs user input to selected section
// */
//void write_input_to_section(t_contents *contents, int section_number) {
//    for (int j = 0; contents->section_ids[section_number][j] != -1; ++j) {
//
//        if (section_pointers_input[section_number] == NULL) {
//            section_pointers_input[section_number] = strdup(str_replace(section_pointers[section_number],
//                                                                        contents->hashes_to_change[contents->section_ids[section_number][j]],
//                                                                        contents->user_input[section_number][contents->section_ids[section_number][j]]));
//        } else {
//            section_pointers_input[section_number] = str_replace(section_pointers_input[section_number],
//                                                                 contents->hashes_to_change[contents->section_ids[section_number][j]],
//                                                                 contents->user_input[section_number][contents->section_ids[section_number][j]]);
//        }
//    }
//}
//
///**
// * \brief           Initial Questions
// * \note            Generate html wouldn't work without initial questions.
// * \param[in]       contents: pointer to t_contents populated structure
// * \param[in]       output_file: pointer to output file
// * \param[out]      name: pointer to name
// */
//char *initial_questions(t_contents *contents, FILE *output_file) {
//    char *site_description, *name, *color;
//
//    printf("Please enter your full name: ");
//    name = read_sanitized_string();
//
//    printf("Please enter website description: ");
//    site_description = read_sanitized_string();
//
//    printf("Please choose preferred color scheme:\n");
//
//    for (int i = 0; i < COLOR_COUNT; i++) {
//        console_text_color('g');
//        printf("[%d] ", i + 1);
//        console_text_color('w');
//        printf("%c%s\n", toupper(colors[i][0]), colors[i] + 1);
//    }
//
//    color = colors[get_number(1, COLOR_COUNT) - 1];
//
//    for (int i = 5; i < 8; i++) {
//        strcpy(contents->user_input[i][contents->section_ids[i][0]], name);
//    }
//
//    strcpy(contents->user_input[5][contents->section_ids[5][1]], site_description);
//    strcpy(contents->user_input[5][contents->section_ids[5][2]], color);
//    strcpy(contents->user_input[7][contents->section_ids[7][1]], get_current_year());
//
//    for (int i = 5; i < 8; i++) {
//        write_input_to_section(contents, i);
//
//        if (i != 7) {
//            write_to_file(&section_pointers_input[i], output_file);
//        }
//    }
//
//    free(site_description);
//
//    return name;
//}
//
///**
// * \brief           User Interface
// * \param[in]       contents: pointer to t_contents populated structure
// * \param[in]       output_file: pointer to output file
// */
//void user_interface(t_contents *contents, FILE *output_file) {
//    char *name, *input;
//    int choice;
//
//    clear_terminal();
//    console_text_color('b');
//    printf("================================================\n"
//           "Create beautiful personalised portfolio website.\n"
//           "================================================\n\n");
//    console_text_color('w');
//
//    name = initial_questions(contents, output_file);
//
//    while (1) {
//        clear_terminal();
//        printf("Choose which section to add: \n");
//        for (int i = 1; i < SECTION_COUNT - 3; i++) {
//            console_text_color('g');
//            printf("[%d]", i);
//            console_text_color('w');
//            printf(" to add %s \n", contents->section_titles[i]);
//        }
//
//        printf("\n\n");
//
//        choice = get_number(1, SECTION_COUNT - 4);
//
//        clear_terminal();
//
//        console_text_color('b');
//        printf("> %s selected!\n\n", contents->section_titles[choice]);
//        console_text_color('w');
//
//        for (int j = 0; contents->section_ids[choice][j] != -1; ++j) {
//            printf("Please enter preferred %s: ", contents->interface_text[contents->section_ids[choice][j]]);
//
//            input = read_sanitized_string();
//
//            strcpy(contents->user_input[choice][contents->section_ids[choice][j]], input);
//
//            free(input);
//        }
//
//        write_input_to_section(contents, choice);
//        write_to_file(&section_pointers_input[choice], output_file);
//        printf("write to full should be null %s", section_pointers_input[choice]);
//
//        console_text_color('b');
//        printf("\n> %s added!\n\n", contents->section_titles[choice]);
//        console_text_color('w');
//
//        printf("Choose what to do next:\n");
//        console_text_color('g');
//        printf("[1]");
//        console_text_color('w');
//        printf(" to add another section\n");
//        console_text_color('g');
//        printf("[2]");
//        console_text_color('w');
//        printf(" to finish generating website\n\n");
//
//        if (get_number(1, 2) == 2) {
//            break;
//        }
//
//        console_text_color('w');
//    }
//
//    write_to_file(&section_pointers_input[7], output_file);
//
//    clear_terminal();
//    console_text_color('b');
//    printf("Your generated website should be in output folder.\n"
//           "Thank you for using this tool!\n");
//    console_text_color('w');
//
//    free(name);
//}

/* TODO */
void generate_html(t_contents *contents, data_t **page) {

};

void print_option(char *string, int number) {
    console_text_color('g');
    printf("[%d] ", number + 1);
    console_text_color('w');
    printf("%s\n", string);
}

void print_sections(t_contents *contents) {
    printf("Choose which section to add: \n");

    for (int i = 0; i < SECTION_COUNT - IGNORE_SECTIONS; i++) {
        print_option(contents->section_titles[i + IGNORE_SECTIONS], i);
    }
}

int print_repeat_message() {
    int size = 3;
    char options[][STR_MAX_LENGTH] = {
            "Add more sections",
            "Add more pages",
            "Finish generating website",
    };

    printf("%s\n", "Choose what to do next:");

    for (int i = 0; i < size; i++) {
        print_option(options[i], i);
    }

    return get_number(1, size) - 1;
}

void print_selected_section(t_contents *contents, int selection) {
    clear_terminal();

    console_text_color('g');
    printf("> %s selected!\n\n", contents->section_titles[selection]);
    console_text_color('w');
}

void print_section_field(t_contents *contents, int section, int field) {
    printf("Please enter preferred %s: ",
           contents->interface_text[contents->section_ids[section][field]]);
}

/* TODO */
void save_section_field(t_contents *contents, int section, int field, char *input) {

}

void handle_section_fields(t_contents *contents, int section, int page) {
    for (int i = 0; contents->section_ids[section][i] != -1; ++i) {
        print_section_field(contents, section, i);
        char *input = read_sanitized_string();
        save_section_field(contents, section, page, input);
    }

    printf("\n");
}

void user_interface(t_contents *contents, data_t **page) {
    int page_number = 0, repeat_selection = 1, section_selection;

    clear_terminal();

    do {
        if (repeat_selection == 1) {
            page_number++;

            /* Add new page */
            /* TODO: Ask for page URL and store in node */
            /* For the first page URL should be index */
            add_node(page);

            /* Should happen only once */
            /* Gets mandatory information for whole website */
            if (page_number == 1) {
                handle_section_fields(contents, 0, page_number);
            }
        }

        clear_terminal();

        /* Get section & store its values */
        print_sections(contents);
        section_selection = get_number(1, SECTION_COUNT - IGNORE_SECTIONS) + IGNORE_SECTIONS;
        print_selected_section(contents, section_selection);
        handle_section_fields(contents, section_selection, page_number);

        clear_terminal();

        repeat_selection = print_repeat_message();
    } while (repeat_selection != 2);

    clear_terminal();
}

int main() {
    data_t *page = NULL;
    t_contents contents;

    create_list(&page);

    init_contents(&contents);

    user_interface(&contents, &page);

    print_generating_message();

    generate_html(&contents, &page);

    print_generated_message();

    delete_list(&page);

    print_outro_message();

    //    FILE *output_file;

    //    output_file = fopen("output/index.html", "w");

    //    user_interface(&contents, output_file);

    //    fclose(output_file);

    return 0;
}
