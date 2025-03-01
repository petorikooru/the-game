/* ========== User Object ========== */
typedef struct {
    long balances;
    int credits;
    // Array Indices
    // 0 : Slot
    // 1 : RNG
    // 2 : Coin
    // 3 : Total | Reserved
    int counter     [4];
    bool persist    [4];
} game_data_t;

typedef struct {
    char username   [BUFF_STR];
    char password   [BUFF_STR];
    char message    [255];
    bool banned;
    game_data_t game_data;
} user_t;

typedef enum {
    NONE,
    USER,
    ADMIN,
} user_type_t;

/* ================== Database Functions ==================== */
void database_create(FILE* database_file){
    database_file = fopen("database.dat", "wb");

    // Create an administrator account
    user_t* admin = malloc(sizeof(user_t));
    memcpy(admin->username, "Admin", 6);
    memcpy(admin->password, "123", 4);
    for (int i=0; i<4; i++){
        admin->game_data.counter[i] = 0;
        admin->game_data.persist[i] = false;
    }
    admin->game_data.counter[3] = 99999;
    admin->game_data.balances = 0;
    admin->game_data.credits  = 0;
    admin->banned = false;

    fwrite(admin, sizeof(user_t), 1, database_file);
    free(admin);
    fclose(database_file);
}

void database_update(user_t* user){
    FILE* database_file = fopen("database.dat", "rb+");
    user_t* database_user = malloc(sizeof(user_t));

    // Update the counter
    int counter_total = 0;
    for (int i=0; i<3; i++){
        counter_total += user->game_data.counter[i];
    }
    user->game_data.counter[3] = counter_total;
    // Update to file
    while (fread(database_user, sizeof(user_t), 1, database_file) == 1) {
        if (strcmp(database_user->username, user->username) == 0) {
            fseek(database_file, -sizeof(user_t), SEEK_CUR);
            fwrite(user, sizeof(user_t), 1, database_file); 
            break;
        }
    }
    fclose(database_file);
    free(database_user);
    return;
}

void database_sort(FILE* database_file, user_t* database_user, int database_user_total){
    // Insertion Sort (from the highest)
    int temp_counter, j;
    user_t* temp_user = malloc(sizeof(user_t));
    for (int i=0; i <= database_user_total - 1; i++){
        temp_counter = database_user[i].game_data.counter[3]; // Gambling Counter
        *temp_user = database_user[i];
        j = i;
        while (j > 0 && temp_counter > database_user[j-1].game_data.counter[3]){
            database_user[j] = database_user[j - 1];
            j = j - 1;
        }
        database_user[j] = *temp_user;
    }
    free(temp_user);
}

int database_user_login(char* username, char* password, user_t* database){
    FILE* database_file = fopen("database.dat", "rb");
    // Check whether the file exist or not
    if (!sys_input_check_file(database_file)){
        return NONE;
    }
    // Searching for the user
    user_type_t user_type;
    bool found = false;
    while(fread(database, sizeof(user_t), 1, database_file) == 1){
        if (strcmp(username, "Admin") == 0){
            found = true;
            if (strcmp(database->password, password) == 0){
                user_type = ADMIN;
                break;
            } else {
                printf(RED "[!] Wrong Password bleh :p" CURSOR_DOWN RESET);
                printf(CURSOR_DOWN);
                user_type = NONE;
                break;
             }
        }
        if (strcmp(username, database->username) == 0){
            found = true;
            if (strcmp(database->password, password) == 0){
                user_type = USER;
                break;
            }
            else {
                printf(RED "[!] Wrong Password!" CURSOR_DOWN RESET);
                printf(CURSOR_DOWN);
                user_type = NONE;
                break;
             }
        }
    }
    if (!found){
        printf(RED"[!] User not found!" CURSOR_DOWN RESET);
        printf(CURSOR_DOWN);
        fclose(database_file);
        user_type = NONE;
    }
    return user_type;
}

int database_user_signup(char* username, char* password, user_t* database){
    // If the user inputs blank
    if (strlen(username) == 0){
        printf(RED"[!] Username cannot be blank!" CURSOR_DOWN RESET);
        return FAILED;
    }
    if (strlen(password) == 0){
        printf(RED"[!] Password cannot be blank!" CURSOR_DOWN RESET);
        return FAILED;
    }
    // Check whether the data exist or not yet
    FILE* database_file = fopen("database.dat", "rb+");
    if (!sys_input_check_file(database_file)){
        printf(BLU "[~] Creating a new one..." CURSOR_DOWN RESET);
        database_create(database_file);
        database_file = fopen("database.dat", "rb+");
    }
    // Check whether the username is already registered
    bool found = false;
    while(fread(database, sizeof(user_t), 1, database_file) == 1){
        if (strcmp(username, database->username) == 0){
            found = true;
            break;
        }
    }
    if (found){
        printf(RED"[!] Username already exist!" CURSOR_DOWN RESET);
        fclose(database_file);
        return FAILED;
    }
    fclose(database_file);
    return SUCCESS;
}

void database_user_setup(char* username, char* password, user_t* user){
    int user_balances;
    while(RUNNING){
        screen_clear();
        printf(
            MAG "You must be new here :shake_hand:                " CURSOR_DOWN RESET
                "Welcome to your \"first time\" main menu!        " CURSOR_DOWN
                "To be able to play the game, you need to transfer" CURSOR_DOWN
                "some amount of money first to us :3              " CURSOR_DOWN
            BLU "Enter the amount of money that you want : "RESET"$"
        );
        if (sys_input_int(&user_balances)) break;
    }
    // Confirmation Dialog
    if (user_balances <=0)
        printf(MAG "Do you want to borrow "RED "$%i? " RESET"(Y/n) : ", user_balances);
    else
        printf(MAG "Are you sure? (Y/n)" RESET);
    if (!sys_input_confirm()){
        printf(
            RED "Transaction cancelled! T-T"   CURSOR_DOWN
                "Account creation failed!"     CURSOR_DOWN RESET
        );
        sys_input_return();
        return;
    }
    // Thank you :D
    printf(MAG "Transfering..." CURSOR_DOWN RESET);
    sys_loading_default();
    printf(
        BLU "Thank you for registering \\(> v <)/" CURSOR_DOWN RESET
            "As for your registration, you will get "GRN"10 free credits!" CURSOR_DOWN RESET
    );
    sys_input_return();
    // Variable initializations
    strncpy(user->username, username, sizeof(user->username));
    strncpy(user->password, username, sizeof(user->password));
    for (int i=0; i<4; i++){
        user->game_data.counter[i] = 0;
        user->game_data.persist[i] = false;
    }
    user->game_data.balances = user_balances;
    user->game_data.credits  = 10;
    user->banned = false;
    // Save the user data
    FILE* database_file = fopen("database.dat", "ab");
    fwrite(user, sizeof(user_t), 1, database_file);
    fclose(database_file);
}
