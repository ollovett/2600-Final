#!/bin/bash
echo "enter 1 to play"
read input
SECONDS=0
duration=$SECONDS
if [[ "$input" -eq "1" ]]; then
    ./ticTacToe.out
elif [[ "$SECONDS" -eq "10" ]]; then
    ./autoToe.out