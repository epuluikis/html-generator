#include "headers/template.h"
#include "headers/contents.h"
#include "headers/helpers.h"

/**
 * \brief           Inputs user input to selected section
 */
void write_input_to_section(t_contents *contents, int section_number)
{
    for (int j = 0; contents->section_ids[section_number][j] != -1; ++j)
    {
        char *input = contents->user_input[section_number][contents->section_ids[section_number][j]];
        
        strcpy(section_pointers[section_number], str_replace(section_pointers[section_number],
                                                             contents->hashes_to_change[contents->section_ids[section_number][j]],
                                                             input));
    }
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
void user_interface(t_contents *contents, FILE* output_file) {
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

    strcpy(contents->user_input[5][contents->section_ids[5][0]], name);
    strcpy(contents->user_input[6][contents->section_ids[6][0]], name);
    strcpy(contents->user_input[7][contents->section_ids[7][0]], name);

    printf("Please enter website description: ");
    site_description = read_string();

    strcpy(contents->user_input[5][contents->section_ids[5][1]], site_description);

    write_input_to_section(contents, 5);
    write_input_to_section(contents, 6);
    write_input_to_section(contents, 7);

    write_to_HTML(section_pointers[5], output_file);
    write_to_HTML(section_pointers[6], output_file);

    free(site_description);

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

            input = read_string();

            /**
             * TODO: allocate more input memory, because overflow can happen if 1 char is replaced with 4
             */

            input = sanitize_input(input); //

            strcpy(contents->user_input[choice][contents->section_ids[choice][j]], input);

            free(input);
        }
		
	    write_input_to_section(contents, choice);
        write_to_HTML(section_pointers[choice], output_file);

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

    write_to_HTML(section_pointers[7], output_file);

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

    output_file = fopen("../output/index.html", "w");
	
    user_interface(&contents, output_file);
	
    fclose(output_file);

    return 0;
}
