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
    * Êîíñòðóêòîð êëàññà Bullet.
    * 
    * @param type Òèï ïàòðîíà.
    */
    Bullet(const string& type) : type(type) {}

    /**
    * Âîçâðàùàåò çâóêîâîé ýôôåêò, ñâÿçàííûé ñ òèïîì ïàòðîíà.
    *
    * @return Ñòðîêà ñ îïèñàíèåì çâóêîâîãî ýôôåêòà.
    */
    string sound_effect() const {
        if (type == "Äîçâóêîâûå") {
            return "Òèõèé çâóê âûñòðåëà";
        }
        else if (type == "Îáû÷íûå") {
            return "Ñòàíäàðòíûé çâóê âûñòðåëà";
        }
        else if (type == "Òðàññèðóþùèå") {
            return "Îáû÷íûé çâóê âûñòðåëà ñ ïîäñâåòêîé";
        }
        return "Íåèçâåñòíûé çâóê";
    }

    /**
    * Ïåðåãðóçêà îïåðàòîðà âûâîäà äëÿ êëàññà Bullet.
    * 
    * @param os Ïîòîê âûâîäà.
    * @param bullet Ýêçåìïëÿð Bullet.
    * @return Èçìåíåííûé ïîòîê âûâîäà.
    */
    friend ostream& operator<<(ostream& os, const Bullet& bullet) {
        os << "Ïàòðîí { Òèï: " << bullet.type << " }";
        return os;
    }

    /**
    * Ôóíêöèÿ äëÿ ïîëó÷åíèÿ òèïà ïàòðîíà.
    *
    * @return Âîçâðàùàåò òèï ïàòðîíà.
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
    * Ïðîâåðÿåò, çàïîëíåí ëè ìàãàçèí.
    * 
    * @return true, åñëè ìàãàçèí ïîëîí, èíà÷å false.
    */
    bool is_full() const {
        return bullets.size() >= capacity;
    }

    /**
    * Ïðîâåðÿåò, ïóñò ëè ìàãàçèí.
    * 
    * @return true, åñëè ìàãàçèí ïóñò, èíà÷å false.
    */
    bool is_empty() const {
        return bullets.empty();
    }

    /**
    * Ïåðåçàðÿæàåò ïàòðîíû â ìàãàçèíå.
    *
    * Çàãðóæàåò ïàòðîíû èç ïåðåäàííîãî âåêòîðà â ìàãàçèí äî òåõ ïîð,
    * ïîêà îí íå çàïîëíèòñÿ.
    *
    * @param bullet_pool Âåêòîð ïàòðîíîâ äëÿ ïåðåçàðÿäêè ìàãàçèíà.
    * @return true, åñëè ïàòðîí óñïåøíî çàãðóæåí, èíà÷å false.
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
    * Ïðîèçâîäèò âûñòðåë èç ìàãàçèíà è èçâëåêàåò ïàòðîí.
    *
    * @param bullet Ïàòðîí, êîòîðûé áóäåò èçâëå÷åí èç ìàãàçèíà.
    * @return true, åñëè âûñòðåë âûïîëíåí óñïåøíî, èíà÷å false.
    */
    bool shoot(Bullet& bullet) {
        if (is_empty()) {
            cout << "Ìàãàçèí ïóñòîé" << endl;
            return false;
        }
        bullet = bullets.top();
        bullets.pop();
        return true;
    }

    /**
    * Ïåðåãðóçêà îïåðàòîðà âûâîäà äëÿ êëàññà Ammo.
    * 
    * @param os Ïîòîê âûâîäà.
    * @param ammo Ýêçåìïëÿð Ammo. 
    * @return Èçìåíåííûé ïîòîê âûâîäà.
    */
    friend ostream& operator<<(ostream& os, const Ammo& ammo) {
        os << "Ìàãàçèí { Ïàòðîíû: " << ammo.bullets.size() << " }";
        return os;
    }

    /**
    * Ôóíêöèÿ äëÿ ïîëó÷åíèÿ êîëè÷åñòâà ïàòðîíîâ â ìàãàçèíå.
    *
    * @return Âîçâðàùàåò êîëè÷åñòâî ïàòðîíîâ â ìàãàçèíå.
    */
    int get_count() const {
        return bullets.size();
    }

    /**
    * Î÷èùàåò ìàãàçèí îò âñåõ ïàòðîíîâ.
    */
    void clear() {
        while (!bullets.empty()) {
            bullets.pop();
        }
    }

    /**
    * Äîáàâëÿåò ïàòðîí â ìàãàçèí.
    *
    * @param bullet Ïàòðîí, êîòîðûé íóæíî äîáàâèòü.
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
    * Ãåíåðèðóåò ñëó÷àéíûå ïàòðîíû â êîëè÷åñòâå count è äîáàâëÿåò èõ â âåêòîð all_bullets.
    *
    * @param count Êîëè÷åñòâî âñåõ ñîçäàâàåìûõ ïàòðîíîâ.
    */
    void generate_bullets(int count) {
        for (int i = 0; i < count; i++) {
            string type;
            switch (rand() % 3) {
            case 0: type = "Äîçâóêîâûå"; break;
            case 1: type = "Îáû÷íûå"; break;
            case 2: type = "Òðàññèðóþùèå"; break;
            }
            all_bullets.emplace_back(type);
        }
    }

    /**
    * Çàãðóæàåò ìàãàçèí ïàòðîíàì èç âåêòîðà all_bullets, ïîêà îí íå áóäåò çàïîëíåí.
    *
    * Ïðîâåðÿåò, ñîçäàí ëè ìàãàçèí ïåðåä çàãðóçêîé.
    */
    void load_magazine() {
        if (!ammo) {
            cout << "------------- Íîâûé ìàãàçèí --------------" << endl;
            ammo = make_unique<Ammo>();
        }

        while (!all_bullets.empty() && ammo->get_count() < 30) {
            ammo->add_bullet(all_bullets.back());
            all_bullets.pop_back();
        }
        total_reload_ammo++;
    }

    /**
    * Ïðîèñõîäèò âûñòðåë èç îðóæèÿ. Â ñëó÷àå óñïåøíîãî âûñòðåëà,
    * óâåëè÷èâàåòñÿ ñîîòâåòñòâóþùàÿ ñòàòèñòèêà è ïðîèçâîäèòñÿ çâóê ïàòðîíà.
    */
    void shoot() {
        if (!ammo || ammo->is_empty()) {
            cout << "Íåò äîñòóïíûõ ïàòðîíîâ äëÿ ñòðåëüáû!" << endl;
            return;
        }

        Bullet bullet("");
        if (ammo->shoot(bullet)) {
            Sleep(100);
            cout << bullet.sound_effect() << endl;
            total_shots++;
            if (bullet.get_type() == "Äîçâóêîâûå") {
                count_subsonic++;
            }
            else if (bullet.get_type() == "Îáû÷íûå") {
                count_conventional++;
            }
            else if (bullet.get_type() == "Òðàññèðóþùèå") {
                count_tracer++;
            }

            if (ammo->is_empty() && get_remaining_bullets() > 0) {
                cout << "==================== Ðåæèì ïåðåçàðÿäêè ====================" << endl;
                Sleep(2000);
                ammo.reset();
            }
        }
    }

    /**
    * Ïðîâåðÿåò, ïóñò ëè ìàãàçèí èëè îòñóòñòâóåò ëè ìàãàçèí.
    *
    * @return true, åñëè ìàãàçèí ïóñò, èíà÷å false.
    */
    bool is_ammo_empty() const {
        return !ammo || ammo->is_empty();
    }

    /**
    * Âûâîä ñòàòèñòèêè ïîñëå çàâåðøåíèÿ ñòðåëüáû.
    */
    void print_statistics() const {
        cout << endl << string(50, '=') << endl;
        cout << "Ñóììàðíîå êîë-âî âûñòðåëåííûõ ïàòðîíîâ: " << total_shots << endl;
        cout << "Ñóììàðíîå êîë-âî ïåðåçàðÿæåííûûõ ìàãàçèíîâ: " << total_reload_ammo << endl;
        cout << "Êîë-âî 'Äîçâóêîâûõ' ïàòðîíîâ: " << count_subsonic << endl;
        cout << "Êîë-âî 'Îáû÷íûõ' ïàòðîíîâ: " << count_conventional << endl;
        cout << "Êîë-âî 'Òðàññèðóþùèõ' ïàòðîíîâ: " << count_tracer << endl;
    }

    /**
    * Ïåðåãðóçêà îïåðàòîðà âûâîäà äëÿ êëàññà Weapon.
    * 
    * @param os Ïîòîê âûâîäà.
    * @param weapon Ýêçåìïëÿð Weapon.
    * @return Èçìåíåííûé ïîòîê âûâîäà.
    */
    friend ostream& operator<<(ostream& os, const Weapon& weapon) {
        os << "Îðóæèå { Ìàãàçèí: " << (weapon.ammo ? *weapon.ammo : Ammo()) << " }";
        return os;
    }

    /**
    * Âîçâðàùàåò êîëè÷åñòâî îñòàâøèõñÿ ïàòðîíîâ.
    *
    * @return Êîëè÷åñòâî ïàòðîíîâ, îñòàâøèõñÿ â all_bullets.
    */
    int get_remaining_bullets() const {
        return all_bullets.size();
    }

    /**
    * Óïðàâëÿåò ìàãàçèíîì, çàãðóæàÿ åãî, åñëè îí ïóñò.
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
    cout << "Ñóììàðíîå âðåìÿ ïîòðà÷åííîå íà çàðÿäêó ïàòðîíîâ â ìàãàçèí: " << time_reload << " ñåê\n";
    cout << "Ñóììàðíîå âðåìÿ ñòðåëüáû: " << time_shot << " cåê\n";
    return 0;
}
