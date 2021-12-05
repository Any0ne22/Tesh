from os import remove
from ptyprocess import PtyProcessUnicode
import sys
import re
import time

p = PtyProcessUnicode.spawn([sys.argv[1]])

time.sleep(0.2)
command = ""
for x in sys.argv[2:]:
    command += x + "\n";
p.write(command)

res = ""
x = p.read(1)
while x != None:
    res += x
    try:
        x = p.read(1)
        p.sendeof()
    except:
        res += x
        break

ansi_escape = re.compile(r'\x1B(?:[@-Z\\-_]|\[[0-?]*[-/]*[@-~])')
res = ansi_escape.sub('', res)
res = ''.join([x for x in res if x != "\r"])
print(res[:-1])