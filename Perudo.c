#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "Perudo.h"

// rolls the dice and populates the dice array
void roll_dice(player *player)
{

    int num_dice = player->num_dice;

    for (int i = 0; i < num_dice; i++)
    {
        player->dice[i] = rand() % 6 + 1;
    }
}

// checks the next bet is valid according to game rules
bool check_valid_bet(game_state *state, bet *bet)
{
    if (bet->amount > (state->user.num_dice + state->computer.num_dice)) {
        return false;
    }
    if (state->palifico)
    {
        return (bet->amount > state->current_bet.amount && bet->value == state->current_bet.value);
    }

    //if the last bet was x 1's
    if (state->current_bet.value == 1) {

        //if new bet is just 1's
        if (bet->value == 1) {
            return (bet->amount > state->current_bet.amount);
        }

	    return (bet->amount > 2*state->current_bet.amount - 1 && bet->value <= 6);
    }
    
    if (bet->value == 1)
    {
        return (bet->amount >= (state->current_bet.amount / 2 + 1));
    }
    // if the amount has increased
    if (bet->amount > state->current_bet.amount)
    {
        // only return true if the value is smaller/the same
        return (bet->value <= state->current_bet.value);
    }
    // if the value has increased
    if (bet->value > state->current_bet.value)
    {
        // only return true if the amount is equal to current amount
        return (bet->amount == state->current_bet.amount);
    }

    return false;
}

// gets number of a particular dice value a player has
int get_dice_values(player *player, int value)
{
    int total = 0;
    for (int i = 0; i < player->num_dice; i++)
    {
        if (player->dice[i] == value || player->dice[i] == 1)
        {
            total++;
        }
    }
    return total;
}

int get_num_of_value(game_state *state, int value)
{
    int total = 0;
    total += get_dice_values(&state->user, value);
    total += get_dice_values(&state->computer, value);
    return total;
}

void new_round(game_state *state, bool is_user_next)
{
    state->users_turn = is_user_next;
    state->current_bet.value = 0;
    state->current_bet.amount = 0;
}

// updates accordingly if current player has called dudo on the prevous players bet
bool call_dudo(game_state *state)
{
    int value = state->current_bet.value;
    int total = get_num_of_value(state, value);

    // if calling dudo was correct
    if (total < state->current_bet.amount)
    {
        if (state->users_turn)
        {
            state->computer.num_dice--;
        }
        else
        {
            state->user.num_dice--;
        }

        new_round(state, !state->users_turn);
        return true;
    }

    // calling dudo incorrect
    if (!state->users_turn)
    {
        state->computer.num_dice--;
    }
    else
    {
        state->user.num_dice--;
    }

    new_round(state, state->users_turn);
    return false;
}

// updates accordingly if player has called calza on the prevous players bet
// any player can call calza, it doesn't have to be the current player
bool call_calza(game_state *state, bool called_by_user)
{
    int value = state->current_bet.value;
    int total = get_num_of_value(state, value);
    player *caller;

    if (called_by_user)
    {
        caller = &state->user;
    }
    else
    {
        caller = &state->computer;
    }

    // if calling calza was correct
    if (total == state->current_bet.amount)
    {
        if (caller->num_dice < MAX_DICE)
        {
            caller->num_dice++;
        }
        new_round(state, called_by_user);
        return true;
    }

    caller->num_dice--;
    new_round(state, called_by_user);
    return false;
}

bet get_bet(game_state * state, bet *var_ranking, bet *prob_ranking)
{
    bet curr_bet;
    bet best_bet;
    int best_eval = -1;

    //if it is the computers first move, it randomly picks 1 or 2 of a dice in its hand
    if (state->current_bet.amount == 0) {
        int val = rand() % 5;
        int amount = rand() % 2;
        bet new_bet;
        new_bet.value = state->computer.dice[val];
        new_bet.amount = amount;
        return new_bet;
    }

    //if it's not the first move, it merges the two lists to get the best bet, weighing probability more
    for (int i = 0; prob_ranking[i].value != 0; i++)
    {
        curr_bet = prob_ranking[i];
        for (int j = 0;; j++)
        {
            if (curr_bet.amount == var_ranking[j].amount && curr_bet.value == var_ranking[j].value)
            {
                if (PROB_WEIGHT * (TOTAL_POS_BETS - i) + (TOTAL_POS_BETS - j) > best_eval)
                {
                    best_eval = PROB_WEIGHT * (TOTAL_POS_BETS - i) + (TOTAL_POS_BETS - j);
                    best_bet = curr_bet;
                }

                break;
            }
        }
    }

    return best_bet;
}

int factorial(int num)
{
    if (num <= 1)
    {
        return 1;
    }

    return (num * factorial(num - 1));
}

// Computes binomial coefficient, n choose k
int choose_function(int n, double k)
{
    return (factorial(n) / (factorial(k) * factorial(n - k)));
}

// Computes probability of total - total in computers hand
//of a dice value appearing in the opposing players hand
double calculate_probabilities(game_state *state, bet bet)
{
    int num_in_hand = get_dice_values(&state->computer, bet.value);
    double amount = bet.amount - num_in_hand;
    int users_dice = state->user.num_dice;
    // returns binomial coefficient multiplied by probabilty to power of quantity
    // then inverse probability to the power of remaining total
    if (bet.value == 1)
    {
        double prob = pow(ONE_PROB, amount);
        double inverse_prob = pow(1 - ONE_PROB, users_dice - amount);
        return (choose_function(users_dice,amount) * prob * inverse_prob);
    }
   
    double prob = pow(NON_ONE_PROB, amount);
    double inverse_prob = pow(1 - NON_ONE_PROB, users_dice - amount);
    return (choose_function(users_dice, amount) * prob * inverse_prob);
}

double calculate_probability_above(game_state *state, bet bet) {
  double total = 0;
  struct bet tmpbet;
  for (int amount = bet.amount; amount <= MAX_DICE_ON_TABLE; amount++) {
    tmpbet.amount = amount;
    tmpbet.value = bet.value;
    total += calculate_probabilities(state, tmpbet);
  }

  return total;
}

double calculate_amounts(game_state *state, bet bet)
{
    return (bet.amount);
}

//comparison function for probabilities
//returns betpos > newbet
double cmp_func_probabilities(bet betpos, bet newbet, game_state * state) {
    double pos_prob = calculate_probability_above(state, betpos);
    double new_prob = calculate_probability_above(state, newbet);
    return (new_prob - pos_prob);
}

//comparison function for amounts
double cmp_func_amounts(bet betpos, bet newbet, game_state * state) {
    double pos_amount = calculate_amounts(state, betpos);
    double new_amount = calculate_amounts(state, newbet);
    return (new_amount - pos_amount);
}

// inserts new bet into already sorted list, based on provided function (either variance or probability)
void sort(game_state *state, bet *list, int num_bets, double (*cmp_func)(struct bet, struct bet, game_state * ))
{
    for (int i = 1; i < num_bets; i++) {
        bet key = list[i];
        int j = i - 1;

        while (j >= 0 && cmp_func(list[j], key, state) > 0) {
            list[j+1] = list[j];
            j = j - 1;
        }
        list[j + 1] = key;
    }
}


// checks legal bets and puts into sorted list
bet *get_list(game_state *state, double (*cmp_func)(bet, bet, game_state *))
{
    bet potential_bet;
    int valid_bets = 0;
    bet *rankings = calloc(TOTAL_POS_BETS, sizeof(bet));
    for (int val = 1; val <= 6; val++)
    { // goes through all dice values
        for (int i = 1; i <= MAX_DICE_ON_TABLE; i++)
        { // goes through quantities of dice
            potential_bet.amount = i;
            potential_bet.value = val;
            if (check_valid_bet(state, &potential_bet))
            {
                rankings[valid_bets] = potential_bet;
                valid_bets++;
            }
        }
    }
    sort(state, rankings, valid_bets, cmp_func); 
    return rankings;
}

// double get_percentage_difference(int guessed, double expected)
// {
//     double difference = guessed - expected;
//     return (difference / expected * 100);
// }

// makes a decision
decision AI(game_state *state)
{
    bet *sorted_by_amounts = get_list(state, &cmp_func_amounts);
    bet *sorted_by_prob = get_list(state, &cmp_func_probabilities);
    // check percentage difference threshold for dudo - percent can be adjusted

    //int expected_amount = calculate_probabilities(state, state->current_bet);
    if (calculate_probability_above(state, state->current_bet) < DUDO_THRESHOLD && state -> current_bet.amount != 0)
    {
        state->moves = 0;
        return DUDO;
    }
    // check variance threshold for calza - value can be adjusted
    else if (calculate_probabilities(state, state->current_bet) > CALZA_THRESHOLD && state -> current_bet.amount != 0)
    { // if within %2 either side
        state->moves = 0;
        return CALZA;
    }
    else
    { // otherwise bet
        bet best_bet = get_bet(state, sorted_by_amounts, sorted_by_prob);
        if (calculate_probability_above(state, best_bet) < NEW_BET_THRESHOLD) {
            state->moves = 0;
            return CALZA;
        }
        state->current_bet.amount = best_bet.amount;
        state->current_bet.value = best_bet.value;
	    state->users_turn = true;
        return BET;
    }
}

game_state init(void)
{
    srand(time(NULL));

    game_state state = {
        {0, 0},
        true,
        {5, {0, 0, 0, 0, 0}},
        {5, {0, 0, 0, 0, 0}},
        false};

    return state;
}


