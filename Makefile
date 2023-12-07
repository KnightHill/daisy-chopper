# Project Name
TARGET = Chopper

# Sources
CPP_SOURCES = chopperMain.cpp chopper.cpp util.cpp base_hardware_module.cpp

# Library Locations
LIBDAISY_DIR = ../../libDaisy
DAISYSP_DIR = ../../DaisySP

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

