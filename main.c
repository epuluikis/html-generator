#include "headers/template.h"
#include "headers/contents.h"
#include "headers/helpers.h"
#include <stdio.h>
#include <string.h>

FILE* output_file;

void write_to_HTML(char* section)
{   
    fprintf(output_file, "%s", section);
}

void create_section(t_contents *contents, int section_number)
{
    printf("\nworking with %s: \n", contents->section_titles[section_number]);
    for (int j = 0; contents->section_ids[section_number][j] >= 0; ++j)
    {
        char user_input[256];
        // basic interface
        printf("Enter a %s: ", contents->interface_text[contents->section_ids[section_number][j]]);
        scanf("%s", user_input);

        strcpy(section_pointers[section_number], str_replace(section_pointers[section_number], contents->hashes_to_change[contents->section_ids[section_number][j]], user_input));
    }
    write_to_HTML(section_pointers[section_number]);
}

int main()
{
    t_contents contents;

    init_contents(&contents);
	
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
    while( input >= 0 && input <= 4 )
    {
        create_section(&contents, input);
        // basic interface
        printf("\ninput a section number or -1 if you are done: ");
        scanf("%d", &input);
    }

	write_to_HTML(footer_section);
	
    fclose(output_file);

    return 0;
}
