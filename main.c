/**
 * \file            main.c
 * \brief           Generate HTML website
 */

#include "headers/template.h"
#include "headers/contents.h"
#include "headers/helpers.h"
#include "headers/list.h"
#include "headers/defines.h"

char *get_page_url(data_t *page) {
    return concat(page->url, ".html");
}

char *get_filename(char *url) {
    char *result = NULL;

    result = concat("output/", url);
    result = concat(result, ".html");

    return result;
}

char *get_links_content(data_t **page) {
    data_t *pages = NULL;
    char *header_links;

    pages = *page;
    header_links = "";

    while (pages != NULL && pages->next != NULL) {
        char *temp_link = strdup(header_link);

        temp_link = str_replace(temp_link, "{{url}}", get_page_url(pages));

        temp_link = str_replace(temp_link, "{{title}}", pages->url);

        header_links = concat(header_links, temp_link);

        pages = pages->next;
    }

    return str_replace(section_pointers[3], "{{links}}", header_links);
}

void generate_html(t_contents *contents, data_t **page) {
    FILE *output_file;
    data_t *ptr = NULL;
    char *header_links, *filename;

    ptr = *page;
    header_links = get_links_content(page);

    while (ptr != NULL) {
        int i = 0;

        filename = get_filename(ptr->url);

        output_file = fopen(filename, "w");

        if (output_file == NULL) {
            print_error("Sorry, but we couldn't create a new file. Please check permissions.");

            exit(0);
        }

        /* Write header */
        write_to_file(section_pointers[0], output_file);

        /* Write url section */
        write_to_file(header_links, output_file);

        if (ptr->next == NULL) {
            i = 3;

            /* Write landing section if it's the first page */
            write_to_file(section_pointers[2], output_file);
        }

        for (; i < ptr->section_count; ++i) {
            char *str_new_section = strdup(section_pointers[ptr->section_ptr[i].section_index]);

            for (int j = 0; j < ptr->section_ptr[i].input_count; ++j) {
                str_new_section = str_copy(str_replace(str_new_section,
                                                    contents->hashes_to_change[contents->section_ids[ptr->section_ptr[i].section_index][j]],
                                                    ptr->section_ptr[i].input[j]));
            }

            write_to_file(str_new_section, output_file);

            free(str_new_section);
        }

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

void save_section_field(data_t *page, int section, char *input) {
    int section_number = page->section_count;

    page->section_ptr[section_number].section_index = section;
    page->section_ptr[section_number].input[page->section_ptr[section_number].input_count] = input;
    page->section_ptr[page->section_count].input_count++;
}

char *handle_color_selection() {
    printf("%s\n", "Choose color theme: ");

    for (int i = 0; i < COLOR_COUNT; i++) {
        print_option(colors[i], i);
    }

    return colors[get_number(1, COLOR_COUNT) - 1];
}

void handle_section_fields(data_t *page, t_contents *contents, int section) {
    for (int i = 0; contents->section_ids[section][i] != -1; ++i) {
        char *input = NULL;

        if (contents->section_ids[section][i] == COLOR_SELECTION_INDEX) {
            input = handle_color_selection();
        } else {
            print_section_field(contents, section, i);
            input = read_sanitized_string();
        }

        save_section_field(page, section, input);
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

                printf("Please enter preferred page title: ");

                /* TODO: validate the page_name */
                page_name = read_string();
            }

            add_node(page);

            /* Should happen only once */
            /* Gets mandatory information for whole website */
            if (page_number == 0) {
                handle_section_fields(*page, contents, 0);

                /* For the first page URL should be index */
                page_name = "index";

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
                        section_pointers[i] = str_copy(str_replace(section_pointers[i],
                                           contents->hashes_to_change[contents->section_ids[(*page)->section_ptr[i].section_index][j]],
                                           (*page)->section_ptr[i].input[j]));
                    }
                }

            }

            (*page)->url = str_copy(page_name);

            page_number++;
        }

        clear_terminal();

        /* Get section & store its values */
        print_sections(contents);
        section_selection = get_number(1, SECTION_COUNT - IGNORE_SECTIONS) + IGNORE_SECTIONS - 1;
        print_selected_section(contents, section_selection);
        handle_section_fields(*page, contents, section_selection);

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

    free(page);

    return 0;
}
