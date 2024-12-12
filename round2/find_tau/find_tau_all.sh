#! /bin/sh

kani(){
  ./find_tau $*
}

echo "#pragma once"

kani 256 127 3 156 54 "if"
kani 256 31 3 165 60
kani 256 31 10 600 70
kani 256 7 10 740 100

kani 384 127 3 228 78
kani 384 31 3 246 87
kani 384 31 10 890 100
kani 384 7 10 1100 140

kani 512 127 3 306 105
kani 512 31 3 324 114
kani 512 31 10 1120 120
kani 512 7 10 1490 190

cat <<__EOS__
#else
#  error "unknown (q,L,v,m)"
#endif
__EOS__
