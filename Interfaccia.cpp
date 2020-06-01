
#include "Interfaccia.h"
#include "vector"
#include <iterator>
#include "string.h"
#include "algorithm"
#include "iostream"
#include "Tabella.h"
using namespace std;
Interfaccia::Interfaccia() {
    _autoincrement=false;
    _not_null=false;
    _chiave_trovata=false;


}

Table Interfaccia::definizioneOperazione(const string &operazione, istream &is) {
    string buffer;
    if (operazione == "CREATE") {
        is>>buffer;
        if(buffer == "TABLE")
            return CREATE_TABLE;
    }
    if (operazione == "DELETE") {
        is>>buffer;
        if(buffer == "FROM")
            return DELETE_FROM;
    }
    if (operazione == "TRUNCATE") {
        is>>buffer;
        if(buffer == "TABLE")
            return TRUNCATE_TABLE;
    }
    if (operazione == "UPDATE")
        return UPDATE;
    if (operazione == "SELECT")
        return SELECT;
    if (operazione == "QUIT()")
        return QUIT;
    if (operazione == "INSERT") {
        is>>buffer;
        if(buffer == "INTO")
            return INSERT_INTO;
    }
    if (operazione == "DROP") {
        is>>buffer;
        if(buffer == "TABLE")
            return DROP_TABLE;
    }
    throw Operazione_Non_Presente();

    //throw domain_error("operazione non presente");
}


// throw invalid_argument("Op non presente");


TIPI Interfaccia::definizioneTipo(const string &tipo){

    if(tipo=="INT")
        return INT;
    if(tipo=="FLOAT")
        return FLOAT;
    if(tipo=="DATA")
        return DATA;
    if(tipo=="TIME")
        return TIME;
    if(tipo=="TEXT")
        return TEXT;
    throw Campo_Non_Corretto();
    //throw invalid_argument("tipo non presente");

}
void Interfaccia::verificaNomeRiga(const string &nome){ //verifica se il nome della riga è presente nel vector NOMI
    for(auto elem:_NOMI)
    {
        if(elem==nome)
            throw Campo_Non_Corretto();
        //throw invalid_argument("campo ripetuto");
    }

}

void Interfaccia::verificaAutoIncrementNotNull(const string &parametro){  //verifica se è stato inserito autoincrement o not null
    int caso=0;
    if(parametro=="NOT NULL")
        caso=1;
    if(parametro=="AUTO_INCREMENT")
        caso=2;
    if(parametro!="AUTO_INCREMENT"&&parametro!="NOT NULL")
        throw Campo_Non_Corretto();
    //throw invalid_argument("stringa non corretta");

    switch(caso)
    {
        case (1):_not_null=true; break;
        case(2):_autoincrement= true;break;
    }
}

void Interfaccia::verificaFormatoChiave(const string & chiave) {
    int lunghezza = 0;
    bool trova = false;
    lunghezza = chiave.size();
    if ((chiave[0] != '(') &&
        ((chiave[lunghezza - 1] != ')') || (((chiave[lunghezza - 2]) != ')') && (chiave[lunghezza - 1] == ','))))
        throw Formato_Chiave_Errato();
    if (_chiave_trovata == true)
        throw invalid_argument("chiave già inserita");
    else {
        _chiave_trovata = true;
        _chiave = chiave;
        _chiave = _chiave.erase(lunghezza - 1);
        _chiave = _chiave.erase(0, 1);
        if (_chiave.back() == ')') {
            _chiave.resize(_chiave.length() - 1);
        }
        for (auto elem : _NOMI) {
            if (elem == _chiave)
                trova = true;
        }
        if (trova != true) {
            throw invalid_argument("la chiave non è presente");
        }

        spostaChiave();
    }
}

//throw invalid_argument ("formato chiave errato")
void Interfaccia::spostaChiave() {
    int pos_chiave=0;
    pos_chiave=find(_NOMI.begin(),_NOMI.end(),_chiave)-_NOMI.begin();
    if(pos_chiave<0)
        throw invalid_argument("chiave non presente nella tabella");
    if(!_NOT_NULL[pos_chiave] && !_AUTO[pos_chiave])
    {
        throw invalid_argument("La chiave deve essere un campo specificato come AUTO_INCREMENT o NOT NULL");
    }
    swap(_NOMI[0],_NOMI[pos_chiave]);
    swap(_NOT_NULL[0],_NOT_NULL[pos_chiave]);
    swap(_AUTO[0],_AUTO[pos_chiave]);
    swap(_TIPI[0],_TIPI[pos_chiave]);
}
void Interfaccia::leggiRiga(const string &nome, string &tipo, const string &parametro1, string &parametro2) { //legge riga nel caso abbia tutti i campi
    _autoincrement=false;
    _not_null=false;
    if(parametro1==parametro2)
        //throw Campo_Ripetuto();
        throw invalid_argument("campo ripetuto");
    verificaNomeRiga(nome);
    if(parametro2.back()==',')
        parametro2.resize(parametro2.size()-1);
    _t=definizioneTipo(tipo);
    verificaAutoIncrementNotNull(parametro1);
    verificaAutoIncrementNotNull(parametro2);
    _NOMI.push_back(nome);
    _TIPI.push_back(_t);
    _NOT_NULL.push_back(_not_null);
    _AUTO.push_back(_autoincrement);
}

void Interfaccia::leggiRiga(const string &nome, string &tipo) { //legge riga nel caso abbia solo i primi due campi
    _autoincrement=false;
    _not_null=false;
    verificaNomeRiga(nome);
    if(tipo.back()==',')
        tipo.resize(tipo.size()-1);
    _t=definizioneTipo(tipo);
    _NOMI.push_back(nome);
    _TIPI.push_back(_t);
    _NOT_NULL.push_back(_not_null);
    _AUTO.push_back(_autoincrement);


}

void Interfaccia::leggiRiga(const string &nome, const string &tipo,  string &parametro) { //legge riga nel caso abbia 3 campi
    _autoincrement=false;
    _not_null=false;
    verificaNomeRiga(nome);
    if(parametro.back()==',')
        parametro.resize(parametro.size()-1);
    _t=definizioneTipo(tipo);
    verificaAutoIncrementNotNull(parametro);
    _NOMI.push_back(nome);
    _TIPI.push_back(_t);
    _NOT_NULL.push_back(_not_null);
    _AUTO.push_back(_autoincrement);


}

vector<string> Interfaccia::ritornoNomi() {  //ritorno dei vector per la creazione della tabella
    return _NOMI;

}

vector<bool> Interfaccia::ritornoAuto() {
    return _AUTO;
}

vector<bool> Interfaccia::ritonrnoNot() {
    return _NOT_NULL;
}

vector<TIPI> Interfaccia::ritornoTipi() {
    return _TIPI;
}

vector<Tabella> Interfaccia::dropTable( string &nometabella, vector<Tabella> & tab,istream & is) {
    char cbuffer;
    int pos=0;
    vector<Tabella>:: iterator it;
    if(nometabella[nometabella.size()-1]!=';')
    {
        is>>cbuffer;
        if(cbuffer!=';')
            throw invalid_argument("Manca il ';'");
    }
    else{
        nometabella.resize(nometabella.size()-1);
    }
    it=find(tab.begin(),tab.end(),nometabella);
    pos=tab.begin()-it;
    if(pos==tab.size())
    {
        throw invalid_argument("tabella non esistente");
    }
    tab.erase(it);
    return tab;
}

Interfaccia::~Interfaccia() {
    _TIPI.clear();
    _NOMI.clear();
    _AUTO.clear();
    _NOT_NULL.clear();
    _chiave.clear();

}

void Interfaccia::cancellaInterfaccia() {
    _TIPI.clear();
    _NOMI.clear();
    _AUTO.clear();
    _NOT_NULL.clear();
    _chiave.clear();
    _autoincrement=false;
    _not_null=false;
    _chiave_trovata=false;
    _chiusura= false;

}
bool Interfaccia::trovaVirgola(const string &campo, const char &ch) const {
    if(campo.back()==',' || ch==',')
        return true;
    else
        return false;


}
string Interfaccia::leggiStringa(string &sbuffer, char &cbuffer, istream &is) const {

    if(sbuffer.back() == '\"')    //controlla se l'ultimo carattere è una "
        return sbuffer;
    else {
        if (sbuffer.find_last_of('\"')>0 && !sbuffer.empty())      //controlla che le virgolette non siano in mezzo alla stringa
            throw invalid_argument("stringa inserita non valida");
        else {
            if (sbuffer.empty())
                sbuffer.push_back('\"');
            do {
                is.get(cbuffer);
                sbuffer.push_back(cbuffer);
            } while(cbuffer!='\"');     //legge finchè non trova "
            is.get(cbuffer);    //controlla il carattere dopo "
            return sbuffer;
        }
    }
}
vector<string> Interfaccia::leggiCampi(istream &is,const bool &leggivalori) const {
    vector<string> result;
    char cbuffer;
    is>>cbuffer;
    if (cbuffer!='(')   //verifica che viene aperta la parentesi
        throw invalid_argument("APERTURA NON EFFETTUATA CORRETTAMENTE");
    bool virgola=false,punto_e_virgola=false;
    string sbuffer;
    if(leggivalori) {
        is >> cbuffer;
        if (cbuffer == '\"') {
            sbuffer = leggiStringa(sbuffer, cbuffer, is);
            result.push_back(sbuffer);
            virgola = cbuffer == ',';
            if (cbuffer!=')' && !virgola) {
                is >> cbuffer;
                if (cbuffer==',') {
                    virgola = true;
                    is >> sbuffer;
                }
                else {
                    is >> sbuffer;
                    sbuffer = cbuffer + sbuffer;
                }
            }
            else
                is >> sbuffer;
        } else {
            is >> sbuffer;
            sbuffer = cbuffer + sbuffer;
        }
    }
    else
        is>>sbuffer;
    if(sbuffer.back()==';' && leggivalori) {
        punto_e_virgola=true;
        sbuffer.pop_back();     //elimina il punto e virgola se è attaccato per non dare problemi al ciclo successivo
    }
    while(((sbuffer.back()!=')') || (leggivalori && sbuffer.front()=='\"')) && (cbuffer!=')')) {    //cicla finche una striga che termina con un ")" oppure ha letto un singolo carattere ")"
        if (virgola && (sbuffer.front()==','))  //controlla che non si verifichi campo1, ,campo2
            throw invalid_argument("argomento mancante");
        virgola=false;
        cbuffer=sbuffer.back();     //acquisisce l'ultimo carattere poichè se è una virgola il getline lo cancella
        istringstream ibuffer(sbuffer);
        while(getline(ibuffer,sbuffer,',')) {     //trova tutte le stringhe delimitate dalla ","
            if (!leggivalori) {     //se sta leggendo i campi deve verificare che non siano passati campi uguali
                if (find(result.begin(), result.end(), sbuffer) == result.end()) {
                    if (!sbuffer.empty())      //controlla che l'utente non ha inserito ",," che la getline identifica come stringa vuota
                        result.push_back(sbuffer);
                    else
                        throw invalid_argument("argomento mancante");
                }
                else
                    throw invalid_argument("non possono essere inseriti due campi uguali");
            }
            else {
                if(sbuffer.front()=='\"') {
                    ibuffer.unget();
                    ibuffer.get(cbuffer);
                    if (cbuffer==',')
                        sbuffer=sbuffer+cbuffer;
                    result.push_back(leggiStringa(sbuffer, cbuffer, is));
                }
                else {
                    if (!sbuffer.empty())
                        result.push_back(sbuffer);
                    else
                        throw invalid_argument("argomento mancante");
                }
            }
        }
        if(cbuffer==',') {
            virgola = true;
            is >> sbuffer;
        }
        else {
            if (cbuffer!=')') {
                is>>cbuffer;
                if((cbuffer!=',') && (cbuffer!=')'))    //controlla che due campi sono separati dalla virgola oppure c'è la chiusura
                    throw invalid_argument("campi non passati correttamente");
                if (cbuffer==',') {
                    is.get(cbuffer);
                    virgola=(cbuffer==' ' || cbuffer==')');     //controlla che attaccata alla virgola non ci sia un campo poichè in quel caso virgola dev'essere assegnata come falsa
                    if (cbuffer!=')') {     //controlla se si deve leggere (se viene chiusa la parentesi non si deve leggere)
                        if(cbuffer!=' ') {      //controlla se è stato letto un campo che fa parte di una stringa
                            is>>sbuffer;
                            sbuffer=cbuffer+sbuffer;
                        }
                        else
                            is>>sbuffer;
                    }
                }
            }
        }
        if(sbuffer.back()==';' && leggivalori) {
            punto_e_virgola=true;
            sbuffer.pop_back();
        }
    }
    if (cbuffer!=')' && (!leggivalori || sbuffer.front()!='\"')) {
        sbuffer.pop_back();     //elimina la ")"
        if (!sbuffer.empty()) {
            if (virgola && (sbuffer.front()==','))
                throw invalid_argument("argomento mancante");
            virgola = (sbuffer.back() == ',' && (!leggivalori || sbuffer.front()!='\"'));
            istringstream ibuffer(sbuffer);
            while (getline(ibuffer, sbuffer, ','))     //trova tutte le stringhe delimitate dalla ,
                if (!leggivalori) {
                    if (find(result.begin(), result.end(), sbuffer) == result.end())    //controlla che i campi non sono stati gia inseriti
                        result.push_back(sbuffer);
                    else
                        throw invalid_argument("non possono essere inseriti due campi uguali");
                }
                else {
                    if(sbuffer.front()=='\"') {
                        result.push_back(leggiStringa(sbuffer, cbuffer, is));
                        if(cbuffer==' ')
                            is >> cbuffer;

                    }
                    else result.push_back(sbuffer);
                }
        }
    }
    if(leggivalori && !punto_e_virgola) {
        is >> cbuffer;
        if (cbuffer != ';')
            throw invalid_argument("chiusura non effettuata correttamente");
    }
    if(virgola)     //controlla che prima di "(" c'è stata una virgola
        throw invalid_argument("ultimo campo non presente");
    return result;
}

vector<size_t> Interfaccia::leggiWhere(istream &is, const Tabella &t) const {
    string campo, op, val1;
    char ch;
    is >> campo >> op;
    if (op == "<" || op == ">" || op == "=") {
        is >> val1;
        ch=val1.back();
        if(val1.back()==';')
            val1.pop_back();
        if(val1.front()=='\"') {
            if (val1.back()==';' && val1[campo.size()-2]=='\"') {
                val1.pop_back();
            }
            val1 = leggiStringa(val1, ch, is);
        }
        if (ch != ';') {
            is >> ch;
            if (ch != ';')
                throw invalid_argument("chiusura non effettuata correttamente");
        }
        return move(t.where(campo, op.front(), val1));
    }
    if (op == "<=" || op == ">=" || op == "<>") {
        is >> val1;
        ch=val1.back();
        if(val1.back()==';')
            val1.pop_back();
        if(val1.front()=='\"') {
            if (val1.back()==';' && val1[campo.size()-2]=='\"') {
                val1.pop_back();
            }
            val1 = leggiStringa(val1, ch, is);
        }
        if (ch != ';') {
            is >> ch;
            if (ch != ';')
                throw invalid_argument("chiusura non effettuata correttamente");
        }
        return move(t.where(campo, op.front(), op.back(), val1));
    }
    if (op == "BETWEEN") {
        string val2;
        is >> val1;
        if(val1.front()=='\"')
            val1 = leggiStringa(val1, ch, is);
        is >> op;
        if (op != "AND")
            throw domain_error("operazione non presente");
        is >> val2;
        ch=val2.back();
        if(val2.back()==';')
            val2.pop_back();
        if(val2.front()=='\"') {
            if (val2.back()==';' && val2[campo.size()-2]=='\"') {
                val2.pop_back();
            }
            val2 = leggiStringa(val2, ch, is);
        }
        if (ch != ';') {
            is >> ch;
            if (ch != ';')
                throw invalid_argument("chiusura non effettuata correttamente");
        }
        return move(t.where(campo, val1, val2));
    }
    throw domain_error("operazione non presente");
}

void Interfaccia::cancellaValoriTabella( string & nometabella,istream & is,vector<Tabella> & tab) {
    char ch;
    int pos=0;
    vector <size_t > res;
    if(nometabella.back()!=';')    //verifica il formato di nometabella
    {
        is>>ch;
        if(ch!=';')
        {
            throw domain_error("Manca ';'");
        }
    }
    else{
        nometabella.pop_back();
    }
    pos=trovaNomeTabella(nometabella,tab);
    if(tab[pos].contaRighe()==0)    //verifica che la tabella non  sia vuota
        throw domain_error("tabella vuota");

    /* for (int i=0;i<tab[pos].contaRighe();i++) //crea un vettore di posizioni con tutte le righe della tabella per poter di nuovo sfruttare la funzione di prima
     {
         res.push_back(i);
     }*/
    tab[pos].svuotaTabella();
}

void Interfaccia::cancellaValore(vector<size_t> &res, Tabella &tab) {
    int i=0;
    if(tab.contaRighe()==0)    //se la size di righe della tabella è 0 la tabella è gia vuota forse si può togliere(?)
        throw domain_error("tabella vuota");
    for(auto r:res)
    {
        tab.cancellaValoriRiga(r); //viene passato il valore di ogni posizioni per cancellare la riga corrispondente
    }


}

int Interfaccia::trovaNomeTabella(const string &nometabella,vector<Tabella> & tab) {
    if(tab.size()==0)
        throw invalid_argument("Non sono presenti tabelle");
    vector<Tabella>:: iterator it;
    int pos=0;
    it=find(tab.begin(),tab.end(),nometabella);
    pos=tab.begin()-it;
    if(pos==tab.size())
    {
        throw invalid_argument("tabella non esistente");
    }
    return pos;
}



