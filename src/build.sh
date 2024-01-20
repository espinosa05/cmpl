#!/bin/bash
gcc -g -O3 -Wall                   \
    main.c                      \
    util.c                      \
    cmpl.c                      \
    media_types/generic.c       \
    media_types/ISO_BMFF.c      \
    media_types/mp3.c           \
    media_types/wav.c           \
    -o cmpl
