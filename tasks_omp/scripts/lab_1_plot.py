#!/bin/python3
import sys
import matplotlib.pyplot as plt

plot_data = {}

with open(sys.argv[1]) as f:
    for line in f.readlines():
        [name, size, time] = line.split(", ")
        size = int(size)
        time = float(time)

        if name in plot_data.keys():
            plot_data[name][0].append(size)
            plot_data[name][1].append(time)
        else:
            plot_data[name] = ([size], [time])


fig, ax = plt.subplots()

lines = []

for key in plot_data.keys():
    (line,) = ax.plot(plot_data[key][0], plot_data[key][1], label=key)
    lines.append(line)

ax.legend(loc='center left', bbox_to_anchor=(1.05, 0.5))

leg = ax.legend()

for legline, origline in zip(leg.get_lines(), lines):
    legline.set_picker(True)  # enable picking
    legline._associated_line = origline  # link legend to line


def on_pick(event):
    legline = event.artist
    origline = legline._associated_line
    visible = not origline.get_visible()
    origline.set_visible(visible)
    # Fade legend entry when line is hidden
    legline.set_alpha(1.0 if visible else 0.2)
    fig.canvas.draw()


fig.canvas.mpl_connect("pick_event", on_pick)
plt.show()

# def onpick(event):
#     legend_line = event.artist
#     for orig_line, legend_line_ in zip(lines, leg.get_lines()):
#         if event.artist == legend_line_:
#             orig_line.set_visible(not orig_line.get_visible())
#             plt.draw()


# for legend_line in leg.get_lines():
#     legend_line.set_picker(5)

# fig.canvas.mpl_connect("pick_event", onpick)
# plt.show()
