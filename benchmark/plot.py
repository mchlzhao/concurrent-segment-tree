import matplotlib.pyplot as plt
import pandas as pd

def plot_compare(x, y_global, y_smart, log, x_label):
	plt.figure()

	plt.plot(x, y_global, label='global-lock', linestyle='--', marker='o')
	plt.plot(x, y_smart, label='smart-lock', linestyle='--', marker='x')

	plt.xlabel(x_label)
	plt.ylabel('Time (s)')

	if y_smart[0] > 1:
		plt.ylim(bottom=0)

	if log:
		plt.xscale('log')
	else:
		plt.xscale('linear')

	plt.legend()
	plt.show()

def plot_threads(df):
	x = df['num_threads'].to_numpy()
	y_global = df['global-lock'].to_numpy()
	y_smart = df['smart-lock'].to_numpy()
	plot_compare(x, y_global, y_smart, False, 'Number of threads')

def plot_ranges(df):
	x = df['ranges'].to_numpy()
	y_global = df['global-lock.1'].to_numpy()
	y_smart = df['smart-lock.1'].to_numpy()
	plot_compare(x, y_global, y_smart, True, 'Range that seg-tree covers (log scale)')

df = pd.read_csv('data.csv')

print(df)

plot_threads(df)
plot_ranges(df)
