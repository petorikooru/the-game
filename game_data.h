/* =========== Libraries ============= */
/*
    // <- already defined in another
          file
*/
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "game_tui.h"
#include "game_tools.h"

/* =========== User Object =========== */
/*
    Index :
        1 : SLOT
        2 : RANDOM NUMBER GUESSER
        3 : COIN TOSS
        4 : TOTAL / RESERVED
*/

typedef struct {
    int32_t balances;
    uint16_t credits;
    uint16_t counter [4];
    bool persist    [4];
} GameData;

typedef struct {
    char username   [BUFF_STR];
    char password   [BUFF_STR];
    char message    [BUFF_STR];
    bool banned;
    GameData game_data;
} User;

#define FILENAME    "database.dat"

/* =============== Enums ============= */
/*
    Basically glorified shortcuts
*/

typedef enum {
    NONE,
    USER,
    ADMIN,
} UserType;

typedef enum {
    FAILED,
    FAILED_N,
    SUCCESS,
    SUCCESS_N,
} Status;

/* ============ Functions ============ */
void database_update(User *user){
    /*
        Update database file

        user  : Update the database of this user

        Return : None
    */

    FILE *database_file = fopen(FILENAME, "rb+");
    User *database_user = malloc(sizeof(User));

    // Update the counter
    uint8_t counter_total = 0;
    for (uint8_t i = 0; i < 3; i++){
        counter_total += user->game_data.counter[i];
    }
    user->game_data.counter[3] = counter_total;
    // Update to file
    while (fread(database_user, sizeof(User), 1, database_file) == 1) {
        if (strcmp(database_user->username, user->username) == 0) {
            fseek(database_file, -sizeof(User), SEEK_CUR);
            fwrite(user, sizeof(User), 1, database_file); 
            break;
        }
    }
    fclose(database_file);
    free(database_user);
    return;
}

static void database_create(FILE *database_file){
    /*
        Create a new database file (*.dat)

        database_file : the database file you want to create

        Return : None
    */

    database_file = fopen(FILENAME, "wb");

    // Default admin configuration
    const char username[] = "Admin";
    const char password[] = "123";
    User *admin = malloc(sizeof(User));

    // Account creation
    memcpy(admin->username, username, sizeof(username));
    memcpy(admin->password, password, sizeof(password));
    for (uint8_t i = 0; i < 4; i++){
        admin->game_data.counter[i] = 0;
        admin->game_data.persist[i] = false;
    }
    admin->game_data.counter[3] = 34463;
    admin->game_data.balances = 0;
    admin->game_data.credits  = 0;
    admin->banned = false;

    fwrite(admin, sizeof(User), 1, database_file);
    fclose(database_file);
    free(admin);
}

static void database_sort(User *database_user, const uint8_t database_Userotal, FILE *database_file){
    /*
        USort the database using Insertion Sort Algorithm

        We are using gambling counter as the reference in here

        database_file       : The file you want to sort
        database_user       : The Users that are in the database that you want to sort
        database_Userotal : How many user are in there

        Return : None
    */

    uint8_t temp_counter, j;
    User *temp_user = malloc(sizeof(User));

    for (uint8_t i = 0; i <= database_Userotal - 1; i++){
        (*temp_user) = database_user[i];
        temp_counter = database_user[i].game_data.counter[3];
        j = i;
        
        while (j > 0 && temp_counter > database_user[j-1].game_data.counter[3]){
            database_user[j] = database_user[j - 1];
            j = j - 1;
        }
        database_user[j] = (*temp_user);
    }

    free(temp_user);
}

uint8_t database_leaderboard(const uint8_t offset_x, const uint8_t offset_y,
                             FILE *database_file, User *database_user){
    /*
        Handle the database processing of leaderboard

        Offset in here is used to handle error message

        offset_x        : x cordinate position
        offset_y        : y cordinate position
        database_file   : The file you want to sort
        database_user   : The Users that are in the database

        Return : unsigned integer (user count)
    */

    // database_file = fopen(FILENAME, "rb");
    const bool check = input_filechecker(offset_x, offset_y, database_file);

    if (check == false)
        return 0;

    // Count how many user is registered
    fseek(database_file, 0, SEEK_END);
    const size_t database_size = ftell(database_file);
    const uint8_t database_Userotal = database_size / sizeof(User);

    // We are skipping the admin in here, hence <= 1
    if (database_Userotal <= 1){
        fclose(database_file);
        return 1;
    }

    // Convert into array
    database_user = malloc(sizeof(User) * database_Userotal);
    rewind(database_file);
    fread(database_user, sizeof(User), database_Userotal, database_file);

    database_sort(database_user, database_Userotal, database_file);

    return database_Userotal;
}

static void database_user_init(const char *username, const char *password, 
                               User *const user, const int32_t user_balances){
    /*
        Initialize new user

        username        : user's username
        password        : user's password
        user            : user that you want to initialize
        user_balances   : user's balances

        Return : None
    */

    strncpy(user->username, username, sizeof(user->username));
    strncpy(user->password, password, sizeof(user->password));
    for (int i = 0; i < 4; i++){
        user->game_data.counter[i] = 0;
        user->game_data.persist[i] = false;
    }
    user->game_data.balances = user_balances;
    user->game_data.credits  = 10;
    user->banned = false;

    // Save the user data
    FILE* database_file = fopen(FILENAME, "ab");
    fwrite(user, sizeof(User), 1, database_file);
    fclose(database_file);
}

UserType database_user_login(const uint8_t offset_x, const uint8_t offset_y, 
                                const char *username, const char *password, 
                                User *database_user){
    /*
        Check whether the user is an admin, user, or none of them

        Offset in here is used to handle error message

        offset_x        : x cordinate position
        offset_y        : y cordinate position
        username        : user's username
        password        : user's password
        database_user   : The Users that are in the database

        Return : user type
    */

    FILE* database_file = fopen(FILENAME, "rb");
    const bool check = input_filechecker(offset_x, offset_y, database_file);

    if (check == false)
        return NONE;

    UserType Userype;
    bool found = false;

    while(fread(database_user, sizeof(User), 1, database_file) == 1){
        if (strcmp(username, database_user->username) == SAME){
            found = true;

            if (strcmp(database_user->password, password) == SAME){
                if (strcmp(username, "Admin") == SAME)
                    Userype = ADMIN;
                else
                    Userype = USER;
                break;
            } else {
                screen_draw_line_center(offset_x, offset_y, WIDTH, "Wrong Password bleh :p", RED, RED);
                Userype = NONE;
                break;
            }
        }
    }

    if (!found){
        screen_draw_line_center(offset_x, offset_y, WIDTH, "User not found!", RED, RED);
        Userype = NONE;
    }

    fclose(database_file);
    return Userype;
}

Status database_user_signup(const uint8_t offset_x, const uint8_t offset_y, 
                                   const char *username, const char *password, 
                                   User *database_user){
    /*
        Check whether the user input valid inputs or whether
        the inputted username already exist in the database

        offset_x        : x cordinate position
        offset_y        : y cordinate position
        username        : user's username
        password        : user's password
        database_user   : The Users that are in the database

        Return : user type
    */

    // If the user input blank
    if (strlen(username) == 0){
        screen_draw_line_center(offset_x, offset_y, WIDTH, "Username cannot be blank!", RED_BG, RED);
        return FAILED;
    } else if (strlen(password) == 0){
        screen_draw_line_center(offset_x, offset_y, WIDTH, "Password cannot be blank!", RED_BG, RED);
        return FAILED;
    }

    // Check whether the data exist or not yet
    FILE* database_file = fopen(FILENAME, "rb+");
    const bool check = input_filechecker(offset_x, offset_y, database_file);
    bool newfile = false;

    if (check == false){
        newfile = true;
        screen_draw_line_center(offset_x, offset_y + 1, WIDTH, "Creating a new one...", LME_BG, LME);
        database_create(database_file);
        database_file = fopen(FILENAME, "rb+");
    }

    // Check whether the username is already registered
    bool found = false;

    while(fread(database_user, sizeof(User), 1, database_file) == 1){
        if (strcmp(username, database_user->username) == SAME){
            found = true;
            break;
        }
    }

    fclose(database_file);
    if (found){
        if (newfile){
            screen_draw_line_center(offset_x, offset_y + 2, WIDTH, "Username already exist!", RED_BG, RED);
            return FAILED_N;
        } else {
            screen_draw_line_center(offset_x, offset_y, WIDTH, "Username already exist!", RED_BG, RED);
            return FAILED;
        }
    }

    if (newfile)
        return SUCCESS_N;
    else
        return SUCCESS;

}