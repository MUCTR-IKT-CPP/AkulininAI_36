import pandas as pd
import matplotlib.pyplot as plt

# Загрузка данных
df = pd.read_csv('C:/Study/CPP/algoriohm/lab6/results.csv')

# Усреднение по сериям тестов
df_avg = df.groupby(['Test Series', 'Data Size', 'Data Type']).mean().reset_index()

# Разделение на случайные и отсортированные данные
df_random = df_avg[df_avg['Data Type'] == 'Random']
df_sorted = df_avg[df_avg['Data Type'] == 'Sorted']

# Создание фигуры с 4 подграфиками
plt.figure(figsize=(16, 12))

# График 1: Время вставки
plt.subplot(2, 2, 1)
plt.plot(df_random['Data Size'], df_random['BST Insert Time'], 'b-o', label='BST (Random)')
plt.plot(df_random['Data Size'], df_random['AVL Insert Time'], 'g-o', label='AVL (Random)')
plt.plot(df_sorted['Data Size'], df_sorted['BST Insert Time'], 'b--s', label='BST (Sorted)')
plt.plot(df_sorted['Data Size'], df_sorted['AVL Insert Time'], 'g--s', label='AVL (Sorted)')
plt.xscale('log')
plt.yscale('log')
plt.xlabel('Размер данных (лог. шкала)')
plt.ylabel('Время (сек)')
plt.title('Сравнение времени вставки')
plt.grid(True, which="both", ls="--")
plt.legend()

# График 2: Время поиска
plt.subplot(2, 2, 2)
plt.plot(df_random['Data Size'], df_random['BST Search Time'], 'b-o', label='BST (Random)')
plt.plot(df_random['Data Size'], df_random['AVL Search Time'], 'g-o', label='AVL (Random)')
plt.plot(df_random['Data Size'], df_random['Array Search Time'], 'r-o', label='Array (Random)')
plt.plot(df_sorted['Data Size'], df_sorted['BST Search Time'], 'b--s', label='BST (Sorted)')
plt.plot(df_sorted['Data Size'], df_sorted['AVL Search Time'], 'g--s', label='AVL (Sorted)')
plt.plot(df_sorted['Data Size'], df_sorted['Array Search Time'], 'r--s', label='Array (Sorted)')
plt.xscale('log')
plt.yscale('log')
plt.xlabel('Размер данных (лог. шкала)')
plt.ylabel('Время (сек)')
plt.title('Сравнение времени поиска')
plt.grid(True, which="both", ls="--")
plt.legend()

# График 3: Время удаления
plt.subplot(2, 2, 3)
plt.plot(df_random['Data Size'], df_random['BST Remove Time'], 'b-o', label='BST (Random)')
plt.plot(df_random['Data Size'], df_random['AVL Remove Time'], 'g-o', label='AVL (Random)')
plt.plot(df_sorted['Data Size'], df_sorted['BST Remove Time'], 'b--s', label='BST (Sorted)')
plt.plot(df_sorted['Data Size'], df_sorted['AVL Remove Time'], 'g--s', label='AVL (Sorted)')
plt.xscale('log')
plt.yscale('log')
plt.xlabel('Размер данных (лог. шкала)')
plt.ylabel('Время (сек)')
plt.title('Сравнение времени удаления')
plt.grid(True, which="both", ls="--")
plt.legend()

# График 4: Все операции AVL (случайные данные)
plt.subplot(2, 2, 4)
plt.plot(df_random['Data Size'], df_random['AVL Insert Time'], 'b-o', label='Вставка')
plt.plot(df_random['Data Size'], df_random['AVL Search Time'], 'g-o', label='Поиск')
plt.plot(df_random['Data Size'], df_random['AVL Remove Time'], 'r-o', label='Удаление')
plt.xscale('log')
plt.yscale('log')
plt.xlabel('Размер данных (лог. шкала)')
plt.ylabel('Время (сек)')
plt.title('Операции AVL (случайные данные)')
plt.grid(True, which="both", ls="--")
plt.legend()

plt.tight_layout()
plt.show()