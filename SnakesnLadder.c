#include "SnakesnLadders.h"

//declaring global variables
int board_upper_hardlimit = 63;
int board_lower_hardlimit = 32; 

int ladder_upper_limit = 10;
int ladder_lower_limit = 1; 

int snake_upper_limit = 10;
int snake_lower_limit = 1; 

int max_players = 4;
int dice_size = 6;

void menu(struct Board *board);

//prints Linked List
void show_Board(struct Board *board, int array_size){
    struct Tile *temp = board->start;
    int i = 1;

    while( temp != NULL){
        int k = 1;
        int j;

        //prints the player position if any of the players are on the tile currently being printed
        for(j = 1; j <= array_size; j++){
            if(i == board->players[j].position_Tile->tile_num){
                printf("P%d ",board->players[j].player_Number);
                k++;  
            }
        }
        while(k <= array_size){
            printf("   ");
            k++;
        }             
        printf(">");  

        //prints the tile number
        printf("[%d]",temp->tile_num);

        //prints any snakes or ladders linked with the tile
        if(temp->snake != NULL){
            printf (" ---===e [%d]",temp->snake->tile_num);
        }
        if(temp->ladder != NULL){
            printf(" ||||||| [%d]",temp->ladder->tile_num);
        }
        printf("\n");

        temp = temp->next;
        i++;
    }
}

//adds node to Linked List
void push(struct Board *board, int tile_num){
    struct Tile *n;
    n = malloc(sizeof(struct Tile));
    n->tile_num = tile_num;
    n->next = board->start;
    n->ladder = NULL;
    n->snake = NULL;
    n->has_link = 0;
    board->start = n;
};

//generating a snake of a random length at a random position
void generate_Snake(struct Board *board, int num_of_tiles){
    int i;
    int j;

    //sets random values for snake location and length
    int random1 = rand() % num_of_tiles - (snake_upper_limit + 1);
    int random2 = rand() % snake_upper_limit;

    //counts from the start of the board until the randomised location and places the snakes foot
    struct Tile *snake_foot = board->start->next->next;
    for(i = 0; i < random1; i++){
        snake_foot = snake_foot->next;
    }

    //counts from the foot until the randomized length and places the snakes head
    struct Tile *snake_head = snake_foot->next;
    for(j = 0; j < random2; j++){
        snake_head = snake_head->next;
    }

    //checks if the current tiles are already occupies by either a snake or a ladder, if not, its creates the snake pointer.
    if(snake_foot->has_link || snake_head->has_link){generate_Snake(board, num_of_tiles);}
    else{
        snake_foot->has_link = 1;
        snake_head->has_link = 1;
        snake_head->snake = snake_foot;
    }
}

//generating a Ladder of a random length and at a random position
void generate_Ladder(struct Board *board, int num_of_tiles){
    int i;
    int j;

    //sets random values for ladder location and length
    int random1 = rand() % num_of_tiles - (ladder_upper_limit + 1);
    int random2 = rand() % ladder_upper_limit;

    //counts from the start of the board until the randomised location and places the ladder foot
    struct Tile *ladder_foot = board->start->next->next;
    for(i = 0; i < random1; i++){
        ladder_foot = ladder_foot->next;
    }

    //counts from the foot until the randomised length and places the ladder head
    struct Tile *ladder_head = ladder_foot->next;
    for(j = 0; j < random2; j++){
        ladder_head = ladder_head->next;
    }

    //checks if the current tiles are already occupies by either a snake or a ladder, if not, its creates the ladder pointer.
    if(ladder_foot->has_link || ladder_head->has_link){generate_Ladder(board, num_of_tiles);}
    else{
        ladder_head->has_link = 1;
        ladder_foot->has_link = 1;
        ladder_foot->ladder = ladder_head;
    }
}

//generating a randomised board
void generate_Board(struct Board *board){
    srand(time(NULL));
    //printf("Generating Board\n");

    //Lower size limit for the board changes depending on the amount of snakes and ladders, 
    //in order to keep the lower limit at 32 like the assignment asks there must be at most 14 snakes and ladders.
    //above 14 the program will make sure the snakes and ladders will fit into the board and raise the lower size limit above 32.

    //Takes and verifies value for snake_amount from user
    int max_links = 24;

    const char temp_snake_amount = 0;
    int snake_amount = 0;

    while(1){
        printf("Enter how many snakes you wish to have on the board: ");
        scanf("%s", &temp_snake_amount);
        if(isdigit(temp_snake_amount)){
            snake_amount = atoi(&temp_snake_amount);
            if(snake_amount >= 0 && snake_amount <= max_links){
                break;
            }else{printf("Please enter an Integer between 0 and %d\n", max_links);}
        }else{printf("Please enter an Integer\n");}
    }

    //takes and verifies value for ladder_amount from user
    const char temp_ladder_amount = 0;
    int ladder_amount = 0;

    while(1){
        printf("Enter how many ladders you wish to have on the board: ");
        scanf("%s", &temp_ladder_amount);
        if(isdigit(temp_ladder_amount)){
            ladder_amount = atoi(&temp_ladder_amount);
            if(ladder_amount >= 0 && ladder_amount <= max_links - snake_amount){
                break;
            }else{printf("Please enter an Integer between 0 and %d\n", (max_links - snake_amount));}
        }else{printf("Please enter an Integer\n");}
    }

    //generates a random number of tiles
    int num_of_tiles;
    
    //calculating soft lower limit based on the number of snakes and ladders in order to make sure the board can accomidate the user-defined amount of snakes and ladders.
    int board_lower_limit;

    if((((snake_amount + ladder_amount) * 2) + 4) > board_lower_hardlimit){
       board_lower_limit = (((snake_amount + ladder_amount) * 2) + 4); 
    }else{
        board_lower_limit = board_lower_hardlimit;
    }

    num_of_tiles = (rand() % (board_upper_hardlimit - board_lower_limit + 1)) + board_lower_limit;
    board->tiles = num_of_tiles;

    //filling the board with nodes
    int i;

    for(i = num_of_tiles; i > 0 ; i--){
        push(board, i);
    }

    //adding ladders
    int ladders;
    //printf("%d Ladders\n", ladder_amount);
    for(ladders = 0; ladders < ladder_amount; ladders++){
        generate_Ladder(board, num_of_tiles);
    }
    
    //adding snakes
    int snakes;
    //printf("%d Snakes\n", snake_amount);
    for(snakes = 0; snakes < snake_amount; snakes++){
        generate_Snake(board, num_of_tiles);
    }
}

//main code for playing the game
void play(struct Board *board, struct Game *game){
    int int_players;
    int i;

    //setting the amount of players in the game.
    while(1){
        const char temp_players;

        //asks the user for the number of players and scans
        printf("How many players(max is %d): ", max_players);
        scanf("%s", &temp_players);

        //checks if the inputed value is an integer and between 1 and max_players
        if(isdigit(temp_players)){
            int_players = atoi(&temp_players);
            if(int_players >= 1 && int_players <= max_players){

                //creates and fills the array of players
                board->players = malloc(sizeof(Player)*int_players);
                for(i=1; i <= int_players;  i++){
                    board->players[i].player_Number = i;
                    board->players[i].position_Tile = board->start;
                }
                break;
            }else{printf("Please enter a number between 1 and %d\n", max_players);}
        }else{printf("Please enter an integer\n");}
    }

    int j = 1;
    while(1){
            //prints the board
            show_Board(board, int_players);

            //prints the UI
            printf("======================================\n");
            printf("Player %d make your move!\n", board->players[j].player_Number);
            printf("======================================\n");
            printf("[ 1 ] Roll Dice\n");
            printf("[ 2 ] Return to Menu\n");
                printf(" --> ");
                
                //scans the input from the user
                const char selection;
                scanf("%s", &selection);

                //checks if the input is an integer and between 1 and 2
                if(isdigit(selection)){
                    int int_selection = atoi(&selection);
                    if(int_selection >= 1 && int_selection <= 2){
                        switch(int_selection){

                            // rolling a dice between 1 and 6 and moving the player that amount;
                            case 1:
                                int k;
                                int dice = (rand() % dice_size) + 1;

                                printf("Player %d rolled a %d!\n", board->players[j].player_Number, dice);

                                //checks if the players position is the final tile and announces the winner of the game.
                                if((board->players[j].position_Tile->tile_num) + dice >= board->tiles){
                                    printf("Player %d moving from tile %d to tile %d\n",board->players[j].player_Number, board->players[j].position_Tile->tile_num ,board->tiles);
                                    printf("Player %d won!\n", board->players[j].player_Number);

                                    game->winner = board->players[j].player_Number;

                                    menu(board);
                                    break;
                                }
                                printf("Player %d moving from tile %d to tile ",board->players[j].player_Number, board->players[j].position_Tile->tile_num);
                                
                                //moves the player forward by the amount the dice throw resulted in
                                struct Tile *player_position = board->players[j].position_Tile;
                                for(k = 0; k < dice; k++){
                                    player_position = player_position->next;
                                }
                                board->players[j].position_Tile = player_position;

                                printf("%d\n",board->players[j].position_Tile->tile_num);
                                break;

                            // returns to the menu
                            case 2:
                                menu(board);
                                break;
                        }
                    }else{printf("Please enter an Integer between 1 and 2\n");}
                }else{printf("Please enter an Integer\n");}

        //checks if players position is a ladder and moves to the top of the ladder if true
        if(board->players[j].position_Tile->ladder != NULL){
            printf("Player %d came across a Ladder!\n", board->players[j].player_Number);
            printf("Player %d moving from tile %d to tile %d\n",board->players[j].player_Number, board->players[j].position_Tile->tile_num, board->players[j].position_Tile->ladder->tile_num);
            board->players[j].position_Tile = board->players[j].position_Tile->ladder;
            game->ladder_hits++;
        }
        //checks if the players position is a snake and moves to the bottom of the snake if true
        if(board->players[j].position_Tile->snake != NULL){
            printf("Player %d came across a Snake!\n", board->players[j].player_Number);
            printf("Player %d moving from tile %d to tile %d\n",board->players[j].player_Number, board->players[j].position_Tile->tile_num, board->players[j].position_Tile->snake->tile_num);
            board->players[j].position_Tile = board->players[j].position_Tile->snake;
            game->snake_hits++;
        }
        printf("======================================\n\n");
        j++;
        game->moves++;
        if(j > int_players){j=1;}
    }
}

//code to display the menu and navigate the program
void menu(struct Board *board){
    const char selection;

    //prints the menu
    printf("\n");
    printf("+=+=+=+=+=+=+=+=+=+=+=+=+=+\n");
    printf("Welcome to Snake n Ladders!\n");
    printf("+=+=+=+=+=+=+=+=+=+=+=+=+=+\n");
    printf("[ 1 ] Generate board\n");
    printf("[ 2 ] Show Board\n");
    printf("[ 3 ] Play the game\n");
    printf("[ 4 ] Exit\n");
    while(1){

        //takes input from the user
        printf(" --> ");
        scanf("%s", &selection);

        //checks if the input is an integer and between 1 and 4
        if(isdigit(selection)){
            int int_selection = atoi(&selection);
            if(int_selection >= 1 && int_selection <= 4){
                switch(int_selection){

                    //if board doesnt exist it generates a new board
                    case 1:
                        if(board->start == NULL){
                            generate_Board(board);
                        }else{
                            printf("The Board has already been generated\n");
                        }
                        menu(board);
                        break;

                    //prints the board
                    case 2:
                        if(board->start == NULL){
                            printf("Please generate the board first");
                        }else{
                            show_Board(board, 0);
                            menu(board);
                            break;
                        }

                    //begins the game
                    case 3:
                        if(board->start == NULL){
                            printf("Please generate the board first\n");
                            break;
                        }else{
                            //initialises the game struct
                            struct Game game;
                            game.ladder_hits = 0;
                            game.moves = 0;
                            game.snake_hits = 0;
                            game.winner = 0;

                            play(board, &game);
                            break;
                        }

                    //terminates the program
                    case 4:
                        exit(0); 
                }
            }else{printf("Please enter an Integer between 1 and 4\n");}
        }else{printf("Please enter an Integer\n");}
    }
}

//files a report based on the recent game
void file_report(struct Board *board, struct Game *game){

}

int main(){
    //initialises the start of the Linked list
    struct Board ThisBoard;
    ThisBoard.start = NULL;
    
    //calls the menu function
    menu(&ThisBoard);

    return 0;
}
