#!/usr/local/bin/python3
import random
import sys
import time
import threading

def play_rhythm_game():
    print("instructions:")
    print("hit the keys h j k or l in order of the appearing note")
    print("the order of the notes to hit are: | h | j | k | l |")
    print("in order to win, you have to hit 1000 keys in 10 seconds")
    input("press enter to begin\n")

    print("")
    print("3...")
    time.sleep(1)
    print("2...")
    time.sleep(1)
    print("1...")
    time.sleep(1)
    print("GO!\n")

    rows = list('|   |   |   |   |')
    keys = ["h", "j", "k", "l"]
    total_notes = 1000
    correct_notes = 0
    start_time = time.time()

    def timer():
        time.sleep(10)
        if correct_notes < total_notes:
            print("\nTime's up! You lost!")
            exit()

    threading.Thread(target=timer).start()

    while correct_notes < total_notes:
        note = random.randint(0, 3)
        buf = rows[:]
        buf[2+note*4] = 'X'
        buf = "".join(buf)
        print(buf)
        try:
            player_input = sys.stdin.read(1)
        except:
            sys.exit(0)

        if player_input == keys[note]:
            correct_notes += 1
        else:
            print("Wrong note! You lost!")
            sys.exit(0)

    print(f"Notes hit: {correct_notes}/{total_notes} in 10 seconds.")
    if correct_notes >= total_notes:
        print("Congratulations! Here's the flag: ", end="")
        print(open("/flag.txt", "r").read())

if __name__ == "__main__":
    play_rhythm_game()

