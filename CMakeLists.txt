
cmake_minimum_required(VERSION 3.16)

project(cpp_spell_checker LANGUAGES CXX)

add_executable(spell_checker
    src/main.cpp
 "src/SpellChecker.cpp")

set_target_properties(spell_checker PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
