import matplotlib.pyplot as plt
import re

thrust = []
desired_pitch = []
filtered_pitch = []
front = []
back = []
pitch_angle = []
pitch_velocity = []
measured_pitch = []

with open('log', 'r') as f:
    line = f.readlines()
    for i, l in enumerate(line):
        if (i == 0): 
            continue
        if (i == len(line) - 1):
            continue
        if l.startswith("Desired Pitch"):
            values = list(map(float, re.findall(r"[-+]?\d*\.\d+|[-+]?\d+", l)))
            desired_pitch.append(values[0])
        elif l.startswith("Filtered Pitch"):
            values = list(map(float, re.findall(r"[-+]?\d*\.\d+|[-+]?\d+", l)))
            filtered_pitch.append(values[0])
        elif l.startswith("Thrust"):
            values = list(map(float, re.findall(r"[-+]?\d*\.\d+|[-+]?\d+", l)))
            thrust.append(values[0])
        elif l.startswith("Motor Front"):
            values = list(map(float, re.findall(r"[-+]?\d*\.\d+|[-+]?\d+", l)))
            front.append(values[0])
        elif l.startswith("Motor Back"):
            values = list(map(float, re.findall(r"[-+]?\d*\.\d+|[-+]?\d+", l)))
            back.append(values[0])
        elif l.startswith("Pitch Angle"):
            values = list(map(float, re.findall(r"[-+]?\d*\.\d+|[-+]?\d+", l)))
            pitch_angle.append(values[0])
        elif l.startswith("Pitch Velocity"):
            values = list(map(float, re.findall(r"[-+]?\d*\.\d+|[-+]?\d+", l)))
            pitch_velocity.append(values[0])
        elif l.startswith("Measured Pitch"):
            values = list(map(float, re.findall(r"[-+]?\d*\.\d+|[-+]?\d+", l)))
            measured_pitch.append(values[0])


# Just plot all the lists in one graph with legends and different colors
def plot_p():
    plt.plot(thrust, label='Thrust')
    plt.plot(desired_pitch, label='Desired Pitch')
    plt.plot(filtered_pitch, label='Filtered Pitch')
    plt.plot(front, label='Front Motors')
    plt.plot(back, label='Back Motors')
    plt.title('Milestone 1')
    plt.legend()
    plt.show()
    
def plot_d():
    plt.plot(thrust, label='Thrust')
    plt.plot(pitch_angle, label='Pitch Angle')
    plt.plot(desired_pitch, label='Desired Pitch')
    plt.plot(front, label='Front Motors')
    plt.plot(back, label='Back Motors')
    plt.title('Milestone 2')
    plt.legend()
    plt.show()


def plot_i():
    #plt.plot(thrust, label='Thrust')
    plt.plot(desired_pitch, label='Desired Pitch')
    plt.plot(measured_pitch, label='Filtered Pitch')    
    plt.plot(front, label='Front Motors')
    plt.plot(back, label='Back Motors')
    plt.title('Milestone 4')
    plt.legend()
    plt.show()


plot_i()