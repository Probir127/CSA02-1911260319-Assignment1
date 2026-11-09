## Task 5 - Headers and conditional compilation (3 marks)

### Structure and header guards
common/billing.h holds public types, constants and prototypes. common/billing.c includes it and implements consumption, tariff arithmetic, money formatting and checked addition. Each task's main function coordinates the demonstration. Task09 further separates main.c and input.c. Global declarations define interfaces and constants; mutable billing state is local to a call or account.

The header uses #ifndef CSA02_BILLING_H / #define CSA02_BILLING_H / #endif. Task05 intentionally includes it twice, demonstrating that typedef/struct declarations are not duplicated. #include <stdio.h> searches the implementation's configured system include paths; #include "../common/billing.h" first uses the implementation's quoted-header search rules, normally relative to the including source, then configured paths. Exact search order is implementation-specific, so the distinction is not a universal absolute-path rule.

The DEBUG build includes a diagnostic fprintf inside bill_for_units; the normal build removes that block during preprocessing. Its diagnostic is written to stderr.

```text
Header guards work; total=Rs 162.50
```

```text
Header guards work; total=Rs 162.50
DEBUG units=150 energy=11250 fixed=5000 total=16250
```

### Alternative: runtime debug flag
A runtime design uses if (debug_enabled) fprintf(...). In a loop over n bills, an actually retained branch is O(n) tests, each O(1), and debug instructions/strings remain in the executable. If the flag is compile-time-known, optimization may eliminate the branch. #ifdef removes the entire block in a release build, so there is no debug test or debug output code from that block. Enabled output has cost proportional to message bytes and I/O overhead; this can dominate either branch strategy. A runtime flag is more convenient for diagnosing a deployed executable; a compile-time flag gives a simpler release binary.
