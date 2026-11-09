## Task 11 - Recommend and defend the design (4 marks)

### Recommendation supported by results
Use uint16_t only for the old hardware register; use unsigned units after validated conversion; keep household numbers as bounded strings; use char for validated category and bool for rollover confirmation; use long long paise for individual charges and aggregate money. Define tariff boundaries and rates once in an include-guarded header, with wide multiplication operands. Prefer functions over side-effect-sensitive calculation macros, keep a controlled DEBUG build, and compile with warnings enabled.

The evidence is concrete: the two rollover examples match 256 and 1 units; all 65,536 tariff calculations agree with an independent slab-array implementation; the five bills total 148750 paise; a 32-bit int is inadequate but long long holds the 23,542,600,000,000-paise district bound; float produces substantial errors. The tested ordinary and smart-meter paths also pass sanitizer checks.

Change the original tariff in the shared definition, rebuild all dependants, rerun the boundary/revision tests, and deploy one consistent version. The design remains arithmetically correct at district scale under its stated maximum consumption and tariff. It does not automatically solve ambiguous rollover, duplicate monthly records, meter replacement, authentication, persistence or concurrent district processing. Those require additional business and storage controls.

```text
Maximum monthly bill=29428250 paise
District bound=23542600000000 paise
long long maximum=9223372036854775807; bound fits=yes
Use checked integer paise, a confirmed rollover flag, and one tariff definition.
```

### Alternative: startup configuration
Read t tariff values from a versioned file, validate required names, duplicates, numeric syntax, bounds and slab ordering, then keep a typed tariff object fixed for the run. Parsing is O(total configuration bytes), commonly O(t) for bounded-length values; retained storage is O(t). This small startup cost avoids recompiling and redistributing binaries for rate-only changes. Runtime configuration must not change midway through an account's bill, and a real bill should retain the tariff version used. Task12 implements this approach and proves that changing one rate changes the same executable's result.
