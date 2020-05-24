# Hashtable optimization
Assembly optimizations of a Hashtable, written in C / C++ in the previous semester. All time profiling data, which was obtained by standard MacOS time profilier, and short overview of results can be found here: /hashtable/hashtable/src.

The results:
name of hash function  |   in what language the hash function was written   |   if it was some g++ optimization  |  if strlen was optimized | all time  | time of hash function | Compare profit of hash functions: time / time_O3, %
--- | --- | --- | --- | --- | --- | ---
murmur | c | O0 | no | 54.63 | 29.85 | 238
murmur |  c  | O3 | yes | 35.59 | 12.49 | 100
murmur       |   asm  |  -   | yes   | 37.10   | 11.42 | 91
murmur     |     asm  |  O3   | no   | 38.34  |  10.75 | 86
--- | --- | --- | --- | --- | --- | ---
crc32     |         c    |O3   | yes  |  38.37 |   15.79 | 100
crc32 (by one symbol) | sse | -  | yes   |   35.25    |  10.73 | 67
crc32 (four symbols by one instruction) | sse   |  -  |  no  |  37.47  |  8.06    | 51
