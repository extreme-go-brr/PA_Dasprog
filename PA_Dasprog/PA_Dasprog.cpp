#include <iostream>
#include <string>
#include <limits>
using namespace std;

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

//APPLY STATUS

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

//RECURSIVE APPLY EFFECTS

void applyEffectsRecursive(StatusNode* list, Character& player) {

    if (list == nullptr) {
        return;
    }

    string nama = list->namaStatus;

    if (nama == "Poison") {

        player.hp -= list->efekValue;

        cout << "[POISON] HP berkurang "
             << list->efekValue << "!\n";
    }

    else if (nama == "Burn") {

        player.hp -= list->efekValue;

        cout << "[BURN] HP berkurang "
             << list->efekValue << "!\n";
    }

    else if (nama == "Freeze") {

        player.skipTurn = true;

        cout << "[FREEZE] Karakter terkena Freeze!\n";
    }

    else if (nama == "Regeneration") {

        player.hp += list->efekValue;

        if (player.hp > player.maxHp) {
            player.hp = player.maxHp;
        }

        cout << "[REGENERATION] HP pulih "
             << list->efekValue << "!\n";
    }

    else if (nama == "Attack Boost") {

        cout << "[ATTACK BOOST] Bonus Attack +"
             << list->efekValue << "!\n";
    }

    // Rekursi ke node berikutnya
    applyEffectsRecursive(list->next, player);
}

//RECURSIVE ATTACK CALCULATION

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

//RECURSIVE REMOVE EXPIRED STATUS

void removeExpiredRecursive(StatusNode*& list) {

    if (list == nullptr) {
        return;
    }

    if (list->durasiTurn <= 0) {

        StatusNode* hapus = list;

        list = list->next;

        cout << hapus->namaStatus
             << " telah berakhir!\n";

        delete hapus;

        removeExpiredRecursive(list);
    }

    else {

        removeExpiredRecursive(list->next);
    }
}

//RECURSIVE FULL CLEANSE

void fullCleanseRecursive(StatusNode*& list) {

    if (list == nullptr) {
        return;
    }

    StatusNode* hapus = list;

    list = list->next;

    cout << "Menghapus " << hapus->namaStatus << "...\n";

    delete hapus;

    fullCleanseRecursive(list);
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

        if (temp->namaStatus == "Attack Boost" ||
            temp->namaStatus == "Regeneration") {

            cout << temp->namaStatus
                 << " | Value: +" << temp->efekValue
                 << " | Duration: " << temp->durasiTurn
                 << " turn\n";

            adaBuff = true;
        }

        temp = temp->next;
    }

    if (!adaBuff) {
        cout << "Tidak ada buff.\n";
    }

    cout << "\n[DEBUFF]\n";

    temp = list;
    bool adaDebuff = false;

    while (temp != nullptr) {

        if (temp->namaStatus == "Poison" ||
            temp->namaStatus == "Burn" ||
            temp->namaStatus == "Freeze") {

            cout << temp->namaStatus
                 << " | Value: " << temp->efekValue
                 << " | Duration: " << temp->durasiTurn
                 << " turn\n";

            adaDebuff = true;
        }

        temp = temp->next;
    }

    if (!adaDebuff) {
        cout << "Tidak ada debuff.\n";
    }

    cout << "===================================\n";
}

//CHARACTER INFORMATION

void displayCharacter(Character player, StatusNode* list) {

    cout << "\n========== CHARACTER INFO ==========\n";

    cout << "Name   : " << player.nama << endl;

    cout << "HP     : " << player.hp
         << "/" << player.maxHp << endl;

    cout << "Attack : "
         << calculateAttack(list, player.attack) << endl;

    cout << "====================================\n";
}

//PROCESS TURN

void processTurn(StatusNode*& list,
                 Character& player,
                 Enemy& enemy,
                 int& turn) {

    cout << "\n====================================\n";
    cout << "              TURN " << turn << endl;
    cout << "====================================\n";

    player.skipTurn = false;

    // Mengaplikasikan seluruh efek status
    cout << "\n--- Applying Status Effects ---\n";

    applyEffectsRecursive(list, player);

    if (player.hp < 0) {
        player.hp = 0;
    }

    // Menghitung jumlah debuff
    cout << "\nActive Debuffs: "
         << countDebuffsRecursive(list) << endl;

    // Mengecek Freeze
    if (player.skipTurn) {

        cout << "\nKnight kehilangan giliran karena Freeze!\n";
    }

    // Pemain menyerang jika tidak terkena Freeze
    else if (player.hp > 0 && enemy.hp > 0) {

        int damage = calculateAttack(list, player.attack);

        enemy.hp -= damage;

        if (enemy.hp < 0) {
            enemy.hp = 0;
        }

        cout << "\n--- COMBAT ---\n";

        cout << player.nama << " menyerang "
             << enemy.nama << "!\n";

        cout << "Total Damage : " << damage << endl;

        cout << enemy.nama << " HP: "
             << enemy.hp << "/" << enemy.maxHp << endl;
    }

    // Musuh menyerang jika masih hidup
    if (enemy.hp > 0 && player.hp > 0) {

        player.hp -= enemy.attack;

        if (player.hp < 0) {
            player.hp = 0;
        }

        cout << "\n" << enemy.nama
             << " menyerang balik!\n";

        cout << "Damage diterima: "
             << enemy.attack << endl;
    }

    // Mengurangi durasi seluruh status
    StatusNode* temp = list;

    while (temp != nullptr) {
        temp->durasiTurn--;
        temp = temp->next;
    }

    // Menghapus status yang durasinya habis
    cout << "\n--- Expired Status Check ---\n";

    removeExpiredRecursive(list);

    // Menampilkan informasi terbaru
    displayCharacter(player, list);

    displayStatus(list);

    // Hasil pertarungan
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
        100,
        100,
        20,
        false
    };

    Enemy enemy = {
        "Goblin",
        100,
        100,
        8
    };

    int pilihan;
    int turn = 1;

    // Status awal
    applyStatus(statusList, "Poison", 5, 3);
    applyStatus(statusList, "Burn", 10, 2);
    applyStatus(statusList, "Attack Boost", 10, 3);
    applyStatus(statusList, "Regeneration", 5, 4);
    applyStatus(statusList, "Freeze", 0, 1);

    do {

        cout << "\n====================================\n";
        cout << "       RPG STATUS EFFECT SYSTEM\n";
        cout << "====================================\n";

        displayCharacter(player, statusList);

        cout << "\nEnemy: " << enemy.nama << endl;
        cout << "Enemy HP: " << enemy.hp
             << "/" << enemy.maxHp << endl;

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
                    cout << "Knight sudah kalah!\n";
                }

                else if (enemy.hp <= 0) {
                    cout << "Musuh sudah dikalahkan!\n";
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