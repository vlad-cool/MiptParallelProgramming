#!/bin/python3
import sys
import matplotlib.pyplot as plt

plot_data = {}

with open(sys.argv[1]) as f:
    for line in f.readlines():
        [name, size, time, *_] = line.split(", ")
        size = int(size)
        time = float(time)

        if name in plot_data.keys():
            plot_data[name][0].append(size)
            plot_data[name][1].append(time)
        else:
            plot_data[name] = ([size], [time])

plt.figure(figsize=(12, 5))

for key in plot_data.keys():
    plt.plot(plot_data[key][0], plot_data[key][1], label=key)

plt.legend()

if len(sys.argv) > 2:
    plt.savefig(sys.argv[2])

for key in plot_data.keys():
    time_1 = plot_data[key][1][-1]
    if "task_simd" in key:
        time_ref = plot_data["fft_simd"][1][-1]
    elif "task" in key:
        time_ref = plot_data["fft"][1][-1]
    else:
        continue
    
    print(f"{key}: ускорение: {time_ref / time}, эффективность: {time_ref / time / 16}")

# plt.show()
