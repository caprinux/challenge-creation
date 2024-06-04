#!/usr/local/bin/python

from time import time
from random import randint

def start_pi_game():
    MAX_NUM_DIGITS = 10000
    MAX_DIGIT_RANGE = 1250000 - MAX_NUM_DIGITS
    NUM_TIMES = 314

    with open("./flag.txt", "r") as f:
        FLAG = f.read()

    CUT_OFF_TIME = time() + 15
    with open("./Pi125MDP.txt", 'r') as f:
        pi = '3' + f.read()

    for i in range(NUM_TIMES):
        if time() > CUT_OFF_TIME:
            print("You ran out of time.")
            return
        offset = randint(0, MAX_DIGIT_RANGE)
        len = randint(1, MAX_NUM_DIGITS)
        offset = 2
        len = 2
        # need proper grammar :D
        if len == 1:
            print(f"What is the {offset}-th digit of pi?")
        else:
            print(f"What are the {len} digits of pi starting from the {offset}-th digit?")

        inp = input("> ")
        if inp != pi[offset-1:offset-1+len]:
            print("Wrong! Please go and memorize it again!")
            return
        else:
            continue

    print("Congratulations, you did it!")
    print("Here is a flag for your hard work :)", FLAG)


print("The first 6 digits of pi are \"3.14159\" whereby `3` is the 1st digit, `1` is the 2nd digit, `4` is the 3rd digit and so on.")
print("Example: What are the 3 digits of pi starting from the 3rd digit?")
print("Answer: 415\n")
print("You will have to do this for 314 times in 1 seconds.")
print("Are you ready?")
ans = input("(y/n) > ")

if ans == 'y' or ans == 'Y':
    print("")
    start_pi_game()
else:
    print("Unfortunate. Goodbye!")
