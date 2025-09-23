########################################################################
# Makefile.am for the HOLOS project - GNU parametrizable build
# This file follows GNU standards and is processed by automake.
# Each variable and section is commented for clarity and portability.
########################################################################

# Automake options:
# 'foreign' allows a less strict structure than GNU standard.
# 'subdir-objects' allows object files (.o) in subdirectories.
AUTOMAKE_OPTIONS = foreign subdir-objects

# Include the header path for all targets.
AM_CPPFLAGS = -I$(top_srcdir)/include

# Compiler flags and required libraries.
# AM_CFLAGS: additional flags for the C compiler.
# LDADD: libraries to link to executables.
AM_CFLAGS = $(GSL_CFLAGS) $(GNUNET_CFLAGS)
LDADD = $(GSL_LIBS) $(READLINE_LIBS) $(GNUNET_LIBS)

# Main static library (libholos.a) containing all physics modules.
# lib_LIBRARIES: lists the static libraries to build.
# holos_a_SOURCES: source files included in the library.
lib_LIBRARIES = holos.a
holos_a_SOURCES = \
    src/core/phys/astro/celestial_mechanics.c \
    src/core/phys/astro/orbital_dynamics.c \
    src/core/phys/em/lorentz_forces.c \
    src/core/phys/em/maxwell.c \
    src/core/phys/em/wave_equations.c \
    src/core/phys/fields/qft.c \
    src/core/phys/mech/analytical/hamilton.c \
    src/core/phys/mech/analytical/lagrange.c \
    src/core/phys/mech/classical/dynamics.c \
    src/core/phys/mech/classical/fluids.c \
    src/core/phys/mech/classical/kinematics.c \
    src/core/phys/mech/classical/oscillations.c \
    src/core/phys/mech/classical/statics.c \
    src/core/phys/mech/continuum/elasticity.c \
    src/core/phys/mech/continuum/fluid_dynamics.c \
    src/core/phys/mech/quantum/particle_in_box.c \
    src/core/phys/mech/quantum/quantum_harmonic_oscillator.c \
    src/core/phys/mech/quantum/schrodinger.c \
    src/core/phys/mech/relativistic/relativistic_mech.c \
    src/core/phys/nuclear/fission_fusion.c \
    src/core/phys/nuclear/nuclear_decay.c \
    src/core/phys/nuclear/nuclear_forces.c \
    src/core/phys/ss/band_structure.c \
    src/core/phys/ss/crystal_lattice.c \
    src/core/phys/ss/phonons.c \
    src/core/phys/thermo/entropy.c \
    src/core/phys/thermo/heat_transfer.c \
    src/core/phys/thermo/thermodynamics.c

# Single CLI executable (holos) that also includes the P2P module.
# bin_PROGRAMS: lists the executables to install.
# holos_SOURCES: source files for the executable.
# holos_LDADD: libraries to link to the executable.
bin_PROGRAMS = holos
holos_SOURCES = src/cli/cli.c src/p2p/p2p.c
holos_LDADD = holos.a $(LDADD)

# Automatic tests.
# check_PROGRAMS: test executables (not installed).
# TESTS: lists the tests to run with 'make check'.
check_PROGRAMS = tests
TESTS = tests
tests_SOURCES = tests/tests.c
tests_LDADD = holos.a $(LDADD)
tests_CFLAGS = $(AM_CFLAGS)