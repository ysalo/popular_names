/*
* hm3.c
*
* Author: Yaro Salo
*/
#define NUM_FILES 10
#define NUM_YEARS 10
#define START_YEAR 1920
#define NUM_ROWS 1000
#define NUM_COLS 30
#define NUM_NAMES 100
#define NUM_NAMES_TO_SORT 366
#define MAX_NAME_LENGTH 20

#include <string.h>
#include <stdio.h>

//Initialize ranks
void initialize_ranks(int ranks[][NUM_YEARS]) { 
    for(int i = 0; i < NUM_ROWS; i++) {
        for(int j = 0; j < NUM_YEARS; j++) {
            ranks[i][j] = -1;
        }
    }
}

void open_files(FILE *files[]) {
    int n = START_YEAR;
    for(int i = 0; i < NUM_FILES; i++) {
        char filename[12];
        sprintf(filename, "yob%d.txt", n);
        n += 10;
        files[i] = fopen(filename, "r");
    }
}

void get_name(char cols[NUM_COLS]) {
    for(int i = 0; i < NUM_COLS; i++) {
        if(cols[i] == ','){
            cols[i] = 0;
            break;
        }
    }
}

//Processes a name in a given year.
void process(char names[][NUM_COLS], int ranks[][NUM_YEARS] , char temp_name[20], int year, int i) {
        for(int j = 0; j < NUM_ROWS; j++) {
            if((strcmp(names[j], temp_name) == 0)) { 
                ranks[j][year] = i + 1; //Add the rank of an existing name in the corresponding year.
                break;
            } else {
                if(strcmp(names[j], "") == 0){ //Find an empty spot to put a new name.
                    strcpy(names[j],temp_name);
                    ranks[j][year ] = i+1;
                    break;
                }
            }
        }

}

//Get the name and corresponding rank from a file.
void read_single_file(char names[][NUM_COLS], int ranks[][NUM_YEARS] ,FILE* file, int year) {
    char temp[20];
    int my_year = year;
    for(int i = 0; i < NUM_NAMES; i++) {
        fscanf(file, "%s", temp);
        get_name(temp); //temp store the name
        process(names,ranks,temp,my_year, i);
    }
}

//Read all the files
void read_multiple_files(char names[][NUM_COLS], int ranks[][NUM_YEARS], FILE* files[]) {
    int year = 0;
    for (int i = 0; i < NUM_FILES; i++) {
        read_single_file(names, ranks, files[i], year);
        year++; //Every time a file is read let the other function know about the year.
    }
}

//Sort the names in alphabetical order.
void sort_name_rank(char names[][NUM_COLS], int ranks[][NUM_YEARS]) {
    int pass;
    for (pass = 0; pass < NUM_NAMES_TO_SORT; pass++) {
         for (int i = 0; i+1 < NUM_NAMES_TO_SORT; i++) {
             if (  strcmp( names[i], names[i+1])  > 0 ) {
                 char temp_name[MAX_NAME_LENGTH]; //Sort the name.
                 strcpy(temp_name, names[i]);
                 strcpy(names[i] ,names[i+1]);
                 strcpy(names[i+1], temp_name);
                 int temp_rank[100];
                 memcpy(temp_rank, ranks[i], sizeof(ranks[0])); //Sort the ranks along with the names.
                 memcpy(ranks[i],ranks[i + 1], sizeof(ranks[0]));
                 memcpy(ranks[i+1], temp_rank, sizeof(ranks[0]));
             }
         }
    }
}


//Write an output file.
void create_output_file(char names[][NUM_COLS], int ranks[][NUM_YEARS]) {
    char *header[] = {"Name", "1920", "1930", "1940", "1950", "1960",
                    "1970", "1980", "1990", "2000", "2010"};
    FILE *output_file;
    output_file = fopen("summary.csv", "w");

    int i = 0;
    while(i <= 10) {
        fprintf(output_file, "%s,", header[i++]);
    }
    fprintf(output_file, "\n");
    
    int n = 0;
    while((strcmp(names[n], "")) != 0) {
        fprintf(output_file, "%s,", names[n]);
        for(int i = 0; i < 10; i++) {
            if(ranks[n][i] == -1) {
                fprintf(output_file, ",");
            } else {
                fprintf(output_file, "%d,", ranks[n][i]);
            }
        }
        fprintf(output_file, "\n");
        n++;
    }
}

int main(void) {
    int ranks[NUM_ROWS][NUM_YEARS];
    char names[NUM_ROWS][NUM_COLS];
    FILE* files[NUM_FILES];
    initialize_ranks(ranks);
    open_files(files);
    read_multiple_files(names, ranks, files);
    sort_name_rank(names,ranks);
    create_output_file(names, ranks);
    return 0;
}
