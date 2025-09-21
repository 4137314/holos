AUTOMAKE_OPTIONS = foreign subdir-objects

bin_PROGRAMS = holos
holos_SOURCES = \
    src/astro/celestial_mechanics.c \
    src/astro/orbital_dynamics.c \
    src/electromagnetism/lorentz_forces.c \
    src/electromagnetism/maxwell.c \
    src/electromagnetism/wave_equations.c \
    src/fields/qft.c \
    src/mechanics/analytical/hamilton.c \
    src/mechanics/analytical/lagrange.c \
    src/mechanics/classical/dynamics.c \
    src/mechanics/classical/fluids.c \
    src/mechanics/classical/kinematics.c \
    src/mechanics/classical/oscillations.c \
    src/mechanics/classical/statics.c \
    src/mechanics/continuum/elasticity.c \
    src/mechanics/continuum/fluid_dynamics.c \
    src/mechanics/quantum/particle_in_box.c \
    src/mechanics/quantum/quantum_harmonic_oscillator.c \
    src/mechanics/quantum/schrodinger.c \
    src/mechanics/relativistic/relativistic_mech.c \
    src/nuclear/fission_fusion.c \
    src/nuclear/nuclear_decay.c \
    src/nuclear/nuclear_forces.c \
    src/solid_state/band_structure.c \
    src/solid_state/crystal_lattice.c \
    src/solid_state/phonons.c \
    src/thermo/entropy.c \
    src/thermo/heat_transfer.c \
    src/thermo/thermodynamics.c

# Includi path delle intestazioni
AM_CPPFLAGS = -I$(top_srcdir)/include

# Librerie richieste
LDADD = $(GSL_LIBS)
AM_CFLAGS = $(GSL_CFLAGS)