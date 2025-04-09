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
#include <time.h>

// Local Libraries /////////////////////
#include "game_tui.h"
#include "game_tools.h"
#include "game_data.h"
#include "game_textify.h"

/* ====== Prototype functions ======= */
void ui_start       ();
void ui_start_login ();
void ui_start_signup();
void ui_start_signup_setup(const char* username, 
                           const char* password, 
                           User *const user);
void ui_leaderboard ();
// User menu ///////////////////////////
void ui_user            (User *const user);
void ui_user_slot       (User *const user);
void ui_user_coin       (User *const user);
void ui_user_rng        (User *const user);
void ui_user_market     (User *const user);
void ui_user_achievement(User *const user);
// Admin menu //////////////////////////
void ui_admin       ();
uint8_t ui_admin_view  (const bool called);
void ui_admin_ban   ();
void ui_admin_unban ();
void ui_admin_delete();
void ui_admin_export();
void ui_fun_ascii   ();
// Cheat menu //////////////////////////
static bool cheat[3] = {false};
void ui_cheat        ();
void ui_cheat_credits();
void ui_cheat_games  (uint8_t type);

void main(){
    srand(time(NULL));
    ui_start();
    return;
}

/* ===================== Start Screen ======================= */
void ui_start(){
    typedef enum {
        LOGIN       = 1,
        SIGNUP      = 2,
        LEADERBOARD = 3,
        EXIT        = 0,
    } MenuStart;
    int32_t input;

    while(RUNNING){
        time_t t = time(NULL);
        char* clock = asctime(localtime(&t));
        clock[strlen(clock) - 1] = '\0';

        screen_clear();
        
        screen_draw_box_title(OFFSET_X, OFFSET_Y, WIDTH, HEIGHT, "Main Menu", CYN);
        screen_draw_line(OFFSET_X, OFFSET_Y + 1, WIDTH, "Welcome to the Game Center, Gamblers!", MAG, CYN);
        screen_draw_line(OFFSET_X, OFFSET_Y + 2, WIDTH, "In here, you can play numerous of very fun games", MAG, CYN);
        screen_draw_line(OFFSET_X, OFFSET_Y + 3, WIDTH, "User, please select one of these option!", BLU, CYN);
        screen_draw_raw(OFFSET_X, OFFSET_Y + 4, CYN, "+------------------+------------------------------------+");
        screen_draw_line(OFFSET_X, OFFSET_Y + 5, WIDTH + 5,  "1. Login         "CYN"|", GRN, CYN);
        screen_draw_line(OFFSET_X, OFFSET_Y + 6, WIDTH + 10, "2. Sign in       "CYN"|"WHT"     Current time :", LME, CYN);
        screen_draw_line(OFFSET_X, OFFSET_Y + 7, WIDTH + 5,  "3. Leaderboard   "CYN"|", YEL, CYN);
        screen_draw_raw(OFFSET_X + 25, OFFSET_Y + 7, YEL, "%s", clock);
        screen_draw_line(OFFSET_X, OFFSET_Y + 8, WIDTH + 5, "0. Exit          "CYN"|", RED, CYN);
        screen_draw_raw(OFFSET_X, OFFSET_Y + 9, CYN, "+------------------+------------------------------------+");
        screen_draw_line_decor(OFFSET_X, OFFSET_Y + 11, WIDTH, YEL);
        screen_draw_line_input(OFFSET_X, OFFSET_Y + 10, WIDTH, "Your input : ", YEL, YEL);

        bool check = input_number(OFFSET_X, OFFSET_Y + 10, &input);

        if (check == false) {
            screen_draw_line_center(OFFSET_X, OFFSET_Y + 10, WIDTH, "Please input the number correctly >w<", RED_BG, RED);
            screen_draw_line_decor(OFFSET_X, OFFSET_Y + 12, WIDTH, RED);
            input_continue(OFFSET_X, OFFSET_Y + 11, WIDTH); 
            continue;
        }

        switch(input){
            case LOGIN: 
                ui_start_login();
                break;
            case SIGNUP: 
                ui_start_signup(); 
                break;
            case LEADERBOARD: 
                ui_leaderboard();
                break;
            case EXIT: 
                screen_clear();
                screen_draw_box(OFFSET_X, OFFSET_Y, WIDTH, 3, MAG);
                screen_draw_line_center(OFFSET_X, OFFSET_Y + 1, WIDTH, "See you next time!", MAG, MAG);
                SET_CURSOR(CURSOR_DOWN);
                SET_CURSOR(CURSOR_DOWN);
                return;
            default:
                screen_draw_line_center(OFFSET_X, OFFSET_X + 10, WIDTH, "Please input the number correctly >w<", RED_BG, RED);
                screen_draw_line_decor(OFFSET_X, OFFSET_X + 12, WIDTH, RED);
                input_continue(OFFSET_X, OFFSET_X + 11, WIDTH); 
                break;
        }
    }   
}

void ui_start_login(){
    screen_clear();

    // Pre-render display
    screen_draw_box_title(OFFSET_X, OFFSET_Y + 2, WIDTH, 4, "Login", LME);
    screen_draw_line(OFFSET_X, OFFSET_Y + 3, WIDTH, "Username : ", BLU, LME);
    screen_draw_line(OFFSET_X, OFFSET_Y + 4, WIDTH, "Password : ", BLU, LME);

    // Input the username and password
    char *username, *password;

    screen_draw_line_input(OFFSET_X, OFFSET_Y + 3, WIDTH, "Username : ", BLU, LME);
    username = input_string();
    screen_draw_line_input(OFFSET_X, OFFSET_Y + 4, WIDTH, "Password : ", BLU, LME);
    password = input_string();

    // Check the inputs with the database
    User* user = malloc(sizeof(User));
    UserType Userype = database_user_login(OFFSET_X, OFFSET_Y + 6, username, password, user);

    // The council will check your data :)
    switch(Userype){
        case ADMIN:
            screen_draw_line_center(OFFSET_X, OFFSET_Y + 6, WIDTH, "Login Successfull >w< !!! Entering as Admin...", CYN_BG, CYN);
            screen_draw_line_decor(OFFSET_X, OFFSET_Y + 8, WIDTH, CYN);
            input_continue(OFFSET_X, OFFSET_Y + 7, WIDTH);
            ui_admin();
            break;
        case USER:
            if (user->banned){ 
                screen_draw_line_center(OFFSET_X, OFFSET_Y + 6, WIDTH, "Mr Admin has banned you due to this reason...", RED_BG, RED);
                screen_draw_line(OFFSET_X, OFFSET_Y + 7, WIDTH, "", RED, RED);
                screen_draw_raw(OFFSET_X + 2, OFFSET_Y + 7, RED, "-> %-55s", user->message);
                screen_draw_line_decor(OFFSET_X, OFFSET_Y + 9, WIDTH, RED);
                input_continue(OFFSET_X, OFFSET_Y + 8, WIDTH);
            } else {
                screen_draw_line_center(OFFSET_X, OFFSET_Y + 6, WIDTH, "Login Successfull !!! Entering as User...", GRN_BG, LME);
                screen_draw_line_decor(OFFSET_X, OFFSET_Y + 8, WIDTH, LME);
                input_continue(OFFSET_X, OFFSET_Y + 7, WIDTH);
                ui_user(user);
            }
            break;
        case NONE:  
            screen_draw_line_center(OFFSET_X, OFFSET_Y + 7, WIDTH, "Login Failed > A <", RED_BG, RED);
            screen_draw_line_decor(OFFSET_X, OFFSET_Y + 9, WIDTH, RED);
            input_continue(OFFSET_X, OFFSET_Y + 8, WIDTH);
            break;
    } 
    free(username); free(password); 
    free(user);
}

void ui_start_signup(){
    screen_clear();

    // Display UI
    screen_draw_box_title(OFFSET_X, OFFSET_Y + 2, WIDTH, 4, "Sign Up", LME);
    screen_draw_line(OFFSET_X, OFFSET_Y + 3, WIDTH, "Username : ", BLU, LME);
    screen_draw_line(OFFSET_X, OFFSET_Y + 4, WIDTH, "Password : ", BLU, LME);

    // Input the username and password
    char *username, *password;
    screen_draw_line_input(OFFSET_X, OFFSET_Y + 3, WIDTH, "Username : ", BLU, LME);
    username = input_string();
    screen_draw_line_input(OFFSET_X, OFFSET_Y + 4, WIDTH, "Password : ", BLU, LME);
    password = input_string();

    // Check the inputs with the database
    User* database = malloc(sizeof(User));
    Status status = database_user_signup(OFFSET_X, OFFSET_Y + 6, username, password, database);

    // Hop on gambling
    switch(status){
        case SUCCESS_N:
        case SUCCESS:
            if (status == SUCCESS_N){
                screen_draw_line_center(OFFSET_X, OFFSET_Y + 8, WIDTH, "Sign-up Successfull >w< ! Entering the First Setup!", BLU, BLU);
                screen_draw_line_decor(OFFSET_X, OFFSET_Y + 10, WIDTH, BLU);
                input_continue(OFFSET_X, OFFSET_Y + 9, WIDTH);
            } else {
                screen_draw_line_center(OFFSET_X, OFFSET_Y + 6, WIDTH, "Sign-up Successfull >w< ! Entering the First Setup!", BLU, BLU);
                screen_draw_line_decor(OFFSET_X, OFFSET_Y + 8, WIDTH, BLU);
                input_continue(OFFSET_X, OFFSET_Y + 7, WIDTH);
            }
            ui_start_signup_setup(username, password, database);
            break;
        case FAILED_N:
        case FAILED:
        if (status == FAILED_N){
            screen_draw_line_center(OFFSET_X, OFFSET_Y + 8, WIDTH, "Sign-up Failed ! Failed to create an account T-T", RED, RED);
            screen_draw_line_decor(OFFSET_X, OFFSET_Y + 10, WIDTH, RED);
            input_continue(OFFSET_X, OFFSET_Y + 9, WIDTH);
        } else {
            screen_draw_line_center(OFFSET_X, OFFSET_Y + 7, WIDTH, "Sign-up Failed ! Failed to create an account T-T", RED, RED);
            screen_draw_line_decor(OFFSET_X, OFFSET_Y + 9, WIDTH, RED);
            input_continue(OFFSET_X, OFFSET_Y + 8, WIDTH);
            break;
        }
    }
    free(username); free(password);
    free(database);
}

void ui_start_signup_setup(const char* username, const char* password, 
                         User *const user){
    int32_t user_balances;

    while(RUNNING){
        screen_clear();

        screen_draw_box_title(OFFSET_X, OFFSET_Y, WIDTH, 6, "Setup", BLU);
        screen_draw_line_center(OFFSET_X, OFFSET_Y + 1, WIDTH, "You must be new here :shake_hand:", MAG, BLU);
        screen_draw_line(OFFSET_X, OFFSET_Y + 2, WIDTH, "Welcome to your \"first time\" main menu!", YEL, BLU);
        screen_draw_line(OFFSET_X, OFFSET_Y + 3, WIDTH, "To be able to play the game, you need to transfer", CYN, BLU);
        screen_draw_line(OFFSET_X, OFFSET_Y + 4, WIDTH, "some amount of money first to us :3", CYN, BLU);
        screen_draw_line_decor(OFFSET_X, OFFSET_Y + 7, WIDTH, BLU);

        screen_draw_line_input(OFFSET_X, OFFSET_Y + 6, WIDTH, "Enter the amount of money that you want : $", BLU, BLU);

        bool check = input_number(OFFSET_X, OFFSET_Y + 6, &user_balances);
        if (check == true) break;
    }

    if (user_balances <= 0){
        screen_draw_line(OFFSET_X, OFFSET_Y + 6, WIDTH, " ", BLU, BLU);
        screen_draw_raw(OFFSET_X + 2, OFFSET_Y + 6, RED, "Do you want to borrow "RED"$%-4i? " RST"(Y/n) : ", user_balances);
        SET_OFFSET(OFFSET_X + 39, OFFSET_Y + 6);
    } else {
        screen_draw_line(OFFSET_X, OFFSET_Y + 6, WIDTH, " ", BLU, BLU);
        screen_draw_line_input(OFFSET_X, OFFSET_Y + 6, WIDTH, "Are you sure? (Y/n) : ", MAG, BLU);
    }
    
    bool confirm = input_confirmation();

    if (confirm == false){
        screen_draw_line_center(OFFSET_X, OFFSET_Y + 6, WIDTH, "Transaction cancelled! T-T", RED_BG, RED);
        screen_draw_line_center(OFFSET_X, OFFSET_Y + 7, WIDTH, "Account creation failed!", RED, RED);
        screen_draw_line_decor(OFFSET_X, OFFSET_Y + 9, WIDTH, RED);
        input_continue(OFFSET_X, OFFSET_Y + 8, WIDTH);
        return;
    }

    // Transfer Display
    screen_draw_line_center(OFFSET_X, OFFSET_Y + 6, WIDTH, "Transfering...", GRN, GRN);
    screen_draw_line_decor(OFFSET_X, OFFSET_Y + 8, WIDTH, GRN);
    screen_loading_default(OFFSET_X, OFFSET_Y + 7, WIDTH);

    // Thank you :D
    screen_draw_line_center(OFFSET_X, OFFSET_Y + 6, WIDTH, " Thank you for registering \\(> v <)/", BLU_BG, BLU);
    screen_draw_line_center(OFFSET_X, OFFSET_Y + 7, WIDTH + 10, "For your registration, you will get "GRN"10 free credits!"BLU"", BLU, BLU);
    screen_draw_line_decor(OFFSET_X, OFFSET_Y + 9, WIDTH, BLU);
    input_continue(OFFSET_X, OFFSET_Y + 8, WIDTH);

    // Variable initializations
     database_user_init(username, password, user, user_balances);
}

void ui_leaderboard(){
    screen_clear();
    screen_draw_box_title(OFFSET_X, OFFSET_Y, WIDTH, 3, "Leaderboard", CYN);
    screen_draw_line(OFFSET_X, OFFSET_Y + 1, WIDTH, MAG"Rank"CYN" | "BLU"Name"CYN"                      | "YEL"Gambled"CYN" | "GRN"Balances", MAG, CYN);
    screen_draw_raw(OFFSET_X, OFFSET_Y + 2, CYN, "+------+---------------------------+---------+----------+");

    FILE *database_file = fopen(FILENAME, "rb");
    User *database_user;
    uint8_t i = 1; // We skip the admin
    uint8_t database_Userotal = database_leaderboard(1, 4, database_file, database_user);

    if (database_Userotal == 0){
        screen_draw_line_center(OFFSET_X, OFFSET_Y + 3, WIDTH, "Database doesnt exist yet > A <", RED, RED);
        screen_draw_line_decor(OFFSET_X, OFFSET_Y + 5, WIDTH, YEL);
        input_continue(OFFSET_X, OFFSET_Y + 4, WIDTH);
        return;
    } else if (database_Userotal == 1){
        screen_draw_line_center(OFFSET_X, OFFSET_Y + 3, WIDTH, "No user has been registered yet > A <", RED, RED);
        screen_draw_line_decor(OFFSET_X, OFFSET_Y + 5, WIDTH, YEL);
        input_continue(OFFSET_X, OFFSET_Y + 4, WIDTH);
        return;
    }

    // Convert into array
    database_user = malloc(sizeof(User) * database_Userotal);
    rewind(database_file);
    fread(database_user, sizeof(User), database_Userotal, database_file);

    for (i = 1; i < database_Userotal; i++) {
        screen_draw_line(OFFSET_X, OFFSET_Y + i + 2, WIDTH, " ", CYN, CYN);
        screen_draw_raw(OFFSET_X + 3, OFFSET_Y + i + 2, MAG, "%-2d.", i);
        screen_draw_raw(OFFSET_X + 7, OFFSET_Y + i + 2, CYN, "%s", "|");
        screen_draw_raw(OFFSET_X + 9, OFFSET_Y + i + 2, (database_user[i].banned) ? RED_BG : BLU, "%-25s", database_user[i].username);
        screen_draw_raw(OFFSET_X + 35, OFFSET_Y + i + 2, CYN, "%s", "|");
        screen_draw_raw(OFFSET_X + 39, OFFSET_Y + i + 2, YEL, "%-3li", database_user[i].game_data.counter[3]);
        screen_draw_raw(OFFSET_X + 45, OFFSET_Y + i + 2, CYN, "%s", "|");
        if (database_user[i].game_data.balances <= 0)
            screen_draw_raw(OFFSET_X + 48, OFFSET_Y + i + 2, RED, "$ %-4i", database_user[i].game_data.balances);
        else
            screen_draw_raw(OFFSET_X + 48, OFFSET_Y + i + 2, GRN, "$ %-4i", database_user[i].game_data.balances);
    }

    screen_draw_raw(OFFSET_X,OFFSET_Y + i + 2, CYN, "+------+---------------------------+---------+----------+");
    screen_draw_raw(OFFSET_X,OFFSET_Y + i + 4, YEL, "+-------------------------------------------------------+");
    input_continue(OFFSET_X, OFFSET_Y + i + 3, WIDTH);

    fclose(database_file);
    free(database_user);
}

/* ====================== User Screen ======================= */
void ui_user(User *const user){
    typedef enum {
        SLOT        = 1,
        COIN        = 2,
        RNG         = 3,
        LEADERBOARD = 7,
        ACHIEVEMENT = 8,
        MARKET      = 9,
        EXIT        = 0,
    } MenuUser;
    int32_t input;

    while(RUNNING){
        screen_clear();
        screen_draw_box_title(OFFSET_X, OFFSET_Y, WIDTH, 10, "User Menu", GRN);
        screen_draw_line(OFFSET_X, OFFSET_Y + 1, WIDTH + 5, "Do you love gambling,           ? "BLU"I do and You are in", MAG, GRN);
        screen_draw_line(OFFSET_X, OFFSET_Y + 2, WIDTH, "the right place! Which game do you want to play?", BLU, GRN);
        screen_draw_line_center(OFFSET_X, OFFSET_Y + 3, WIDTH + 18, GRN_BG"[Balances :       ]"RST" "YEL_BG"[Credits :    ]"RST, WHT, GRN);

        screen_draw_raw(OFFSET_X + 24, OFFSET_Y + 1, MAG_BG, "%-10s", user->username);
        if (user->game_data.balances <= 0)
            screen_draw_raw(OFFSET_X + 22, OFFSET_Y + 3, RED_BG, "$%-4i", user->game_data.balances);
        else 
            screen_draw_raw(OFFSET_X + 23, OFFSET_Y + 3, GRN_BG, "$%-4i", user->game_data.balances); 
        screen_draw_raw(OFFSET_X + 42, OFFSET_Y + 3, YEL_BG, "%-3i", user->game_data.credits);

        screen_draw_raw(OFFSET_X, OFFSET_Y + 4, GRN, "+--------------------------+----------------------------+");
        screen_draw_line(OFFSET_X, OFFSET_Y + 5, WIDTH, "1. Slot          [7 7 7] "GRN"| "YEL"7. Leaderboard   [1st 2nd]", GRN, GRN);
        screen_draw_line(OFFSET_X, OFFSET_Y + 6, WIDTH, "2. Coin Toss       [0/1] "GRN"| "BLU"8. Achievement     [+ + +]", LME, GRN);
        screen_draw_line(OFFSET_X, OFFSET_Y + 7, WIDTH, "3. Rand Num Guesser  [?] "GRN"| "GRN"9. Buy Credits         [$]", YEL, GRN);
        screen_draw_line(OFFSET_X, OFFSET_Y + 8, WIDTH + 5, "0. Exit              [X] "GRN"| ", RED, GRN);
        screen_draw_raw(OFFSET_X, OFFSET_Y + 9, GRN, "+--------------------------+----------------------------+");
        screen_draw_line_decor(OFFSET_X, OFFSET_Y + 11, WIDTH, YEL);
        screen_draw_line_input(OFFSET_X, OFFSET_Y + 10, WIDTH, "Your input : ", YEL, YEL);

        bool check = input_number(OFFSET_X, OFFSET_Y + 10, &input);
        if (check == false) {
            screen_draw_line_center(OFFSET_X, OFFSET_Y + 10, WIDTH, "Please input the number correctly >w<", RED_BG, RED);
            screen_draw_line_decor(OFFSET_X, OFFSET_Y + 12, WIDTH, RED);
            input_continue(OFFSET_X, OFFSET_Y + 11, WIDTH); 
            continue;
        }

        switch(input){
            case SLOT: 
                ui_user_slot(user);
                database_update(user);
                break;
            case COIN: 
                ui_user_coin(user);
                database_update(user);
                break;
            case RNG:
                ui_user_rng(user);
                database_update(user);
                break;
            case LEADERBOARD: 
                ui_leaderboard();
                break;
            case ACHIEVEMENT: 
                ui_user_achievement(user);
                break;
            case MARKET:
                ui_user_market(user);
                database_update(user);
                break;
            case EXIT: 
                screen_clear();

                SET_CURSOR(CURSOR_HIDE);
                if (user->game_data.balances < 0) {
                    screen_draw_box_title(OFFSET_X, OFFSET_Y + 2, WIDTH, 5, "Warning!", RED);
                    screen_draw_line_center(OFFSET_X, OFFSET_Y + 3, WIDTH, "You are in debt for       ", RED, RED);
                    screen_draw_raw(OFFSET_X + 35, OFFSET_Y + 3, RED_BG, "$%-4i", user->game_data.balances);
                    screen_draw_line_center(OFFSET_X, OFFSET_Y + 4, WIDTH, "Please pay it or debt collector will haunt you >w<!!!", RED_BG, RED);
                    screen_draw_line_center(OFFSET_X, OFFSET_Y + 5, WIDTH, "Exiting...    ", MAG_BG, MAG);
                    screen_loading_mini(OFFSET_X + 32, OFFSET_Y + 5);
                } else {
                    screen_draw_box_title(OFFSET_X, OFFSET_Y + 2, WIDTH, 4, "Goodbye!", MAG);
                    screen_draw_line_center(OFFSET_X, OFFSET_Y + 3, WIDTH, "Thank you for Playing!", MAG, MAG);
                    screen_draw_line_center(OFFSET_X, OFFSET_Y + 4, WIDTH, "Exiting...    ", MAG_BG, MAG);
                    screen_loading_mini(OFFSET_X + 32, OFFSET_Y + 4);
                }
                SET_CURSOR(CURSOR_SHOW);

                return;
            default:
                screen_draw_line_center(OFFSET_X, OFFSET_Y + 10, WIDTH, "Please input correctly >w<", RED_BG, RED);
                screen_draw_line_decor(OFFSET_X, OFFSET_Y + 12, WIDTH, RED);
                input_continue(OFFSET_X, OFFSET_Y + 11, WIDTH); 
                break;
        }
    }   
}

void ui_user_slot(User *const user){
    char *slot_icons[10] = {
        MAG"🍇"RST, GRN"🍉"RST, MAG"🍆"RST, CYN"🍪"RST, RED"🐶"RST, 
        YEL"🐱"RST, CYN"🐻"RST, BLU"🐟️"RST, RED"🍎"RST, GRN"🐢"RST
    };
    char *slot_value_icons[3];
    uint8_t slot_value[3];

    screen_clear();
    screen_draw_box_title(OFFSET_X, OFFSET_Y, WIDTH, 5, "Madoka Slot Machine !!!", BLU);
    screen_draw_line(OFFSET_X, OFFSET_Y + 1, WIDTH, "Pretty much just your regular slot game", BLU, BLU);
    screen_draw_line(OFFSET_X, OFFSET_Y + 2, WIDTH, "Win condition : Get atleast two matching pictures", YEL, BLU);
    screen_draw_line(OFFSET_X, OFFSET_Y + 3, WIDTH, "Simple right?", BLU, BLU);
    screen_draw_line_decor(OFFSET_X, OFFSET_Y + 6, WIDTH, YEL);
    input_continue(OFFSET_X, OFFSET_Y + 5, WIDTH);
    
    while(RUNNING){
        screen_clear();
        screen_draw_box_title(OFFSET_X, OFFSET_Y, WIDTH, 7, "Madoka Slot Machine !!!", BLU);
        screen_draw_line_center(OFFSET_X, OFFSET_Y + 1, WIDTH, "Credit(s) left = ", YEL, BLU);
        screen_draw_raw(OFFSET_X + 37, OFFSET_Y + 1, YEL, "%i", user->game_data.credits);
        
        if (user->game_data.credits == 0){
            screen_draw_raw(OFFSET_X + 23, OFFSET_Y + 3, CYN, "[-]  [-]  [-]");
            screen_draw_line_center(OFFSET_X, OFFSET_Y + 5, WIDTH + 10, "Too bad, but try"MAG_BG" KEEP GAMBLING "RED_BG"again!", RED_BG, RED);
            screen_draw_line_decor(OFFSET_X, OFFSET_Y + 8, WIDTH, YEL);
            input_continue(OFFSET_X, OFFSET_Y + 7, WIDTH);
            break;
        } else {
            user->game_data.credits--;
            user->game_data.counter[0]++;
        }
        // Rollcake
        for (uint8_t i = 0; i < 3; i++){
            if (cheat[0] == true){
                slot_value[i] = 7;
            } else
                slot_value[i] = rand() % (9); 
            slot_value_icons[i] = slot_icons[slot_value[i]];             
        }
        // Display
        screen_draw_raw(OFFSET_X + 21, OFFSET_Y + 3, CYN, "[%s]  [%s]  [%s]", 
            slot_value_icons[0], slot_value_icons[1], slot_value_icons[2] );

        // If you get le fishe [🐟️] [🐟️] [🐟️]
        if (slot_value[0] == 7 && 
            slot_value[1] == 7 && 
            slot_value[2] == 7) {
            screen_draw_line_center(OFFSET_X, OFFSET_Y + 4, WIDTH, BLU"🐟️ 🐟️ 🐟️"GRN_BG" Jackpot! "BLU"🐟️ 🐟️ 🐟️", GRN_BG, GRN);
            screen_draw_line_center(OFFSET_X, OFFSET_Y + 5, WIDTH, "The fish council just gives you $500", GRN_BG, GRN);
            user->game_data.balances += 500;
        }
        // If you get all the same pictures
        else if (slot_value[0] == slot_value[1] && 
                 slot_value[0] == slot_value[2] && 
                 slot_value[1] == slot_value[2]) {
            screen_draw_line_center(OFFSET_X, OFFSET_Y + 5, WIDTH, "Congrats, you just win $50!", GRN_BG, GRN);
            user->game_data.balances += 50;
        }
        // If you get atleast 2 pictures matching
        else if (slot_value[0] == slot_value[1] || 
                 slot_value[0] == slot_value[2] || 
                 slot_value[1] == slot_value[2]) {
            screen_draw_line_center(OFFSET_X, OFFSET_Y + 5, WIDTH, "Congrats, you just win $20!", GRN_BG, GRN);
            user->game_data.balances += 20;
        }

        else screen_draw_line_center(OFFSET_X, OFFSET_Y + 5, WIDTH, "Try again!", RED_BG, RED);
        
        screen_draw_line_decor(OFFSET_X, OFFSET_Y + 8, WIDTH, YEL);
        screen_draw_line_input(OFFSET_X, OFFSET_Y + 7, WIDTH, "Continue playing!? (Y/n) ", YEL, YEL);

        bool confirm = input_confirmation();
        if (confirm == false) {
            user->game_data.credits++;
            user->game_data.counter[0]--;
            break;
        }
    }
}

void ui_user_coin(User *const user){
    uint8_t coin_throw; 
    int32_t coin_guess;
    char *coin_icons[] = {YEL"😼"RST, RED"🐶"RST};

    screen_clear();
    screen_draw_box_title(OFFSET_X, OFFSET_Y, WIDTH, 5, "Coin Tosser !!!", BLU);
    screen_draw_line(OFFSET_X, OFFSET_Y + 1, WIDTH, "The classic coin toss", BLU, BLU);
    screen_draw_line(OFFSET_X, OFFSET_Y + 2, WIDTH, "Win condition : Just guess Head or tails", YEL, BLU);
    screen_draw_line(OFFSET_X, OFFSET_Y + 3, WIDTH, "Simple right?", BLU, BLU);
    screen_draw_line_decor(OFFSET_X, OFFSET_Y + 6, WIDTH, YEL);
    input_continue(OFFSET_X, OFFSET_Y + 5, WIDTH);

    while(RUNNING){
        screen_clear();
        screen_draw_box_title(OFFSET_X, OFFSET_Y, WIDTH, 7, "Coin Tosser !!!", BLU);
        screen_draw_line_center(OFFSET_X, OFFSET_Y + 1, WIDTH, "Credit(s) left = ", YEL, BLU);
        screen_draw_raw(OFFSET_X + 37, OFFSET_Y + 1, YEL, "%i", user->game_data.credits);
        
        if (user->game_data.credits == 0){
            screen_draw_line_center(OFFSET_X, OFFSET_Y + 3, WIDTH, "[-]", CYN, CYN);
            screen_draw_line_center(OFFSET_X, OFFSET_Y + 5, WIDTH + 10, "Too bad, but try"MAG_BG" KEEP GAMBLING "RED_BG"again!", RED_BG, RED);
            screen_draw_line_decor(OFFSET_X, OFFSET_Y + 8, WIDTH, YEL);
            input_continue(OFFSET_X, OFFSET_Y + 7, WIDTH);
            break;
        } else {
            user->game_data.credits--;
            user->game_data.counter[0]++;
        }

        screen_draw_line_center(OFFSET_X, OFFSET_Y + 3, WIDTH, "[#]", CYN, CYN);
        screen_draw_line_center(OFFSET_X, OFFSET_Y + 5, WIDTH, "Input your guess!", CYN, CYN);
        screen_draw_line_decor(OFFSET_X, OFFSET_Y + 8, WIDTH, CYN);
        screen_draw_line_input(OFFSET_X, OFFSET_Y + 7, WIDTH + 4, "[Head 😼 (0) | Tail 🐶 (1)] : ", YEL, CYN);

        bool check = input_number(OFFSET_X, OFFSET_Y + 7, &coin_guess);

        if (check == false) {
            user->game_data.credits++;
            user->game_data.counter[1]--;
            continue;
        } else if (coin_guess != 0 && coin_guess != 1){
            user->game_data.credits++;
            user->game_data.counter[1]--;

            screen_draw_line_center(OFFSET_X, OFFSET_Y + 5, WIDTH, "Please input correctly >w< !", RED_BG, RED);
            screen_draw_line_decor(OFFSET_X, OFFSET_Y + 8, WIDTH, YEL);
            screen_draw_line_input(OFFSET_X, OFFSET_Y + 7, WIDTH, "Continue playing!? (Y/n) ", YEL, YEL);

            bool confirm = input_confirmation();
            if (confirm == false) break;
            else continue;
        }

        // Reveal the guess
        if (cheat[1] == true)
            coin_throw = coin_guess;
        else
            coin_throw = rand() % 2;

        char *coin_throw_icons  = coin_icons[coin_throw];

        screen_draw_raw(OFFSET_X + 26, OFFSET_Y + 3, YEL, "[%s"YEL"]", coin_throw_icons);

        if (coin_guess == coin_throw){
            screen_draw_line_center(OFFSET_X, OFFSET_Y + 5, WIDTH, "You just win $20 !!!", GRN_BG, GRN);
            user->game_data.balances += 20;
        } else {
            screen_draw_line_center(OFFSET_X, OFFSET_Y + 5, WIDTH + 10, "Too bad, but try"MAG_BG" KEEP GAMBLING "RED_BG"again!", RED_BG, RED);
        }

        screen_draw_line_decor(OFFSET_X, OFFSET_Y + 8, WIDTH, YEL);
        screen_draw_line_input(OFFSET_X, OFFSET_Y + 7, WIDTH, "Continue playing!? (Y/n) ", YEL, YEL);

        bool confirm = input_confirmation();
        if (confirm == false) {
            user->game_data.credits++;
            user->game_data.counter[0]--;
            break;
        }
    }
}

void ui_user_rng(User *const user){
    int32_t number_throw;
    int32_t number_guess;

    screen_clear();
    screen_draw_box_title(OFFSET_X, OFFSET_Y, WIDTH, 5, "Random Number Guesser !!!", BLU);
    screen_draw_line(OFFSET_X, OFFSET_Y + 1, WIDTH, "RNG God", BLU, BLU);
    screen_draw_line(OFFSET_X, OFFSET_Y + 2, WIDTH, "Win condition : Guess a number between 0-99", YEL, BLU);
    screen_draw_line(OFFSET_X, OFFSET_Y + 3, WIDTH, "Simple right?", BLU, BLU);
    screen_draw_line_decor(OFFSET_X, OFFSET_Y + 6, WIDTH, YEL);
    input_continue(OFFSET_X, OFFSET_Y + 5, WIDTH);

    while(RUNNING){
        screen_clear();
        screen_draw_box_title(OFFSET_X, OFFSET_Y, WIDTH, 7, "Random Number Guesser !!!", BLU);
        screen_draw_line_center(OFFSET_X, OFFSET_Y + 1, WIDTH, "Credit(s) left = ", YEL, BLU);
        screen_draw_raw(OFFSET_X + 37, OFFSET_Y + 1, YEL, "%i", user->game_data.credits);
        
        if (user->game_data.credits == 0){
            screen_draw_line_center(OFFSET_X, OFFSET_Y + 3, WIDTH, "[-]", BLU, BLU);
            screen_draw_line_center(OFFSET_X, OFFSET_Y + 5, WIDTH + 10, "Too bad, but try"MAG_BG" KEEP GAMBLING "RED_BG"again!", RED_BG, RED);
            screen_draw_line_decor(OFFSET_X, OFFSET_Y + 8, WIDTH, YEL);
            input_continue(OFFSET_X, OFFSET_Y + 7, WIDTH);
            break;
        } else {
            user->game_data.credits--;
            user->game_data.counter[0]++;
        }

        screen_draw_line_center(OFFSET_X, OFFSET_Y + 3, WIDTH, "[##]", BLU, BLU);
        screen_draw_line_center(OFFSET_X, OFFSET_Y + 5, WIDTH, "Input your guess!", BLU, BLU);
        screen_draw_line_decor(OFFSET_X, OFFSET_Y + 8, WIDTH, BLU);
        screen_draw_line_input(OFFSET_X, OFFSET_Y + 7, WIDTH, "(0 - 99): ", YEL, BLU);

        bool check = input_number(OFFSET_X, OFFSET_Y + 7, &number_guess);

        if (check == false) {
            user->game_data.credits++;
            user->game_data.counter[1]--;
            continue;
        } else if (number_guess < 0 && number_guess > 99){
            user->game_data.credits++;
            user->game_data.counter[1]--;

            screen_draw_line_center(OFFSET_X, OFFSET_Y + 4, WIDTH, "Please input correctly >w< !", RED_BG, RED);
            screen_draw_line_decor(OFFSET_X, OFFSET_Y + 8, WIDTH, YEL);
            screen_draw_line_input(OFFSET_X, OFFSET_Y + 7, WIDTH, "Continue playing!? (Y/n) ", YEL, YEL);

            bool confirm = input_confirmation();
            if (confirm == false) break;
            else continue;
        }

        // Reveal the guess
        if (cheat[2] == true)
            number_throw = number_guess;
        else
            number_throw = rand() % 100;

        screen_draw_raw(OFFSET_X + 26, OFFSET_Y + 3, YEL, "[%2i"YEL"]", number_throw);

        // If they are the same
        if (number_guess == number_throw){
            screen_draw_line_center(OFFSET_X, OFFSET_Y + 5, WIDTH, "NO WAY !!! you just win $500 !!!", GRN_BG, GRN);
            user->game_data.balances += 500;
        }
        // If +- 5 differences
        else if (number_guess >= number_throw - 5 && 
                 number_guess <= number_throw + 5) {
            screen_draw_line_center(OFFSET_X, OFFSET_Y + 4, WIDTH, "You are just [  ] number away from your guess [  ]", MAG, BLU);
            screen_draw_raw(OFFSET_X + 16, OFFSET_Y + 4, RED, "[%2i"RED"]", number_throw - number_guess);
            screen_draw_raw(OFFSET_X + 49, OFFSET_Y + 4, BLU, "[%2i"BLU"]", number_guess);
            screen_draw_line_center(OFFSET_X, OFFSET_Y + 5, WIDTH, "You just win $50 !!!", GRN_BG, GRN);
            user->game_data.balances += 50;
        } 
        // If miss
        else {
            screen_draw_line_center(OFFSET_X, OFFSET_Y + 5, WIDTH + 10, "Too bad, but try"MAG_BG" KEEP GAMBLING "RED_BG"again!", RED_BG, RED);
        }

        screen_draw_line_decor(OFFSET_X, OFFSET_Y + 8, WIDTH, YEL);
        screen_draw_line_input(OFFSET_X, OFFSET_Y + 7, WIDTH, "Continue playing!? (Y/n) ", YEL, YEL);

        bool confirm = input_confirmation();
        if (confirm == false) {
            user->game_data.credits++;
            user->game_data.counter[0]--;
            break;
        }
    }
}

void ui_user_market(User *const user){
    int32_t credits_add, credits_cost, balances_difference, balances_change;

    screen_clear();
    screen_draw_box_title(OFFSET_X, OFFSET_Y, WIDTH, 5, "Market !!!", GRN);
    screen_draw_line(OFFSET_X, OFFSET_Y + 1, WIDTH, "Uhmmm... hi > w < !!!", BLU, GRN);
    screen_draw_line(OFFSET_X, OFFSET_Y + 2, WIDTH, "Hh-how much do you want to pay T-T?", BLU, GRN);
    screen_draw_line_center(OFFSET_X, OFFSET_Y + 3, WIDTH, "> A <", MAG, GRN);
    screen_draw_line_decor(OFFSET_X, OFFSET_Y + 6, WIDTH, GRN);
    screen_draw_line_input(OFFSET_X, OFFSET_Y + 5, WIDTH, "Credits (1 = $10) : ", GRN, GRN);

    bool check = input_number(OFFSET_X, OFFSET_Y + 5, &credits_add);

    if (check == false) {
        screen_draw_line_center(OFFSET_X, OFFSET_Y + 5, WIDTH, "Please input correctly >w< !", RED_BG, RED);
        screen_draw_line_decor(OFFSET_X, OFFSET_Y + 7, WIDTH, YEL);
        input_continue(OFFSET_X, OFFSET_Y + 6, WIDTH);
        return;
    }

    credits_cost    = 10 * credits_add;
    balances_change = user->game_data.balances - credits_cost;

    if (credits_add < 0) {
        screen_draw_line_center(OFFSET_X, OFFSET_Y + 5, WIDTH + 3, "Nuh uh... Too bad you can't do that (·ω·)", RED_BG, RED);
        screen_draw_line_decor(OFFSET_X, OFFSET_Y + 7, WIDTH, YEL);
        input_continue(OFFSET_X, OFFSET_Y + 6, WIDTH);
        return;
    } else if (credits_add == 0) {
        screen_draw_line_center(OFFSET_X, OFFSET_Y + 5, WIDTH, "Transaction cancelled! > v <", RED_BG, RED);
        screen_draw_line_decor(OFFSET_X, OFFSET_Y + 7, WIDTH, YEL);
        input_continue(OFFSET_X, OFFSET_Y + 6, WIDTH);
        return;
    } else if (credits_add < 0) {
        if (user->game_data.balances >= 0) 
            balances_difference = credits_cost -  user->game_data.balances;
        else 
            balances_difference = credits_cost; // Debt
        screen_draw_raw(OFFSET_X + 2, OFFSET_Y + 5, RED, "Do you want to borrow $%i", balances_difference);
    } else 
        screen_draw_line_input(OFFSET_X, OFFSET_Y + 5, WIDTH, "Are you sure? (Y/n) ", MAG, RED);

    bool confirm = input_confirmation();
    
    if (confirm == false) {
        screen_draw_line_center(OFFSET_X, OFFSET_Y + 5, WIDTH, "Transaction cancelled! > v <", RED_BG, RED);
        screen_draw_line_decor(OFFSET_X, OFFSET_Y + 7, WIDTH, YEL);
        input_continue(OFFSET_X, OFFSET_Y + 6, WIDTH);
        return;
    }

    screen_draw_line_center(OFFSET_X, OFFSET_Y + 5, WIDTH, "Transfering...", GRN, GRN);
    screen_draw_line_decor(OFFSET_X, OFFSET_Y + 7, WIDTH, GRN);
    screen_loading_default(OFFSET_X, OFFSET_Y + 6, WIDTH);
    user->game_data.credits += credits_add;
    user->game_data.balances-= 10 * credits_add;

    screen_draw_line_center(OFFSET_X, OFFSET_Y + 5, WIDTH, "Money transfered! Current balances : $", GRN, GRN);
    screen_draw_raw(OFFSET_X + 47, OFFSET_Y + 5, (user->game_data.balances <= 0) ? RED : GRN, "%i", user->game_data.balances);
    screen_draw_line_center(OFFSET_X, OFFSET_Y + 6, WIDTH, "Erm... please gamble responsibly T-T...", GRN_BG, GRN);
    screen_draw_line_decor(OFFSET_X, OFFSET_Y + 8, WIDTH, GRN);
    input_continue(OFFSET_X, OFFSET_Y + 7, WIDTH);

    return;
}

void ui_user_achievement(User *const user){
    uint8_t i = 0;
    bool    track      = false;
    uint8_t counter[]  = {10, 50, 100};
    char    *colors[]  = {GRN, MAG, RED};
    char    *game[]    = {"Slot", "Coin Toss", "Random Number Guesser"};

    screen_clear();
    screen_draw_box_title(OFFSET_X, OFFSET_Y, WIDTH, 3, "Achievement", MAG);

    for(i=0; i < 3; i++){
        for(uint8_t j=0; i < 3; i++){
            if (user->game_data.counter[i] >= counter[j]) {
                screen_draw_line(OFFSET_X, OFFSET_Y + i + 1, WIDTH, "", MAG, MAG);
                screen_draw_raw(OFFSET_X + 2, OFFSET_Y + i + 1, BLU, 
                    "Play %s%s%s for %s%i times",
                    BLU, game[i], RST, colors[j], counter[j]
                );
                track = true;
            }
        }
    }

    if (user->game_data.balances <= -100 || user->game_data.persist[0] == true) {
        screen_draw_line(OFFSET_X, OFFSET_Y + i - 1, WIDTH, "", MAG, MAG);
        screen_draw_raw(OFFSET_X + 2, OFFSET_Y + i - 1, BLU,
            "%sDebt Collector's Ghost%s : In %sdebt%s for more than %s$100",
            MAG, RST, RED, RST, RED
        );
        user->game_data.persist[0] = true;
        track = true;
        i++;
    }
    if (user->game_data.balances >= 1000 && user->game_data.counter[3] >= 100 || user->game_data.persist[1] == true) {
        screen_draw_line(OFFSET_X, OFFSET_Y + i - 1, WIDTH, "", MAG, MAG);
        screen_draw_raw(OFFSET_X + 2, OFFSET_Y + i - 1, BLU,
            "%sMasterful Gambit%s : Earn %s$1000",
            MAG, RST, GRN
        );
        user->game_data.persist[1] = true;
        track = true;
        i++;
    }
    if (user->game_data.counter[3] >= 100 || user->game_data.persist[2] == true) {
        screen_draw_line(OFFSET_X, OFFSET_Y + i - 1, WIDTH, "", MAG, MAG);
        screen_draw_raw(OFFSET_X + 2, OFFSET_Y + i - 1, BLU,      
            "%sA True Gambler Mindset%s : Play for more than %s100 times",
            MAG, RST, GRN
        );
        user->game_data.persist[2] = true;
        track = true;
        i++;
    }

    if (track == None) screen_draw_line_center(OFFSET_X, OFFSET_Y + 1, WIDTH, "There's no achievement yet ('A`)", RED_BG, RED);

    screen_draw_line_decor(OFFSET_X, OFFSET_Y + i - 1, WIDTH, YEL);
    screen_draw_line_decor(OFFSET_X, OFFSET_Y + i + 1, WIDTH, YEL);
    input_continue(OFFSET_X, OFFSET_Y + i, WIDTH);
}

// /* ===================== Admin Screen ======================= */
void ui_admin(){
    typedef enum {
        VIEW    = 1,
        BAN     = 2,
        UNBAN   = 3,
        REMOVE  = 4,
        EXPORT  = 5,
        CHEAT   = 8,
        ASCII   = 9,
        EXIT    = 0
    } MenuAdmin;
    int32_t input;

    while(RUNNING){
        screen_clear();
        screen_draw_box_title(OFFSET_X, OFFSET_Y, WIDTH, HEIGHT - 1, "Admin Menu", CYN);
        screen_draw_line(OFFSET_X, OFFSET_Y + 1, WIDTH, "Hello Mr. Admin!", MAG, CYN);
        screen_draw_line(OFFSET_X, OFFSET_Y + 2, WIDTH, "What do you want to select? >///<", WHT, CYN);
        screen_draw_raw(OFFSET_X, OFFSET_Y + 3, CYN, "+-------------------------+-----------------------------+");
        screen_draw_line(OFFSET_X, OFFSET_Y + 4, WIDTH, YEL"1. View registered user "CYN"| "RED"4. Remove user             ", GRN, CYN);
        screen_draw_line(OFFSET_X, OFFSET_Y + 5, WIDTH, PNK"2. Ban user             "CYN"| "CYN"5. Export into text (.yaml)", LME, CYN);
        screen_draw_line(OFFSET_X, OFFSET_Y + 6, WIDTH, BLU"3. Unban user           "CYN"| "GRN"8. Cheat                   ", YEL, CYN);
        screen_draw_line(OFFSET_X, OFFSET_Y + 7, WIDTH, RED"0. Exit                 "CYN"| "MAG"9. ASCII art               ", RED, CYN);
        screen_draw_raw(OFFSET_X, OFFSET_Y + 8, CYN, "+-------------------------+-----------------------------+");
        screen_draw_line_decor(OFFSET_X, OFFSET_Y + 10, WIDTH, YEL);
        screen_draw_line_input(OFFSET_X, OFFSET_Y + 9, WIDTH, "Your input : ", YEL, YEL);

        bool check = input_number(OFFSET_X, OFFSET_Y + 9, &input);

        if (check == false) {
            screen_draw_line_center(OFFSET_X, OFFSET_Y + 9, WIDTH, "Please input the number correctly >w<", RED_BG, RED);
            screen_draw_line_decor(OFFSET_X, OFFSET_Y + 11, WIDTH, RED);
            input_continue(OFFSET_X, OFFSET_Y + 10, WIDTH); 
            continue;
        }

        switch(input){
            case VIEW :
                ui_admin_view(false);
                break;
            case BAN :
                ui_admin_ban();
                break;
            case UNBAN :
                ui_admin_unban();
                break;
            case REMOVE :
                ui_admin_delete();
                break;
            case EXPORT :
                ui_admin_export();
                break;
            case CHEAT :
                ui_cheat();
                break;
            case ASCII :
                ui_fun_ascii();
                break;
            case EXIT :
                screen_clear();
                screen_draw_box_title(OFFSET_X, OFFSET_Y + 2, WIDTH, 4, "Goodbye!", MAG);
                screen_draw_line_center(OFFSET_X, OFFSET_Y + 3, WIDTH, "Thank you for administrating us!", MAG, MAG);
                screen_draw_line_center(OFFSET_X, OFFSET_Y + 4, WIDTH, "Exiting...    ", MAG_BG, MAG);
                screen_loading_mini(OFFSET_X + 32, OFFSET_Y + 4);
                return;
            default :
                screen_draw_line_center(OFFSET_X, OFFSET_Y + 9, WIDTH, "Please input the number correctly >w<", RED_BG, RED);
                screen_draw_line_decor(OFFSET_X, OFFSET_Y + 11, WIDTH, RED);
                input_continue(OFFSET_X, OFFSET_Y + 10, WIDTH); 
                break;
        }
    }
}

uint8_t ui_admin_view(const bool called){
    FILE* database_file = fopen("database.dat", "rb");
    User* database_user = malloc(sizeof(User));
    uint8_t i = 1;

    if (called == false){
        screen_clear();
        screen_draw_box_title(OFFSET_X, OFFSET_Y, WIDTH, 3, "List of users", CYN);
    }

    // Check whether the file exist or not
    bool exist = input_filechecker(OFFSET_X, OFFSET_Y + 1, database_file);

    if (exist == None && called == false){
        screen_draw_line_decor(OFFSET_X, OFFSET_Y + 4, WIDTH, YEL);
        input_continue(OFFSET_X ,OFFSET_Y + 3, WIDTH);
        return 0;
    }

    screen_draw_line(OFFSET_X, OFFSET_Y + 1, WIDTH, MAG" No "CYN" | "BLU"Name"CYN"                      | "YEL"Gambled"CYN" | "GRN"Balances", MAG, CYN);
    while(fread(database_user, sizeof(User), 1, database_file) == 1){
        screen_draw_line(OFFSET_X, OFFSET_Y + i + 2, WIDTH, "", CYN, CYN);
        screen_draw_raw(OFFSET_X + 3, OFFSET_Y + i + 2, MAG, "%-2d.", i);
        screen_draw_raw(OFFSET_X + 7, OFFSET_Y + i + 2, CYN, "%s", "|");
        screen_draw_raw(OFFSET_X + 9, OFFSET_Y + i + 2, (database_user->banned) ? RED_BG : BLU, "%-25s", database_user->username);
        screen_draw_raw(OFFSET_X + 35, OFFSET_Y + i + 2, CYN, "%s", "|");
        screen_draw_raw(OFFSET_X + 39, OFFSET_Y + i + 2, YEL, "%-3li", database_user->game_data.counter[3]);
        screen_draw_raw(OFFSET_X + 45, OFFSET_Y + i + 2, CYN, "%s", "|");
        if (database_user[i].game_data.balances <= 0)
            screen_draw_raw(OFFSET_X + 48, OFFSET_Y + i + 2, RED, "$ %-4i", database_user->game_data.balances);
        else
            screen_draw_raw(OFFSET_X + 48, OFFSET_Y + i + 2, GRN, "$ %-4i", database_user->game_data.balances);

        usleep(MILISECONDS * 70);
        fflush(stdout);
        i++;
    }

    free(database_user);
    fclose(database_file);

    if (called == false) {
        screen_draw_line_decor(OFFSET_X, OFFSET_Y + i + 2, WIDTH, YEL);
        screen_draw_line_decor(OFFSET_X, OFFSET_Y + i + 4, WIDTH, YEL);
        input_continue(OFFSET_X ,OFFSET_Y + i + 3, WIDTH);
    }

    return i;
}

void ui_admin_ban(){
    char *username;
    char *message;

    screen_clear();
    screen_draw_box_title(OFFSET_X, OFFSET_Y, WIDTH, 3, "Banhammer", RED);
    uint8_t i = ui_admin_view(true);
    screen_draw_line_decor(OFFSET_X, OFFSET_Y + i + 2, WIDTH, RED);
    screen_draw_line_decor(OFFSET_X, OFFSET_Y + i + 4, WIDTH, RED);
    screen_draw_line_input(OFFSET_X, OFFSET_Y + i + 3, WIDTH, "Which user do you want to be banned? ", RED, RED);
    
    username = input_string();

    FILE* database_file = fopen("database.dat", "rb+");
    User* database = malloc(sizeof(User));
    bool found = false;

    while(fread(database, sizeof(User), 1, database_file) == 1){
        if (strcmp(username, "Admin") == 0){
            found = true;

            screen_draw_line_center(OFFSET_X, OFFSET_Y + i + 3, WIDTH, "Nuh uh! you can't ban yourself !!!", RED_BG, RED);
            screen_draw_line_decor(OFFSET_X, OFFSET_Y + i + 5, WIDTH, YEL);
            input_continue(OFFSET_X ,OFFSET_Y + i + 4, WIDTH);
            break;
        } else if (strcmp(username, database->username) == 0){
            found = true;

            screen_draw_line_input(OFFSET_X, OFFSET_Y + i + 3, WIDTH, "Reason ?  ", RED, RED);
            message = input_string();

            database->banned = true;
            strcpy(database->message, message);

            screen_draw_line_center(OFFSET_X, OFFSET_Y + i + 3, WIDTH, "Banning...", RED_BG, RED);
            screen_draw_line_decor(OFFSET_X, OFFSET_Y + i + 5, WIDTH, RED);
            screen_loading_nuke(OFFSET_X, OFFSET_Y + i + 4, WIDTH);
            screen_draw_line_center(OFFSET_X, OFFSET_Y + i + 3, WIDTH, "Successfully banned ", RED_BG, RED);
            screen_draw_raw(OFFSET_X + 38, OFFSET_Y + i + 3, MAG_BG, "%s", username);
            input_continue(OFFSET_X, OFFSET_Y + i + 4, WIDTH);

            fseek(database_file, -sizeof(User), SEEK_CUR);
            fwrite(database, sizeof(User), 1, database_file); 
            free(message);
            break;
        }
    }
    if (found == false){
        screen_draw_line_center(OFFSET_X, OFFSET_Y + i + 3, WIDTH, "User not found!", RED_BG, RED);
        screen_draw_line_decor(OFFSET_X, OFFSET_Y + i + 5, WIDTH, YEL);
        input_continue(OFFSET_X ,OFFSET_Y + i + 4, WIDTH);
    }

    free(database);
    free(username);
    fclose(database_file);
}

void ui_admin_unban(){
    char *username;
    char *emoji = "😇";

    screen_clear();
    screen_draw_box_title(OFFSET_X, OFFSET_Y, WIDTH, 3, "  Repentance  ", BLU);
    screen_draw_raw(OFFSET_X + 20, OFFSET_Y, BLU, "%s", emoji);
    screen_draw_raw(OFFSET_X + 33, OFFSET_Y, BLU, "%s", emoji);
    uint8_t i = ui_admin_view(true);
    screen_draw_line_decor(OFFSET_X, OFFSET_Y + i + 2, WIDTH, BLU);
    screen_draw_line_decor(OFFSET_X, OFFSET_Y + i + 4, WIDTH, BLU);
    screen_draw_line_input(OFFSET_X, OFFSET_Y + i + 3, WIDTH, "Which user do you want to be unbanned? ", BLU, BLU);

    username = input_string();

    FILE* database_file = fopen("database.dat", "rb+");
    User* database = malloc(sizeof(User));
    bool found = false;

    while(fread(database, sizeof(User), 1, database_file) == 1){
        if (strcmp(username, "Admin") == 0){
            found = true;

            screen_draw_line_center(OFFSET_X, OFFSET_Y + i + 3, WIDTH, "You already have the immunity !!!", RED_BG, RED);
            screen_draw_line_decor(OFFSET_X, OFFSET_Y + i + 5, WIDTH, YEL);
            input_continue(OFFSET_X ,OFFSET_Y + i + 4, WIDTH);
            break;
        } else if (strcmp(username, database->username) == 0){
            found = true;
            database->banned = false;            
            
            screen_draw_line_center(OFFSET_X, OFFSET_Y + i + 3, WIDTH, "Successfully unbanned ", BLU_BG, BLU);
            screen_draw_raw(OFFSET_X + 40, OFFSET_Y + i + 3, MAG_BG, "%s", username);
            screen_draw_line_decor(OFFSET_X, OFFSET_Y + i + 5, WIDTH, YEL);
            input_continue(OFFSET_X ,OFFSET_Y + i + 4, WIDTH);
            
            fseek(database_file, -sizeof(User), SEEK_CUR);
            fwrite(database, sizeof(User), 1, database_file); 
            break;
        }
    }
    if (!found){
        screen_draw_line_center(OFFSET_X, OFFSET_Y + i + 3, WIDTH, "User not found!", RED_BG, RED);
        screen_draw_line_decor(OFFSET_X, OFFSET_Y + i + 5, WIDTH, YEL);
        input_continue(OFFSET_X ,OFFSET_Y + i + 4, WIDTH);
    }

    free(database);
    free(username);
    fclose(database_file);
}

void ui_admin_delete(){
    char *username;

    screen_clear();
    screen_draw_box_title(OFFSET_X, OFFSET_Y, WIDTH, 3, "Nuclear Missile Launch", RED);
    uint8_t i = ui_admin_view(true);
    screen_draw_line_decor(OFFSET_X, OFFSET_Y + i + 2, WIDTH, RED);
    screen_draw_line_decor(OFFSET_X, OFFSET_Y + i + 4, WIDTH, RED);
    screen_draw_line_input(OFFSET_X, OFFSET_Y + i + 3, WIDTH, "Which one do you want to nuke? ", RED, RED);

    username = input_string();

    FILE* database_file = fopen("database.dat", "rb+");
    FILE* database_new = fopen("database.dat.temp", "wb");
    User* database = malloc(sizeof(User));
    bool found = false;

    while(fread(database, sizeof(User), 1, database_file) == 1){
        if (strcmp(username, "Admin") == 0){
            found = true;

            screen_draw_line_center(OFFSET_X, OFFSET_Y + i + 3, WIDTH, "Nuh uh! you can't delete yourself !!!", RED_BG, RED);
            screen_draw_line_decor(OFFSET_X, OFFSET_Y + i + 5, WIDTH, YEL);
            input_continue(OFFSET_X ,OFFSET_Y + i + 4, WIDTH);

            free(database);
            free(username);
            fclose(database_new);
            fclose(database_file);
            remove("database.dat.temp");
            return;

        } else if (strcmp(username, database->username) == 0){
            found = true;
            continue;
        }
        fwrite(database, sizeof(User), 1, database_new);
    }
    if (found == false){   
        screen_draw_line_center(OFFSET_X, OFFSET_Y + i + 3, WIDTH, "User not found!", RED_BG, RED);
        screen_draw_line_decor(OFFSET_X, OFFSET_Y + i + 5, WIDTH, YEL);
        input_continue(OFFSET_X ,OFFSET_Y + i + 4, WIDTH);
    } else {
        screen_draw_line_input(OFFSET_X, OFFSET_Y + i + 3, WIDTH, "Are you really sure? (Y/n) ", RED, RED);
        bool confirm = input_confirmation();
        if (confirm == false){
            screen_draw_line_center(OFFSET_X, OFFSET_Y + i + 3, WIDTH, "Cancelling the action!", RED_BG, RED);
            screen_draw_line_decor(OFFSET_X, OFFSET_Y + i + 5, WIDTH, RED);
            input_continue(OFFSET_X, OFFSET_Y + i + 4, WIDTH);

            free(database);
            free(username);
            fclose(database_new);
            fclose(database_file);
            remove("database.dat.temp");
            return;
        }
        screen_draw_line_center(OFFSET_X, OFFSET_Y + i + 3, WIDTH, "Deleting...", RED_BG, RED);
        screen_draw_line_decor(OFFSET_X, OFFSET_Y + i + 5, WIDTH, RED);
        screen_loading_nuke(OFFSET_X, OFFSET_Y + i + 4, WIDTH);
        screen_draw_line_center(OFFSET_X, OFFSET_Y + i + 3, WIDTH, "Successfully nuked ", RED_BG, RED);
        screen_draw_raw(OFFSET_X + 38, OFFSET_Y + i + 3, MAG_BG, "%s", username);
        input_continue(OFFSET_X, OFFSET_Y + i + 4, WIDTH);
    }
    free(database);
    free(username);
    fclose(database_new);
    fclose(database_file);
    remove("database.dat");
    rename("database.dat.temp","database.dat");
}

void ui_admin_export(){
    screen_clear();

    screen_draw_box_title(OFFSET_X, OFFSET_Y, WIDTH, 3, "Exporting", LME);
    screen_draw_line_center(OFFSET_X, OFFSET_Y + 1, WIDTH, "Please wait a minute...", LME_BG, LME);
    screen_loading_mini(OFFSET_X + 41, OFFSET_Y + 1);

    screen_draw_line_center(OFFSET_X, OFFSET_Y + 1, WIDTH, "Successfully Exported!", LME_BG, LME);
    screen_draw_line_decor(OFFSET_X,OFFSET_Y + 3, WIDTH, CYN);
    input_continue(OFFSET_X, OFFSET_Y + 2, WIDTH);

    database_textify();

    return;
}

void ui_fun_ascii(){
    FILE *file = fopen("ascii.txt", "r");
    uint16_t i = 0, j = 0;
    char *status; // Check whether the line reach EOF
    char line[255];
    
    screen_clear();

    if(!file){
        screen_draw_line(OFFSET_X, OFFSET_Y + 1, sizeof(line), "No ascii.txt file was found", RED_BG, RED);
        screen_draw_line_decor(OFFSET_X, OFFSET_Y + 2, WIDTH, YEL);
        screen_draw_line_decor(OFFSET_X, OFFSET_Y +  4, WIDTH, YEL);
        input_continue(OFFSET_X, OFFSET_Y + 3, WIDTH);
        return;
    }

    // Basically print every line in ascii.txt file
    do {
        status = fgets(line, sizeof(line), file);
        while (line[j] != '\n'){
            j++;
        }
        if (i == 0)
            screen_draw_line_decor(OFFSET_X, 1, j + 4, PNK);

        screen_draw_line(OFFSET_X, i + 2, j + 4, "", PNK, PNK);
        screen_draw_raw(OFFSET_X + 2, i + 2, WHT, "%s", line);

        usleep(MILISECONDS * 70);
        fflush(stdout);
        i++;
    } while(status != NULL);

    input_continue(OFFSET_X, OFFSET_Y + i + 4, j + 4);

    fclose(file);
    free(status);
}

// /* ===================== Cheat Screen ======================= */
void ui_cheat(){
    typedef enum {
        CREDITS = 1,
        SLOT    = 2,
        COIN    = 3,
        RNG     = 4,
        EXIT    = 0
    } MenuCheat;
    int32_t input;

    while(RUNNING){
        screen_clear();
        screen_draw_box_title(OFFSET_X, OFFSET_Y, WIDTH, HEIGHT - 1, "!!! Cheat menu !!!", PNK);
        screen_draw_line(OFFSET_X, OFFSET_Y + 1, WIDTH, "Which cheat do you want to apply?", PNK, PNK);
        screen_draw_line_decor(OFFSET_X, OFFSET_Y + 2, WIDTH, PNK);
        screen_draw_line(OFFSET_X, OFFSET_Y + 3, WIDTH, "1 : Change the amount of credit", GRN, PNK);
        screen_draw_line(OFFSET_X, OFFSET_Y + 4, WIDTH, "2 : Always win on slot", GRN, PNK);
        screen_draw_line(OFFSET_X, OFFSET_Y + 5, WIDTH, "3 : Always win on coin toss", LME, PNK);
        screen_draw_line(OFFSET_X, OFFSET_Y + 6, WIDTH, "4 : Always win on rng", YEL, PNK);
        screen_draw_line(OFFSET_X, OFFSET_Y + 7, WIDTH, "0 : Return to previous Selection", RED, PNK);
        screen_draw_line_decor(OFFSET_X, OFFSET_Y + 10, WIDTH, YEL);
        screen_draw_line_input(OFFSET_X, OFFSET_Y + 9, WIDTH, "Your input : ", YEL, YEL);

        bool check = input_number(OFFSET_X, OFFSET_Y + 9, &input);

        if (check == false) {
            screen_draw_line_center(OFFSET_X, OFFSET_Y + 9, WIDTH, "Please input the number correctly >w<", RED_BG, RED);
            screen_draw_line_decor(OFFSET_X, OFFSET_Y + 11, WIDTH, RED);
            input_continue(OFFSET_X, OFFSET_Y + 10, WIDTH); 
            continue;
        }

        switch(input){
            case CREDITS :
                ui_cheat_credits();
                break;
            case SLOT :
            case COIN :
            case RNG :
                ui_cheat_games(input);
                break;
            case EXIT :
                return;
            default :
                screen_draw_line_center(OFFSET_X, OFFSET_Y + 9, WIDTH, "Please input the number correctly >w<", RED_BG, RED);
                screen_draw_line_decor(OFFSET_X, OFFSET_Y + 11, WIDTH, RED);
                input_continue(OFFSET_X, OFFSET_Y + 10, WIDTH); 
                break;
        }
    }
}

void ui_cheat_credits(){
    char *username;
    int32_t credits;

    screen_clear();
    screen_draw_box_title(OFFSET_X, OFFSET_Y, WIDTH, 3, "Free Credits !!!", GRN);
    uint8_t i = ui_admin_view(true);
    screen_draw_line_decor(OFFSET_X, OFFSET_Y + i + 2, WIDTH, GRN);
    screen_draw_line_decor(OFFSET_X, OFFSET_Y + i + 4, WIDTH, GRN);
    screen_draw_line_input(OFFSET_X, OFFSET_Y + i + 3, WIDTH, "Which user do you want to add? ", GRN, GRN);

    username = input_string();

    FILE* database_file = fopen("database.dat", "rb+");
    User* database = malloc(sizeof(User));
    bool found = false;

    while(fread(database, sizeof(User), 1, database_file) == 1){
        if (strcmp(username, "Admin") == 0){
            found = true;

            screen_draw_line_center(OFFSET_X, OFFSET_Y + i+ 3, WIDTH, "You are the game master! No need for that", RED_BG, RED);
            screen_draw_line_decor(OFFSET_X, OFFSET_Y + i+ 5, WIDTH, YEL);
            input_continue(OFFSET_X ,OFFSET_Y + i + 4, WIDTH);
            break;
        } else if (strcmp(username, database->username) == 0){
            found = true;

            screen_draw_line_input(OFFSET_X, OFFSET_Y + i + 3, WIDTH, "New credits (Previous =    ) : ", GRN, GRN);
            screen_draw_raw(OFFSET_X + 26, OFFSET_Y + i + 3, GRN, "%-3i", database->game_data.credits);
            SET_OFFSET(OFFSET_X + 33, OFFSET_Y + i + 3);
            
            bool check = input_number(OFFSET_X, OFFSET_Y + i + 3, &credits);

            if (check == false) {
                screen_draw_line_center(OFFSET_X, OFFSET_Y + i + 3, WIDTH, "Please input the number correctly >w<", RED_BG, RED);
                screen_draw_line_decor(OFFSET_X, OFFSET_Y + i + 5, WIDTH, RED);
                input_continue(OFFSET_X, OFFSET_Y + i + 4, WIDTH); 
                break;
            }

            database->game_data.credits += credits;                
            screen_draw_line_center(OFFSET_X, OFFSET_Y + i + 3, WIDTH, "Credits changed to : ", GRN_BG, RED);
            screen_draw_raw(OFFSET_X + 39, OFFSET_Y + i + 3, GRN, "%-3i", database->game_data.credits);
            screen_draw_line_decor(OFFSET_X, OFFSET_Y + i + 5, WIDTH, RED);
            input_continue(OFFSET_X, OFFSET_Y + i + 4, WIDTH); 

            fseek(database_file, -sizeof(User), SEEK_CUR);
            fwrite(database, sizeof(User), 1, database_file); 
            break;
        }
    }
    if (found == false){
        screen_draw_line_center(OFFSET_X, OFFSET_Y + i + 3, WIDTH, "User not found!", RED_BG, RED);
        screen_draw_line_decor(OFFSET_X, OFFSET_Y + i + 5, WIDTH, YEL);
        input_continue(OFFSET_X ,OFFSET_Y + i + 4, WIDTH);
    }

    free(database);
    free(username);
    fclose(database_file);
}

void ui_cheat_games(uint8_t game_type){
    // slot = 0
    // coin = 1
    // rng  = 2
    char *game[] = {"Slot", "Coin Toss", "RNG"};
    game_type -= 2; // Offset-ed to fit into array

    screen_clear();
    screen_draw_box_title(OFFSET_X, OFFSET_Y, WIDTH, 5, "Cheat mode ON !!!", MAG);
    screen_draw_line_center(OFFSET_X, OFFSET_Y + 1, WIDTH, "The change will be temporary!", RED_BG, MAG);
    screen_draw_line(OFFSET_X, OFFSET_Y + 2, WIDTH, "Selected game : ", MAG, MAG);
    screen_draw_raw(OFFSET_X + 18, OFFSET_Y + 2, BLU_BG, "%s", game[game_type]);
    screen_draw_line(OFFSET_X, OFFSET_Y + 3, WIDTH, "Current value : ", MAG, MAG);
    screen_draw_raw(OFFSET_X + 18, OFFSET_Y + 3, WHT, "%s", (cheat[game_type]) ? GRN_BG "True " RST : RED_BG "False" RST);
    screen_draw_line_decor(OFFSET_X, OFFSET_Y + 6, WIDTH, YEL);
    screen_draw_line_input(OFFSET_X, OFFSET_Y + 5, WIDTH, "Change? (Y/n) ", YEL, YEL);

    bool confirm = input_confirmation();
    if (confirm == true){
        // Toogle cheat
        if (cheat[game_type] == false){
            cheat[game_type]    = true;
        } else 
            cheat[game_type] = false;

        screen_draw_raw(OFFSET_X + 18, OFFSET_Y + 3, WHT, "%s", (cheat[game_type]) ? GRN_BG "True " RST : RED_BG "False" RST);
        screen_draw_line_center(OFFSET_X, OFFSET_Y + 5, WIDTH, "Successfully changed the value!", MAG_BG, MAG);
        screen_draw_line_decor(OFFSET_X, OFFSET_Y + 7, WIDTH, MAG);
        input_continue(OFFSET_X, OFFSET_Y + 6, WIDTH);

    } else {
        screen_draw_line_center(OFFSET_X, OFFSET_Y + 5, WIDTH, "No change was made...", RED_BG, RED);
        screen_draw_line_decor(OFFSET_X, OFFSET_Y + 7, WIDTH, RED);
        input_continue(OFFSET_X, OFFSET_Y + 6, WIDTH);
    }
}