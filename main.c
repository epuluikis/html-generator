/**
 * \file            main.c
 * \brief           Generate HTML website
 */

#include "headers/template.h"
#include "headers/contents.h"
#include "headers/helpers.h"
#include "headers/list.h"
#include "headers/defines.h"

/**
 * \brief           Get page URL
 * \param[in]       page: page data
 * \param[out]      Page URL
 */
char *get_page_url(data_t *page) {
    return concat(page->url, ".html");
}

/**
 * \brief           Get page filename
 * \param[in]       name: page name
 * \param[out]      Page filename
 */
char *get_filename(char *name) {
    return concat(concat("output/", name), ".html");
}

/**
 * \brief           Get page header links
 * \param[in]       page: page data
 * \param[out]      Page header links HTML
 */
char *get_links_content(data_t **page) {
    data_t *pages = NULL;
    char *header_links;

    pages = *page;
    header_links = "";

    while (pages != NULL && pages->next != NULL) {
        char *temp_link = strdup(header_link);

        temp_link = str_replace(
                str_replace(temp_link, "{{url}}", get_page_url(pages)),
                "{{title}}",
                pages->url
        );

        header_links = concat(header_links, temp_link);

        free(temp_link);

        pages = pages->next;
    }

    return str_replace(section_pointers[3], "{{links}}", header_links);
}

/**
 * \brief           Generate HTML and output to files
 * \param[in]       contents: generator contents
 * \param[in]       page: page data
 */
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
                str_new_section = str_copy(
                        str_replace(
                                str_new_section,
                                contents->hashes_to_change[contents->section_ids[ptr->section_ptr[i].section_index][j]],
                                ptr->section_ptr[i].input[j]
                        )
                );
            }

            write_to_file(str_new_section, output_file);

            free(str_new_section);
        }

        ptr = ptr->next;

        /* Write footer */
        write_to_file(section_pointers[1], output_file);

        fclose(output_file);
    }

    free(header_links);
    free(filename);
}

/**
 * \brief           Print option selection
 * \param[in]       string: string to print
 * \param[in]       number: number for selection
 */
void print_option(char *string, int number) {
    console_text_color('g');
    printf("[%d] ", number + 1);
    console_text_color('w');
    printf("%s\n", string);
}

/**
 * \brief           Print sections selection
 * \param[in]       contents: generator contents
 */
void print_sections(t_contents *contents) {
    printf("Choose which section to add: \n");

    for (int i = 0; i < SECTION_COUNT - IGNORE_SECTIONS; i++) {
        print_option(contents->section_titles[i + IGNORE_SECTIONS], i);
    }
}

/**
 * \brief           Print repeat message and get option value
 * \param[out]      Option value
 */
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

/**
 * \brief           Print section selected message
 * \param[in]       contents: generator contents
 * \param[in]       selection: selected section index
 */
void print_selected_section(t_contents *contents, int selection) {
    clear_terminal();

    console_text_color('g');
    printf("> %s selected!\n\n", contents->section_titles[selection]);
    console_text_color('w');
}

/**
 * \brief           Print section field
 * \param[in]       contents: generator contents
 * \param[in]       section: section index
 * \param[in]       field: section field index
 */
void print_section_field(t_contents *contents, int section, int field) {
    printf("Please enter preferred %s: ",
           contents->interface_text[contents->section_ids[section][field]]);
}

/**
 * \brief           Save section field
 * \param[in]       page: page data
 * \param[in]       section: section index
 * \param[in]       input: user input
 */
void save_section_field(data_t *page, int section, char *input) {
    page->section_ptr[page->section_count].section_index = section;
    page->section_ptr[page->section_count].input[page->section_ptr[page->section_count].input_count] = input;
    page->section_ptr[page->section_count].input_count++;
}

/**
 * \brief           Handle color scheme selection
 * \param[out]      Selected color index
 */
char *handle_color_selection() {
    printf("%s\n", "Choose color theme: ");

    for (int i = 0; i < COLOR_COUNT; i++) {
        print_option(colors[i], i);
    }

    return colors[get_number(1, COLOR_COUNT) - 1];
}

/**
 * \brief           Handle section fields
 * \param[in]       page: page data
 * \param[in]       contents: generator contents
 * \param[in]       section: section index
 */
void handle_section_fields(data_t *page, t_contents *contents, int section) {
    page->section_ptr[page->section_count].input_count = 0;

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
}

/**
 * \brief           Get page name
 * \param[out]      Page name
 */
char *get_page_name() {
    printf("Please enter preferred page title: ");

    return read_string();
}

/**
 * \brief           Handle first page sections
 * \param[in]       contents: generator contents
 * \param[in]       page: page data
 */
void handle_first_page_sections(t_contents *contents, data_t **page) {
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
            section_pointers[i] = str_copy(
                    str_replace(
                            section_pointers[i],
                            contents->hashes_to_change[contents->section_ids[(*page)->section_ptr[i].section_index][j]],
                            (*page)->section_ptr[i].input[j]
                    )
            );
        }
    }
}

/**
 * \brief           Add new page node
 * \param[in]       page: page data
 */
void add_new_page_node(data_t **page) {
    add_node(page);
    (*page)->section_count = 0;
}

/**
 * \brief           User interface for generating pages
 * \param[in]       contents: generator contents
 * \param[in]       page: page data
 */
void user_interface(t_contents *contents, data_t **page) {
    int page_number = 0, repeat_selection = 1, section_selection;
    char *page_name;

    clear_terminal();

    do {
        if (repeat_selection == 1) {
            add_new_page_node(page);

            if (page_number != 0) {
                clear_terminal();

                page_name = get_page_name();
            } else {
                /* Should happen only once */
                /* Gets mandatory information for whole website */
                handle_section_fields(*page, contents, 0);

                /* For the first page URL should be index */
                page_name = "index";

                handle_first_page_sections(contents, page);
            }

            (*page)->url = page_name;

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
