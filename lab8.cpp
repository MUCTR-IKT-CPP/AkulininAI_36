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
    * ����������� ������ Bullet.
    * 
    * @param type ��� �������.
    */
    Bullet(const string& type) : type(type) {}

    /**
    * ���������� �������� ������, ��������� � ����� �������.
    *
    * @return ������ � ��������� ��������� �������.
    */
    string sound_effect() const {
        if (type == "����������") {
            return "����� ���� ��������";
        }
        else if (type == "�������") {
            return "����������� ���� ��������";
        }
        else if (type == "������������") {
            return "������� ���� �������� � ����������";
        }
        return "����������� ����";
    }

    /**
    * ���������� ��������� ������ ��� ������ Bullet.
    * 
    * @param os ����� ������.
    * @param bullet ��������� Bullet.
    * @return ���������� ����� ������.
    */
    friend ostream& operator<<(ostream& os, const Bullet& bullet) {
        os << "������ { ���: " << bullet.type << " }";
        return os;
    }

    /**
    * ������� ��� ��������� ���� �������.
    *
    * @return ���������� ��� �������.
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
    * ���������, �������� �� �������.
    * 
    * @return true, ���� ������� �����, ����� false.
    */
    bool is_full() const {
        return bullets.size() >= capacity;
    }

    /**
    * ���������, ���� �� �������.
    * 
    * @return true, ���� ������� ����, ����� false.
    */
    bool is_empty() const {
        return bullets.empty();
    }

    /**
    * ������������ ������� � ��������.
    *
    * ��������� ������� �� ����������� ������� � ������� �� ��� ���,
    * ���� �� �� ����������.
    *
    * @param bullet_pool ������ �������� ��� ����������� ��������.
    * @return true, ���� ������ ������� ��������, ����� false.
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
    * ���������� ������� �� �������� � ��������� ������.
    *
    * @param bullet ������, ������� ����� �������� �� ��������.
    * @return true, ���� ������� �������� �������, ����� false.
    */
    bool shoot(Bullet& bullet) {
        if (is_empty()) {
            cout << "������� ������" << endl;
            return false;
        }
        bullet = bullets.top();
        bullets.pop();
        return true;
    }

    /**
    * ���������� ��������� ������ ��� ������ Ammo.
    * 
    * @param os ����� ������.
    * @param ammo ��������� Ammo. 
    * @return ���������� ����� ������.
    */
    friend ostream& operator<<(ostream& os, const Ammo& ammo) {
        os << "������� { �������: " << ammo.bullets.size() << " }";
        return os;
    }

    /**
    * ������� ��� ��������� ���������� �������� � ��������.
    *
    * @return ���������� ���������� �������� � ��������.
    */
    int get_count() const {
        return bullets.size();
    }

    /**
    * ������� ������� �� ���� ��������.
    */
    void clear() {
        while (!bullets.empty()) {
            bullets.pop();
        }
    }

    /**
    * ��������� ������ � �������.
    *
    * @param bullet ������, ������� ����� ��������.
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
    * ���������� ��������� ������� � ���������� count � ��������� �� � ������ all_bullets.
    *
    * @param count ���������� ���� ����������� ��������.
    */
    void generate_bullets(int count) {
        for (int i = 0; i < count; i++) {
            string type;
            switch (rand() % 3) {
            case 0: type = "����������"; break;
            case 1: type = "�������"; break;
            case 2: type = "������������"; break;
            }
            all_bullets.emplace_back(type);
        }
    }

    /**
    * ��������� ������� �������� �� ������� all_bullets, ���� �� �� ����� ��������.
    *
    * ���������, ������ �� ������� ����� ���������.
    */
    void load_magazine() {
        if (!ammo) {
            cout << "------------- ����� ������� --------------" << endl;
            ammo = make_unique<Ammo>();
        }

        while (!all_bullets.empty() && ammo->get_count() < 30) {
            ammo->add_bullet(all_bullets.back());
            all_bullets.pop_back();
        }
        total_reload_ammo++;
    }

    /**
    * ���������� ������� �� ������. � ������ ��������� ��������,
    * ������������� ��������������� ���������� � ������������ ���� �������.
    */
    void shoot() {
        if (!ammo || ammo->is_empty()) {
            cout << "��� ��������� �������� ��� ��������!" << endl;
            return;
        }

        Bullet bullet("");
        if (ammo->shoot(bullet)) {
            Sleep(100);
            cout << bullet.sound_effect() << endl;
            total_shots++;
            if (bullet.get_type() == "����������") {
                count_subsonic++;
            }
            else if (bullet.get_type() == "�������") {
                count_conventional++;
            }
            else if (bullet.get_type() == "������������") {
                count_tracer++;
            }

            if (ammo->is_empty() && get_remaining_bullets() > 0) {
                cout << "==================== ����� ����������� ====================" << endl;
                Sleep(2000);
                ammo.reset();
            }
        }
    }

    /**
    * ���������, ���� �� ������� ��� ����������� �� �������.
    *
    * @return true, ���� ������� ����, ����� false.
    */
    bool is_ammo_empty() const {
        return !ammo || ammo->is_empty();
    }

    /**
    * ����� ���������� ����� ���������� ��������.
    */
    void print_statistics() const {
        cout << endl << string(50, '=') << endl;
        cout << "��������� ���-�� ������������ ��������: " << total_shots << endl;
        cout << "��������� ���-�� ��������������� ���������: " << total_reload_ammo << endl;
        cout << "���-�� '����������' ��������: " << count_subsonic << endl;
        cout << "���-�� '�������' ��������: " << count_conventional << endl;
        cout << "���-�� '������������' ��������: " << count_tracer << endl;
    }

    /**
    * ���������� ��������� ������ ��� ������ Weapon.
    * 
    * @param os ����� ������.
    * @param weapon ��������� Weapon.
    * @return ���������� ����� ������.
    */
    friend ostream& operator<<(ostream& os, const Weapon& weapon) {
        os << "������ { �������: " << (weapon.ammo ? *weapon.ammo : Ammo()) << " }";
        return os;
    }

    /**
    * ���������� ���������� ���������� ��������.
    *
    * @return ���������� ��������, ���������� � all_bullets.
    */
    int get_remaining_bullets() const {
        return all_bullets.size();
    }

    /**
    * ��������� ���������, �������� ���, ���� �� ����.
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
    cout << "��������� ����� ����������� �� ������� �������� � �������: " << time_reload << " ���\n";
    cout << "��������� ����� ��������: " << time_shot << " c��\n";
    return 0;
}