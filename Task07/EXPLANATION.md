## Task 7 - Type conversion and precision (4 marks)

### Demonstrated surprises

```text
150/100=1; (double)150/100=1.50
-1 < 1U = 0 (after unsigned conversion)
Signed-domain comparison corrected = 1
unsigned char 255 + 1 = 256 (integer promotion)
(int)(INT_MAX+1L)=-2147483648 (implementation-defined in C17)
float 16777216 + 1 = 16777216
llround(1.125*100)=113 paise (half away from zero)
```

150/100 performs integer division before any later assignment; casting an operand before division produces 1.50. The signed -1 becomes a large unsigned value in comparison with unsigned 1, so the original comparison is false. The corrected signed-domain comparison first handles negativity, then casts only a non-negative value. The compiler's -Wsign-compare warning for the original teaching expression is retained in warning.txt; billing comparisons use compatible types.

unsigned char promotes to int on this machine, so 255+1 is the int value 256 rather than an 8-bit wrap. Narrowing a large signed long to int when it is outside range is implementation-defined in C17; the observed -2147483648 is not a cross-platform contract. At 2^24, this machine's float spacing becomes 2, so adding one can leave the value unchanged.

### Conversion points and rounding rule
Text readings become unsigned long only after digit validation, then narrow to meter_t after checking 65535. Rollover subtraction is deliberately converted modulo 2^16. Unit-rate multiplication widens the unit count to money_t before multiplying, avoiding a narrow intermediate. Printf arguments either match their format exactly or are explicitly cast to the declared matching type. District additions check LLONG_MAX before adding.

The original whole-kWh tariff needs no rounding because every rate is an integer number of paise. For an externally calculated non-negative fractional charge, the chosen rule is nearest paisa with exact half rounded upward. llround demonstrates half-away-from-zero on a positive exact example; validate finiteness and destination range before calling it. Multiplying a binary double by 100 can be slightly off a decimal halfway value, so it is not a universal exact decimal implementation. Task12 instead retains an integer rational numerator and performs half-up rounding once at the final energy-charge boundary.

### Alternative: float everywhere and one final rounding
Task07/precision.c runs the same 4,000/60,000/800,000-bill stress experiment as Task10, with three measured repetitions retained in precision_runs.json. Float's accumulating error changes sign and grows substantially in these examples; rounding once at the end cannot recover bits already lost. An integer addition and a float addition are both O(1); float uses 4 bytes and money_t 8 bytes here. Floating-point error depends on magnitudes and ordering: a standard repeated-addition bound has a factor approximately n*u for small n*u times the sum of absolute terms, not a guaranteed fixed error per bill. Exact integer arithmetic has zero rounding error within range, but overflow must be prevented.
