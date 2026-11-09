## Task 2 - Choose the data types (4 marks)

### Measured sizes and ranges
The command named gcc on this Mac is Apple Clang 21.0.0, targeting arm64-apple-darwin27.0.0. The examples were compiled as C17. They use the requested gcc command syntax; these are not claimed as GNU GCC results. sizeof reports C bytes; CHAR_BIT reports the number of bits in each byte.

```text
CHAR_BIT=8
char: 1 bytes, -128..127
signed char: 1 bytes, -128..127
unsigned char: 1 bytes, 0..255
short: 2 bytes, -32768..32767
unsigned short: 2 bytes, 0..65535
int: 4 bytes, -2147483648..2147483647
unsigned int: 4 bytes, 0..4294967295
long: 8 bytes, -9223372036854775808..9223372036854775807
unsigned long: 8 bytes, 0..18446744073709551615
long long: 8 bytes, -9223372036854775808..9223372036854775807
unsigned long long: 8 bytes, 0..18446744073709551615
float: 4 bytes, normal min=1.17549435e-38 max=3.40282347e+38 digits=6 mantissa=24
double: 8 bytes, normal min=2.2250738585072014e-308 max=1.7976931348623157e+308 digits=15 mantissa=53
long double: 8 bytes, max=1.79769313e+308 mantissa=53
16-bit register after 65535 + 1: 0
```

The chosen core types are uint16_t for the hardware register, unsigned for consumed units, char arrays for identifiers, char for category, bool for confirmation, and long long for paise and batch totals. uint16_t is available on this platform and its exact 16-bit range is asserted. The exercise's unsigned short is also explicitly asserted to have maximum 65535. A 16-bit register increment wraps to zero through conversion to the unsigned type; this does not imply signed overflow is allowed.

C guarantees at least 8 bits in a byte, short/int ranges of at least -32767..32767, long at least -2147483647..2147483647 and long long at least -9223372036854775807..9223372036854775807. Unsigned counterparts cover at least 0..65535, 0..4294967295 and 0..18446744073709551615 respectively for short/int, long and long long. sizeof(char) is always 1; plain char signedness is implementation-defined. This machine has 8-byte long, but a platform with 4-byte long is conforming. Never assume long is always district-safe.

float has a large exponent range, but only 24 binary precision bits here; double has 53. FLT_MIN/DBL_MIN above are the smallest positive normal values, not the most negative values or the smallest subnormal values. The standard minimum decimal precision is 6 digits for float and 10 for double; this machine provides 6 and 15 through FLT_DIG/DBL_DIG.

### Exact money and a measured float failure
A maximum original-period bill is 29,428,250 paise = Rs 294,282.50. Adding that valid bill 4,000 times should give Rs 1,177,130,000.00. The float accumulator gives Rs 1,177,099,648.00: an understatement of Rs 30,352.00. The integer result is exactly 117,713,000,000 paise. This is a deliberately high-consumption stress case, not a statement about typical household usage.

The original tariff's quarter-rupee increments are binary-representable; the demonstrated error comes from precision lost as the accumulated total becomes large, not from pretending that 2.25 itself is inexact. Future decimal tariffs can introduce representation error even before accumulation. Integer paise preserves amounts exactly until overflow, which must still be checked.

### Completed rollover code and explanation
Only the TODO region was filled; it adds a compile-time width assertion and returns (unsigned short)(current - previous), without if-else. On this system the operands promote to int, subtraction gives -65280, and conversion to 16-bit unsigned reduces modulo 65536. Thus the rollover result is 136 remaining increments to zero plus 120 further increments = 256.

```text
sizeof(meter_t) = 2 bytes, maximum reading = 65535
Consumption: 330 units
Consumption: 256 units
```

### Alternative: double rupees
A double design can compute a bill in rupees, validate finite/range bounds, and round half-up to paise at each bill boundary before adding final monetary amounts. Rounding only the final district sum can differ from summing individually issued bills. Both fixed-width additions are O(1); double and long are both 8 bytes on this machine, while long can be 4 bytes elsewhere. double exactly represents the quarter-rupee values in this particular tested range, so it matched these totals, but that does not prove arbitrary decimal tariff computations are exact. Binary rounding error depends on values, ordering and precision; it is not necessarily monotonic or always in one direction.
