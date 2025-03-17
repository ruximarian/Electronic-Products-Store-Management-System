#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

template<typename T>
class Singleton
{
private:
	/*static T Instance;*/

protected:
	/** Constructor */
	Singleton() = default;

public:
	/** Copy Constructor */
	Singleton(const Singleton&) = delete;

	/** Operator = */
	Singleton& operator = (const Singleton&) = delete;

	/** Getters */
	static T& GetInstance()
	{
		static T Instance;
		return Instance;
	}
};

enum TipUtilizator
{
	MAGAZIN,
	CLIENT
};

int getInputInt(const vector<int>& alegeriValide)
{
	bool alegereValida = false;

	while (!alegereValida)
	{
		cout << "Alegere: ";
		int alegere = 0;
		cin >> alegere;

		if (find(alegeriValide.begin(), alegeriValide.end(), alegere) != alegeriValide.end())
		{
			return alegere;
		}
		else
		{
			cout << "Input invalid\n";
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}
}

class Produs
{
public:
	// Constructor fara parametrii
	Produs()
		: ID(++LastID), denumire(""), pret(0.0)
	{

	}

	// Constructor cu parametrii
	Produs(const string& denumire, const double& pret)
		: ID(++LastID), denumire(denumire), pret(pret)
	{

	}

	// Destructor
	~Produs() = default;

	// Copy Constructor
	Produs(const Produs& p)
		: ID(++LastID), denumire(p.denumire), pret(p.pret)
	{

	}

	// Copy Assignment
	Produs& operator = (const Produs& p)
	{
		denumire = p.denumire;
		pret = p.pret;

		return *this;
	}

	// Operator >>
	friend istream& operator >> (istream& is, Produs& p)
	{
		p.read(is);
		return is;
	}

	// Operator <<
	friend ostream& operator << (ostream& os, const Produs& p)
	{
		p.print(os);
		return os;
	}

	// getters
	inline const unsigned int& getID() const { return ID; }
	inline const string& getDenumire() const { return denumire; }
	inline const double& getPret() const { return pret; }

	// setters
	inline void setDenumire(const string& denumireNoua) { denumire = denumireNoua; }
	inline void setPret(const double& pretNou) { pret = pretNou; }

	virtual void readBinary(ifstream& is)
	{
		// read denumire
		size_t len;
		is.read(reinterpret_cast<char*>(&len), sizeof(len));
		char* buffer = new char[len + 1];  // +1 pentru terminatorul de sir '\0'
		is.read(buffer, len);
		buffer[len] = '\0';
		denumire = string(buffer);
		delete[] buffer;

		// read pret
		is.read(reinterpret_cast<char*>(&pret), sizeof(pret));
	}

	virtual void writeBinary(ofstream& os)
	{
		// write denumire
		size_t len = denumire.size();
		os.write(reinterpret_cast<char*>(&len), sizeof(len));
		os.write(denumire.c_str(), len);

		// write pret
		os.write((char*)&pret, sizeof(pret));
	}

	virtual void read(istream& is)
	{
		cout << "Denumire: ";
		is >> denumire;

		cout << "Pret: ";
		is >> pret;
	}

	virtual void print(ostream& os) const
	{
		os << "ID: " << ID << '\n';
		os << "Denumire: " << denumire << '\n';
		os << "Pret: " << pret << '\n';
	}

private:
	static unsigned int LastID;

	const unsigned int ID;
	string denumire;
	double pret;
};

unsigned int Produs::LastID = 0;

class ProdusElectronic : public Produs
{
public:
	// Constructor fara parametrii
	ProdusElectronic()
		: Produs(), luniGarantie(0), producator("")
	{

	}

	// Constructor cu parametrii
	ProdusElectronic(const string& denumire, const double& pret, const int& luniGarantie, const string& producator)
		: Produs(denumire, pret), luniGarantie(luniGarantie), producator(producator)

	{

	}

	// Destructor
	~ProdusElectronic() = default;

	// Copy Constructor
	ProdusElectronic(const ProdusElectronic& p)
		: Produs(p), luniGarantie(p.luniGarantie), producator(p.producator)
	{

	}

	// Copy Assignment
	ProdusElectronic& operator = (const ProdusElectronic& p)
	{
		Produs::operator=(p);

		luniGarantie = p.luniGarantie;
		producator = p.producator;

		return *this;
	}

	// getters
	int getLuniGarantie() { return luniGarantie; };
	string getProducator() { return producator; };

	// setters
	void setLuniGarantie(const int& luni) { luniGarantie = luni; };
	void setProducator(string prod) { producator = prod; };

	virtual void readBinary(ifstream& is) override
	{
		// citeste atributele mostenite
		Produs::readBinary(is);

		// read luniGarantie
		is.read(reinterpret_cast<char*>(&luniGarantie), sizeof(luniGarantie));

		// read producator
		size_t len;
		is.read(reinterpret_cast<char*>(&len), sizeof(len));
		char* buffer = new char[len + 1];  // +1 pentru terminatorul de sir '\0'
		is.read(buffer, len);
		buffer[len] = '\0';
		producator = string(buffer);
		delete[] buffer;
	}

	virtual void writeBinary(ofstream& os) override
	{
		// scrie atributele mostenite
		Produs::writeBinary(os);

		// write luniGarantie
		os.write((char*)&luniGarantie, sizeof(luniGarantie));

		// write producator
		size_t len = producator.size();
		os.write(reinterpret_cast<char*>(&len), sizeof(len));
		os.write(producator.c_str(), len);
	}

	virtual void read(istream& is) override
	{
		Produs::read(is);

		cout << "Luni garantie: ";
		is >> luniGarantie;

		cout << "Producator: ";
		is >> producator;
	}

	virtual void print(ostream& os) const override
	{
		Produs::print(os);

		os << "Luni garantie: " << luniGarantie << '\n';
		os << "Producator: " << producator << '\n';
	}

private:
	int luniGarantie;
	string producator;
};

class Telefon : public ProdusElectronic
{
public:
	// Constructor fara parametrii
	Telefon()
		: ProdusElectronic(), capacitateBaterie(0), numarCamere(0), sistemOperare("")
	{

	}

	// Constructor cu parametrii
	Telefon(const string& denumire, const double& pret, const int& luniGarantie, const string& producator, const int& capacitateBaterie, const int& numarCamere, const string& sistemOperare)
		: ProdusElectronic(denumire, pret, luniGarantie, producator), capacitateBaterie(capacitateBaterie), numarCamere(numarCamere), sistemOperare(sistemOperare)
	{

	}

	// Destructor
	~Telefon() = default;

	// Copy Constructor
	Telefon(const Telefon& t)
		: ProdusElectronic(t), capacitateBaterie(t.capacitateBaterie), numarCamere(t.numarCamere), sistemOperare(t.sistemOperare)
	{

	}

	// Copy Assignment
	Telefon& operator = (const Telefon& t)
	{
		ProdusElectronic::operator=(t);

		capacitateBaterie = t.capacitateBaterie;
		numarCamere = t.numarCamere;
		sistemOperare = t.sistemOperare;

		return *this;
	}

	// getters
	int getCapacitateBaterie() { return capacitateBaterie; };
	int getNumarCamere() { return numarCamere; };
	string getSistemOperare() { return sistemOperare; };

	// setters
	void setCapacitateBaterie(const int& cap) { capacitateBaterie = cap; }
	void setNumarCamere(const int& nr) { numarCamere = nr; }
	void setSistemOperare(const string& sist) { sistemOperare = sist; }

	virtual void readBinary(ifstream& is) override
	{
		// citeste atributele mostenite
		ProdusElectronic::readBinary(is);

		// read capacitateBaterie
		is.read(reinterpret_cast<char*>(&capacitateBaterie), sizeof(capacitateBaterie));

		// read numarCamere
		is.read(reinterpret_cast<char*>(&numarCamere), sizeof(numarCamere));

		// read sistemOperare
		size_t len;
		is.read(reinterpret_cast<char*>(&len), sizeof(len));
		char* buffer = new char[len + 1];  // +1 pentru terminatorul de sir '\0'
		is.read(buffer, len);
		buffer[len] = '\0';
		sistemOperare = string(buffer);
		delete[] buffer;
	}

	virtual void writeBinary(ofstream& os) override
	{
		// write tip_produs
		int tip_produs = 1;
		os.write((char*)&tip_produs, sizeof(tip_produs));

		// scrie atributele mostenite
		ProdusElectronic::writeBinary(os);

		// write capacitateBaterie
		os.write((char*)&capacitateBaterie, sizeof(capacitateBaterie));

		// write numarCamere
		os.write((char*)&numarCamere, sizeof(numarCamere));

		// write producator
		size_t len = sistemOperare.size();
		os.write(reinterpret_cast<char*>(&len), sizeof(len));
		os.write(sistemOperare.c_str(), len);
	}

	virtual void read(istream& is) override
	{
		ProdusElectronic::read(is);

		cout << "Capacitate baterie: ";
		is >> capacitateBaterie;

		cout << "Numar camere: ";
		is >> numarCamere;

		cout << "Sistem operare: ";
		is >> sistemOperare;
	}

	virtual void print(ostream& os) const override
	{
		ProdusElectronic::print(os);

		os << "Capacitate baterie: " << capacitateBaterie << '\n';
		os << "Numar camere: " << numarCamere << '\n';
		os << "Sistem operare: " << sistemOperare << '\n';
	}

private:
	int capacitateBaterie;
	int numarCamere;
	string sistemOperare;
};

class Televizor : public ProdusElectronic
{
public:
	// Constructor fara parametrii
	Televizor()
		: ProdusElectronic(), tehnologieEcran(""), rezolutieEcran(""), numarPorturiHDMI(0)
	{

	}

	// Constructor cu parametrii
	Televizor(const string& denumire, const double& pret, const int& luniGarantie, const string& producator, const string& tehnologieEcran, const string& rezolutieEcran, const int& numarPorturiHDMI)
		: ProdusElectronic(denumire, pret, luniGarantie, producator), tehnologieEcran(tehnologieEcran), rezolutieEcran(rezolutieEcran), numarPorturiHDMI(numarPorturiHDMI)
	{

	}

	// Destructor
	~Televizor() = default;

	// Copy Constructor
	Televizor(const Televizor& t)
		: ProdusElectronic(t), tehnologieEcran(t.tehnologieEcran), rezolutieEcran(t.rezolutieEcran), numarPorturiHDMI(t.numarPorturiHDMI)
	{

	}

	// Copy Assignment
	Televizor& operator = (const Televizor& t)
	{
		ProdusElectronic::operator=(t);

		tehnologieEcran = t.tehnologieEcran;
		rezolutieEcran = t.rezolutieEcran;
		numarPorturiHDMI = t.numarPorturiHDMI;

		return *this;
	}

	// getters
	string getTehnologieEcran() { return tehnologieEcran; }
	string getRezolutieEcran() { return rezolutieEcran; }
	int getNumarPorturiHDMI() { return numarPorturiHDMI; }

	// setters
	void setTehnologieEcran(const string& teh) { tehnologieEcran = teh; }
	void setRezolutieEcran(const string& rez) { rezolutieEcran = rez; }
	void setNumarPorturiHDMI(const int& nr) { numarPorturiHDMI = nr; }

	virtual void readBinary(ifstream& is) override
	{
		// citeste atributele mostenite
		ProdusElectronic::readBinary(is);

		// read tehnologieEcran
		size_t len;
		is.read(reinterpret_cast<char*>(&len), sizeof(len));
		char* buffer = new char[len + 1];  // +1 pentru terminatorul de sir '\0'
		is.read(buffer, len);
		buffer[len] = '\0';
		tehnologieEcran = string(buffer);
		delete[] buffer;

		// read rezolutieEcran
		len;
		is.read(reinterpret_cast<char*>(&len), sizeof(len));
		buffer = new char[len + 1];  // +1 pentru terminatorul de sir '\0'
		is.read(buffer, len);
		buffer[len] = '\0';
		rezolutieEcran = string(buffer);
		delete[] buffer;

		// read numarPorturiHDMI
		is.read(reinterpret_cast<char*>(&numarPorturiHDMI), sizeof(numarPorturiHDMI));
	}

	virtual void writeBinary(ofstream& os) override
	{
		// write tip_produs
		int tip_produs = 2;
		os.write((char*)&tip_produs, sizeof(tip_produs));

		// scrie atributele mostenite
		ProdusElectronic::writeBinary(os);

		// write tehnologieEcran
		size_t len = tehnologieEcran.size();
		os.write(reinterpret_cast<char*>(&len), sizeof(len));
		os.write(tehnologieEcran.c_str(), len);

		// write rezolutieEcran
		len = rezolutieEcran.size();
		os.write(reinterpret_cast<char*>(&len), sizeof(len));
		os.write(rezolutieEcran.c_str(), len);

		// write numarPorturiHDMI
		os.write((char*)&numarPorturiHDMI, sizeof(numarPorturiHDMI));
	}

	virtual void read(istream& is) override
	{
		ProdusElectronic::read(is);

		cout << "Tehnologie ecran: ";
		is >> tehnologieEcran;

		cout << "Rezolutie ecran: ";
		is >> rezolutieEcran;

		cout << "Numar porturi HDMI: ";
		is >> numarPorturiHDMI;
	}

	virtual void print(ostream& os) const override
	{
		ProdusElectronic::print(os);

		os << "Tehnologie ecran: " << tehnologieEcran << '\n';
		os << "Rezolutie ecran: " << rezolutieEcran << '\n';
		os << "Numar porturi HDMI: " << numarPorturiHDMI << '\n';
	}

private:
	string tehnologieEcran;
	string rezolutieEcran;
	int numarPorturiHDMI;
};

class Comanda
{
public:
	// Constructor fara parametrii
	Comanda()
		: ID(++LastID), status(""), numeComplet(""), email(""), adresa("")
	{

	}

	// Constructor cu parametrii
	Comanda(string numeComplet, string email, string adresa)
		: ID(++LastID), status("primita"), numeComplet(numeComplet), email(email), adresa(adresa)
	{

	}

	// Destructor
	~Comanda() = default;

	// Copy Constructor
	Comanda(const Comanda& c)
		: ID(++LastID), status(c.status), numeComplet(c.numeComplet), email(c.email), adresa(c.adresa), telefoane(c.telefoane), televizoare(c.televizoare)
	{

	}

	// Copy Assignment
	Comanda& operator = (const Comanda& c)
	{
		status = c.status;
		numeComplet = c.numeComplet;
		email = c.email;
		adresa = c.adresa;
		telefoane = c.telefoane;
		televizoare = c.televizoare;

		return *this;
	}

	// ordoneaza comenzile dupa email
	bool operator < (const Comanda& c)
	{
		return this->email < c.email;
	}

	friend ostream& operator << (ostream& os, const Comanda& p)
	{
		os << "ID Comanda: " << p.ID << '\n';
		os << "Status Comanda: " << p.status << '\n';
		os << "Nume complet client: " << p.numeComplet << '\n';
		os << "Email client: " << p.email << '\n';
		os << "Adresa client: " << p.adresa << '\n';

		cout << '\n';
		for (const Telefon& telefon : p.telefoane)
		{
			telefon.print(os);
		}

		cout << '\n';
		for (const Televizor& televizor : p.televizoare)
		{
			televizor.print(os);
		}

		return os;
	}

	void readBinary(ifstream& is)
	{
		// read telefoane.size()
		size_t telefoaneSize = 0;
		is.read(reinterpret_cast<char*>(&telefoaneSize), sizeof(telefoaneSize));

		for (int i = 0; i < telefoaneSize; i++)
		{
			// read tip_produs
			int tip_produs = 0;
			is.read(reinterpret_cast<char*>(&tip_produs), sizeof(tip_produs));

			Telefon telefon;
			telefon.readBinary(is);
			telefoane.push_back(telefon);
		}

		// read televizoare.size()
		size_t televizoareSize = 0;
		is.read(reinterpret_cast<char*>(&televizoareSize), sizeof(televizoareSize));

		for (int i = 0; i < televizoareSize; i++)
		{
			// read tip_produs
			int tip_produs = 0;
			is.read(reinterpret_cast<char*>(&tip_produs), sizeof(tip_produs));

			Televizor televizoar;
			televizoar.readBinary(is);
			televizoare.push_back(televizoar);
		}

		// read status
		{
			size_t len;
			is.read(reinterpret_cast<char*>(&len), sizeof(len));
			char* buffer = new char[len + 1];  // +1 pentru terminatorul de sir '\0'
			is.read(buffer, len);
			buffer[len] = '\0';
			status = string(buffer);
			delete[] buffer;
		}

		// read numeComplet
		{
			size_t len;
			is.read(reinterpret_cast<char*>(&len), sizeof(len));
			char* buffer = new char[len + 1];  // +1 pentru terminatorul de sir '\0'
			is.read(buffer, len);
			buffer[len] = '\0';
			numeComplet = string(buffer);
			delete[] buffer;
		}

		// read email
		{
			size_t len;
			is.read(reinterpret_cast<char*>(&len), sizeof(len));
			char* buffer = new char[len + 1];  // +1 pentru terminatorul de sir '\0'
			is.read(buffer, len);
			buffer[len] = '\0';
			email = string(buffer);
			delete[] buffer;
		}

		// read adresa
		{
			size_t len;
			is.read(reinterpret_cast<char*>(&len), sizeof(len));
			char* buffer = new char[len + 1];  // +1 pentru terminatorul de sir '\0'
			is.read(buffer, len);
			buffer[len] = '\0';
			adresa = string(buffer);
			delete[] buffer;
		}
	}

	void writeBinary(ofstream& os)
	{
		// write telefoane.size()
		size_t telefoaneSize = telefoane.size();
		os.write((char*)&telefoaneSize, sizeof(telefoaneSize));

		for (size_t i = 0; i < telefoaneSize; i++)
		{
			telefoane[i].writeBinary(os);
		}

		// write televizoare.size()
		size_t televizoareSize = televizoare.size();
		os.write((char*)&televizoareSize, sizeof(televizoareSize));

		for (size_t i = 0; i < televizoareSize; i++)
		{
			televizoare[i].writeBinary(os);
		}

		// write status
		size_t len = status.size();
		os.write(reinterpret_cast<char*>(&len), sizeof(len));
		os.write(status.c_str(), len);

		// write numeComplet
		len = numeComplet.size();
		os.write(reinterpret_cast<char*>(&len), sizeof(len));
		os.write(numeComplet.c_str(), len);

		// write email
		len = email.size();
		os.write(reinterpret_cast<char*>(&len), sizeof(len));
		os.write(email.c_str(), len);

		// write adresa
		len = adresa.size();
		os.write(reinterpret_cast<char*>(&len), sizeof(len));
		os.write(adresa.c_str(), len);
	}

	void adaugareTelefon(const Telefon& telefon)
	{
		telefoane.push_back(telefon);
	}

	void adaugareTelevizor(const Televizor& televizor)
	{
		televizoare.push_back(televizor);
	}

	void editareStatus()
	{
		cout << "Status comanda: ";
		cin >> status;
	}

	// getters
	unsigned int getID() { return ID; }

private:
	static unsigned int LastID;

	const unsigned int ID;
	string status;

	vector<Telefon> telefoane;
	vector<Televizor> televizoare;

	string numeComplet;
	string email;
	string adresa;
};

unsigned int Comanda::LastID = 0;

class Magazin
{
public:
	// Constructor fara parametrii
	Magazin()
	{
		// citeste toate produsele dintr-un fisier binar
		{
			ifstream file("produse.bin", std::ios::binary | std::ios::in);

			if (!file.is_open())
			{
				cout << "Eroare la deschiderea fisierului!" << std::endl;
			}
			else
			{
				// read totalProduse
				size_t totalProduse = 0;
				file.read(reinterpret_cast<char*>(&totalProduse), sizeof(totalProduse));

				for (int i = 0; i < totalProduse; i++)
				{
					Produs* produs = nullptr;

					// read tip_produs
					int tip_produs = 0;
					file.read(reinterpret_cast<char*>(&tip_produs), sizeof(tip_produs));

					if (tip_produs == 1) // telefon
					{
						produs = new Telefon();
					}
					else if (tip_produs == 2) // televizor
					{
						produs = new Televizor();
					}
					else
					{
						cout << "EROARE\n";
					}

					produs->readBinary(file);
					produse.push_back(produs);
				}

				file.close();
			}
		}

		// citeste toate comenzile dintr-un fisier binar
		{
			ifstream file("comenzi.bin", std::ios::binary | std::ios::in);

			if (!file.is_open())
			{
				cout << "Eroare la deschiderea fisierului!" << std::endl;
			}
			else
			{
				// read totalComenzi
				size_t totalComenzi = 0;
				file.read(reinterpret_cast<char*>(&totalComenzi), sizeof(totalComenzi));

				for (int i = 0; i < totalComenzi; i++)
				{
					Comanda comanda;
					comanda.readBinary(file);
					comenzi.push_back(comanda);
				}

				file.close();
			}
		}
	}

	// Destructor
	~Magazin()
	{
		// incarca toate produsele intr-un fisier binar
		{
			ofstream file("produse.bin", std::ios::binary | std::ios::out | std::ios::trunc);

			// write totalProduse
			size_t totalProduse = produse.size();
			file.write((char*)&totalProduse, sizeof(totalProduse));

			for (const auto& produs : produse)
			{
				produs->writeBinary(file);
			}

			file.close();

			// dezalocare produse
			for (int i = 0; i < produse.size(); i++)
			{
				delete produse[i];
			}
		}

		// incarca toate comenzile intr-un fisier binar
		{
			ofstream file("comenzi.bin", std::ios::binary | std::ios::out | std::ios::trunc);

			// write totalComenzi
			size_t totalComenzi = comenzi.size();
			file.write((char*)&totalComenzi, sizeof(totalComenzi));

			for (auto& comanda : comenzi)
			{
				comanda.writeBinary(file);
			}

			file.close();
		}
	}

	// Copy Constructor
	Magazin(const Magazin& m)
		: produse(m.produse), comenzi(m.comenzi)
	{

	}

	// Copy Assignment
	Magazin& operator = (const Magazin& m)
	{
		produse = m.produse;
		comenzi = m.comenzi;

		return *this;
	}

	void afisareProduse()
	{
		cout << "\n\n\n\tAfisare produse\n";

		for (const Produs* produs : produse)
		{
			cout << *produs << '\n';
		}
	}

	void adugareProdus()
	{
		cout << "\n\n\n\tAdaugare produs\n";
		cout << "Selectati tipul produsului:\n";
		cout << "1. Telefon\n";
		cout << "2. Televizor\n";

		int alegere = getInputInt({ 1, 2 });

		Produs* produsNou = nullptr;

		if (alegere == 1)
		{
			produsNou = new Telefon();
		}
		else if (alegere == 2)
		{
			produsNou = new Televizor();
		}

		cin >> *produsNou;

		produse.push_back(produsNou);
	}

	void editareProdus()
	{
		cout << "\n\n\n\tEditare produs\n";
		cout << "Selectati produsul care urmeaza sa fie editat:\n";

		vector<int> alegeriValide;

		for (const Produs* produs : produse)
		{
			alegeriValide.push_back(produs->getID());

			cout << *produs << '\n';
		}

		// nu avem produse din care sa alegem
		if (alegeriValide.size() == 0)
		{
			return;
		}

		Produs* produsDeEditat = nullptr;
		int alegere = getInputInt(alegeriValide);

		for (Produs* produs : produse)
		{
			if (produs->getID() == alegere)
			{
				produsDeEditat = produs;
				break;
			}
		}

		cin >> *produsDeEditat;
	}

	void stergereProdus()
	{
		cout << "\n\n\n\tStergere produs\n";
		cout << "Selectati produsul care urmeaza sa fie sters:\n";

		vector<int> alegeriValide;

		for (const Produs* produs : produse)
		{
			alegeriValide.push_back(produs->getID());

			cout << *produs << '\n';
		}

		// nu avem produse din care sa alegem
		if (alegeriValide.size() == 0)
		{
			return;
		}

		int alegere = getInputInt(alegeriValide);

		for (int i = 0; i < produse.size(); i++)
		{
			if (produse[i]->getID() == alegere)
			{
				produse.erase(produse.begin() + i);
				break;
			}
		}
	}

	Comanda* adaugareComanda(const string& numeComplet, const string& email, const string& adresa, const vector<Produs*>& cos)
	{
		Comanda* comandaNoua = new Comanda(numeComplet, email, adresa);

		for (Produs* p : cos)
		{
			if (Telefon* telefon = dynamic_cast<Telefon*>(p))
			{
				comandaNoua->adaugareTelefon(*telefon);
			}
			else if (Televizor* televizor = dynamic_cast<Televizor*>(p))
			{
				comandaNoua->adaugareTelevizor(*televizor);
			}
		}

		comenzi.push_back(*comandaNoua);

		return comandaNoua;
	}

	void realizareRaportComanzi()
	{
		cout << "\n\n\n\tRealizare raport pentru toate comenzile magazinului\n";

		ofstream fout("RaportComenzi.txt");

		// sorteaza comenzile dupa email
		sort(comenzi.begin(), comenzi.end());

		for (const Comanda& comanda : comenzi)
		{
			fout << comanda << '\n';
		}

		fout.close();
	}

	void prelucrareComanda()
	{
		cout << "\n\n\n\tAlegeti comanda care trebuie sa fie prelucrata\n";

		vector<int> alegeriValide;
		for (Comanda& comanda : comenzi)
		{
			alegeriValide.push_back(comanda.getID());

			cout << comanda;
		}

		// nu avem comenzi din care sa alegem
		if (alegeriValide.size() == 0)
		{
			return;
		}

		int alegere = getInputInt(alegeriValide);

		for (Comanda& comanda : comenzi)
		{
			if (comanda.getID() == alegere)
			{
				comanda.editareStatus();
				break;
			}
		}
	}

	// getters
	vector<Produs*> getProduse() { return produse; }
	vector<Comanda>& getComenzi() { return comenzi; }

private:
	vector<Produs*> produse;
	vector<Comanda> comenzi;
};

class Client
{
public:
	// Constructor cu parametrii
	Client(Magazin& magazin)
		: magazin(magazin), numeComplet(""), email(""), adresa("")
	{

	}

	// Destructor
	~Client()
	{
		if (ultimaComanda)
		{
			delete ultimaComanda;
		}
	}

	// Copy Constructor
	Client(const Client& c)
		: magazin(c.magazin), numeComplet(c.numeComplet), email(c.email), adresa(c.adresa)
	{
		// pune o copie a produsului in cos
		for (Produs* p : c.cos)
		{
			if (Telefon* telefon = dynamic_cast<Telefon*>(p))
			{
				Produs* copieTelefon = new Telefon(*telefon);
				cos.push_back(copieTelefon);
			}
			else if (Televizor* televizor = dynamic_cast<Televizor*>(p))
			{
				Produs* copieTelevizor = new Televizor(*televizor);
				cos.push_back(copieTelevizor);
			}
		}
	}

	// Copy Assignment
	Client& operator = (const Client& c)
	{
		numeComplet = c.numeComplet;
		email = c.email;
		adresa = c.adresa;

		// sterge vechile valori din cos
		for (Produs* p : c.cos)
		{
			delete p;
		}

		// pune o copie a produsului in cos
		for (Produs* p : c.cos)
		{
			if (Telefon* telefon = dynamic_cast<Telefon*>(p))
			{
				Produs* copieTelefon = new Telefon(*telefon);
				cos.push_back(copieTelefon);
			}
			else if (Televizor* televizor = dynamic_cast<Televizor*>(p))
			{
				Produs* copieTelevizor = new Televizor(*televizor);
				cos.push_back(copieTelevizor);
			}
		}

		return *this;
	}

	// getters
	string getNumeComplet() { return numeComplet; }
	string getEmail() { return email; }
	string getAdresa() { return adresa; }

	// setters
	void setNumeComplet(const string& input) { numeComplet = input; }
	void setEmail(const string& input) { email = input; }
	void setAdresa(const string& input) { adresa = input; }

	void afisareCos()
	{
		cout << "\n\n\n\tAfisare cos\n";

		for (const Produs* produs : cos)
		{
			cout << *produs << '\n';
		}
	}

	void adugareProdus()
	{
		cout << "\n\n\n\tSelectare produse pentru introducerea acestora in cosul electronic\n";

		vector<int> alegeriValide = { };

		vector<Produs*> produseMagazin = magazin.getProduse();

		for (const Produs* p : produseMagazin)
		{
			alegeriValide.push_back(p->getID());

			cout << *p << '\n';
		}

		int alegere = getInputInt(alegeriValide);

		// pune o copie a produsului in cos
		for (Produs* p : produseMagazin)
		{
			if (p->getID() == alegere)
			{
				if (Telefon* telefon = dynamic_cast<Telefon*>(p))
				{
					Produs* copieTelefon = new Telefon(*telefon);
					cos.push_back(copieTelefon);
				}
				else if (Televizor* televizor = dynamic_cast<Televizor*>(p))
				{
					Produs* copieTelevizor = new Televizor(*televizor);
					cos.push_back(copieTelevizor);
				}

				break;
			}
		}
	}

	void stergeProdus()
	{
		cout << "\n\n\n\tScoate un produs din cos\n";

		vector<int> alegeriValide;
		for (const Produs* p : cos)
		{
			alegeriValide.push_back(p->getID());

			cout << *p << '\n';
		}

		// nu avem produse din care sa alegem
		if (alegeriValide.size() == 0)
		{
			return;
		}

		int alegere = getInputInt(alegeriValide);

		for (int i = 0; i < cos.size(); i++)
		{
			if (cos[i]->getID() == alegere)
			{
				cos.erase(cos.begin() + i);
				break;
			}
		}
	}

	void completareDate()
	{
		cout << "\n\n\n\tCompletarea datelor personale\n";

		cout << "Nume complet: ";
		cin >> numeComplet;

		cout << "Email: ";
		cin >> email;

		cout << "Adresa: ";
		cin >> adresa;
	}

	void trimiteComanda()
	{
		cout << "\n\n\n\tTrimite comanda cu produsele selectate din cos\n";

		// verifica daca avem toate datele completate + daca e ceva in cos
		if (numeComplet.size() != 0 && email.size() != 0 && adresa.size() != 0 && cos.size() != 0)
		{
			ultimaComanda = magazin.adaugareComanda(numeComplet, email, adresa, cos);
			cout << "Comanda a fost procesata\n";
		}
		else
		{
			cout << "Comanda nu a putut fi procesata\n";
			cout << "Asigurati-va ca aveti produse in cos si ca toate datele personale sunt completate\n";
		}
	}

	void realizareRaportComanda()
	{
		cout << "\n\n\n\tRaport pentru ultima comanda trimisa\n";

		if (ultimaComanda == nullptr)
		{
			cout << "Nu ati efectuat o comanda\n";
		}
		else
		{
			cout << "Raportul pentru ultima comanda a fost efectuat\n";

			ofstream fout("RaportComanda.txt");
			fout << *ultimaComanda;
			fout.close();
		}
	}

private:
	Magazin& magazin;
	vector<Produs*> cos;

	string numeComplet;
	string email;
	string adresa;

	Comanda* ultimaComanda;
};

class Meniu : public Singleton<Meniu>
{
public:
	// Constructor
	Meniu()
		: magazin(), running(true), client(magazin)
	{

	}

	// Copy Constructor
	Meniu(const Meniu&) = delete;

	// Copy Assignment
	Meniu& operator = (const Meniu&) = delete;

	// Destructor
	~Meniu() = default;

	void run()
	{
		login();

		while (running)
		{
			optiuni();
		}
	}

private:
	void login()
	{
		cout << "\n\n\n\tSelectati tipul de utilizator\n";
		cout << "1. Magazin\n";
		cout << "2. Client\n";

		vector<int> alegeriValide = { 1, 2 };
		int alegere = getInputInt(alegeriValide);

		if (alegere == 1)
		{
			tipUtilizator = TipUtilizator::MAGAZIN;
		}
		else if (alegere == 2)
		{
			tipUtilizator = TipUtilizator::CLIENT;
		}
	}

	void optiuni()
	{
		cout << "\n\n\n\tMeniu:\n";

		if (tipUtilizator == TipUtilizator::MAGAZIN)
		{
			cout << "1. Afisare produse\n";
			cout << "2. Adaugare produs\n";
			cout << "3. Editare produs\n";
			cout << "4. Stergere produs\n";
			cout << "5. Prelucrare comenzi\n";
			cout << "6. Realizare raport\n";
			cout << "7. Iesire\n";

			vector<int> alegeriValide = { 1, 2, 3, 4, 5, 6, 7 };
			int alegere = getInputInt(alegeriValide);

			switch (alegere)
			{
			case 1:
				magazin.afisareProduse();
				break;

			case 2:
				magazin.adugareProdus();
				break;

			case 3:
				magazin.editareProdus();
				break;

			case 4:
				magazin.stergereProdus();
				break;

			case 5:
				magazin.prelucrareComanda();
				break;

			case 6:
				magazin.realizareRaportComanzi();
				break;

			case 7:
				running = false;
				break;
			}
		}
		else if (tipUtilizator == TipUtilizator::CLIENT)
		{
			cout << "1. Vizualizare produse\n";
			cout << "2. Afisare cos\n";
			cout << "3. Selectare produse pentru introducerea acestora in cosul electronic\n";
			cout << "4. Elimina produse din cosul electronic\n";
			cout << "5. Completare date personale\n";
			cout << "6. Trimite comanda din cosul electronic\n";
			cout << "7. Realizare raport\n";
			cout << "8. Iesire\n";

			vector<int> alegeriValide = { 1, 2, 3, 4, 5, 6, 7, 8 };
			int alegere = getInputInt(alegeriValide);

			switch (alegere)
			{
			case 1:
				magazin.afisareProduse();
				break;

			case 2:
				client.afisareCos();
				break;

			case 3:
				client.adugareProdus();
				break;

			case 4:
				client.stergeProdus();
				break;

			case 5:
				client.completareDate();
				break;

			case 6:
				client.trimiteComanda();
				break;

			case 7:
				client.realizareRaportComanda();
				break;

			case 8:
				running = false;
				break;
			}
		}
	}

	TipUtilizator tipUtilizator;
	Magazin magazin;
	Client client;
	bool running;
};

int main()
{
	Meniu::GetInstance().run();

	return 0;
}

