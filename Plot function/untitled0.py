# -*- coding: utf-8 -*-
"""
Created on Tue Dec  5 13:43:14 2023

@author: Ke Ma
"""

import math

# Sigmoid function
def sigmoid(x, k, d1):
    return 1.0 / (1.0 + math.exp(-k * (x - d1)))

# Calculate acceleration for each piece
def calc_accel(vf, v_delta, d, piece):
    if piece == 1:
        return -5.6226 - 0.79831 * vf + 0.24575 * v_delta + 0.50558 * d
    elif piece == 2:
        return -4.6795 - 0.027768 * vf + 0.25227 * v_delta + 0.090712 * d
    elif piece == 3:
        return -4.8431 - 0.024813 * vf + 0.24804 * v_delta + 0.090502 * d
    elif piece == 4:
        return 5.4891 - 3.6667 * vf - 2.9898 * v_delta + 2.0431 * d
    else:
        return 0 # Default case

# NewModel function with smoothed transitions
def new_model(vf, v_delta, d, w=1, k=2):
    bp1 = 9.793
    bp2 = 9.507
    value, a_hat1, a_hat2, x = 0, 0, 0, 0
    smooth = False

    if vf < bp1 - w and v_delta < bp2 - w:
        # Piece 1
        value = calc_accel(vf, v_delta, d, 1)
    elif vf < bp1 - w and v_delta >= bp2 + w:
        # Piece 2
        value = calc_accel(vf, v_delta, d, 2)
    elif vf >= bp1 + w and v_delta < bp2 - w:
        # Piece 3
        value = calc_accel(vf, v_delta, d, 3)
    elif vf >= bp1 + w and v_delta >= bp2 + w:
        # Piece 4
        value = calc_accel(vf, v_delta, d, 4)
    else:
        # Smooth transitions
        smooth = True
        if vf < bp1 - w and bp2 - w <= v_delta < bp2 + w:
            # Transition between Piece 1 and Piece 2 (A)
            a_hat1 = calc_accel(vf, bp2 - w, d, 1)
            a_hat2 = calc_accel(vf, bp2 + w, d, 2)
            x = v_delta
        elif vf >= bp1 + w and bp2 - w <= v_delta < bp2 + w:
            # Transition between Piece 3 and Piece 4 (B)
            a_hat1 = calc_accel(vf, bp2 - w, d, 3)
            a_hat2 = calc_accel(vf, bp2 + w, d, 4)
            x = v_delta
        elif bp1 - w <= vf < bp1 + w and v_delta >= bp2 + w:
            # Transition between Piece 2 and Piece 4 (C)
            a_hat1 = calc_accel(bp1 - w, v_delta, d, 2)
            a_hat2 = calc_accel(bp1 + w, v_delta, d, 4)
            x = vf
        elif bp1 - w <= vf < bp1 + w and v_delta < bp2 - w:
            # Transition between Piece 1 and Piece 3 (D)
            a_hat1 = calc_accel(bp1 - w, v_delta, d, 1)
            a_hat2 = calc_accel(bp1 + w, v_delta, d, 3)
            x = vf
        else:
            # Smoothed transition for all four pieces
            A = calc_accel(bp1 - w, bp2 - w, d, 1)
            B = calc_accel(bp1 + w, bp2 - w, d, 3)
            C = calc_accel(bp1 + w, bp2 + w, d, 4)
            D = calc_accel(bp1 - w, bp2 + w, d, 2)
            value = 0.25 * (A + B + C + D)
            return value

    if smooth:
        s = sigmoid(x, k, bp2 if x < bp1 else bp1)
        value = a_hat1 + (a_hat2 - a_hat1) * s

    return value

# Example usage of the new_model function
a = new_model(9.5, 9.5, 30) 
b = new_model(10, 10, 30) 
a0 = new_model(7, 5, 30) 
a00 = new_model(8, 5, 30) 
a000 = new_model(9, 5, 30) 
a1 = new_model(10, 5, 30)  # Example input values
a2 = new_model(10, 5, 40) 
a3 = new_model(10, 5, 50) 
a4 = new_model(10, 5, 60) 