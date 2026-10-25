cmake_minimum_required(VERSION 3.16)

project(cpp_spell_checker LANGUAGES CXX)

add_executable(spell_checker
    src/main.cpp
    src/SpellChecker.cpp
)

add_executable(SpellCheckerTests
    tests/SpellCheckerTests.cpp
    src/SpellChecker.cpp
)

target_include_directories(SpellCheckerTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(spell_checker PRIVATE cxx_std_17)
target_compile_features(SpellCheckerTests PRIVATE cxx_std_17)

set_target_properties(spell_checker SpellCheckerTests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

enable_testing()

add_test(
    NAME SpellCheckerTests
    COMMAND SpellCheckerTests
)