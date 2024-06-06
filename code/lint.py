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
            import re

            # Rule: 'break' is not allowed outside of switch statements
            # This rule is difficult to enforce with a regex as it requires context-sensitive parsing.
            # We will skip this rule for now.

            # Rule: 'goto' is not allowed at all
            if re.search(r'\ goto\ ', line):
                print(f'{filename}:{line_num}: Warning: Found goto')

            # Rule: 'const' is not allowed in local scope (inside functions)
            # If const is preceded by more than one space it is probably not in global scope
            if re.search(r'\ +\ +const\ ', line):
                print(f'{filename}:{line_num}: Warning: Found const in local scope')  

            # Rule: No built in integer types are allowed as they can vary in size. Use `inttypes.h` instead.
            if re.search(r'\ (int|short|long)\ ', line):
                print(f'{filename}:{line_num}: Warning: Found built-in integer type')

            # Rule: Implicit ints are not used nor allowed.
            # This rule is difficult to enforce with a regex as it requires context-sensitive parsing.
            # We will skip this rule for now.

            # Rule: The long and short keywords are not allowed as they can vary in size between systems.
            if re.search(r'\ (long|short)\ ', line):
                print(f'{filename}:{line_num}: Warning: Found long or short keyword')

            # Rule: All variables and pointers must be initiated with a value.
            # We will skip this rule for now.

            # Rule: Prefer using long variable names and avoid abbreviations
            # Check for variable names that are less than 3 characters long
            if re.search(r'\ \w{1,2}\ =\ ', line):
                print(f'{filename}:{line_num}: Warning: Found short variable name')

            # Rule: Tentative definitions are not allowed
            # This rule is difficult to enforce with a regex as it requires context-sensitive parsing.
            # We will skip this rule for now.

            # Rule: Names of structs and unions must be unique
            # This rule is difficult to enforce with a regex as it requires context-sensitive parsing.
            # We will skip this rule for now.

            # Rule: Use `const` for constants in global scope
            # This rule is difficult to enforce with a regex as it requires context-sensitive parsing.
            # We will skip this rule for now.

            # Rule: All structs are typedefs
            if re.search(r'[^(typedef )]struct\s+\w+\s*\{', line):
                print(f'{filename}:{line_num}: Warning: Found struct without typedef')

            # Rule: Do not use flexible array members in structs
            if re.search(r'\[\]\s*;', line):
                print(f'{filename}:{line_num}: Warning: Found flexible array member in struct')

            # Rule: K&R function definition style is not allowed
            if re.search(r'\)\s*\n\s*\w+', line):
                print(f'{filename}:{line_num}: Warning: Found K&R function definition style')

            # Rule: Function declaration without parameters is not allowed
            if re.search(r'\(\s*\)\s*\{', line):
                print(f'{filename}:{line_num}: Warning: Found function declaration without parameters')

            # Rule: Prefer initalizing functions as static except main
            if re.search(r'^\s*int32_t\s+\w+\(', line):
                print(f'{filename}:{line_num}: Warning: Found non-static function')

            # Rule: Variable length arrays are not allowed.
            if re.search(r'\[\w+\]', line):
                print(f'{filename}:{line_num}: Warning: Found variable length array')

            # Rule: Pointers are allowed but avoided when possible by passing by value instead of reference.
            # This rule is subjective and depends on the specific use cases in your codebase.
            # We will skip this rule for now.

            # Rule: Prefer using the stack instead of the heap when possible.
            if re.search(r'\bmalloc\b', line):
                print(f'{filename}:{line_num}: Warning: Found use of malloc')

            # Rule: Freeing in the same scope as the allocation is mandatory.
            # This rule is difficult to enforce with a regex as it requires context-sensitive parsing.
            # We will skip this rule for now.

            # Rule: Preprocessor is only used for includes and conditionals
            if re.search(r'^\s*#\s*(undef|ifdef|else|elif|error|pragma)', line):
                print(f'{filename}:{line_num}: Warning: Found use of preprocessor directive')

            # Rule: No macros, object-like or functional-like are allowed
            if re.search(r'^\s*#define', line):
                print(f'{filename}:{line_num}: Warning: Found macro definition')

            # Rule: Only single line `//` comments are allowed
            if re.search(r'/\*', line):
                print(f'{filename}:{line_num}: Warning: Found /* comment')

# Lint all files passed as arguments
for filename in sys.argv[1:]:
    lint(filename)