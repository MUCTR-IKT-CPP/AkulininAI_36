import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

times = pd.read_csv('C:/Study/CPP/algoriohm/lab2/times.csv', header=None).values
func_calls = pd.read_csv('C:/Study/CPP/algoriohm/lab2/funcCalls.csv', header=None).values
in_func_calls = pd.read_csv('C:/Study/CPP/algoriohm/lab2/inFuncCalls.csv', header=None).values
sizes = pd.read_csv('C:/Study/CPP/algoriohm/lab2/sizes.csv', header=None).values.squeeze()
data = pd.read_csv('C:/Study/CPP/algoriohm/lab2/maxDepths.csv', header=None)


mean_func_calls = np.mean(func_calls, axis=1)
min_func_calls = np.min(func_calls, axis=1)
max_func_calls = np.max(func_calls, axis=1)

mean_in_func_calls = np.mean(in_func_calls, axis=1)
min_in_func_calls = np.min(in_func_calls, axis=1)
max_in_func_calls = np.max(in_func_calls, axis=1)

mean_depths = data.mean(axis=1)
min_depths = data.min(axis=1)
max_depths = data.max(axis=1)


def plot_worst_time_and_complexity():
    max_times = np.max(times, axis=1)
    c = 1e-4

    plt.figure(figsize=(10, 6))
    plt.plot(sizes, max_times, label='Наихудшее время выполнения')
    plt.plot(sizes,c * sizes * np.log2(sizes), label='O(N log N)')
    plt.xlabel('Размер массива')
    plt.ylabel('Время выполнения (милли сек)')
    plt.title('Наихудшее время выполнения и сложность алгоритма O(N log N)')
    plt.legend()
    plt.grid(True)
    plt.show()

def plot_mean_best_worst_time():
    mean_times = np.mean(times, axis=1)
    min_times = np.min(times, axis=1)
    max_times = np.max(times, axis=1)

    plt.figure(figsize=(10, 6))
    plt.plot(sizes, mean_times, label='Среднее время')
    plt.plot(sizes, min_times, label='Наилучшее время')
    plt.plot(sizes, max_times, label='Наихудшее время')
    plt.xlabel('Размер массива')
    plt.ylabel('Время выполнения (милли сек)')
    plt.title('Среднее, наилучшее и наихудшее время исполнения')
    plt.legend()
    plt.grid(True)
    plt.show()

def plot_mean_best_worst_recursion_depth():
    plt.figure(figsize=(10, 6))
    plt.plot(sizes, mean_depths, label='Средняя глубина рекурсии', marker='o', linestyle='-', color='blue')
    plt.plot(sizes, min_depths, label='Наилучшая глубина рекурсии', marker='s', linestyle='--', color='green')
    plt.plot(sizes, max_depths, label='Наихудшая глубина рекурсии', marker='^', linestyle='-.', color='red')

    plt.title('Глубина рекурсии в зависимости от размера массива')
    plt.xlabel('Размер массива')
    plt.ylabel('Глубина рекурсии')
    plt.legend()
    plt.grid(True) 
    plt.show()
    
# Функция для построения совмещенного графика среднего количества вызовов функции и внутренней функции
def plot_mean_func_calls_and_in_func_calls():
    plt.figure(figsize=(10, 6))
    plt.plot(sizes, mean_func_calls, label='Среднее количество вызовов функции')
    plt.plot(sizes, mean_in_func_calls, label='Среднее количество внутренних вызовов')
    plt.xlabel('Размер массива')
    plt.ylabel('Количество вызовов')
    plt.title('Среднее количество вызовов функции и внутренней функции')
    plt.legend()
    plt.grid(True)
    plt.show()

# Функция для построения графика среднего процентного соотношения внутренних вызовов к общему вызову функции
def plot_mean_in_func_calls_ratio():
    mean_in_func_calls_ratio = mean_in_func_calls / mean_func_calls * 100

    plt.figure(figsize=(10, 6))
    plt.plot(sizes, mean_in_func_calls_ratio, label='Процентное соотношение внутренних вызовов')
    plt.xlabel('Размер массива')
    plt.ylabel('Процентное соотношение внутренних вызовов (%)')
    plt.title('Среднее процентное соотношение внутренних вызовов к общему вызову функции')
    plt.legend()
    plt.grid(True)
    plt.show()

# Вызов функций для построения графиков
plot_worst_time_and_complexity()
plot_mean_best_worst_time()
plot_mean_best_worst_recursion_depth()
plot_mean_func_calls_and_in_func_calls()
plot_mean_in_func_calls_ratio()