#include <stdbool.h>
#include <stdlib.h>

#define ONE_PROB 1.0/6.0
#define NON_ONE_PROB 1.0/3.0
#define TOTAL_POS_BETS 54
#define MAX_DICE_ON_TABLE 9
#define MAX_DICE 5
#define PROB_WEIGHT 2
#define DUDO_THRESHOLD 0.1
#define CALZA_THRESHOLD 0.7
#define NEW_BET_THRESHOLD 0.08
#define EPSILON 0.00001



typedef enum decision{
    BET, 
    DUDO,
    CALZA
} decision;

typedef struct bet{
    int amount;
    int value;
} bet;

typedef struct player {
    int num_dice;
    int dice[5]; 
} player;

typedef struct game_state {
    bet current_bet;
    bool users_turn;
    player user;
    player computer;
    bool palifico;
    int moves;
} game_state;

bool call_calza(game_state *state, bool called_by_user);
bool call_dudo(game_state *state);
int get_dice_values(player *player, int value);
bool check_valid_bet(game_state *state, bet *bet);
void print_dice(player *player);
void roll_dice(player *player);
int get_num_of_value(game_state *state, int value);
game_state init(void);
bet get_bet(game_state *state, bet *var_ranking, bet *prob_ranking);
decision AI(game_state *state);
int choose_function(int n, double k);
double calculate_probabilities(game_state *state, bet bet);
double calculate_probability_above(game_state *state, bet bet);
double cmp_func_probabilities(bet betpos, bet newbet, game_state * state);
double cmp_func_amounts(bet betpos, bet newbet, game_state * state);
void sort(game_state *state, bet *list, int num_bets, double (*cmp_func)(struct bet, struct bet, game_state * ));

