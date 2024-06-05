#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

struct Attr {
    char name[10];
    char attr[10][50];
    int attr_content_num;
};

struct Information {
    char tag[10];
    char text[100];
    int attr_num;
    struct Attr attribute[10];
    struct Information * parent;
};

void print_help(int depth);

int main(int argc, char * argv[]) {
    if(argc < 2) {
        puts("Error: not enough arguments");
        print_help(0);
        return 1;
    }

    int i = 0;
    int j = 0;
    int k = 0;
    
    bool intags = false;
    bool is_esc = false;

    char title_class[] = "title";
    char toc_class[] = "table-of-contents";

    int parsed_lindex = 0;
    int parsed_cindex = 0;

    int info_counter = 0;
    int attr_counter = 0;

    int unclosed_tag = 0;

    //char html_data[5000] = {'\0'};
    char html_data[100][50] = {{'\0'}};
    
    char parsed[100][20] = {{'\0'}};

    struct Information info[500] = {{
        {"\0"},
        {"\0"},
        0,
        {{ {"\0"}, {{"\0"}}, 0,}},
        (struct Information *)NULL,
    }};

    FILE * fp;
    fp = fopen(argv[1], "r+");

    if(fp == NULL) {
        puts("Error: failed to open the specified file");
        print_help(0);
        return 1;
    }

    //fread(html_data, sizeof(html_data), 1, fp);
    for(i = 0; fgets(html_data[i], sizeof(html_data[i]), fp) != NULL; i++) {
        printf("%s", html_data[i]);
    }
    printf("\n");

    // display data read from the file
    //for(i = 0; i < sizeof(html_data); i++) {
    //    printf("%c", html_data[i]);
    //}
    //printf("\n");
    
    // "i" represents the number of the line read from the file 
    while(k < i) {
        for(j = 0; j < sizeof(html_data[i]); j++) {
            if(html_data[k][j] == '\n') {
                j++;
                continue;
            }
            parsed[parsed_lindex][parsed_cindex] = html_data[k][j];

            parsed_cindex++;

            if(html_data[k][j] == '<' ||
                    html_data[k][j] == '>' ||
                    html_data[k][j] == '=' ||
                    html_data[k][j] == ' ' ||
                    html_data[k][j] == '&' ||
                    html_data[k][j] == '!' ||
                    html_data[k][j] == '\"' ||
                    html_data[k][j + 1] == '\"' ||
                    html_data[k][j + 1] == '=' ||
                    html_data[k][j + 1] == '!' ||
                    html_data[k][j + 1] == '>' ||
                    html_data[k][j + 1] == '&' ||
                    html_data[k][j + 1] == '<' ) {
                parsed_lindex++;
                parsed_cindex = 0;
            }
        }
        k++;
    }



    puts("DEBUG: parsed_line");
    for(int line = 0; line < parsed_lindex; line++ )
            printf("line: %d --- \"%s\"\n", line, parsed[line]);

    for(i = 0; i < parsed_lindex; i++) {
        if(!strcmp(parsed[i], "\\") && !intags) {
            is_esc = true;
        }

        if(!strcmp(parsed[i], "<")) {
            strcpy(info[info_counter].tag, parsed[i + 1]);
            intags = true;
            i++;
        }

        if(!strcmp(parsed[i], "=") && intags) {
            strcpy(info[info_counter].attribute[attr_counter].name, parsed[i - 1]);
            for(j = 0; strcmp(parsed[i + 2 + j], "\"") != 0; j++) {
                strcpy(info[info_counter].attribute[attr_counter].attr[j], parsed[i + 2 + j]);
                info[info_counter].attribute[attr_counter].attr_content_num = j + 1;
            }
            attr_counter++;
        }

        if(!strcmp(parsed[i], ">") && intags) {
            info[info_counter].attr_num = attr_counter;

            info_counter++;
            attr_counter = 0;

            intags = false;
        }
    }

    puts("DEBUG: processed information");
    for(i = 0; i < info_counter; i++) {
        printf("pointer: %p\n", (void *) &info[i]);
        printf("tag      %s\n", info[i].tag);
        for(j = 0; j < info[i].attr_num; j++) {
            printf("%s: ", info[i].attribute[j].name);
            for(k = 0; k < info[i].attribute[j].attr_content_num; k++)
                printf("%s ", info[i].attribute[j].attr[k]);
            printf("\n");
        }
        printf("text:    %s\n", info[i].text);
        printf("parent:  %p\n\n", info[i].parent);

    }
    fclose(fp);
}


void print_help(int depth) {
    if(depth >= 0) {
        puts("Author: D.A.");
        puts("Description: a program that generate table of contents in html file");
        puts("autoc [INPUT FILE]");
        puts("-h, --help: display this help");
    }
}
