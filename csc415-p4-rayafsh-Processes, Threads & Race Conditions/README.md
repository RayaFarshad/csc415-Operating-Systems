# CSC 415 - Project 4 - Thread Racing

## Student Name: Raya Farshad

## Student ID :  917940106

## Build Instructions
gcc -I -Wall pthread_race.c -o threadracer -pthread
## Run Instructions
./threadracer
## Explain why your program produces the wrong output
As we know the correct answer is zero cause we are adding and subtracting the same amount, so the global value should stay the same. But here I try to put nanosleep within the adder function (before storing the temp value to the global value) in order to suspend the execution of the calling thread until the signal delivered to the calling thread.and that suspension time may be longer due to the scheduling of other activity of the system.
