#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <Windows.h>

using namespace std;

/**
 * Генерирует случайное целое число в заданном диапазоне.
 *
 * @param min Минимальное значение диапазона.
 * @param max Максимальное значение диапазона.
 * @return Случайное целое число от min до max.
 */
int generate_random_int(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Родительский класс 'Fighter' (Боец) с общими свойствами.
class Fighter {
protected:
    string name;                    // Имя бойца
    int health;                     // Здоровье (100)
    int armor;                      // Броня (1-20)
    int bonus_damage;               // Бонусный урон (1-8) 
    int initiative_bonus;           // Бонус инициативы (1-20)
    int range;                      // Дальность хода (10, 5, 1)
    vector<int> position;           // Позиция ([0, 0]; [N-1, N-1])
    vector<string> action_history;  // История статистических данных

public:
    Fighter(string name, int health, int armor, int bonus_damage, int initiative_bonus,
        int range, vector<int> position)
        : name(name), health(health), armor(armor), bonus_damage(bonus_damage),
        initiative_bonus(initiative_bonus), range(range), position(position) {}

    /**
     * Виртуальная функция для атаки.
     *
     * @param target Указывает на свойсва цели.
     */
    virtual void attack(Fighter& target, vector<Fighter*>& team1, vector<Fighter*>& team2) = 0;
    
    /**
    * Виртуальная функция для движения.
    *
    * @param target_position Указывает на позицию цели.
    */
    virtual void move(vector<int>& target_position) = 0;

    // Функции, возвращающие свойства боца 
    int get_health() const { return health; }
    int get_armor() const { return armor; }
    int get_initiative() const { return initiative_bonus; }
    int get_range() const { return range; }
    const string& get_name() const { return name; }
    const vector<int>& get_position() const { return position; }
    const vector<string>& get_action_history() const { return action_history; }

    /**
    * Функция для рассчета урона.
    *
    * @param damage Наносимый урон.
    */
    void take_damage(int damage) {
        if (damage < armor) {
            armor -= damage;
        }
        else {
            damage -= armor;
            armor = 0;
            health -= damage;
        }
        if (health < 0) health = 0;
    }

    /**
    * Функция для добавления статисктики в вектор.
    *
    * @param action Строка со статистикой.
    */
    void log_action(const string& action) {
        action_history.push_back(action);
    }
    
    /**
    * Перегруженный оператор вывода для отображения информации о бойце.
    *
    * @param os Поток вывода.
    * @param fighter Объект бойца.
    * @return Поток вывода с информацией о бойце.
    */
    friend ostream& operator<<(ostream& os, const Fighter& fighter) {
        os << "Имя бойца: " << fighter.name << endl;
        os << "\t-Здоровье: " << fighter.health << endl;
        os << "\t-Броня: " << fighter.armor << endl;
        os << "\t-Бонусный урон: " << fighter.bonus_damage << endl;
        os << "\t-Бонус инициативы: " << fighter.initiative_bonus << endl;
        os << "\t-Дальность хода: " << fighter.range << endl;
        os << "\t-Позиция: " << fighter.position.front() << ", " << fighter.position.back() << endl;
        return os;
    }
};

// Класс 'Archer' (Лучник) наследует от класса Fighter.
class Archer : public Fighter {
public:
    Archer(vector<int> position) : Fighter("Лучник", 100, generate_random_int(1, 20), generate_random_int(1, 8),
        generate_random_int(1, 20), 10, position) {}

    /**
    * Функция для атаки лучником.
    *
    * @param target Ссылка на свойства цели.
    */
    void attack(Fighter& target, vector<Fighter*>& team1, vector<Fighter*>& team2) override {
        if (abs(position[0] - target.get_position()[0]) + abs(position[1] - target.get_position()[1]) <= range) {
            int damage = generate_random_int(1, 20) + bonus_damage;
            target.take_damage(damage);
            log_action("атакует " + target.get_name() + " на расстоянии: " + to_string(damage) + " урона!");
        }
        else {
            log_action("не может атаковать " + target.get_name() + ", он слишком далеко!");
        }
    }

    /**
    * Функция для движения лучника.
    *
    * @param target_position Ссылка на позицию цели.
    */
    void move(vector<int>& target_position) override {
        position = target_position;
        log_action("перемещается на позицию: (" + to_string(position[0]) + ", " + to_string(position[1]) + ")");
    }
};

// Класс 'Mage' (Маг) наследует от класса Fighter.
class Mage : public Fighter {
public:
    Mage(vector<int> position) : Fighter("Маг", 100, generate_random_int(1, 20), generate_random_int(1, 8),
        generate_random_int(1, 20), 5, position) {}
    
    /**
    * Функция для атаки магом.
    *
    * @param target Ссылка на свойства цели.
    */
    void attack(Fighter& target, vector<Fighter*>& team1, vector<Fighter*>& team2) override {
        if (abs(position[0] - target.get_position()[0]) + abs(position[1] - target.get_position()[1]) <= range) {
            int damage = generate_random_int(1, 20) + bonus_damage;
            target.take_damage(damage);
            log_action("использует магию на " + target.get_name() + ", нанося " + to_string(damage) + " урона!");

            for (auto& fighter : (target.get_name() == "Лучник" || target.get_name() == "Воин" ? team2 : team1)) {
                if (fighter->get_health() > 0 && fighter != &target) {
                    if (abs(fighter->get_position()[0] - target.get_position()[0]) <= 1 &&
                        abs(fighter->get_position()[1] - target.get_position()[1]) <= 1) {
                        fighter->take_damage(damage);
                        log_action("наносит " + to_string(damage) + " урона " + fighter->get_name() + " в радиусе действия!");
                    }
                }
            }
        }
        else {
            log_action("не может атаковать " + target.get_name() + ", он слишком далеко!");
        }
    }

    /**
    * Функция для движения мага.
    *
    * @param target_position Ссылка на позицию цели.
    */
    void move(vector<int>& target_position) override {
        position = target_position;
        log_action("перемещается на позицию: (" + to_string(position[0]) + ", " + to_string(position[1]) + ")");
    }
};

// Класс 'Wars' (Воин) наследует от класса Fighter.
class Wars : public Fighter {
public:
    Wars(vector<int> position) : Fighter("Воин", 100, generate_random_int(10, 20), generate_random_int(1, 8),
        generate_random_int(1, 20), 1, position) {}

    /**
    * Функция для атаки воином.
    *
    * @param target Ссылка на свойства цели.
    */
    void attack(Fighter& target, vector<Fighter*>& team1, vector<Fighter*>& team2) override {
        if (abs(position[0] - target.get_position()[0]) + abs(position[1] - target.get_position()[1]) <= range) {
            int damage = generate_random_int(1, 20) + bonus_damage;
            target.take_damage(damage);
            log_action("атакует " + target.get_name() + ", нанося " + to_string(damage) + " урона!");
        }
        else {
            log_action("должен подойти ближе к " + target.get_name() + "!");
        }
    }

    /**
    * Функция для движения воина.
    *
    * @param target_position Ссылка на позицию цели.
    */
    void move(vector<int>& target_position) override {
        position = target_position;
        log_action("перемещается на позицию: (" + to_string(position[0]) + ", " + to_string(position[1]) + ")");
    }
};

// Класс Battle для моделирования боя
class Battle {
private:
    vector<Fighter*> team1;     // Бойцы первой команды
    vector<Fighter*> team2;     // Бойцы второй команды
    vector<Fighter*> fighters;  // Все бойцы (живые и мертвые)
    int field_size;             // Размер поля

public:
    Battle(int size) : field_size(size) {}

    /**
    * Функция для добавление бойцов первой команды в вектор первых бойцов и в вектор всех бойцов.
    *
    * @param fighter Указатель на бойца.
    */
    void add_fighter_to_team1(Fighter* fighter) {
        team1.push_back(fighter);
        fighters.push_back(fighter);
    }

    /**
    * Функция для добавление бойцов второй команды в вектор первых бойцов и в вектор всех бойцов.
    *
    * @param fighter Указатель на бойца.
    */
    void add_fighter_to_team2(Fighter* fighter) {
        team2.push_back(fighter);
        fighters.push_back(fighter);
    }

    /**
    * Функция вывода статиски команд во время боя.
    */
    void print_team_statistics() {
        cout << "Команда 1:" << endl;
        for (const auto& fighter : team1) {
            cout << *fighter;
            cout << endl;
        }

        cout << "Команда 2:" << endl;
        for (const auto& fighter : team2) {
            cout << *fighter;
            cout << endl;
        }
    }

    /**
    * Функция вывода статиски команд после боя.
    */
    void print_statistics() {
        cout << "Статистика бойцов:" << endl;
        for (auto& fighter : fighters) {
            cout << fighter->get_name() << ":\n";
            cout << "\tЗдоровье: " << fighter->get_health() << " (" << (fighter->get_health() > 0 ? "жив" : "мертв") << ")" << endl;
            cout << "\tБроня: " << fighter->get_armor() << endl;
            cout << "\tДействия: " << endl;
            for (const auto& action : fighter->get_action_history()) {
                cout << "\t\t" << action << endl;
            }
            cout << endl;
        }
    }

    /**
    * Функция для моделирования боя.
    */
    void combat() {
        int round = 1;
        while (!team1.empty() && !team2.empty()) {
            cout << "----Раунд " << round << "----" << endl;

            // Сортировка бойцов по инициативе
            sort(team1.begin(), team1.end(), [](Fighter* a, Fighter* b) {
                return a->get_initiative() > b->get_initiative();
                });
            sort(team2.begin(), team2.end(), [](Fighter* a, Fighter* b) {
                return a->get_initiative() > b->get_initiative();
                });

            int team1_index = 0, team2_index = 0;

            while (team1_index < team1.size() && team2_index < team2.size()) {
                Fighter* fighter1 = team1[team1_index];
                Fighter* fighter2 = team2[team2_index];

                // Проверяем, жив ли боец команды 1 и боец команды 2
                if (fighter1->get_health() > 0 && fighter2->get_health() > 0) {
                    cout << "\n\t+(команда 1) " << fighter1->get_name();
                    if (abs(fighter1->get_position()[0] - fighter2->get_position()[0]) +
                        abs(fighter1->get_position()[1] - fighter2->get_position()[1]) <= fighter1->get_range()) {
                        cout << " атакует " << fighter1->get_name() << endl;
                        fighter1->attack(*fighter2, team1, team2);
                    }
                    else {
                        cout << " перемещается к " << fighter2->get_name() << endl;
                        move_towards(fighter1, fighter2);
                    }
                }
                team1_index++;

                // Проверяем, жив ли боец команды 2 и боец команды 1
                if (fighter2->get_health() > 0 && fighter1->get_health() > 0) {
                    cout << "\n\t-(команда 2) " << fighter2->get_name();
                    if (abs(fighter2->get_position()[0] - fighter1->get_position()[0]) +
                        abs(fighter2->get_position()[1] - fighter1->get_position()[1]) <= fighter2->get_range()) {
                        cout << " атакует " << fighter2->get_name() << endl;
                        fighter2->attack(*fighter1, team2, team1);
                    }
                    else {
                        cout << " перемещается к " << fighter1->get_name() << endl;
                        move_towards(fighter2, fighter1);
                    }
                }
                team2_index++;
            }

            // Удаление мертвых бойцов
            team1.erase(remove_if(team1.begin(), team1.end(), [](Fighter* fighter) {
                return fighter->get_health() <= 0;
                }), team1.end());

            team2.erase(remove_if(team2.begin(), team2.end(), [](Fighter* fighter) {
                return fighter->get_health() <= 0;
                }), team2.end());

            cout << endl;
            round++;
        }

        if (team1.empty()) {
            cout << "Команда 2 выиграла!" << endl;
        }
        else {
            cout << "Команда 1 выиграла!" << endl;
        }
    }

    /**
   * Функция для движения и определения новой позии бойцов.
   *
   * @param fighter Указатель на бойца.
   * @param target Указатель на цель.
   */
    void move_towards(Fighter* fighter, Fighter* target) {
        int fx = fighter->get_position()[0];
        int fy = fighter->get_position()[1];
        int tx = target->get_position()[0];
        int ty = target->get_position()[1];

        vector<int> new_position = { fx, fy };
        int dx = 0, dy = 0;

        if (fx < tx && abs(fx - (fx + 1)) <= fighter->get_range()) { dx = 1; }
        else if (fx > tx && abs(fx - (fx - 1)) <= fighter->get_range()) { dx = -1; }

        if (fy < ty && abs(fy - (fy + 1)) <= fighter->get_range()) { dy = 1; }
        else if (fy > ty && abs(fy - (fy - 1)) <= fighter->get_range()) { dy = -1; }

        // Ограничение передвижения по дальности хода
        if (abs(dx) + abs(dy) > fighter->get_range()) {
            float ratio = static_cast<float>(fighter->get_range()) / (abs(dx) + abs(dy));
            dx = static_cast<int>(round(dx * ratio));
            dy = static_cast<int>(round(dy * ratio));
        }

        new_position[0] += dx;
        new_position[1] += dy;

        fighter->move(new_position);
    }
};

int main() {
    setlocale(LC_ALL, "");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    srand(static_cast<unsigned int>(time(0)));

    int N, M;
    cout << "Введите размер квадратного поля: ";
    cin >> N;
    cout << "Введите общее количество бойцов в каждой команде (M): ";
    cin >> M;
    cout << string(52, '=') << endl;

    Battle battle(N);

    int melee_count = M * 50 / 100;     // 50% ближний бой
    int archer_count = M * 30 / 100;    // 30% дальний бой
    int mage_count = M * 20 / 100;      // 20% маги

    // Создание группы 1 (координаты 0, 0)
    for (int i = 0; i < melee_count; i++)
        battle.add_fighter_to_team1(new Wars(vector<int>{0, i}));

    for (int i = 0; i < archer_count; i++)
        battle.add_fighter_to_team1(new Archer(vector<int>{0, melee_count + i}));

    for (int i = 0; i < mage_count; i++)
        battle.add_fighter_to_team1(new Mage(vector<int>{0, melee_count + archer_count + i}));

    int total_team1 = melee_count + archer_count + mage_count;
    if (total_team1 < M) {
        int remaining = M - total_team1;
        for (int i = 0; i < remaining; i++) {
            battle.add_fighter_to_team1(new Wars(vector<int>{0, melee_count + archer_count + mage_count + i}));
        }
    }

    // Создание группы 2 (координаты N-1, N-1)
    for (int i = 0; i < melee_count; i++)
        battle.add_fighter_to_team2(new Wars(vector<int>{N - 1, N - 1 - i}));

    for (int i = 0; i < archer_count; i++)
        battle.add_fighter_to_team2(new Archer(vector<int>{N - 1, N - 1 - melee_count - i}));

    for (int i = 0; i < mage_count; i++)
        battle.add_fighter_to_team2(new Mage(vector<int>{N - 1, N - 1 - melee_count - archer_count - i}));

    int total_team2 = melee_count + archer_count + mage_count;
    if (total_team2 < M) {
        int remaining = M - total_team2;
        for (int i = 0; i < remaining; i++) {
            battle.add_fighter_to_team2(new Wars(vector<int>{N - 1, N - 1 - melee_count - archer_count - mage_count - i}));
        }
    }

    battle.print_team_statistics();
    cout << string(52, '=') << endl;

    battle.combat();
    cout << string(52, '=') << endl;

    battle.print_statistics();

    return 0;
}
