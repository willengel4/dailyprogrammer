import matplotlib.pyplot as plt
import numpy as numpy
import tkinter
import sys

""" Gets the output image file name """
outFileName = sys.argv[sys.argv.index('-o') + 1] + '.png'

numArrays = 5
nums = 1000

arraysX = []
arraysS = []

for i in range(numArrays):
    x = numpy.random.randint(5, size=1000)
    x += 1
    s = []
    arraysX.append(x)
    arraysS.append(s)

for i in range(1, nums):
    for f in range(numArrays):
        arraysS[f].append(numpy.average(arraysX[f][0:i]))

for i in range(numArrays):
    plt.plot(arraysS[i])

plt.savefig(outFileName)