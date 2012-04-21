#!/bin/sh
# Usage chiptest.sh <system#> <test#>
# For example: chiptest.sh 5 0
# shell file to test chiptest through mtip, using mtip ~S, ~s "sleep" commands:
#  ~S for 10-sec wait, ~s for 1-sec wait
#  use 20 secs for reset, then CR, then 1 sec, then ~d, 10 secs for download
echo systems 5 and 6 each have a good LS00 chip to test
echo and systems 7 and 8 each have a good LS138
mtip -b $1 -f chiptest.lnx << MRK
~r~S~S
~S~d~SGO 100100
~s$2
~s
~S~q
MRK
