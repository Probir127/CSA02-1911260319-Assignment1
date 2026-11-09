## Task 12 - Re-evaluate and adapt the design (8 marks)

### A. Impact analysis (2 marks)
Retain integer money, validation, bounded input, functions, header guards and warning/test practices. Modify the old 16-bit/monthly-reading model to wider cumulative watt-hour readings and timestamped 15-minute deltas. Redesign tariff storage as validated runtime data and separate exact fractional charge accumulation from final paisa rounding.

There are 96 intervals/day and 35,040 intervals/365-day year per household. Annual network record counts are 140,160,000 for 4,000 homes; 2,102,400,000 for 60,000; and 28,032,000,000 for 800,000. The district record count alone exceeds a 32-bit unsigned counter. A leap year has more intervals; the given 35,040 figure assumes 365 days.

The brief gives no maximum power draw or meter lifetime capacity, so a numeric maximum cannot be inferred from interval frequency alone. The implementation therefore declares and enforces a configurable engineering cap of at most 1,000,000 Wh per interval, not a claim about a real household's consumption. Rates are capped at 100,000 paise/kWh, surcharge at 100%, fixed charge at 1,000,000 paise/month, and the second slab boundary at 1,000,000,000 Wh. These conservative bounds make the range analysis explicit and are adjustable only through a reviewed design change if exceeded.

### B. Evaluate the original design with bounds (2 marks)
| Quantity | Bound under the declared limits | Consequence |
| --- | --- | --- |
| One-house annual energy | 35,040 x 1,000,000 = 35,040,000,000 Wh | Too large for 16-bit or 32-bit unsigned |
| District annual energy | 28,032,000,000,000,000 Wh | Fits uint64_t; far beyond 32-bit |
| One-house annual numerator | 35,040,000,000 x 100,000 x 200 = 700,800,000,000,000,000 | Fits uint64_t; check multiply/add before computing |
| One-house annual paise upper bound | 7,008,000,000,000 + 12,000,000 fixed = 7,008,012,000,000 | Fits 64-bit integer money |
| District annual paise upper bound | 5,606,409,600,000,000,000 | Below signed 64-bit maximum 9,223,372,036,854,775,807 |

The numerator is an exact rational charge scaled by 100000, not an amount already in paise. Summing every household's unrounded numerator could exceed 64 bits; finish each household/month in paise first, then use checked aggregation of the issued bills. Do not rely on a 64-bit type alone without operation-specific overflow checks. uint64_t cumulative device readings can still exhaust their lifetime range; this revised demonstration rejects decreases rather than assuming a new meter's rollover modulus.

Rates, fixed charges, slab thresholds and peak-window parameters must now be runtime tariff fields, not #define values requiring a rebuild. Technical constants such as the 15-minute slot model, scale denominator and input buffer size can remain compile-time constants.

### C. Implement the revised design (2 marks)
Task12 loads ten key=value fields from tariff.cfg. It rejects unknown/missing/duplicate keys, negative or malformed numbers, overlarge rates, invalid boundaries and peak windows. A Tariff struct holds the validated values. One Account struct holds cumulative period Wh, a rational charge numerator and interval count. Arithmetic helpers reject overflow; apply_interval updates a temporary account and commits it only after every operation succeeds.

The original marginal monthly slabs are retained, now expressed in Wh: 100000 and 200000. Consumption is allocated to slabs in timestamp order, including intervals that cross a threshold. The current interval's base marginal energy charge receives a 20% uplift if its start time is in [18:00,22:00). The fixed charge is not multiplied by the peak factor. This is an explicit interpretation of the changed tariff, since the brief does not specify whether the slabs disappear or how crossing intervals should be allocated. Replacing this interpretation requires revising the business rule and its tests.

For each portion, exact numerator += Wh x rate_paise_per_kWh x percentage_factor; dividing by 1000 x 100 converts Wh and percentage to paise. Retain the numerator through the period and round once: quotient plus one when the remainder is at least 50000. This handles 0.45-paisa subcharges without rounding each to zero.

The input command processes one household's supplied billing-period sequence: day number (1..31), slot (0..95), previous cumulative Wh and current cumulative Wh. Consecutive records must have consecutive slots and matching boundary readings. It rejects missing, duplicate, decreasing, over-limit or discontinuous intervals. The example permits a partial sequence for demonstration and prints a coverage reminder; the caller must establish the actual billing calendar/month and complete coverage before issuing a production bill. Each new billing month needs a fresh account so the free slab resets and the fixed charge is applied once. Times are interpreted as local slot starts; timezone/DST and clock-correction policies need explicit production metadata.

```text
ALGORITHM BILL_SMART_PERIOD
INPUT: validated tariff and ordered 15-minute readings for one account/month
OUTPUT: exact rounded energy charge and fixed-plus-energy total
BEGIN
    LOAD and VALIDATE tariff once
    SET consumed Wh, charge numerator and interval count TO zero
    FOR EACH contiguous interval DO
        CHECK readings, timestamp order and interval limit
        SET remaining Wh TO current reading minus previous reading
        SET percentage factor TO 120 in peak slots, otherwise 100
        WHILE remaining Wh is positive DO
            SELECT marginal slab using period consumption
            TAKE Wh up to the next slab boundary
            CHECK all multiplications and additions for overflow
            ADD Wh x slab rate x percentage factor TO numerator
            ADVANCE consumed Wh and remaining Wh
        ENDWHILE
    ENDFOR
    ROUND numerator / 100000 once, half upward
    ADD one fixed charge with an overflow check
    RETURN energy and total; require complete-period coverage for issuance
END
```

### D. Evidence-based defence (2 marks)

```text
Intervals=4 used_Wh=350000
Energy=108000 paise; fixed=5000 paise; total=Rs 1130.00
One supplied billing-period sequence; coverage must be checked before issuing a bill.
```

```text
Intervals=4 used_Wh=350000
Energy=108600 paise; fixed=5000 paise; total=Rs 1136.00
One supplied billing-period sequence; coverage must be checked before issuing a bill.
```

```text
350 kWh mixed/slab-crossing test: energy=108000 total=113000 paise PASS
slot=71 (start 17:45) energy=450 paise PASS
slot=72 (start 18:00) energy=540 paise PASS
slot=87 (start 21:45) energy=540 paise PASS
slot=88 (start 22:00) energy=450 paise PASS
Two 0.45-paisa intervals -> 1 paisa after one final rounding: PASS
Overflow and over-limit rejection leave account unchanged: PASS
Annual interval traversal=35040 count; total=35040000 Wh PASS
```

The first 100 kWh are free. The next 100 kWh at peak cost 100 x 225 x 1.20 = 27000 paise; the next 150 cost 150 x 450 x 1.20 = 81000 paise. Energy is 108000, fixed is 5000, and total is 113000 paise. Changing rate2 to 230 in the configuration raises the total to 113600 without recompilation. Slots 71/72/87/88 verify 17:45, 18:00, 21:45 and 22:00 boundary behaviour. Two off-peak 1-Wh charges at 450 paise/kWh produce 0.9 paisa together and correctly round to 1 paisa at the end.

The tests traverse 35,040 intervals and verify total Wh; this checks annual iteration/count range, not a claim to issue a yearly bill with only one monthly slab or fixed charge. Separate negative tests reject repeated/invalid configuration, missing intervals, decreasing counters, an excessive delta and a duplicated slot. Overflow rejection leaves the account unchanged. A rational-numerator stress bound supports the choice of 64-bit state; full district event data is not materialised or falsely claimed as an executed 28-billion-row benchmark.

Each interval crosses at most three slabs, so the current fixed-three-slab computation is O(1) per interval and O(N) for N intervals. A configurable s-slab version is O(s) per crossing calculation in the simple implementation. Streaming one account uses O(1) calculation state plus its tariff; processing many interleaved households needs O(h) active-account state or persistent storage. Binary-only configuration updates, calendar validation, immutable tariff versions, source authentication and a database of issued bills are the next production concerns; the classroom implementation demonstrates the revised arithmetic and validation contract.
