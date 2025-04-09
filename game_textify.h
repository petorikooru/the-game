// idk why i use yaml but whatever
#ifndef FILENAME
    #define FILENAME    "database.dat"
#endif

#define FILENAME_T      "database.yaml"

void database_textify(){
    FILE* database_file = fopen(FILENAME, "rb+");
    FILE* database_text = fopen(FILENAME_T, "w");
    User* database_user = malloc(sizeof(User));
    int i = 0;

    // Update to file
    while (fread(database_user, sizeof(User), 1, database_file) == 1) {
        fprintf(database_text, "---\n");
        fprintf(database_text, "# User no.%i\n" , ++i);
        fprintf(database_text, "Username : %s\n", database_user->username);
        fprintf(database_text, "Password : %s\n", database_user->password);
        fprintf(database_text, "\n");
        fprintf(database_text, "Banned   : %s\n", (database_user->banned) ? "true" : "false");
        fprintf(database_text, "Messages : %s\n", (database_user->banned) ? database_user->message : "null");
        fprintf(database_text, "\n");
        fprintf(database_text, "Game Data :\n");
        fprintf(database_text, "  Balances : %i\n", database_user->game_data.balances);
        fprintf(database_text, "  Credits  : %i\n", database_user->game_data.credits);
        fprintf(database_text, "  Counter  : # How many times you have played\n");
        fprintf(database_text, "    - %i # Slot\n",                     database_user->game_data.counter[0]);
        fprintf(database_text, "    - %i # Random Number Guesser\n",    database_user->game_data.counter[1]);
        fprintf(database_text, "    - %i # Coin Toss\n",                database_user->game_data.counter[2]);
        fprintf(database_text, "    - %i # Total\n",                    database_user->game_data.counter[3]);
        fprintf(database_text, "  Persist  : # Save the achievement state\n");
        fprintf(database_text, "    - %s # Slot\n",                     (database_user->game_data.counter[0]) ? "true" : "false");
        fprintf(database_text, "    - %s # Random Number Guesser\n",    (database_user->game_data.counter[1]) ? "true" : "false");
        fprintf(database_text, "    - %s # Coin Toss\n",                (database_user->game_data.counter[2]) ? "true" : "false");
        fprintf(database_text, "    - %s # Reserved\n",                 (database_user->game_data.counter[3]) ? "true" : "false");
        fprintf(database_text, "...\n");
    }
    fclose(database_file);
    fclose(database_text);
    free(database_user);
}

int database_intify(char *string){
    int value = 0;
    int convert = sscanf(string, "%i", value);
    return value;
}

/* Idk why the token doesnt work or failed or whatever on "Persist"*/
// void database_readify(){
//     FILE* database_text = fopen("database.yaml", "r");
//     User* database_user = malloc(sizeof(User));

//     char *token, string[BUFF_STR], value_string[BUFF_STR];
//     int i = 0, value_int = 0;

//     while (fgets(string, BUFF_STR, database_text)) {
//         token = strtok(string, " : ");

//         if (strcmp("Username", token) == 0) {
//             token = strtok(NULL, " : ");
//             strcpy(database_user->username, token);
//         }
//         if (strcmp("Password", token) == 0) {
//             token = strtok(NULL, " : ");
//             strcpy(database_user->password, token);
//         }
//         if (strcmp("Banned", token) == 0) {
//             token = strtok(NULL, " : ");
//             database_user->banned = database_intify(token);
//         }
//         if (strcmp("Balances", token) == 0){
//             token = strtok(NULL, " : ");
//             database_user->game_data.balances = database_intify(string);
//         }
//         if (strcmp("Credits", token) == 0) {
//             token = strtok(NULL, " : ");
//             database_user->game_data.credits = database_intify(string);
//         }
//         if (strcmp("Counter", token) == 0) {
//             while(fgets(string, BUFF_STR, database_text) && i < 4){
//                 sscanf(string, "    - %i", &value_int);
//                 database_user->game_data.counter[i] = value_int;
//                 printf("%i ", database_user->game_data.counter[i]);
//                 i++;

//                 token = strtok(string, " : ");
//                 token = strtok(NULL, " : ");
//             }
//         }
//         i = 0;
//         if (strcmp("Persist", token) == 0) {
//             while(fgets(string, BUFF_STR, database_text) && i < 4){
//                 sscanf(string, "    - %s", value_string);
//                 database_user->game_data.persist[i] = database_intify(value_string);
//                 printf("%i ", database_user->game_data.persist[i]);
//                 i++;
//             }
//         }
//         if (strcmp("...\n", string) == 0) break;
//     }

//     free(database_user);
//     fclose(database_text);
// }
