/* =========== Libraries ============= */
/*
    // <- already defined in another
          file
*/
#include <stdbool.h>     // ::{bool}
#include <stdlib.h>      // ::{malloc, free, size_t} 
#include <stdint.h>      // ::{uint8_t}
#include <stdio.h>       // ::{fgets, getchar, fseek, rewind, fclose}
#include <string.h>      // ::{strlen, strcmp}
#include "game_tui.h"    // ::{screen_draw, screen_clear}

/* =========== Checking... =========== */
#ifndef GAME_TOOLS
#define GAME_TOOLS

/* ============== Macros ============= */
/*
    I'm too lazy
*/
#define BUFF_STR    128
#define RUNNING     1
#define SAME        0
#define None        false

/* ============ Functions ============ */

char* input_string(){
    /*
        Handle string input

        Return : string (char) type
    */

    // Allocate memory for the string
    char *string = malloc(BUFF_STR);
    
    // Use fgets for input handling (i mean, i can use printf, but whatever)
    fgets(string, BUFF_STR, stdin);
    string[strlen(string) - 1] = '\0';
    return string;
}

bool input_confirmation(){
    /*
        Handle confirmation dialog (yes or no)

        Return : bool
    */

    char *input = input_string();

    if (strlen(input) == 0){ // Press enter
        free(input);
        return true;
    } else if (strcmp(input, "y") == SAME ||  strcmp(input, "Y") == SAME){
        free(input);
        return true;
    } else {
        free(input);
        return false;
    }
}

void input_continue(const uint8_t offset_x, const uint8_t offset_y, const uint16_t width){
    /*
        Basically give a time for the user to read the
        contents before it get cleared

        offset_x : x cordinate position
        offset_y : y cordinate position

        Return : None
    */

    screen_draw_line_center(offset_x, offset_y, width, "Press enter to continue...", YEL_BG, YEL);

    SET_CURSOR(CURSOR_HIDE); while(getchar() != '\n'); SET_CURSOR(CURSOR_SHOW);

    screen_clear();
}

bool input_number(const uint8_t offset_x, const uint8_t offset_y, int32_t *integer){
    /*
        Handle number input

        It is quite special since the return type
        is a boolean instead of int, because it will
        handle whether the user is actually inputting
        a number or just random stuff, hence there are
        also offset parameters for the box.

        offset_x : x cordinate position
        offset_y : y cordinate position

        Return : bool
    */

    char *input = input_string();

    // Check whether it is truly an integer
    const uint8_t check = sscanf(input, "%d", integer);

    if (check == 1){
        free(input);
        return true;
    } else {
        screen_draw_line_center(offset_x, offset_y, WIDTH, "Please input the number correctly >w<", RED_BG, RED);
        free(input);
        return false;
    }
}
 
bool input_filechecker(const uint8_t offset_x, const uint8_t offset_y, FILE *file){
    /*
        Handle file input
       
        Offset in here is used to handle error message

        offset_x : x cordinate position
        offset_y : y cordinate position

        Return : bool
    */

    // Check whether the file exists at all
    if (file == false) {
        screen_draw_line_center(offset_x, offset_y, WIDTH, "There's no data file yet >_<", RED_BG, RED);
        return false;
    }

    // Check whether the file has 0 bytes of data
    fseek(file, 0, SEEK_END);
    const size_t size = ftell(file);

    if (size > 0){
        rewind(file);
        return true;
    } else {
        screen_draw_line_center(offset_x, offset_y, WIDTH, "There's no one yet >_<", RED_BG, RED);
        fclose(file);
        return false;
    }
}

#endif