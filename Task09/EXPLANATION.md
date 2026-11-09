## Task 9 - Implement and test the billing program (4 marks)

### Architecture and pseudocode
The caller supplies a line containing household category previous current rollover, for example 0003 D 0 150 N. The main program owns input iteration and the district total; input.c validates the source fields; common/billing.c computes units and money. Invalid lines are reported and skipped, while accepted lines continue. Exit 0 means all supplied records were accepted; exit 2 means at least one was rejected; exit 3 reports an I/O or aggregate-overflow failure.

```csa_uml
Reading -> validation -> Bill -> checked total
```

```text
ALGORITHM GENERATE_MONTHLY_BILLS
INPUT: bounded lines of household/category/readings/rollover
OUTPUT: accepted bills, reject reasons, checked batch total
BEGIN
    SET batch total TO zero
    FOR EACH input line DO
        IF line is too long or fields are invalid THEN
            REPORT rejection and CONTINUE
        ENDIF
        IF the decrease and rollover confirmation disagree THEN
            REPORT rejection and CONTINUE
        ENDIF
        COMPUTE units using validated 16-bit modular difference
        COMPUTE marginal energy charge with wide integer products
        ADD fixed charge TO form the bill total
        IF adding the bill would exceed the accumulator range THEN
            REPORT total overflow and STOP
        ENDIF
        ADD bill TO batch total
        OUTPUT household, units and bill components
    ENDFOR
    OUTPUT accepted count, rejected count and batch total
END
```

### Expected versus actual results
The expected values below were independently hand-calculated at the slab boundaries; the C assertion suite compares them to actual results.

```text
units=0 expected=5000 actual=5000 PASS
units=99 expected=5000 actual=5000 PASS
units=100 expected=5000 actual=5000 PASS
units=101 expected=5225 actual=5225 PASS
units=150 expected=16250 actual=16250 PASS
units=199 expected=27275 actual=27275 PASS
units=200 expected=27500 actual=27500 PASS
units=201 expected=27950 actual=27950 PASS
units=350 expected=95000 actual=95000 PASS
units=65535 expected=29428250 actual=29428250 PASS
65,536 array-vs-fixed comparisons; rollover, parser, sum and overflow guards: PASS
Five hand-calculated bills total: 148750 paise = Rs 1487.50
```

The five hand bills are Rs 50.00 + Rs 50.00 + Rs 162.50 + Rs 275.00 + Rs 950.00 = Rs 1487.50. Their execution is:

```text
0001 units=0 energy=Rs 0.00 fixed=Rs 50.00 total=Rs 50.00
0002 units=100 energy=Rs 0.00 fixed=Rs 50.00 total=Rs 50.00
0003 units=150 energy=Rs 112.50 fixed=Rs 50.00 total=Rs 162.50
0004 units=200 energy=Rs 225.00 fixed=Rs 50.00 total=Rs 275.00
0005 units=350 energy=Rs 900.00 fixed=Rs 50.00 total=Rs 950.00
Accepted=5 rejected=0 total=Rs 1487.50
```

The separate invalid/rollover file demonstrates letter input, negative input, a decreasing reading without confirmation, both rollover examples, a reading above 65535 and an inconsistent Y flag:

```text
REJECT line 1: invalid fields/range/category
REJECT line 2: invalid fields/range/category
REJECT line 3: inconsistent or unconfirmed rollover
0009 units=1 energy=Rs 0.00 fixed=Rs 50.00 total=Rs 50.00
0010 units=256 energy=Rs 477.00 fixed=Rs 50.00 total=Rs 527.00
REJECT line 6: invalid fields/range/category
REJECT line 7: inconsistent or unconfirmed rollover
Accepted=2 rejected=5 total=Rs 577.00
```

Additional parser assertions reject unknown category, trailing data and an overlarge numeric token. The overflow test attempts to add one to LLONG_MAX and checks that the accumulator remains unchanged. Sanitized builds run the same valid invariants and invalid-input paths. An extra bounded-line test in the execution harness confirms that a long malformed line is rejected without corrupting the following valid line.

### Alternative: arrays of slabs
The test suite also implements widths [100,100,unbounded] and rates [0,225,450], iterating across the remaining units. It agrees with the fixed code for all 65,536 possible unit values. A loop across s slabs costs O(s) time and O(s) tariff storage; the original three fixed tests are O(1) because their count never changes. Generalising a hard-coded chain to s tests also makes it O(s). Arrays are easier to extend or load from configuration, but boundaries, lengths, ordering and arithmetic limits must be validated once before use.
