#Eli Goodwin
#CS344
#2015.07.29
#Simple progam that demonstrates simple file in and out in python. Generates a random string,
#saves it to disk in a file, opens the file and displays it contents, then two random numbers are generated
#and their product is calculated and displayed to screen.

import string 
import random
from random import randint
def stringGenerator(size=10, chars=string.ascii_lowercase):
		return ''.join(random.choice(chars) for _ in range(size))

fileNames = ['file1', 'file2', 'file3']

#make the files using the for loop
for name in fileNames:
#create file
	f = open(name, 'w+')
#create string
	aString = stringGenerator()
	aString +='\n'
#write string to file
	f.write("%s" % aString)
	f.close()
	
#read files in
for name in fileNames:
	f = open(name, 'r')
#print file contents
	print f.read()
	f.close()




#get random numbers
random1 = randint(1, 42)
random2 = randint(1, 42)

print("%d" % random1)
print("%d" % random2)
print("%d" % (random1 * random2))

