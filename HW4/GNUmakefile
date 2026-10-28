prog=wam

#include source code file
SRCS=wam.cpp

#dynamic flags to automatically adapt to Wayland
ccflags=-pthread $(shell fltk-config --cxxflags)
ldflags=-pthread $(shell fltk-config --ldflags)

ld=g++

include ../GNUmakefile
