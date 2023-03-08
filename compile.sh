#!/bin/sh -e

cmake -S . -B build -G "Unix Makefiles"

cmake --build build

