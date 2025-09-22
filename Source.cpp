#include<iostream>
#include<vector>
using namespace std;

class Ploca;

class Element {
protected:
    char boja;//boja elemenata
    int x, y;//pozicija na ploci
    Ploca* ploca;
public:
    Element(char b) : boja(b),x(0),y(0),ploca(nullptr){}

    Element(const Element& obj) {
        boja = obj.boja;
        x = obj.x;
        y = obj.y;
        ploca = obj.ploca;
    }

    Element& operator=(const Element& obj) {
        if (this!=&obj)
        {
            boja = obj.boja;
            x = obj.x;
            y = obj.y;
            ploca = obj.ploca;
        }
        return *this;
    }

    virtual ~Element() {}
    char getBoja() const { return boja; };
    int getPozicijaX() const { return x; };
    int getPozicijaY() const { return y; };

    virtual Element* getCopy() const = 0;

    virtual bool postaviSeNaPlocu(Ploca*, int, int) = 0;
    virtual bool pomjeriSeZa(int, int) = 0;
    virtual bool zauzima(int, int) = 0;
    virtual bool ukloniSaPloce() = 0;

};

class Ploca {
    int sirina, visina;
    vector <Element*> elementi; // pokazivači na elemente

public:
    Ploca(int s, int v) :sirina(s), visina(v) {}

    Ploca(const Ploca& obj) {
        sirina = obj.sirina;
        visina = obj.visina;
        for (auto e : obj.elementi) {
            elementi.push_back(e->getCopy());   // clone each element
        }
    }

    Ploca& operator=(const Ploca& obj) {
        if (this != &obj) {
            for (auto e : elementi) delete e;
            elementi.clear();

            sirina = obj.sirina;
            visina = obj.visina;
            for (auto e : obj.elementi) {
                elementi.push_back(e->getCopy());
            }
        }
        return *this;
    }


    ~Ploca() {
        elementi.clear();
    }

    int getSirina() const { return sirina; }
    int getVisina() const { return visina; }

    bool zauzeto(int pozX, int pozY) {
        for (auto el : elementi)
            if (el->zauzima(pozX, pozY)) return true;
        return false;
    }
    
    bool postaviElement(Element& element, int x, int y) {
        if (x < 0 || y < 0 || x >= sirina || y >= visina)return false;
        if (!element.postaviSeNaPlocu(this, x, y))return false;
        elementi.push_back(&element);
        return true;
    };
    bool ukloniElement(Element& element) {
        for (auto it=elementi.begin(); it!=elementi.end();++it)
        {
            if ((*it)->zauzima(element.getPozicijaX(),element.getPozicijaY()))
            {
                if (element.ukloniSaPloce()) {
                    elementi.erase(it);
                    return true;
                }
                false;
            }
        }
        return false;
    };
    bool pomjeriElementZa(Element& element, int pomijeranjeX, int pomijeranjeY) {
        for (auto elem : elementi)
        {
            if (elem==&element)
            {
                return elem->pomjeriSeZa(pomijeranjeX, pomijeranjeY);
            }
        }
        return false;
    };

    friend ostream& operator<<(ostream& COUT, const Ploca& obj) {
        for (size_t i = 0; i < obj.visina; i++)
        {
            for (size_t j = 0; j < obj.sirina; j++)
            {
                bool zauzeto = false;
                for (auto elem : obj.elementi)
                {
                    if (elem->zauzima(j,i))
                    {
                        COUT << elem->getBoja();
                        zauzeto = true;
                        break;
                    }
                }
                if (!zauzeto) COUT << ".";
                {

                }
            }
            COUT << "\n";
        }
        return COUT;
    };
};

class Tacka : public Element {
public:
    Tacka(char b):Element(b){}
    bool postaviSeNaPlocu(Ploca* tabla, int tX, int tY) override {
        if (tX < 0 || tY < 0 || tX >= tabla->getSirina() || tY >= tabla->getVisina()) return false;
        if (tabla->zauzeto(tX, tY)) return false;
        ploca = tabla;
        x = tX;
        y = tY;
        return true;
    };

    bool zauzima(int tX, int tY) override {
        return(tX == x && tY == y);
    };

    bool ukloniSaPloce() override {
        if (!ploca)return false;
        ploca = nullptr;
        x = y = -1;
        return true;
    };
    bool pomjeriSeZa(int pomjeriX, int pomjeriY)override {
        if (!ploca)return false;
        int novaPozicijaX = x + pomjeriX;
        int novaPozicijaY = y + pomjeriY;
        if (ploca->zauzeto(novaPozicijaX, novaPozicijaY) && !zauzima(novaPozicijaX, novaPozicijaY)) return false;
        if (novaPozicijaX < 0 || novaPozicijaY < 0 || novaPozicijaX >= ploca->getSirina() || novaPozicijaY >= ploca->getVisina())return false;
        x = novaPozicijaX;
        y = novaPozicijaY;
        return true;
    };

    Element* getCopy() const override{ return new Tacka(*this); };

};

class Pravougaonik :public Element {
protected:
    int sirina;
    int visina;
public:
    Pravougaonik(char b, int s,int v):Element(b), sirina(s),visina(v){}

    bool postaviSeNaPlocu(Ploca* tabla, int pX, int pY) override {
        if (pX<0 || pY<0 || pX + sirina>tabla->getSirina() || pY + visina>tabla->getVisina())return false;
        for (size_t i = 0; i < sirina; i++) {
            for (size_t j = 0; j < visina; j++)
            {
                if (tabla->zauzeto(pX + i, pY + j))return false;
            }
        }
        ploca = tabla;
        x = pX;
        y = pY;
        return true;
    };
    bool zauzima(int pX, int pY) override {
        return (ploca != nullptr && pX >= x && pY >= y && pX < x + sirina && pY < y + visina);
    };
    bool ukloniSaPloce() override {
        if (!ploca)return false;
        ploca = nullptr;
        x = y = -1;
        return true;
    };
    bool pomjeriSeZa(int pomjeriX, int pomjeriY)override {
        if (!ploca)return false;
        int novaPozicijaX = x + pomjeriX;
        int novaPozicijaY = y + pomjeriY;
        for (size_t i = 0; i < sirina; i++) {
            for (size_t j = 0; j < visina; j++)
            {
                if (ploca->zauzeto(novaPozicijaX + i, novaPozicijaY + j)&& !zauzima(novaPozicijaX + i, novaPozicijaY + j))return false;
            }
        }
        if (novaPozicijaX < 0 || novaPozicijaY < 0 || novaPozicijaX + sirina>= ploca->getSirina() || novaPozicijaY + visina>= ploca->getVisina())return false;
        x = novaPozicijaX;
        y = novaPozicijaY;
        return true;

        
    };

    Element* getCopy() const override { return new Pravougaonik(*this); };

};

class OblikL : public Element {
    int sirina, visina;
public:
    OblikL(char b, int s, int v) : Element(b), sirina(s), visina(v) {}
    bool postaviSeNaPlocu(Ploca* tabla, int oblikX, int oblikY) override {
        if (oblikX < 0 || oblikY < 0 || oblikX + sirina > tabla->getSirina() || oblikY + visina > tabla->getVisina())
            return false;
        for (size_t i = 0; i < sirina; i++)
        {
            if (tabla->zauzeto(oblikX+i,oblikY)) return false;
        }
        for (size_t i = 0; i < sirina; i++)
        {
            if (tabla->zauzeto(oblikX, oblikY+i)) return false;
        }
        ploca = tabla;
        x = oblikX;
        y = oblikY;
        return true;
    };
    bool zauzima(int oblikX, int oblikY) override {
        if (oblikX == x && (oblikY >= y && oblikY < y + visina)) return true;
        if (oblikY == y && (oblikX>= x && oblikX < x + sirina)) return true;
        return false;
    };
    bool ukloniSaPloce() override {
        if (!ploca)return false;
        ploca = nullptr;
        x = y = -1;
        return true;
    };
    bool pomjeriSeZa(int pomjeriX, int pomjeriY) override {
        if (!ploca)return false;
        int novaPozicijaX = x + pomjeriX;
        int novaPozicijaY = y + pomjeriY;

        for (size_t i = 0; i < sirina; i++)
        {
            if (ploca->zauzeto(novaPozicijaX + i, novaPozicijaY) && !zauzima(novaPozicijaX + i, novaPozicijaY))return false;
        }
        for (size_t i = 0; i < visina; i++)
        {
            if (ploca->zauzeto(novaPozicijaX, novaPozicijaY + i) && !zauzima(novaPozicijaX, novaPozicijaY + i))return false;
        }
        x = novaPozicijaX;
        y = novaPozicijaY;
        return true;
    };

    Element* getCopy() const override { return new OblikL(*this); };

};

const char* crt = "\n-------------------------------------------\n";

int main() {
    Ploca tabla(12, 8);
    Tacka tacka('X');
    Pravougaonik pravougaonik('P', 3, 2);
    OblikL oblik('L', 4, 4);
    

    cout << "Postavljanje oblika na tablu..." << crt;
    if (!tabla.postaviElement(tacka, 5, 5)) cout << "Tacka ne moze biti postavljena na tu poziciju!" << endl;
    if (!tabla.postaviElement(pravougaonik, 2, 2)) cout << "Pravougaonik ne moze biti postavljena na tu poziciju!" << endl;
    if (!tabla.postaviElement(oblik, 7, 1)) cout << "OblikL ne moze biti postavljena na tu poziciju!" << endl;

    cout << tabla << crt;

    cout << "Postavljanje elemenata jednog preko drugog..." << crt;

    Tacka tacka2('Y');
    Pravougaonik pravougaonik2('T', 3, 2);
    OblikL oblik2('U', 4, 4);


    if (!tabla.postaviElement(tacka2, 5, 5)) cout << "Tacka ne moze biti postavljena na tu poziciju!"<<endl
        <<"Pozicija (5,5) je vec zauzeta" << endl;
    if (!tabla.postaviElement(pravougaonik2, 2, 2)) cout << "Pravouganoik ne moze biti postavljena na tu poziciju!" << endl
        << "Pozicija (2,2) je vec zauzeta" << endl;
    if (!tabla.postaviElement(oblik2, 7, 1)) cout << "OblikL ne moze biti postavljena na tu poziciju!" << endl
        << "Pozicija (7,1) je vec zauzeta" << endl;

    cout << tabla << crt;
    
    cout << "Pomijeramo tacku lijevo..."<<crt;
    if (!tacka.pomjeriSeZa(-2, 0)) cout << "Tacka se ne moze pomjeriti!" << endl;;
    cout << tabla << crt;

    cout << "Pomijeramo pravougaonik dole..."<<crt;
    if (!pravougaonik.pomjeriSeZa(0, 2)) cout << "Pravougaonik se ne moze pomjeriti!"<<endl;
    cout << tabla << crt;

    cout << "Pomijeramo oblikL desno..."<<endl;
    if (!oblik.pomjeriSeZa(1, 0)) cout << "OblikL se ne moze pomjeriti!"<<endl;
    cout << tabla << crt;

    // kretanje koje se sudara
    cout << "Pomijeramo tacku prema pravougaoniku..."<<crt;
    if (!tacka.pomjeriSeZa(0, 0)) cout << "Sudaranje! Tacka se nije pomjerila!" << endl;
    cout << tabla << crt;

    cout << "Pomijeramo oblik prema tacki..." << crt;
    if (!oblik.pomjeriSeZa(-5, 4)) cout << "Sudaranje! Oblik se nije pomjerio!"<<endl;
    cout << tabla << crt;

    cout << "Pomijeramo pravougaonik prema obliku..." << crt;
    if (!pravougaonik.pomjeriSeZa(3, 1)) cout << "Sudaranje! Pravougaonik nije pomjeren!" << endl;
    cout << tabla << crt;

    return 0;
}