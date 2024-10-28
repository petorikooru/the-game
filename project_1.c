#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Color for the text
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define RESET "\x1B[0m"
#define CURSOR_H "\e[?25l"
#define CURSOR_E "\e[?25h"

// Cheat options
int cheat[3] = {0};

//Initial value
int balance = 100;
int credits = 20;
int opt;  // Yes or No selection
int counter[3] = {0}; // Gambling counter
int persist[3];  // Related to achievement

void slot();
void coin();
void rng();
void market();
void passcode();
void hidden();
void back();
void option();
void achievement();

void main(){
    int input;
    while (1){
        system("clear");
        srand(time(NULL)); // Start the gambling mode
        
        printf( BLU"Do you love gambling? "RESET"I do and You are in the right place!\n"
                "Which game do you want to play?\n"
                "[Your current balances : %s$%i"RESET"] ", (balance <= 0) ? RED : GRN, balance);
        printf( "[Your current credits : "MAG"%i"RESET"]\n"            
                "1 : Slot\n"
                "2 : Coin\n"
                "3 : Random Number Generator\n"
                BLU"8 : Achievement\n"RESET
                GRN"9 : Buy credits\n"RESET
                RED"0 : Exit\n"RESET
                "Your input: ", credits);
        
        scanf("%i", &input);
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
                if (balance < 0) {
                    printf( RED"You are in debt for $%i!\n"
                            "Please pay it or else debt collector will haunt you > w <!!!\n"
                            RESET, balance);
                    }
                    printf(MAG"Thank you for Playing!\n"RESET);
                    printf(MAG"Exiting..."RESET);
                return;
            case 727:
                passcode();
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
    printf( BLU"::: Madoka Slot Machine!!! :::\n"RESET
            "Pretty much just your regular slot game\n"
            "Win condition : "MAG"Get atleast 2 matching number \n"RESET
            "Simple right?\n"
    );
    back();
    
    while(1){
        int value[3];
        printf(BLU"::: Madoka Slot Machine!!! :::\n"RESET);
        printf("Credit(s) left = %i\n", credits);

        // If your have no credit left
        if (credits == 0){
            printf("\nToo bad... but try "MAG"KEEP GAMBLING "RESET"again!\n");
            break;
        }
        
        // Rolling
        printf( BLU"===================\n"RESET
                BLU"|  "RESET);

        for (int i=0; i<3; i++){
            if (cheat[0] == 0){
                value[i] = rand() % (9);
            }else {
                value[i] = 7; // Cheat code (why)
            }
            printf("[%i]  ",value[i]);   
        }

        printf( BLU"|--"RED"O\n"RESET
                BLU"===================\n"RESET);        

        // If you get [7] [7] [7]
        if (value[0] == 7 && value[1] == 7 && value[2] == 7){
            printf( "Jackpot! You just win a "GRN"real estate!\n"RESET
                    GRN"$100 "RESET"has been added into your balance\n");
            balance += 100;
        }
        // If you get all the same numbers
        else if (value[0] == value[1] && value[0] == value[2] && value[1] == value[2]){
            printf( "Congrats, you just win "GRN"$50!\n"RESET);
            balance += 50;
        }
        // If you get atleast 2 numbers matching
        else if (value[0] == value[1] || value[0] == value[2] || value[1] == value[2]){
            printf( "Congrats, you just win "GRN"$20!\n"RESET);
            balance += 20;
        }
        else{
            printf("\n");
        }

        printf("Continue playing!? (Y/n) ");
        option();
        if (opt == 0) break;

        credits--;
        counter[0]++;
        system("clear");        
    }
    back();
}

void coin(){
    system("clear");
    printf( BLU"::: Coin Toss!!! :::\n"RESET
            "The classic coin toss\n"
            "Win Condition : "MAG"Just guess Head or tails\n"RESET
            "If you guess correct, you will get "GRN"$20\n"RESET
    ); 
    back();

    while(1){
        int coin = rand() % (2);
        int guess;
        printf(BLU"::: Coin Toss!!! :::\n"RESET);
        printf("Credit(s) left = %i\n", credits);
        
        // If you have no credit left
        if (credits == 0){
            printf("\nToo bad... but try "MAG"KEEP GAMBLING "RESET"again!\n");
            break;
        }

        printf( "Input your guess\n" 
                YEL"[Head(0)/Tail(1)] : "RESET); 
        scanf("%i", &guess);
        getchar();
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
                balance += 20;
            }
            else {
                printf( RED"Too bad...\n"RESET
                        "The actual face = "MAG"%s\n"RESET, (coin == 0) ? "Head" : "Tail"  ); 
            }
            printf("Continue playing!? (Y/n) ");
            option();
            if (opt == 0) break;

            credits--;
            counter[1]++;
            system("clear"); 
        }
    }
    back();
    return;
}

void rng(){
    system("clear");
    printf( BLU"::: Random Number Generator!!! :::\n"RESET
            "It is quite a simple game\n"
            "Win Condition : " MAG"Guess a number between 1-100\n"RESET
            "If you guess correctly, you will get "GRN"$100\n"RESET
            "If you guess 5 numbers above or bellow, you will get "GRN"$50\n"RESET
    );
    back();

    while(1){
        int num = rand() % (100) + 1;
        int guess;
        
        printf(BLU"::: Random Number Generator!!! :::\n"RESET);
        printf("Credit(s) left = %i\n", credits);

        // If you have no credit left
        if (credits == 0){
            printf("\nToo bad... but try "MAG"KEEP GAMBLING "RESET"again!\n");
            back();
            break;
        }

        printf("Input your guess : "); 
        scanf("%i", &guess); getchar();

        if (guess < 1 || guess > 100){
            printf(RED"Please input correctly >w< !\n"RESET);
            back();
        } else {
            // Cheat code (why)
            if (cheat[2] == 1){
                num = guess;
            }

            // If they are the same
            if (guess == num){
                printf("No way, you are right on the money! You just get "GRN"$100!\n"RESET);
                balance += 100;
            }
            // If +- 5 differences
            else if (guess >= num-5 && guess <= num+5){
                printf("You are just "MAG"%i "RESET"number away!\n", num - guess);
                printf("The actual number = "MAG"%i\n"RESET, num);
                printf("You just get "GRN"$50!\n"RESET);
                balance += 50;
            } 
            // If miss
            else {
                printf( RED"Too bad...\n"RESET);
                printf("The actual number = "MAG"%i\n"RESET, num);
            }
            printf("Continue playing!? (Y/n) ");
            option();
            if (opt == 0) break;

            credits--;
            counter[2]++;
            system("clear");
        }
    }
    back();
    return;
}

void market(){
    system("clear");
    printf( BLU"::: Market!!! :::\n"RESET
            BLU"Uhmmm... hi > w < !!!\n"RESET
            "Hh-how much do you want to pay T-T ?\n"
            "Credits "GRN"(1 credits = $10) : "RESET
    );
    int add;
    scanf("%i", &add); getchar();   
    int cost = 10 * add;
    int change = balance - cost;

    if (change < 0){
        int diff; 
        if (balance >= 0) diff = cost - balance; // Non-negative balances
        else diff = cost; // Negative balances
        printf( MAG"Do you want to borrow " RED"$%i?"RESET" (Y/n) : ", diff);
        option();
        if (opt != 1){
            printf("Transaction cancelled! "BLU"> v < \n");
            back();
            return;
        }
    }else if (add < 0){
        printf(RED"Nuh uh... Too bad you can't do that (·ω·)\n"RESET);
        back();
        return;
    }
    credits += add;
    balance -= 10 * add;

    // loading bar
    int size = 34;
    int i;
    printf("\n[");
    for (int i=0; i < size/2; i++){
        printf(" o");
    }
    printf("]");
    printf("\r");
    for (int i=0; i <= size; i++){
        fflush(stdout);
        usleep(1000*75);
        if (i==0) printf(CURSOR_H"[");
        if (i==size) printf("]");
        else {
            if (i>2){
                if (i%2 >= 1) printf(YEL"\b-"MAG"C"RESET);
                else printf(YEL"\b-"MAG"c"RESET);
            }else printf("-");
        }
    }
    printf(BLU" Done! (≧∇≦)"RESET CURSOR_E);
    fflush(stdout);
    usleep(1000*500);

    printf(GRN"\nMoney transfered!"RESET" Current balances : ");
    printf("%s$%i\n"RESET, (balance <= 0) ? RED : GRN, balance);
    printf( "Erm... please gamble responsibly T-T...\n");
    back();
    return;
}

void passcode(){
    system("clear");

    char password [100];
    const char passcode[] = "nihahaha";

    getchar(); // input buffer or something
    printf(RED"Enter the passcode : "RESET); 
    fgets(password, 100, stdin); 
    password[strcspn(password, "\n")] = 0; // remove newline from fgets
    if (strcmp(password,passcode) == 0){
        hidden();
    }else{
        printf(RED"Sorry, you are not authorized!\n"RESET);
        back();
        return;
    } 

}

void hidden(){
    int input, i;

    while(1){
        system("clear");
        printf( "You just opened "MAG"hidden options! "RESET"What option do you want?\n"
            "(just select the option again if you want to change it)\n"
            "1 : Change the amount of credit\n"
            "2 : Always win on slot\n"
            "3 : Always win on coin toss\n"
            "4 : Always win on rng\n"
            RED"0 : Return to previous Selection\n"RESET
            "Your input: "
        );
        scanf("%i", &input);
        getchar();
        if (input == 1){
            int new;
            printf("\nNew credits (previous = %i) : ", credits);
            scanf("%i", &new);
            credits += new;
            printf("Credits changed to"GRN" %i\n"RESET, credits);
            back();
        } else if (input == 2 || input == 3 || input == 4){
            if (input == 2) i = 0;
            else if(input == 3) i = 1;
            else i = 2;
            printf("\nPrevious value: %s\n", (cheat[i] == 1) ? BLU"True"RESET : RED"False"RESET);
            printf("Change? (Y/n) : ");
            option();
            if (opt == 1) {
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
}

void back(){
    printf(YEL"Press enter to continue..."RESET);
    getchar(); while(getchar() != '\n' ); // Detect Enter
    system("clear");
}

void option(){
    char select[3];
    opt = 0;

    fgets(select, 3, stdin);
    select[strcspn(select, "\n")] = 0; // remove newline from fgets
    if (strlen(select) == 0 || select[0] == 'y' || select[0] == 'Y') opt = 1;
    else opt = 0;
}

void achievement(){
    system("clear");

    int track = 0;

    printf(MAG"Your achievement :\n"RESET);
    char *game[] = {"Slot","Coin Toss","Random Number Generator"};

    for(int i=0; i < 3; i++){
        if (counter[i] >= 10) {
            printf("- Play "BLU"%s"RESET" for "GRN"10 times\n"RESET, 
            game[i]);
            track++;
        }
        if (counter[i] >= 50) {
            printf("- Play "BLU"%s"RESET" for "MAG"50 times\n"RESET, 
            game[i]);
            track++;
        }
        if (counter[i] >= 100) {
            printf("- Play "BLU"%s"RESET" for "RED"100 times\n"RESET, 
            game[i]);
            track++;
        }
    }
    if (balance <= -100 || persist[0] == 1){
        printf("- "MAG"Debt Collector's Ghost "RESET": In "RED"debt"RESET" for more than "RED"$100\n"RESET);
        persist[0] = 1;
        track++;
    }
    if (balance >= 1000 || persist[1] == 1){
        printf("- "MAG"Masterful Gambit "RESET": Earned"GRN" $1000\n"RESET);
        persist[1] = 1;
        track++;
    }

    if (counter[0] + counter[1] + counter[2] >= 100){
        printf("- "MAG"A True Gambler Mindset "RESET": Gambled for more than "GRN"100 times\n"RESET);
        persist[1] = 1;
        track++;
    }

    if (track == 0){
        printf(RED"There's no achievement yet :(\n"RESET);
    }
    back();
}