#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <stack>
#include <vector>
#include <Windows.h>

using namespace std;

class Bullet {
private:
    string type;

public:
    /**
    * Конструктор класса Bullet.
    * 
    * @param type Тип патрона.
    */
    Bullet(const string& type) : type(type) {}

    /**
    * Возвращает звуковой эффект, связанный с типом патрона.
    *
    * @return Строка с описанием звукового эффекта.
    */
    string sound_effect() const {
        if (type == "Дозвуковые") {
            return "Тихий звук выстрела";
        }
        else if (type == "Обычные") {
            return "Стандартный звук выстрела";
        }
        else if (type == "Трассирующие") {
            return "Обычный звук выстрела с подсветкой";
        }
        return "Неизвестный звук";
    }

    /**
    * Перегрузка оператора вывода для класса Bullet.
    * 
    * @param os Поток вывода.
    * @param bullet Экземпляр Bullet.
    * @return Измененный поток вывода.
    */
    friend ostream& operator<<(ostream& os, const Bullet& bullet) {
        os << "Патрон { Тип: " << bullet.type << " }";
        return os;
    }

    /**
    * Функция для получения типа патрона.
    *
    * @return Возвращает тип патрона.
    */
    string get_type() const {
        return type;
    }
};

class Ammo {
private:
    stack<Bullet> bullets;
    const int capacity = 30;

public:
    /** 
    * Проверяет, заполнен ли магазин.
    * 
    * @return true, если магазин полон, иначе false.
    */
    bool is_full() const {
        return bullets.size() >= capacity;
    }

    /**
    * Проверяет, пуст ли магазин.
    * 
    * @return true, если магазин пуст, иначе false.
    */
    bool is_empty() const {
        return bullets.empty();
    }

    /**
    * Перезаряжает патроны в магазине.
    *
    * Загружает патроны из переданного вектора в магазин до тех пор,
    * пока он не заполнится.
    *
    * @param bullet_pool Вектор патронов для перезарядки магазина.
    * @return true, если патрон успешно загружен, иначе false.
    */
    bool reload(const vector<Bullet>& bullet_pool) {
        for (const auto& bullet : bullet_pool) {
            if (is_full()) {
                return false;
            }
            bullets.push(bullet);
        }
        return true;
    }

    /**
    * Производит выстрел из магазина и извлекает патрон.
    *
    * @param bullet Патрон, который будет извлечен из магазина.
    * @return true, если выстрел выполнен успешно, иначе false.
    */
    bool shoot(Bullet& bullet) {
        if (is_empty()) {
            cout << "Магазин пустой" << endl;
            return false;
        }
        bullet = bullets.top();
        bullets.pop();
        return true;
    }

    /**
    * Перегрузка оператора вывода для класса Ammo.
    * 
    * @param os Поток вывода.
    * @param ammo Экземпляр Ammo. 
    * @return Измененный поток вывода.
    */
    friend ostream& operator<<(ostream& os, const Ammo& ammo) {
        os << "Магазин { Патроны: " << ammo.bullets.size() << " }";
        return os;
    }

    /**
    * Функция для получения количества патронов в магазине.
    *
    * @return Возвращает количество патронов в магазине.
    */
    int get_count() const {
        return bullets.size();
    }

    /**
    * Очищает магазин от всех патронов.
    */
    void clear() {
        while (!bullets.empty()) {
            bullets.pop();
        }
    }

    /**
    * Добавляет патрон в магазин.
    *
    * @param bullet Патрон, который нужно добавить.
    */
    void add_bullet(const Bullet& bullet) {
        if (!is_full()) {
            bullets.push(bullet);
        }
    }
};

class Weapon {
private:
    unique_ptr<Ammo> ammo;
    int total_shots = 0;
    int total_reload_ammo = 0;
    int count_subsonic = 0;
    int count_conventional = 0;
    int count_tracer = 0;
    vector<Bullet> all_bullets;

public:
    /**
    * Генерирует случайные патроны в количестве count и добавляет их в вектор all_bullets.
    *
    * @param count Количество всех создаваемых патронов.
    */
    void generate_bullets(int count) {
        for (int i = 0; i < count; i++) {
            string type;
            switch (rand() % 3) {
            case 0: type = "Дозвуковые"; break;
            case 1: type = "Обычные"; break;
            case 2: type = "Трассирующие"; break;
            }
            all_bullets.emplace_back(type);
        }
    }

    /**
    * Загружает магазин патронам из вектора all_bullets, пока он не будет заполнен.
    *
    * Проверяет, создан ли магазин перед загрузкой.
    */
    void load_magazine() {
        if (!ammo) {
            cout << "------------- Новый магазин --------------" << endl;
            ammo = make_unique<Ammo>();
        }

        while (!all_bullets.empty() && ammo->get_count() < 30) {
            ammo->add_bullet(all_bullets.back());
            all_bullets.pop_back();
        }
        total_reload_ammo++;
    }

    /**
    * Происходит выстрел из оружия. В случае успешного выстрела,
    * увеличивается соответствующая статистика и производится звук патрона.
    */
    void shoot() {
        if (!ammo || ammo->is_empty()) {
            cout << "Нет доступных патронов для стрельбы!" << endl;
            return;
        }

        Bullet bullet("");
        if (ammo->shoot(bullet)) {
            Sleep(100);
            cout << bullet.sound_effect() << endl;
            total_shots++;
            if (bullet.get_type() == "Дозвуковые") {
                count_subsonic++;
            }
            else if (bullet.get_type() == "Обычные") {
                count_conventional++;
            }
            else if (bullet.get_type() == "Трассирующие") {
                count_tracer++;
            }

            if (ammo->is_empty() && get_remaining_bullets() > 0) {
                cout << "==================== Режим перезарядки ====================" << endl;
                Sleep(2000);
                ammo.reset();
            }
        }
    }

    /**
    * Проверяет, пуст ли магазин или отсутствует ли магазин.
    *
    * @return true, если магазин пуст, иначе false.
    */
    bool is_ammo_empty() const {
        return !ammo || ammo->is_empty();
    }

    /**
    * Вывод статистики после завершения стрельбы.
    */
    void print_statistics() const {
        cout << endl << string(50, '=') << endl;
        cout << "Суммарное кол-во выстреленных патронов: " << total_shots << endl;
        cout << "Суммарное кол-во перезаряженныых магазинов: " << total_reload_ammo << endl;
        cout << "Кол-во 'Дозвуковых' патронов: " << count_subsonic << endl;
        cout << "Кол-во 'Обычных' патронов: " << count_conventional << endl;
        cout << "Кол-во 'Трассирующих' патронов: " << count_tracer << endl;
    }

    /**
    * Перегрузка оператора вывода для класса Weapon.
    * 
    * @param os Поток вывода.
    * @param weapon Экземпляр Weapon.
    * @return Измененный поток вывода.
    */
    friend ostream& operator<<(ostream& os, const Weapon& weapon) {
        os << "Оружие { Магазин: " << (weapon.ammo ? *weapon.ammo : Ammo()) << " }";
        return os;
    }

    /**
    * Возвращает количество оставшихся патронов.
    *
    * @return Количество патронов, оставшихся в all_bullets.
    */
    int get_remaining_bullets() const {
        return all_bullets.size();
    }

    /**
    * Управляет магазином, загружая его, если он пуст.
    */
    void manage_magazine() {
        if (is_ammo_empty()) {
            load_magazine();
        }
    }
};

int main() {
    setlocale(LC_ALL, "");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    srand(static_cast<unsigned int>(time(0)));
    Weapon weapon;

    int total_bullets = rand() % 201 + 300;
    weapon.generate_bullets(total_bullets);
    weapon.load_magazine();

    int time_shot = 0, time_reload = 0;
    while (weapon.get_remaining_bullets() > 0 || !weapon.is_ammo_empty()) {
        while (!weapon.is_ammo_empty()) {
            weapon.shoot();
            weapon.manage_magazine();
            time_shot += 3;
            time_reload += 6;
        }
    }

    weapon.print_statistics();
    cout << "Суммарное время потраченное на зарядку патронов в магазин: " << time_reload << " сек\n";
    cout << "Суммарное время стрельбы: " << time_shot << " cек\n";
    return 0;
}