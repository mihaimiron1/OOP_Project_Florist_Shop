#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <cstdlib> 
#include <fstream>
#include <vector>
#include <ctime>
#include "json.hpp"
using namespace std;

void cont_bancar();
int verificare_numar(string input) 
{
    int numar;
    istringstream stream(input);
    if (stream >> numar && stream.eof()) {
       
        if (numar > 0) {  
            return numar;
        } else {
            return -1;  
        }
    } else {
        return -1;  
    }
}


class ExceptieStocInsuficient : public exception {
private:
    string message;

public:
    // Constructorul fără parametri, mesajul este setat direct în clasă
    ExceptieStocInsuficient() {
        message = "Stoc insuficient pentru achizitionarea produsului!";
    }

    // Suprascrierea metodei what() pentru a returna mesajul de eroare
    string what()  throw()
    {
        return message;
    }
};



class ExceptieDiscountInvalid : public exception {
private:
    string message;

public:
    // Constructor care setează mesajul de eroare
    ExceptieDiscountInvalid(const string& msg) : message(msg) {}

    // Override pentru metoda what() care returnează mesajul de eroare
    const char* what() const throw() {
        return message.c_str();
    }
};



class ExceptiePlataInvalida: public exception{
    private:
    string message;

    public:
    // Constructor care setează mesajul de eroare
    ExceptiePlataInvalida(const string& msg) : message(msg) {}

    // Override pentru metoda what() care returnează mesajul de eroare
    const char* what() const throw() {
        return message.c_str();
    }
};


class ExceptieProdusInvalid: public exception{
    private:
    string message;

    public:
    // Constructor care setează mesajul de eroare
    ExceptieProdusInvalid(const string& msg) : message(msg) {}

    // Override pentru metoda what() care returnează mesajul de eroare
    const char* what() const throw() {
        return message.c_str();
    }
};

class ExceptieClientNeeligibil : public exception {
private:
    string message;

public:
    // Constructor care setează mesajul de eroare
    ExceptieClientNeeligibil(const string& msg) : message(msg) {}

    // Override pentru metoda what() care returnează mesajul de eroare
    const char* what() const throw() {
        return message.c_str();
    }
};



class ExceptieCodInvalid : public exception {
private:
    string message;

public:
    // Constructorul fără parametri, mesajul este setat direct în clasă
    ExceptieCodInvalid() {
        message = "Cod de  introdus de client nu este valid!";
    }

    // Suprascrierea metodei what() pentru a returna mesajul de eroare
    string what()  throw()
    {
        return message;
    }
};


class AbstractClient{

public:
    int* cantitate_id;
    int* id;
    int* produse_bogo;
    bool idGasit;
    int raspuns;
    string rs;
    string id_ver;
    int idver;
    
    AbstractClient() 
    {
        cantitate_id = new int[40];
        id = new int[40];
        produse_bogo = new int[40];

        for (int i = 0; i < 40; i++) {
            cantitate_id[i] = 0;
            produse_bogo[i] = 0;
        }

        for (int i = 0; i < 40; i++) {
            id[i] = 0;
        }
    }

    virtual void setData(int u_id, int buy, string n, int v, int t_money, int v_status) = 0;
    virtual void afiseazaDetalii() = 0;
    virtual void scriere_json(nlohmann::json& jsonData) = 0;
    virtual void login(nlohmann::json jsonData) = 0;

};


class InfoClient: public virtual AbstractClient{
    public:
    string nume;
protected:
    int user_id;
    int buy_500;
    int vizite;
    float total_money;
    int vip;
    public:

    
    InfoClient(int u_id, int buy, string n, int v, int t_money, int v_status)
        : user_id(u_id), buy_500(buy), nume(n), vizite(v), total_money(t_money), vip(v_status) {}

    void setData (int u_id, int buy, string n, int v, int t_money, int v_status)override {
        user_id = u_id;
        buy_500 = buy;
        nume = n;
        
    }


    void afiseazaDetalii () override
    {
        if(vip==1) cout<<"\nClientul este VIP\n";
        else cout<<"\nClientul nu este VIP\n";
    }




    void login(nlohmann::json jsonData) override{
        
        cout<<"Bine ati venit la magazinul nostru"<<endl;
        
    }


    void scriere_json(nlohmann::json& jsonData) override
    {
        
        bool persoanaGasita = false;
        for (auto& item : jsonData["persoane"])
        {
            if (item["ID"] == user_id) 
            {
                // Actualizăm datele persoanei
                item["Vizite"] = vizite;
                item["Vip"] = vip;
                item["Buy_500"] = buy_500;
                item["Total_money"] = total_money;
                persoanaGasita = true;
                break;
            }
        }

        if (!persoanaGasita) 
        {
            nlohmann::json  newPersoana;
            newPersoana["Buy_500"]= buy_500;
            newPersoana["ID"] = user_id;
            newPersoana["Nume"]= nume;
            newPersoana["Total_money"]= total_money;
            newPersoana["Vip"]= vip;
            newPersoana["Vizite"]= vizite;
            jsonData["persoane"].push_back(newPersoana);
        }
        

    }


    // Supraincarcarea operatorului () pentru a seta eligibilitatea la reducere
    

    InfoClient& operator-() 
        {
            if(total_money > 500000) vip=1;
            return *this;
        }


};

class ReducereClient: public virtual AbstractClient{
public:
    int card_reducere;//daca se poate face reducere
    float suma;
    
	int nr_elemente;//cate produse am cumparat
	float r_suma;

    ReducereClient(): suma(0), r_suma(0.0), nr_elemente(0),card_reducere(0){}

    
   

    void login(nlohmann::json jsonData) override
    {
        
        cout << "Introduceti ID-ul persoanei pe care doriti sa o verificati: ";
        getline(cin, id_ver);
        idver = verificare_numar(id_ver);
             
    }

    ReducereClient& operator%(float d) {
            suma -= suma * (d / 100);
            return *this;
        }


    ReducereClient& operator+(int d) 
        {
            suma+=d;
            return *this;
        }

    void scriere_json(nlohmann::json& jsonData) override
{
    ofstream outFile("data.json");
    if (outFile.is_open()) {
        outFile << jsonData.dump(4); // Scriere cu indentare pentru lizibilitate
        outFile.close();
    } else {
        cout << "Nu am putut deschide fișierul pentru salvare!" << endl;
    }
}

};





template <typename T>
class PaymentSystem {
public:
    // Metodă pentru plata prin card
    void payByCard(const T& amount) {
        cont_bancar();
    }

    // Metodă pentru plata numerar
    void payByCash(const T& amount) {
        if (amount <= 0) {
            throw invalid_argument("Suma pentru plata numerar trebuie să fie pozitivă.");
        }
        cout << "Plată numerar efectuată cu suma: " << amount << " lei." << endl;
    }

    // Metodă pentru plata cu reducere
    void payWithDiscount(const T& amount, const T& discount) {
        if (amount <= 0 || discount < 0 || discount >= amount) {
            throw invalid_argument("Suma sau reducerea este invalidă.");
        }
        T finalAmount = amount - discount;
        cout << "Plată cu reducere efectuată. Suma inițială: " << amount << " lei, reducere: " << discount << " lei, sumă finală: " << finalAmount << " lei." << endl;
    }
};

template <typename T>
class DiscountCard {
public:
    string cardCode;   // Codul cardului
    T discountValue;   // Valoarea reducerii (procentajul reducerii)

    // Constructor pentru inițializarea cardului
    DiscountCard(string cardCode, T discountValue)
        : cardCode(cardCode), discountValue(discountValue) {}

    // Metodă pentru a aplica discountul la o comandă
    T applyDiscount(T amount) const {
        return amount - (amount * discountValue / 100);  // Reducere pe bază de procentaj
    }
};








class Client : public ReducereClient , public InfoClient{
public:
PaymentSystem<double> paymentSystem;
DiscountCard<int>* discountCard = nullptr;  // Un singur card de reducere activ
string reduceri[5] = {"UTM5", "lab2", "flori9", "dai3", "--7"};  // Coduri valide de reducere
int red[5] = {5, 2, 9, 3, 7};  // Valori de reducere asociate fiecărui cod
Client(int u_id, int buy, string n, int v, int t_money, int v_status):
        InfoClient(u_id, buy, n, v, t_money, v_status), ReducereClient(), AbstractClient(){}


void assignDiscountCard(string inputCode) {
        // Căutăm dacă codul introdus este valid în array-ul de reduceri
        bool validCode = false;
        int discount = 0;
        
        for (int i = 0; i < 5; ++i) {
            if (reduceri[i] == inputCode) {
                validCode = true;
                discount = red[i];
                break;
            }
        }
        
        if (validCode) {
            discountCard = new DiscountCard<int>(inputCode, discount);  // Atribuim reducerea
            cout << "Discount code '" << inputCode << "' applied with " << discount << "% discount!" << endl;
        } else {
            cout << "Invalid discount code!" << endl;
        }
    }

    // Metodă pentru a aplica reduceri la o comandă
    double applyDiscount(double amount) {
        if (discountCard != nullptr) {
            return discountCard->applyDiscount(amount);
        } else {
            cout << "No discount card assigned!" << endl;
            return amount;
        }
    }


void payOrder() {
    cout<<"Cum ati dori sa efectati plata?\n1) card\n2 cash\n";
    cout<<"Type: ";
    string method;
    cin >> method;

        try {
            if (method == "card") {
                paymentSystem.payByCard(suma);
            } else if (method == "cash") {
                paymentSystem.payByCash(suma);
            } else {
                throw invalid_argument("Metodă de plată invalidă.");
            }
        } catch (const exception& e) {
            cerr << "Eroare la procesarea plății: " << e.what() << endl;
        }
    }




void scriere_json(nlohmann::json& jsonData) override {
    InfoClient::scriere_json(jsonData);
    ReducereClient::scriere_json(jsonData);
    for (const auto& item : jsonData["persoane"]) //de pus fiecare item in cos
    {       
        Client* user=new Client(item["ID"],item["Buy_500"],item["Nume"],item["Vizite"],item["Total_money"],item["Vip"]);
        -(*user);
        if (user->getVip()== 1)
        {
            string filename = "fisier_" + item["Nume"].get<string>() + ".txt"; // Folosește get<string>() pentru a obține numele si a o converti în tipul string
            ofstream outFile(filename);

            if (outFile.is_open()) 
            {
                time_t now = time(0);
                tm *ltm = localtime(&now);
                outFile << "************************************" << endl;
                outFile << "*          INVITATIE VIP           *" << endl;
                outFile << "************************************" << endl << endl;
                outFile << "Draga " << item["Nume"].get<string>()  << "," << endl << endl;
                outFile << "   Avem placerea de a te invita la o expozitie speciala de flori rare" << endl;
                outFile << "organizata de magazinul nostru, Florarie Magnolia." << endl;
                outFile << "   Evenimentul va avea loc pe data de " << ltm->tm_mday+1 << "-"<< ltm->tm_mon + 1 << "-" << 1900 + ltm->tm_year<< ", si vei avea ocazia" << endl;
                outFile << "sa descoperi colectii unice de flori rare din intreaga lume." << endl << endl;
                outFile << "   Te asteptam cu drag si entuziasm "<<item["Nume"].get<string>() <<"!" << endl << endl;
                outFile << "Cu respect," << endl;
                outFile << "Echipa Florarie Magnolia"<< endl;
                outFile << "************************************" << endl;
            } 
            else 
            {
                cerr << "Eroare la deschiderea fisierului " << filename << "!" << endl;
            }
        }
        delete user;
    }
}

          
Client& operator!() 
        {
            if (buy_500 >= 5 && total_money > 4000) 
                card_reducere = 1;
            return *this;
        } 


 Client& operator++()
    { 
        ++vizite; 
        if(suma>500) buy_500++;
        total_money+=suma;
        return *this; 
    }

    int getVip() {
        return vip;
    }
    void setData (int u_id, int buy, string n, int v, int t_money, int v_status)override {
        InfoClient::setData(u_id, buy, n, v, t_money, v_status);
        vizite = v;
        total_money = t_money;
        vip = v_status;
    }
    void afiseazaDetalii () override
    {
        InfoClient::afiseazaDetalii();
        cout << "Detalii utilizator:\n";
        cout << "ID utilizator: " << user_id << endl;
        cout << "Buy 500: " << buy_500 << endl;
        cout << "Nume: " << nume << endl;
        cout << "Vizite: " << vizite << endl;
        cout << "Total bani: " << total_money << endl;
        
    }

     
    
void login(nlohmann::json jsonData) override
{
    InfoClient::login(jsonData);
    ReducereClient::login(jsonData);
    if(idver==-1)
    { 
       cerr << "Eroare: Input invalid sau numărul nu este pozitiv!" << endl;
       login(jsonData);
    }
    else
    {
        // Verificare dacă ID-ul există în JSON
        idGasit = false;
        for (const auto& item : jsonData["persoane"]) 
        {
            if (item["ID"] == idver)
            {
                setData(item["ID"],item["Buy_500"],item["Nume"],item["Vizite"],item["Total_money"],item["Vip"]);
                cout<<"Id gasit";
                afiseazaDetalii();
                // afiseazaDetalii(*user);
                idGasit = true;
                break;
            }
        } 
    } 
        

    if (!idGasit) 
    {
                // Dacă ID-ul nu a fost găsit, adăugăm o persoană nouă cu date goale sau completate de utilizator
        string nume;
        cout << "ID-ul " << idver << " nu exista. Introduceti numele persoanei: ";           
        getline(cin, nume);
        setData(idver,0,nume,0,0,0);
        afiseazaDetalii();
    }
    
        
}


void reducere_conditie(int contor_reducere){
    if(card_reducere == 0){
        throw ExceptieClientNeeligibil("Nu dispuneti de reducere!\n");
    }
           
    if (contor_reducere == 1) {
            throw ExceptieClientNeeligibil("Reducerea a fost deja aplicata!\n");
        }
        
        // Verificăm dacă utilizatorul este eligibil pentru reducere
    if (suma <= 0) {
            throw ExceptieClientNeeligibil("Inca nu ati cumparat nimic\n");
        }

}




    
    friend void scriere_json(Client& user, nlohmann::json& jsonData);
    friend void scrisoare_vip(nlohmann::json& jsonData);
    ~Client() 
	{
        delete[] cantitate_id; 
		delete[] id;  
		delete[] produse_bogo;    
    }
};



class Florarie {
public:
    string nume_produs;
    int pret;
    int id;
    int cantitate;
    int tip;
    bool promotie_aplicata;    //pentru a verifica dacă promoția a fost aplicată deja
    bool bogo_eligibil;        // pentru a indica dacă produsul este eligibil pentru BOGO

    Florarie() : nume_produs(""), pret(0), id(0), cantitate(0), tip(0), promotie_aplicata(false), bogo_eligibil(false) {}


    Florarie(string nume, int p, int i, int c, int t, bool bogo) 
        : nume_produs(nume), pret(p), id(i), cantitate(c), tip(t), promotie_aplicata(false), bogo_eligibil(bogo) {}
    //modify
    void display() const{
        cout << "Produs: " << nume_produs << ", Pret: " << pret<< ", ID: " << id << ", Cantitate: " << cantitate << endl;
    }
    


    void achizitioneaza(int cant, int stoc) {
        if (cant > stoc) {
            throw ExceptieStocInsuficient(); // Aruncăm excepția fără parametri
        } else {
            cout << "Achizitie reusita! Cantitate ramasa: " << stoc-cant << endl;
        }
    }

    bool isSufficient(int quantity) const {
        return cantitate >= quantity;
    }

    // Metodă pentru a reduce cantitatea din stoc
    void reduceStock(int quantity) {
        if (isSufficient(quantity)) {
            cantitate -= quantity;
        } else {
            cout << "Eroare: Nu sunt suficiente produse in stoc!\n";
        }
    }


};







template <typename T>
class Inventory {
public:
    vector<T> products;

    // Metodă pentru adăugarea unui produs
    void addProduct(const T& product) {
        products.push_back(product);
    }

    // Metodă pentru ștergerea unui produs
    void removeProduct(const string& productName) {
        for (auto it = products.begin(); it != products.end(); ++it) {
            if (it->nume_produs == productName) {
                products.erase(it);
                return;
            }
        }
        cout << "Product not found!\n";
    }

    // Metodă pentru a lista produsele
    void listProducts() const {
        for (const auto& product : products) {
            product.display();
        }
    }

    // Getter pentru produsele din inventar
    vector<T>& getProducts(){
        return products;
    }
};






class ProductCatalog {
public:
vector<Florarie> products;
    // Metodă pentru a căuta produse după nume sau categorie
    void searchProduct(const int& query) {
        bool found = false;
        for (const auto& product : products) {
            if (product.id == query) {
                product.display();
                found = true;
            }
        }
        if (!found) {
            cout << "No products found for query: " << query << endl;
        }
    }
};






// Clasa Store care agregă Inventory
class Store {
private:
    Inventory<Florarie> inventory;
    ProductCatalog catalog;

public:
    // Adaugă produs în magazin
    void addProductToStore(const Florarie& product) {
        inventory.addProduct(product);
    }

    // Șterge produs din magazin
    void removeProductFromStore(const std::string& productName) {
        inventory.removeProduct(productName);
    }

    // Listează produsele din magazin
    void listStoreProducts(int tipFilter){
        for (auto& product : inventory.getProducts()) {
            if (product.tip == tipFilter && product.cantitate>0) {  // Comparăm cu cantitatea specificată
                product.display();
            }
        }
    }
    void searchProductInCatalog(const int& query) {
         bool found = false;
        for (const auto& product : inventory.getProducts()) {
            if (product.id == query) {
                product.display();
                found = true;
            }
        }
        if (!found) {
            cout << "No products found for query: " << query << endl;
        }
    }
    Inventory<Florarie>& getInventory(){
        return inventory;
    }

};




template <typename T>
class Order {
private:
    Client customer;                  // Compoziție: Order conține un obiect Customer
    vector<T> products;                 // Lista de produse
    double totalValue;                  // Valoarea totală a comenzii

public:
    // Constructor
    Order(const Client& customer) : customer(customer), totalValue(0) {}

    // Adaugă produs în comandă
    void addProduct(const T& product, int quantity) {
        if (product.isSufficient(quantity)) {
            products.push_back(product);
            totalValue += product.unitPrice * quantity;
        } else {
            cout << "Eroare: Nu sunt suficiente produse in stoc!\n";
        }
    }

    
    // Metodă pentru a genera factura
    void generateInvoice(const Client& customer, const std::vector<Florarie>& products) const {
    cout << "Factura pentru " << customer.nume << ":\n";
    cout << "Produse comandate:\n";
    for (size_t i = 0; i < customer.nr_elemente; ++i) {
        for (const auto& product : products) {
            if (product.id == customer.id[i]) {
                cout << product.nume_produs << ", " << customer.cantitate_id[i] << " : " 
                     << product.pret * customer.cantitate_id[i] << " lei"
                     << " + " << customer.produse_bogo[i] << " produse bogo" << endl;
                break;
            }
        }
    }
}
};


void salvaEroareInJurnal(const string& mesajEroare, const string& tipEroare) {
    // Deschidem fișierul de jurnal în mod aditiv
    ofstream jurnal("jurnal_erori.txt", ios::app);//mod aditiv (append). date scrise în fișier vor fi adăugate la sfârșitul fișierului, fără a șterge conținutul existent

    if (jurnal.is_open()) {
        // Obținem data și ora curentă
        time_t now = time(0);
        char* dt = ctime(&now);

        // Scriem detalii în fișierul jurnal
        jurnal << "Data si ora: " << dt;
        jurnal << "Tipul erorii: " << tipEroare << endl;
        jurnal << "Mesajul erorii: " << mesajEroare << endl;
        jurnal << "------------------------------------" << endl;

        jurnal.close();
    } else {
        cout << "Nu s-a putut deschide fișierul de jurnal!" << endl;
    }
}




void cautare_dupa_ID(Store& flo, int numar_produse_flori)
{
    string id_temp;
    int id;
    cout << "Introdu ID-ul plantei dorite:";
    getline(cin, id_temp);
    id = verificare_numar(id_temp);
    flo.searchProductInCatalog(id);
}

int meniu(int*contor_reduceri){
    string temp_c;
    int c;
    cout<<"\n******Meniu******\n";
    cout<<"Ce tip de planta doriti?\n1 Plante de interior\n2 Plante de exterior\n3 Suplimente pentru gradina\n";
    cout<<"4 Cautare dupa ID \n";
    cout<<"5 Suma spre plata \n";
	cout<<"6 Pentru garantare de pret \n";
    cout<<"7 Pentru reducere \n";
    if(*contor_reduceri==0)
		cout<<"8 Pentru a aplica o reducere \n";
    cout<<"10 Pentru adaugare produse (Dev only) \n";
    cout<<"11 Pentrureducere sociala\n";    
    cout<<"9 Iesire\n";
    getline(cin, temp_c);
    c=verificare_numar(temp_c);
    return c;
}

void bogo_produs(Florarie* produs, int nr_cumparate, Client& user, int iter)
{
	int ramas;
	if (produs->bogo_eligibil && !produs->promotie_aplicata)
	{
		int free_prod=nr_cumparate/2;
		if(produs->cantitate>=free_prod &&free_prod!=0)
		{
			cout<<"Deoarece avem oferta BOGO ati obtinu adaugator "<<free_prod<<" produse gratuit\n";
			produs->cantitate-=free_prod;
			produs->promotie_aplicata=true;
			user.produse_bogo[iter]+=free_prod;
		}
		else if(produs->cantitate>0 && produs->cantitate<free_prod && free_prod!=0)
		{
			ramas=produs->cantitate;
			user.cantitate_id[iter]+=ramas;
			produs->cantitate-=ramas;
			cout<<"Deoarece avem oferta BOGO dar un stoc mai mic, ati obtinut "<<ramas<<" produse gratuit\n";
			produs->promotie_aplicata=true;
			user.produse_bogo[iter]+=ramas;
		}
	}
}

void afisare_plante(Store& flo, int* contor, int b, Client& user,int numar_produse_flori) {
	int co;
	int iter = 0;

    flo.listStoreProducts(b);

	co = 1;
	string var_temp;
	while (co == 1) {
    	cout << "Doriti sa cumparati ceva?\n1 Da\n2 Nu\n";
    	getline(cin, var_temp);
    	co = verificare_numar(var_temp);
    	if (co == 2) {
    		*contor = 0;
    		return;
    	}
    	if (co == 1) {
    		int id_temp, can_temp;
    		string var_id_temp, var_can_temp;
    		cout << "\nIntrodu Id produsului pe care doriti sa il cumparati:";
    		getline(cin, var_id_temp);
    		id_temp = verificare_numar(var_id_temp);
    		
    		if (id_temp == -1) {
    			cerr << "Eroare: Input invalid sau numărul nu este pozitiv!" << endl << endl; 
    			break;
    		}
    		cout << "\nIntrodu cantitatea produsului pe care doriti sa il cumparati:";
    		getline(cin, var_can_temp);
    		can_temp = verificare_numar(var_can_temp);
    		if (can_temp == -1) {
    			cerr << "Eroare: Input invalid sau numărul nu este pozitiv!" << endl << endl; 
    			break;
    		}
    		int gasire = 0;

            auto& products = flo.getInventory().getProducts();

    		for (auto& product : products) {
    		    if (product.id == id_temp) {
    		        gasire = 1;
                    
                    try {
                        if(!product.isSufficient(can_temp))
                        {
                            throw ExceptieStocInsuficient(); // Aruncăm excepția fără parametri
                        } else {
                            cout << "Achizitie reusita! Cantitate ramasa: " << product.cantitate-can_temp << endl;
                        }
                                        
                        
                        user.suma += can_temp * product.pret;
        		        product.cantitate -= can_temp;
        		        cout << "\nSuma totala: " << user.suma << endl;
						int contor_gasire_in_user = 0;
						for (iter = 0; iter < user.nr_elemente; iter++) {
							if (user.id[iter] == id_temp) {
								user.cantitate_id[iter] += can_temp;
								contor_gasire_in_user = 1;
								break;
							}
						}
						if (contor_gasire_in_user == 0) {
							user.cantitate_id[user.nr_elemente] = can_temp;
							user.id[user.nr_elemente] = id_temp;
							user.nr_elemente++;
						}
                        
                        
                        
                        
                        
                        break;  // Ieșim din buclă dacă achiziția a fost reușită

                    } catch (ExceptieStocInsuficient& e) {
                        // Capturăm și tratăm excepția
                        cout << "Eroare: " << e.what() << endl;

                        // Salvăm eroarea în fișierul jurnal
                        salvaEroareInJurnal(e.what(), "ExceptieStocInsuficient");
                    }
                    
                
    		        
    		        break;
    		    }
    		}
    		
    		if (gasire == 0) {
    		    cout << "Nu ati introdus ID-ul corect\n";
    		}
    	} else if (co == -1) {
    	    cerr << "Eroare: Input invalid sau numărul nu este pozitiv!" << endl;
    	    co = 1;
    	}
	}
}

void gradare_pret(Store& flo, Client& user, int numar_produse_flori)
{
    if(user.suma != 0)
    {
        string raspuns_temp;
        int raspuns;
        string id_temp;
        int id, iter, f;
        int contor = 0;
        int contor2 = 0;

        cout << "Ati gasit un produs mai ieftin ca la magazinul nostru si doriti sa il restituiti?\n1 -- Da\n2 -- Nu\n";
        getline(cin, raspuns_temp);
        raspuns = verificare_numar(raspuns_temp);
        
        if(raspuns == 1)
        {
            cout << "Introdu ID-ul produsului pe care l-ati gasit mai ieftin: ";
            getline(cin, id_temp);
            id = verificare_numar(id_temp);
            
            // Verificăm dacă produsul există în coșul utilizatorului
            for(iter = 0; iter < user.nr_elemente; iter++)
            {
                if(user.id[iter] == id)
                {
                    contor = 1;
                    cout<<"Am gasit produsul in cos1\n";
                    break;
                }
            }

            // Verificăm dacă produsul există în stocul florăriei
            auto& products = flo.getInventory().getProducts();
            for (auto& product : products) 
            {
                if(id == product.id)
                {
                    contor2 = 1;
                    cout<<"Am gasit produsul in stoc2\n";
                    // Dacă am găsit produsul în coș și în stoc
                    if(contor == 1 && contor2 == 1)
                    {
                        user.suma -= user.cantitate_id[iter] * product.pret;
                        user.suma+=user.r_suma;
                        product.cantitate += user.cantitate_id[iter]+user.produse_bogo[iter];
                        user.cantitate_id[iter] = 0;
                        user.id[iter] = 0;
                        user.produse_bogo[iter] = 0;
                        cout << "Suma a fost restituita, DAR AM ANULAT REDUCERILE in cont: " << user.suma<<"lei";
                    }
                    break;
                }
                
            }
            cout << "ID introdus gresit sau nu ati cumparat produsul\n";

        }
    }
    else
    {
        cout << "Inca nu ati cumparat nimic\n";
    }
}

void suma_plata(Client& user, Store& flo ,int* contor_suma,int numar_produse_flori)
{
	if(user.suma==0) cout<<"Nu ati cumparat nimic inca";
    else
    {

        auto& products = flo.getInventory().getProducts();
        for(int i=0;i<numar_produse_flori;i++)
            if(user.cantitate_id[i]!=0)
            {
                for (auto& product : products)
                if(product.id==user.id[i])
                    {cout<<product.nume_produs<<", "<<user.cantitate_id[i]<<" : "<<product.pret*user.cantitate_id[i]<<" lei"<<" + "<<user.produse_bogo[i]<<" produse bogo"<<endl;break;}
            }
        cout<<"Suma totala: "<<user.suma<<endl;
        if(user.r_suma!=0)
        {
            cout<<"-Reducere:"<<user.r_suma<<endl;
        }
        if(user.suma+user.r_suma>1000)
            cout<<"Deoarece ati achizitionat produse in suma mai mare de 1000 lei va oferim transport gratis, suma spre achitare: "<<user.suma<<endl;
        else if(*contor_suma == 1) cout<<"Suma ce include transportul: "<<user.suma;
        else 
        {
            int tr;
            string tr_t;
            cout<<"Doriti transport?\n1--Da\n2--Nu  ";
            getline(cin, tr_t);
            tr=verificare_numar(tr_t);
            if(tr == -1)  {cerr << "Eroare: Input invalid sau numărul nu este pozitiv!" << endl<< endl;}

            if(tr==2){
                cout<<"Suma totala spre achitare: "<<user.suma<<endl;
                
            }
            else
            {
                *contor_suma=1;
                int ch;
                string ch_t;
                cout<<"Pentru transport:\nIn municipiul Chisinau 100 lei -- 1\nInafara municipiului Chisinau 200 lei  -- 2: ";
                getline(cin, ch_t);
                ch=verificare_numar(ch_t);
                if(ch == -1)  {cerr << "Eroare: Input invalid sau numărul nu este pozitiv!" << endl<< endl;}

                if(ch==1)user+100;
                else user+200; 
                cout<<"Suma totala spre achitare: "<<user.suma<<endl;
            }
        }
    }
}

void reducere(Client& user, int* contor_reducere) {
    try{
        user.reducere_conditie(*contor_reducere);
        int c=user.suma;
            cout<<"Benefeciati de 20% reducere\n";
            user % 20.0f;
            *contor_reducere=1;
            user.r_suma=c-user.suma;
    }catch (const ExceptieClientNeeligibil& e) {
        cout << "Eroare: " << e.what() << endl;
        salvaEroareInJurnal(e.what(), "ExceptieClientNeeligibil");
    }

}

void scrisoare_vip(const nlohmann::json& jsonData) // Se folosește referință const pentru a evita copierea
{
    for (const auto& item : jsonData["persoane"]) //de pus fiecare item in cos
    {
        Client* user=new Client(item["ID"],item["Buy_500"],item["Nume"],item["Vizite"],item["Total_money"],item["Vip"]);
        -(*user);
        if (user->getVip()== 1) 
        {
            string filename = "fisier_" + item["Nume"].get<string>() + ".txt"; // Folosește get<string>() pentru a obține numele si a o converti în tipul string
            ofstream outFile(filename);

            if (outFile.is_open()) 
            {
                time_t now = time(0);
                tm *ltm = localtime(&now);
                outFile << "************************************" << endl;
                outFile << "*          INVITATIE VIP           *" << endl;
                outFile << "************************************" << endl << endl;
                outFile << "Draga " << item["Nume"].get<string>()  << "," << endl << endl;
                outFile << "   Avem placerea de a te invita la o expozitie speciala de flori rare" << endl;
                outFile << "organizata de magazinul nostru, Florarie Magnolia." << endl;
                outFile << "   Evenimentul va avea loc pe data de " << ltm->tm_mday+1 << "-"<< ltm->tm_mon + 1 << "-" << 1900 + ltm->tm_year<< ", si vei avea ocazia" << endl;
                outFile << "sa descoperi colectii unice de flori rare din intreaga lume." << endl << endl;
                outFile << "   Te asteptam cu drag si entuziasm "<<item["Nume"].get<string>() <<"!" << endl << endl;
                outFile << "Cu respect," << endl;
                outFile << "Echipa Florarie Magnolia"<< endl;
                outFile << "************************************" << endl;
            } 
            else 
            {
                cerr << "Eroare la deschiderea fisierului " << filename << "!" << endl;
            }
        }
        delete user;
    }
}

void scriere_json(Client& user, nlohmann::json& jsonData){
    bool persoanaGasita = false;
    for (auto& item : jsonData["persoane"])
    {
        if (item["ID"] == user.user_id) {
            // Actualizăm datele persoanei
            item["Vizite"] = user.vizite;
            item["Vip"] = user.vip;
            item["Buy_500"] = user.buy_500;
            item["Total_money"] = user.total_money;
            persoanaGasita = true;
            break;
        }
    }

    if (!persoanaGasita) 
    {
        nlohmann::json  newPersoana;
        newPersoana["Buy_500"]= user.buy_500;
        newPersoana["ID"] = user.user_id;
        newPersoana["Nume"]= user.nume;
        newPersoana["Total_money"]= user.total_money;
        newPersoana["Vip"]= user.vip;
        newPersoana["Vizite"]= user.vizite;
        jsonData["persoane"].push_back(newPersoana);
    }
}

void scrieJSON(const nlohmann::json& jsonData ){
    ofstream outFile("data.json");
    if (outFile.is_open()) {
        outFile << jsonData.dump(4); // Scriere cu indentare pentru lizibilitate
        outFile.close();
    } else {
        cout << "Nu am putut deschide fișierul pentru salvare!" << endl;
    }
}



void reducere_cod(Client& user, string reduceri[], int* contor, int red[], int contor_reducere,float *r_suma) {
    if (user.suma == 0) {
        cout << "Inca nu ati cumparat nimic\n";
    }
	else if(*contor==1)
		{cout<<"Deja ati aplicat reducerea";}
	else 
	{
        int choise;
        string var;
        cout << "Doriti sa aplicati reducerea la suma de " << user.suma << " lei\n1 Da\n2 Nu\n";
        getline(cin, var);
        choise = verificare_numar(var);
        
        if (choise == 1) 
		{
            
            cout << "Introdu codul reducerii:\n";
            string reduc;
            getline(cin, reduc); // Citește codul reducerii
            user.assignDiscountCard(reduc);
            float temporar=user.suma;
            user.suma=user.applyDiscount(user.suma);
            *r_suma += temporar - user.suma;
            cout << "Suma redusa este: " << user.suma << " lei\n";
            *contor=1;
        }
		else if(choise!=2)
			cerr << "Eroare: Input invalid sau numărul nu este pozitiv!" << endl;
    }
}


int verificare_numar_eror(string input) 
{
    int numar;
    istringstream stream(input);
    if (stream >> numar && stream.eof()) {
       
        if (numar >= 0) {  
            return numar;
        } else {
            return -1;  
        }
    } else {
        return -1;  
    }
}



void verificare_pret(int pret) 
{
        if(pret==0)
            throw ExceptieProdusInvalid("Pretul produsului nu poate fi 0!");
        else if (pret < 0) {
            throw ExceptieProdusInvalid("Pretul produsului nu poate fi negativ sau nu ati introdus un integer!");
        }
    
}

void verificare_id(int id){
    if(id<-1){
        throw ExceptieProdusInvalid("Id invalid");
    }
}


void verificare_cantitate(int can){
    if(can==0){
        throw ExceptieProdusInvalid("Cantitatea nu poate sa fie 0");
    }
    else if (can==-1)
        throw ExceptieProdusInvalid("Cantitate invalida sau negativa");
}

void verificare_tip(int tip){
    if(tip<0 || tip >2)
        throw ExceptieProdusInvalid("Tip invalid, trebuie sa fie 0, 1 sau 2");
}

void verificare_bogo(int b){
    if(b!=0 && b!=1)
        throw ExceptieProdusInvalid("Valoare invalida, trebuie sa fie 0 sau 1");
}

    



void only_dev(Store& flo, int& numar_produse){
    cout<<"Pentru a accesa aceasta comanda trebuie sa fiti dezvoltator\nIntrodu codul dezvoltaorului: ";
    int cod;
    string var;
    string nume_produs_nou;
    int pret_nou;
    int id_nou;
    int cantitate_nou;
    int tip_nou;
    bool bogo_eligibil_nou;
    getline(cin, var);
    cod = verificare_numar(var);
    if(cod!=-1)
    {
        if (cod==200411)
        {   
            int contor_while=1;
            while (contor_while==1)
            {
                try
                {
                    cout<<"Introdu numele produsului: ";
                    getline(cin, nume_produs_nou);
                    
                    cout<<"Introdu pretul produsului: ";
                    getline(cin, var);
                    pret_nou = verificare_numar_eror(var);
                    verificare_pret(pret_nou);

                    cout<<"Introdu ID-ul produsului: ";
                    getline(cin, var);
                    id_nou=verificare_numar_eror(var);
                    verificare_id(id_nou);

                    cout<<"Introdu cantitatea produsului: ";
                    getline(cin, var);
                    cantitate_nou=verificare_numar_eror(var);
                    verificare_cantitate(cantitate_nou);

                    cout<<"Introdu tipul produsului: ";
                    getline(cin, var);
                    tip_nou=verificare_numar_eror(var);
                    verificare_tip(tip_nou);

                    cout<<"Introdu daca produsul este eligibil pentru BOGO 1-true, 0- flase: ";
                    getline(cin, var);
                    int contor=verificare_numar_eror(var);
                    verificare_bogo(contor);
                    if(contor==0) bogo_eligibil_nou = false;
                    else if(contor ==1) bogo_eligibil_nou = true;
                    contor_while=0;
                }
                 catch (const ExceptieProdusInvalid& e) 
                {
                    cout << "Eroare: " << e.what() << endl;
                    salvaEroareInJurnal(e.what(), "ExceptieProdusInvalid");
                    
                }
                
            }
        flo.addProductToStore(Florarie(nume_produs_nou, pret_nou, id_nou, cantitate_nou, tip_nou, bogo_eligibil_nou));
        numar_produse++;
    
        }
        else 
        {
            cout<<"Cod incorect!";
        }
    }
    else
    {
        cerr << "Eroare: Input invalid sau numărul nu este pozitiv!" << endl;
    }
    
}


bool esteNumar(const string& sir) {
    for (char c : sir) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// Funcție pentru verificarea numărului cardului
bool verificaNumarCard(const string& numar_card) {
    return numar_card.length() == 16 && esteNumar(numar_card);
}

// Funcție pentru verificarea CVC-ului
bool verificaCVC(const string& cvc) {
    return cvc.length() == 3 && esteNumar(cvc);
}

// Funcție pentru verificarea datei de expirare
bool verificaDataExpirare(int luna, int an) {
    // Obținem data curentă
    time_t t = time(0);
    tm* now = localtime(&t);
    int luna_curenta = now->tm_mon + 1; // luni între 0 și 11, așa că adăugăm 1
    int an_curent = now->tm_year + 1900; // anii sunt de la 1900

    // Verificăm dacă data de expirare este validă
    if (an < an_curent) return false;
    if (an == an_curent && luna < luna_curenta) return false;

    return luna >= 1 && luna <= 12;
}

void cont_bancar(){

    string numar_card, cvc;
    int luna_expirare, an_expirare;

    int contor_while=0;

    while(contor_while==0) 
    {

        try{

        // Introducem detaliile cardului
        cout << "Introduceti numarul cardului (16 cifre): ";
        cin >> numar_card;
        if (!verificaNumarCard(numar_card)) 
            throw  ExceptiePlataInvalida("Numarul cardului este invalid! Trebuie sa contina exact 16 cifre.\n");
        
        cout << "Introduceti codul CVC (3 cifre): ";
        cin >> cvc;
        if (!verificaCVC(cvc)) 

            throw  ExceptiePlataInvalida("CVC-ul este invalid! Trebuie sa contina exact 3 cifre.\n");
        cout << "Introduceti luna expirarii (1-12): ";
        cin >> luna_expirare;

        cout << "Introduceti anul expirarii (ex. 2025): ";
        cin >> an_expirare;
        if (!verificaDataExpirare(luna_expirare, an_expirare)) 
            throw  ExceptiePlataInvalida("Data de expirare este invalida sau cardul a expirat.\n");
        
        cout << "Plata a fost efectuata cu succes, va multumim pentru cumparatura!\n";
        contor_while = 1;
        } catch (ExceptiePlataInvalida& e) 
        {
            cout << "Eroare: " << e.what() << endl;
            salvaEroareInJurnal(e.what(), "ExceptiePlataInvalida");
        }

    }
    
}

void social_reducere(int* reducere_sociala,float* suma){
    

    if(*suma!=0)
    {
        if(reducere_sociala[0]==1 && reducere_sociala[1]==1 && reducere_sociala[2]==1)
        {
            cout<<"Din pacate ati utilizat toate reducerile sau nu aveti suma destul de mare(\n"; 
        }
        else{
            if(reducere_sociala[0]==0)
            {
                cout<<"Doriti sa obineti o reducere in valoare de 10 lei pentru ca ati dat like paginii noastre?\n";
                string raspuns_temp;
                int raspuns;
                cout<<"1--Da\n2--Nu\n";
                getline(cin, raspuns_temp);
                raspuns=verificare_numar(raspuns_temp);
                if(raspuns==1)
                {


                    try {
                            if (10 >= *suma) {
                                throw ExceptieDiscountInvalid("Eroare: Suma negativa sau 0!");
                            }
                            cout<<"Felicitari, ati obtinut o reducere de 10 lei\n";
                            *suma-=10;
                        } catch (const ExceptieDiscountInvalid& e) {
                             cout << "Eroare: " << e.what() << endl;

                            salvaEroareInJurnal(e.what(), "ExceptieStocInsuficient");
                        }
                    
                    reducere_sociala[0]=1;


                }
                else if(raspuns==-1)
                    cerr << "Eroare: Input invalid sau numărul nu este pozitiv!" << endl;
                    
            }
            if(reducere_sociala[1]==0)
            {
                cout<<"Doriti sa obineti o reducere in valoare de 10 lei pentru ca ati dat share paginii noastre?\n";
                string raspuns_temp;
                int raspuns;
                cout<<"1--Da\n2--Nu\n";
                getline(cin, raspuns_temp);
                raspuns=verificare_numar(raspuns_temp);
                if(raspuns==1)
                {


                    try {
                            if (10 >= *suma) {
                                throw ExceptieDiscountInvalid("Eroare: Suma negativa sau 0!");
                            }
                            cout<<"Felicitari, ati obtinut o reducere de 10 lei\n";
                            *suma-=10;
                        } catch (const ExceptieDiscountInvalid& e) {
                             cout << "Eroare: " << e.what() << endl;

                            salvaEroareInJurnal(e.what(), "ExceptieStocInsuficient");
                        }
                    
                    reducere_sociala[1]=1;


                }
                else if(raspuns==-1)
                    cerr << "Eroare: Input invalid sau numărul nu este pozitiv!" << endl;
            }
            if(reducere_sociala[2]==0)
            {
                cout<<"Doriti sa obineti o reducere in valoare de 10 lei pentru ca ati dat follow paginii noastre?\n";
                string raspuns_temp;
                int raspuns;
                cout<<"1--Da\n2--Nu\n";
                getline(cin, raspuns_temp);
                raspuns=verificare_numar(raspuns_temp);
                if(raspuns==1)
                {

                    try {
                            if (10 >= *suma) {
                                throw ExceptieDiscountInvalid("Eroare: Suma negativa sau 0!");
                            }
                            cout<<"Felicitari, ati obtinut o reducere de 10 lei\n";
                            *suma-=10;
                        } catch (const ExceptieDiscountInvalid& e) {
                             cout << "Eroare: " << e.what() << endl;

                            salvaEroareInJurnal(e.what(), "ExceptieStocInsuficient");
                        }
                    
                    reducere_sociala[2]=1;


                }
                else if(raspuns==-1)
                    cerr << "Eroare: Input invalid sau numărul nu este pozitiv!" << endl;
            }
        }
    }else cout<<"Inca nu ati cumparat nimic\n";
}

int main(){


    Store flo;

    // Adăugăm produsele în magazin
    flo.addProductToStore(Florarie("Zamioculcas Zamiifolia", 70, 4401, 7, 0, true));
    flo.addProductToStore(Florarie("Delosperma echinatum", 80, 4402, 4, 0, false));
    flo.addProductToStore(Florarie("Craciunita cactus", 44, 4403, 10, 0, false));
    flo.addProductToStore(Florarie("Ficus lyrata Bambino XL", 68, 4404, 8, 0, false));
    flo.addProductToStore(Florarie("Monstera", 50, 4405, 9, 0, true));
    flo.addProductToStore(Florarie("Bonsai Carmona", 55, 4406, 3, 0, false));
    flo.addProductToStore(Florarie("Orhidee fluture", 70, 4407, 9, 0, false));
    flo.addProductToStore(Florarie("Palmier Areca", 85, 4408, 4, 0, false));
    flo.addProductToStore(Florarie("Planta ZZ", 100, 4409, 12, 0, true));
    flo.addProductToStore(Florarie("Crinul păcii", 125, 4410, 9, 0, true));

    // Adăugăm produsele suplimentare
    flo.addProductToStore(Florarie("Mini Tuia", 55, 5501, 10, 1, false));
    flo.addProductToStore(Florarie("Petunia Alba", 23, 5502, 9, 1, false));
    flo.addProductToStore(Florarie("Petunia Roz", 102, 5503, 3, 1, true));
    flo.addProductToStore(Florarie("Petunia Violeta", 120, 5504, 6, 1, false));
    flo.addProductToStore(Florarie("Thuja Smaragd", 80, 5505, 9, 1, true));
    flo.addProductToStore(Florarie("Begonia rosie", 50, 5506, 12, 1, false));
    flo.addProductToStore(Florarie("Calluna trio mini", 90, 5507, 12, 1, true));
    flo.addProductToStore(Florarie("Liliac japonez Rubella", 100, 5508, 8, 1, false));
    flo.addProductToStore(Florarie("Lavanda mini", 88, 5509, 10, 1, true));
    flo.addProductToStore(Florarie("Buxus", 80, 5510, 5, 1, false));

    flo.addProductToStore(Florarie("Nutrient universal", 20, 6001, 9, 2, false));
    flo.addProductToStore(Florarie("Ghiveci ceramic", 35, 6002, 11, 2, false));
    flo.addProductToStore(Florarie("Tija suport", 12, 6003, 5, 2, false));
    flo.addProductToStore(Florarie("Piatra decorativa", 45, 6004, 4, 2, false));
    flo.addProductToStore(Florarie("Fertilizant pentru cactusi", 25, 6005, 10, 2, false));
    flo.addProductToStore(Florarie("Substrat universal", 50, 6006, 2, 2, false));
    flo.addProductToStore(Florarie("Ingrasamant lichid", 30, 6007, 10, 2, false));
    flo.addProductToStore(Florarie("Pulverizator", 40, 6008, 2, 9, false));
    flo.addProductToStore(Florarie("Tavita drenaj", 15, 6009, 2, 7, false));
    flo.addProductToStore(Florarie("Granule pentru sol", 28, 6010, 6, 2, false));

    int numar_produse_flori=30;
    ifstream inFile("data.json");
    nlohmann::json jsonData;
    // Citește datele din fișierul JSON
    if (inFile.is_open()) {
        inFile >> jsonData;
        inFile.close();
    } else {
        cout << "Nu am putut deschide fișierul!" << endl;
        return 1;
    }
    string reduceri[5] = {"UTM5", "lab2", "flori9", "dai3", "--7"};
	int red[5] = {5, 2, 9, 3, 7};
    int contor_reduceri=0;

	Client* user=new Client(0, 0, "", 0, 0, 0);
	int contor = 0;
	int contor_suma=-1,contor_reducere=0;    
    user->login(jsonData);
    !(*user);
    int reducere_sociala[3]={0,0,0};
    Order<Florarie> order(*user);
	while(contor != 9) {
	    if(contor == 0) contor = meniu(&contor_reduceri);
	    else if(contor == 1) afisare_plante(flo, &contor,0,*user,numar_produse_flori);
        else if(contor == 3) afisare_plante(flo, &contor,2,*user,numar_produse_flori);
	    else if(contor == 4) {cautare_dupa_ID(flo, numar_produse_flori); contor=0;}
	    else if(contor == 2) afisare_plante(flo, &contor,1,*user,numar_produse_flori);
	    else if(contor == 5) {!(*user); suma_plata(*user,flo , &contor_suma,numar_produse_flori); contor=0;}
		else if(contor == 6) {gradare_pret(flo, *user,numar_produse_flori); contor=0;}
        else if(contor == 7) {reducere(*user,&contor_reducere); contor=0;}//only_   dev(&flo, &numar_produse_flori);
        else if(contor == 10) { only_dev(flo, numar_produse_flori); contor=0;}
        else if(contor == 11) {social_reducere(reducere_sociala,&user->suma); contor=0;}
        else if(contor == 8) {reducere_cod(*user,reduceri,&contor_reduceri,red,contor_reduceri,&user->r_suma); contor=0;}
	    else if(contor == -1) {cerr << "Eroare: Input invalid sau numărul nu este pozitiv!" << endl; contor=0;}
	    else {cout<<"Out of range"; contor=0;}
	}
    if(user->suma!=0)
    {
        order.generateInvoice(*user, flo.getInventory().getProducts());
        user->payOrder();
    }
    ++(*user);
    -(*user);
    user->scriere_json(jsonData);

    delete user;
	return 0;
}