/**
 * \file            contents.c
 * \brief           Initialize contents structure & dependencies
 */

#include "headers/contents.h"

/**
 * \brief           Initialize contents structure
 * \note            This function does not return value, it stores it to pointer instead
 * \param[in]       contents: Structure to initialize
 */
void init_structure(t_contents* contents)
{
    const char title_names[FIELD_COUNT][STR_MAX_LENGTH] = {
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
        "{{current_year}}",
    };

    const char interface_text_names[FIELD_COUNT][STR_MAX_LENGTH] = {
        "User Name",
        "Description",
        "Title",
        "Text",
        "Subtitle",
        "Phone Title",
        "Phone Number",
        "Email Title",
        "Email",
        "Address Title",
        "Address",
        "Image",
        "Alt",
        "Current Year",
    };

    const char section_title_names[SECTION_COUNT][STR_MAX_LENGTH] = {
        "General Section",
        "Text + Image",
        "Hero Section",
        "Contacts Section",
        "Carousel Section",
    };

    /* -1 indicates end */
    const int section_ids[SECTION_COUNT][SECTION_MAX_SIZE + 1] = {
        { 0, 1, 2, -1 },                /* General Section */
        { 2, 3, -1 },                   /* Text + Image */
        { 2, 4, -1 },                   /* Hero Section */
        { 2, 5, 6, 7, 8, 9, 10, -1 },   /* Contacts Section */
        { 0, 11, 12, -1 },              /* Carousel Section */
    };

    /* initialize hashes_to_change and interface_text */
    for (int i = 0; i < FIELD_COUNT; ++i) {
        strcpy(contents->hashes_to_change[i], title_names[i]);
        strcpy(contents->interface_text[i], interface_text_names[i]);
    }

    /* initialize section_titles and section_ids */
    for (int i = 0; i < SECTION_COUNT; ++i) {
        strcpy(contents->section_titles[i], section_title_names[i]);

        for (int j = 0; j < SECTION_MAX_SIZE; ++j) {
            contents->section_ids[i][j] = section_ids[i][j];
        }
    }

    // strcpy( contents->titles_tochange[0], "{{user_name}}" );
}

/**
 * \brief           Copy HTML dependencies using Windows API
 * \param[out]      1 on success, 0 otherwise
 */
int copy_dependencies()
{
    if (
        !CopyFile("../resources/template/app.css", "../output/app.css", 0) ||
        !CopyFile("../resources/template/img/decoration.png", "../output/img/decoration.png", 0)
    ) {
        return 0;
    }

    return 1;
}

/**
 * \brief           Create directories using Windows API
 * \param[out]      1 on success, 0 otherwise
 */
int create_output_directories()
{
    if (!CreateDirectoryA("../output", NULL)) {
        if (GetLastError() != ERROR_ALREADY_EXISTS) {
            return 0;
        }
    }

    if (!CreateDirectoryA("../output/img", NULL)) {
        if (GetLastError() != ERROR_ALREADY_EXISTS) {
            return 0;
        }
    }

    return 1;
}

/**
 * \brief            Initialize contents structure & dependencies
 * \param[in]        contents: Structure to initialize
 * \param[out]       1 on success, 0 otherwise
 */
int init_contents(t_contents* contents)
{
    init_structure(contents);

    if (!create_output_directories() || !copy_dependencies())
    {
        return 0;
    }

    return 1;
}