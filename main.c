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

//This should be all in main and not a function.
void user_interface(t_contents *contents){
 system("clear");
 console_text_color('b');
 printf("===================================================\n"
 "This program create personalised portfolio website.\n"
 "===================================================\n"
 "Please enter your full name: ");

 char name[STR_MAX_LENGTH];
 
 while(1)
 {
    fgets(name, STR_MAX_LENGTH, stdin); 
    if(strlen(name)+1==STR_MAX_LENGTH)
    {
    fflush(stdin);
    console_text_color('r');
    printf("Full name is too long, allowed full name lenght %d \n"
    "Please enter your full name: ", STR_MAX_LENGTH-3);
    console_text_color('b');
    }
    else
    {
        break;
    }
 }

 while(1)
 {
    system("clear");
    printf("Choose what section to add next: \n");
    int i = 0;
    for(i; i<5; i++) // TODO Remove header option . Ask it before letting user choose.
    {    
        printf("[%d] to add %s \n", i, contents->section_titles[i]);
    }
    int choice;
    choice=get_number(i-1);
    system("clear");

    for (int j=0 ; contents->section_ids[choice][j]!=-1; ++j)
    {
        printf("Please enter %s \n", contents->interface_text[contents->section_ids[choice][j]]);

        char *input;
        input=calloc(sizeof(char), STR_MAX_LENGTH);
        if (input==NULL)
        {
            printf("Failed to allocate memory");
            exit(0);
        }

        fgets(input, STR_MAX_LENGTH, stdin);   //TODO will implement larger (w validation) input once i'll know where/how to send input
        fflush(stdin);
        printf("%s", input);
        input = validation(input); //TODO improve, bad since overflow can happen 1 char is replaced with 4
        //TODO sends input 
        free(input);
        system("clear");
    }

    printf("%s added. To continue enter: \n"
    "[0] to quit \n"
    "[1] to add another section \n", contents->section_titles[choice]);

    if(get_number(1))
    {
        continue;
    }

    console_text_color('w');
    break;
    }
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

    user_interface(&contents);
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
