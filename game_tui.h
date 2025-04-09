/* =========== Libraries ============= */
/*
    // <- already defined in another
          file
*/
#include <stdarg.h> // ::{va_list, va_start, vprintf, va_end}
#include <stdlib.h> // ::{malloc, free}
#include <stdint.h> // ::{uint8_t}
#include <stdio.h>  // ::{printf, fflush}
#include <string.h> // ::{strlen, strcpy, strcat}
#include <unistd.h> // ::{usleep}

/* =========== Checking... =========== */
#ifndef GAME_TUI
#define GAME_TUI

/* =============== TUI =============== */
/*
    You can set the default width your
    default ui HEIGHT and WIDTH here
*/
#define WIDTH       57
#define HEIGHT      10
#define OFFSET_X    60
#define OFFSET_Y    2

/* =========== Loading Bar =========== */
/*
    Customize how long your loading
    bar will be!!!!
*/
#define MILISECONDS 1000
#define HOLD_DELAY  750

/* ============= Colors ============== */
/*
    I love Eye Candy
*/
#define BLK             "\e[30m"
#define BLK_BG          "\e[40m"
#define RED             "\e[31m"
#define RED_BG          "\e[41m"
#define GRN             "\e[32m"
#define GRN_BG          "\e[42m"
#define YEL             "\e[33m"
#define YEL_BG          "\e[43m"
#define BLU             "\e[34m"
#define BLU_BG          "\e[44m"   
#define MAG             "\e[35m"
#define MAG_BG          "\e[45m"
#define CYN             "\e[36m"
#define CYN_BG          "\e[46m"
#define WHT             "\e[37m"
#define WHT_BG          "\e[47m"
#define GRY             "\e[90m"
#define GRY_BG          "\e[100m"
#define PNK             "\e[91m"
#define PNK_BG          "\e[101m"
#define LME             "\e[92m"
#define LME_BG          "\e[102m"
#define RST           "\e[0m"

/* ============= Cursor ============== */
/*
    Cursor controls
*/                                                            
#define SET_COLOR(color)        printf("%s", (color));
#define SET_CURSOR(cursor)      printf("%s", (cursor));
#define SET_OFFSET(x, y)        printf("\e[%d;%dH", (y), (x)); 

#define CURSOR_UP       "\e[A"
#define CURSOR_DOWN     "\n"
#define CURSOR_DOWN_ALT "\e[B"
#define CURSOR_LEFT     "\b"
#define CURSOR_RIGHT    " "
#define CURSOR_HIDE     "\e[?25l"
#define CURSOR_SHOW     "\e[?25h"
#define CURSOR_START    "\r"
#define CURSOR_HOME     "\e[H"

/* ============ Functions ============ */

void screen_clear(){
    /*
        Clear the terminal screen
    */

    #if   defined (_WIN32) 
        #include <conio.h>
        clrscr();
    #else
        printf("\e[1;1H\e[2J");
    #endif
}

void screen_draw_raw(const uint8_t offset_x, const uint8_t offset_y, 
                     const char* color, const char* format, ...){
    /*
        Draw a text in (x, y) coordinate as you wish 

        offset_x : x cordinate position
        offset_y : y cordinate position
        color    : box's color
        format   : the text format ("%s", "%d", etc.)
        ...      : contents
    */

    va_list args;

    SET_OFFSET(offset_x, offset_y);
    SET_COLOR(color);

    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    SET_COLOR(RST);

    SET_OFFSET(offset_x, offset_y);
}

void screen_draw_box(const uint8_t offset_x, const uint8_t offset_y, 
                     const uint8_t size_x, const uint8_t size_y, 
                     const char* color){
    /*
        Draw a plain box in your terminal screen

        offset_x : x cordinate position
        offset_y : y cordinate position
        size_x   : box's width
        size_y   : box's height
        color    : box's color
    */

    SET_COLOR(color);

    for (uint8_t i = 0; i < size_y; i++){
        SET_OFFSET(offset_x, offset_y + i);

        for (uint8_t j = 0; j < size_x; j++){    
            if (i == 0 || i == size_y - 1){
                if (j == 0 || j == size_x - 1)
                    printf("+");
                else
                    printf("-");
            } else {
                if (j == 0 || j == size_x - 1)
                    printf("|");
                else
                    printf(" ");
            }
        }
        SET_CURSOR(CURSOR_DOWN);
    }
    SET_COLOR(RST);
}

void screen_draw_box_title(const uint8_t offset_x, const uint8_t offset_y, 
                           const uint8_t size_x, const uint8_t size_y, 
                           const char* title, const char* color){
    /*
        Draw a identifiable box in your terminal screen

        offset_x : x cordinate position
        offset_y : y cordinate position
        size_x   : box's width
        size_y   : box's height
        title    : box's title
        color    : box's color
    */

    SET_COLOR(color);

    const uint8_t middle = size_x / 2;
    const uint8_t title_width = strlen(title);
    const uint8_t title_offset = 2;
    const uint8_t title_placement = middle - (title_width / 2) - title_offset;

    for (uint8_t i = 0; i < size_y; i++){
        SET_OFFSET(offset_x, offset_y + i);

        for (uint8_t j = 0; j < size_x; j++){    
            if (i == 0){
                if (j == 0 || j == (size_x - title_width) - 4)
                    printf("+");
                else if (j == title_placement)
                    printf("{ %s }", title);
                else if(j < (size_x - title_width) - 4)
                    printf("-");
                else break;
            } else if (i == size_y - 1){
                if (j == 0 || j == size_x - 1)
                    printf("+");
                else
                    printf("-");
            } else {
                if (j == 0 || j == size_x - 1)
                    printf("|");
                else
                    printf(" ");
            }
        }
        SET_CURSOR(CURSOR_DOWN);
    }
    SET_COLOR(RST);
}

void screen_draw_line(const uint8_t offset_x, const uint8_t offset_y,
                      const uint8_t size_x, const char* string, 
                      const char* string_color, const char* border_color){
    /*
        Draw a line segment of the box with contents

        offset_x : x cordinate position
        offset_y : y cordinate position
        size_x   : line's width
        string   : contents
        string_color    : contents's color
        border_color    : box's color
    */

    SET_OFFSET(offset_x, offset_y);

    const uint8_t size_x_offset = size_x - 4;
    const uint8_t string_len = strlen(string);

    SET_COLOR(RST); SET_COLOR(border_color); printf("| "); 
    SET_COLOR(string_color);

    printf("%s", string);
    for (uint8_t i = 0; i < size_x_offset - string_len; i++)
        printf(" ");

    SET_COLOR(RST); SET_COLOR(border_color); printf(" |"); 
}

void screen_draw_line_input(const uint8_t offset_x, const const uint8_t offset_y,
                            const uint8_t size_x, const char* string, 
                            const char* string_color, const char* border_color){
    /*
        Draw a line segment of the box with contents, but as an input
        for the user

        offset_x : x cordinate position
        offset_y : y cordinate position
        size_x   : line's width
        string   : contents
        string_color    : contents's color
        border_color    : box's color
    */

    SET_OFFSET(offset_x, offset_y);

    const uint8_t size_x_offset = size_x - 4;
    const uint8_t string_len = strlen(string);

    screen_draw_line(offset_x, offset_y, size_x, string, string_color, border_color);

    SET_COLOR(WHT);
    SET_OFFSET(string_len + offset_x + 2, offset_y);
}

void screen_draw_line_center(const uint8_t offset_x, const uint8_t offset_y,
                             const uint8_t size_x, const char *string, 
                             const char *string_color, const char *border_color){
    /*
        Draw a line segment of the box with contents centered

        offset_x : x cordinate position
        offset_y : y cordinate position
        size_x   : line's width
        string   : contents
        string_color    : contents's color
        border_color    : box's color
    */

    const uint8_t middle = size_x / 2;
    const uint8_t string_len = strlen(string);
    const uint8_t string_offset = 2;
    const uint8_t string_placement = middle - (string_len / 2) - string_offset;
    char* string_padding = malloc(string_placement+1);

    for (uint8_t i = 0; i < string_placement; i++){
        string_padding[i] = ' ';
    }
    string_padding[string_placement] = '\0';

    char *string_center = malloc(string_len + string_placement + 1);
    strcpy(string_center, string_padding);
    strcat(string_center, string);

    screen_draw_line(offset_x, offset_y, size_x, string_center, string_color, border_color);

    free(string_padding);
    free(string_center);
}

void screen_draw_line_decor(const uint8_t offset_x, const uint8_t offset_y,
                            const uint8_t size_x, const char* border_color){
    /*
        Draw the top/bottom side of the box

        offset_x : x cordinate position
        offset_y : y cordinate position
        size_x   : line's width
        border_color    : box's color
    */

    SET_OFFSET(offset_x, offset_y);
    SET_COLOR(border_color);
    for (uint8_t i = 0; i < size_x; i++){
        if (i == 0 || i == size_x - 1)
            printf("+");
        else
            printf("-");
    }
    SET_COLOR(RST);
    SET_CURSOR(CURSOR_DOWN);
}

void screen_loading_mini(const uint8_t offset_x, const uint8_t offset_y){
    /*
        Draw the minified loading bar (just for decoration though)

        offset_x : x cordinate position
        offset_y : y cordinate position
    */

    const uint8_t durations = 100; //In miliseconds
    const uint8_t chunks    = durations / 6;
    const char *shape[]     = {"—", "\\", "|", "/"};
    
    SET_CURSOR(CURSOR_HIDE);
    SET_OFFSET(offset_x, offset_y);

    printf("[ ]");
    for (uint8_t i=0; i <= chunks; i++){
        SET_OFFSET(offset_x + 1, offset_y);

        
        if      (i%4 == 1)  printf(YEL"%s"RST, shape[1]);
        else if (i%4 == 2)  printf(YEL"%s"RST, shape[2]);
        else if (i%4 == 3)  printf(YEL"%s"RST, shape[3]);
        else                printf(YEL"%s"RST, shape[0]);
       
        fflush(stdout); 
        usleep(durations * MILISECONDS);
    }

    SET_CURSOR(CURSOR_SHOW);
}

void screen_loading_default(const uint8_t offset_x, const uint8_t offset_y, 
                            const uint8_t width){
    /*
        Draw the regular loading bar (just for decoration though)

        offset_x : x cordinate position
        offset_y : y cordinate position
        width    : line's width
    */

    const uint8_t durations = 60; //In miliseconds
    const uint8_t size      = width - 6; 
    const char *shape[]     = {" ", "-", "o", "c", "C"};

    //Display pre-filled bar
    SET_CURSOR(CURSOR_HIDE);
    screen_draw_line_input(offset_x, offset_y, width, "", GRN, GRN);

    printf("[");
    for (uint8_t i = 0; i < size; i++){
        if (i % 2 == 1) printf("%s", shape[0]);
        else            printf("%s", shape[2]);   
    }
    printf("]");

    //I love eating candies
    for (uint8_t i = 1; i < size; i++){ 
        SET_OFFSET(offset_x + i + 2, offset_y);
        printf(YEL "%s" RST, shape[1]);

        if (i%2 >= 1)   printf(MAG"%s"RST, shape[4]);
        else            printf(MAG"%s"RST, shape[3]);

        fflush(stdout); 
        usleep(durations * MILISECONDS);
    }

    screen_draw_line(offset_x, offset_y, width, "", GRN_BG, GRN);
    screen_draw_line_center(offset_x, offset_y, width, "Done! (≧ ∇ ≦)", GRN_BG, GRN);
    SET_CURSOR(CURSOR_SHOW);

    fflush(stdout);
    usleep(HOLD_DELAY * MILISECONDS);
}

void screen_loading_nuke(const uint8_t offset_x, const uint8_t offset_y, 
                         const uint8_t width){
    /*
        Draw the nuke loading bar (think fast!)

        offset_x : x cordinate position
        offset_y : y cordinate position
        width    : line's width
    */

    const uint8_t durations = 75; //In miliseconds
    const uint8_t size      = width - 6; 
    const char *shape[]     = {".", "~", "/", "—", "\\", "|"};

    //Display pre-filled bar
    SET_CURSOR(CURSOR_HIDE);
    screen_draw_line_input(offset_x, offset_y, width, "", RED, RED);

    printf("[");
    for (uint8_t i = 0; i < size * 2; i++){
        if (i % 2 == 1) printf("%s", shape[0]);
    }
    printf("]");

    //Tactical Nuke Incoming!!!
    for (uint8_t i=0; i < size - 1; i++){
        SET_OFFSET(offset_x + i + 3, offset_y);
        printf(YEL "%s" RST, shape[1]);

        if      (i == size-1) printf(RED"*"RST);
        else if (i%5 == 1)  printf(RED"%s"RST, shape[2]);
        else if (i%5 == 2)  printf(RED"%s"RST, shape[3]);
        else if (i%5 == 3)  printf(RED"%s"RST, shape[4]);
        else                printf(RED"%s"RST, shape[5]);

        fflush(stdout); 
        usleep(durations * MILISECONDS);
    }
    screen_draw_line_center(offset_x, offset_y, width, "Nuked! (≧ ∇ ≦)", RED_BG, RED);
    SET_CURSOR(CURSOR_SHOW);

    fflush(stdout); 
    usleep(HOLD_DELAY * MILISECONDS);
}

#endif