# Spell Checker

A spell checker implemented in standard C++17 without third-party libraries.

## Requirements

- CMake 3.16 or newer
- A compiler supporting C++17 (e.g. MSVC in Visual Studio 2022 or GCC 14)

## Project structure

```text
cpp-spell-checker/
├── src/
│   ├── SpellChecker.h
│   ├── SpellChecker.cpp
│   └── main.cpp
├── tests/
│   └── SpellCheckerTests.cpp
├── CMakeLists.txt
└── README.md
```

## Build

```sh
cmake -S . -B build
cmake --build build --config Debug
```

## Run

The program reads a dictionary from standard input (stdin) until a line equal to `===`, then reads text until the next `===` line. It writes corrected text to standard output (stdout), preserving non-word characters and whitespace.

Run the application from the Windows terminal:

```
.\build\Debug\spell_checker.exe
```

## Correction rules

- Words are matched case-insensitively.
- A word already in the dictionary is printed unchanged.
- Suggestions use at most two edits; an edit is insertion or deletion of one letter.
- Two insertions or two deletions cannot affect adjacent characters.
- One-edit suggestions take priority over two-edit suggestions.
- Multiple suggestions are printed in dictionary order.
- If no correction is found, the original word is printed as `{word?}`.

## Tests

Run the tests from the Windows terminal:

```powershell
ctest --test-dir build -C Debug --output-on-failure
.\build\Debug\SpellCheckerTests.exe
```

The test program uses only the C++ standard library.
