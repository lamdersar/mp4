#!/bin/sh
# Usage: scope.sh <system#> <char> <baudrate>
# for example: scope.sh 9 a 9600
# shell file to test scope through mtip, using mtip "sleep" commands:
#  ~S for 10-sec wait, ~s for 1-sec wait
#  use 20 secs for reset, then CR, then 1 sec, then ~d, 10 secs for download
echo systems 9 and 10 each have a scope setup:
echo  COM1 TXD is connected to LPT1 BUSY, via a line receiver chip
mtip -b $1 -f scope.lnx << MRK
~r~S~S
~S~d~SGO 100100
~s$2
~s$3
~S~q
MRK
