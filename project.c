// Library (Macros and stuff go there)
#include "game_tools.h"
#include "game_data.h"
bool cheat[3] = {false};

// Function Prototype
void screen_start       ();
void screen_start_login ();
void screen_start_signup();
void screen_leaderboard ();
/* Game Menu */ 
void screen_user            (user_t*);
void screen_user_slot       (user_t*);
void screen_user_coin       (user_t*);
void screen_user_rng        (user_t*);
void screen_user_market     (user_t*);
void screen_user_achievement(user_t*);
/* Admin */
void screen_admin       ();
void screen_admin_view  ();
void screen_admin_ban   ();
void screen_admin_unban ();
void screen_admin_delete();
void screen_fun_ascii   ();
/* Cheat screen */
void screen_cheat        ();
void screen_cheat_credits();
void screen_cheat_games  (int);

void main(){
    srand(time(NULL));
    screen_start();
    exit(0);
}

/* ===================== Start Screen ======================= */
void screen_start(){
    typedef enum {
        LOGIN       = 1,
        SIGNUP      = 2,
        LEADERBOARD = 3,
        EXIT        = 0,
    } selection_start_t;
    int input;

    while(RUNNING){
        screen_clear();
        printf( 
            MAG "Welcome to Game Center, where you can play numerous of games!" CURSOR_DOWN RESET
            BLU "User, please select one of these option!" CURSOR_DOWN RESET
                "1. Login       " CURSOR_DOWN
                "2. Sign in     " CURSOR_DOWN
            YEL "3. Leaderboard " CURSOR_DOWN
            RED "0. Exit        " CURSOR_DOWN RESET
                "Your input : "
        );
        if (!sys_input_int(&input)) continue;

        switch(input){
            case LOGIN: 
                screen_start_login();
                break;
            case SIGNUP: 
                screen_start_signup(); 
                break;
            case LEADERBOARD: 
                screen_leaderboard();
                break;
            case EXIT: 
                screen_clear(); 
                printf(MAG "See you next time!" CURSOR_DOWN RESET); 
                return;
            default:
                printf(RED "Please input correctly >w<" CURSOR_DOWN RESET);
                sys_input_return(); 
                break;
        }
    }   
}

void screen_start_login(){
    screen_clear();

    // Display UI
    printf(BLU "::: Login :::" CURSOR_DOWN RESET);
    printf(    "Username :" CURSOR_DOWN);
    printf(    "Password :" CURSOR_DOWN);

    // Input the username and password
    char *username, *password;
    printf(CURSOR_UP CURSOR_UP);
    printf(    "Username : ");
    username = sys_input_str();
    printf(    "Password : ");
    password = sys_input_str();

    // Check the inputs with the database
    user_t* user = malloc(sizeof(user_t));
    user_type_t user_type = database_user_login(username, password, user);
    free(username); free(password);

    // The council will check your data :)
    switch(user_type){
        case ADMIN:
            printf(BLU ":: Login Successfull >w< -> Entering as Admin..." CURSOR_DOWN RESET);
            sys_input_return();
            screen_admin();
            break;
        case USER:
            if (user->banned){
                printf(RED "Unfortunately, the admin has banned you due to this reason :" CURSOR_DOWN RESET);
                printf(    "-> %s" CURSOR_DOWN, user->message);
                sys_input_return();
            } else {
                printf(BLU ":: Login Successfull >w< -> Entering as User..." CURSOR_DOWN RESET);
                sys_input_return();
                screen_user(user);
            }
            break;
        case NONE:
        default:
            printf(RED ":: Login Failed >A<" CURSOR_DOWN RESET);
            sys_input_return();
            break;
    }

    free(user);
}

void screen_start_signup(){
    screen_clear();

    // Display UI
    printf(BLU "::: Sign Up :::" CURSOR_DOWN RESET);
    printf(    "Username :" CURSOR_DOWN);
    printf(    "Password :" CURSOR_DOWN);

    // Input the username and password
    char *username, *password;
    printf(CURSOR_UP CURSOR_UP);
    printf(    "Username : ");
    username = sys_input_str();
    printf(    "Password : ");
    password = sys_input_str();

    // Check the inputs with the database
    user_t* database = malloc(sizeof(user_t));
    status_t status = database_user_signup(username, password, database);

    // Hop on gambling
    switch(status){
        case SUCCESS:
            printf(BLU ":: Sign-up Successfull >w< -> Entering the First Setup..." CURSOR_DOWN RESET);
            sys_input_return();
            database_user_setup(username, password, database);
            break;
        case FAILED:
        default:
            printf(RED ":: Sign-up Failed -> Failed to create an account T-T" CURSOR_DOWN RESET);
            sys_input_return();
            break;
    }
    free(username); free(password);
    free(database);
}

void screen_leaderboard(){
    FILE* database_file = fopen("database.dat", "rb");

    screen_clear();
    // Check whether the file exist or not
    if (!sys_input_check_file(database_file)){
        printf(RED "[!] No user has been registered >A<" RESET CURSOR_DOWN);
        sys_input_return();
        return;
    }
    // Count how many user is registered
    fseek(database_file, 0, SEEK_END);
    size_t database_size = ftell(database_file);
    int database_user_total = database_size / sizeof(user_t);
    if (database_user_total <= 1){
        sys_input_return();
        fclose(database_file);
        return;
    }
    // Convert into array
    user_t* database = malloc(sizeof(user_t) * database_user_total);
    rewind(database_file);
    fread(database, sizeof(user_t), database_user_total, database_file);

    // Sorting part
    database_sort(database_file, database, database_user_total);

    // Display part
    printf(BLU "           ::: Leaderboard :::           " CURSOR_DOWN RESET);
    printf(MAG "Rank | Name          | Gambled | Balances" CURSOR_DOWN RESET);
    printf(MAG "-----------------------------------------" CURSOR_DOWN RESET);
    for (int i=1; i < database_user_total; i++) {;
        printf( 
            "%2i.  %s|%s %-10s%s    %s|%s   %-3li   %s| %s$%-i   " RESET CURSOR_DOWN, 
            i, // skip admin
            MAG, // bar
            RESET,
            database[i].username, 
            (database[i].banned) ? RED"🔨"RESET : "",
            MAG, // bar
            YEL, database[i].game_data.counter[3],
            MAG, // bar
            (database[i].game_data.balances <= 0) ? RED : GRN, 
            database[i].game_data.balances              
        );
    }
    sys_input_return();
}

/* ====================== User Screen ======================= */
void screen_user(user_t* user){
    typedef enum {
        SLOT        = 1,
        COIN        = 2,
        RNG         = 3,
        LEADERBOARD = 7,
        ACHIEVEMENT = 8,
        MARKET      = 9,
        EXIT        = 0,
    } selection_user_t;
    int input;

    while(RUNNING){
        screen_clear();
        printf( 
            BLU "Do you love gambling, %s%s%s? I do and You are in the right place!" CURSOR_DOWN
                "Which game do you want to play?        " CURSOR_DOWN
                "[Balances : %s$%i%s] [Credits : %s%i%s]" CURSOR_DOWN          
                "1 : Slot                       " CURSOR_DOWN
                "2 : Coin                       " CURSOR_DOWN
                "3 : Random Number Generator    " CURSOR_DOWN
            YEL "7 : Leaderboard                " CURSOR_DOWN 
            BLU "8 : Achievement                " CURSOR_DOWN
            GRN "9 : Buy Credits                " CURSOR_DOWN
            RED "0 : Exit                       " CURSOR_DOWN RESET
                "Your input: ", 
            MAG, user->username, RESET, 
            (user->game_data.balances <= 0) ? RED : GRN,user->game_data.balances, RESET,
            MAG, user->game_data.credits, RESET
        );
        if (!sys_input_int(&input)) continue;

        switch(input){
            case SLOT: 
                screen_user_slot(user);
                database_update(user);
                break;
            case COIN: 
                screen_user_coin(user);
                database_update(user);
                break;
            case RNG:
                screen_user_rng(user);
                database_update(user);
                break;
            case LEADERBOARD: 
                screen_leaderboard();
                break;
            case ACHIEVEMENT: 
                screen_user_achievement(user);
                break;
            case MARKET:
                screen_user_market(user);
                database_update(user);
                break;
            case EXIT: 
                screen_clear();
                printf(CURSOR_HIDE);
                if (user->game_data.balances < 0) {
                    printf( 
                        RED "You are in debt for $%i!" CURSOR_DOWN
                            "Please pay it or else debt collector will haunt you > w <!!!" CURSOR_DOWN
                        RESET, user->game_data.balances
                    );
                }
                printf(CURSOR_SHOW);
                printf(MAG "Thank you for Playing!" CURSOR_DOWN RESET);
                printf(MAG "Exiting..." CURSOR_DOWN RESET);
                sys_loading_mini();
                return;
            default:
                printf(RED "Please input correctly >w<" CURSOR_DOWN RESET);
                sys_input_return(); 
                break;
        }
    }   
}

void screen_user_slot(user_t* user){
    char *slot_icons[10] = {
        MAG"🍇"RESET, GRN"🍉"RESET, MAG"🍆"RESET, CYN"🍪"RESET, RED"🐶"RESET, 
        YEL"🐱"RESET, CYN"🐻"RESET, BLU"🐟️"RESET, RED"🍎"RESET, GRN"🐢"RESET
    };
    char *slot_value_icons[3];
    int slot_value[3];

    screen_clear();
    printf( 
        BLU "          ::: Madoka Slot Machine!!! :::         " CURSOR_DOWN RESET
            "Pretty much just your regular slot game          " CURSOR_DOWN
            "Win condition : %sGet atleast 2 matching pictures" CURSOR_DOWN RESET
            "Simple right?" CURSOR_DOWN,
            MAG
    );
    sys_input_return();
    
    while(RUNNING){
        screen_clear();
        printf(BLU "          ::: Madoka Slot Machine!!! :::         " CURSOR_DOWN RESET);
        printf(    "                Credit(s) left = %s%i           " CURSOR_DOWN RESET, 
            MAG, user->game_data.credits
        );
        if (user->game_data.credits == 0){
            printf("Too bad... but try %sKEEP GAMBLING%s again!" CURSOR_DOWN, MAG, RESET);
            sys_input_return();
            break;
        } else {
            user->game_data.credits--;
            user->game_data.counter[0]++;
        }
        // Rollcake
        for (int i=0; i<3; i++){
            if (cheat[0] == true){
                slot_value[i] = 7;
            } else
                slot_value[i] = rand() % (9); 
            slot_value_icons[i] = slot_icons[slot_value[i]];             
        }
        // Display
        printf( 
            BLU "              +==================+              " CURSOR_DOWN RESET
            BLU "              |%s [%s]  [%s]  [%s] %s|--%sO     " CURSOR_DOWN RESET
            BLU "              +==================+              " CURSOR_DOWN RESET,
            RESET,
            slot_value_icons[0], slot_value_icons[1], slot_value_icons[2],
            BLU, RED
        );
        // If you get le fishe [🐟️] [🐟️] [🐟️]
        if (slot_value[0] == 7 && 
            slot_value[1] == 7 && 
            slot_value[2] == 7) {
            printf( 
                "            %s🐟️ 🐟️ 🐟️%s Jackpot! %s🐟 🐟 🐟️" CURSOR_DOWN RESET,
                BLU, GRN, BLU
            );
            printf( 
                "The fish council just give you %s$500!!!" CURSOR_DOWN RESET,
                GRN
            );
            user->game_data.balances += 500;
        }
        // If you get all the same pictures
        else if (slot_value[0] == slot_value[1] && 
                 slot_value[0] == slot_value[2] && 
                 slot_value[1] == slot_value[2]) {
            printf( 
                "Congrats, you just win %s$50!" CURSOR_DOWN RESET,
                GRN
            );
            user->game_data.balances += 50;
        }
        // If you get atleast 2 pictures matching
        else if (slot_value[0] == slot_value[1] || 
                 slot_value[0] == slot_value[2] || 
                 slot_value[1] == slot_value[2]) {
            printf( 
                "Congrats, you just win %s$20!" CURSOR_DOWN RESET,
                GRN
            );
            user->game_data.balances += 20;
        }
        else{
            printf(
                RED "Try again!" CURSOR_DOWN RESET
            );
        }

        printf(YEL "Continue playing!? (Y/n) " RESET);
        if (!sys_input_confirm()) break;
    }
}

void screen_user_coin(user_t* user){
    int coin_throw, coin_guess;
    char *coin_icons[] = {YEL"😼"RESET, RED"🐶"RESET};

    screen_clear();
    printf( 
        BLU "               ::: Coin Toss!!! :::        " CURSOR_DOWN RESET
            "The classic coin toss                      " CURSOR_DOWN
            "Win Condition : %sJust guess Head or tails " CURSOR_DOWN RESET
            "If you guess correct, you will get %s$20   " CURSOR_DOWN RESET,
            MAG, GRN
    );
    sys_input_return();

    while(RUNNING){
        screen_clear();
        printf(BLU  "               ::: Coin Toss!!! :::        " CURSOR_DOWN RESET);
        printf(     "               Credit(s) left = %s%i     " CURSOR_DOWN RESET, 
            MAG, user->game_data.credits
        );        
        if (user->game_data.credits == 0){
            printf("Too bad... but try %sKEEP GAMBLING%s again!" CURSOR_DOWN, MAG, RESET);
            sys_input_return();
            break;
        } else {
            user->game_data.credits--;
            user->game_data.counter[1]++;
        }

        printf(     
                "                       [#]                    " CURSOR_DOWN
            CYN "               Input your guess!              " CURSOR_DOWN RESET
                "            [%sHead 😼 (0)%s|%sTail 🐶 (1)%s] : ",
                YEL, RESET, RED, RESET
        );

        if (!sys_input_int(&coin_guess)) {
            user->game_data.credits++;
            user->game_data.counter[1]--;
            continue;
        } else if (coin_guess != 0 && coin_guess != 1){
            user->game_data.credits++;
            user->game_data.counter[1]--;

            printf(RED "Please input correctly >w< !" CURSOR_DOWN RESET);
            printf(YEL "Continue playing!? (Y/n) " RESET);
            if (!sys_input_confirm()) break;
            else continue;
        }

        // Reveal the guess
        if (cheat[1] == true)
            coin_throw = coin_guess;
        else
            coin_throw = rand() % 2;

        char *coin_throw_icons  = coin_icons[coin_throw];
        printf(CURSOR_UP CURSOR_UP CURSOR_UP);
        printf(     
            "                       [%s]                    " CURSOR_DOWN
        CYN "               Input your guess!              " CURSOR_DOWN RESET
            "            [%sHead 😼 (0)%s|%sTail 🐶 (1)%s] : %i " CURSOR_DOWN,
            coin_throw_icons, 
            YEL, RESET, RED, RESET,
            coin_guess
        );

        if (coin_guess == coin_throw){
            printf("You just get "GRN"$20!\n"RESET);
            user->game_data.balances += 20;
        } else {
            printf("Too bad... but try %sKEEP GAMBLING%s again!" CURSOR_DOWN, MAG, RESET);
        }
        printf(YEL "Continue playing!? (Y/n) " RESET);
        if (!sys_input_confirm()) break;
    }
}

void screen_user_rng(user_t* user){
    int number_throw, number_guess;

    screen_clear();
    printf( 
        BLU "      ::: Random Number Guesser!!! :::              " CURSOR_DOWN RESET
            "RNG God                                                    " CURSOR_DOWN RESET
            "Win Condition : %sGuess a number between 0-99              " CURSOR_DOWN RESET
            "If you guess correctly, you will get %s$500                " CURSOR_DOWN RESET
            "If you guess 5 numbers above or bellow, you will get %s$50 " CURSOR_DOWN RESET,
            MAG, GRN, GRN
    );
    sys_input_return();

    while(RUNNING){
        screen_clear();
        printf(BLU  "      ::: Random Number Guesser!!! :::    " CURSOR_DOWN RESET);
        printf(     "           Credit(s) left = %s%i   " CURSOR_DOWN RESET, 
            MAG, user->game_data.credits
        );        
        if (user->game_data.credits == 0){
            printf("Too bad... but try %sKEEP GAMBLING%s again!" CURSOR_DOWN, MAG, RESET);
            sys_input_return();
            break;
        } else {
            user->game_data.credits--;
            user->game_data.counter[2]++;
        }

        printf(     
                "                    [#]           " CURSOR_DOWN
            CYN "        Input your guess! (0-99) : " RESET
        );

        if (!sys_input_int(&number_guess)) {
            user->game_data.credits++;
            user->game_data.counter[2]--;
            continue;
        } else if (number_guess < 0 || number_guess > 99){
            user->game_data.credits++;
            user->game_data.counter[2]--;

            printf(RED "Please input correctly >w< !" CURSOR_DOWN RESET);
            printf(YEL "Continue playing!? (Y/n) " RESET);
            if (!sys_input_confirm()) break;
            else continue;
        }

        // Reveal the guess
        if (cheat[2] == true)
            number_throw = number_guess;
        else
            number_throw = rand() % 100;

        printf(CURSOR_UP CURSOR_UP);
        printf(     
                "                    [%2i]              " CURSOR_DOWN
            CYN "        Input your guess! (0-99) : %i  " CURSOR_DOWN RESET,
            number_throw, number_guess
        );

        // If they are the same
        if (number_guess == number_throw){
            printf("No way, you are right on the money! You just get %s$500!" CURSOR_DOWN RESET, GRN);
            user->game_data.balances += 500;
        }
        // If +- 5 differences
        else if (number_guess >= number_throw - 5 && 
                 number_guess <= number_throw + 5) {
            printf("You are just %s%i%s number away!" CURSOR_DOWN, MAG, number_throw - number_guess, RESET);
            printf("You just get %s$50!%s           " CURSOR_DOWN, GRN, RESET);
            user->game_data.balances += 50;
        } 
        // If miss
        else {
            printf("Too bad... but try %sKEEP GAMBLING%s again!" CURSOR_DOWN, MAG, RESET);
        }
        
        printf(YEL "Continue playing!? (Y/n) " RESET);
        if (!sys_input_confirm()) break;
    }
}

void screen_user_market(user_t* user){
    int credits_add, credits_cost, balances_difference, balances_change;

    screen_clear();
    printf( 
        BLU "           ::: Market!!! :::           " CURSOR_DOWN RESET
        BLU "Uhmmm... hi > w < !!!                  " CURSOR_DOWN RESET
            "Hh-how much do you want to pay T-T?    " CURSOR_DOWN RESET
            "Credits %s(1 = $10) : " RESET,
            GRN
    );
    if (!sys_input_int(&credits_add)) return;
    credits_cost    = 10 * credits_add;
    balances_change = user->game_data.balances - credits_cost;

    if (credits_add < 0) {
        printf(RED "Nuh uh... Too bad you can't do that (·ω·)\n"RESET);
        sys_input_return();
        return;
    } else if (credits_add == 0) {
        printf("Transaction cancelled! "BLU"> v < \n"RESET);
        sys_input_return();
        return;
    } else if (credits_add < 0) {
        if (user->game_data.balances >= 0) 
            balances_difference = credits_cost -  user->game_data.balances;
        else 
            balances_difference = credits_cost; // Debt
        printf( 
            MAG "Do you want to borrow %s$%i?%s (Y/n) : ", 
            RED, RESET, balances_difference
        );
    } else 
        printf(MAG "Are you sure? " RESET "(Y/n)");
    if (!sys_input_confirm()) {
        printf("Transaction cancelled! "BLU"> v < \n"RESET);
        sys_input_return();
        return;
    }

    printf(MAG "Transfering..." CURSOR_DOWN RESET);
    sys_loading_default();
    user->game_data.credits += credits_add;
    user->game_data.balances-= 10 * credits_add;

    printf(
        GRN "Money transfered! %sCurrent balances : %s$%i   " CURSOR_DOWN RESET
            "Erm... please gamble responsibly T-T...        " CURSOR_DOWN,
            RESET, (user->game_data.balances <= 0) ? RED : GRN, user->game_data.balances
    );
    sys_input_return();
    return;
}

void screen_user_achievement(user_t* user){
    bool track      = false;
    int  counter[]  = {10, 50, 100};
    char *colors[]  = {GRN, MAG, RED};
    char *game[]    = {"Slot", "Coin Toss", "Random Number Guesser"};

    screen_clear();
    printf(MAG "Your achievement :" CURSOR_DOWN RESET);

    for(int i=0; i < 3; i++){
        for(int j=0; i < 3; i++){
            if (user->game_data.counter[i] >= counter[j]) {
                printf( "-> Play %s%s%s for %s%i times" CURSOR_DOWN RESET, 
                        BLU, game[i], RESET, colors[j], counter[j]
                );
                track = true;
            }
        }
    }
    if (user->game_data.balances <= -100 || user->game_data.persist[0] == true) {
        printf(
            "-> %sDebt Collector's Ghost%s : In %sdebt%s for more than %s$100" CURSOR_DOWN RESET,
            MAG, RESET, RED, RESET, RED
        );
        user->game_data.persist[0] = true;
        track = true;
    }
    if (user->game_data.balances >= 1000 && user->game_data.counter[3] >= 100 || user->game_data.persist[1] == true) {
        printf(
            "-> %sMasterful Gambit%s : Earned %s$1000" CURSOR_DOWN RESET,
            MAG, RESET, GRN
        );
        user->game_data.persist[1] = true;
        track = true;
    }
    if (user->game_data.counter[3] >= 100 || user->game_data.persist[2] == true) {
        printf(
            "-> %sA True Gambler Mindset%s : Gambled for more than %s100 times" CURSOR_DOWN RESET,
            MAG, RESET, GRN
        );
        user->game_data.persist[2] = true;
        track = true;
    }

    if (!track) printf(RED "There's no achievement yet ('A`)" CURSOR_DOWN RESET);
    sys_input_return();
}

/* ===================== Admin Screen ======================= */
void screen_admin(){
    typedef enum {
        VIEW    = 1,
        BAN     = 2,
        UNBAN   = 3,
        REMOVE  = 4,
        CHEAT   = 8,
        ASCII   = 9,
        EXIT    = 0
    } selection_admin_t;
    int input;

    while(RUNNING){
        screen_clear();
        printf( 
            MAG "Hello Mr. Admin!                   " CURSOR_DOWN
                "What do you want to select? >///<  " CURSOR_DOWN RESET
                "1. View Registered user            " CURSOR_DOWN
                "2. Ban User                        " CURSOR_DOWN
                "3. Unban User                      " CURSOR_DOWN
                "4. Remove User                     " CURSOR_DOWN
            GRN "8. Cheat                           " CURSOR_DOWN
            BLU "9. ASCII art                       " CURSOR_DOWN
            RED "0. Exit                            " CURSOR_DOWN RESET
                "Your input : "
        );
        if (!sys_input_int(&input)) continue;
        switch(input){
            case VIEW :
                screen_admin_view();
                sys_input_return();
                break;
            case BAN :
                screen_admin_ban();
                break;
            case UNBAN :
                screen_admin_unban();
                break;
            case REMOVE :
                screen_admin_delete();
                break;
            case CHEAT :
                screen_cheat();
                break;
            case ASCII :
                screen_fun_ascii();
                sys_input_return();
                break;
            case EXIT :
                screen_clear();
                printf(MAG "Exiting..." CURSOR_DOWN RESET);
                sys_loading_mini();
                return;
            default :
                printf(RED "Please input correctly >w< !" CURSOR_DOWN RESET);
                sys_input_return();
                break;
        }
    }
}

void screen_admin_view(){
    FILE* database_file = fopen("database.dat", "rb");
    user_t* database_user = malloc(sizeof(user_t));
    int i=1;

    // Check whether the file exist or not
    if (!sys_input_check_file(database_file)){
        return;
    }

    screen_clear();
    printf(MAG "List of registered user :" CURSOR_DOWN RESET);
    while(fread(database_user, sizeof(user_t), 1, database_file) == 1){
        printf(
            "%2i. Username : %s %s   " CURSOR_DOWN RESET
            "    Password : %s      " CURSOR_DOWN RESET
            "    Balances : %s%i    " CURSOR_DOWN RESET
            "    Credits  : %i      " CURSOR_DOWN RESET,
            i, database_user->username, (database_user->banned) ? RED"(banned)"RESET : "",
            database_user->password,
            (database_user->game_data.balances <= 0) ? RED : GRN, database_user->game_data.balances,
            database_user->game_data.credits
        );
        printf(CURSOR_DOWN);
        i++;
    }
    free(database_user);
    fclose(database_file);
}

void screen_admin_ban(){
    char *username;
    char *message;

    screen_admin_view();
    printf("Which user do you want to be %sbanned?%s ", RED, RESET);
    username = sys_input_str();

    FILE* database_file = fopen("database.dat", "rb+");
    user_t* database = malloc(sizeof(user_t));
    bool found = false;
    while(fread(database, sizeof(user_t), 1, database_file) == 1){
        if (strcmp(username, "Admin") == 0){
            found = true;
            printf(RED "[!] Nuh uh, you can't ban this user!" CURSOR_DOWN RESET);
            break;
        } else if (strcmp(username, database->username) == 0){
            found = true;
            printf("Reason for the ban? ");
            message = sys_input_str();
            database->banned = true;
            strcpy(database->message, message);
            printf(GRN "Successfully banning %s\"%s\"" CURSOR_DOWN, RESET, username);

            fseek(database_file, -sizeof(user_t), SEEK_CUR);
            fwrite(database, sizeof(user_t), 1, database_file); 
            free(message);
            break;
        }
    }
    if (!found){
        printf(RED"[!] User not found!" CURSOR_DOWN RESET);
    }
    sys_input_return();
    free(database);
    free(username);
    fclose(database_file);
}

void screen_admin_unban(){
    char *username;

    screen_admin_view();
    printf("Which user do you want to be %sunbanned?%s ", BLU, RESET);
    username = sys_input_str();

    FILE* database_file = fopen("database.dat", "rb+");
    user_t* database = malloc(sizeof(user_t));
    bool found = false;
    while(fread(database, sizeof(user_t), 1, database_file) == 1){
        if (strcmp(username, "Admin") == 0){
            found = true;
            printf(RED "[!] Nuh uh, you can't do thing with this user!" CURSOR_DOWN RESET);
            break;
        } else if (strcmp(username, database->username) == 0){
            found = true;
            database->banned = false;
            printf(GRN "Successfully unbanning %s\"%s\"" CURSOR_DOWN, RESET, username);
            
            fseek(database_file, -sizeof(user_t), SEEK_CUR);
            fwrite(database, sizeof(user_t), 1, database_file); 
            break;
        }
    }
    if (!found){
        printf(RED"[!] User not found!" CURSOR_DOWN RESET);
    }
    sys_input_return();
    free(database);
    free(username);
    fclose(database_file);
}

void screen_admin_delete(){
    char *username;

    screen_admin_view();
    printf("Which user do you want to be %snuked?%s ", RED, RESET);
    username = sys_input_str();

    FILE* database_file = fopen("database.dat", "rb+");
    FILE* database_new = fopen("database.dat.temp", "wb");
    user_t* database = malloc(sizeof(user_t));
    bool found = false;
    while(fread(database, sizeof(user_t), 1, database_file) == 1){
        if (strcmp(username, "Admin") == 0){
            printf(RED "[!] Nuh uh, you can't delete this user!" CURSOR_DOWN RESET);
            sys_input_return();
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
        fwrite(database, sizeof(user_t), 1, database_new);
    }
    if (found){
        sys_loading_nuke();
        printf(GRN "Successfully nuked %s\"%s\"" CURSOR_DOWN, RESET, username);
    } else
        printf(RED"[!] User not found!" CURSOR_DOWN RESET);
    sys_input_return();
    free(database);
    free(username);
    fclose(database_new);
    fclose(database_file);
    remove("database.dat");
    rename("database.dat.temp","database.dat");
}

void screen_fun_ascii(){
    FILE *file = fopen("ascii.txt", "r");
    char *status; // Check whether the line reach EOF
    char line[255];
    
    screen_clear();

    if(!file){
        printf(RED "No ascii.txt file was found" CURSOR_DOWN RESET);
        sys_input_return();
        return;
    }

    // Basically print every line in ascii.txt file
    do {
        status = fgets(line, sizeof(line), file);
        printf("%s", line);
    } while(status != NULL);

    fclose(file);
    free(status);
}

/* ===================== Cheat Screen ======================= */
void screen_cheat(){
    typedef enum {
        CREDITS = 1,
        SLOT    = 2,
        COIN    = 3,
        RNG     = 4,
        EXIT    = 0
    } selection_cheat_t;
    int input;

    while(RUNNING){
        screen_clear();
        printf( 
            MAG"Which cheat do you want to apply?\n"
            GRN "1 : Change the amount of credit\n"RESET
                "2 : Always win on slot\n"
                "3 : Always win on coin toss\n"
                "4 : Always win on rng\n"
            RED "0 : Return to previous Selection\n"RESET
                "Your input: "
        );
        if (!sys_input_int(&input)) continue;
        switch(input){
            case CREDITS :
                screen_cheat_credits();
                break;
            case SLOT :
            case COIN :
            case RNG :
                screen_cheat_games(input);
                break;
            case EXIT :
                return;
            default :
                printf(RED "Please input correctly >w< !" CURSOR_DOWN RESET);
                sys_input_return();
                break;
        }
    }
}

void screen_cheat_credits(){
    char *username;
    int32_t credits;

    screen_admin_view();
    printf("Which user do you want to add the %scredits?%s ", MAG, RESET);
    username = sys_input_str();

    FILE* database_file = fopen("database.dat", "rb+");
    user_t* database = malloc(sizeof(user_t));
    bool found = false;
    while(fread(database, sizeof(user_t), 1, database_file) == 1){
        if (strcmp(username, "Admin") == 0){
            found = true;
            printf(RED "[!] Mr Admin already has infinite credits!" CURSOR_DOWN RESET);
            break;
        } else if (strcmp(username, database->username) == 0){
            found = true;
            printf("New credits (previous = %i) : ", database->game_data.credits);
            if (!sys_input_int(&credits)){
                printf(RED "[!] No changes was made" CURSOR_DOWN RESET);
                break;
            }
            database->game_data.credits += credits;
            printf("Credits changed to %s%i" CURSOR_DOWN RESET, GRN, database->game_data.credits);

            fseek(database_file, -sizeof(user_t), SEEK_CUR);
            fwrite(database, sizeof(user_t), 1, database_file); 
            break;
        }
    }
    if (!found){
        printf(RED"[!] User not found!" CURSOR_DOWN RESET);
    }
    sys_input_return();
    free(database);
    free(username);
    fclose(database_file);
}

void screen_cheat_games(int game_type){
    // slot = 0
    // coin = 1
    // rng  = 2
    char *game[] = {"Slot", "Coin Toss", "RNG"};
    game_type -= 2; // Offset to fit into array

    screen_clear();
    printf( 
        RED "[!] %sThe change will be in effect until the program is closed %s[!]" CURSOR_DOWN RESET
            "Selected      : %s "CURSOR_DOWN RESET
            "Current value : %s "CURSOR_DOWN
        YEL "Change? (Y/n) : "RESET,
            MAG, RED,
            game[game_type], 
            (cheat[game_type]) ? BLU "True" RESET : RED "False" RESET
    );

    if (sys_input_confirm()) {
        // Toogle cheat
        if (cheat[game_type] == false){
            cheat[game_type]    = true;
        } else 
            cheat[game_type] = false;

        printf(CURSOR_UP CURSOR_UP);
        printf(
            "Current value : %s  "CURSOR_DOWN
        YEL "Change? (Y/n) : %sY "CURSOR_DOWN
        MAG "Successfully changed the value!" CURSOR_DOWN RESET,
            (cheat[game_type]) ? BLU"True"RESET : RED"False"RESET,
            RESET
        );
    } else {
        printf(RED "No change was made..." CURSOR_DOWN RESET);
    }
    sys_input_return();
}