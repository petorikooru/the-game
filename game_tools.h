/* ========== Library ========== */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

/* ==== Formats and Colors ===== */
#define RED             "\x1B[31m"
#define GRN             "\x1B[32m"
#define YEL             "\x1B[33m"
#define BLU             "\x1B[34m"
#define MAG             "\x1B[35m"
#define CYN             "\x1B[36m"
#define RESET           "\x1B[0m"
#define CURSOR_UP       "\033[A"
#define CURSOR_DOWN     "\n"
#define CURSOR_HIDE     "\x1B[?25l"
#define CURSOR_SHOW     "\x1B[?25h"
#define CURSOR_START    "\r"
#define CURSOR_BACK     "\b"

/* ========== Macros ========== */
#define BUFF_STR 32
#define RUNNING 1
#define MILISECONDS 1000
#define HOLD_DELAY 500

/* =========== Enums ========== */
typedef enum {
    FAILED,
    SUCCESS,
} status_t;

void screen_clear(){
    #if   defined(_WIN32) 
        system("cls");
    #elif defined(__unix__)
        printf("\e[1;1H\e[2J");
    #endif
}

char* sys_input_str(){
    // Allocate memory for the string
    char* string = malloc(BUFF_STR);
    
    // Use fgets for input handling (i mean, i can use printf, but whatever)
    fgets(string, BUFF_STR, stdin);
    string[strlen(string) - 1] = '\0';
    return string;
}

bool sys_input_confirm(){
    char* input;
    input = sys_input_str();

    if (strlen(input) == 0){ // Press enter
        free(input);
        return true;
    } else if (strcmp(input, "y") == 0 ||  strcmp(input, "Y") == 0){
        free(input);
        return true;
    } else {
        free(input);
        return false;
    }
}

void sys_input_return(){
    printf(YEL "Press enter to continue..." RESET);
    while(getchar() != '\n');
    screen_clear();
}

bool sys_input_int(int * integer){
    char* input;
    input = sys_input_str();

    // Check whether it is truly an integer
    int32_t check = sscanf(input, "%i", integer);
    free(input);
    if (check == 1){
        return true;
    } else {
        printf(RED "Please input the number correctly >w<\n" RESET);
        sys_input_return();
        return false;
    }
}

bool sys_input_check_file(FILE * file){
    // Check whether the file exist at all
    if (!file) {
        printf(RED "[!] There's no data file yet >_<\n" RESET);
        return false;
    }

    // Check whether the file has 0 bytes of data
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);

    if (size > 0){
        rewind(file);
        return true;
    } else {
        printf(RED "[!] There's no one yet >_<\n" RESET);
        fclose(file);
        return false;
    }
}

void sys_loading_mini(){
    uint8_t durations = 100; //In miliseconds
    uint8_t chunks    = durations/6;
    char *shape[] = {"—", "\\", "|", "/"};

    //The actual part
    printf(CURSOR_HIDE);
    for (uint8_t i=0; i <= chunks; i++){
        printf(CURSOR_START);

        printf("[");
        if      (i%4 == 1)  printf(YEL"%s"RESET, shape[1]);
        else if (i%4 == 2)  printf(YEL"%s"RESET, shape[2]);
        else if (i%4 == 3)  printf(YEL"%s"RESET, shape[3]);
        else                printf(YEL"%s"RESET, shape[0]);
        printf("]");

        fflush(stdout); 
        usleep(durations * MILISECONDS);
    }

    printf(CURSOR_SHOW);
    screen_clear();
}

void sys_loading_default(){
    uint8_t durations = 75; //In miliseconds
    uint8_t size    = 30; 
    char *shape[] = {" ", "-", "o", "c", "C"};

    //Display pre-filled bar
    printf(CURSOR_HIDE);
    printf("[");
    for (int i=0; i < size; i++){
        if (i % 2 == 1) printf("%s", shape[0]);
        else            printf("%s", shape[2]);   
    }
    printf("]");

    //I love eating candies
    printf(CURSOR_START);
    printf("[ ");
    for (int i=1; i < size; i++){ 
        printf(CURSOR_BACK YEL "%s" RESET, shape[1]);

        if (i%2 >= 1)   printf(MAG"%s"RESET, shape[4]);
        else            printf(MAG"%s"RESET, shape[3]);

        fflush(stdout); 
        usleep(durations * MILISECONDS);
    }
    printf("]");
    printf(BLU" Done! (≧ ∇ ≦)"RESET);
    printf(CURSOR_DOWN);
    printf(CURSOR_SHOW);

    fflush(stdout); 
    usleep(HOLD_DELAY * MILISECONDS);
}

void sys_loading_nuke(){
    uint8_t durations = 75; //In miliseconds
    uint8_t size    = 30; 
    char *shape[] = {" ", "~", "/", "—", "\\", "|"};

    //Display pre-filled bar
    printf(CURSOR_HIDE);
    printf("[");
    for (int i=0; i < size+1; i++){
        printf("%s", shape[0]);   
    }
    printf("]");

    //Tactical Nuke Incoming!!!
    printf(CURSOR_START);
    printf("[ ");
    for (int i=0; i < size; i++){ 
        printf(CURSOR_BACK YEL "%s" RESET, shape[1]);

        if      (i == size-1) printf(RED"*"RESET);
        else if (i%5 == 1)  printf(RED"%s"RESET, shape[2]);
        else if (i%5 == 2)  printf(RED"%s"RESET, shape[3]);
        else if (i%5 == 3)  printf(RED"%s"RESET, shape[4]);
        else                printf(RED"%s"RESET, shape[5]);

        fflush(stdout); 
        usleep(durations * MILISECONDS);
    }
    printf("]");
    printf(RED" Nuked! (≧ ∇ ≦)"RESET);
    printf(CURSOR_DOWN);
    printf(CURSOR_SHOW);

    fflush(stdout); 
    usleep(HOLD_DELAY * MILISECONDS);
}
