// Library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

// Format and color for the text
#define RED         "\x1B[31m"
#define GRN         "\x1B[32m"
#define YEL         "\x1B[33m"
#define BLU         "\x1B[34m"
#define MAG         "\x1B[35m"
#define CYN         "\x1B[36m"
#define RESET       "\x1B[0m"
#define UP          "\033[A"    // move 1 line to the top
#define CURSOR_H    "\x1B[?25l" // hide cursor
#define CURSOR_E    "\x1B[?25h" // enable cursor

/* Clear Screen */
void clear_screen(){
    #if defined(_WIN32)
        system("cls");
    #elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #else
        printf("erm.... (your os is too niche sorry, but trying to clean it anyway)");
        system("clear");
    #endif
}

/* Go back to previous function */
void back(){
    printf(YEL"Press enter to continue..."RESET);
    while(getchar() != '\n' ); // Detect Enter
    system("clear");
}

/* Number Input */
bool num_input(int * num){
    char input[100];

    fgets(input, sizeof(input), stdin);
    input[strlen(input) - 1] = '\0';

    int result = sscanf(input, "%i", num);
    
    if (result == 1) {
        return true;
    } else {
        printf(RED"Please input correctly >w< !\n"RESET);
        back();
        return false;
    }
}

/* Yes and no selection*/
bool confirm(){
    char select[2+1];

    fgets(select, sizeof(select), stdin);
    select[strcspn(select, "\n")] = 0;

    if (strlen(select) == 0 || select[0] == 'y' || select[0] == 'Y') return true;
    else return false;
}

/* File size check*/
bool check(FILE *data){
    // If the file doesnt exist
    if (!data) {
        printf(RED"There is no one yet (>_<)\n"RESET);
        return false;
    }

    fseek(data, 0, SEEK_END);
    long size = ftell(data);
    fclose(data);

    if (size <= 0) {
        printf(RED"There is no one yet (>_<)\n"RESET);
        return false;
    }
    else return true;
}

/* Miniature loading animation */
void mini_loading(){
    int duration = 200;

    printf("[—]");
    for (int i=0; i <= duration/10; i++){
        printf("\r"CURSOR_H);

        printf("[");
        if      (i%4 == 1)  printf(YEL"\\"RESET);
        else if (i%4 == 2)  printf(YEL"|"RESET);
        else if (i%4 == 3)  printf(YEL"/"RESET);
        else                printf(YEL"—"RESET);
        printf("]");

        fflush(stdout); usleep(1000*duration/2);
    }

    printf(CURSOR_E);
    system("clear");
}

/* Loading animation */
void loading(){
    int size = 34; // Loading bar size
    int candy = 2; // the "o"

    for (int i=0; i < size/2; i++){
        if (i==0)        printf("\n[");
        printf("%*co", candy-1, ' ');
        if (i==size/2-1) printf("]");
    }

    printf("\r");
    for (int i=0; i <= size; i++){
        if (i==0)       printf(CURSOR_H"[ ");
        if (i==size)    printf("]");
        else {
            if      (i == size - 1 ) printf(YEL"\b-"RESET);
            else if (i%2 >= 1)       printf(YEL"\b-"MAG"C"RESET);
            else                     printf(YEL"\b-"MAG"c"RESET);
        }

        fflush(stdout); usleep(1000*75);
    }

    printf(BLU" Done! (≧ ∇ ≦)\n"RESET CURSOR_E);
    fflush(stdout); usleep(1000*500);
}

/* Nuke loading animation */
void nuke_loading(){
    int size = 27; // Loading bar size

    printf(RED"\n[!] "RESET"Tactical Nuke Incoming "RED"[!]"RESET);
    for (int i=0; i <= size; i++){
        if (i == 0)     printf("\n[");
        printf("-");
        if (i == size)  printf("]");
    }

    printf("\r");
    for (int i=0; i <= size+1; i++){
        if (i == 0)         printf(CURSOR_H"[ ");
        if (i == size+1)    printf("]");
        else {
            if      (i == size) printf(RED"\b*"RESET);
            else if (i%4 == 1)  printf(BLU"\b~"MAG"\\"RESET);
            else if (i%4 == 2)  printf(BLU"\b~"MAG"|"RESET);
            else if (i%4 == 3)  printf(BLU"\b~"MAG"/"RESET);
            else                printf(BLU"\b~"MAG"—"RESET);
        }

        fflush(stdout); usleep(1000*100);
    }
    printf(CURSOR_E);

    fflush(stdout); usleep(1000*500);
}
