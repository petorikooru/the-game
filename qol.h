#include <stdio.h>
#include <stdlib.h>

// Color for the text
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define RESET "\x1B[0m"

void back(){
    printf(YEL"Press enter to continue..."RESET);
    getchar(); while(getchar() != '\n' ); // Detect Enter
    system("clear");
}

int confirm(){
    char select[3];

    fgets(select, sizeof(select), stdin);
    select[strcspn(select, "\n")] = 0; // remove newline from fgets

    if (strlen(select) == 0 || select[0] == 'y' || select[0] == 'Y') return 1;
    else return 0;
}
