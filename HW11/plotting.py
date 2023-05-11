#!/usr/bin/python
# -*- coding: utf-8 -*-
import matplotlib.pyplot as plt
import numpy as np
import csv
"""
Some helpful basic signal processing functions, 
made by Klemens Iten for ME433 Advanced Mechatronics at Northwestern
based on templates by Nick Marchuk
"""

def plot_fft(t,y,frq,Y,title = None):
    """
    Plots signal over time as well as frequency spectrum over frequency range.
    Inputs:
        t (time as list), 
        data (signal data as list)
        frq (one side frequency range of signal), 
        Y (fft of signal)
    """
    fig, (ax1, ax2) = plt.subplots(2, 1)
    if title is not None:
        fig.suptitle(title, fontsize=16)
    ax1.plot(t,y,'b')
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Amplitude')
    ax2.loglog(frq,Y,'b') # plotting the fft
    ax2.set_xlabel('Freq (Hz)')
    ax2.set_ylabel('|Y(freq)|')
    plt.show()

def fft(t, data):
    """
    Computes FFT of some given function.
    Inputs: 
        t (time as list), 
        data (signal data as list)
    Returns: 
        frq (one side frequency range of signal), 
        Y (fft of signal)
    """
    dt = t[-1] / len(t)
    Fs = 1/dt
    n = len(data) # length of the signal
    k = np.arange(n)
    T = n/Fs
    frq = k/T # two sides frequency range
    frq = frq[range(int(n/2))] # one side frequency range
    Y = np.fft.fft(data)/n # fft computing and normalization
    Y = Y[range(int(n/2))]
    return frq,Y

def main():
    """
    Main Loop
    """
    for filename in ['sine.csv']:
        t = []
        data = []
        with open(filename) as f:
            # open the csv file
            reader = csv.reader(f)
            for row in reader:
                # read the rows 1 one by one
                t.append(float(row[0])) # leftmost column
                data.append(float(row[1])) # second column

    for filename in ['fft.csv']:
        frq = []
        Y = []
        with open(filename) as f:
            # open the csv file
            reader = csv.reader(f)
            for row in reader:
                # read the rows 1 one by one
                frq.append(float(row[0])) # leftmost column
                Y.append(abs(float(row[1]))) # second column

        plot_fft(t,data,frq,Y, title='FFT of three combined sine waves with frequencies 100/200/300 Hz and sampling rate 1024 Hz')

if __name__ == "__main__":
    exit(main())