import numpy
import matplotlib.pyplot
from matplotlib.ticker import (AutoMinorLocator, MultipleLocator)

x_values = [10000, 50000, 100000]
samples = 100
fig, ax = matplotlib.pyplot.subplots(3, 1, figsize=(10, 10))

for i in range(0, 3):
    x_max = x_values[i]
    x = numpy.linspace(0, x_max, samples)

    y1 = 1 / (1 + 0.000000002 * x * x)  # <-
    y2 = 1 / (1 + 0.0001 * x)
    y3 = 1 * numpy.power(0.9999, x)

    ax[i].plot(x, y1, color='blue', label='1 / (1 + 0.000000002 * x * x)')
    ax[i].plot(x, y2, color='green', label='1 / (1 + 0.0001 * x)')
    ax[i].plot(x, y3, color='red', label='1 * numpy.power(0.99995, x)')
    ax[i].axis([0, x_max, 0, 1.0])
    ax[i].xaxis.set_major_locator(MultipleLocator(x_max / 10))
    ax[i].yaxis.set_major_locator(MultipleLocator(0.1))
    ax[i].grid(which='minor', alpha=0.2)
    ax[i].grid(which='major', alpha=0.5)
    ax[i].annotate(f'{y1[-1]:.3f}', (x[-1], y1[-1]), textcoords="offset points", ha='left', color='blue',
                   xytext=(10, -10))
    ax[i].annotate(f'{y2[-1]:.3f}', (x[-1], y2[-1]), textcoords="offset points", ha='left', color='green',
                   xytext=(10, 0))
    ax[i].annotate(f'{y3[-1]:.3f}', (x[-1], y3[-1]), textcoords="offset points", ha='left', color='red', xytext=(10, 0))

fig.suptitle("Distanz d - Schallfaktor s")
ax[-1].set_xlabel('Distanz d in m')
matplotlib.pyplot.legend(loc='upper right')
matplotlib.pyplot.savefig("sonar_entfernung.png")
