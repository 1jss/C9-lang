# Linter for the C9 language
# Uses simple regex to check for disallowed patterns

import re
import sys

def lint(filename):
    # Open the file
    with open (filename, 'r') as file:
        line_num = 0
        for line in file:
            line_num += 1

            # Rule: 'goto' is not allowed at all
            if re.search(r'\ goto\ ', line):
                print(f'{filename}:{line_num}: Warning: Found goto')  

            # Rule: No built in integer types are allowed as they can vary in size. Use `inttypes.h` instead.
            if re.search(r'\ (int|short|long)\ ', line):
                print(f'{filename}:{line_num}: Warning: Found built-in integer type')

            # Rule: The long and short keywords are not allowed as they can vary in size between systems.
            if re.search(r'\ (long|short)\ ', line):
                print(f'{filename}:{line_num}: Warning: Found long or short keyword')

            # Rule: Prefer using long variable names and avoid abbreviations
            # Check for variable names that are less than 3 characters long
            if re.search(r'\ \w{1,2}\ =\ ', line):
                print(f'{filename}:{line_num}: Warning: Found short variable name')

            # Rule: Do not use flexible array members in structs
            if re.search(r'\[\]\s*;', line):
                print(f'{filename}:{line_num}: Warning: Found flexible array member in struct')

            # Rule: Function declaration without parameters is not allowed
            if re.search(r'\(\s*\)\s*\{', line):
                print(f'{filename}:{line_num}: Warning: Found function declaration without parameters')

            # Rule: Prefer using the stack instead of the heap when possible.
            if re.search(r'\bmalloc\b', line):
                print(f'{filename}:{line_num}: Warning: Found use of malloc')

            # Rule: Preprocessor is only used for includes and conditionals
            if re.search(r'^\s*#\s*(undef|ifdef|else|elif|error|pragma)', line):
                print(f'{filename}:{line_num}: Warning: Found use of preprocessor directive')

# Lint all files passed as arguments
for filename in sys.argv[1:]:
    lint(filename)