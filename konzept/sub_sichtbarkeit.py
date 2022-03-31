import numpy
import matplotlib.pyplot
from matplotlib.ticker import (AutoMinorLocator, MultipleLocator)

# x = Geschwindigkeit in m/s
# y = Sichtbarkeit [0,1] (über 1 kappen?)
entfernungen = [500, 1000, 5000, 10000, 25000, 50000, 100000]
fig, ax = matplotlib.pyplot.subplots(len(entfernungen), 1, figsize=(10, 4 * len(entfernungen)))
for i, entfernung in enumerate(entfernungen):
    x_max = 25
    samples = 100
    x = numpy.linspace(0, x_max, samples)

    sichtbarkeiten = [0.01, 0.1, 0.25, 0.5, 0.75, 1.0]
    entfernungsfaktor = 1 / (1 + 0.000000002 * entfernung * entfernung)  # <- !!! verwendete Entfernungsformel
    for s, sichtbarkeit in enumerate(sichtbarkeiten):
        # y = (0.001 * x) + (0.0005 * x*x*x) * entfernungsfaktor * sichtbarkeit
        # y = numpy.power((0.5 + (numpy.arctan(0.25 * x * x * entfernungsfaktor - (5.0 / numpy.sqrt(sichtbarkeit))) / (3.15))), 0.25) - 0.25
        sichtbarkeitseinfluss = 3
        x_verschiebung = -3
        x_faktor = 0.25
        y_streckung = 2.5
        y_verschibung = 0.2
        steigungsglaettung = 0.15
        y = entfernungsfaktor * y_streckung * \
            (y_verschibung + (steigungsglaettung *
             numpy.arctan(((x_faktor * x) + x_verschiebung) + (sichtbarkeitseinfluss * sichtbarkeit))))
        ax[i].plot(x, y, label=f'Sichtbarkeit {sichtbarkeit})')

    ax[i].axis([0, x_max, 0, 1.0])
    ax[i].xaxis.set_major_locator(MultipleLocator(x_max / 10))
    ax[i].yaxis.set_major_locator(MultipleLocator(0.1))
    ax[i].grid(which='minor', alpha=0.2)
    ax[i].grid(which='major', alpha=0.5)
    ax[i].set_ylabel(f'd = {entfernung}m ({entfernungsfaktor:.3f})')

fig.suptitle("Sichtbarkeitsfaktor nach Geschwindigkeit v in m/s und Entferndung d in m")
ax[0].legend(loc='lower right')
ax[-1].set_xlabel('v in m/s')
matplotlib.pyplot.savefig("sub_sichtbarkeit.png")
