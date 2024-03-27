import sys

def to_code(line: str) -> str:
    (name, *args) = line.split()
    return f'\to.{name}({", ".join(args)});'

for line in sys.stdin.readlines():
    print(to_code(line.strip()))
