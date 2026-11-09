## Task 10 - Expectations versus execution (4 marks)

### Measured totals and overflow checks
Every test bill uses 65535 units, the largest admissible original-period consumption, so exact totals can be independently checked by multiplying 29,428,250 paise by n. The experiment uses volatile accumulators to ensure each addition is executed, without fast-math reassociation.
| Bills | Exact paise | Float error (paise) | Double error (paise) | int / long / long long safe |
| --- | --- | --- | --- | --- |
| 4000 | 117713000000 | -3035200 | 0 | 0 / 1 / 1 |
| 60000 | 1765695000000 | -802328000 | 0 | 0 / 1 / 1 |
| 800000 | 23542600000000 | 45780672000 | 0 | 0 / 1 / 1 |

Here 1 means the entire total is representable, and 0 means it is not. The exact district total is 23,542,600,000,000 paise, well within this machine's 64-bit long and long long, but outside 32-bit int. The first maximum-bill addition that would exceed INT_MAX is number 73. The program tests representability before addition rather than executing signed overflow, whose behaviour is undefined. A 32-bit long platform also fails this district requirement, despite being valid C.

The float difference is -3,035,200 paise at 4,000 bills, -802,328,000 paise at 60,000, and +45,780,672,000 paise at 800,000. The different signs show why a universal fixed correction is invalid. Integer paise matches the independently computed product exactly, and double rupees also matches for this quarter-rupee dataset and magnitude.

### Timing and storage
| Bills | Float ms | Integer ms | Double ms |
| --- | --- | --- | --- |
| 4000 | 0.019000 | 0.008000 | 0.015000 |
| 60000 | 0.230000 | 0.120000 | 0.233000 |
| 800000 | 3.189000 | 0.414000 | 2.963000 |

These are CPU-clock medians from three local runs, in milliseconds, with raw runs retained. Very small differences are sensitive to clock granularity, optimization and background work. The volatile loop deliberately prevents elision, so the results demonstrate the implemented additions rather than a universal performance ranking. Total runtime is O(n), each fixed-width addition O(1), and the three accumulators require O(1) extra memory. The integer and double variables are 8 bytes each; float is 4 bytes here.

### Alternative: a double district total
Double provides enough precision for this measured 800,000-bill case. As an integer-paise accumulator, a binary64 double can represent every integer only through 2^53; above that some adjacent paise cannot be distinguished even though the exponent range is far from exhausted. As rupees, its unit spacing must remain small enough for the required decimal cents, and decimal rates can introduce earlier rounding error. A long long retains every integer paise within its range, then overflows if unchecked. Both use 8 bytes and O(1) additions on this machine; exact fixed-point money with explicit overflow guards offers the clearest invariant.
