// Library
#include <string.h>
#include <time.h>
#include "qol.h"

// Options
int cheat[3] = {0};
int auth;

//Personal user data (see who's the true gambler)
typedef struct{
    char username[32+1];
    char password[32+1];
    int balances;
    int credits;
    int counter[4]; // Gambling counter
    int persist[3]; // Related to achievement
} data;
data current;
data temp;
data record;

/* Game Center! See who's the most gamble addicted person! */

/* Start Menu */
void start_screen();
void login();
void sign_up();
void leaderboard();
/* Game Menu */ 
void main_screen();
void slot();
void coin();
void rng();
void market();
void achievement();
void hidden();
/* Admin */
void admin_screen();
int view();
void nuke();
void update_data();

/* Login Menu */
int main(){
    srand(time(NULL)); // Start the gambling mode
    start_screen();
    return 0; 
}

void start_screen(){
    int input;
    while(1){
        system("clear");
        printf( MAG "Welcome to Game Center, where you can play numerous of games!\n"RESET
                BLU "User, please select one of these option!\n"RESET
                    "1. Login\n"
                    "2. Sign in\n"
                    "3. Leaderboard\n"
                RED "0. Exit\n"RESET
                    "Your input : ");
        scanf("%i", &input); getchar();
        switch (input){
            case 1 :
                login();
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
                system("clear");
                printf(MAG"See you next time!"RESET);
                return;
            default :
                printf(RED"Invalid Input!\n"RESET);
                back();
                break;
        }
    }
}

void login(){
    FILE *user_data;
    auth = 0;

    system("clear");

    printf(BLU"::: Login :::\n"RESET);
    printf("Username :\n");
    printf("Password : \n");

    printf(UP UP"Username : ");
    fgets(temp.username, sizeof(temp.username), stdin);
    temp.username[strlen(temp.username) - 1] = '\0';

    printf("Password : ");
    fgets(temp.password, sizeof(temp.password), stdin);
    temp.password[strlen(temp.password) - 1] = '\0';

    // Match username and password with in the database
    int *ptr = &auth;
    int found;

    // Admin login
    if (strcmp(temp.username, "admin") == 0){
        if (strcmp(temp.password, "123") == 0){
            printf(BLU"\n[Entering as an admin...]\n"RESET);
            *ptr = 2;
        } else{
            printf(RED"\nSorry you are not authorized! T_T\n"RESET);
            *ptr = 0;
        }
        back();
        return;
    }
    
    // User login
    user_data = fopen("data.dat","rb");
    if (!user_data) {
        printf(RED"There is no one yet (>_<)\n"RESET);
        back();
        return;
    }
    // Check if the user input is the same as recorded one
    while(fread(&record,sizeof(data), 1, user_data) == 1){
        if (strcmp(record.username, temp.username) == 0){
            found = 1;
            // Check the password
            if (strcmp(record.password, temp.password) == 0){
                printf(BLU"Login Successfull!\n"RESET);
                current = record; 
                *ptr = 1;
            } else{
                printf(RED"Password incorrect!\n"RESET);
                *ptr = 0;
            }       
        }
    }

    if (found != 1){
        printf(RED"User not found!\n"RESET);
        *ptr = 0;
    }
    fclose(user_data);
    back();
    return;
}

void sign_up(){
    FILE *user_data = fopen("data.dat", "rb");
    
    system("clear");

    // Create a new binary if it doesn't exist yet
    if (!user_data) {
        user_data = fopen("data.dat", "wb");
        fclose(user_data);
        fopen("data.dat", "rb+");
    }

    // Username Registration
    while (1){
        int exist = 0;
        
        printf(BLU"::: Sign Up :::\n"RESET);
        printf("Username :\n");
        printf("Password :\n");

        printf(UP UP"Username : ");
        fgets(temp.username, sizeof(temp.username), stdin);
        temp.username[strlen(temp.username) - 1] = '\0';

        // Check if the username already exists
        while(fread(&record, sizeof(data), 1, user_data) == 1){
            if (strcmp(record.username, temp.username) == 0){
                printf(RED"\nUser with the same name already exists!\n"RESET);
                exist = 1;
                break;
            }
            if (strcmp("admin", temp.username) == 0){
                printf(RED"\nNuh uh! You cannot do that!\n"RESET);
                exist = 1;
                break;
            }
        }
        if (exist) {
            printf("\nQuit? (Y/n)"); if(confirm()) return;
            system("clear");
            continue;
        }
        // If they just input enter
        if (strlen(temp.username) == 0) {
            printf(RED"Please input your username!\n"RESET);
            printf("Quit? (Y/n)"); if(confirm()) return;
            system("clear");
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
            printf("Quit? (Y/n)"); if(confirm()) return;
            system("clear");
            printf(BLU"::: Sign Up :::\n"RESET);
            printf("Username : %s\n", temp.username);
        }else break;     
    }

    // Reset the counter
    memset(temp.counter, 0, sizeof(temp.counter));
    memset(temp.persist, 0, sizeof(temp.persist));

    printf(GRN"Registration completed!\n"RESET);
    fflush(stdin); sleep(1);

    // First time Setup
    system("clear");
    printf( MAG "You must be new here :shake_hand:\n"RESET
                "Welcome to your \"first time\" main menu!\n"
                "To be able to play the game, you need to transfer\n"
                "some amount of money first to us :3\n\n"
            BLU "Enter the amount of money that you want : "RESET"$"
    ); 
    scanf("%d", &temp.balances); getchar();
    
    printf(MAG  "Transfering..."RESET);
    loading();
    printf(BLU  "Thank you for registering \\(>∇<)/\n" RESET
                "As for your registration, you will get "GRN"10 free credits!\n"RESET
    );
    back();

    // Write the data
    temp.credits = 10;
    user_data = fopen("data.dat", "ab");
    fwrite(&temp, sizeof(temp), 1, user_data);
    fclose(user_data);

    return;
}

void leaderboard(){
    FILE *user_data = fopen("data.dat", "rb");
    if (!user_data ) {
        printf(RED"There is no one yet (>_<)\n"RESET);
        back();
        return;
    }

    system("clear");

    // Basically count how many player registered
    fseek(user_data, 0, SEEK_END);
    long size = ftell(user_data);  // the file size
    fseek(user_data, 0, SEEK_SET);
    int total_users = size / sizeof(data); // by default it is 100 bytes

    // Array things [i]
    data *user =  malloc(size) ;
    fread(user, sizeof(data), total_users, user_data);
    fclose(user_data); // Close the file so it wont overwrite

    // Insertion Sort (from the highest)
    for (int i = 0; i <= total_users-1; i++){
        int temp = user[i].counter[3]; // Gambling Counter
        data new = user[i];
        int j = i;
        while (j > 0 && temp > user[j-1].counter[3]){
            user[j] = user[j - 1];
            j = j - 1;
        }
        user[j].counter[3] = temp;
        user[j] = new;
    }

    printf(BLU"\t  ::: Leaderboard :::\n"RESET);
    printf(MAG  "Rank\tName\t\tGambled\tBalances\n"RESET);
    for (int i = 0; i < total_users; i++) {
        printf( "%2i." "\t" "%-10s" "\t" YEL"%-3i" "\t" "%s" "$%i\n"RESET, 
                i + 1, user[i].username, user[i].counter[3], 
                (user[i].balances<= 0) ? RED : GRN, user[i].balances
        );
    }
    back();
}

/* Game Menu */ 
void main_screen(){
    int input;
    
    while (1){
        system("clear");
        
        printf( BLU "Do you love gambling, "MAG"%s"RESET"? ""I do and You are in the right place! "
                    "Which game do you want to play?\n"
                    "[Balances : %s$%i"RESET"]" " [Credits : "MAG"%i"RESET"]\n"          
                    "1 : Slot\n"
                    "2 : Coin\n"
                    "3 : Random Number Generator\n"
                BLU "8 : Achievement\n"RESET
                GRN "9 : Buy Credits\n"RESET
                RED "0 : Exit\n"RESET
                    "Your input: ", 
                current.username, (current.balances<= 0) ? RED : GRN, current.balances, current.credits
        );
        
        scanf("%i", &input); getchar();
        switch (input){
            case 1:
                slot();
                break;
            case 2:
                coin();
                break;
            case 3:
                rng();
                break;
            case 8:
                achievement();
                break;
            case 9:
                market();
                break;
            case 0:
                system("clear");
                printf(CURSOR_H);
                if (current.balances < 0) {
                    printf( RED "You are in debt for $%i!\n"
                                "Please pay it or else debt collector will haunt you > w <!!!\n"
                            RESET, current.balances);
                    }
                printf(MAG"Thank you for Playing!\n"RESET);
                printf(MAG"Exiting...\n"RESET);
                update_data();
                sleep(1);
                printf(CURSOR_E);
                return;
            case 727:
                hidden();
                break;
            default:
                printf(RED"Invalid option inputted, Please select the correct one...\n"RESET);
                back();
                break;
        }
    }    
}

void slot(){

    system("clear");
    printf( BLU "::: Madoka Slot Machine!!! :::\n"RESET
                "Pretty much just your regular slot game\n"
                "Win condition : "MAG"Get atleast 2 matching number \n"RESET
                "Simple right?\n"
    );
    back();
    
    while(1){
        system("clear");
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
        for (int i=0; i<3; i++){
            if (cheat[0] == 1) value[i] = 7; // Cheat code (why)
            else value[i] = rand() % (9); 
        }
        printf( BLU"    ===================\n"RESET
                BLU"    |  "RESET"[%i]  [%i]  [%i]  "BLU"|--"RED"O\n"RESET
                BLU"    ===================\n"RESET,
                value[0], value[1], value[2]
        );      

        // If you get [7] [7] [7]
        if (value[0] == 7 && value[1] == 7 && value[2] == 7){
            printf( "Jackpot! You just win a "GRN"real estate!\n"RESET
                    GRN"$500 "RESET"has been added into your balances\n");
            current.balances += 500;
        }
        // If you get all the same numbers
        else if (value[0] == value[1] && value[0] == value[2] && value[1] == value[2]){
            printf( "Congrats, you just win "GRN"$50!\n"RESET);
            current.balances += 50;
        }
        // If you get atleast 2 numbers matching
        else if (value[0] == value[1] || value[0] == value[2] || value[1] == value[2]){
            printf( "Congrats, you just win "GRN"$20!\n"RESET);
            current.balances += 20;
        }
        else{
            printf("\n");
        }

        printf("Continue playing!? (Y/n) ");
        if (confirm() == 0) break;

        current.credits--;
        current.counter[0]++;
    }
    update_data();
}

void coin(){

    system("clear");
    printf( BLU "::: Coin Toss!!! :::\n"RESET
                "The classic coin toss\n"
                "Win Condition : "MAG"Just guess Head or tails\n"RESET
                "If you guess correct, you will get "GRN"$20\n"RESET
    ); 
    back();

    while(1){
        int coin = rand() % (2);
        int guess;
        printf(BLU  "::: Coin Toss!!! :::\n"RESET);
        printf(     "Credit(s) left = %i\n", current.credits);
        
        // If you have no credit left
        if (current.credits == 0){
            printf("\nToo bad... but try "MAG"KEEP GAMBLING "RESET"again!\n");
            back();
            break;
        }

        printf(     "Input your guess\n" 
                YEL "[Head(0)/Tail(1)] : "RESET); 
        scanf("%i", &guess); getchar();
        if (guess != 0 && guess != 1){
            printf(RED"Please input correctly >w< !\n"RESET);
            back();
        }
        else{
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
                printf( RED "Too bad...\n"RESET
                            "The actual face = "MAG"%s\n"RESET, 
                        (coin == 0) ? "Head" : "Tail"  ); 
            }
            printf("Continue playing!? (Y/n) ");

            if (confirm() == 0) break;

            current.credits--;
            current.counter[1]++;
            system("clear"); 
        }
    }
    update_data();
    return;
}

void rng(){

    system("clear");
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
        scanf("%i", &guess); getchar();

        if (guess < 1 || guess > 100){
            printf(RED"\nPlease input correctly >w< !\n"RESET);
            back();
        } else {
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
            printf("Continue playing!? (Y/n) ");
            if (confirm() == 0) break;

            current.credits--;
            current.counter[2]++;
            system("clear");
        }
    }
    update_data();
    return;
}

void market(){

    system("clear");
    printf( BLU "::: Market!!! :::\n"RESET
            BLU "Uhmmm... hi > w < !!!\n"RESET
                "Hh-how much do you want to pay T-T ?\n"
                "Credits "GRN"(1 = $10) : "RESET
    );
    int add;
    scanf("%i", &add); getchar();   
    int cost = 10 * add;
    int change = current.balances - cost;

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
        int diff; 
        if (current.balances >= 0) diff = cost - current.balances; // Non-negative balances
        else diff = cost; // Negative balances
        printf( MAG "Do you want to borrow " RED"$%i?"RESET" (Y/n) : ", diff);
    }else printf(MAG"Are you sure? "RESET"(Y/n)");

    if (confirm() == 0) {
        printf("Transaction cancelled! "BLU"> v < \n"RESET);
        back();
        return;
    }
    system("clear");

    printf(MAG"Transfering..."RESET);
    loading();
    current.credits += add;
    current.balances -= 10 * add;
    
    printf(GRN  "Money transfered!"RESET" Current balances : ");
    printf(     "%s$%i\n"RESET, (current.balances <= 0) ? RED : GRN, current.balances);
    printf(     "Erm... please gamble responsibly T-T...\n");
    update_data();
    back();
    return;
}

void hidden(){
    int input, i;

    while(1){
        system("clear");
        printf( "You just opened "MAG"hidden options!\n"RESET"What option do you want?\n"
            GRN "1 : Change the amount of credit\n"RESET
                "2 : Always win on slot\n"
                "3 : Always win on coin toss\n"
                "4 : Always win on rng\n"
            RED "0 : Return to previous Selection\n"RESET
                "Your input: "
        );
        scanf("%i", &input); getchar();
        if (input == 1){
            int new;
            printf("\nNew credits (previous = %i) : ", current.credits);
            scanf("%i", &new); getchar();
            current.credits += new;
            printf("Credits changed to"GRN" %i\n"RESET, current.credits);
            back();
        } else if (input == 2 || input == 3 || input == 4){
            if (input == 2) i = 0;
            else if(input == 3) i = 1;
            else i = 2;
            printf("\nPrevious value: %s\n", (cheat[i] == 1) ? BLU"True"RESET : RED"False"RESET);
            printf("Change? (Y/n) : ");
            if (confirm() == 1) {
                if (cheat[i] == 0){
                    cheat[i] = 1;
                }else cheat [i] = 0;
                printf("Successfully changed to %s\n", (cheat[i] == 1) ? BLU"True"RESET : RED"False"RESET);
            } else{
                printf(RED"No change is made...\n"RESET);
            }
            back();
        } else if (input == 0) return;
        else {
            printf(RED"Please choose the correct option > w <\n"RESET);
            back();
        }
    }
    update_data();
}

void achievement(){
 
    int track = 0;
    system("clear"); 

    printf(MAG"Your achievement :\n"RESET);
    char *game[] = {"Slot","Coin Toss","Random Number Generator"};

    for(int i=0; i < 3; i++){
        if (current.counter[i] >= 10) {
            printf("- Play "BLU"%s"RESET" for "GRN"10 times\n"RESET, 
            game[i]);
            track = 1;
        }
        if (current.counter[i] >= 50) {
            printf("- Play "BLU"%s"RESET" for "MAG"50 times\n"RESET, 
            game[i]);
            track = 1;
        }
        if (current.counter[i] >= 100) {
            printf("- Play "BLU"%s"RESET" for "RED"100 times\n"RESET, 
            game[i]);
            track = 1;
        }
    }
    if (current.balances <= -100 || current.persist[0] == 1){
        printf("- "MAG"Debt Collector's Ghost "RESET": In "RED"debt"RESET" for more than "RED"$100\n"RESET);
        current.persist[0] = 1;
        track = 1;
    }
    if (current.balances >= 1000 && current.counter[3] >= 100|| current.persist[1] == 1){
        printf("- "MAG"Masterful Gambit "RESET": Earned"GRN" $1000\n"RESET);
        current.persist[1] = 1;
        track = 1;
    }

    if (current.counter[3] >= 100){
        printf("- "MAG"A True Gambler Mindset "RESET": Gambled for more than "GRN"100 times\n"RESET);
        current.persist[1] = 1;
        track = 1;
    }

    if (track == 0){
        printf(RED"There's no achievement yet ('A`)\n"RESET);
    }
    update_data();
    back();
}

/* Admin */
void admin_screen(){
    void idk();

    int input; 
    while(1){
        system("clear");
        printf( MAG "What do you want?\n"RESET
                    "1. View Registered user\n"
                    "2. Remove User\n"
                    "3. Idk\n"
                RED "0. Exit\n"RESET
                    "Your input : ");
        scanf("%i", &input); getchar();
        switch (input){
            case 1 :
                view();
                back();
                break;
            case 2 :
                nuke();
                break;
            case 3 :
                idk();
                break;
            case 0 :
                system("clear");
                printf(MAG"Exiting..."RESET);
                fflush(stdout); sleep(1);
                return;
            default :
                printf(RED"Invalid Input!\n"RESET);
                back();
                break;
        }
    }
}

int view(){
    data user;
    FILE *user_data = fopen("data.dat", "rb");

    system("clear");
    if (!user_data ) {
        printf(RED"There is no one yet (>_<)\n"RESET);
        return 1;
    }

    printf(MAG"List of registered user :\n"RESET);
    int i = 1;
    while(fread(&user, sizeof(data), 1, user_data) == 1){
        printf("%i. Username  : %s\n", i, user.username);
        printf("   Password  : %s\n", user.password);
        printf("   Balances  : $%i\n", user.balances);
        printf("\n");
        i++;
    }

    fclose(user_data);
    return 0;
}

void nuke(){
    FILE *user_data = fopen("data.dat", "rb");
    FILE *temp = fopen("data_temp.dat", "wb");

    // View Registered user
    if (view()) {back(); return;}

    // Name input
    char input[32+1];
    printf(RED"Which user you want to nuke?\n"RESET);
    printf(YEL"Your input : "RESET); 
    fgets(input, sizeof(input), stdin);
    input[strlen(input) - 1] = '\0';

    // Delete the user data
    rewind(user_data);
    while(fread(&record, sizeof(record), 1, user_data) == 1){
        if (strcmp(record.username, input) != 0){
            fwrite(&record, sizeof(record), 1, temp);
        }
    }

    nuke_loading();
    printf(GRN"\nSuccessfully deleting "BLU"\"%s\"\n"RESET, input);

    fclose(temp);
    fclose(user_data);

    remove("data.dat");
    rename("data_temp.dat","data.dat");

    back();
}

void idk(){
    system("clear");
    printf("                               --.+.                             \n");
    printf("                    -.----++############        -                \n");
    printf("              .---------------.-----#########                    \n");
    printf("          .  .---.----------------------+#######                 \n");
    printf("        .   .-----------------------.-----.-#######              \n");
    printf("        ..-------------- ---------------------#######            \n");
    printf("      ..-.------ - -.--- ---.-------------------########         \n");
    printf("    .. .------- - -.---   ---..-- ---------------#######  #      \n");
    printf("    .  -----.----------  ----- -------------------########       \n");
    printf("   .   ---------.------ -+.---. -------------------##### ## #    \n");
    printf("     .----------------- +++ --.-----.--.- ----------####  ###    \n");
    printf("    .---------- -------.++--- -+--+----- ------------####  ##    \n");
    printf("  ..-.--------- .--+---.-++--.+.. +++---- .----------+###  ##    \n");
    printf(".   .----.-.-...+--..---.---.-++++++++ --. -----.-----###  ##.   \n");
    printf("   - ----.-..  ..+--.-----++++++--.--++    ------------######    \n");
    printf("     ----.-.- -+++++++----------           -------------#####    \n");
    printf("  - -----.---+-+-------------------     -. --------------####    \n");
    printf("    ------.---       ------------------#  .--------------## #    \n");
    printf(" .  ------.. .-  .---------------------+.-.------ + ------##  ## \n");
    printf(" -- -------.-  .----------------++------.--------  ++------+#    \n");
    printf(" #-  ------- -#.------+-++++ -----+++++-.-------. ++.----+--+#-  \n");
    printf("  +- -------.+#-----+++ ---------- +++++.+-------    ----#---####\n");
    printf("   #- ----- ..+----++++------------+++++.+------+   -----#----# +\n");
    printf("     ----.- .. +-++++++ ---.---.. ++++++- ------#    ----#+----# \n");
    printf("     #---.--   -++++++++++++++++++++++--+-------#    ----##-----#\n");
    printf("     #.------   -++++++++++++++++++++- + ------##     ----##-----\n");
    printf("     #.------     .- -++++++++++++- +.  .------##     ----## .---\n");
    printf("     . .-----.                          -- ---##      .- -+##   -\n");
    printf("    #   ------                          ------##       .---###   \n");
    printf("    +.  ------                   ...... - ---+##        .---###  \n");
    printf("   #--  ------              ... ........-.---##+          ---####\n");
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