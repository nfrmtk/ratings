import re
matches = re.findall('\(#[0-9]+\)', input())
print(matches[0][2:-1] if len(matches) == 1 else 'bad-commit') # pr numbe

