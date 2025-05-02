#!/bin/python3
import matplotlib.pyplot as plt

with open("result/plot_data") as f:
    a = list(map(float, f.readlines()))

c = a[0]

for i in range(len(a)):
    a[i] = c / a[i]

plt.plot(a)
plt.show()
