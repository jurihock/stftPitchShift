import os
import re

files = dict(
    cpp=os.path.join('cpp', 'StftPitchShift', 'Version.h'),
    python=os.path.join('python', 'stftpitchshift', '__init__.py')
)

with open('VERSION', 'r') as file:
    old = file.read().strip()

new = input(f'{old} => ')

old = ([int(x) for x in old.split('.')] + [0]*4)[:4]

if not new:
    new = old

elif '+' in new:
    old[len(new) - 1] += 1
    new = old

elif '-' in new:
    old[len(new) - 1] -= 1
    new = old

else:
    new = [int(x) for x in new.split('.') if x]

while len(new) > 2 and new[-1] < 1:
    new.pop()

new = '.'.join([str(x) for x in new])

with open('VERSION', 'w') as file:
    file.write(new)

with open(files['cpp'], 'r+') as file:
    code = file.read()
    code = re.sub(r'".*"', f'"{new}"', code)
    file.seek(0)
    file.write(code)
    file.truncate()

with open(files['python'], 'r+') as file:
    code = file.read()
    code = re.sub(r'".*"', f'"{new}"', code)
    file.seek(0)
    file.write(code)
    file.truncate()
