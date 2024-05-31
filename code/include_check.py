import re
import sys

# extract comments after include statements
def get_import_comments(file_path):
  with open(file_path, 'r') as file:
    content = file.read()
  includes = re.findall(r'#include [<"].*[>"] \/\/ (.*)', content)
  includes = [word for comment in includes for word in comment.split(', ')]
  return includes

# extract C function definitions
def get_function_definitions(file_path):
  with open(file_path, 'r') as file:
    content = file.read()
  function_definitions = re.findall(r'\w+\s+(\w+\s*)\(.+\)\s*{', content)
  return function_definitions

# extract C function usage
def get_function_usage(file_path):
  with open(file_path, 'r') as file:
    content = file.read()
  function_usage = re.findall(r'(\w+\s*)\(.+\);', content)
  internal_function_usage = re.findall(r'[\( ]+(\w+)\([\w]+\).*\);', content)
  return function_usage + internal_function_usage

# extract C typedef struct and enum definition names
def get_type_definitions(file_path):
  with open(file_path, 'r') as file:
    content = file.read()
  struct_definitions = re.findall(r'typedef struct {(?:\s+.+\s)+} (\w+);', content)
  enum_definitions = re.findall(r'typedef enum {(?:\s+\w+.+)+} (\w+);', content)
  return struct_definitions + enum_definitions

# extract type usage such as variable declarations, function return types and function arguments
def get_type_usage(file_path):
  with open(file_path, 'r') as file:
    content = file.read()
  variable_declarations = re.findall(r'(\w+)\s+\*?\w+\s=\s', content)
  structs = re.findall(r'typedef struct {\n(?:.+\n)+} \w+;', content)
  struct_member_lines = []
  # loop through structs and split by newline and remove first and last element of each struct. Put the remaining lines flat in struct_lines
  for struct in structs:
    struct_member_lines += struct.split('\n')[1:-1]
  struct_member_types = []
  # Loop through the struct member lines and extract the type of each member and put in struct_member_types
  for struct_member_line in struct_member_lines:
    struct_member_types += re.findall(r'\s+(\w+)\s+\w+;', struct_member_line)
  function_return_types = re.findall(r'static\s+(\w+)\s+\w+\(.+\)', content)
  function_arguments = re.findall(r'\w+\s+\w+\((?:(\w+)\s+\w+\,*\s*)+\)', content)
  return variable_declarations + struct_member_types + function_return_types + function_arguments

def check_functions(file_path):
  import_comments = get_import_comments(file_path)
  print(f'  Included in comments: {import_comments}')

  builtin_functions = ["sizeof"]
  function_definitions = get_function_definitions(file_path)
  function_definitions = list(set(function_definitions))
  print(f'  Function definitions: {function_definitions}')

  function_usage = get_function_usage(file_path)
  function_usage = list(set(function_usage))
  print(f'    Function usage: {function_usage}')

  typedef_definitions = get_type_definitions(file_path)
  print(f'  Type definitions: {typedef_definitions}')

  builtin_types = ['float', 'double']
  type_usage = get_type_usage(file_path)
  type_usage = list(set(type_usage))
  print(f'    Type usage: {type_usage}')

  for function_name in function_usage:
    if function_name not in function_definitions and function_name not in import_comments and function_name not in builtin_functions:
      print(f'Warning: Function {function_name} is not defined or mentioned in #include comments')

  for type_name in type_usage:
    if type_name not in typedef_definitions and type_name not in import_comments and type_name not in builtin_types:
      print(f'Warning: Type {type_name} is not defined or mentioned in #include comments')

  # warn for unused import comments
  for comment in import_comments:
    if comment not in function_usage and comment not in type_usage:
      print(f'Warning: #include comment for {comment} is not used')

  # warn for unused typedefs
  for typedef in typedef_definitions:
    if typedef not in type_usage:
      print(f'Warning: Type {typedef} is not used')
  
  # warn for unused functions
  for function in function_definitions:
    if function not in function_usage:
      if function != 'main':
        print(f'Warning: Function "{function}" is not used')

# Check all files passed as arguments
for filename in sys.argv[1:]:
  print(f'Checking {filename}')
  check_functions(filename)