#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

// ==========================================
// 1. RECORD (Struct)
// ==========================================
struct Pasien {
    string nik;
    string nama;
    int umur;
    string penyakit;
    int prioritas; // 1: Darurat (masuk antrean depan), 2: Reguler
};

// ==========================================
// 2. POINTER (Node Linked List untuk Antrean)
// ==========================================
struct Node {
    Pasien data;
    Node* next;
};

// Variabel Global
Node* head = NULL; 
int jumlahAntrean = 0;

// ==========================================
// 3. ARRAY (Menyimpan Database Rekam Medis)
// ==========================================
const int MAX_DB = 100;
Pasien databasePasien[MAX_DB];
int jumlahDB = 0;


// ==========================================
// 4. REKURSI & 5. RECURRENCE
// Relasi Berulang: W(n) = W(n-1) + 15, dengan W(0) = 0
// Fungsi: Menghitung total waktu tunggu berdasarkan sisa antrean
// ==========================================
int hitungWaktuTunggu(int n) {
    if (n == 0) {
        return 0; // Base case
    }
    return 15 + hitungWaktuTunggu(n - 1); // Rekursi: 15 menit per pasien
}


// ==========================================
// 6. FILE I/O (Menyimpan & Membaca .txt)
// ==========================================
void simpanKeFile(Pasien p) {
    // Mode ios::app untuk append (menambahkan di baris bawah)
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
            // --- PERBAIKAN FATAL: Lewati baris jika kosong agar tidak crash ---
            if (baris.empty()) {
                continue; 
            }
            
            stringstream ss(baris);
            Pasien p;
            getline(ss, p.nik, ',');
            getline(ss, p.nama, ',');
            
            string umurStr;
            getline(ss, umurStr, ',');
            
            // --- PERBAIKAN FATAL: Pastikan umurStr ada isinya sebelum di-stoi ---
            if (!umurStr.empty()) {
                p.umur = stoi(umurStr); 
            } else {
                p.umur = 0; // Beri nilai 0 jika gagal
            }
            
            getline(ss, p.penyakit, ',');
            
            databasePasien[jumlahDB] = p;
            jumlahDB++;
        }
        file.close();
    }
}


// ==========================================
// FUNGSI OPERASI POINTER (Priority Queue)
// ==========================================
void tambahAntrean() {
    Pasien p;
    cout << "\n--- Tambah Antrean Pasien ---\n";
    cout << "NIK         : "; cin >> p.nik;
    cin.ignore(); // Membersihkan buffer
    cout << "Nama        : "; getline(cin, p.nama);
    cout << "Umur        : "; cin >> p.umur;
    cin.ignore();
    cout << "Keluhan     : "; getline(cin, p.penyakit);
    cout << "Status (1: Darurat, 2: Reguler): "; cin >> p.prioritas;

    // Alokasi memori Pointer
    Node* newNode = new Node();
    newNode->data = p;
    newNode->next = NULL;

    // Logika Priority Queue (Darurat ditaruh di depan)
    if (head == NULL || p.prioritas < head->data.prioritas) {
        newNode->next = head;
        head = newNode;
    } else {
        Node* temp = head;
        while (temp->next != NULL && temp->next->data.prioritas <= p.prioritas) {
            temp = temp->next;
        }
        newNode->next = temp->next;
        temp->next = newNode;
    }
    jumlahAntrean++;
    cout << "Pasien berhasil masuk antrean! Estimasi menunggu: " << hitungWaktuTunggu(jumlahAntrean - 1) << " menit.\n";
}

void prosesPasien() {
    if (head == NULL) {
        cout << "\nAntrean kosong. Tidak ada pasien untuk diproses.\n";
        return;
    }
    
    Node* temp = head;
    Pasien p = temp->data;
    head = head->next; // Geser head ke antrean berikutnya
    delete temp; // Dealokasi memori
    
    jumlahAntrean--;
    
    // Simpan ke array database dan file
    if(jumlahDB < MAX_DB) {
        databasePasien[jumlahDB] = p;
        jumlahDB++;
        simpanKeFile(p);
    }
    
    cout << "\nPasien " << p.nama << " telah selesai diperiksa dan disimpan ke rekam medis.\n";
}


// ==========================================
// 7. SORTING (Bubble Sort)
// ==========================================
void urutkanDatabase() {
    // Mengurutkan database array berdasarkan NIK (Ascending)
    for (int i = 0; i < jumlahDB - 1; i++) {
        for (int j = 0; j < jumlahDB - i - 1; j++) {
            if (databasePasien[j].nik > databasePasien[j+1].nik) {
                // Tukar (Swap)
                Pasien temp = databasePasien[j];
                databasePasien[j] = databasePasien[j+1];
                databasePasien[j+1] = temp;
            }
        }
    }
    cout << "\nDatabase berhasil diurutkan berdasarkan NIK.\n";
}


// ==========================================
// 8. SEARCHING (Sequential Search)
// ==========================================
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

void tampilkanAntrean() {
    cout << "\n--- ANTREAN SAAT INI ---\n";
    if (head == NULL) {
        cout << "Antrean kosong.\n";
        return;
    }
    Node* temp = head;
    int no = 1;
    while (temp != NULL) {
        string prioritas = (temp->data.prioritas == 1) ? "[DARURAT]" : "[Reguler]";
        cout << no << ". " << prioritas << " " << temp->data.nama << " - " << temp->data.penyakit << "\n";
        temp = temp->next;
        no++;
    }
}


// ==========================================
// MAIN FUNCTION (Menu Utama)
// ==========================================
int main() {
    // Memuat data dari file saat program pertama kali dijalankan
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
