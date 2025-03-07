#!/bin/bash

function printStyle() {
  local cyan='\033[0;36m'
  local bold=$(tput bold)
  local nc='\033[0m'
  local style="${cyan}${bold}"
  local msg=$1
  echo -e "$style##### $msg ##### $nc"
}

function startAdas() {
  printStyle 'Building CMakelist.txt'
    g++ -std=c++17 PC2.cpp Kommunication.cpp -lb15fdrv -O3 -o PC2.o # && ./$_
    g++ -std=c++17 PC1.cpp Kommunication.cpp -lb15fdrv -O3 -o PC1.o #  && ./$_
  printStyle 'done'
}

function test() {
  g++ -std=c++17 -O3 bitset_test.cpp -o out.o && ./out.o
}

startAdas
