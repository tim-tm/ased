#!/bin/sh

BOLD=$(tput bold)
NORMAL=$(tput sgr0)

for f in build/test/*; do
    echo "${BOLD}Testing ${f}${NORMAL} 🧪";
    ./${f};
    if [ $? -ne 0 ]; then
        echo "${BOLD}${f} failed!${NORMAL} ❎"
    else
        echo "${BOLD}${f} passed!${NORMAL} ✅"
    fi
done
