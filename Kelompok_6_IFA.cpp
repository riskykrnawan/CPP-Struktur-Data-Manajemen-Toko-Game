
#include <iostream>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <ctime>
#include <math.h> 

using namespace std;



struct Account {
	string _id, userName, nama, pass, email;
	string isiKeranjang[100];
	int jumlahKeranjang;
};	
struct Node_1 {
	struct Account data;
	struct Node_1 *next;	
	struct Node_1 *prev;	
};

Node_1 *head = NULL, *tail = NULL, *del;

struct Game{
	string ID, namaGame, tanggalRilis;
	int rating, stok, harga;
};


//ini array sementara untuk nyimpan data gamenya, jadi ini nanti di convert dari array ke linked list
Game games[100];


struct Node_2 {
	struct Game data;
	struct Node_2 *next;
	struct Node_2 *prev;
};

Node_2 *TOP = NULL, *END = NULL, *del_2;

struct History {
	string idPembelian, userName, tanggalBeli;
	int totalHarga;
};

struct Node_3 {
	struct History data;
	struct Node_3 *next;
};

Node_3 *FRONT = NULL, *REAR = NULL, *del_3;

int accountCount = 0;
int gameCount = 0;
int historyCount = 0;


void hubungiAdmin();
void saveToCSV(Node_2 *TOP);
void csvToArr(Node_2 **TOP, Node_2 **END, int &gameCount);
void convertListToArr( Node_2 **TOP, Game *games );
void convertArrToList(Node_2 **TOP, Node_2 **END, Game games);

void auth(struct Node_1 *head, string userName, string pass);
void menuLogin();
void menuUser(string userName);
void menuAdmin();
void menuSort();

void dataGame();
void dataUser();
void dataHistory();

void addUser(Node_1 **head, Node_1 **tail, int &accountCount);
void lihatDataUser(Node_1 *head);
void ubahDataUser( Node_1 *head, string selectedID );
Node_1 *hapusDataUser(Node_1 *head, Node_1 *tail, string selectedID);

void lihatDataGame(Node_2 *TOP);
void lihatDataSatuGame( Node_2 *TOP, string SelectedID );
void addDataGameLast(Node_2 **TOP, Node_2 **END, int &gameCount);
void ubahDataGame(Node_2 *TOP, string SelectedID);
Node_2 *hapusDataGame(Node_2 *TOP, Node_2 *END, string SelectedID);
void shellSort( Game *games, int &arrSize, string orderItem, string orderMethod );
int jumpSearch(Game *arr, string x, int n);
void lihatDataSatuGameArr( Game *games );

//menu user
void lihatKeranjang(Node_1 *head, Node_2 *TOP, string userName);
void tambahKeranjang(Node_1 *head, Node_2 *TOP, string userName);
void hapusIsiKeranjang (Node_1 *head, Node_2 *TOP, string userName);
void checkout( Node_1 *head, Node_2 *TOP, Node_3 **FRONT, string userName );
void lihatHistory(Node_3 *FRONT);
Node_3 *hapusDataHistory(Node_3 *FRONT, Node_3 *REAR);


int main() {
	//masukkan data csv ke linked list
	csvToArr(&TOP, &END, gameCount);

	//login 
	menuLogin();
}



string timeNow() {
	string currTime;
	time_t now = time(0);
	tm *ltm = localtime(&now);
	currTime = to_string(1900+ltm->tm_year) + '/' + to_string(1+ltm->tm_mon) + '/' + to_string(ltm->tm_mday);
	return currTime;
}

void saveToCSV(Node_2 *TOP) {	
	Node_2 *temp = new Node_2;
	temp = TOP;
	if (temp == NULL) {
		cout << "\nData Masih Kosong !! " << endl << endl;
		return;
	} else {
		ofstream f("game.csv");
		while ( temp != NULL ) {
			f<<temp->data.ID<<","<<temp->data.namaGame<<","<<temp->data.tanggalRilis<<","<<temp->data.rating<<","<<temp->data.stok<<","<<temp->data.harga<<"\n";
			temp = temp->next;
		}
		f.close();	
	}
}



void csvToArr(Node_2 **TOP, Node_2 **END, int &gameCount) {
	
	Node_2 *newNode;
	
	ifstream input("game.csv");
	string line, text;
	int i;
	
	string arr[500];
	while(getline(input, line)){
		std::stringstream ss(line);
		while(getline(ss, text, ',')){
			arr[i] = text;
			i++;
		}
		gameCount++;
	}
	
	
	
	for ( i = 0; i<gameCount; i++ ) {
		//karena ini ada 6 kolom, maka dikali 6, jadi ketemu lagi di kolom yg sama di kelipatan 6
		games[i].ID = arr[0+i*6];
		games[i].namaGame = arr[1+i*6];
		games[i].tanggalRilis = arr[2+i*6];
		games[i].rating = stoi(arr[3+i*6]);
		games[i].stok = stoi(arr[4+i*6]);
		games[i].harga = stoll(arr[5+i*6]);
	}
	
	
	shellSort(&games[0], gameCount, "3", "1");	
	for (int i = 0; i <gameCount; i++){
		convertArrToList(TOP, END, games[i]);
	}
		
}



void convertListToArr( Node_2 **TOP, Game *games ) {
	int index = 0;
	Node_2 *temp = *TOP;
	while (temp != NULL) {
		games[index].ID = temp->data.ID;
		games[index].namaGame = temp->data.namaGame;
		games[index].tanggalRilis = temp->data.tanggalRilis;
		games[index].rating = temp->data.rating;
		games[index].stok = temp->data.stok;
		games[index].harga = temp->data.harga;
		index++;
		temp = temp->next;
	}
}


	
//fungsi ini untuk ngubah array awal jadi list.	
void convertArrToList(Node_2 **TOP, Node_2 **END, Game games) {
	Node_2 *temp = new Node_2;
	Node_2 *ptr;
	temp->data.ID = games.ID;
	temp->data.namaGame = games.namaGame;
	temp->data.tanggalRilis = games.tanggalRilis;
	temp->data.rating = games.rating;
	temp->data.stok = games.stok;
	temp->data.harga = games.harga;
	
	temp->data.stok = games.stok;
	*END = temp;
	temp->next = NULL;
	if (*TOP == NULL) *TOP = temp;
	else {
		ptr = *TOP;
		while (ptr->next != NULL){
			ptr = ptr->next;
		}
		ptr->next = temp;
	}
}




//verifikasi username akun user baru untuk bagian pendaftaran
string verifikasi(Node_1 *head, string userName) {
	if(head == NULL && userName == "admin") {
		userName = "";
		cout<<"Mohon maaf, \nUsername sudah terdaftar, Silahkan menggunakan Username yang lain!.\n";
	} 
	while ( head != NULL ) {
		if(userName == head->data.userName || userName == "admin") {
			userName = "";
			cout<<"Mohon maaf, \nUsername sudah terdaftar, Silahkan menggunakan Username yang lain!.\n";
			break;
		}
		head = head->next;
	}
	
	return userName;
}



// fungsi untuk mendaftarkan user baru
void addUser(Node_1 **head, Node_1 **tail, int &accountCount) {
	string userName, pass, nama, email;
	Node_1 *newNode = new Node_1();
	newNode->data._id = "USER"+to_string(accountCount+1);
	while (userName == "") {//tidak boleh kosong
		cout<<"\n\nMasukkan Username	:  ";fflush(stdin);getline(cin, userName);	
		//mengecek ketersediaan username
		userName = verifikasi(*head, userName);
		newNode->data.userName = userName;
	}
	while (pass == "") {//tidak boleh kosong
		cout<<"Masukkan Password	:  ";fflush(stdin);getline(cin, pass);		
		newNode->data.pass = pass;
	}
	while (nama == "") {//tidak boleh kosong
		cout<<"Masukkan Nama		:  ";fflush(stdin);getline(cin, nama);		
		newNode->data.nama = nama;
	}
	
	while (email == "") {//tidak boleh kosong
		cout<<"Masukkan Email		:  ";fflush(stdin);getline(cin, email);		
		newNode->data.email = email;
	}
	newNode->next = NULL;
	accountCount++;
	if(*head == NULL) {//kalau linked list kosong
		*head = newNode;
		*tail = newNode;
	} else {//kalau linked list sudah terisi
		newNode->next = NULL;
		(*tail)->next = newNode;
		*tail = newNode; 	
	}
}



void menuLogin() {
	string userName, pass;
	string pil;
	do {
		system("cls");
		
		cout<<"\n\n\n=========================================================\n";
		cout<<"=========================TOKO GAME=======================";
		cout<<"\n=========================================================\n\n\n\n\n";
		cout<<"1. Login\n";
		cout<<"2. Daftar\n";
		cout<<"\n\n\n0. Keluar\n";
		
		cout<<"\nPilih Menu >> ";pil = getch();
		if(pil == "1"){
			cout<<"\n\nMasukkan Username 	:  ";cin>>userName;	
			cout<<"Masukkan Password	:  ";cin>>pass;
			auth(head, userName, pass);
			//kalau salah!
//			cout<<"\n\n\nPassword anda salah!!!\n\n\n"; 	
			continue;
		} else if (pil == "2") {
			addUser(&head, &tail, accountCount);
			continue;
		} else if (pil == "0") {
			exit(0);
		} else {
			continue;
		}
	} while (pil != "0");
		
}



//menggunakan parameter username untuk mengetahui siapa yg login
void menuUser(string userName) {
	string pil;
	string SelectedID;
	do{
		system("cls");
		string pil;
		cout<<"\n\n==================== HAI " << userName << " ==================\n\n";
		cout<<"1. Lihat Daftar Game"<<endl;
		cout<<"2. Cari Daftar Game"<<endl;
		cout<<"3. Lihat Keranjang"<<endl;
		cout<<"4. Tambah Barang Ke Keranjang"<<endl;
		cout<<"5. Hapus Barang Di Keranjang"<<endl;
		cout<<"6. Checkout"<<endl;
		cout<<"7. Hubungi Admin"<<endl;
		cout<<"8. Log Out"<<endl;
		cout<<"\nPilih Menu >> ";pil = getch();
		if (pil == "1") {
			system("cls");
			lihatDataGame(TOP);
			cout<<"\n\n Tekan Enter Untuk Melanjutkan...   ";
			getch();
			system("cls");
		} else if (pil == "2") {
			system("cls");
			cout<<" \n\n== Search Berdasarkan ID Barang ==\n\n "<<endl;
			cout<<"\n\n Masukkan ID game yang ingin anda cari:   ";
			cin>>SelectedID;
			
			convertListToArr(&TOP, &games[0]);
			shellSort(&games[0], gameCount, "3", "1");	
			int res = jumpSearch(&games[0], SelectedID, gameCount);
			if (res == -1) {
				cout<<"\n ID tidak ditemukan!!";
			} else {
				lihatDataSatuGameArr( &games[res] );
			}
			TOP=NULL;
			for (int i = 0; i < gameCount; i++){
				convertArrToList(&TOP, &END, games[i]);
			};
			cout<<"\n\n Tekan Enter Untuk Melanjutkan...   ";
			getch();
			system("cls");
		} else if (pil == "3") {
			system("cls");
			lihatKeranjang(head, TOP, userName);
			cout<<"\n\n Tekan Enter Untuk Melanjutkan...   ";
			getch();
			system("cls");
		} else if (pil == "4") {
			system("cls");
			tambahKeranjang(head, TOP, userName);
			system("cls");
		} else if (pil == "5") {
			system("cls");
			hapusIsiKeranjang(head, TOP, userName);
			cout<<"\n\n Tekan Enter Untuk Melanjutkan...   ";
			getch();
			system("cls");
		} else if (pil == "6") {
			system("cls");
			lihatKeranjang(head, TOP, userName);	
			checkout(head, TOP, &FRONT, userName);
			head->data.jumlahKeranjang=0;
			saveToCSV(TOP);
			cout<<"\n\n Berhasil!!   ";
			cout<<"\n Tekan Enter Untuk Melanjutkan...   ";
			getch();
			system("cls");
		} else if (pil == "7") {
			system("cls");
			hubungiAdmin();
			cout<<"\n\n Tekan Enter Untuk Melanjutkan...   ";
			getch();
			system("cls");
		} else if (pil == "8") {
			menuLogin();
		} else {
			continue;
		}
	} while(pil != "8");
		
}



void menuAdmin() {
	string pil;
	do{
		system("cls");
		cout<<"\n\n==================== HAI ADMIN ==================\n\n";
		cout<<"1. Data Game"<<endl;
		cout<<"2. Data User"<<endl;
		cout<<"3. Data History"<<endl;
		cout<<"4. Log Out"<<endl;
		cout<<"\nPilih Menu >> ";pil = getch();
		
		if (pil ==  "1") {
			dataGame();
		} else if (pil ==  "2") {
			dataUser();
		} else if (pil ==  "3") {
			dataHistory();
		} else if (pil ==  "4") {
			menuLogin();
		} else {
			continue;
		}
	} while (pil != "4");
}



void menuSort() {
	string orderItem, orderMethod;
	bool repeat=true;
	cout<<"\n\n==================== MENU SORT ==================\n\n";
	cout<<"1. Sorting Berdasarkan Harga"<<endl;
	cout<<"2. Sorting Berdasarkan Nama"<<endl;
	cout<<"\nPilih Menu >> ";orderItem = getch();
	while(repeat) {
		if(orderItem == "1" || orderItem=="2" ) {
			cout<<"\n\n1. Ascending"<<endl;
			cout<<"2. Descending"<<endl;
			cout<<"\nPilih Menu >> ";orderMethod = getch();
			convertListToArr(&TOP, &games[0]);
			shellSort(&games[0], gameCount, orderItem, orderMethod);	
			TOP=NULL;
			for (int i = 0; i < gameCount; i++){
				convertArrToList(&TOP, &END, games[i]);
			}
			repeat=false;
			cout<<"\n\nBerhasil!!\n\n";
			lihatDataGame(TOP);
			cout<<"\n\n Tekan Enter Untuk Melanjutkan...   ";
			getch();
		} else {
			continue;
		}
	}
}



void dataGame() {
	string pil, SelectedID;
	do{
		cout<<"\n\n==================== DATA GAME ==================\n\n";
		cout<<"1. Lihat Data Game"<<endl;
		cout<<"2. Tambah Data Game"<<endl;
		cout<<"3. Ubah Data Game"<<endl;
		cout<<"4. Hapus Data Game"<<endl;
		cout<<"5. Sortir Data Game"<<endl;
		cout<<"6. Cari Data Game"<<endl;
		cout<<"7. Kembali"<<endl;
		cout<<"\nPilih Menu >> ";pil = getch();
		if (pil ==  "1") {
			lihatDataGame(TOP);
			cout<<"\n\n Tekan Enter Untuk Melanjutkan...   ";
			getch();
			system("cls");
		} else if (pil ==  "2") {
			addDataGameLast(&TOP, &END, gameCount);
			cout<<"\n\n Tekan Enter Untuk Melanjutkan...   ";
			getch();
			system("cls");
		} else if (pil ==  "3") {
			lihatDataGame(TOP);
			cout<<"\n\n Masukkan ID game yang ingin anda ubah:   ";
			cin>>SelectedID;
			ubahDataGame(TOP, SelectedID);	
			cout<<"\n\n Tekan Enter Untuk Melanjutkan...   ";
			saveToCSV(TOP);
			getch();
			system("cls");
		} else if (pil ==  "4") {
			lihatDataGame(TOP);
			cout<<"\n\n Masukkan ID game yang ingin anda hapus:   ";
			cin>>SelectedID;
			TOP = hapusDataGame(TOP, END, SelectedID);
			saveToCSV(TOP);
			cout<<"\n\n Tekan Enter Untuk Melanjutkan...   ";
			getch();
			system("cls");
		} else if (pil ==  "5") {
			menuSort();
		} else if (pil ==  "6") {
			system("cls");
			cout<<" \n\n== Search Berdasarkan ID Barang ==\n\n "<<endl;
			cout<<"\n\n Masukkan ID game yang ingin anda cari:   ";
			cin>>SelectedID;
			
			convertListToArr(&TOP, &games[0]);
			shellSort(&games[0], gameCount, "3", "1");	
			int res = jumpSearch(&games[0], SelectedID, gameCount);
			if (res == -1) {
				cout<<"\n ID tidak ditemukan!!";
			} else {
				lihatDataSatuGameArr( &games[res] );
			}
			TOP=NULL;
			for (int i = 0; i < gameCount; i++){
				convertArrToList(&TOP, &END, games[i]);
			};
			cout<<"\n\n Tekan Enter Untuk Melanjutkan...   ";
			getch();
			system("cls");
		} else {
			continue;
		}
	} while(pil != "7");
		
}



void dataUser() {
	string pil, selectedID;
	do{
		cout<<"\n\n==================== DATA USER ==================\n\n";
		cout<<"1. Lihat Data User"<<endl;
		cout<<"2. Ubah Data User"<<endl;
		cout<<"3. Hapus Data User"<<endl;
		cout<<"4. Kembali"<<endl;
		cout<<"\nPilih Menu >> ";pil = getch();
		if (pil ==  "1") {
			lihatDataUser(head);
			cout<<"\n\n Tekan Enter Untuk Melanjutkan...   ";
			getch();
			system("cls");
		} else if (pil ==  "2") {
			lihatDataUser(head);
			cout<<"Masukkan ID yang ingin anda ubah :  ";
			cin>>selectedID;
			ubahDataUser(head, selectedID);		
			cout<<"\n\n Tekan Enter Untuk Melanjutkan...   ";
			getch();
			system("cls");
		} else if (pil ==  "3") {
			lihatDataUser(head);
			if(head != NULL) {
				cout<<"\n\n Masukkan ID User yang ingin anda hapus:   ";
				cin>>selectedID;
				head = hapusDataUser(head, tail, selectedID);
				cout<<"\n\n Berhasil!!   ";	
			}
			cout<<"\n Tekan Enter Untuk Melanjutkan...   ";
			getch();
			system("cls");
		} else {
			continue;
		}
	} while(pil != "4");		
}



void dataHistory() {
	string pil;
	do{
		cout<<"\n\n==================== DATA HISTORY ==================\n\n";
		cout<<"1. Lihat History Penjualan"<<endl;
		cout<<"2. Hapus History Penjualan"<<endl;
		cout<<"3. Kembali";
		cout<<"\nPilih Menu >> ";pil = getch();
		if (pil ==  "1") {
			lihatHistory(FRONT);
			cout<<"\n\n Tekan Enter Untuk Melanjutkan...   ";
			getch();
			system("cls");
		} else if (pil ==  "2") {
			FRONT = hapusDataHistory(FRONT, REAR);
			cout<<"\n\n Berhasil!!   ";
			cout<<"\n Tekan Enter Untuk Melanjutkan...   ";
			getch();
			system("cls");
		} else {
			continue;
		}
	} while(pil != "3");
		
}



void auth(struct Node_1 *head, string userName, string pass) {
	if(userName == "admin" and pass == "admin")	{
		menuAdmin();
	} else {
		if(head == NULL) {
			cout<<"\n\n\n User Tidak Ada!"; 	
			cout<<"\n\n Tekan Enter Untuk Melanjutkan...\n\n\n"; 	
			getch();
		}
		while (head != NULL) {	
			if (head->data.userName == userName && head->data.pass == pass) {
				menuUser(userName);
			}
			head = head->next;
		}
	}
	
}



void lihatDataUser(struct Node_1 *head){
	if (head == NULL) {
		cout << "\n User Tidak Ada !!\n\n ";
		return;
	} else {
		cout << "\nData: " << "\n\n";
		while ( head != NULL ) {
			cout << "ID		: " << head->data._id << endl;
			cout << "Nama		: " << head->data.nama << endl;
			cout << "Username	: " << head->data.userName << endl;
			cout << "Passowrd	: " << head->data.pass << endl;
			cout << "Email		: " << head->data.email << "\n\n";
			head = head->next;
		}
	}
}



void lihatDataSatuUser( Node_1 *head, string selectedID ) {
	if (head == NULL) {
		cout << "\nData Masih Kosong !!\n\n ";
		return;
	} else {
		while ( head != NULL ) {
			if (head->data._id == selectedID) {
				cout << "\n\n\nBerikut Adalah Data User yang ingin anda ubah: \n";
				cout << "\nID		: "<<head->data._id << "\n";
				cout << "Nama		: "<<head->data.nama << "\n";
				cout << "Username	: "<<head->data.userName<<"\n";
				cout << "Password	: "<<head->data.pass<< "\n";
				cout << "Email		: "<< head->data.email<<"\n\n";
			}
			head = head->next;
		}
	}
}



void ubahDataUser( Node_1 *head, string selectedID ) {
	while (head != NULL) {
		char pil;
		bool repeat = true;
        if (head->data._id == selectedID) {
        	do {
        		system("cls");
        		lihatDataSatuUser(head, selectedID);
        		cout << "\n\n+==============================+" << endl;
	        	cout << "1. Ubah Nama"<<endl;
	        	cout << "2. Ubah Password"<<endl;
	        	cout << "3. Ubah Email"<<endl;
	        	cout << "\nPilih Menu: ";pil = getch();
	        	if(pil == '1') {
	        		cout<<"\n\nMasukkan Nama Baru  :  " ;	fflush(stdin);getline(cin,head->data.nama);	
	        		repeat = false;
	        		cout << "\n\nBerhasil!!!\n\n\n\n" << endl;
				} else if (pil == '2') {
					cout<<"\n\nMasukkan Password Baru :  "; 	cin>>head->data.pass;	
					repeat = false;
					cout << "\n\nBerhasil!!!\n\n\n\n" << endl;
				} else if (pil == '3') {
					cout<<"\n\nMasukkan Email Baru :  "; 	cin>>head->data.email;
					repeat = false;	
					cout << "\n\nBerhasil!!!\n\n\n\n" << endl;
				} else {
					continue;
				}
			} while(repeat);
        }        
		head = head -> next;
	}
}



//Fungsi Hapus game bertipe data struct jadi ini kembaliannya nanti berupa Struct.
Node_1 *hapusDataUser(Node_1 *head, Node_1 *tail, string selectedID){
	//Buat Struct temp untuk perulangan nanti
	Node_1 *temp = new Node_1;
	if (head == NULL) {
		cout << "User Tidak ada!!" << endl;
		return head;
 	}
	
	//	di logika didalam while itu kita tidak bisa menghapus elemen pertama, karena 
	//	dari awal itu sudah mengecek data ke 2, jadi bisa diakali dengan menambah 1 kondisi
	//	dimana headnya itu sama dengan pil, kalau sama langsung hapus
	//  trus setelah dihapus, di kembalikan structnya. (yang sudah dihapus elemennya)
	if (head->data._id == selectedID) {		
		//karena ini Posisi head nya adalah elemen yang mau dihapus, jadi langsung arahkan saja TOPnya ke alamat selanjutnya
        head = head->next;
        head->prev = NULL;
        accountCount--;
        //free kita menghemat memory karena del_2 sudah tidak terpakai, fungsinya sama seperti delete
        return head;
    }
    //masukkan nilai dari head ke temp
    temp = head;
    //Ulangi sampai next dari temp itu NULL
    while (temp -> next != NULL) {
    	// kalau dalam perulangan itu alamat selanjutnya memiliki data._id yang mau kita hapus
        if (temp->next->data._id == selectedID) {
        	//del_1 nya itu menyimpan dari alamat next variable temp
        	//misalnya saat ini berada di alamat pertama, maka del_2 menyimpan alamat kedua:
            del = temp -> next;
            //setelah itu alamat next dari temp itu menyimpan alamat next dari variable del
            //misalnya saat ini berada di alamat pertama, maka yang seharusnya temp->next itu menyimpan alamat kedua, jadi menyimpan alamat ketiga
            temp->next = del->next;
            //free kita menghemat memory karena del_2 sudah tidak terpakai, fungsinya sama seperti delete
            if (temp->next == NULL) {
				tail = temp->next;
			}
			accountCount--;
            free(del);
            return head;
        }
        temp = temp->next;
    }
    return head;
}



void lihatDataGame(Node_2 *TOP){
	if (TOP == NULL) {
		cout << "\nData Masih Kosong !! " << endl << endl;
		return;
	} else {
		cout << "\nData Game : " << endl;
		while ( TOP != NULL ) {
			cout << "\n\nID		: " << TOP->data.ID << endl;
			cout << "Nama Game  	: " << TOP->data.namaGame << endl;
			cout << "Tanggal Rilis	: " << TOP->data.tanggalRilis << endl;
			cout << "Rating		: " << TOP->data.rating << endl;
			cout << "Stok		: " << TOP->data.stok << endl;
			cout << "Harga		: " << TOP->data.harga << endl;
			TOP = TOP->next;
		}
	}
}



void lihatDataSatuGameArr( Game *games ) {
		cout << "\n\nID		: " << games->ID << endl;
		cout << "Nama Game  	: " << games->namaGame << endl;
		cout << "Tanggal Rilis	: " << games->tanggalRilis << endl;
		cout << "Rating		: " << games->rating << endl;
		cout << "Stok		: " << games->stok << endl;
		cout << "Harga		: " << games->harga << endl;
}



void lihatDataSatuGame( Node_2 *TOP, string selectedID ) {
	if (TOP == NULL) {
		cout << "\nData Masih Kosong !!\n\n ";
		return;
	} else {
		while ( TOP != NULL ) {
			if (TOP->data.ID == selectedID) {
				cout << "\n\nID		: " << TOP->data.ID << endl;
				cout << "Nama Game  	: " << TOP->data.namaGame << endl;
				cout << "Tanggal Rilis	: " << TOP->data.tanggalRilis << endl;
				cout << "Rating		: " << TOP->data.rating << endl;
				cout << "Stok		: " << TOP->data.stok << endl;
				cout << "Harga		: " << TOP->data.harga << endl;
			}
			TOP = TOP->next;
		}
	}
}



// fungsi untuk mendaftarkan game baru
void addDataGameLast(Node_2 **TOP, Node_2 **END, int &gameCount) {
	string ID, namaGame, tanggalRilis, rating, stok, harga;
//	int rating = 0;
//	int stok = 0;
//	int harga = 0;
	Node_2 *newGame = new Node_2();
	Node_2 *temp = new Node_2();	
	temp = (*END);
	while (ID == "") {//tidak boleh kosong
		cout<<"\n\nMasukkan ID Game	:  ";fflush(stdin);getline(cin, ID);	
		newGame->data.ID = ID;
	}
	while (namaGame == "") {//tidak boleh kosong
		cout<<"\nMasukkan Nama Game	:  ";fflush(stdin);getline(cin, namaGame);	
		newGame->data.namaGame = namaGame;
	}
	while ( tanggalRilis== "") {//tidak boleh kosong
		cout<<"Masukkan Tanggal Rilis	:  ";fflush(stdin);getline(cin, tanggalRilis);		
		newGame->data.tanggalRilis= tanggalRilis;
	}
	while (rating == "") {//tidak boleh kosong
		cout << "Masukan Rating  : ";fflush(stdin);getline(cin, rating);
		newGame->data.rating = stoi(rating);	
	}
	while (stok == "") {//tidak boleh kosong
		cout << "Masukan Stok  : ";fflush(stdin);getline(cin, stok);
		newGame->data.stok = stoi(stok);		
		
	}
	while (harga == "") {//tidak boleh kosong
		cout << "Masukan Harga : ";fflush(stdin);getline(cin, harga);
		newGame->data.harga = stoi(harga);			
	}
	
	ofstream csvNewGame("game.csv", fstream::app);
    csvNewGame<<ID<<"," <<namaGame<<"," <<tanggalRilis<<"," <<rating<<"," <<stok<<"," <<harga<<"\n";
  	csvNewGame.close();
  	
	gameCount++;
	if(*TOP == NULL) {//kalau linked list kosong
		*TOP = newGame;
		*END = newGame;
	} else {//kalau linked list sudah terisi
//		while(temp->next != NULL) {
//			temp = temp->next;
//		}	
		newGame->next = NULL;	
		temp->next = newGame;
		*END = newGame;
	}
	free(newGame);
	
}

void ubahDataGame(Node_2 *TOP, string selectedID) {
	while (TOP != NULL) {
		char pil;
		bool repeat = true;
        if (TOP->data.ID == selectedID) {
        	do {
        		system("cls");
        		lihatDataSatuGame(TOP, selectedID);
        		cout << "\n\n+==============================+" << endl;
	        	cout << "1. Ubah Nama"<<endl;
	        	cout << "2. Ubah Tanggal Rilis"<<endl;
	        	cout << "3. Ubah Rating"<<endl;
	        	cout << "4. Ubah Stok"<<endl;
	        	cout << "5. Ubah Harga"<<endl;
	        	cout << "\nPilih Menu: ";pil = getch();
	        	if(pil == '1') { 
	        		cout<<"\n\nMasukkan Nama Game Baru  :  " ;	fflush(stdin);getline(cin,TOP->data.namaGame);	
	        		repeat = false;
	        		cout << "\n\nBerhasil!!!\n\n\n\n" << endl;
				} else if (pil == '2') {
					cout<<"\n\nMasukkan Tanggal Rilis Baru :  "; fflush(stdin);getline(cin,TOP->data.tanggalRilis);	
					repeat = false;
					cout << "\n\nBerhasil!!!\n\n\n\n" << endl;
				} else if (pil == '3') {
					cout<<"\n\nMasukkan Rating Baru :  "; 	cin>>TOP->data.rating;
					repeat = false;
					cout << "\n\nBerhasil!!!\n\n\n\n" << endl;
				} else if (pil == '4') {
					cout<<"\n\nMasukkan Stok Baru :  "; 	cin>>TOP->data.stok;
					repeat = false;
					cout << "\n\nBerhasil!!!\n\n\n\n" << endl;
				} else if (pil == '5') {
					cout<<"\n\nMasukkan Harga Baru :  "; 	cin>>TOP->data.harga;
					repeat = false;
					cout << "\n\nBerhasil!!!\n\n\n\n" << endl;
				} else {
					continue;
				}
			} while(repeat);
        }        
		TOP = TOP -> next;
	}
}



//Fungsi Hapus game bertipe data struct jadi ini kembaliannya nanti berupa Struct.
Node_2 *hapusDataGame(Node_2 *TOP, Node_2 *END, string SelectedID){
	//Buat Struct temp untuk perulangan nanti
	Node_2 *temp = new Node_2;
	if (TOP == NULL) {
		cout << "Game Tidak ada!!" << endl;
 	}
	
	//	di logika didalam while itu kita tidak bisa menghapus elemen pertama, karena 
	//	dari awal itu sudah mengecek data ke 2, jadi bisa diakali dengan menambah 1 kondisi
	//	dimana headnya itu sama dengan pil, kalau sama langsung hapus
	//  trus setelah dihapus, di kembalikan structnya. (yang sudah dihapus elemennya)
	if (TOP->data.ID == SelectedID) {		
		//karena ini Posisi TOP nya adalah elemen yang mau dihapus, jadi langsung arahkan saja TOPnya ke alamat selanjutnya
        TOP = TOP->next;
        TOP->prev = NULL;
        gameCount--;

        return TOP;
    }
    //masukkan nilai dari TOP ke temp
    temp = TOP;
    //Ulangi sampai next dari temp itu NULL
    while (temp -> next != NULL) {
    	// kalau dalam perulangan itu alamat selanjutnya memiliki data.ID yang mau kita hapus
        if (temp->next->data.ID == SelectedID) {
			
        	//del_2 nya itu menyimpan dari alamat next variable temp
        	//misalnya saat ini berada di alamat pertama, maka del_2 menyimpan alamat kedua:
            del_2 = temp -> next;
            //setelah itu alamat next dari temp itu menyimpan alamat next dari variable del
            //misalnya saat ini berada di alamat pertama, maka yang seharusnya temp->next itu menyimpan alamat kedua, jadi menyimpan alamat ketiga
            //pindahkan end nya kalau elemen terakhir yang dihapus
            temp->next = del_2->next;
            if (temp->next == NULL) {
				END = temp->next;
			}
            //free kita menghemat memory karena del_2 sudah tidak terpakai, fungsinya sama seperti delete
            gameCount--;
            free(del_2);
            return TOP;
        }
        temp = temp->next;
    }
    return TOP;
}

void lihatKeranjang( Node_1 *head,  Node_2 *TOP, string userName) {
	cout<<"\n";
	if (head->data.jumlahKeranjang == 0) {
		cout << "Keranjang Kosong!!" << endl;
 	}
	//buat variabel temp yang nilainya sampa dengan TOP karena kita akan menggunakan variabel TOP
	//ketika variabel TOP digunakan, nilainya akan berubah ubah sesuai alur dari linked list
	//jadi yang tercetak itu elemen list yang berada di alamat selanjutnya, 
	//karena elemen list yang terletak sebelum TOP tidak akan terbaca,
	//jadi triknya, buat variabel temp, trus kalau elemen sudah ketemu, kita kembalikan TOP tadi ke semula (nilai semulanya disimpan di temp)
	//jadi karena TOP tadi kembali ke alamat awal, jadi tidak masalah dimanapun alamat yang mau dicetak
	Node_2 *temp;
	temp = TOP;
	while (head != NULL) {	
		//kita memilih ini user mana yang punya keranjangnya
		if (head->data.userName == userName) {
			//kita ngecek keranjangnya satu per satu pakai perulangan
			for(int i = 0; i < head->data.jumlahKeranjang; i++){
				//kita lakukan perulangan selama TOP nya itu tidak NULL alias sampai data masih ada
				while(TOP != NULL) {
					//disini kita ngecek, apakah data yang ada di isi keranjang index ke-i itu sama dengan data game yang berada di top
					//kalau sama, kita print data tersebut, trus karna kita menggunakan TOP untuk looping, kita kembalikan alamatnya ke semula
					if(head->data.isiKeranjang[i] == TOP->data.ID) {
						TOP->data.stok++;//karna tadi waktu masukin ke keranjang itu --.
						lihatDataSatuGame(TOP, TOP->data.ID);
						TOP->data.stok--;
						TOP = temp;
						
						//ini kita break untuk menghemat memory, karena kalau sudah ketemu, data selanjutnya itu ga mungkin ada yg sama
						//karena kita hanya mencari berdasarkan index yang saat ini diloop
						break;
					} else {
						//kalau masih belum ketemu kita next
						TOP = TOP->next;		
					}
				}
			}
		//ini kita break untuk menghemat memory, karena kalau sudah ketemu, data selanjutnya itu ga mungkin ada yg sama			
		break;
		}
		head = head->next;	
	}
}



void tambahKeranjang( Node_1 *head, Node_2 *TOP, string userName ) {
	Node_2 *temp = new Node_2;
	temp = TOP;
	while (head != NULL) {
		//kita memilih ini user mana yang punya keranjangnya	
		if (head->data.userName == userName) {
			string selectedID;
			//print dulu data gamenya baru suruh user masukkan ke keranjang
			lihatDataGame(TOP);
			cout<<"\n\nMasukkan ID Game >>>  ";
			cin>>selectedID;	
			//nah kita masukkan data inputan tadi kedalam array keranjang tapi pastikan dulu kalau item tersebut ada didalam daftar game
			//karna isi keranjangnya bertambah, kita tambah juga ini
			while (TOP != NULL) {
				if (TOP->data.ID == selectedID){
					if(TOP->data.stok > 0 ) {
						head->data.isiKeranjang[head->data.jumlahKeranjang] = selectedID;
						head->data.jumlahKeranjang++;
						TOP->data.stok--;
					} else if (TOP->data.stok == 0){
						cout<<"\n\nSTOK tidak tersedia!!";
						cout<<"\nTekan Enter Untuk Melanjutkan!!\n\n";
						getch();
					}
				} TOP = TOP->next;	
			}
			break;
		}
		head = head->next;
		
	}
}



void hapusIsiKeranjang( Node_1 *head, Node_2 *TOP, string userName ) {
	bool found = false;
	while (head != NULL) {
		if (head->data.userName == userName) {
			string selectedID;
			//print dulu keranjangnya supaya user bisa milih
			lihatKeranjang(head, TOP, userName);
			cout<<"\n\nMasukkan ID Game >>>  ";
			cin>>selectedID;	
			for (int i = 0; i < head->data.jumlahKeranjang; i++) {
				//kalau data yang dihapus itu ada didalam array
				if (head->data.isiKeranjang[i] == selectedID){
					//kita timpa elemen yang mau kita hapus dengan elemen selanjutnya
					//lanjut terus begitu sampai index tarakhir
					for (int j = i; j < head->data.jumlahKeranjang; j++) {
						head->data.isiKeranjang[j] = head->data.isiKeranjang[j+1];
					}
					head->data.jumlahKeranjang--;
					TOP->data.stok++;
					cout<<"\n\n Berhasil!!! ";
					found = true;
				}
			}
			break;
		}
		head = head->next;	
	} if(!found) cout<<"\n\n ID Tidak ditemukan!!! ";
}



void checkout( Node_1 *head, Node_2 *TOP, Node_3 **FRONT, string userName ) {
	char pil;
	bool repeat=true;
	int totalBayar=0;
	Node_2 *temp = new Node_2;
	temp = TOP;
	while (head != NULL) {	
		if (head->data.userName == userName) {
			if (head->data.jumlahKeranjang == 0) {
				return;
			}
			for(int i = 0; i < head->data.jumlahKeranjang; i++){
				while(TOP != NULL) {
					if(head->data.isiKeranjang[i] == TOP->data.ID) {
						totalBayar += TOP->data.harga;
//						TOP->data.stok--;
						TOP = temp;
						break;
					}
					TOP = TOP->next;		
				}
			}
		}
		head = head->next;	
	}
	while(repeat) {
		cout<<"\n\n\nJumlah yang perlu anda bayarkan untuk semua game ini adalah: Rp."<<totalBayar<<"\n\n\n";
		cout << "Apakah Anda Yakin Membeli Semua Game Ini? (Y/N) : ";pil=getch();
		
		if(pil == 'Y' || pil == 'y') {						
			//akan terjadi dequeue ketika history mencapai 10
			if(historyCount > 3) {
				Node_3 *temp = *FRONT;	
				*FRONT = temp->next;
				if (*FRONT == NULL) REAR = NULL;
				historyCount--;
				delete (temp);
			}
			
			//enqueue
			time_t now = time(0);
			Node_3 *newNode 			= new Node_3;
			newNode->data.idPembelian	= "BUY-"+to_string(now);
			newNode->data.userName		= userName;
			newNode->data.tanggalBeli	= timeNow();
			newNode->data.totalHarga	= totalBayar;
			newNode->next				= NULL;
			
			
			if(*FRONT == NULL) {//kalau linked list kosong
				REAR = *FRONT = newNode;
			} else {
				REAR->next = newNode;	
				REAR = newNode;
			}
			
			historyCount++;			
		
		
		
			repeat = false;	
		} else if(pil == 'N' || pil == 'n') {
			repeat = false;
			menuUser(userName);
		} else {
			continue;
		}
	}
}



void lihatHistory(Node_3 *FRONT){
	if (FRONT == NULL) {
		cout<<"\n\n\nTidak ada transaksi!\n\n\n"; 
		return;
	} else {
		cout << "\nData Game : " << endl;
		while ( FRONT != NULL ) {
			cout << "\n\nID Pembelian	: " << FRONT->data.idPembelian << endl;
			cout << "Username	: " << FRONT->data.userName << endl;
			cout << "Tanggal Beli	: " << FRONT->data.tanggalBeli << endl;
			cout << "Harga		: " << FRONT->data.totalHarga << endl;
			FRONT = FRONT->next;
		}
	}
}



Node_3 *hapusDataHistory(Node_3 *FRONT, Node_3 *REAR){
	//Buat Struct temp untuk perulangan nanti
	if (FRONT == NULL) {
		cout<<"\n\n\nTidak ada transaksi!\n\n\n"; 
 	}
    while (FRONT != NULL) {
        del_3 = FRONT;
        FRONT = FRONT->next;
        free(del_3);
    }
    FRONT = NULL;
    historyCount=0;
    return FRONT;
}



void shellSort( Game *games, int &arrSize, string orderItem, string orderMethod ) {
	Game temp;
	for ( int gap = arrSize/2; gap > 0; gap /= 2 ) {
		for ( int i = gap; i < arrSize; i += 1 ) {
			temp = games[i];
			int j;
			//ASC			
			if( orderItem == "1" && orderMethod == "1" ){
				for ( j = i; j >= gap &&  games[j-gap].harga > temp.harga; j -= gap ) {
					games[j] = games[j - gap];
				}
		
			} else if( orderItem == "2" && orderMethod == "1" ){
				for ( j = i; j >= gap &&  games[j-gap].namaGame > temp.namaGame; j -= gap ) {
					games[j] = games[j - gap];
				}
			} else if( orderItem == "3" && orderMethod == "1" ){
				for ( j = i; j >= gap &&  games[j-gap].ID > temp.ID; j -= gap ) {
					games[j] = games[j - gap];
				}
			} 
			//DESC
			if( orderItem == "1" && orderMethod == "2" ){
				for ( j = i; j >= gap &&  games[j-gap].harga < temp.harga; j -= gap ) {
					games[j] = games[j - gap];
				}
			} else if( orderItem == "2" && orderMethod == "2" ){
				for ( j = i; j >= gap &&  games[j-gap].namaGame < temp.namaGame; j -= gap ) {
					games[j] = games[j - gap];
				}
			} 
			games[j] = temp;
		}
	}
} 



int jumpSearch(Game *arr, string x, int n) {
	int step = sqrt(n);
	int prev = 0;
	while (arr[min(step, n)-1].ID < x) {
		prev = step;
		step += sqrt(n);
		if (prev >= n) return -1;
	}
	while (arr[prev].ID < x) {
		prev++;
		if (prev == min(step, n))
		return -1;
	}
	if (arr[prev].ID == x) return prev;
	return -1;
}





//taroh paling bawah aja ga penting	
void hubungiAdmin() {
	system("cls");
	cout<<"\n\n\n\n\n\n";
	cout<<"Admin 1: 2009106017   ||   +62 852-4883-2218   ||   Riska Nurliyanti\n";
	cout<<"Admin 2: 2009106036   ||   +62 822-5126-6939   ||   Muhammad Daffa Putra Mahardika\n";
	cout<<"Admin 3: 2009106050   ||   +62 821-5831-7722   ||   Risky Kurniawan\n\n\n";
}
