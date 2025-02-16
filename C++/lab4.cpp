#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <Windows.h>

using namespace std;

// Класс для описания навыков оружия
class Skill {
public:
    string name;
    Skill(string n) : name(n) {}
};

// Доступные навыки для оружия
vector<Skill> available_skills = {
    Skill("Flame Strike"),
    Skill("Ice Thrust"),
    Skill("Fire Slash"),
    Skill("Ice Strike"),
    Skill("Lightning Strike"),
    Skill("Wind Slash")
};

// Класс, представляющий копье
class Spear {
public:
    int damage;                 // Урон
    int durability;             // Прочность
    float attack_speed;         // Скорость атаки
    string rarity;              // Редкость
    vector<string> skills;      // Навыки

    Spear() {
        generate_attributes();      // Генерируем атрибуты оружия при создании
    }

    // Генерируем атрибуты копья
    void generate_attributes() {
        rarity = generate_rarity();     // Генерация редкости
        damage = (rarity == "Common") ? rand() % 5 + 5 : (rarity == "Rare") ? rand() % 10 + 10 : rand() % 20 + 20;
        durability = (rarity == "Common") ? rand() % 20 + 30 : (rarity == "Rare") ? rand() % 30 + 50 : rand() % 50 + 80;
        attack_speed = (rarity == "Common") ? (rand() % 5 + 5) / 10.0f : (rarity == "Rare") ? (rand() % 3 + 6) / 10.0f : (rand() % 2 + 8) / 10.0f;

        set_skills();   // Установка навыков оружия
    }

    // Генерация редкости
    string generate_rarity() {
        int chance = rand() % 100;
        if (chance < 70) return "Common";   // 70% шанс на "Common"
        if (chance < 90) return "Rare";     // 20% шанс на "Rare"
        return "Legendary";                 // 10% шанс на "Legendary"
    }

    // Установка навыков в зависмости от редкости
    void set_skills() {
        int skill_count = (rarity == "Common") ? 0 : (rarity == "Rare") ? 1 : 2;
        for (int i = 0; i < skill_count; ++i) {
            int random_index = rand() % available_skills.size();
            skills.push_back(available_skills[random_index].name);      // Добавление навыка
        }
    }
};

// Класс, представляющий меч
class Sword {
public:
    int damage;                 // Урон
    int durability;             // Прочность
    float attack_speed;         // Скорость атаки
    string rarity;              // Редкость
    vector<string> skills;      // Навыки

    Sword() {
        generate_attributes();      // Генерируем атрибуты оружия при создании
    }

    // Генерация атрибутов меча
    void generate_attributes() {
        rarity = generate_rarity();     // Генерация редкости
        damage = (rarity == "Common") ? rand() % 6 + 4 : (rarity == "Rare") ? rand() % 8 + 10 : rand() % 25 + 20;
        durability = (rarity == "Common") ? rand() % 25 + 25 : (rarity == "Rare") ? rand() % 20 + 50 : rand() % 30 + 90;
        attack_speed = (rarity == "Common") ? (rand() % 5 + 5) / 10.0f : (rarity == "Rare") ? (rand() % 4 + 5) / 10.0f : (rand() % 2 + 7) / 10.0f;

        set_skills();   // Установка навыков оружия
    }

    // Генерация редкости
    string generate_rarity() {
        int chance = rand() % 100;
        if (chance < 70) return "Common";   // 70% шанс на "Common"
        if (chance < 90) return "Rare";     // 20% шанс на "Rare"
        return "Legendary";                 // 10 % шанс на "Legendary"
    }
  
    // Установка навыков
    void set_skills() {
        int skill_count = (rarity == "Common") ? 0 : (rarity == "Rare") ? 1 : 2;
        for (int i = 0; i < skill_count; ++i) {
            int random_index = rand() % available_skills.size();
            skills.push_back(available_skills[random_index].name);
        }
    }
};

// Класс, представляющий лук
class Bow {
public:
    int damage;                 // Урон
    int durability;             // Прочность
    float attack_speed;         // Скорость атаки
    string rarity;              // Редкость
    vector<string> skills;      // Навыки

    Bow() {
        generate_attributes();      // Генерируем атрибуты оружия при создании
    }

    // Генерация атрибутов лука
    void generate_attributes() {
        rarity = generate_rarity();     // Генерация редкости
        damage = (rarity == "Common") ? rand() % 6 + 4 : (rarity == "Rare") ? rand() % 8 + 10 : rand() % 25 + 20;
        durability = (rarity == "Common") ? rand() % 25 + 25 : (rarity == "Rare") ? rand() % 20 + 50 : rand() % 30 + 90;
        attack_speed = (rarity == "Common") ? (rand() % 5 + 5) / 10.0f : (rarity == "Rare") ? (rand() % 4 + 5) / 10.0f : (rand() % 2 + 7) / 10.0f;

        set_skills();   // Установка навыков оружия
    }

    // Генерация редкости
    string generate_rarity() {
        int chance = rand() % 100;
        if (chance < 70) return "Common";   // 70% шанс на "Common"
        if (chance < 90) return "Rare";     // 20% шанс на "Rare"
        return "Legendary";                 // 10 % шанс на "Legendary"
    }

    // Установка навыков
    void set_skills() {
        int skill_count = (rarity == "Common") ? 0 : (rarity == "Rare") ? 1 : 2;
        for (int i = 0; i < skill_count; ++i) {
            int random_index = rand() % available_skills.size();
            skills.push_back(available_skills[random_index].name);
        }
    }
};

// Класс универсального оружия
class Weapon {
public:
    void* weaponPtr;    // Указатель на оружие
    string type;

    Weapon() : weaponPtr(nullptr) {}    // Инициализация конструктора

    // Создание оружия в зависимости от типа
    void create_weapon(int weapon_type) {
        switch (weapon_type) {
        case 1:
            weaponPtr = new Spear();
            type = "Spear";
            break;
        case 2:
            weaponPtr = new Sword();
            type = "Sword";
            break;
        case 3:
            weaponPtr = new Bow();
            type = "Bow";
            break;
        default:
            cout << "Нет такого оружия." << endl;
            return;
        }
    }

    // Печать информации об оружии
    void print_info() const {
        if (!weaponPtr) return;

        if (type == "Spear") {
            Spear* spear = static_cast<Spear*>(weaponPtr);
            print_weapon_details("Spear", spear->damage, spear->durability, spear->attack_speed, spear->rarity, spear->skills);
        }
        else if (type == "Sword") {
            Sword* sword = static_cast<Sword*>(weaponPtr);
            print_weapon_details("Sword", sword->damage, sword->durability, sword->attack_speed, sword->rarity, sword->skills);
        }
        else if (type == "Bow") {
            Bow* bow = static_cast<Bow*>(weaponPtr);
            print_weapon_details("Bow", bow->damage, bow->durability, bow->attack_speed, bow->rarity, bow->skills);
        }
    }

    // Получение цены оружия
    int get_price() const {
        if (!weaponPtr) return 0;

        if (type == "Spear") {
            return static_cast<Spear*>(weaponPtr)->damage + static_cast<Spear*>(weaponPtr)->durability;
        }
        else if (type == "Sword") {
            return static_cast<Sword*>(weaponPtr)->damage + static_cast<Sword*>(weaponPtr)->durability;
        }
        else if (type == "Bow") {
            return static_cast<Bow*>(weaponPtr)->damage + static_cast<Bow*>(weaponPtr)->durability;
        }
        return 0;
    }

    // Печать деталей оружия
    void print_weapon_details(const string& type, int damage, int durability, float attack_speed, const string& rarity, const vector<string>& skills) const {
        cout << "Weapon Type: " << type << endl;
        cout << "Damage: " << damage << endl;
        cout << "Durability: " << durability << endl;
        cout << "Attack Speed: " << attack_speed << endl;
        cout << "Rarity: " << rarity << endl;
        cout << "Skills: ";
        if (skills.empty()) {
            cout << "Нет" << endl;
        }
        else {
            for (const auto& skill : skills) {
                cout << skill << " ";
            }
            cout << endl;
        }

        cout << "Визуализация навыков: ";
        for (const auto& skill : skills) {
            cout << "*" << skill << "* ";
        }
        cout << endl;
    }
};

// Класс для магазина
class Shop {
public:
    vector<Weapon> weapons;

    // Заполнение магазина оружием
    void fill_shop() {
        const int common_count = 50;        // Количество обычного оружия
        const int rare_count = 30;          // Количество редкого оружия
        const int legendary_count = 10;     // Количество легендарного оружия

        // Заполнение магазина обычным оружием
        for (int i = 0; i < common_count; i++) {
            Weapon weapon;
            weapon.create_weapon(rand() % 3 + 1);   // Генерация случайного типа оружия
            weapons.push_back(weapon);      // Добавление в магазин
        }

        // Заполнение магазина редким оружием
        for (int i = 0; i < rare_count; i++) {
            Weapon weapon;
            weapon.create_weapon(rand() % 3 + 1);
            weapons.push_back(weapon);
        }

        // Заполнение магазина легендарным оружием
        for (int i = 0; i < legendary_count; i++) {
            Weapon weapon;
            weapon.create_weapon(rand() % 3 + 1);
            weapons.push_back(weapon);
        }
    }

    // Печать оружия в пределах бюджета
    void print_weapons(int weapon_type, int budget) const {
        for (const auto& weapon : weapons) {
            if ((weapon.type == "Spear" && weapon_type == 1) ||
                (weapon.type == "Sword" && weapon_type == 2) ||
                (weapon.type == "Bow" && weapon_type == 3)) {

                int price = weapon.get_price();     // Получение цены оружия
                if (price < budget) {
                    weapon.print_info();    // Печать информации об оружии
                    cout << "Price: " << price << endl;
                    cout << "--------------------------------" << endl;
                }
            }
        }
    }

    // Печать всего оружия в магазине
    void print_all_weapons() const {
        for (const auto& weapon : weapons) {
            weapon.print_info();    // Печать информации об оружии
            cout << "--------------------------------" << endl;
        }
    }

    // Посещение магазина
    void visit_shop() {
        int weapon_type, budget;

        cout << "Добро пожаловать в магазин оружия" << endl;
        cout << "Какое оружие вы рассматриваете? (1 - Копье, 2 - Меч, 3 - Лук): ";
        cin >> weapon_type;

        cout << "Введите ваш бюджет: ";
        cin >> budget;

        cout << "Ваше оружие: ";

        switch (weapon_type) {
        case 1:
            cout << "Копье" << endl;
            break;
        case 2:
            cout << "Меч" << endl;
            break;
        case 3:
            cout << "Лук" << endl;
            break;
        default:
            cout << "Нет такого оружия." << endl;
            return;
        }
        cout << "--------------------------------" << endl;

        print_weapons(weapon_type, budget);     // Печать доступного оружия

        cout << "Спасибо, приходите еще!" << endl;
    }
};

int main() {
    setlocale(LC_ALL, "");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    srand(static_cast<unsigned int>(time(0)));

    Shop myShop;
    myShop.fill_shop();     // Заполнение магазина оружием
    myShop.visit_shop();    // Посещение магазина

    char choice;
    cout << "Показать все оружие, доступное в магазине? (y/n): ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        cout << "Весь товар магазина:" << endl;
        myShop.print_all_weapons();     // Печать всего доступного оружия
    }
    cout << "Спасибо, приходите еще!" << endl;

    return 0;
}
