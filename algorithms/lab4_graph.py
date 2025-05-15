import pandas as pd
import matplotlib.pyplot as plt


data = pd.read_csv('C:/Study/CPP/algoriohm/lab4/results.csv')
data = data.sort_values(by='Vertices')

plt.figure(figsize=(10, 6))
plt.plot(data['Vertices'], data['BFS Time (micro s)'], label='BFS Time (micro s)')
plt.plot(data['Vertices'], data['DFS Time (micro s)'], label='DFS Time (micro s)')
plt.xlabel('Vertices')
plt.ylabel('Time (micro s)')
plt.title('BFS vs DFS Time Complexity')
plt.legend()
plt.grid(True)
plt.show()