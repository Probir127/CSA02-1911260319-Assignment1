## Task 3 - Define the constants (3 marks)

### Central tariff and completed exercise
The production examples share common/billing.h. SLAB1_UNITS, SLAB2_UNITS, RATE1_PAISE, RATE2_PAISE, RATE3_PAISE and FIXED_CHARGE_PAISE have uppercase names that expose units and purpose. Wide money literals use LL, and operands are widened before multiplication. The separate supplied Complete the Code exercise retains its long return type and original macros; its TODO implements the three marginal blocks from highest to lowest. Negative units are outside that helper's valid domain and are rejected by the input layer.

```text
0 units -> Rs 50.00
100 units -> Rs 50.00
150 units -> Rs 162.50
200 units -> Rs 275.00
350 units -> Rs 950.00
```

Changing only RATE2_PAISE from 225 to 230 in revised_rate.c produces:

```text
0 units -> Rs 50.00
100 units -> Rs 50.00
150 units -> Rs 165.00
200 units -> Rs 280.00
350 units -> Rs 955.00
```

The 0- and 100-unit bills correctly remain Rs 50.00 because their second-slab consumption is zero. The 150-unit bill rises by 50 x 5 = 250 paise; the 200- and 350-unit bills each rise by 100 x 5 = 500 paise. A consistent tariff revision changes every affected bill, not necessarily every bill.

### #define, const and enum
#define performs token replacement before C type checking, has no run-time object/storage, and is also useful in #if expressions. Parenthesise expression-like replacements and choose a suitable suffix. A typed const variable has C scope and type checking and can have an address/debugger representation; the optimizer may remove its storage. In C17 a const int is not generally an integer constant expression for a case label or file-scope array bound. enum is suitable for small integral named choices such as categories/status codes; C17 enumerator values must fit int, so it is not the general solution for very large monetary limits.

Replacing macros with static const money_t rates and const unsigned boundaries preserves O(1) work per fixed-three-slab bill. A compiler can fold either form. Memory differs only if const objects are retained; #define and enum constants create no corresponding addressable run-time object. A tariff revision is made in the one shared header, then all dependent translation units are rebuilt together. Tasks 11-12 show why runtime tariff data is preferable when revisions must not require a rebuild.
