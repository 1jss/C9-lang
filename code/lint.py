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

            # Rule: 'goto' is not allowed except for error handling inside functions
            # This rule is also difficult to enforce with a regex as it requires context-sensitive parsing.
            # We will skip this rule for now.

            # Rule: 'const' is not allowed in local scope (inside functions)
            # This rule is also difficult to enforce with a regex as it requires context-sensitive parsing.
            # We will skip this rule for now.

            # Rule: No built in integer types are allowed as they can vary in size. Use `inttypes.h` instead.
            if re.search(r'\b(int|short|long)\b', line):
                print(f'{filename}:{line_num}: Warning: Found built-in integer type')

            # Rule: Implicit ints are not used nor allowed.
            # This rule is difficult to enforce with a regex as it requires context-sensitive parsing.
            # We will skip this rule for now.

            # Rule: The long and short keywords are not allowed as they can vary in size between systems.
            if re.search(r'\b(long|short)\b', line):
                print(f'{filename}:{line_num}: Warning: Found long or short keyword')

            # Rule: All variables and pointers must be initiated with a value.
            if re.search(r'(\bint32_t\b|\bint32_t\s*\*).*(;|$)', line):
                print(f'{filename}:{line_num}: Warning: Found variable or pointer without initial value')

            # Rule: Prefer using long variable names and avoid abbreviations
            # This rule is subjective and depends on the specific abbreviations used in your codebase.
            # We will skip this rule for now.

            # Rule: Tentative definitions are not allowed
            # This rule is difficult to enforce with a regex as it requires context-sensitive parsing.
            # We will skip this rule for now.

            # Rule: Names of structs, unions and enums must be unique
            # This rule is difficult to enforce with a regex as it requires context-sensitive parsing.
            # We will skip this rule for now.

            # Rule: Use `const` for constants in global scope
            # This rule is difficult to enforce with a regex as it requires context-sensitive parsing.
            # We will skip this rule for now.

            # Rule: Never use `const` in local scope (inside functions) as it lifts the variable to global scope.
            # This rule is difficult to enforce with a regex as it requires context-sensitive parsing.
            # We will skip this rule for now.

            # Rule: All structs are typedefs
            if re.search(r'struct\s+\w+\s*\{', line):
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
            if re.search(r'^\s*#\s*(define|undef|ifdef|ifndef|else|elif|error|pragma)', line):
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