#!/usr/bin/python
# -*- coding: utf-8 -*-
from ulab import numpy as np
"""
Some helpful basic signal processing functions, 
made by Klemens Iten for ME433 Advanced Mechatronics at Northwestern
based on templates by Nick Marchuk
"""

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
    frq = frq[:int(n/2)] # one side frequency range
    Y = np.fft.fft(data) # fft computing and normalization
    Y = - Y[0] / n
    Y = Y[:int(n/2)]
    return frq,Y