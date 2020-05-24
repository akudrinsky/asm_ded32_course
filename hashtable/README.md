# Hashtable optimization
Assembly optimizations of a Hashtable, written in C / C++ in the previous semester. All time profiling data, which was obtained by standard MacOS time profilier, and short overview of results can be found here: /hashtable/hashtable/src.

The results:
name of hash function  |   in what language the hash function was written   |   if it was some g++ optimization  |  if strlen was optimized | all time  | time of hash function
--- | --- | --- | --- | --- | --- 
murmur | c | O0 | no | 54.63 | 29.85
murmur |  c  | O3 | yes | 35.59 | 12.49
crc32(1)sse | -  | yes   |   35.25    |  10.73
crc32(4)sse   |  -  |  no  |  37.47  |  8.06    
crc32     |         c    |O3   | yes  |  38.37 |   15.79
murmur       |   asm  |  -   | yes   | 37.10   | 11.42
murmur     |     asm  |  O3   | no   | 38.34  |  10.75
