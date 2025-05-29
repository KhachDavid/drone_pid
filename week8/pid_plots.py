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
integral_pitch = []
desired_yaw = []
desired_roll = []
measured_yaw = []
motor_tr = []
motor_tl = []
camera_data_yaw = []

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
        elif l.startswith("Camera Data Yaw"):
            values = list(map(float, re.findall(r"[-+]?\d*\.\d+|[-+]?\d+", l)))
            camera_data_yaw.append(values[0])
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
        elif l.startswith("Integral Pitch"):
            values = list(map(float, re.findall(r"[-+]?\d*\.\d+|[-+]?\d+", l)))
            integral_pitch.append(values[0])
        elif l.startswith("Desired Yaw"):
            values = list(map(float, re.findall(r"[-+]?\d*\.\d+|[-+]?\d+", l)))
            desired_yaw.append(values[0])
        elif l.startswith("Measured Yaw"):
            values = list(map(float, re.findall(r"[-+]?\d*\.\d+|[-+]?\d+", l)))
            measured_yaw.append(values[0])
        elif l.startswith("Motor Top Right"):
            values = list(map(float, re.findall(r"[-+]?\d*\.\d+|[-+]?\d+", l)))
            motor_tr.append(values[0])
        elif l.startswith("Motor Top Left"):
            values = list(map(float, re.findall(r"[-+]?\d*\.\d+|[-+]?\d+", l)))
            motor_tl.append(values[0])
        elif l.startswith("Desired roll"):
            values = list(map(float, re.findall(r"[-+]?\d*\.\d+|[-+]?\d+", l)))
            desired_roll.append(values[0])







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
    plt.plot(filtered_pitch, label='Filtered Pitch')    
    plt.plot(front, label='Front Motors')
    plt.plot(back, label='Back Motors')
    plt.plot(integral_pitch, label='Integral Pitch')
    plt.title('Milestone 1')
    plt.legend()
    plt.show()

def plot_yaw():
    plt.plot(desired_yaw, label='Desired yaw')
    plt.plot(measured_yaw, label='Measured yaw velocity')    
    plt.plot(motor_tl, label='Motor Top Left')
    plt.plot(motor_tr, label='Motor Top Right')
    plt.title('Yaw Milestone')
    plt.legend()
    plt.show()

def plot_camera_yaw():
    plt.plot(camera_data_yaw, label='Camera Yaw')
    plt.title('Milestone 2: Yaw over time')
    plt.legend()
    plt.show()

def plot_desired_roll():
    plt.plot(desired_roll, label='Desired Roll')
    plt.title('Desired Roll')
    plt.legend()
    plt.show()


plot_desired_roll()