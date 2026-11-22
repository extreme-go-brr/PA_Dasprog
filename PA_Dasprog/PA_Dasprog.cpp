#include <iostream>
#include <string>
#include <limits>
using namespace std;

// POIN 1: POINTER/ALOKASI DINAMIS === STRUCTURE LINKED LIST

//STRUCT STATUS NODE

struct StatusNode {
    string namaStatus;
    int efekValue;
    int durasiTurn;
    StatusNode* next;
};

//STRUCT CHARACTER

struct Character {
    string nama;
    int hp;
    int maxHp;
    int attack;
    bool skipTurn;
};

//STRUCT ENEMY

struct Enemy {
    string nama;
    int hp;
    int maxHp;
    int attack;
};

//INPUT VALIDATION


int inputAngka(string pesan) {
    int angka;

    cout << pesan;

    while (!(cin >> angka)) {
        cout << "Input harus berupa angka!\n";
        cout << pesan;

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    return angka;
}

//POIN 2: APPLY STATUS

void applyStatus(StatusNode*& list, string nama, int val, int turn) {

    if (turn <= 0 || val < 0) {
        cout << "Nilai efek atau durasi tidak valid!\n";
        return;
    }

    if (nama != "Poison" &&
        nama != "Burn" &&
        nama != "Freeze" &&
        nama != "Attack Boost" &&
        nama != "Regeneration") {

        cout << "Nama status tidak dikenal!\n";
        return;
    }

    StatusNode* baru = new StatusNode;

    baru->namaStatus = nama;
    baru->efekValue = val;
    baru->durasiTurn = turn;
    baru->next = nullptr;

    if (list == nullptr) {
        list = baru;
    }
    else {
        StatusNode* temp = list;

        while (temp->next != nullptr) {
            temp = temp->next;
        }

        temp->next = baru;
    }

    cout << nama << " berhasil diterapkan!\n";
}

//POIN 3: RECURSIVE APPLY EFFECTS

void applyEffect(StatusNode* list, Character& player) {

    if (list == nullptr) {
        return;
    }

    string statusEffect = list->namaStatus;

    if (statusEffect == "Poison") {
        player.hp -= list->efekValue;
        cout << "[POISON] HP berkurang "
            << list->efekValue << "!\n";
    }

    else if (statusEffect == "Burn") {
        player.hp -= list->efekValue;
        cout << "[BURN] HP berkurang "
            << list->efekValue << "!\n";
    }

    else if (statusEffect == "Freeze") {
        player.skipTurn = true;
        cout << "[FREEZE] Karakter terkena Freeze!\n";
    }

    else if (statusEffect == "Regeneration") {
        player.hp += list->efekValue;
        if (player.hp > player.maxHp) {
            player.hp = player.maxHp;
        }
        cout << "[REGENERATION] HP pulih "
            << list->efekValue << "!\n";
    }

    else if (statusEffect == "Attack Boost") {
        cout << "[ATTACK BOOST] Bonus Attack +"
            << list->efekValue << "!\n";
    }

	if (player.hp < 0) {
		player.hp = 0;
	}
    // next rekursi
    applyEffect(list->next, player);
}

//RECURSIVE ATTACK CALCULATION0

int calculateAttack(StatusNode* list, int baseAttack) {

    if (list == nullptr) {
        return baseAttack;
    }
    int bonus = 0;
    if (list->namaStatus == "Attack Boost") {
        bonus = list->efekValue;
    }
    return bonus + calculateAttack(list->next, baseAttack);
}

//RECURSIVE DEBUFF COUNTER

int countDebuffsRecursive(StatusNode* list) {

    if (list == nullptr) {
        return 0;
    }
    int jumlah = 0;
    if (list->namaStatus == "Poison" ||
        list->namaStatus == "Burn" ||
        list->namaStatus == "Freeze") {
        jumlah = 1;
    }

    return jumlah + countDebuffsRecursive(list->next);
}

// PENGURANGAN DURASI STATUS
void decreaseDurationRecursive(StatusNode* list) {
    if (list == nullptr) {
        return;
    }

    list->durasiTurn--;
    decreaseDurationRecursive(list->next);
}

//RECURSIVE REMOVE EXPIRED STATUS

void removeExpiredRecursive(StatusNode*& list) {
    if (list == nullptr) {
        return;
    }
        
    removeExpiredRecursive(list->next);
   
    if (list->durasiTurn <= 0) {
        StatusNode* hapus = list;
        list = list->next;
        cout << hapus->namaStatus << " telah berakhir (memori didealokasikan).\n";
        delete hapus;
    }
}

//RECURSIVE FULL CLEANSE

void fullCleanseRecursive(StatusNode*& list) {
    if (list == nullptr) {
        return;
    }
    fullCleanseRecursive(list->next);
    StatusNode* hapus = list;
    list = nullptr;
    cout << "Menghapus " << hapus->namaStatus << "...\n";
    delete hapus;
}

//DISPLAY STATUS

void displayStatus(StatusNode* list) {

    if (list == nullptr) {
        cout << "\nTidak ada status aktif.\n";
        return;
    }
    cout << "\n========== ACTIVE STATUS ==========\n";
    cout << "\n[BUFF]\n";
    StatusNode* temp = list;
    bool adaBuff = false;
    while (temp != nullptr) {
        if (temp->namaStatus == "Attack Boost" ||temp->namaStatus == "Regeneration") {
            cout << temp->namaStatus
                << " | Value: +" << temp->efekValue
                << " | Duration: " << temp->durasiTurn
                << " turn\n";
            adaBuff = true;
        }
        temp = temp->next;
    }
    if (!adaBuff) cout << "Tidak ada buff.\n";
    
    cout << "\n[DEBUFF]\n";
    temp = list;
    bool adaDebuff = false;
    while (temp != nullptr) {
        if (temp->namaStatus == "Poison" || temp->namaStatus == "Burn" || temp->namaStatus == "Freeze") {
            cout << temp->namaStatus
                << " | Value: " << temp->efekValue
                << " | Duration: " << temp->durasiTurn
                << " turn\n";
            adaDebuff = true;
        }
        temp = temp->next;
    }
    if (!adaDebuff) cout << "Tidak ada debuff.\n";

    cout << "===================================\n";
}

void displayCharacter(Character player, StatusNode* list) {
    cout << "\n========== CHARACTER INFO ==========\n";
    cout << "Name   : " << player.nama << endl;
    cout << "HP     : " << player.hp << "/" << player.maxHp << endl;
    cout << "Attack : " << calculateAttack(list, player.attack) << endl;
	cout << "Debuff : " << countDebuffsRecursive(list) << endl;
    cout << "====================================\n";
}

void processTurn(StatusNode*& list, Character& player, Enemy& enemy, int& turn) {
    cout << " TURN " << turn << endl;
    player.skipTurn = false;

    cout << "\n-Applying Status Effects-\n";
    applyEffect(list, player);
    cout << "\nTotal Active Debuffs: " << countDebuffsRecursive(list) << endl;
    
    if (player.hp <= 0) {
        cout << "\nKnight has been defeated!\n";
    }
    
    else if (player.skipTurn) {
        cout << "\nKnight lost turn because of Freeze!\n";
    }
    
    else if (player.hp > 0 && enemy.hp > 0) {
        int damage = calculateAttack(list, player.attack);
        enemy.hp -= damage;
        if (enemy.hp < 0) {
            enemy.hp = 0;
        }
        cout << "\n--- COMBAT ---\n";
        cout << player.nama << " menyerang " << enemy.nama << "!\n";
        cout << "Total Damage : " << damage << endl;
        cout << enemy.nama << " HP: " << enemy.hp << "/" << enemy.maxHp << endl;
    }

    if (enemy.hp > 0 && player.hp > 0) {
        player.hp -= enemy.attack;
        if (player.hp < 0) {
            player.hp = 0;
        }
        cout << "\n" << enemy.nama << " menyerang balik!\n";

        cout << "Damage diterima: " << enemy.attack << endl;
    }
    
    cout << "\n--- Updating & Expiring Status ---\n";
    decreaseDurationRecursive(list);
    removeExpiredRecursive(list);

    displayCharacter(player, list);
    displayStatus(list);

    if (player.hp <= 0) {
        cout << "\nKNIGHT HAS BEEN DEFEATED!\n";
    }
    else if (enemy.hp <= 0) {
        cout << "\nVICTORY! GOBLIN HAS BEEN DEFEATED!\n";
    }

    turn++;
}


//MAIN PROGRAM

int main() {

    StatusNode* statusList = nullptr;

    Character player = {
        "Knight",
        180,
        180,
        15,
        false
    };

    Enemy enemy = {
        "Goblin",
        120,
        120,
        12
    };

    int pilihan;
    int turn = 1;

    // Status awal
    applyStatus(statusList, "Poison", 7, 4);
    applyStatus(statusList, "Burn", 10, 3);
    applyStatus(statusList, "Attack Boost", 10, 3);
    applyStatus(statusList, "Regeneration", 5, 2);
    applyStatus(statusList, "Freeze", 0, 1);

    do {

        cout << "\n====================================\n";
        cout << "       RPG STATUS EFFECT SYSTEM\n";
        cout << "====================================\n";

        displayCharacter(player, statusList);

        cout << "\nEnemy: " << enemy.nama << endl;
        cout << "Enemy HP: " << enemy.hp << "/" << enemy.maxHp << endl;
        cout << "Enemy Attack: " << enemy.attack << endl;

        cout << "\n--------------- MENU ---------------\n";

        cout << "1. Display Active Status\n";
        cout << "2. Process Turn & Combat\n";
        cout << "3. Apply New Status\n";
        cout << "4. Full Cleanse\n";
        cout << "0. Exit\n";

        cout << "------------------------------------\n";

        pilihan = inputAngka("Choose option: ");
        switch (pilihan) {

        case 1:
            displayStatus(statusList);
            break;

        case 2:
            if (player.hp <= 0) {
                cout << "Knight defeated!\n";
            }
            else if (enemy.hp <= 0) {
                cout << "Enemy has been defeated!\n";
            }
            else {
                processTurn(statusList, player, enemy, turn);
            }
            break;

        case 3: {
            string nama;
            int val, durasi;

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nNama Status: ";
            getline(cin, nama);

            val = inputAngka("Nilai Efek: ");
            durasi = inputAngka("Durasi Turn: ");

            applyStatus(statusList, nama, val, durasi);
            break;
        }

        case 4:
            fullCleanseRecursive(statusList);
            cout << "Semua status berhasil dibersihkan!\n";
            break;

        case 0:
            cout << "Exiting Game...\n";
            break;

        default:
            cout << "Pilihan tidak valid!\n";
        }

    } while (pilihan != 0);

    cout << "\n--- MEMORY CLEANUP ---\n";
    fullCleanseRecursive(statusList);
    cout << "All allocated memory has been released.\n";
    return 0;
}