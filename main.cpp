#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <set>
#include <random>
#include <algorithm>
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

/**
 * Генерирует случайные эффекты для заклинаний.
 *
 * @param count Определяет диапазон выбора эффектов:
 *              -1: регенерация и починка,
 *              -2: пропуск хода и снятие щита,
 *              любое другое значение: двойной урон и пронзающий удар.
 * @return Вектор строк, представляющий случайные эффекты.
 */
vector<string> generate_random_effects(int count) {
    vector<string> all_effects = { "Регенерация", "Починка", "Пропуск хода", "Снятие щита",
        "Двойной урон", "Пронзающий удар" };
    set<string> current_eff;
    int index;

    // Случайный выбор эффектов в зависимости от переданного count
    if (count == -1) {
        int c = generate_random_int(1, 2);
        while (current_eff.size() < static_cast<size_t>(c)) {
            index = generate_random_int(0, 1);
            current_eff.insert(all_effects[index]);
        }
    }
    else if (count == -2) {
        int c = generate_random_int(1, 2);
        while (current_eff.size() < static_cast<size_t>(c)) {
            index = generate_random_int(2, 3);
            current_eff.insert(all_effects[index]);
        }
    }
    else {
        while (current_eff.size() < static_cast<size_t>(count)) {
            index = generate_random_int(4, 5);
            current_eff.insert(all_effects[index]);
        }
    }

    return vector<string>(current_eff.begin(), current_eff.end());
}

// Родительский класс 'Spell' (Заклинания) с общими свойствами.
class Spell {
protected:
    int mana_cost;              // Стоимость маны (от 1 до 100)
    int difficulty;             // Сложность заклинания (от 1 до 10)
    int cast_duration;          // Длительность применения заклинания (от 1 секунды до 1 часа)
    int range;                  // Диапазон заклинания (от 1 до 100)
    int effect_duration;        // Длительность эффекта в раундах (от 1 до 10)
    bool mass;                  // Массовое заклинание (0 - нет, 1 - да)
    vector<string> effects;     // Эффекты заклинаний
    string name;                // Название заклинания

public:
    /**
     * Конструктор класса Spell.
     *
     * @param name Название заклинания.
     * @param mana_cost Стоимость маны.
     * @param difficulty Уровень сложности.
     * @param cast_duration Длительность заклинания.
     * @param range Диапазон заклинания.
     * @param effect_duration Длительность эффекта.
     * @param mass Массовость.
     * @param effects Эффекты заклинания.
     */
    Spell(string name, int mana_cost, int difficulty, int cast_duration, int range,
        int effect_duration, bool mass, vector<string> effects)
        : name(name), mana_cost(mana_cost), difficulty(difficulty),
        cast_duration(cast_duration), range(range),
        effect_duration(effect_duration), mass(mass), effects(effects) {}

    /**
     * Виртуальная функция для применения эффекта заклинания.
     *
     * @param target_health Ссылка на здоровье цели.
     * @param target_shield Ссылка на щит цели.
     * @param statuses Вектор статусов, в который добавляются сообщения о применении заклинания.
     * @param self_cast Указывает, применяется ли заклинание на себя.
     */
    virtual void apply_effect(int& target_health, int& target_shield, vector<string>& statuses, bool self_cast = false) = 0;

    /**
     * Получает название заклинания.
     *
     * @return Название заклинания.
     */
    string getName() const {
        return name;
    }

    /**
     * Перегруженный оператор вывода для отображения информации о заклинании.
     *
     * @param os Поток вывода.
     * @param spell Объект заклинания.
     * @return Поток вывода с информацией о заклинании.
     */
    friend ostream& operator<<(ostream& os, const Spell& spell) {
        os << "Заклинание: " << spell.name << endl;
        os << "\t-Стоимость маны: " << spell.mana_cost << endl;
        os << "\t-Уровень сложности: " << spell.difficulty << endl;
        os << "\t-Продолжительность заклинания: " << spell.cast_duration << " секунд" << endl;
        os << "\t-Диапазон: " << spell.range << endl;
        os << "\t-Длительность эффекта: " << spell.effect_duration << " раундов" << endl;
        os << "\t-Массовость: " << (spell.mass ? "Да" : "Нет") << endl;
        os << "\t-Эффекты: ";
        for (const string& effect : spell.effects) {
            os << effect << " ";
        }
        os << endl << endl;
        return os;
    }
};

// Класс 'RangedSpell' (Дальнобойное заклинание) наследует от класса Spell.
class RangedSpell : public Spell {
public:
    /**
     * Конструктор класса RangedSpell.
     * Генерирует случайные параметры для заклинания.
     */
    RangedSpell() : Spell("Дальнобойное заклинание", generate_random_int(1, 100), generate_random_int(1, 10),
        generate_random_int(1, 3600), generate_random_int(51, 100),
        generate_random_int(1, 10), generate_random_int(0, 1), generate_random_effects(generate_random_int(0, 1))) {}

    /**
     * Применяет эффект дальнобойного заклинания к цели.
     *
     * @param target_health Ссылка на здоровье цели.
     * @param target_shield Ссылка на щит цели.
     * @param statuses Вектор статусов, в который добавляются сообщения о применении заклинания.
     * @param self_cast Указывает, применяется ли заклинание на себя (по умолчанию false).
     */
    void apply_effect(int& target_health, int& target_shield, vector<string>& statuses, bool self_cast = false) override {
        // Логика применения эффекта заклинания по разным эффектам
        if ((find(effects.begin(), effects.end(), "Двойной урон") != effects.end()) &&
            (find(effects.begin(), effects.end(), "Пронзающий удар") != effects.end())) {
            int damage_hp = generate_random_int(10, 30);
            int damage_sp = generate_random_int(10, 30);
            target_health -= damage_hp * 2;
            target_shield -= damage_sp * 2;
            statuses.push_back("Нанес урона: " + to_string(damage_hp * 2) + " HP " + to_string(damage_sp * 2) + " Sp ");
        }
        else if (find(effects.begin(), effects.end(), "Двойной урон") != effects.end()) {
            if (target_shield > 0) {
                int damage = generate_random_int(10, 30);
                target_shield -= damage * 2;
                statuses.push_back("Нанес урона: " + to_string(damage * 2) + " SP ");
            }
            else {
                int damage = generate_random_int(10, 30);
                target_health -= damage * 2;
                statuses.push_back("Нанес урона: " + to_string(damage * 2) + " HP ");
            }
        }
        else if (find(effects.begin(), effects.end(), "Пронзающий удар") != effects.end()) {
            int damage = generate_random_int(10, 30);
            target_health -= damage;
            statuses.push_back("Нанес урона: " + to_string(damage) + " HP ");
        }
        else {
            if (target_shield > 0) {
                int damage = generate_random_int(10, 30);
                target_shield -= damage;
                statuses.push_back("Нанес урона: " + to_string(damage) + " SP ");
            }
            else {
                int damage = generate_random_int(10, 30);
                target_health -= damage;
                statuses.push_back("Нанес урона: " + to_string(damage) + " HP ");
            }
        }
    }
};

// Класс 'MeleeSpell' (Заклинание ближнего действия) наследует от класса Spell.
class MeleeSpell : public Spell {
public:
    /**
     * Конструктор класса MeleeSpell.
     * Генерирует случайные параметры для заклинания.
     */
    MeleeSpell() : Spell("Заклинание ближнего действия", generate_random_int(1, 100), generate_random_int(1, 10),
        generate_random_int(1, 3600), generate_random_int(1, 9),
        generate_random_int(1, 10), generate_random_int(0, 1), generate_random_effects(generate_random_int(0, 2))) {}

    /**
     * Применяет эффект ближнего заклинания к цели.
     *
     * @param target_health Ссылка на здоровье цели.
     * @param target_shield Ссылка на щит цели.
     * @param statuses Вектор статусов, в который добавляются сообщения о применении заклинания.
     * @param self_cast Указывает, применяется ли заклинание на себя (по умолчанию false).
     */
    void apply_effect(int& target_health, int& target_shield, vector<string>& statuses, bool self_cast = false) override {
        // Логика применения эффекта заклинания по разным эффектам
        if ((find(effects.begin(), effects.end(), "Двойной урон") != effects.end()) &&
            (find(effects.begin(), effects.end(), "Пронзающий удар") != effects.end())) {
            int damage_hp = generate_random_int(5, 20);
            int damage_sp = generate_random_int(5, 20);
            target_health -= damage_hp * 2;
            target_shield -= damage_sp * 2;
            statuses.push_back("Нанес урона: " + to_string(damage_hp * 2) + " HP " + to_string(damage_sp * 2) + " Sp ");
        }
        else if (find(effects.begin(), effects.end(), "Двойной урон") != effects.end()) {
            if (target_shield > 0) {
                int damage = generate_random_int(5, 20);
                target_shield -= damage * 2;
                statuses.push_back("Нанес урона: " + to_string(damage * 2) + " SP ");
            }
            else {
                int damage = generate_random_int(5, 20);
                target_health -= damage * 2;
                statuses.push_back("Нанес урона: " + to_string(damage * 2) + " HP ");
            }
        }
        else if (find(effects.begin(), effects.end(), "Пронзающий удар") != effects.end()) {
            int damage = generate_random_int(5, 20);
            target_health -= damage;
            statuses.push_back("Нанес урона: " + to_string(damage) + " HP ");
        }
        else {
            if (target_shield > 0) {
                int damage = generate_random_int(5, 20);
                target_shield -= damage;
                statuses.push_back("Нанес урона: " + to_string(damage) + " SP ");
            }
            else {
                int damage = generate_random_int(5, 20);
                target_health -= damage;
                statuses.push_back("Нанес урона: " + to_string(damage) + " HP ");
            }
        }
    }
};

// Класс 'SupportSpell' (Поддерживающее заклинание) наследует от класса Spell.
class SupportSpell : public Spell {
public:
    /**
     * Конструктор класса SupportSpell.
     * Генерирует случайные параметры для заклинания.
     */
    SupportSpell() : Spell("Поддерживающее заклинание", generate_random_int(1, 100), generate_random_int(1, 10),
        generate_random_int(1, 3600), generate_random_int(1, 50),
        generate_random_int(1, 10), generate_random_int(0, 1), generate_random_effects(-1)) {}

    /**
     * Применяет эффект поддерживающего заклинания к цели.
     *
     * @param target_health Ссылка на здоровье цели.
     * @param target_shield Ссылка на щит цели.
     * @param statuses Вектор статусов, в который добавляются сообщения о применении заклинания.
     * @param self_cast Указывает, применяется ли заклинание на себя (по умолчанию false).
     */
    void apply_effect(int& target_health, int& target_shield, vector<string>& statuses, bool self_cast = false) override {
        // Логика применения эффекта поддерживающего заклинания
        if ((find(effects.begin(), effects.end(), "Регенерация") != effects.end()) &&
            (find(effects.begin(), effects.end(), "Починка") != effects.end())) {
            int healing = generate_random_int(15, 30);
            int shielding = generate_random_int(15, 30);
            target_health += healing;
            target_shield += shielding;
            statuses.push_back("Исцеление + починка: " + to_string(healing) + " HP " + to_string(shielding) + " SP ");
        }
        else if (find(effects.begin(), effects.end(), "Регенерация") != effects.end()) {
            int healing = generate_random_int(15, 30);
            target_health += healing;
            statuses.push_back("Исцеление: " + to_string(healing) + " HP ");
        }
        else {
            int shielding = generate_random_int(15, 30);
            target_shield += shielding;
            statuses.push_back("Починка: " + to_string(shielding) + " SP ");
        }
    }
};

// Класс 'CurseSpell' (Проклятие) наследует от класса Spell.
class CurseSpell : public Spell {
private:
    int remaining_turns;    // Остаток раундов действия эффекта

public:
    /**
     * Конструктор класса CurseSpell.
     * Генерирует случайные параметры для заклинания.
     */
    CurseSpell() : Spell("Проклятие", generate_random_int(1, 100), generate_random_int(1, 10),
        generate_random_int(1, 3600), generate_random_int(1, 50),
        generate_random_int(1, 10), generate_random_int(0, 1), generate_random_effects(-2)),
        remaining_turns(0) {}

    /**
     * Применяет эффект проклятия к цели.
     *
     * @param target_health Ссылка на здоровье цели.
     * @param target_shield Ссылка на щит цели.
     * @param statuses Вектор статусов, в который добавляются сообщения о применении заклинания.
     * @param self_cast Указывает, применяется ли заклинание на себя (по умолчанию false).
     */
    void apply_effect(int& target_health, int& target_shield, vector<string>& statuses, bool self_cast = false) override {
        if (remaining_turns > 0) {
            remaining_turns--;
            statuses.push_back("Эффект 'Снятие щита' активен. Осталось раундов: " + to_string(remaining_turns));
            target_shield = 0;      // Снятие щита
            return;
        }

        // Логика применения эффекта проклятия
        if ((find(effects.begin(), effects.end(), "Пропуск хода") != effects.end()) &&
            (find(effects.begin(), effects.end(), "Снятие щита") != effects.end())) {
            int shielding = 0;
            target_shield *= shielding;     // Эффект 'Снятие щита'
            statuses.push_back("Проклятия (пропуск хода и снятие щита): " + to_string(shielding) + " ");
            remaining_turns = effect_duration;  // Время действия эффекта
        }
        else if (find(effects.begin(), effects.end(), "Пропуск хода") != effects.end()) {
            statuses.push_back("Проклятие - Пропуск хода: ");
        }
        else {
            int shielding = 0;
            target_shield *= shielding;     // Эффект 'Снятие щита'
            statuses.push_back("Проклятие - снятие щита: " + to_string(shielding) + " ");
            remaining_turns = effect_duration;  // Время действия эффекта
        }
    }
};

// Класс 'Mage' (Маг) представляет одного персонажа.
class Mage {
private:
    int name;               // Имя мага
    int health;             // Здоровье
    int shield;             // Щит
    vector<Spell*> spells;  // Вектор заклинаний

public:
    /**
     * Конструктор класса Mage.
     *
     * @param name Имя мага (числовое значение).
     */
    Mage(int name) : name(name), health(100), shield(100) {
        set<int> unique_numbers;
        while (unique_numbers.size() < 2) {
            int num = generate_random_int(0, 3);
            unique_numbers.insert(num);
        }
        vector<int> numbers_vector(unique_numbers.begin(), unique_numbers.end());
        int n = generate_random_int(1, 10);     // Количество заклинаний
        for (int i = 0; i < n; i++) {
            int random_index = rand() % numbers_vector.size();
            switch (numbers_vector[random_index]) {
            case 0:
                spells.push_back(new RangedSpell());
                break;
            case 1:
                spells.push_back(new MeleeSpell());
                break;
            case 2:
                spells.push_back(new SupportSpell());
                break;
            case 3:
                spells.push_back(new CurseSpell());
                break;
            default:
                break;
            }
        }
    }

    /**
     * Применяет заклинания к цели.
     *
     * @param target Цель для применения заклинаний.
     */
    void apply_spells(Mage& target) {
        vector<string> statuses;
        int caster_name = name;
        cout << string(50, '-') << endl << "Маг " << caster_name << " применяет заклинания на мага " << target.name << ":" << endl;

        if (!spells.empty()) {
            // Выбор случайного заклинания
            int random_index = generate_random_int(0, spells.size() - 1);
            Spell* selected_spell = spells[random_index];

            if (selected_spell->getName() == "Поддерживающее заклинание") {
                selected_spell->apply_effect(health, shield, statuses, true);    // Применение на себя
            }
            else {
                selected_spell->apply_effect(target.health, target.shield, statuses);    // Применение на цель
            }
        }

        // Вывод статусов применения заклинаний
        for (const auto& status : statuses) {
            cout << status << endl;
        }
        cout << string(50, '-') << endl;
    }


    /**
     * Выводит заклинания мага на экран.
     */
    void print_spell() const {
        for (auto spell : spells) {
            cout << *spell;
        }
    }

    /**
    * Получает текущее имя мага.
    *
    * @return Имя мага.
    */
    int get_name() const {
        return name;
    }

    /**
     * Получает текущее здоровье мага.
     *
     * @return Здоровье мага.
     */
    int get_health() const {
        return health;
    }

    /**
     * Получает текущее значение щита мага.
     *
     * @return Щит мага.
     */
    int get_shield() const {
        return shield;
    }

    /**
     * Проверяет, жив ли маг.
     *
     * @return true, если маг жив; в противном случае false.
     */
    bool is_alive() const {
        return health > 0;
    }

    /**
     * Деструктор класса Mage.
     * Освобождает память, выделенную для заклинаний.
     */
    ~Mage() {
        for (auto spell : spells) {
            delete spell;
        }
    }
};

// Класс 'Team' (Команда) представляет группу магов.
class Team {
private:
    vector<Mage*> mages;    // Вектор магов в команде

public:
    /**
     * Конструктор класса Team.
     *
     * @param team_size Размер команды.
     * @param team_number Номер команды.
     */
    Team(int team_size, int team_number) {
        // Инициализация команды случайными магами
        for (int i = 0; i < team_size; i++) {
            mages.push_back(new Mage(team_number * 10 + i + 1));
        }
    }

    /**
     * Организует бой между командами.
     *
     * @param enemy_team Команда врагов.
     */
    void battle(Team& enemy_team) {
        while (true) {
            vector<string> statuses;
            // Каждый маг из первой команды использует свои заклинания на случайном маге второй команды
            for (size_t i = 0; i < mages.size(); i++) {
                if (mages[i]->is_alive()) {
                    // Выбираем случайного врага
                    Mage* target = enemy_team.mages[generate_random_int(0, enemy_team.mages.size() - 1)];
                    // Применяем заклинание
                    if (target->is_alive()) {
                        mages[i]->apply_spells(*target);
                    }
                }
            }
            // Каждый маг из второй команды использует свои заклинания на случайном маге первой команды
            for (size_t i = 0; i < enemy_team.mages.size(); i++) {
                if (enemy_team.mages[i]->is_alive()) {
                    // Выбираем случайного врага
                    Mage* target = mages[generate_random_int(0, mages.size() - 1)];
                    // Применяем заклинание
                    if (target->is_alive()) {
                        enemy_team.mages[i]->apply_spells(*target);
                    }
                }
            }

            // Вывод статусов применения заклинаний
            cout << string(50, '-') << endl;
            for (const auto& status : statuses) {
                cout << status << endl;
            }

            // Информация о здоровье и щите каждого мага
            cout << "\t<====Состояние после раунда====>\nКоманда 1:" << endl;
            for (auto mage : mages) {
                cout << "\tМаг " << mage->get_name() << " - Здоровье = " << mage->get_health() << ", Щит = " << mage->get_shield() << endl;
            }
            cout << "Команда 2:" << endl;
            for (auto mage : enemy_team.mages) {
                cout << "\tМаг " << mage->get_name() << " - Здоровье = " << mage->get_health() << ", Щит = " << mage->get_shield() << endl;
            }
            cout << string(50, '=') << "\n\t\tРаунд завершен\n" << string(50, '=') << endl << endl;

            // Проверка, есть ли живые маги в обеих командах
            bool any_alive = false;
            for (auto mage : mages) {
                if (mage->is_alive()) {
                    any_alive = true;
                    break;
                }
            }
            bool any_enemy_alive = false;
            for (auto mage : enemy_team.mages) {
                if (mage->is_alive()) {
                    any_enemy_alive = true;
                    break;
                }
            }
            if (!any_alive || !any_enemy_alive) {
                break; // Завершение боя, если одна из команд потерпела поражение
            }
        }
        cout << "Бой закончился!" << endl;
    }

    /**
     * Выводит информацию о заклинаниях каждого мага в команде.
     */
    void print_team() const {
        for (const auto& mage : mages) {
            mage->print_spell();
        }
    }

    /**
     * Деструктор класса Team.
     * Освобождает память, выделенную для магов.
     */
    ~Team() {
        for (auto mage : mages) {
            delete mage;
        }
    }
};

int main() {
    setlocale(LC_ALL, "");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    srand(static_cast<unsigned int>(time(0)));

    // int team_size = generate_random_int(5, 10); 
    int team_size = 2; // Значение для теста
    Team team1(team_size, 1); // Создание первой команды
    Team team2(team_size, 2); // Создание второй команды

    cout << "--------------Заклинания команды 1-----------------" << endl;
    team1.print_team();
    cout << "--------------Заклинания команды 2-----------------" << endl;
    team2.print_team();

    // Запуск боя между командами
    team1.battle(team2);

    return 0;
}
