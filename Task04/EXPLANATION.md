## Task 4 - Use macros safely (4 marks)

### Expansion and pitfalls
The examples use SQUARE(x) ((x)*(x)), MAX(a,b) (((a)>(b))?(a):(b)) and RUPEES_TO_PAISE(r) ((r)*100LL). The last macro is for whole non-negative rupees within the multiplication range, not arbitrary fractional rupees. Full parentheses fix grouping; they do not validate range or remove duplicated evaluation.

```text
BAD_SQUARE(a+1)=7; SQUARE(a+1)=16
MAX(i++,j)=6; i after=7
max_int(i++,j)=5; i after=6
square_int(a+1)=16; 50 rupees=5000 paise
```

For a=3, BAD_SQUARE(a+1) expands to a+1*a+1, which is 7 rather than 16. Parentheses give ((a+1)*(a+1)). For i=5,j=3, MAX(i++,j) increments i during the condition, then evaluates the selected i++ arm again: result 6 and final i 7. These two evaluations are sequenced by the conditional operator. By contrast, SQUARE(i++) has unsequenced modifications and must not be used; no undefined-behaviour output is offered as a portable result.

The actual gcc -E -P output is retained in Task04/preprocessed.i. Its main function contains the expanded expression tokens; includes and macro definitions have already been processed. static inline square_int and max_int evaluate each argument once. The function call example gives result 5 and final i 6.

### Alternative: static inline functions
Use functions for nontrivial calculations, validation, side-effectful inputs and clearer type contracts. Parameter types constrain conversions. Both forms are O(1) per small operation. A non-inlined function has call/return overhead; static inline permits but does not force inlining. With k uses, macro-expanded source grows O(k); emitted machine code depends on optimization and may merge/fold expressions. An outlined function has one body plus O(k) call sites, while an inlined body can also grow O(k). It is incorrect to claim that macro expansion necessarily runs faster or always creates a larger final binary.
