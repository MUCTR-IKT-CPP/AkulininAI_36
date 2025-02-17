import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

def load_data_from_csv(filename):
    df = pd.read_csv(filename, header=None)
    return df.values

def calculate_big_o_n_squared(sizes, c=1e-7):
    return c * np.array(sizes)**2

def plot_worst_case_and_complexity(times, sizes):
    worst_times = np.max(times, axis=1)
    big_o_values = calculate_big_o_n_squared(sizes)
    
    fig, ax = plt.subplots()
    ax.plot(sizes, worst_times, label='Наихудшее время')
    ax.plot(sizes, big_o_values, label='O(n^2)')
    ax.set_xlabel('Размер массива')
    ax.set_ylabel('Время (сек.)')
    ax.legend()
    plt.title('Совмещенный график наихудшего времени выполнения сортировки и сложности алгоритма')
    plt.show()

def plot_average_best_worst_times(times, sizes):
    average_times = np.mean(times, axis=1)
    best_times = np.min(times, axis=1)
    worst_times = np.max(times, axis=1)
    
    fig, ax = plt.subplots()
    ax.plot(sizes, average_times, label='Среднее время')
    ax.plot(sizes, best_times, label='Лучшее время')
    ax.plot(sizes, worst_times, label='Наихудшее время')
    ax.set_xlabel('Размер массива')
    ax.set_ylabel('Время (сек.)')
    ax.legend()
    plt.title('Графики среднего, наилучшего и наихудшего времени исполнения')
    plt.show()

def plot_average_exchanges(exchanges, sizes):
    average_exchanges = np.mean(exchanges, axis=1)
    
    fig, ax = plt.subplots()
    ax.plot(sizes, average_exchanges)
    ax.set_xlabel('Размер массива')
    ax.set_ylabel('Количество обменов')
    plt.title('Среднее количество обменов значений')
    plt.show()

def plot_repeat_passes(repeats, sizes):
    average_repeats = np.mean(repeats, axis=1)
    
    fig, ax = plt.subplots()
    ax.plot(sizes, average_repeats)
    ax.set_xlabel('Размер массива')
    ax.set_ylabel('Повторные проходы')
    plt.title('Повторные обходы массива')
    plt.show()

if __name__ == "__main__":
    times = load_data_from_csv('times.csv')
    exchanges = load_data_from_csv('exchanges.csv')
    repeats = load_data_from_csv('repeats.csv')
    sizes = load_data_from_csv('sizes.csv').flatten()

    plot_worst_case_and_complexity(times, sizes)
    plot_average_best_worst_times(times, sizes)
    plot_average_exchanges(exchanges, sizes)
    plot_repeat_passes(repeats, sizes)