/*
 * ==========================================
 * PROGRAM MANAJEMEN KLINIK & REKAM MEDIS
 * ==========================================
 * Nama      : Fauzan Abdillah Batra
 * NIM       : 123250176
 * Prodi     : Jurusan Informatika
 * Instansi  : UPN Veteran Yogyakarta
 * Pengampu  : Bapak Rifki
 * ==========================================
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

// 1. RECORD
struct Pasien {
    string nik;
    string nama;
    int umur;
    string penyakit;
    int prioritas; // 1: Darurat, 2: Reguler
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
        return 0; 
    }
    return 15 + hitungWaktuTunggu(posisi - 1); 
}

// 6. FILE I/O
void simpanKeFile(Pasien p) {
    ofstream file("rekam_medis.txt", ios::app);
    if (file.is_open()) {
        file << p.nik << "," << p.nama << "," << p.umur << "," << p.penyakit << "\n";
        file.close();
    }
}

void tulisUlangFile() {
    ofstream file("rekam_medis.txt");
    if (file.is_open()) {
        for (int i = 0; i < jumlahDB; i++) {
            file << databasePasien[i].nik << "," 
                 << databasePasien[i].nama << "," 
                 << databasePasien[i].umur << "," 
                 << databasePasien[i].penyakit << "\n";
        }
        file.close();
    }
}

void bacaFileDatabase() {
    ifstream file("rekam_medis.txt");
    string baris;
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
    cout << "\n--- AMBIL NOMOR ANTREAN ---\n";
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
    cout << "\n[!] Berhasil masuk antrean. Silakan cek menu antrean saat ini.\n";
}

void tampilkanAntrean() {
    cout << "\n--- ANTREAN KLINIK SAAT INI ---\n";
    if (head == NULL) {
        cout << "Antrean sedang kosong.\n";
        return;
    }
    
    AntreanPasien* temp = head;
    int posisi = 0; 
    
    while (temp != NULL) {
        string prioritas = (temp->data.prioritas == 1) ? "[DARURAT]" : "[REGULER]";
        int waktuTunggu = hitungWaktuTunggu(posisi);
        
        cout << posisi + 1 << ". " << prioritas << " " << temp->data.nama;
        if (posisi == 0) cout << " -> Silakan masuk ruangan\n";
        else cout << " -> Estimasi tunggu: " << waktuTunggu << " menit\n";
        
        temp = temp->next;
        posisi++;
    }
}

void prosesPasien() {
    if (head == NULL) {
        cout << "\n[!] Antrean kosong.\n";
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
    cout << "\n[SUKSES] " << p.nama << " selesai diperiksa. Data masuk rekam medis.\n";
}

// 7. SORTING & 8. SEARCHING
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
    tulisUlangFile();
    cout << "\n[SUKSES] Database diurutkan berdasarkan NIK.\n";
}

void cariRekamMedis() {
    string targetNIK;
    cout << "\nMasukkan NIK: "; cin >> targetNIK;
    
    bool ditemukan = false;
    for (int i = 0; i < jumlahDB; i++) {
        if (databasePasien[i].nik == targetNIK) {
            cout << "\n-- REKAM MEDIS DITEMUKAN --\n";
            cout << "NIK     : " << databasePasien[i].nik << "\n";
            cout << "Nama    : " << databasePasien[i].nama << "\n";
            cout << "Umur    : " << databasePasien[i].umur << " tahun\n";
            cout << "Riwayat : " << databasePasien[i].penyakit << "\n";
            ditemukan = true; break;
        }
    }
    if (!ditemukan) cout << "\n[!] Data dengan NIK " << targetNIK << " tidak ditemukan.\n";
}

void tampilkanDatabase() {
    cout << "\n--- DATABASE REKAM MEDIS ---\n";
    if (jumlahDB == 0) { cout << "Database kosong.\n"; return; }
    for (int i = 0; i < jumlahDB; i++) {
        cout << i+1 << ". " << databasePasien[i].nik << " | " << databasePasien[i].nama 
             << " | " << databasePasien[i].penyakit << "\n";
    }
}

void editRekamMedis() {
    string targetNIK;
    int indexTarget = -1;
    int pilihan;
    
    cout << "\n--- EDIT DATA REKAM MEDIS ---\n";
    cout << "Masukkan NIK Pasien yang ingin diedit: "; 
    cin >> targetNIK;
    
    for (int i = 0; i < jumlahDB; i++) {
        if (databasePasien[i].nik == targetNIK) {
            indexTarget = i;
            break;
        }
    }

    Pasien &target = databasePasien[indexTarget];
    
    if (indexTarget == -1) {
        cout << "\n[!] Data dengan NIK " << targetNIK << " tidak ditemukan.\n";
    } else {
        do {
            cout << "\n--- Data Saat Ini ---" << endl;
            cout << "1. NIK              : " << target.nik << " (Terkunci)" << endl;
            cout << "2. Nama             : " << target.nama << endl;
            cout << "3. Umur             : " << target.umur << " tahun" << endl;
            cout << "4. Riwayat Penyakit : " << target.penyakit << endl;
            cout << "0. Selesai Edit" << endl;
            cout << "Pilih field yang ingin diubah: ";
            cin >> pilihan;

            switch (pilihan) {
                case 1: 
                    cout << "[!] NIK adalah identitas utama, tidak boleh diubah!" << endl; 
                    break;
                case 2: 
                    cout << "Nama baru: "; 
                    cin.ignore();
                    getline(cin, target.nama); 
                    break;
                case 3: 
                    cout << "Umur baru: "; 
                    cin >> target.umur; 
                    break;
                case 4: 
                    cout << "Riwayat Penyakit baru: "; 
                    cin.ignore();
                    getline(cin, target.penyakit); 
                    break;
                case 0: 
                    cout << "Menyimpan perubahan..." << endl; 
                    break;
                default: 
                    cout << "Pilihan tidak valid." << endl;
            }
        } while (pilihan != 0);
        
        tulisUlangFile();
        cout << "\n[SUKSES] Data rekam medis berhasil diperbarui ke database.\n";
    }
}

void hapusRekamMedis() {
    string targetNIK;
    cout << "\nMasukkan NIK Pasien yang ingin dihapus: "; cin >> targetNIK;

    for (int i = 0; i < jumlahDB; i++) {
        if (databasePasien[i].nik == targetNIK) {
            for (int j = i; j < jumlahDB - 1; j++) {
                databasePasien[j] = databasePasien[j+1];
            }
            jumlahDB--;
            tulisUlangFile();
            cout << "\n[SUKSES] Data rekam medis berhasil dihapus.\n";
            return;
        }
    }
    cout << "\n[!] NIK tidak ditemukan.\n";
}

void menuUser() {
    int pilihan;
    do {
        cout << "\n=== MENU PASIEN / PENGUNJUNG ===\n";
        cout << "1. Ambil Nomor Antrean\n";
        cout << "2. Lihat Layar Antrean\n";
        cout << "3. Cek Riwayat Rekam Medis (Cari NIK)\n";
        cout << "0. Kembali ke Menu Utama\n";
        cout << "Pilih: "; cin >> pilihan;
        
        switch(pilihan) {
            case 1: tambahAntrean(); break;
            case 2: tampilkanAntrean(); break;
            case 3: cariRekamMedis(); break;
            case 0: break;
            default: cout << "Pilihan tidak valid.\n";
        }
    } while(pilihan != 0);
}

void menuAdmin() {
    string username, password;
    int kesempatan = 3;
    bool loginSukses = false;

    while (kesempatan > 0) {
        cout << "\n=== LOGIN DOKTER / ADMIN ===\n";
        cout << "Username : "; cin >> username;
        cout << "Password : "; cin >> password;

        if (username == "admin" && password == "123") {
            loginSukses = true;
            cout << "\n[SUKSES] Login berhasil. Selamat datang, Admin!\n";
            break;
        } else {
            kesempatan--;
            cout << "[!] Login gagal. Username atau password salah.\n";
            if (kesempatan > 0) {
                cout << "[!] Sisa percobaan: " << kesempatan << " kali.\n";
            }
        }
    }

    if (!loginSukses) {
        cout << "\n[!]Anda telah gagal login 3 kali.\n";
        cout << "Mengembalikan ke Menu Utama...\n";
        return; 
    }

    int pilihan;
    do {
        cout << "\n=== MENU DOKTER / ADMIN ===\n";
        cout << "1. Panggil & Proses Pasien Antrean\n";
        cout << "2. Lihat Seluruh Database\n";
        cout << "3. Urutkan Database (Berdasarkan NIK)\n";
        cout << "4. Edit Data Pasien (Update)\n";
        cout << "5. Hapus Data Pasien (Delete)\n";
        cout << "0. Log Out (Kembali)\n";
        cout << "Pilih: "; cin >> pilihan;
        
        switch(pilihan) {
            case 1: prosesPasien(); break;
            case 2: tampilkanDatabase(); break;
            case 3: urutkanDatabase(); tampilkanDatabase(); break;
            case 4: editRekamMedis(); break;
            case 5: hapusRekamMedis(); break;
            case 0: cout << "Berhasil Log Out.\n"; break;
            default: cout << "Pilihan tidak valid.\n";
        }
    } while(pilihan != 0);
}

int main() {
    bacaFileDatabase(); 
    
    int pilihan;
    do {
        cout << "\n=====================================\n";
        cout << "   SISTEM KLINIK & REKAM MEDIS\n";
        cout << "=====================================\n";
        cout << "1. Masuk Sebagai Pasien\n";
        cout << "2. Masuk Sebagai Admin/Dokter\n";
        cout << "0. Tutup Aplikasi\n";
        cout << "Pilih hak akses: "; cin >> pilihan;
        
        switch(pilihan) {
            case 1: menuUser(); break;
            case 2: menuAdmin(); break;
            case 0: cout << "Menutup sistem. Terima kasih!\n"; break;
            default: cout << "Pilihan tidak valid!\n";
        }
    } while(pilihan != 0);

    return 0;
}
