/* To properly run it on windows cmd, change "clear" on system() to "cls" */

// Library (included all in qol.h)
#include "qol.h"

// Options
bool cheat[3] = {0};

/* ====================== User Data ======================= */
typedef struct{
    char username   [32+1];
    char password   [32+1];
    char message    [254+1]; // Display ban message
    int balances;
    int credits;
    int counter     [4]; // Gambling counter
    bool persist    [3]; // Related to achievement
    bool banned;
} data;
data current;   // Logged in user data
data temp;      // Temporary data
data record;    // Recorded data

/* Game Center! See who's the most gamble addicted person! */

/* ================== Function Prototype ================== */
/* Start Menu */
void start_screen();
void login(int * auth);
void sign_up();
void leaderboard();
/* Game Menu */ 
void main_screen();
void slot();
void coin();
void rng();
void market();
void achievement();
/* Admin */
void admin_screen();
bool view();
void banned();
void unbanned();
void nuke();
void update_data();
/* Cheat screen */
void cheat_screen();
void cheat_credits();
void cheat_games();

/* ===================== Start Menu ======================= */
int main(){
    srand(time(NULL)); // Start the gambling mode
    start_screen();
    return 0; 
}

void start_screen(){
    while(1){
        int opt;
        int auth = 0;
        
        clear_screen();
        printf( MAG "Welcome to Game Center, where you can play numerous of games!\n" RESET
                BLU "User, please select one of these option!\n" RESET
                    "1. Login       \n"
                    "2. Sign in     \n"
                YEL "3. Leaderboard \n"
                RED "0. Exit        \n" RESET
                    "Your input : "
        );
        if (!num_input(&opt)) continue;
        switch (opt){
            case 1 :
                login(&auth);
                if      (auth == 1) main_screen();
                else if (auth == 2) admin_screen();
                break;
            case 2 :
                sign_up();
                break;
            case 3 :
                leaderboard();
                break;
            case 0 :
                clear_screen();
                printf( MAG "See you next time!             "RESET);
                return;
            default :
                printf( RED "Please input correctly >w<!    \n"RESET);
                back();
                break;
        }
    }
}

void login(int * auth){
    FILE *user_data;

    clear_screen();

    printf(BLU "::: Login :::      \n"RESET);
    printf(    "Username :         \n");
    printf(    "Password :         \n");

    printf(UP UP); 
    printf(    "Username : ");
    fgets(temp.username, sizeof(temp.username), stdin);
    temp.username[strlen(temp.username) - 1] = '\0';

    printf(    "Password : ");
    fgets(temp.password, sizeof(temp.password), stdin);
    temp.password[strlen(temp.password) - 1] = '\0';

    // Match username and password with in the database
    bool found = false;

    // Admin login
    if (strcmp(temp.username, "admin") == 0){
        if (strcmp(temp.password, "123") == 0){
            printf(BLU "[Entering as an admin...]           \n"RESET);
            *auth = 2;
        } else{
            printf(RED "Sorry you are not authorized! T_T   \n"RESET);
        }
        back();
        return;
    }
    
    // User login
    user_data = fopen("data.dat","rb");
    if (!check(user_data)) {back(); return;}
    else user_data = fopen("data.dat","rb");

    // Check if the user input is the same as recorded one
    while(fread(&record,sizeof(data), 1, user_data) == 1){
        if (strcmp(record.username, temp.username) == 0){
            found = true;

            // Check the password
            if (strcmp(record.password, temp.password) == 0){
                printf(BLU"Login Successfull!\n"RESET);
                current = record;
                if (current.banned) {
                    printf(RED "\nUnfortunately, the admin has banned you due to this reason :\n"RESET);
                    printf(    "-> %s \n\n", current.message);
                    * auth = 0;
                } else * auth = 1;
            } else {
                printf(RED"Password incorrect!\n"RESET);
            }       
        }
    }

    if (!found){
        printf(RED"User not found!\n"RESET);
    } 
    
    fclose(user_data);
    back();
    return;
}

void sign_up(){
    FILE *user_data = fopen("data.dat", "rb");

    // Create a new binary if it doesn't exist yet
    if (!user_data) {
        user_data = fopen("data.dat", "wb");
        fclose(user_data);
        fopen("data.dat", "rb+");
    }

    // Username Registration
    clear_screen();
    while (1){
        bool exist = false;
        
        printf(BLU "::: Sign Up :::     \n"RESET);
        printf(    "Username :          \n");
        printf(    "Password :          \n");

        printf(UP UP);
        printf(    "Username : ");
        fgets(temp.username, sizeof(temp.username), stdin);
        temp.username[strlen(temp.username) - 1] = '\0';

        if (strcmp("admin", temp.username) == 0){
                printf(RED "\n Nuh uh! You cannot do that!          \n"RESET);
                back();
                return;
        }

        // Check if the username already exists
        while(fread(&record, sizeof(data), 1, user_data) == 1){
            if (strcmp(record.username, temp.username) == 0){
                printf(RED"\nUser with the same name already exists!\n"RESET);
                exist = true;
                break;
                }
            }
            
        if (exist) {
            printf(YEL"Quit? (Y/n)"RESET); if(confirm()) return;
            clear_screen();
            continue;
        }
        // If they just input enter
        if (strlen(temp.username) == 0) {
            printf(RED"\nPlease input your username!\n"RESET);
            printf(YEL"Quit? (Y/n)"RESET); if(confirm()) return;
            clear_screen();
        } else break;
    }
    fclose(user_data);

    // Password Input
    while(1){
        printf("Password : ");
        fgets(temp.password, sizeof(temp.password), stdin);
        temp.password[strlen(temp.password) - 1] = '\0';

        if (strlen(temp.password) == 0) {
            printf(RED"Password cannot be blank!\n"RESET);
            printf(YEL"Quit? (Y/n)"RESET); if(confirm()) return;

            clear_screen();
            printf(BLU"::: Sign Up :::\n"RESET);
            printf("Username : %s\n", temp.username);
        }else break;     
    }

    // Reset the counter
    temp.credits = 10;
    temp.banned  = false;
    memset(temp.counter, 0, sizeof(temp.counter));
    memset(temp.persist, 0, sizeof(temp.persist));

    printf(GRN"Registration completed!\n"RESET);
    mini_loading();

    // First time Setup
    while (1){
        clear_screen();
        printf( MAG "You must be new here :shake_hand:                  \n"RESET
                    "Welcome to your \"first time\" main menu!          \n"
                    "To be able to play the game, you need to transfer  \n"
                    "some amount of money first to us :3                \n\n"
                BLU "Enter the amount of money that you want : "        RESET"$"
        ); 
        if (num_input(&temp.balances)) break;
    }

    // If user input negative balances (they are in debt basically)
    if (temp.balances <= 0) 
         printf(MAG "Do you want to borrow " RED"$%i? " RESET"(Y/n) : ", temp.balances);
    else printf(MAG "Are you sure? "                    RESET"(Y/n)");
    
    if (!confirm()) {
        printf( "\nTransaction cancelled! "BLU"> v < \n"RESET);
        back();
        return;
    }
    
    printf(MAG  "\nTransfering..."RESET);
    loading();
    printf(BLU  "Thank you for registering \\(> v <)/\n" RESET
                "As for your registration, you will get "GRN"10 free credits!\n"RESET
    );
    back();

    // Write the data
    user_data    = fopen("data.dat", "ab");
    fwrite(&temp, sizeof(temp), 1, user_data);
    fclose(user_data);

    return;
}

void leaderboard(){
    FILE *user_data = fopen("data.dat", "rb");
    if (!check(user_data)) {back(); return;}
    else user_data = fopen("data.dat","rb");

    clear_screen();

    // Basically count how many player registered
    fseek(user_data, 0, SEEK_END);
    long size = ftell(user_data);  // the file size
    int total_users = size / sizeof(data); // by default, the size of data is 100 bytes

    // Array things [i]
    data *user =  malloc(size) ;
    rewind(user_data);
    fread(user, sizeof(data), total_users, user_data);
    fclose(user_data); // Close the file so it wont overwrite

    // Insertion Sort (from the highest)
    int temp, i, j;
    for (i=0; i <= total_users-1; i++){
        temp = user[i].counter[3]; // Gambling Counter
        data new = user[i];
        j = i;
        while (j > 0 && temp > user[j-1].counter[3]){
            user[j] = user[j - 1];
            j = j - 1;
        }
        user[j] = new;
    }

    printf(BLU"\t  ::: Leaderboard :::\n"RESET);
    printf(MAG  "Rank\tName\t\tGambled\tBalances\n"RESET);
    for (int i = 0; i < total_users; i++) {
        printf( "%2i." "\t%-10s %s" "\t" YEL"%-3i" "\t%s" "$%i" RESET "\n", 
                i + 1, user[i].username, (user[i].banned) ? RED"🔨"RESET : "",
                user[i].counter[3], (user[i].balances<= 0) ? RED : GRN, user[i].balances              
        );
    }
    back();
}

/* ====================== Game Menu ======================= */
void main_screen(){
    int opt;

    while (1){
        clear_screen();
        printf( BLU "Do you love gambling, "MAG"%s"RESET"? ""I do and You are in the right place! "
                    "Which game do you want to play?\n"
                    "[Balances : %s$%i"RESET"] " "[Credits : "MAG"%i"RESET"]\n"          
                    "1 : Slot                       \n"
                    "2 : Coin                       \n"
                    "3 : Random Number Generator    \n"
                YEL "7 : Leaderboard                \n"
                BLU "8 : Achievement                \n"
                GRN "9 : Buy Credits                \n"
                RED "0 : Exit                       \n"RESET
                    "Your input: ", 
                current.username, (current.balances<= 0) ? RED : GRN, current.balances, current.credits
        );
        if (!num_input(&opt)) continue;
        switch (opt){
            case 1:
                slot();
                break;
            case 2:
                coin();
                break;
            case 3:
                rng();
                break;
            case 7:
                leaderboard();
                break;
            case 8:
                achievement();
                break;
            case 9:
                market();
                break;
            case 0:
                clear_screen();
                printf(CURSOR_H);

                if (current.balances < 0) {
                    printf( RED "You are in debt for $%i!\n"
                                "Please pay it or else debt collector will haunt you > w <!!!\n\n"
                            RESET, current.balances);
                    }

                printf(MAG"Thank you for Playing!\n"RESET);
                printf(MAG"Exiting...\n"RESET);

                update_data();
                mini_loading();
                return;
            default:
                printf(RED"Please input correctly >w< !\n"RESET);
                back();
                break;
        }
    }    
}

void slot(){
    clear_screen();
    printf( BLU "::: Madoka Slot Machine!!! :::\n"RESET
                "Pretty much just your regular slot game\n"
                "Win condition : "MAG"Get atleast 2 matching pictures \n"RESET
                "Simple right?\n"
    );
    back();
    
    while(1){
        clear_screen();
        int value[3];
        printf(BLU  "::: Madoka Slot Machine!!! :::\n"RESET);
        printf(     "      Credit(s) left = %i\n", current.credits);

        // If your have no credit left
        if (current.credits == 0){
            printf("\nToo bad... but try "MAG"KEEP GAMBLING "RESET"again!\n");
            back();
            break;
        }
        
        // Rolling
        char *pic[10] = {
            MAG"🍇"RESET, GRN"🍉"RESET, MAG"🍆"RESET, CYN"🍪"RESET, RED"🐶"RESET, 
            YEL"🐱"RESET, CYN"🐻"RESET, BLU"🐟️"RESET, RED"🍎"RESET, GRN"🐢"RESET
        };
        char *roll[3];
        for (int i=0; i<3; i++){
            if (cheat[0] == 1)  
                 value[i] = 7; // Cheat code (why)
            else value[i] = rand() % (9); 

            roll[i] = pic[value[i]];             
        }


        printf( BLU"    +==================+\n"RESET
                BLU"    | "RESET"[%s]  [%s]  [%s] "BLU"|--"RED"O\n"RESET
                BLU"    +==================+\n"RESET,
                roll[0], roll[1], roll[2]
        );      

        // If you get [🐟️] [🐟️] [🐟️] le fishe
        if (value[0] == 7 && value[1] == 7 && value[2] == 7){
            printf( "Jackpot! You just win a "GRN"real estate!\n"RESET
                    GRN"$500 "RESET"has been added into your balances\n");
            current.balances += 500;
        }
        // If you get all the same pictures
        else if (value[0] == value[1] && value[0] == value[2] && value[1] == value[2]){
            printf( "Congrats, you just win "GRN"$50!\n"RESET);
            current.balances += 50;
        }
        // If you get atleast 2 pictures matching
        else if (value[0] == value[1] || value[0] == value[2] || value[1] == value[2]){
            printf( "Congrats, you just win "GRN"$20!\n"RESET);
            current.balances += 20;
        }
        else{
            printf("\n");
        }

        printf(YEL"Continue playing!? (Y/n) "RESET);
        if (!confirm()) break;

        current.credits--;
        current.counter[0]++;
    }

    update_data();
}

void coin(){
    clear_screen();
    printf( BLU "::: Coin Toss!!! :::\n"RESET
                "The classic coin toss\n"
                "Win Condition : "MAG"Just guess Head or tails\n"RESET
                "If you guess correct, you will get "GRN"$20\n"RESET
    ); 
    back();

    while(1){
        int coin = rand() % (2);
        int guess;
        printf(BLU "    ::: Coin Toss!!! :::\n"RESET);
        printf(    "     Credit(s) left = %i\n", current.credits);
        
        // If you have no credit left
        if (current.credits == 0){
            printf("\nToo bad... but try "MAG"KEEP GAMBLING "RESET"again!\n");
            back();
            break;
        }

        printf( "            [#]         \n");
        printf( CYN"     Input your guess!  \n" RESET
                "["YEL"Head 😼 (0)"RESET"|"RED"Tail 🐶 (1)"RESET"] : "
        );

        if (!num_input(&guess)) continue;
        if (guess != 0 && guess != 1){
            printf(RED"Please input correctly >w< !\n"RESET);
            back();
            continue;
        }

        // Reveal the guess
        char *pic[] = {YEL"😼"RESET, RED"🐶"RESET};
        char *icon  = pic[coin];

        printf(UP UP UP);
        printf( "            [%s]        \n", icon);
        printf( CYN"     Input your guess!  \n" RESET
                "["YEL"Head 😼 (0)"RESET"|"RED"Tail 🐶 (1)"RESET"] : %i \n",
                guess
        ); 
  
        // Cheat code (why)
        if (cheat[1] == 1){
            guess = coin;
        }
        // If correct
        if (guess == coin){
            printf("You just get "GRN"$20!\n"RESET);
            current.balances += 20;
        }
        else {
            printf(RED "Too bad...\n"RESET);
        }
        printf(YEL"Continue playing!? (Y/n) "RESET);
        if (confirm() == 0) break;

        current.credits--;
        current.counter[1]++;
        clear_screen(); 

    }

    update_data();
    return;
}

void rng(){
    clear_screen();
    printf( BLU "::: Random Number Guesser!!! :::\n"RESET
                "It is quite a simple game\n"
                "Win Condition : " MAG"Guess a number between 1-100\n"RESET
                "If you guess correctly, you will get "GRN"$500\n"RESET
                "If you guess 5 numbers above or bellow, you will get "GRN"$50\n"RESET
    );
    back();

    while(1){
        int num = rand() % (100) + 1;
        int guess;
        
        printf(BLU  "::: Random Number Guesser!!! :::\n"RESET);
        printf(     "Credit(s) left = %i\n", current.credits);

        // If you have no credit left
        if (current.credits == 0){
            printf("\nToo bad... but try "MAG"KEEP GAMBLING "RESET"again!\n");
            back();
            break;
        }

        printf("Input your guess : "); 
        if (!num_input(&guess)) continue;

        if (guess < 1 || guess > 100){
            printf(RED"\nPlease input correctly >w< !\n"RESET);
            back();
            continue;
        }
        // Cheat code (why)
        if (cheat[2] == 1){
            num = guess;
        }
        // If they are the same
        if (guess == num){
            printf("\nNo way, you are right on the money! You just get "GRN"$500!\n"RESET);
            current.balances += 500;
        }
        // If +- 5 differences
        else if (guess >= num-5 && guess <= num+5){
            printf("\nYou are just "MAG"%i "RESET"number away!\n", num - guess);
            printf("The actual number = "MAG"%i\n"RESET, num);
            printf("You just get "GRN"$50!\n"RESET);
            current.balances += 50;
        } 
        // If miss
        else {
            printf( RED "\nToo bad...\n"RESET);
            printf(     "The actual number = "MAG"%i\n"RESET, num);
        }

        printf(YEL"Continue playing!? (Y/n) "RESET);
        if (confirm() == 0) break;

        current.credits--;
        current.counter[2]++;
        clear_screen();
    }

    update_data();
    return;
}

void market(){
    int add, diff, cost, change;

    clear_screen();
    printf( BLU "::: Market!!! :::\n"RESET
            BLU "Uhmmm... hi > w < !!!\n"RESET
                "Hh-how much do you want to pay T-T ?\n"
                "Credits "GRN"(1 = $10) : "RESET
    );
    if (!num_input(&add)) return;  
    cost    = 10 * add;
    change  = current.balances - cost;

    if (add < 0){
        printf(RED"Nuh uh... Too bad you can't do that (·ω·)\n"RESET);
        back();
        return;
    }else if (add == 0){
        printf("Transaction cancelled! "BLU"> v < \n"RESET);
        back();
        return;
    }
    else if (change < 0){
        if (current.balances >= 0) 
             diff = cost - current.balances; // Non-negative balances
        else diff = cost; // Negative balances

        printf( MAG "Do you want to borrow " RED"$%i?"RESET" (Y/n) : ", diff);
    }
    else printf(MAG"Are you sure? "RESET"(Y/n)");

    if (confirm() == 0) {
        printf("Transaction cancelled! "BLU"> v < \n"RESET);
        back();
        return;
    }

    clear_screen();

    printf(MAG"Transfering..."RESET);
    current.credits += add;
    current.balances -= 10 * add;
    loading();
    
    printf(GRN "Money transfered!"RESET" Current balances : %s$%i\n"RESET
               "Erm... please gamble responsibly T-T...\n",       
               (current.balances <= 0) ? RED : GRN, current.balances
    );

    update_data();
    back();
    return;
}

void achievement(){
    bool track      = false;
    int  counter[]  = {10, 50, 100};
    char *colors[]  = {GRN, MAG, RED};
    char *game[]    = {"Slot","Coin Toss","Random Number Guesser"};

    clear_screen(); 
    printf(MAG"Your achievement :\n"RESET);

    for(int i=0; i < 3; i++){
        for(int j=0; i < 3; i++){
            if (current.counter[i] >= counter[j]) {
                printf( "- Play "BLU"%s"RESET" for %s%i times\n"RESET, 
                        game[i], colors[j], counter[j]
                );
                track = true;
            }
        }
    }
    if (current.balances <= -100 || current.persist[0] == true){
        printf("- "MAG"Debt Collector's Ghost "RESET": In "RED"debt"RESET" for more than "RED"$100\n"RESET);
        current.persist[0] = true;
        track = true;
    }
    if (current.balances >= 1000 && current.counter[3] >= 100 || current.persist[1] == true){
        printf("- "MAG"Masterful Gambit "RESET": Earned"GRN" $1000\n"RESET);
        current.persist[1] = true;
        track = true;
    }
    if (current.counter[3] >= 100 || current.persist[2] == true){
        printf("- "MAG"A True Gambler Mindset "RESET": Gambled for more than "GRN"100 times\n"RESET);
        current.persist[2] = true;
        track = true;
    }

    if (!track) printf(RED"There's no achievement yet ('A`)\n"RESET);

    update_data();
    back();
}

/* ===================== Admin Menu ======================= */
void admin_screen(){
    int opt;
    void ascii();

    while(1){
        clear_screen();
        printf( MAG "Hello Mr. Admin!\n"
                    "What do you want to select? >///<\n"RESET
                    "1. View Registered user\n"
                    "2. Ban User\n"
                    "3. Unban User\n"
                    "4. Remove User\n"
                GRN "8. Cheat\n"
                // BLU "9. ASCII art\n"
                RED "0. Exit\n"RESET
                    "Your input : ");
        if (!num_input(&opt)) continue;
        switch (opt){
            case 1 :
                view();
                back();
                break;
            case 2 :
                banned();
                break;
            case 3 :
                unbanned();
                break;
            case 4 :
                nuke();
                break;
            case 8 :
                cheat_screen();
                break;
            case 9 :
                ascii();
                break;
            case 0 :
                clear_screen();
                printf(MAG"Exiting...\n"RESET);
                mini_loading();
                return;
            default :
                printf(RED"Please input correctly >w< !\n"RESET);
                back();
                break;
        }
    }
}

bool view(){
    data user;
    FILE *user_data = fopen("data.dat", "rb");

    clear_screen();
    if (!check(user_data)) return false;
    else user_data = fopen("data.dat","rb");

    printf(MAG"List of registered user :\n"RESET);
    int i = 1;
    while(fread(&user, sizeof(data), 1, user_data) == 1){
        printf("%i. Username  : %s", i, user.username);
        printf("%s\n", (user.banned) ? RED" (banned)"RESET : "");
        printf("   Password  : %s\n", user.password);
        printf("   Balances  : %s$%i\n"RESET, (user.balances <= 0) ? RED : GRN, user.balances);
        printf("   Credits   : %i\n", user.credits);
        printf("\n");
        i++;
    }

    fclose(user_data);
    return true;
}

void banned(){
    FILE *user_data = fopen("data.dat", "rb");
    char input[32+1];
    char message[254+1];

    // View Registered user
    if (!view()) {back(); return;}

    // Name input
    printf(RED"Which user do you want to be banned?\n"RESET);
    printf(YEL"Your input : "RESET); 
    fgets(input, sizeof(input), stdin);
    input[strlen(input) - 1] = '\0';

    bool exist = false;
    rewind(user_data);
    while(fread(&record, sizeof(record), 1, user_data) == 1){
        if (strcmp(record.username, input) == 0){
            exist = true;
            current = record;
            current.banned = true;

            printf(MAG"\nReason for ban?\n"RESET);
            printf(YEL"Your input : "RESET);
            fgets(message, sizeof(message), stdin);
            message[strlen(message) - 1] = '\0';

            printf(MAG"Are you sure? "RESET"(Y/n)");
             if (!confirm()) {
                printf( "\nBanning cancelled! "BLU"> v < \n"RESET);
                back();
                return;
            }

            strcpy(current.message,message);
            update_data();
            break;
        }
    }
    if (exist)  printf(GRN"\nSuccessfully banning "RESET"\"%s\"\n", input);
    else        printf(RED"\nThere is no user with that name! (>_<)\n"RESET);

    fclose(user_data);
    back();
}

void unbanned(){
    FILE *user_data = fopen("data.dat", "rb");
    char input[32+1];
    char message[254+1];

    // View Registered user
    if (!view()) {back(); return;}

    // Name input
    printf(BLU"Which user do you want to be unbanned?\n"RESET);
    printf(YEL"Your input : "RESET); 
    fgets(input, sizeof(input), stdin);
    input[strlen(input) - 1] = '\0';

    bool exist = false;
    rewind(user_data);
    while(fread(&record, sizeof(record), 1, user_data) == 1){
        if (strcmp(record.username, input) == 0){
            exist = true;
            if (record.banned == true){
                current = record;
                current.banned = false;

                printf(MAG"Are you sure? "RESET"(Y/n)");
                if (!confirm()) {
                    printf( "\nBanning cancelled! "BLU"> v < \n"RESET);
                    back();
                    return;
                }

                printf(GRN"\nSuccessfully unbanning "RESET"\"%s\"\n", input);
                memset(current.message, 0 , sizeof(current.message));
                update_data();
                break;
            }
            else {
                printf(BLU"\nUser isn't banned currently!\n"RESET);
            }
        }
    }
    if (!exist) printf(RED"\nThere is no user with that name! (>_<)\n"RESET); 

    fclose(user_data);
    back();
}

void nuke(){
    FILE *user_data = fopen("data.dat", "rb");
    FILE *temp = fopen("data_temp.dat", "wb");

    // View Registered user
    if (!view()) {back(); return;}

    // Name input
    char input[32+1];
    printf(RED"Which user do you want to nuke?\n"RESET);
    printf(YEL"Your input : "RESET); 
    fgets(input, sizeof(input), stdin);
    input[strlen(input) - 1] = '\0';

    // Delete the user data
    bool exist = false;
    rewind(user_data);
    while(fread(&record, sizeof(record), 1, user_data) == 1){
        if (strcmp(record.username, input) == 0){
            exist = true;
        }
        if (strcmp(record.username, input) != 0){
            fwrite(&record, sizeof(record), 1, temp);
        }
    }
    if (exist){
        nuke_loading();
        printf(GRN"\nSuccessfully deleting "RESET"\"%s\"\n", input);
    } else{
        printf(RED"\nThere is no user with that name! (>_<)\n"RESET);
    }

    fclose(temp);
    fclose(user_data);

    remove("data.dat");
    rename("data_temp.dat","data.dat");

    back();
}

void cheat_screen(){
    int opt;
    while(1){
        clear_screen();
        printf( MAG"Which cheat do you want to apply?\n"
            GRN "1 : Change the amount of credit\n"RESET
                "2 : Always win on slot\n"
                "3 : Always win on coin toss\n"
                "4 : Always win on rng\n"
            RED "0 : Return to previous Selection\n"RESET
                "Your input: "
        );
        if (!num_input(&opt)) continue;
        switch (opt){
            case 1 :
                cheat_credits();
                break;
            case 2 :
            case 3 :
            case 4 :
                cheat_games(opt);
                break;
            case 0 :
                return;
            default :
                printf(RED"Please input correctly >w< !\n"RESET);
                back();
                break;
        }
    }
}

void cheat_credits(){
    int new;
    bool found = false;
    FILE *user_data = fopen("data.dat", "rb+");

    clear_screen();
    if (!view()) {back(); return;}
    if (!check(user_data)) return;
    else user_data = fopen("data.dat","rb+");

    printf(MAG"Which user do you want to chose? "RESET);
    fgets(temp.username, sizeof(temp.username), stdin);
    temp.username[strlen(temp.username) - 1] = '\0';

    while(fread(&record, sizeof(data), 1, user_data) == 1){
        if (strcmp(record.username, temp.username) == 0){
            current = record;
            found = true;
        }
    }
    if (!found) {
        printf(RED"\nSorry, user doesnt exist!\n"RESET);
        fclose(user_data);
        back();
        return;
    }

    printf("\nNew credits (previous = %i) : ", current.credits);
    num_input(&new);

    current.credits += new;
    printf("Credits changed to"GRN" %i\n"RESET, current.credits);
    update_data();
    back();
}

void cheat_games(int input){
    int i;

    if      (input == 2) i = 0;
    else if (input == 3) i = 1;
    else    i = 2;

    char *sel[] = {"Slot", "Coin Toss", "RNG"};

    clear_screen();
    printf( RED "[!] "MAG"The change is in effect until the program is closed"RED" [!]\n"RESET);
    printf( "Selected : "BLU"%s  \n"RESET
            "Current value: %s     \n",
            sel[i], (cheat[i]) ? BLU"True"RESET : RED"False"RESET);
    printf(YEL "Change? (Y/n) : "RESET);

    if (confirm()) {
        // Toogle cheat
        if (cheat[i] == false){
            cheat[i]    = true;
        }else cheat [i] = false;

        printf(UP UP);
        printf( "Current value: %s     \n",
            (cheat[i]) ? BLU"True"RESET : RED"False"RESET
        );
        printf(YEL "Change? (Y/n) : Y            \n\n"RESET);
        printf(MAG "Successfully changed the value!\n"RESET);
    } else{
        printf( RED"\nNo change was made...\n"RESET);
    }
    back();
}

void ascii(){
    FILE * ascii = fopen("ascii.txt", "r");
    char * stat; // Check whether the line reach EOF
    char text[255];
    
    clear_screen();

    if(!ascii){
        printf(RED"No ascii.txt file was found\n"RESET);
        back();
        return;
    }

    // Basically print every line in ascii.txt file
    while(stat){
        stat = fgets(text, sizeof(text), ascii);
        printf("%s", text);
    }

    fclose(ascii);
    back();
}

void update_data(){
    FILE *user_data = fopen("data.dat", "rb+");

    // Update the gambling counter
    current.counter[3] = current.counter[0] + current.counter[1] + current.counter[2];

    while (fread(&record, sizeof(data), 1, user_data) == 1) {
        if (strcmp(record.username, current.username) == 0) {
            fseek(user_data, -sizeof(data), SEEK_CUR);
            fwrite(&current, sizeof(data), 1, user_data); 
            break;
        }
    }
    fclose(user_data);
}
