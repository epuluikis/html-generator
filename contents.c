/**
 * \file            contents.c
 * \brief           Initialize contents structure & dependencies
 */

#include "headers/contents.h"
#include "headers/helpers.h"

/**
 * \brief           Initialize contents structure
 * \note            This function does not return value, it stores it to pointer instead
 * \param[in]       contents: Structure to initialize
 */
void init_structure(t_contents *contents) {
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
            "{{color_theme}}",
            "{{name}}",
            "{{position}}",
            "{{subtitle}}"
    };

    const char interface_text_names[FIELD_COUNT][STR_MAX_LENGTH] = {
            "Name",
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
            "Image Link",
            "Image Description",
            "Current Year",
            "Color Theme",
            "Name",
            "Work Position",
            "Subtitle"
    };

    const char section_title_names[SECTION_COUNT][STR_MAX_LENGTH] = {
            "Header",
            "Footer",
            "Landing section",
            "URL section",
            "Text + Image Section",
            "Hero Section",
            "Contacts Section",
            "Carousel Section",
            "Testimonial Section",
            "Quote Section",
            "Paragraph Section",
            "Highlighted Paragraph Section",
            "Title & Subtitle Section",
            "Image Banner Section"
    };

    /* -1 indicates end */
    const int section_ids[SECTION_COUNT][SECTION_MAX_SIZE] = {
            {0,  1,  2,  14, -1},             /* Header */
            {0,  13, -1},                     /* Footer */
            {0,  -1},                          /* Landing section */
            {2,  15, -1},                     /* URL section */
            {2,  3,  11, 12, -1},             /* Text + Image */
            {2,  4,  -1},                     /* Hero Section */
            {2,  5,  6,  7,  8, 9, 10, -1},   /* Contacts Section */
            {11, 12, -1},                     /* Carousel Section */
            {3,  15, 16, 11, -1},             /* Testimonial Section */
            {3,  -1},                         /* Quote Section */
            {3,  -1},                         /* Paragraph Section */
            {3,  -1},                         /* Highlighted Paragraph Section */
            {3,  17, -1},                     /* Title & Subtitle Section */
            {11, 12, -1},                     /* Image Banner Section */
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
}

/**
 * \brief           Copy HTML dependencies using Windows API
 * \param[out]      1 on success, 0 otherwise
 */
int copy_dependencies() {
    if (!CopyFile("resources/template/app.css", "output/app.css", 0) ||
        !CopyFile("resources/template/app.js", "output/app.js", 0) ||
        !CopyFile("resources/template/img/decoration.png", "output/img/decoration.png", 0)) {
        return 0;
    }

    return 1;
}

/**
 * \brief           Create directories using Windows API
 * \param[out]      1 on success, 0 otherwise
 */
int create_output_directories() {
    if (!CreateDirectoryA("output", NULL)) {
        if (GetLastError() != ERROR_ALREADY_EXISTS) {
            return 0;
        }
    }

    if (!CreateDirectoryA("output/img", NULL)) {
        if (GetLastError() != ERROR_ALREADY_EXISTS) {
            return 0;
        }
    }

    return 1;
}

/**
 * \brief            Initialize contents structure & dependencies
 * \param[in]        contents: Structure to initialize
 */
void init_contents(t_contents *contents) {
    init_structure(contents);

    if (!create_output_directories() || !copy_dependencies()) {
        print_error("Sorry, but we couldn't initialize project. Please check permissions.");

        exit(0);
    }
}
