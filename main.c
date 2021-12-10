#include "headers/template.h"
#include "headers/contents.h"
#include "headers/helpers.h"
#include <stdio.h>
#include <string.h>

void writeHTML()
{
    FILE* output_file;

    output_file = fopen("./output/index.html", "w");

    fprintf(output_file, "%s", test_template);

    fclose(output_file);
}

int main()
{
    t_contents contents;

    printf("%d", init_contents(&contents));

    for (int i = 0; i < SECTION_COUNT; ++i) {
        printf("%s: \n", contents.section_titles[i]);
        int j = 0;
        for (int j = 0; contents.section_ids[i][j] >= 0; j++) {
            char user_input[256];

            printf("Enter a %s: ", contents.interface_text[contents.section_ids[i][j]]);
            scanf("%s", user_input);

            strcpy(test_template, strcpy(test_template, str_replace(test_template, contents.hashes_to_change[contents.section_ids[i][j]], user_input)));
        }

        printf("\n");
    }

    writeHTML();

    return 0;
}
