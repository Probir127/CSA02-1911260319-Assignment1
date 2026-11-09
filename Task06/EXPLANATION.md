## Task 6 - Read and print data correctly (4 marks)

### Format specifiers and safe conversion
| Data type | scanf destination | printf argument |
| --- | --- | --- |
| int | %d with int* | %d |
| unsigned int | %u with unsigned int* | %u |
| long | %ld with long* | %ld |
| long long | %lld with long long* | %lld |
| float | %f with float* | %f: float promotes to double |
| double | %lf with double* | %f (printf %lf has the same floating conversion in C17) |
| char | %c with char*; leading space in format skips whitespace | %c: promoted to int |
| char array | width-limited %31s into char[32] | %s with a terminated string |
| uint16_t / uint64_t | Exact-width SCN macros or checked text conversion | PRI macros or a validated cast to a matching type |

scanf_demo checks that two assignments succeeded, then validates 0..65535. The saved valid, letter and negative input cases show both parsing and range rejection:

```text
Enter previous and current readings: scanf assignments=2
Accepted previous=1200 current=1530
Example formatted bill: Rs 162.50
```

```text
Enter previous and current readings: scanf assignments=0
Invalid numeric input
```

```text
Enter previous and current readings: scanf assignments=2
Reading outside 0..65535
```

Integer paise is printed using quotient and a two-digit remainder: p/100 and p%100. This is exact for a non-negative validated monetary amount and prints Rs 162.50 for 16250 paise.

### Wrong-specifier test
Task06/wrong_format.c deliberately passes a long to printf %d. Compiling with -Werror=format rejects it; the diagnostic is saved in format_diagnostic.txt. A mismatched variadic format has undefined behaviour, so there is no portable meaningful runtime answer to promise. A scanf %d destination pointing to long is also wrong and can overwrite only part of an object or worse. The corrected form is printf("%ld", amount) and scanf("%ld", &amount), or the appropriate fixed-width macro. The deliberately invalid example is not executed.

### Alternative: fgets plus checked parsing
A direct numeric scanf conversion also cannot safely handle an arbitrary numeric token outside the destination type's representable range merely by checking its return value. It can leave an invalid token in the stream, making repeated reads fail. The robust Task09 program therefore reads each bounded line with fgets, detects/discards an overlong line, parses bounded string tokens with sscanf and converts digit-only readings through strtoul with errno/end-pointer/range checks. Negative strings are rejected before conversion.

Both approaches inspect O(L) characters for a line of length L. fgets requires an O(L) buffer if the whole line is retained, or a fixed-cap buffer plus explicit overlength rejection as used here. Direct scanf can avoid a full line buffer, but recovery requires consuming the rest of the failed line; %s still needs a bounded destination. Line-based parsing makes each rejection independent of the next input record.
