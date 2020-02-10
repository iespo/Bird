/*
	WP: 27.01.2020
	Test von DoorBird
*/

#include<iostream>	//standart libraries
#include<string>	//fuer klass string
#include<fstream>	//fuer Files (read/write)
#include <sstream>	//fuer ostringstream ost;
using namespace std;

struct Kunde
{
	string Vorname;
	string Name;
	string trenn;
	int param;
};

class bhaKunde
{
private:
	string Vorname;
	string Name;
	int param;
public:
	bhaKunde() { Vorname = Name = ""; param = 0; }
	bhaKunde(string Vorname, string Name, int param)
	{
		this->Vorname = Vorname;
		this->Name = Name;
		this->param = param;
	}

	void setParam(int param)		//fuer die Fuelung die Parameter mit code: 100
	{
		this->param = param;
	}

	//void bhaPrint()
	//{
	//	cout << Vorname << " " << Name << " : " << param << endl;
	//}

	friend ostream& operator<<(ostream &os, const bhaKunde &ku);	//erlaube zum private:
	friend istream& operator>>(istream &is, bhaKunde &ku);			//erlabte zum priwate:
};

ostream& operator<<(ostream &os, const bhaKunde &ku)				//method: operator fuer write
{
	os << ku.Vorname << " " << ku.Name << " " << ku.param;
	return os;
}
istream& operator>>(istream &is, bhaKunde &ku)						//method: operator fuer read, kein const!
{
	is >> ku.Vorname >> ku.Name >> ku.param;
	return is;
}
//---------------------------------------------

//---------- READ ----------------------
void readFromListe(string &path)
{
	ifstream rdKu;
	string line = "";

	rdKu.open(path);
	if (rdKu.is_open())
	{
		cout << "File open" << endl;
		while (!rdKu.eof())
		{
			getline(rdKu, line);
			cout << line << endl;
		}
	}
	else { cout << "READ: file cannot be opened for reading!" << endl; }

	rdKu.close();
}

//---------- STATISTIK ----------------------
int readStatistik(string &path)
{
	ifstream rdKu;
	int symbols = 0, lines = 0;				//fuer Statistik

	rdKu.open(path);
	if (rdKu.is_open())
	{
		char ch;
		string line = "";

		cout << "Statistik: " << endl;

		while (!rdKu.eof())
		{
			rdKu.get(ch); symbols++;		//Symbolszahl
			if ((int)ch == 10)lines++;		//Linienzahl ***(ASCII: 32=Lehzeichen, 10=Enter, 9=Tab)
			//getline(rdKu, line); lines++;
		}
		lines++;
		cout << "Symbols: " << symbols << "\t Kundenzahl: " << lines << endl;
	}
	else { cout << "STATISTIK: File cannot be opened for reading!" << endl; }

	rdKu.close();

	return lines;
}


//---------- WRITE ----------------------
void writeParam(string &fPath)
{
	fstream fst;
	fst.open(fPath, fstream::in | fstream::out | fstream::app);
	//ifstream fst;
	fst.open(fPath);

	if (fst.is_open())
	{
		string fnam, lnam;
		int par;

		bhaKunde k[50];
		string kundestr;
		int param = 99;
		int pos = 0;


		fst.seekg(0, ios::beg);
		cout << "Cursor Pos: " << fst.tellg() << endl;

		while (!fst.eof()) //(s!='\n'&&!fA.eof())  //wp: poisk '\n'-perenos strok
		{

			//fnam = lnam = "";
			fst >> fnam;
			fst >> lnam;
			//fst.getline(kundestr, sizeof(kundestr));	//fA.getline(stroka,sizeof(stroka)); // Считываем строчку в переменную stroka (255 znakow :[ )
			fst << " : ";
			fst << param << endl;

			//k[pos].Vorname = fnam;
			//k[pos].Name = lnam;
			//k[pos].param = par;

			//cout << k[pos] << endl;
			//getline(fst, kundestr);
			fst >> kundestr;
			cout << kundestr << endl;
			pos++;
		}
	}
	else { cout << "file open error!" << endl; }

	fst.close();

}

string intToStr(int &par)
{
	stringstream strpar;	//fuer Int to Str

	strpar.str("");				//cleaning string von alten parameter
	strpar.clear();
	strpar << par;				//param as string
								//line += strpar.str();			//<- param as string
								//line.append(strpar.str());	//<- param as string
	return strpar.str();
}

// 5 ---------- Param EDIT ----------------------
void paramEdit(ifstream &rdKu, ofstream &wrKu, string &stofind)
{
	//string stofind;		// z.B. "Luise";
	int16_t param;			// 1...99
	string line = "";		//ein Satz
	int pos = 0;			//1.Buchstabenpos
	string yn;				// ja/nein

	Kunde ku;
	string trenn;			//tempstring fuer ":"


	//cout << "Enter a search word:" << endl;
	//cin >> stofind;
	while (!rdKu.eof())						//read customer list
	{
		rdKu >> ku.Vorname;
		rdKu >> ku.Name;
		rdKu >> trenn;						//fuer kurs-Bewegung
		rdKu >> ku.param;

		line = ku.Vorname + " " + ku.Name + " : \t";

		pos = line.find(stofind);
		if (pos >= 0)
		{
			cout << line << ku.param << endl << endl;				// for edit
			cout << "Edit parameter? [Y,J]-yes/[N]-no:" << endl;
			cin >> yn;
			if (yn == "Y" || yn == "y" || yn == "J" || yn == "j")
			{
				cout << "Enter new parameter:" << endl;
				cin >> ku.param;
			}
			else
			{
				wrKu << line << endl;		//write one line to file
			}
		}//end if(pos < 0)

		if (!rdKu.eof())
		{
			line += intToStr(ku.param);
			wrKu << line << endl;		//write one line to file
			cout << line << endl;		//konsole
		}
	}
}

// 4 ---------- FIND ----------------------
void fileLoescheKunde(ifstream &rdKu, ofstream &wrKu, string &stofind)
{
	string line = "";			//ein Satz
	int zeilz = 0;				//Zeilenzahl
	int pos = 0;				//1.Buchstabenpos
	string yn;

	while (!rdKu.eof())					//read customer list
	{
		//zeilz++;
		getline(rdKu, line);			//listet eine Zeile

		pos = line.find(stofind);
		if (pos < 0)					// =-1 -wenn nich gefunden
		{
			wrKu << line << endl;		//write one line to file
			cout << line << endl;		//konsole 
		}
		else
		{
			cout << line << endl << endl;	// 
			cout << "Delete line? [Y,J]-yes/[N]-no: " << endl;
			cin >> yn;
			if (yn == "Y" || yn == "y" || yn == "J" || yn == "j")
			{

				cout << "Deleted!" << endl << endl;
				rdKu.clear();
			}
			else
			{
				wrKu << line << endl;		//write one line to file
			}
		}//end if(pos < 0)
	}//end while
}

// 3 ---------- File neue KUNDE -------------------
void fileNeuKunde(string &fPathRd, string &fPathWr, int16_t dialog)
{
	//bhaKunde ku;
	fstream fs;
	fs.open(fPathWr, fstream::in | fstream::out | fstream::app);


	if (fs.is_open())
	{
		string msg;
		int pos = 0;

		if (dialog == 3)
		{
			cout << "Enter your first name: ";
			cin >> msg;					//from keyboard
			fs << endl << msg << " ";	//write to file
			cout << "Enter your last name: ";
			cin >> msg;					//from keyboard
			fs << msg << " : \t";		//write to file
			cout << "Enter Paraneter (1...99): ";

			cin >> msg;					//from keyboard
			//pos = fs.tellg();
			//pos--;
			//fs.seekg(pos);
			fs.clear();
			fs << msg;					//write to file
		}
		else
		{
			cout << "File read: " << endl;
			readFromListe(fPathWr);

			/*string finame, laname;
			while (!fs.eof())
			{
				msg = "";
				fs >> msg;	cout << msg << " ";
				fs >> msg;	cout << msg << endl;
			}*/
		}
	}
	else { cout << "file open error!" << endl; }

	fs.close();
}

// 2 ---------- File COPIE ----------------------
void fileCopie(ifstream &rdKu, ofstream &wrKu)
{
	string line = "";				//ein Satz

	char ch;
	while (rdKu.get(ch))			//get() - gibt true/false
	{
		cout << ch;					//symbolsweise reading
		wrKu << ch;					//symbolsweise reading
	}

	/*	while (!rdKu.eof())				 //read customer list
		{
			getline(rdKu, line);			//listet eine Zeile
			wrKu << line << endl;			//write one line to file
			cout << line << endl;			// <--!Ein Enter am ende ist zufiel
		}//end while  */
}

// 100 ---------- Param FILLING ----------------------
void paramAutoFill(ifstream &rdKu, ofstream &wrKu)
{
	int param;
	//stringstream strpar;
	string line = "";				//ein Satz

	//while (!rdKu.eof())				//read customer list	//(s!='\n'&&!f.eof())
	//{
	while (getline(rdKu, line))			//listet eine Zeile
	{
		line.append(" : \t");			//Trennzeichen ":"

		param = 1 + rand() % 98;		//1...99 - Zufaelige param

		line += intToStr(param);		//param as string

		cout << line << endl;

		if (!rdKu.eof()) {
			wrKu << line << endl;		//write one line to file <- !Ein Enter am ende ist zufiel
		}
		else {
			wrKu << line;				//write one line to file <- !Ein Enter am ende ist zufiel
		}
	}//end while

}

//---------- OPENFILE ----------------------
void openFile(string &fPathRd, string &fPathWr, int16_t &dialog, string stofind = "")
{
	ifstream rdKu;				//for read from file

	rdKu.open(fPathRd);
	if (rdKu.is_open())
	{
		cout << "File open to read: ..\\" << fPathRd << endl;

		ofstream wrKu;			//for write to file
		wrKu.open(fPathWr);		// , ofstream::app);
		if (wrKu.is_open())
		{
			cout << "File open to write: ..\\" << fPathWr << endl;

			if (dialog == 2)				//File Copie
			{
				fileCopie(rdKu, wrKu);		//copying customer list
			}
			else if (dialog == 3)			//Add neuen Kunden
			{
				//hat eigene Methode
			}
			else if (dialog == 4)			//streichen die Kunden
			{
				fileLoescheKunde(rdKu, wrKu, stofind);
			}
			else if (dialog == 5)			//Parameter Editor
			{
				paramEdit(rdKu, wrKu, stofind);	//Parameter Editor
			}
			else if (dialog == 100)
			{
				paramAutoFill(rdKu, wrKu);
			}
			else {}

		}//end if (wrKu.is_open())
		else { cout << "OPENFILE: File cannot open to write!" << endl; }

		wrKu.close();
		cout << "OPENFILE: File is closed for writing" << endl;

	}//end if(rdKu.is_open())
	else { cout << "OPENFILE: File cannot be opened for reading!" << endl; }	// !Füllen Sie zuerst die Parameter mit dem Code 100 aus

	rdKu.close();
	cout << "OPENFILE: File is closed for reading" << endl;
}

//---------- DIALOG ----------------------
int16_t fooDialog()
{
	int di = 0;	//read=0/write=1

	cout << "Enter: \n";
	cout << " [100] - (Start) autofill param ( only from new customer-file! ) \n";
	cout << " [1] - Reading files\n [2] - File copie \n [3] - Add neue Kunde \n";
	cout << " [4] - Streichen die Kunden \n";
	cout << " [5] - Parameter aendern \n";
	cout << " Other - Exit \n ";
	cin >> di;

	return di;
}

//---------- MAIN ----------------------
int main()
{
	setlocale(LC_ALL, "ru");

	cout << "DoorBird!\n";
	string fPathKd = "Namensliste.txt";		//Kundenliste ohne Parameter fuer read
	string fPath1 = "Kunden1.txt";			//fuer die Fuellung die Parameter
	string fPath2 = "Kunden2.txt";			//Add neuen Kunden nach die Kopierung von "Kunden1.txt"
	string fPath3 = "Kunden3.txt";			//streichen die Kunden
	string fPath4 = "Kunden4.txt";			//Aendern die Parameter

	int16_t dg;
	dg = fooDialog();


	if (dg == 100)	//!!! Anfang			
	{
		openFile(fPathKd, fPath1, dg);	//Erstellt eine neue Datei mit Parametern
	}
	else if (dg == 1)					//Vergleichen
	{
		cout << "Read Namens-Liste: ..\\" << fPathKd << endl;
		readStatistik(fPathKd);
		readFromListe(fPathKd);
		cout << "Read Kunden-Liste: ..\\" << fPath1 << endl;
		readStatistik(fPath1);
		readFromListe(fPath1);
		//readFromListe(fPath2);
	}
	else if (dg == 2)					//File Copie
	{
		openFile(fPath1, fPath2, dg);
		//fileCopie(fPath1, fPath2);
		cout << "\n--- New file: --- " << endl;
		readFromListe(fPath2);
	}
	else if (dg == 3)					//Add neuen Kunden
	{
		fileNeuKunde(fPath1, fPath2, 3);
		//writeToListe(fPath2);			
	}
	else if (dg == 4)					//streichen die Kunden
	{
		string sfind;	// = "Luise";
		cout << "Enter a search word:" << endl;
		cin >> sfind;
		openFile(fPath2, fPath3, dg, sfind);
		//fileLoescheKunde(fPath2, fPath3, sfind);	
	}
	else if (dg == 5)					//Parameter Editor
	{
		string sfind;	// = "Luise";
		cout << "Enter a search word:" << endl;
		cin >> sfind;
		openFile(fPath3, fPath4, dg, sfind);
	}
	else
	{
		// terminates the loop if number equals 0.0
		// break;
	}


	system("pause");
	return 0;
}
