#!/usr/bin/env bash
# Compile the C17 solutions and run their positive and negative checks.
set -eu
cd "$(dirname "$0")/.."
mkdir -p build/checks
CC=${CC:-gcc}
flags=(-std=c17 -Wall -Wextra -Wpedantic -O2)

compile() {
    local name=$1
    shift
    "$CC" "${flags[@]}" "$@" -o "build/$name"
}

check() {
    local name=$1 expected=$2 actual=0
    shift 2
    "$@" > "build/checks/$name.txt" 2>&1 || actual=$?
    if [ "$actual" -ne "$expected" ]; then
        cat "build/checks/$name.txt"
        printf 'FAIL %s: expected exit %s, received %s\n' "$name" "$expected" "$actual" >&2
        exit 1
    fi
    printf 'PASS %s\n' "$name"
}

contains() {
    if ! grep -Fq -- "$2" "build/checks/$1.txt"; then
        printf 'FAIL %s: missing expected output: %s\n' "$1" "$2" >&2
        exit 1
    fi
}

compile task1 Task01/data_demo.c common/billing.c
check task1 0 build/task1
compile task2 Task02/types.c
check task2 0 build/task2
compile rollover Task02/complete_code.c
check rollover 0 build/rollover
contains rollover 'Consumption: 330 units'
contains rollover 'Consumption: 256 units'
compile task3 Task03/complete_code.c
check task3 0 build/task3
compile task3_revised Task03/revised_rate.c
check task3_revised 0 build/task3_revised
contains task3_revised '150 units -> Rs 165.00'
contains task3_revised '350 units -> Rs 955.00'
compile task4 Task04/macros.c
check task4 0 build/task4
"$CC" -E -P Task04/macros.c -o build/checks/macros.i
compile task5 Task05/header_demo.c common/billing.c
check task5 0 build/task5
compile task5_debug -DDEBUG Task05/header_demo.c common/billing.c
check task5_debug 0 build/task5_debug
contains task5_debug 'DEBUG units=150 energy=11250 fixed=5000 total=16250'
compile task6 Task06/scanf_demo.c
check scanf_valid 0 build/task6 < Task06/valid.txt
check scanf_letters 2 build/task6 < Task06/letters.txt
check scanf_negative 2 build/task6 < Task06/negative.txt
check format_error 1 "$CC" "${flags[@]}" -Werror=format -c Task06/wrong_format.c -o build/wrong.o
compile task7 Task07/conversions.c -lm
check task7 0 build/task7

"$CC" -E Task08/stages.c -o build/checks/stages.i
"$CC" "${flags[@]}" -S Task08/stages.c -o build/checks/stages.s
"$CC" "${flags[@]}" -c Task08/stages.c -o build/stages.o
"$CC" build/stages.o -o build/stages
check stages 0 build/stages
wc -c Task08/stages.c build/checks/stages.i build/checks/stages.s build/stages.o build/stages > build/checks/stage_sizes.txt
check compile_error 1 "$CC" "${flags[@]}" Task08/compile_error.c -o build/compile_error
check link_error 1 "$CC" "${flags[@]}" Task08/link_error.c -o build/link_error
compile runtime_error Task08/runtime_error.c
check runtime_error 2 build/runtime_error

compile billing Task09/main.c Task09/input.c common/billing.c
check bills_valid 0 build/billing < Task09/input_valid.txt
contains bills_valid 'total=Rs 1487.50'
check bills_invalid 2 build/billing < Task09/input_invalid.txt
check bills_overlong 2 build/billing < Task09/overlong_input.txt
contains bills_overlong 'line too long'
contains bills_overlong '0003 units=150'
compile tests Task09/tests.c Task09/input.c common/billing.c
check billing_tests 0 build/tests
compile tests_sanitized -O1 -fsanitize=address,undefined -fno-omit-frame-pointer Task09/tests.c Task09/input.c common/billing.c
check billing_sanitizers 0 build/tests_sanitized

compile task7_precision Task07/precision.c common/billing.c
compile task10_precision Task10/totals.c common/billing.c
for run in 1 2 3; do
    check "task7_precision_$run" 0 build/task7_precision
    check "task10_precision_$run" 0 build/task10_precision
done
"$CC" "${flags[@]}" -c Task09/main.c -o build/main.o
"$CC" "${flags[@]}" -c Task09/input.c -o build/input.o
"$CC" "${flags[@]}" -c common/billing.c -o build/billing.o
"$CC" build/main.o build/input.o build/billing.o -o build/billing_split
check separate_build 0 build/billing_split < Task09/input_valid.txt
cmp build/checks/bills_valid.txt build/checks/separate_build.txt
compile task11 Task11/recommendation.c common/billing.c
check task11 0 build/task11

compile smart Task12/main.c Task12/smart.c
check smart 0 build/smart Task12/tariff.cfg < Task12/intervals.txt
contains smart 'total=Rs 1130.00'
check smart_revised 0 build/smart Task12/revised_tariff.cfg < Task12/intervals.txt
contains smart_revised 'total=Rs 1136.00'
for name in duplicate_key bad_percent negative_rate; do
    check "smart_$name" 2 build/smart "Task12/$name.cfg" < Task12/intervals.txt
done
for name in gap decrease too_large duplicate; do
    check "smart_$name" 2 build/smart Task12/tariff.cfg < "Task12/$name.txt"
done
compile smart_tests Task12/tests.c Task12/smart.c
check smart_tests 0 build/smart_tests Task12/tariff.cfg
compile smart_tests_sanitized -O1 -fsanitize=address,undefined -fno-omit-frame-pointer Task12/tests.c Task12/smart.c
check smart_sanitizers 0 build/smart_tests_sanitized Task12/tariff.cfg
"$CC" --version > build/checks/compiler.txt
printf '\nAll C programs and expected-error checks passed. Results: build/checks/\n'
