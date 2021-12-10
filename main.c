#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <fileapi.h>
#include "resources/template.h"

#define STR_MAX_LENGTH 256
// amount of variables
#define FIELD_COUNT 14
#define SECTION_COUNT 5
#define SECTION_MAX_SIZE 7

struct Contents {
	
	char titles_tochange[FIELD_COUNT][STR_MAX_LENGTH];		// text to replace in the html template
	char interface_text[FIELD_COUNT][STR_MAX_LENGTH];		// text to show in the interface
	char section_titles[SECTION_COUNT][STR_MAX_LENGTH];		// names of different sections
	int section_ids[SECTION_COUNT][SECTION_MAX_SIZE];		// store which section replaces which titles
	char user_input[FIELD_COUNT][STR_MAX_LENGTH];			// currently now used as we are not storing user input but writing it into HTML right away
	
}; 

void InitStructure(struct Contents *contents)
{
	const char title_names[FIELD_COUNT][STR_MAX_LENGTH] =
	{
		"{{user_name}}",
		"{{description}}",
		"{{title}}",
		"{{text}}",
		"{{subtitle}}",
		"{{phone_title}}",
		"{{phone}}",
		"{{email_title}}",
		"{{email}}",
		"{{address_title}}",
		"{{address}}",
		"{{image}}",
		"{{alt}}",
		"{{current_year}}"
	};
	
	const char interface_text_names[FIELD_COUNT][STR_MAX_LENGTH] =
	{
		"User Name",		// 0
		"Description",		// 1
		"Title",			// 2
		"Text",				// 3
		"Subtitle",			// 4
		"Phone Title",		// 5
		"Phone Number",		// 6
		"Email Title",		// 7
		"Email",			// 8
		"Address Title",	// 9
		"Address",			// 10
		"Image",			// 11
		"Alt",				// 12
		"Current Year"		// 13
	};
	
	const char section_title_names[SECTION_COUNT][STR_MAX_LENGTH] =
	{
		"General Section",	// 0
		"Text + Image",		// 1
		"Hero Section",		// 2
		"Contacts Section",	// 3
		"Carousel Section"	// 4
	};
	
	// -1 indicates stop reading ids
	const int section_ids[SECTION_COUNT][SECTION_MAX_SIZE + 1] =
	{
		{ 0, 1, 2, -1 },	// General Section	
		{ 2, 3, -1 },		// Text + Image
		{ 2, 4, -1 },		// Hero Section	
		{ 2, 5, 6, 7, 8, 9, 10, -1 },	// Contacts Section	
		{ 0, 11, 12, -1 },	// Carousel Section	
	};
	
	// initialize titles_tochange and interface_text
	for (int i = 0; i < FIELD_COUNT; ++i)
	{
		strcpy( contents->titles_tochange[i], title_names[i] );
		strcpy( contents->interface_text[i], interface_text_names[i] );
	}
	
	// initialize section_titles and section_ids
	for (int i = 0; i < SECTION_COUNT; ++i)
	{
		strcpy( contents->section_titles[i], section_title_names[i] );
		
		for (int j = 0; j < SECTION_MAX_SIZE; ++j)
		{
			contents->section_ids[i][j] = section_ids[i][j];
		}
	}
	

	
	//strcpy( contents->titles_tochange[0], "{{user_name}}" );
}

char *str_replace(char *orig, char *rep, char *with)
{
    char *result;  // the return string
    char *ins;     // the next insert point
    char *tmp;     // varies
    int len_rep;   // length of rep (the string to remove)
    int len_with;  // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;     // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count)
    {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--)
    {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);

    return result;
}

// Create directories using Windows API
int create_output_directories()
{
    int output = CreateDirectoryA("./output", NULL);
    int output_img = CreateDirectoryA("./output/img", NULL);

    if (!output || !output_img)
    {
        return 0;
    }

    return 1;
}

// Copy HTML dependencies using Windows API
int copy_dependencies()
{
    int main_css = CopyFile("./resources/template/app.css", "./output/app.css", 0);
    int landing_image = CopyFile("./resources/template/img/decoration.png", "./output/img/decoration.png", 0);

    if (!main_css || !landing_image)
    {
        return 0;
    }

    return 1;
}

void writeHTML()
{
	FILE *output_file;

    create_output_directories();

    output_file = fopen("./output/index.html", "w");

    fprintf(output_file, "%s", test_template);

    fclose(output_file);

    copy_dependencies();
}

void test_str_replace()
{
    char name[] = "{{user_name}}", with[] = "Test User";

    strcpy(test_template, str_replace(test_template, name, with));

    writeHTML();
}

int main()
{
    //test_str_replace();
    
    struct Contents contents;
    
    InitStructure(&contents);
	
	for (int i = 0; i < SECTION_COUNT; ++i)
	{
		printf("%s: \n", contents.section_titles[i]);
		int j = 0;
		while ( contents.section_ids[i][j] >= 0 )
		{
			char user_input[256];
			
			printf("Enter a %s: ", contents.interface_text[ contents.section_ids[i][j] ]);
			scanf("%s", user_input);
			
			strcpy( test_template, strcpy( test_template, str_replace(test_template, contents.titles_tochange[ contents.section_ids[i][j] ], user_input) ) );
			++j;
		}

		printf("\n");
	}
	
	writeHTML();

    return 0;
}
