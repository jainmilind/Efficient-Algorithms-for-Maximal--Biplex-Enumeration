#!/bin/bash
g++ main.cpp -o main.out -O3
./main.out $1 < input.txt > out.txt
