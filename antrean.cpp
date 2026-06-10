#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

// 1. RECORD (Struct)
struct Pasien {
    string nik;
    string nama;
    int umur;
    string penyakit;
    int prioritas;
};

// 2. POINTER
struct AntreanPasien {
    Pasien data;
    AntreanPasien* next;
};

AntreanPasien* head = NULL; 
int jumlahAntrean = 0;

// 3. ARRAY
const int MAX_DB = 100;
Pasien databasePasien[MAX_DB];
int jumlahDB = 0;


// 4. REKURSI & 5. RECURRENCE
int hitungWaktuTunggu(int posisi) {
    if (posisi == 0) {
        return 0; // Base case: Pasien pertama langsung dilayani
    }
    return 15 + hitungWaktuTunggu(posisi - 1); // Rekursi: 15 menit per pasien di depannya
}

// 6. FILE I/O
void simpanKeFile(Pasien p) {
    ofstream file("rekam_medis.txt", ios::app);
    if (file.is_open()) {
        file << p.nik << "," << p.nama << "," << p.umur << "," << p.penyakit << "\n";
        file.close();
    } else {
        cout << "Gagal membuka file untuk menyimpan data!\n";
    }
}

void bacaFileDatabase() {
    ifstream file("rekam_medis.txt");
    string baris, token;
    jumlahDB = 0;
    
    if (file.is_open()) {
        while (getline(file, baris) && jumlahDB < MAX_DB) {
            stringstream ss(baris);
            Pasien p;
            getline(ss, p.nik, ',');
            getline(ss, p.nama, ',');
            
            string umurStr;
            getline(ss, umurStr, ',');
            if (!umurStr.empty()) p.umur = stoi(umurStr); 
            
            getline(ss, p.penyakit, ',');
            
            databasePasien[jumlahDB] = p;
            jumlahDB++;
        }
        file.close();
    }
}

void tambahAntrean() {
    Pasien p;
    cout << "\n--- Tambah Antrean Pasien ---\n";
    cout << "NIK         : "; cin >> p.nik;
    cin.ignore();
    cout << "Nama        : "; getline(cin, p.nama);
    cout << "Umur        : "; cin >> p.umur;
    cin.ignore();
    cout << "Keluhan     : "; getline(cin, p.penyakit);
    cout << "Status (1: Darurat, 2: Reguler): "; cin >> p.prioritas;

    AntreanPasien* pasienBaru = new AntreanPasien();
    pasienBaru->data = p;
    pasienBaru->next = NULL;

    if (head == NULL || p.prioritas < head->data.prioritas) {
        pasienBaru->next = head;
        head = pasienBaru;
    } else {
        AntreanPasien* temp = head;
        while (temp->next != NULL && temp->next->data.prioritas <= p.prioritas) {
            temp = temp->next;
        }
        pasienBaru->next = temp->next;
        temp->next = pasienBaru;
    }
    jumlahAntrean++;
    cout << "\n[!] Pasien berhasil masuk antrean.\n";
    cout << "[!] Silakan cek Menu 2 untuk melihat detail estimasi waktu panggil.\n";
}

void tampilkanAntrean() {
    cout << "\n--- ANTREAN SAAT INI ---\n";
    if (head == NULL) {
        cout << "Antrean kosong. Tidak ada pasien.\n";
        return;
    }
    
    AntreanPasien* temp = head;
    int posisi = 0; 
    
    while (temp != NULL) {
        string prioritas = (temp->data.prioritas == 1) ? "[DARURAT]" : "[REGULER]";
        
        int waktuTunggu = hitungWaktuTunggu(posisi);
        
        cout << posisi + 1 << ". " << prioritas << " " << temp->data.nama 
             << " (" << temp->data.penyakit << ")";
             
        if (posisi == 0) {
            cout << " -> Giliran saat ini\n";
        } else {
            cout << " -> Estimasi tunggu: " << waktuTunggu << " menit\n";
        }
        
        temp = temp->next;
        posisi++;
    }
}

void prosesPasien() {
    if (head == NULL) {
        cout << "\nAntrean kosong. Tidak ada pasien untuk diproses.\n";
        return;
    }
    
    AntreanPasien* temp = head;
    Pasien p = temp->data;
    head = head->next;
    delete temp;
    
    jumlahAntrean--;
    
    if(jumlahDB < MAX_DB) {
        databasePasien[jumlahDB] = p;
        jumlahDB++;
        simpanKeFile(p);
    }
    
    cout << "\n[SUKSES] Pasien " << p.nama << " telah selesai diperiksa.\n";
    cout << "Data rekam medis berhasil diarsipkan ke database.\n";
}

// 7. SORTING (Bubble Sort)

void urutkanDatabase() {
    for (int i = 0; i < jumlahDB - 1; i++) {
        for (int j = 0; j < jumlahDB - i - 1; j++) {
            if (databasePasien[j].nik > databasePasien[j+1].nik) {
                Pasien temp = databasePasien[j];
                databasePasien[j] = databasePasien[j+1];
                databasePasien[j+1] = temp;
            }
        }
    }
    cout << "\n[SUKSES] Database berhasil diurutkan berdasarkan NIK.\n";
}

// 8. SEARCHING (Sequential Search)

void cariRekamMedis() {
    string targetNIK;
    cout << "\nMasukkan NIK Pasien yang dicari: ";
    cin >> targetNIK;
    
    bool ditemukan = false;
    for (int i = 0; i < jumlahDB; i++) {
        if (databasePasien[i].nik == targetNIK) {
            cout << "\n-- REKAM MEDIS DITEMUKAN --\n";
            cout << "NIK     : " << databasePasien[i].nik << "\n";
            cout << "Nama    : " << databasePasien[i].nama << "\n";
            cout << "Umur    : " << databasePasien[i].umur << " tahun\n";
            cout << "Riwayat : " << databasePasien[i].penyakit << "\n";
            ditemukan = true;
            break;
        }
    }
    
    if (!ditemukan) {
        cout << "\nData rekam medis dengan NIK " << targetNIK << " tidak ditemukan.\n";
    }
}

void tampilkanDatabase() {
    cout << "\n--- DATABASE REKAM MEDIS ---\n";
    if (jumlahDB == 0) {
        cout << "Database kosong.\n";
        return;
    }
    for (int i = 0; i < jumlahDB; i++) {
        cout << i+1 << ". " << databasePasien[i].nik << " | " << databasePasien[i].nama 
             << " | " << databasePasien[i].penyakit << "\n";
    }
}

int main() {
    bacaFileDatabase();
    
    int pilihan;
    do {
        cout << "\n=====================================\n";
        cout << "   SISTEM KLINIK & REKAM MEDIS\n";
        cout << "=====================================\n";
        cout << "1. Daftar Antrean Pasien (Queue)\n";
        cout << "2. Lihat Antrean Saat Ini\n";
        cout << "3. Proses Pemeriksaan Pasien\n";
        cout << "4. Lihat Database Rekam Medis (Array)\n";
        cout << "5. Urutkan Database berdasarkan NIK (Sort)\n";
        cout << "6. Cari Rekam Medis Pasien (Search)\n";
        cout << "0. Keluar\n";
        cout << "Pilih menu: ";
        cin >> pilihan;
        
        switch(pilihan) {
            case 1: tambahAntrean(); break;
            case 2: tampilkanAntrean(); break;
            case 3: prosesPasien(); break;
            case 4: tampilkanDatabase(); break;
            case 5: urutkanDatabase(); tampilkanDatabase(); break;
            case 6: cariRekamMedis(); break;
            case 0: cout << "Keluar dari program. Terima kasih!\n"; break;
            default: cout << "Pilihan tidak valid!\n";
        }
    } while(pilihan != 0);

    return 0;
}
