#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Perudo.h"

int asserts_ran = 0, asserts_failed = 0, tests_ran = 0, tests_failed = 0;

#define run_test(test) do { \
asserts_ran = asserts_failed = 0; test(); \
tests_ran++;\
if (asserts_failed > 0) { \
tests_failed++; \
 printf("****%s: %d asserts failed out of %d asserts\n\n", #test, asserts_failed, asserts_ran); \
 }\
} while (0)

#define assert_eq(a, b) do { asserts_ran++; if ((a) != (b)) { printf("%s(line %d): got: %d | expected: %d\n", __func__, __LINE__, a, b); asserts_failed++;} } while (0)

#define assert_true(a) do { asserts_ran++; if (!a) { printf("%s(line %d): got: %d | expected: %d\n", __func__, __LINE__, a, true); asserts_failed++;} } while (0)

#define assert_false(a) do { asserts_ran++; if (a) { printf("%s(line %d): got: %d | expected: %d\n", __func__, __LINE__, a, false); asserts_failed++;} } while (0)

bet one_one = {1, 1};   
bet one_two = {1, 2};
bet one_three = {1, 3};
bet one_four = {1, 4};
bet one_five = {1, 5};
bet one_six = {1, 6};
bet two_ones = {2, 1};
bet two_twos = {2, 2};
bet two_threes = {2 , 3};
bet two_fours = {2, 4};
bet two_fives = {2, 5};
bet two_sixes = {2, 6};
bet three_ones = {3, 1};
bet three_twos = {3, 2};
bet nine_sixes = {9, 6};
bet two_sevens = {2, 7};
bet three_sixes = {3, 6};
bet twelve_twos = {12, 2};
bet four_fives = {4, 5};
bet four_ones = {4, 1};
bet five_fives = {5, 5};
bet six_fives = {6, 5};
bet seven_fives = {7, 5};
bet eight_fives = {8, 5};
bet nine_fives = {9, 5};
bet ten_fives = {10, 5};

player user = {5, {0, 0, 0, 0, 0}};
player comp = {5, {0, 0, 0, 0, 0}};


void test_check_valid_bet(void) {
    player user = {5, {1,2,3,4,5}};
    player comp = {5, {2,3,4,5,6}};
    player userpalifico = {1, {2}};
    game_state state = {one_two, 1, user, comp, 0};
    game_state palificostate = {one_one, 0, userpalifico, comp, 1};
    assert_false(check_valid_bet(&state, &nine_sixes)); //bet increases by amount and value
    assert_true(check_valid_bet(&state, &one_three)); //bet increases by value but not amount
    assert_true(check_valid_bet(&state, &three_twos)); //bet increases by amount but not value
    assert_false(check_valid_bet(&state, &one_two)); //bet same as previous bet
    assert_false(check_valid_bet(&state, &two_sevens)); //bet on a invalid number
    assert_true(check_valid_bet(&palificostate, &two_ones));
    assert_false(check_valid_bet(&palificostate, &one_three));  
}

void test_dudo(void) {
    player user = {5, {1,2,3,4,5}};
    player comp = {5, {2,3,4,5,6}};
    //state is bet, bool for is its users turn, user, comp, palifico 
    game_state state = {one_one, true, user, comp, false};
    game_state state0 = {two_ones, false, user, comp, false};

    call_dudo(&state);
    assert_eq(state.user.num_dice, 4);
    assert_eq(state.computer.num_dice, 5);

    call_dudo(&state0);
    assert_eq(state0.user.num_dice, 4);
    assert_eq(state0.computer.num_dice, 5);
}

void test_calza(void) {
    player user = {3, {6,6,3}};
    player comp = {3, {2,3,6,0,0}};
    game_state state = {one_one, 1, user, comp, 0};
    game_state state0 = {three_sixes, 0, user, comp, 0};

    call_calza(&state, true);
    assert_eq(state.user.num_dice, 2);

    call_calza(&state0, false);
    assert_eq(state0.computer.num_dice, 4);
}

void test_choose(void) {
    int res; 
    res = choose_function(3, 2);
    assert_eq(res, 3);

    res = choose_function(10, 2);
    assert_eq(res, 45);

    res = choose_function(10, 4);
    assert_eq(res, 210);
}

void test_sort_amount(void) {
    bet list1[5] ={one_five, three_ones, two_fours, three_twos};
    bet list2[5] ={three_ones, two_fours, one_six, two_fives};
    bet list3[5] ={one_two, three_ones, two_fours, one_three};

    bet list1_good[5] = {three_ones, three_twos, two_fours, one_five};
    bet list2_good[5] = {three_ones, two_fours, two_fives, one_six};
    bet list3_good[5] = {three_ones, two_fours, one_two, one_three};

    game_state state = init();

    sort(&state, list1, 4, cmp_func_amounts);
    sort(&state, list2, 4, cmp_func_amounts);
    sort(&state, list3, 4, cmp_func_amounts);

    for(int i = 0; i < 4; ++i){
        assert_eq(list1[i].amount, list1_good[i].amount);
        assert_eq(list1[i].value, list1_good[i].value);
   
    }
    for(int i = 0; i < 4; ++i){
        assert_eq(list2[i].amount, list2_good[i].amount);
        assert_eq(list2[i].value, list2_good[i].value);
   
    }
    for(int i = 0; i < 4; ++i){
        assert_eq(list3[i].amount, list3_good[i].amount);
        assert_eq(list3[i].value, list3_good[i].value);
   
    }
}

void test_probabilities(void) {
    player puser = {5, {1, 2, 3, 4, 5}};
    player pcomp = {5, {2, 3, 4, 5, 6}};
    game_state pstate = {one_one, 1, puser, pcomp, 0};
    assert_true((abs(calculate_probabilities(&pstate, one_one) - 0.401878) < EPSILON));
    assert_true((abs(calculate_probabilities(&pstate, three_ones) - 0.032150) < EPSILON));
    assert_true((abs(calculate_probabilities(&pstate, three_twos) - 0.329218) < EPSILON));
    assert_true((abs(calculate_probabilities(&pstate, four_ones) - 0.003215) < EPSILON));
}

void test_calculate_probability_above(void) {
    game_state state = {one_one, 1, user, comp, 0};
    double manual_sum = calculate_probabilities(&state, four_fives) + 
    calculate_probabilities(&state, five_fives) + 
    calculate_probabilities(&state, six_fives) +
    calculate_probabilities(&state, seven_fives) +
    calculate_probabilities(&state, eight_fives) +
    calculate_probabilities(&state, nine_fives) +
    calculate_probabilities(&state, ten_fives);

    assert_true((abs(calculate_probability_above(&state, four_fives) - manual_sum) < EPSILON));
}


    
void test_get_bet(void) {
    bet sentinel = {0, 0};
    bet res;

    game_state state = {one_one, 1, user, comp, 0};
    state.moves = 3;

    bet lst1[] = {nine_sixes, one_one, two_twos, two_threes, sentinel};
    bet lst2[] = {one_one, two_twos, two_threes, nine_sixes, sentinel, sentinel};
    bet lst3[] = {two_threes, two_twos, nine_sixes, one_one, sentinel};
    bet lst4[] = {one_one, nine_sixes, two_twos, two_threes};

    res = get_bet(&state, lst1, lst2);
    assert_eq(res.value, 1);
    assert_eq(res.amount, 1);

    res = get_bet(&state, lst1, lst3);
    assert_eq(res.value, 3);
    assert_eq(res.amount, 2);

    res = get_bet(&state, lst3, lst4);
    assert_eq(res.value, 1);
    assert_eq(res.amount, 1);

    res = get_bet(&state, lst4, lst3);
    assert_eq(res.value, 3);
    assert_eq(res.amount, 2);
}


void run_tests(void) {
    tests_ran = 0;
    tests_failed = 0;

    run_test(test_check_valid_bet);
    run_test(test_dudo);
    run_test(test_calza);
    run_test(test_choose);
    run_test(test_sort_amount);
    run_test(test_get_bet);
    run_test(test_probabilities);
    run_test(test_calculate_probability_above);

    if (tests_failed == 0) {
        printf("****************\n"
               "All tests passed\n"
               "****************\n");
    } else {
        printf("****************************************************\n"
               "Perudo has %d incorrect functions out of %d tested\n"
               "****************************************************\n\n",
               tests_failed, tests_ran);
    }
}


int main(void) {
     run_tests();
     return EXIT_SUCCESS;
}
