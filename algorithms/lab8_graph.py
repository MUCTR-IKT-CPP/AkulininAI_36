import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def plot_heap_performance(csv_file):
    # Загрузка данных
    df = pd.read_csv(csv_file)
    
    # Настройка стиля графиков (используем доступный стиль)
    plt.style.use('ggplot')  # Изменено с 'seaborn' на доступный стиль
    plt.rcParams['figure.figsize'] = (15, 10)
    plt.rcParams['font.size'] = 12
    
    # Создаем сетку графиков
    fig, axs = plt.subplots(2, 3, figsize=(20, 12))
    fig.suptitle('Производительность операций кучи (усредненные и максимальные значения)', y=1.02, fontsize=16)
    
    # Размеры кучи для оси X (логарифмическая шкала)
    sizes = df['Size'].unique()
    x_labels = [f'10^{int(np.log10(s))}' for s in sizes]
    
    # Цвета для разных типов куч
    colors = {'BinaryHeap': '#1f77b4', 'BinomialHeap': '#ff7f0e'}
    
    # Операции для анализа
    operations = ['Find', 'Extract', 'Insert']
    
    # Построение графиков для каждой операции
    for i, op in enumerate(operations):
        # График среднего времени
        ax_avg = axs[0, i]
        for heap_type in df['HeapType'].unique():
            data = df[df['HeapType'] == heap_type]
            ax_avg.plot(data['Size'], data[f'{op}Avg(us)'], 
                       'o-', color=colors[heap_type], 
                       label=heap_type, markersize=8, linewidth=2)
        
        ax_avg.set_title(f'Среднее время {op} (µs)')
        ax_avg.set_xticks(sizes)
        ax_avg.set_xticklabels(x_labels)
        ax_avg.grid(True, which="both", linestyle="--", alpha=0.5)
        ax_avg.legend()
        
        # График максимального времени
        ax_max = axs[1, i]
        for heap_type in df['HeapType'].unique():
            data = df[df['HeapType'] == heap_type]
            ax_max.plot(data['Size'], data[f'{op}Max(us)'], 
                       'o-', color=colors[heap_type], 
                       label=heap_type, markersize=8, linewidth=2)
        
        ax_max.set_title(f'Максимальное время {op} (µs)')
        ax_max.set_xticks(sizes)
        ax_max.set_xticklabels(x_labels)
        ax_max.grid(True, which="both", linestyle="--", alpha=0.5)
        ax_max.legend()
    
    # График времени заполнения
    plt.figure(figsize=(12, 6))
    for heap_type in df['HeapType'].unique():
        data = df[df['HeapType'] == heap_type]
        plt.plot(data['Size'], data['FillTime(us)'], 
                'o-', color=colors[heap_type], 
                label=heap_type, markersize=8, linewidth=2)
    
    plt.title('Время заполнения кучи', fontsize=14)
    plt.xlabel('Размер кучи', fontsize=12)
    plt.ylabel('Время (µs)', fontsize=12)
    plt.xticks(sizes, x_labels)
    plt.grid(True, which="both", linestyle="--", alpha=0.5)
    plt.legend(fontsize=12)
    plt.tight_layout()
    plt.savefig('heap_fill_time.png', dpi=300, bbox_inches='tight')
    
    # Сохранение и отображение графиков операций
    plt.tight_layout()
    plt.savefig('heap_operations_performance.png', dpi=300, bbox_inches='tight')
    plt.show()

if __name__ == "__main__":
    # Убедитесь, что путь к файлу указан правильно
    plot_heap_performance("C:/Study/CPP/algoriohm/lab8/heap_performance.csv")