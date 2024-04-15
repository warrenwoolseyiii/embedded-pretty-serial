#!/bin/bash
uncrustify -c utilities/format/format.cfg --no-backup src/*.c
uncrustify -c utilities/format/format.cfg --no-backup src/*.h
uncrustify -c utilities/format/format.cfg --no-backup example/*.cpp
dos2unix src/*.c
dos2unix src/*.h
dos2unix example/*.cpp
