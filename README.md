# Perudo
Final Year 1 Programming Project Extension, written in C

Our Year 1 Final Programming Project was to design and build an ARM11 emulator and assembler in C. This code has not been uploaded, due to plagiarism restrictions (as the assignment is reused for other years).

However, the extension is individual, so this can be uploaded. 

For our extension we wanted to make something distinct from the Emulator and Assembler. We
decided to build Perudo, a dice game of luck, strategy and bluff. We considered a two-player game,
with the user playing against the computer. In Perudo, each player starts with 5 dice. One player
starts by placing a bet on a quantity of a particular dice value between the two players, for example
”Three fours”. This bet is true if there are three or more dice with a four face-up on the table.
The next player then ”improves” this bet, by increasing it either by value (e.g. ”Three fives”) or by
quantity. When increasing quantity, the value can remain the same or go down, (e.g. ”Five threes”
or ”Five fours” would both be valid). The value 1 acts not only as a 1, but is ”wild”, in the sense
that it can also take on any value. As only a 1 counts as a 1, this in turn reduces the expected
number of 1s present on a given round. As a result, one can ”improve” upon the previous bet by
declaring a number of 1s that, if the previous player bet n of a certain dice roll, is > n/2. For example, one could bet ”Three ones” after the bet ”Four threes” (as the next viable quantity after three is
four, and 4/2 = 2.

A round ends in two ways. Firstly, instead of betting, the player can call Dudo. This means that
they believe that the previous player’s bet exceed the true number of dice of a certain value on the
table. If they are indeed correct, the other player loses one of their dice, and if the Dudo call was
incorrect, the player calling Dudo loses a dice. Alternatively, a player can call Calza. This means
they think there are exactly that number of a particular dice value on the table. If they are right,
they gain a dice they have previously lost (in the case which they already have 5 dice, they start the
next turn with 5 dice), otherwise, they lose a dice. The game ends when a player loses all of their
dice, with the player with dice remaining as the winner. The loser of the previous round then starts,
excluding the case of a Calza.

When one player only has one dice, we enter Palifico mode. In Palifico mode, the player on one
dice must start the round and whatever value they choose as their starting bet must be the same
for all following bets, for example, if a player starts with ”One four”, the opponent must then reply
with ”Two fours” or any quantity of fours greater than 1.

The game logic and computer player AI code can be found in Perudo.c and Perudo.h. The GUI
code as well as game loop logic, that fuses the two parts together, can be found in ext.c and ext.h.
Example Use We managed to complete a functioning version of the game, which combines the
logic with the interface. From playing the game, we found the computer wins about 50% of the time.
