#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <Windows.h>

using namespace std;

// Общий класс Skill, представляющий навык
class Skill {
public:
    string name;
    Skill(string n) : name(n) {}
};

// Инициализация глобального вектора навыков
vector<Skill> available_skills = {
    Skill("Flame Strike"),
    Skill("Ice Thrust"),
    Skill("Fire Slash"),
    Skill("Ice Strike"),
    Skill("Lightning Strike"),
    Skill("Wind Slash")
};

// Класс Spear (Копье), представляющий копье в игре
class Spear {
public:
    int damage;
    int durability;
    float attack_speed;
    string rarity;
    vector<string> skills;

    Spear() {
        generate_attributes();
    }

    void generate_attributes() {
        rarity = generate_rarity();
        damage = (rarity == "Common") ? rand() % 5 + 5 : (rarity == "Rare") ? rand() % 10 + 10 : rand() % 20 + 20;
        durability = (rarity == "Common") ? rand() % 20 + 30 : (rarity == "Rare") ? rand() % 30 + 50 : rand() % 50 + 80;
        attack_speed = (rarity == "Common") ? (rand() % 5 + 5) / 10.0f : (rarity == "Rare") ? (rand() % 3 + 6) / 10.0f : (rand() % 2 + 8) / 10.0f;

        // Определение навыков
        set_skills();
    }

    string generate_rarity() {
        int chance = rand() % 100;
        if (chance < 70) return "Common";
        if (chance < 90) return "Rare";
        return "Legendary";
    }

    void set_skills() {
        int skill_count = (rarity == "Common") ? 0 : (rarity == "Rare") ? 1 : 2;
        for (int i = 0; i < skill_count; ++i) {
            int random_index = rand() % available_skills.size();
            skills.push_back(available_skills[random_index].name);
        }
    }
};

// Класс Sword (Меч), представляющий меч в игре
class Sword {
public:
    int damage;
    int durability;
    float attack_speed;
    string rarity;
    vector<string> skills;

    Sword() {
        generate_attributes();
    }

    void generate_attributes() {
        rarity = generate_rarity();
        damage = (rarity == "Common") ? rand() % 6 + 4 : (rarity == "Rare") ? rand() % 8 + 10 : rand() % 25 + 20;
        durability = (rarity == "Common") ? rand() % 25 + 25 : (rarity == "Rare") ? rand() % 20 + 50 : rand() % 30 + 90;
        attack_speed = (rarity == "Common") ? (rand() % 5 + 5) / 10.0f : (rarity == "Rare") ? (rand() % 4 + 5) / 10.0f : (rand() % 2 + 7) / 10.0f;

        // Определение навыков
        set_skills();
    }

    string generate_rarity() {
        int chance = rand() % 100;
        if (chance < 70) return "Common";
        if (chance < 90) return "Rare";
        return "Legendary";
    }

    void set_skills() {
        int skill_count = (rarity == "Common") ? 0 : (rarity == "Rare") ? 1 : 2;
        for (int i = 0; i < skill_count; ++i) {
            int random_index = rand() % available_skills.size();
            skills.push_back(available_skills[random_index].name);
        }
    }
};

// Класс Bow (Лук), представляющий лук в игре
class Bow {
public:
    int damage;
    int durability;
    float attack_speed;
    string rarity;
    vector<string> skills;

    Bow() {
        generate_attributes();
    }

    void generate_attributes() {
        rarity = generate_rarity();
        damage = (rarity == "Common") ? rand() % 6 + 4 : (rarity == "Rare") ? rand() % 8 + 10 : rand() % 25 + 20;
        durability = (rarity == "Common") ? rand() % 25 + 25 : (rarity == "Rare") ? rand() % 20 + 50 : rand() % 30 + 90;
        attack_speed = (rarity == "Common") ? (rand() % 5 + 5) / 10.0f : (rarity == "Rare") ? (rand() % 4 + 5) / 10.0f : (rand() % 2 + 7) / 10.0f;

        // Определение навыков
        set_skills();
    }

    string generate_rarity() {
        int chance = rand() % 100;
        if (chance < 70) return "Common";
        if (chance < 90) return "Rare";
        return "Legendary";
    }

    void set_skills() {
        int skill_count = (rarity == "Common") ? 0 : (rarity == "Rare") ? 1 : 2;
        for (int i = 0; i < skill_count; ++i) {
            int random_index = rand() % available_skills.size();
            skills.push_back(available_skills[random_index].name);
        }
    }
};

// Класс Weapon (Оружие), представляющий оружие в магазине
class Weapon {
public:
    void* weaponPtr;
    string type;

    Weapon() : weaponPtr(nullptr) {}

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
            cout << "Неверный тип оружия." << endl;
            return;
        }
    }

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

// Класс Shop (Магазин), представляющий магазин оружия
class Shop {
public:
    vector<Weapon> weapons;

    void fill_shop() {
        const int common_count = 50;
        const int rare_count = 30;
        const int legendary_count = 10;

        for (int i = 0; i < common_count; i++) {
            Weapon weapon;
            weapon.create_weapon(rand() % 3 + 1);
            weapons.push_back(weapon);
        }

        for (int i = 0; i < rare_count; i++) {
            Weapon weapon;
            weapon.create_weapon(rand() % 3 + 1);
            weapons.push_back(weapon);
        }

        for (int i = 0; i < legendary_count; i++) {
            Weapon weapon;
            weapon.create_weapon(rand() % 3 + 1);
            weapons.push_back(weapon);
        }
    }

    void print_weapons(int weapon_type, int budget) const {
        for (const auto& weapon : weapons) {
            if ((weapon.type == "Spear" && weapon_type == 1) ||
                (weapon.type == "Sword" && weapon_type == 2) ||
                (weapon.type == "Bow" && weapon_type == 3)) {

                int price = weapon.get_price();
                if (price < budget) {
                    weapon.print_info();
                    cout << "Цена: " << price << endl;
                    cout << "--------------------------------" << endl;
                }
            }
        }
    }

    void print_all_weapons() const {
        for (const auto& weapon : weapons) {
            weapon.print_info();
            cout << "--------------------------------" << endl;
        }
    }

    void visit_shop() {
        int weapon_type, budget;

        cout << "Добро пожаловать в магазин оружия" << endl;
        cout << "Введите тип необходимого вам оружия (1 - Копье, 2 - Меч, 3 - Лук): ";
        cin >> weapon_type;

        cout << "Введите ваш бюджет на оружие: ";
        cin >> budget;

        cout << "Вы ищете ";

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
            cout << "Неверный тип оружия." << endl;
            return;
        }

        cout << "Вот доступное оружие в рамках вашего бюджета:" << endl;
        print_weapons(weapon_type, budget);

        cout << "Спасибо, приходите еще!" << endl;
    }
};

int main() {
    setlocale(LC_ALL, "");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    srand(static_cast<unsigned int>(time(0)));

    Shop myShop;
    myShop.fill_shop();
    myShop.visit_shop();

    char choice;
    cout << "Хотите увидеть весь ассортимент оружия? (y/n): ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        cout << "Вот весь ассортимент оружия:" << endl;
        myShop.print_all_weapons();
    }
    else {
        cout << "Спасибо за ваше время!" << endl;
    }

    return 0;
}
