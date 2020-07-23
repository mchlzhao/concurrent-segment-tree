import math
import sys

RANGE = 100
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
has_a_wrong = False
is_wrong = False

for line in sys.stdin:
	if DEBUG:
		print(line.strip(), end="        ")

	operation = line.split()
	args = list(map(int, operation[1:]))
	operation = line[0]

	if operation == 'q':
		ans = query(args[0], args[1])
		if ans != args[2]:
			if not has_a_wrong:
				print("FIRST WRONG ON LINE %d" % done)
			has_a_wrong = True
			is_wrong = True
		else:
			is_wrong = False
	else:
		update(args[0], args[1], args[2])
		if DEBUG:
			for i in array:
				print(i, end=",")

	if DEBUG:
		print()

	done += 1

if is_wrong:
	print("LAST ONE IS WRONG\n")
else:
	print("LAST ONE IS RIGHT\n")

if not has_a_wrong:
	print("ALL CORRECT %d LINES" % done)
