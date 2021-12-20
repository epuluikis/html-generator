/**
 * \file            main.c
 * \brief           Generate HTML website
 */

#include "headers/template.h"
#include "headers/contents.h"
#include "headers/helpers.h"
#include "headers/list.h"
#include "headers/defines.h"

char *get_next_page_url(data_t *previous_page) {
    if (previous_page == NULL) {
        return "index.html";
    }

    return previous_page->url;
}

void generate_html(t_contents *contents, data_t **page) {
    FILE *output_file;
    data_t *previous_ptr = NULL;
    data_t *ptr = NULL;
    ptr = *page;
    int page_count = 0;

    while (ptr != NULL) {
        int i = 0;

        char filename[] = "output/";
        strcat(filename, ptr->url);
        output_file = fopen(filename, "w");

        if (output_file == NULL) {
            printf("Failed to create page number %d, bad file name.\n", page_count + 1);
            ptr = ptr->next;
            ++page_count;
            continue;
        }

        /* Write header */
        write_to_file(section_pointers[0], output_file);

        /* Write url section */
        char *url_sect = strdup(section_pointers[3]);
        strcpy(url_sect, str_replace(url_sect, "{{url}}", get_next_page_url(previous_ptr)));
        strcpy(url_sect, str_replace(url_sect, "{{title}}", get_next_page_url(previous_ptr)));
        write_to_file(url_sect, output_file);

        if (ptr->next == NULL) {
            i = 3;
            /* Write landing section if its the first page */
            write_to_file(section_pointers[2], output_file);
        }

        for (; i < ptr->section_count; ++i) {
            char *str_new_section = (char *) malloc(strlen(section_pointers[ptr->section_ptr[i].section_index]) + 1);
            strcpy(str_new_section, section_pointers[ptr->section_ptr[i].section_index]);

            for (int j = 0; j < ptr->section_ptr[i].input_count; ++j) {
                strcpy(str_new_section, str_replace(str_new_section,
                                                    contents->hashes_to_change[contents->section_ids[ptr->section_ptr[i].section_index][j]],
                                                    ptr->section_ptr[i].input[j]));
            }

            write_to_file(str_new_section, output_file);

            free(str_new_section);
        }

        previous_ptr = ptr;
        ptr = ptr->next;
        /* Write footer */
        write_to_file(section_pointers[1], output_file);
        fclose(output_file);
    }
}

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

void save_section_field(data_t *page, int section, int page_number, char *input) {
    int section_number = page->section_count;

    page->section_ptr[section_number].section_index = section;
    page->section_ptr[section_number].input[page->section_ptr[section_number].input_count] = input;
    page->section_ptr[page->section_count].input_count++;
}

void handle_section_fields(data_t *page, t_contents *contents, int section, int page_number) {
    for (int i = 0; contents->section_ids[section][i] != -1; ++i) {
        char *input = NULL;

        print_section_field(contents, section, i);
        input = read_sanitized_string();

        save_section_field(page, section, page_number, input);
    }
    page->section_count++;
    printf("\n");
}

void user_interface(t_contents *contents, data_t **page) {
    int page_number = 0, repeat_selection = 1, section_selection;
    char *page_name;

    clear_terminal();

    do {
        if (repeat_selection == 1) {

            if (page_number != 0) {
                clear_terminal();

                printf("What is your page name: ");
                /* TODO: validate the page_name */
                page_name = read_string();
                strcat(page_name, ".html");
            }

            add_node(page);

            /* Should happen only once */
            /* Gets mandatory information for whole website */
            if (page_number == 0) {
                handle_section_fields(*page, contents, 0, page_number);

                /* For the first page URL should be index */
                page_name = "index.html";

                /* Take info from header and write to footer */
                (*page)->section_ptr[1].section_index = 1;
                (*page)->section_ptr[1].input[0] = (*page)->section_ptr[0].input[0];
                (*page)->section_ptr[1].input[1] = get_current_year();
                (*page)->section_ptr[1].input_count = 2;

                /* Take info from header and write to landing */
                (*page)->section_ptr[2].section_index = 2;
                (*page)->section_ptr[2].input[0] = (*page)->section_ptr[0].input[0];
                (*page)->section_ptr[2].input_count = 1;

                /* First page has 3 sections by default - header, footer, landing */
                (*page)->section_count = 3;

                for (int i = 0; i < IGNORE_SECTIONS - 1; ++i) {
                    for (int j = 0; j < (*page)->section_ptr[i].input_count; ++j) {
                        strcpy(section_pointers[i],
                               str_replace(section_pointers[i],
                                           contents->hashes_to_change[contents->section_ids[(*page)->section_ptr[i].section_index][j]],
                                           (*page)->section_ptr[i].input[j]));
                    }
                }

            }

            strcpy((*page)->url, page_name);

            page_number++;
        }

        clear_terminal();

        /* Get section & store its values */
        print_sections(contents);
        section_selection = get_number(1, SECTION_COUNT - IGNORE_SECTIONS) + IGNORE_SECTIONS - 1;
        print_selected_section(contents, section_selection);
        handle_section_fields(*page, contents, section_selection, page_number);

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

    return 0;
}
