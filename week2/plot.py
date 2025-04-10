import matplotlib.pyplot as plt
import re
roll = []
pitch = []

with open('log', 'r') as f:
    line = f.readlines()
    for i, l in enumerate(line):
        if (i == 0): 
            continue
        if (i == len(line) - 1):
            continue
        if l.startswith("Pitch"):
            values = list(map(float, re.findall(r"[-+]?\d*\.\d+|\d+", l)))
            pitch.append(tuple(values))
        elif l.startswith("Roll"):
            values = list(map(float, re.findall(r"[-+]?\d*\.\d+|\d+", l)))
            roll.append(tuple(values))

accel_pitch = []
intl_pitch = []
filter_pitch = []
for element in pitch:
    # Append the first value of each tuple to the list
    accel_pitch.append(element[0])
    intl_pitch.append(element[1])
    filter_pitch.append(element[2])


accel_roll = []
intl_roll = []
filter_roll = []
for element in roll:
    # Append the first value of each tuple to the list
    accel_roll.append(element[0])
    intl_roll.append(element[1])
    filter_roll.append(element[2])
    


# Plot each item in roll list that corresponds to t...for each t we have three values

def plot_roll():
    plt.plot(accel_roll, label='Accel Roll')
    plt.plot(intl_roll, label='Intl Roll')
    plt.plot(filter_roll, label='Filter Roll')
    plt.title('Roll')
    plt.xlabel('Time')
    plt.ylabel('Roll Value')
    plt.legend()
    plt.show()
 
def plot_pitch():
    plt.plot(accel_pitch, label='Accel Pitch')
    plt.plot(intl_pitch, label='Intl Pitch')
    plt.plot(filter_pitch, label='Filter Pitch')
    plt.title('Pitch')
    plt.xlabel('Time')
    plt.ylabel('Pitch Value')
    plt.legend()
    plt.show()
    
plot_roll()
plot_pitch()