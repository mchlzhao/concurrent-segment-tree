import math
import sys

RANGE = 4
DEBUG = False

array = [0]*RANGE

def update(a, b, c):
	for i in range(a, b):
		array[i] += c

def query(a, b):
	max_num = -math.inf
	for i in range(a, b):
		max_num = max(max_num, array[i])
	return max_num

done = 0

for line in sys.stdin:
	if DEBUG:
		print(line.strip(), end="        ")

	operation = line.split()
	args = list(map(int, operation[1:]))
	operation = line[0]

	if operation == 'q':
		ans = query(args[0], args[1])
		if ans != args[2]:
			print("WRONG ON LINE %d" % done)
			sys.exit(1)
	else:
		update(args[0], args[1], args[2])
		if DEBUG:
			for i in array:
				print(i, end=",")

	if DEBUG:
		print()

	done += 1

print("ALL CORRECT %d LINES" % done)
