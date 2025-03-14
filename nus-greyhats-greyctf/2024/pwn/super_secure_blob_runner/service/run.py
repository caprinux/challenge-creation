#!/usr/bin/python3

import os
import secrets
import hashlib
from time import time

class NcPowser:
    def __init__(self, difficulty=22, prefix_length=16):
        self.difficulty = difficulty
        self.prefix_length = prefix_length

    def get_challenge(self):
        return secrets.token_urlsafe(self.prefix_length)[:self.prefix_length].replace('-', 'b').replace('_', 'a')

    def verify_hash(self, prefix, answer):
        h = hashlib.sha256()
        h.update((prefix + answer).encode())
        bits = ''.join(bin(i)[2:].zfill(8) for i in h.digest())
        return bits.startswith('0' * self.difficulty)

# print(os.urandom(4).hex())
#

if __name__ == '__main__':
    powser = NcPowser(difficulty=20)
    prefix = powser.get_challenge()
    print(f"sha256({prefix} + ???) == {'0'*powser.difficulty}({powser.difficulty})...")
    last = int(time())
    if powser.verify_hash(prefix, input("answer: ")):
        os.execve("./chall", ["./chall"], {})
    else:
        print("wrong, exiting.")
