import pandas as pd
import matplotlib.pyplot as plt
import numpy as np


def plot_comparison(random_file, sorted_file, operation):
    plt.figure(figsize=(14, 8))
    
    # Load and plot random data
    random_data = pd.read_csv(random_file)
    for tree_type in ['AVL', 'Treap', 'RBT']:
        subset = random_data[random_data['Tree Type'] == tree_type]
        plt.plot(subset['Data Size'], subset[operation], 
                label=f'{tree_type} (Random)', marker='o', linestyle='-')
    
    # Load and plot sorted data
    sorted_data = pd.read_csv(sorted_file)
    for tree_type in ['AVL', 'Treap', 'RBT']:
        subset = sorted_data[sorted_data['Tree Type'] == tree_type]
        plt.plot(subset['Data Size'], subset[operation], 
                label=f'{tree_type} (Sorted)', marker='x', linestyle='--')
    
    plt.xscale('log')
    
    # Для временных графиков используем логарифмическую шкалу только если нет нулевых значений
    if "Time" in operation:
        min_value = min(random_data[operation].min(), sorted_data[operation].min())
        if min_value > 0:
            plt.yscale('log')
        plt.ylabel('Time (microseconds)')
    
    plt.xlabel('Data Size (log scale)')
    plt.title(f'{operation} Comparison')
    plt.legend()
    plt.grid(True, which="both", ls="--")
    
    filename = operation.lower().replace(' ', '_').replace('(', '').replace(')', '') + '_comparison.png'
    plt.savefig(filename)
    plt.close()
    print(f"Saved {filename}")


# Generate all comparison plots
for op in ['Insert Time (us)', 'Search Time (us)', 'Remove Time (us)']:
    try:
        plot_comparison('C:/Study/CPP/algoriohm/lab7/results_random.csv', 'C:/Study/CPP/algoriohm/lab7/results_sorted.csv', op)
    except Exception as e:
        print(f"Error plotting {op}: {str(e)}")


# Чтение данных
heights = pd.read_csv('C:/Study/CPP/algoriohm/lab7/tree_heights.csv')

# Создание отдельных графиков для случайных и отсортированных данных
plt.figure(figsize=(14, 10))

# График для случайных данных
plt.subplot(2, 1, 1)
random_data = heights[heights['Tree Type'].str.contains('Random')]
for tree_type in random_data['Tree Type'].unique():
    subset = random_data[random_data['Tree Type'] == tree_type]
    plt.plot(subset['Data Size'], subset['Max Height'], 'o-', label=tree_type.replace(" Random", ""))

plt.xscale('log', base=2)
plt.xlabel('Data Size (N)')
plt.ylabel('Max Tree Height')
plt.title('Maximum Tree Heights for Random Data')
plt.legend()
plt.grid(True)

# График для отсортированных данных
plt.subplot(2, 1, 2)
sorted_data = heights[heights['Tree Type'].str.contains('Sorted')]
for tree_type in sorted_data['Tree Type'].unique():
    subset = sorted_data[sorted_data['Tree Type'] == tree_type]
    plt.plot(subset['Data Size'], subset['Max Height'], 'o-', label=tree_type.replace(" Sorted", ""))

plt.xscale('log', base=2)
plt.xlabel('Data Size (N)')
plt.ylabel('Max Tree Height')
plt.title('Maximum Tree Heights for Sorted Data')
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.savefig('tree_heights_comparison.png')
plt.show()