#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Color for the text
#define RED         "\x1B[31m"
#define GRN         "\x1B[32m"
#define YEL         "\x1B[33m"
#define BLU         "\x1B[34m"
#define MAG         "\x1B[35m"
#define RESET       "\x1B[0m"
#define CURSOR_H    "\x1B[?25l" // hide cursor
#define CURSOR_E    "\x1B[?25h" // enable cursor

void back(){
    printf(YEL"Press enter to continue..."RESET);
    // getchar(); 
    while(getchar() != '\n' ); // Detect Enter
    system("clear");
}

int confirm(){
    char select[3];

    fgets(select, sizeof(select), stdin);
    select[strcspn(select, "\n")] = 0; // remove newline from fgets

    if (strlen(select) == 0 || select[0] == 'y' || select[0] == 'Y') return 1;
    else return 0;
}
void loading(){
    int size = 34; // Loading bar size
    int candy = 2; // the "o"

    for (int i=0; i < size/2; i++){
        if (i==0) printf("\n[");
        printf("%*co", candy-1, ' ');
        if (i==size/2-1) printf("]");
    }

    printf("\r");
    for (int i=0; i <= size; i++){
        fflush(stdout);
        usleep(1000*75);
        if (i==0) printf(CURSOR_H"[ ");
        if (i==size) printf("]");
        else {
            if (i == size - 1 )printf(YEL"\b-"RESET);
            else if (i%2 >= 1) printf(YEL"\b-"MAG"C"RESET);
            else printf(YEL"\b-"MAG"c"RESET);
        }
    }

    printf(BLU" Done! (≧∇≦)\n"RESET CURSOR_E);
    fflush(stdout); usleep(1000*500);
}
