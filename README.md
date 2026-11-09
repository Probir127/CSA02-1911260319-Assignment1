# CSA02 - C Programming - Assignment 1 (CO1)

**Student:** PROBIR SAHA SHOHOM  
**Register number:** 1911260319  
**Course:** CSA02 - C Programming  
**Title:** Reliable Billing for a Rural Electricity Co-operative  
**Total marks:** 50  
**Repository:** https://github.com/Probir127/CSA02-1911260319-Assignment1

## Compiler

All assignment programs are written in **C17**, using `.c` source files and `.h` headers. Executed on macOS arm64 using `gcc`, which resolves to **Apple Clang 21.0.0** on this system. `compiler.txt` records the version. The optional Bash script below compiles and runs the C programs.

## Build and run everything

From the repository root:

```sh
bash tools/run_all.sh
```

This compiles/runs every task, both completed-code exercises, revision cases, intentionally failing examples and sanitizer tests. It writes executables/objects to `build/` and fresh output to `build/checks/`. The task folders retain the recorded results used in the report. An expected rejection or compiler failure counts as a passing negative test. The original valid programs use:

```sh
gcc -std=c17 -Wall -Wextra -Wpedantic -O2 SOURCE.c -o build/PROGRAM
```

`execution_commands.json` records the exact per-program commands used. Programs that call shared billing functions also compile `common/billing.c` in the same command. The core application and smart-meter commands are:

```sh
mkdir -p build
gcc -std=c17 -Wall -Wextra -Wpedantic -O2 Task09/main.c Task09/input.c common/billing.c -o build/billing
./build/billing < Task09/input_valid.txt
./build/billing < Task09/input_invalid.txt

gcc -std=c17 -Wall -Wextra -Wpedantic -O2 Task09/tests.c Task09/input.c common/billing.c -o build/tests
./build/tests

gcc -std=c17 -Wall -Wextra -Wpedantic -O2 Task12/main.c Task12/smart.c -o build/smart
./build/smart Task12/tariff.cfg < Task12/intervals.txt
./build/smart Task12/revised_tariff.cfg < Task12/intervals.txt

gcc -std=c17 -Wall -Wextra -Wpedantic -O2 Task12/tests.c Task12/smart.c -o build/smart_tests
./build/smart_tests Task12/tariff.cfg
```

For the standalone exercises:

```sh
gcc -std=c17 -Wall -Wextra -Wpedantic -O2 Task02/complete_code.c -o build/rollover
./build/rollover
gcc -std=c17 -Wall -Wextra -Wpedantic -O2 Task03/complete_code.c -o build/tariff
./build/tariff
```

Task05 DEBUG command:

```sh
gcc -std=c17 -Wall -Wextra -Wpedantic -O2 -DDEBUG Task05/header_demo.c common/billing.c -o build/debug
./build/debug
```

The exact format error is tested with `-Werror=format`; do not run `Task06/wrong_format.c`. Task08's three error examples are intentionally incorrect/missing-input programs. Task07 retains an intentional mixed-sign warning alongside the corrected comparison. On a platform requiring an explicit math library, add `-lm` when linking Task07/conversions.c. AddressSanitizer and UndefinedBehaviorSanitizer support is required for the harness's sanitizer checks.

## Task results

| Task | Result |
| --- | --- |
| 01 | Validated data model and example bill components |
| 02 | Measured type ranges/sizes and completed 330/256-unit rollover exercise |
| 03 | Completed slab exercise; one rate revision updates every affected bill |
| 04 | Macro expansion, precedence and repeated-evaluation demonstrations |
| 05 | Include guards and DEBUG/release behaviour |
| 06 | Correct formats, scanf return-value checks and rejected format mismatch |
| 07 | Conversion surprises, corrected signed comparison and precision experiment |
| 08 | Preprocessed/assembly output, object/executable sizes and three error classes |
| 09 | Five bills total Rs 1487.50; 65,536 unit-value comparisons; invalid-input tests |
| 10 | 4,000/60,000/800,000-bill precision/range/timing comparison |
| 11 | Data-type and tariff recommendation supported by the measured bounds |
| 12 | Runtime tariffs, exact fractional paise, peak window, malformed input and overflow tests |

## Rules and files

Original billing assumes domestic category D, whole kWh, one record per household/month and less than 65,536 units per period. Decreasing readings require confirmed rollover Y; an inconsistent Y is rejected. Money is integer paise with checked totals. The smart-meter demonstration processes one supplied period sequence, retains marginal slabs, applies a 20% energy-only peak uplift for slot starts from 18:00 through 21:45, and rounds once at the end. Its declared capacity/rate bounds, partial-period coverage limit and production requirements are explained in Task12/EXPLANATION.md.

Each Task folder contains C source, inputs where applicable, captured output, explanation and labelled output images. `common/` contains shared C declarations and definitions. `SOLUTIONS.md` and the PDF contain the full report, structure diagram, pseudocode, test tables and complexity comparisons. Large/generated executable artifacts remain in `build/`; Task08 retains text compilation artifacts and sizes. Output PNGs are renderings of actual captured output. Capture the terminal screenshots required by the brief when personally running the commands, and submit/share the repository link with the faculty.
