## Task 8 - Trace compilation (4 marks)

### Four stages and observed artifacts
The following commands were executed; build holds local object/executable artifacts, while the text outputs are included with this task.

```sh
gcc -E Task08/stages.c -o Task08/stages.i
gcc -std=c17 -Wall -Wextra -S Task08/stages.c -o Task08/stages.s
gcc -std=c17 -Wall -Wextra -c Task08/stages.c -o build/stages.o
gcc build/stages.o -o build/stages
./build/stages
```

Preprocessing expands the CHARGE macro to 5000L, includes header declarations, handles conditional compilation and removes comments. The .i file contains resulting C tokens and source-location directives, not executable instructions. Compilation produces target assembly in .s. Assembly creates a relocatable object containing machine code and symbol/relocation information. Linking resolves references such as printf against the runtime/libraries and creates the executable. The machine here produces arm64 Mach-O artifacts; a GNU/Linux system would normally produce ELF, so binaries are rebuilt rather than distributed as portable deliverables.

```json
{
  "stages.c": 111,
  "stages.i": 25825,
  "stages.s": 1035,
  "stages.o": 760,
  "stages": 33432
}
```

```text
Fixed charge=5000 paise
```

### Errors and warnings, correctly classified
compile_error.c references undeclared missing_variable; translation fails before an object can be linked. link_error.c has a valid declaration of missing_tariff but no definition; compilation can succeed, yet linking reports an undefined symbol. runtime_error.c builds successfully, then tries to open a deliberately absent input file, reports the failure and exits 2. These are respectively a compilation error, a link error and a handled runtime failure; they are separate from undefined behaviour.

-Wall -Wextra identified the wrong printf format and the mixed-sign comparison. The production code uses the correct format, explicit validated conversions and compatible types. The original mixed-sign expression is retained only in the teaching demonstration with its warning and a corrected expression alongside it. Additional AddressSanitizer/UndefinedBehaviorSanitizer test builds completed without reported faults in the tested billing and smart-meter paths.

### Alternative: separate translation units
The final billing executable was also built from separate objects for Task09/main.c, Task09/input.c and common/billing.c, then linked; its five-bill output exactly matches the single compile command. A change only to input.c needs recompiling that translation unit and relinking, whereas a monolithic source needs recompiling its whole body. A shared-header change can require every dependent file to rebuild; a dependency-aware build system must track that. Compile time roughly follows the amount of translated source and optimization work, not simply the number of files. Multiple object files consume the combined machine-code/data/symbol storage plus per-file overhead; the linked result can still have similar executable size.
