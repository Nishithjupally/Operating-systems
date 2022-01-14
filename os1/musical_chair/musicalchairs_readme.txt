Read me:

Total Files submitted : 3

1. musicalchairs_prog.cpp
2. musicalchairs_readme.txt
3. musicalchairs_report.pdf


Instructions to compile and execute :

To compile :
$ g++ Prog_Asg2.cpp -pthread

To execute :
$ ./a.out --np <nplayers> < <inputfile.txt>

<nplayers> is the total number of players in the game and <inputfile.txt> is file with all the input instructions.


Sample Input and Output :


Input :

lap_start
player_sleep 0 1000
player_sleep 1 2000
player_sleep 2 3000
player_sleep 3 4000
music_start
umpire_sleep 200
music_stop
lap_stop
lap_start
player_sleep 0 1000
player_sleep 1 2000
player_sleep 2 3000
music_start
umpire_sleep 200000
music_stop
lap_stop
lap_start
player_sleep 0 1000
player_sleep 1 2000
music_start
umpire_sleep 800000
music_stop
lap_stop


Output :

Musical Chairs: 4 player game with 3 laps.
======= lap# 1 =======
3 could not get chair
**********************
======= lap# 2 =======
0 could not get chair
**********************
======= lap# 3 =======
2 could not get chair
**********************
Winner is 1
Time taken for the game: 1004796 us

