Import('env')
from os.path import join, realpath

import subprocess

bashCommand = "yarn release"
print(bashCommand)
process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE)
output, error = process.communicate()

print(output)
print(error)
