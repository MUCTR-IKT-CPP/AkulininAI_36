import matplotlib.pyplot as plt
import pandas as pd

data = pd.read_csv('C:/Study/CPP/algoriohm/lab5/results.csv')

plt.figure(figsize=(10, 6))
plt.plot(data['Vertices'], data['AverageTime(microseconds)'], marker='o', linestyle='-', color='b')

plt.xlabel("Number of Vertices (N)")
plt.ylabel("Average Time (microseconds)")
plt.title("Prim's Algorithm Performance")
plt.grid(True)

plt.show()