#!/bin/bash
uncrustify -c utilities/format/format.cfg --no-backup src/*.c
uncrustify -c utilities/format/format.cfg --no-backup src/*.h
uncrustify -c utilities/format/format.cfg --no-backup example/c++/*.cpp
uncrustify -c utilities/format/format.cfg --no-backup example/c/*.c
dos2unix src/*.c
dos2unix src/*.h
dos2unix example/c++/*.cpp
dos2unix example/c/*.c
