#!/bin/python3

i = 0

with open("e_mine", "r") as a:
    with open("e_nasa", "r") as b:
        c = True
        d = True
        
        while c and d:
            c = a.read(1)
            while c and c == '\n':
                c = a.read(1)
            d = b.read(1)
            while d and d == '\n':
                d = b.read(1)
            if c != d:
                print(f"Different values at {i}")
                break
            i += 1

        if not (c and d):
            print("One of the files ended")