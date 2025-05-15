import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def plot_results():
    # Чтение данных из CSV
    df = pd.read_csv('C:/Study/CPP/algoriohm/lab9/hash_test_results.csv')
    
    # Вычисление статистик
    stats = df.groupby('differences')['max_common_length'].agg(['mean', 'max', 'min'])
    stats.reset_index(inplace=True)
    
    # Настройка графика
    plt.figure(figsize=(10, 6))
    
    # Основной график средних значений
    plt.plot(stats['differences'], stats['mean'], 
             marker='o', linestyle='-', linewidth=2, markersize=8,
             color='blue', label='Average')
    
    # График максимальных значений
    plt.plot(stats['differences'], stats['max'], 
             marker='^', linestyle='--', linewidth=1.5, markersize=8,
             color='red', label='Maximum')
    
    # График минимальных значений
    plt.plot(stats['differences'], stats['min'], 
             marker='v', linestyle='--', linewidth=1.5, markersize=8,
             color='green', label='Minimum')
    
    # Настройка осей и заголовка
    plt.xlabel('Количество отличающихся символов', fontsize=12)
    plt.ylabel('Максимальная общая длина подпоследовательности (в битах)', fontsize=12)
    plt.title('SipHash Avalanche Effect Test\n(1000 пар для каждого уровня разницы)', fontsize=14)
    
    # Настройка сетки и легенды
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend(fontsize=12)
    
    # Настройка делений на осях
    plt.xticks(stats['differences'], fontsize=10)
    plt.yticks(fontsize=10)
    
    # Сохранение и отображение
    plt.tight_layout()
    plt.show()

def plot_collision_results():
    # Чтение данных
    df = pd.read_csv('C:/Study/CPP/algoriohm/lab9/collision_test_results.csv')
    
    # Построение графика
    plt.figure(figsize=(10, 6))
    
    # Основной график
    plt.plot(df['N'], df['collisions_count'], 
             marker='o', linestyle='-', linewidth=2,
             color='blue', markersize=8)
    
    # Настройки графика
    plt.xscale('log')
    plt.xlabel('Количество сгенерированных хэшей (log scale)', fontsize=12)
    plt.ylabel('Количество обнаруженных коллизий', fontsize=12)
    plt.title('Hash Collision Test Results\nДля случайных строк из 256 символов', fontsize=14)
    plt.grid(True, which="both", ls="--", alpha=0.5)
    
    # Добавление значений на график
    for i, row in df.iterrows():
        plt.text(row['N'], row['collisions_count'], 
                 f"N=10^{i+2}\n{row['collisions_count']}",
                 ha='center', va='bottom', fontsize=10)
    
    plt.tight_layout()
    plt.show()

def plot_hash_speed():
    # Чтение данных
    df = pd.read_csv('C:/Study/CPP/algoriohm/lab9/hash_speed_results.csv')
    
    # Построение графика
    plt.figure(figsize=(12, 6))
    
    # Основной график
    plt.plot(df['length'], df['avg_time_ns'], 
             marker='o', linestyle='-', linewidth=2,
             color='purple', markersize=8, label='Average time')
    
    # Настройки графика
    plt.xscale('log', base=2)
    plt.yscale('log')
    plt.xlabel('Длина входной строки (bits, log scale)', fontsize=12)
    plt.ylabel('Время перемешивания (ns, log scale)', fontsize=12)
    plt.title('Hash Function Speed vs Input Size\n(1000 итераций на точку данных)', fontsize=14)
    plt.grid(True, which="both", ls="--", alpha=0.5)
    
    plt.legend(fontsize=12)
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    plot_results()
    plot_collision_results()
    plot_hash_speed()