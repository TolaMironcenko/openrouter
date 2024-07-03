#!/bin/sh

Green='\033[0;32m'
Color_off='\033[0m'
Red='\033[0;31m'

if [ ! -f IP ]; then
    printf "[$Red ERR $Color_off] Cant't find IP file\n"
    exit 1
fi




