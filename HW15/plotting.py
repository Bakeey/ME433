# get a line of raw bitmap and plot the components
import serial
import numpy as np

ser = serial.Serial('COM6',230400) # the name of your Pico port
print('Opening port: ')
print(ser.name)

ser.write(b'hi\r\n') # send a newline to request data
data_read = ser.read_until(b'\n',50) # read the echo

sampnum = 0
index = 0
raw = []
reds = []
greens = []
blues = []
bright = []

# Pico sends back index and raw pixel value
while sampnum < 60: # width of bitmap
    data_read = ser.read_until(b'\n',50) # read until newline
    data_text = str(data_read,'utf-8') # convert bytes to string
    data = list(map(int,data_text.split())) # convert string to values

    if(len(data)==2):
        index = data[0]
        raw.append(data[1])
        reds.append(((data[1]>>5)&0x3F)/0x3F*100) # red value is middle 6 bits
        greens.append((data[1]&0x1F)/0x1F*100) # green value is rightmost 5 bits
        blues.append(((data[1]>>11)&0x1F)/0x1F*100) # blue vale is leftmost 5 bits
        bright.append((data[1]&0x1F)+((data[1]>>5)&0x3F)+((data[1]>>11)&0x1F)) # sum of colors
        sampnum = sampnum + 1

# print the raw color as a 16bit binary to double check bitshifting
for i in range(len(reds)):
    print(f"{raw[i]:#018b}")

def gaussian_array(n):

    if n == 1:
        return np.array([1])

    if n % 2 == 0:
        raise ValueError("n must be odd.")

    midpoint = (n - 1) // 2
    array = np.zeros(n)

    for i in range(n):
        array[i] = np.exp(-0.5 * ((i - midpoint) / (0.25 * midpoint)) ** 2)

    return array

n = 7  # Size of the array

reds = np.convolve(gaussian_array(1), reds, mode='same')
greens = np.convolve(gaussian_array(1), greens, mode='same')
blues = np.convolve(gaussian_array(1), blues, mode='same')



# plot the colors 
import matplotlib.pyplot as plt 
x = range(len(reds)) # time array
fig, axs = plt.subplots(4)
axs[0].plot(x,reds,'r*-',x,greens,'g*-',x,blues,'b*-')

# second order finite difference
fd_filter = np.array([1,-2,1])
reds_edge = np.convolve(fd_filter, reds, mode='same')
reds_edge[0] = 0
reds_edge[-1] = 0

axs[1].plot(reds_edge)
axs[1].stem(np.argsort(reds_edge)[:2],[100,100], 'r')

# second order finite difference
fd_filter = np.array([-1,0,1])
blue_edge = np.convolve(fd_filter, blues, mode='same')
blue_edge[0] = 0
blue_edge[-1] = 0

axs[2].plot(blue_edge)
axs[2].stem([np.argmin(blue_edge), np.argmax(blue_edge)],[100,100], 'b')

# second order finite difference
green_edge = np.convolve(fd_filter, greens, mode='same')
fd_filter = np.array([-1,0,1])
green_edge[0] = 0
green_edge[-1] = 0

axs[3].plot(green_edge)
axs[3].stem(np.argsort(green_edge)[:2],[100,100], 'g')

plt.show()

plt.ylabel('color')
plt.xlabel('position')
plt.show()







# be sure to close the port
ser.close()