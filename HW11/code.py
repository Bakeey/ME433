import board
import time
import digitalio

from ulab import numpy as np
from python_fft import fft

# heartbeat
built_in_led = digitalio.DigitalInOut(board.LED)
built_in_led.direction = digitalio.Direction.OUTPUT
timestamp = time.time()

# Define the sampling rate and duration
fs = 1024
duration = 1

# Define the frequencies of the sine waves
f1 = 100
f2 = 200
f3 = 300

# Create an array of time values
t = np.linspace(0, duration, duration * fs, endpoint=False)

# Generate the three sine waves with different frequencies
sine_wave1 = np.sin(2 * np.pi * f1 * t)
sine_wave2 = np.sin(2 * np.pi * f2 * t)
sine_wave3 = np.sin(2 * np.pi * f3 * t)

# Sum the three sine waves to get the final signal
signal = sine_wave1 + sine_wave2 + sine_wave3
freq, Y = fft(t, signal)

for ti,sine in zip(t,signal):
    print(str(ti)+","+str(sine))	# Writing data in the opened file

for frequency,magnitude in zip(freq,Y):
    print(str(frequency)+","+str(abs(magnitude)))	# Writing data in the opened file

# # write to CSV
# file=open("fft.csv","a")	# creation and opening of a CSV file in Write mode
# # Type Program Logic Here
# for frequency,magnitude in zip(freq,Y):
#     file.write(str(freq)+","+str(magnitude)+",")	# Writing data in the opened file
# # file.flush()			# Internal buffer is flushed (not necessary if close() function is used)
# file.close()			# The file is closed

while 1:
    # print("("+str(adc_pin_a0.value)+",)") # print with plotting format
    if time.time() > timestamp:
        timestamp = time.time()
        if built_in_led.value == 1:
            built_in_led.value = 0
        else:
            built_in_led.value = 1
    


    time.sleep(.05) # delay in seconds