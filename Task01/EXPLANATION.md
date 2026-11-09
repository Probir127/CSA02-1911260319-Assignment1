## Task 1 - Understand the problem and identify the data (4 marks)

### Data, assumptions and invariants
The co-operative needs one monthly bill per household: energy charges for consumed kWh plus a fixed Rs 50 charge. This solution uses the assignment's worked tariff: the first 100 whole kWh are free, the next 100 cost 225 paise/kWh, and units above 200 cost 450 paise/kWh. Only domestic category D is supported because no other category tariff is specified. Unknown categories are rejected instead of borrowing an invented tariff.

| Item | Representation and reason |
| --- | --- |
| Household number | Digit string up to 31 characters, preserving leading zeroes; an identifier, not an amount |
| Category | char D after checking the complete category token |
| Previous/current reading | Unsigned 16-bit register values in 0..65535; negative/fractional input is invalid |
| Confirmed rollover | Boolean Y/N field, supplied by a verified meter event or operator |
| Consumed units | unsigned whole kWh, at most 65535 under the period assumption |
| Energy, fixed and total charge | Non-negative integer paise; products and totals use long long |
| Batch total | Checked long long accumulator, potentially much larger than an individual bill |
| Tariff and slab boundaries | One shared set of constants for the original design; stable during a run |
| Rejected records | Line number plus reason, allowing correction without treating rejection as a zero bill |

At most one rollover is allowed, with consumption strictly below 65536 units per period. If current < previous, the Y flag is required. If current >= previous, Y is inconsistent with this assumption and is rejected. Equal readings therefore mean zero consumption; a complete 65536-unit cycle cannot be inferred from readings alone. The flag is necessary because a falling reading could be a transcription error, a replacement meter, or a rollover. The arithmetic cannot establish the cause.

A correct bill has non-negative units and charges; total equals energy plus fixed; increasing units cannot reduce the bill under the supplied rates; and accumulated totals must not overflow. The original input batch is assumed to contain one record per household for one billing month. Duplicate household/month detection, replacement-meter workflows, tax, subsidy, arrears and payment handling are outside this exercise and need additional records/rules before real deployment.

### Text readings as an alternative
A reading such as 01530 can be stored in char[6] including the terminating zero, while uint16_t uses two bytes here. Validated conversion requires O(d) work for d digits and checks for signs, non-digits and out-of-range values. Comparing arbitrary-length numeric strings requires removing leading zeroes, comparing significant lengths, then digits: O(d), not ordinary lexicographic comparison (which orders 100 before 20). Integer comparison is O(1) for these fixed-width types. The final input implementation deliberately reads bounded text and converts once, then keeps numeric readings for calculation.

### Program result

```text
Household=00001234 category=D previous=1200 current=1530 units=330
Energy: Rs 810.00
Fixed: Rs 50.00
Total: Rs 860.00
Nonnegative and sum invariant: PASS
```
