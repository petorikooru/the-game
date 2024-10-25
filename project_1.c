#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Color for the text
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define RESET "\x1B[0m"

// Cheat options
int cheat[3] = {0};

//Initial value
int balance = 100;
int credits = 10;
int opt;

void slot();
void coin();
void rng();
void market();
void passcode();
void hidden();
void back();
void option();

void main(){
    srand(time(NULL)); // Start the gambling mode
    int input;
    while (1){
        system("clear");
        printf( BLU"Do you love gambling? "RESET"I do and You are in the right place!\n"
                "Which game do you want to play?\n"
                "[Your current balances : ");
        if (balance <= 0) {
            printf(RED"$%i"RESET"] ", balance);
        }
        else {
            printf(GRN"$%i"RESET"] ", balance);
        }
        printf( "[Your current credits : "MAG"%i"RESET"]\n"            
                "1 : Slot\n"
                "2 : Coin\n"
                "3 : Random Number Generator\n"
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
    
    while(credits >= 0){
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
                BLU"==================="RESET);

        // If you get [7] [7] [7]
        if (value[0] == 7 && value[1] == 7 && value[2] == 7){
            printf( "\nJackpot! You just win a "GRN"real estate!\n"RESET
                    GRN"100$ "RESET"has been added into your balance\n"
                    YEL"Continue? (Y/n) "RESET
            );
            balance += 100;
            option();
            if (opt == 0) break;
        }
        // If you get all the same numbers
        else if (value[0] == value[1] && value[0] == value[2] && value[1] == value[2]){
            printf( "\nCongrats, you just win "GRN"$50!\n"RESET
                    YEL"Continue? (Y/n) "RESET
            );
            balance += 50;
            option();
            if (opt == 0) break;
        }
        // If you get atleast 2 numbers matching
        else if (value[0] == value[1] || value[0] == value[2] || value[1] == value[2]){
            printf( "\nCongrats, you just win "GRN"$10!\n"RESET
                    YEL"Continue? (Y/n) "RESET
            );
            balance += 10;
            option();
            if (opt == 0) break;
        }
        // No matching number
        else {
            printf( YEL"\nContinue? (Y/n) "RESET);
            option();
            if (opt == 0) break;
        }

        credits--;
        system("clear");        
    }
    back();
}

void coin(){
    system("clear");
    printf( BLU"::: Coin Toss!!! :::\n"RESET
            "The classic coin toss\n"
            "Win Condition : "MAG"Just guess Head or tails\n"RESET
            "If you guess correct, you will get "GRN"20$\n"RESET
    ); 
    back();

    while(credits>=0){
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
                printf("You just get "GRN"20$!\n"RESET);
                balance += 20;
                printf(YEL"Continue playing!? (Y/n) "RESET);
                option();
                if (opt == 0) break;
            }
            else {
                printf( RED"Too bad...\n"RESET
                        "The actual face = "MAG"%s\n"RESET
                        YEL"Try again? (Y/n) "RESET, (coin == 0) ? "Head" : "Tail"
                );
                option();
                if (opt == 0) break;
            }
            system("clear");
            credits--;
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
            "If you guess correctly, you will get "GRN"100$\n"RESET
            "If you guess 5 numbers above or bellow, you will get "GRN"$20\n"RESET
    );
    back();

    while(credits>=0){
        int num = rand() % (100) + 1;
        int guess;
        printf(BLU"::: Random Number Generator!!! :::\n"RESET);
        printf("Credit(s) left = %i\n", credits);

        // If you have no credit left
        if (credits == 0){
            printf("\nToo bad... but try "MAG"KEEP GAMBLING "RESET"again!\n");
            break;
        }

        printf("Input your guess : "); scanf("%i", &guess);
        getchar();
        // Cheat code (why)
        if (cheat[2] == 1){
            num = guess;
        }

        // If they are the same
        if (guess == num){
            printf("No way, you are right on the money! You just get "GRN"100$!\n"RESET);
            balance += 100;
            printf("Continue playing!? (Y/n) ");
            option();
            if (opt == 0) break;
        }
        // If +- 5 differences
        else if (guess >= num-5 && guess <= num+5){
            printf("You are just "MAG"%i "RESET"number away!\n", num - guess);
            printf("The actual number = "MAG"%i\n"RESET, num);
            printf("You just get "GRN"20$!\n"RESET);
            balance += 20;
            printf(YEL"Continue playing!? (Y/n) "RESET);
            option();
            if (opt == 0) break;
        } 
        // If miss
        else {
            printf( RED"Too bad...\n"RESET
                    "The actual number = "MAG"%i\n"RESET
                    YEL"Try again? (Y/n) "RESET, num
            );
            option();
            if (opt == 0) break;
        }
        system("clear");
        credits--;
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
    scanf("%i", &add);
    getchar();
    int paid = balance - (10 * add);
    if (paid < 0){
        int diff = (10 * add) - balance;
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
    printf("\nMoney transfered! Current balances : ");
    (balance <= 0) ? printf(RED"$%i\n"RESET, balance) : printf(GRN"$%i\n"RESET, balance);
    printf( "Erm... please gamble responsibly T-T...\n");
    back();
    return;
}

void passcode(){
    system("clear");

    char password [100];
    const char passcode[] = "nihahaha";

    // Password locked
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
    char passcode [100];
    int input, i;
    char option[2];

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
        if (input == 1){
            printf("New credits (previous = %i): ", credits);
            scanf("%i", &credits);

        }else if (input == 2 || input == 3 || input ==4){
            if(input == 2) i = 0;
            else if(input ==3) i = 1;
            else i = 4;
            printf("Previous value: %s\n", (cheat[i] == 1) ? BLU"True"RESET : RED"False"RESET);
            printf("Change it? (y/n) ");
            scanf("%s", &option);
            if (strcmp(option, "y") == 0) {
                if (cheat[i] == 0){
                    cheat[i] = 1;
                }else cheat [i] = 0;
            }
        }

        else if(input == 0) return;
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
    char select[2];
    opt = 0;

    fgets(select, 2, stdin);
    select[strcspn(select, "\n")] = 0; // remove newline from fgets
    if (strlen(select) == 0 || strcmp(select, "Y") == 0 || strcmp(select, "y") == 0) {
        opt = 1;
    } 
    else {
        opt = 0;
    }
}