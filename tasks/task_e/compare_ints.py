#!/bin/python3

i = 0

with open("e_mine", "r") as a:
    with open("e_nasa", "r") as b:
# with open("test_1", "r") as a:
#     with open("test_2", "r") as b:
        c = True
        d = True
        
        while c and d:
            c = a.read(1)
            d = b.read(1)
            print(c, d)
            if c != d:
                print(f"Different values at {i}")
                break
            i += 1
