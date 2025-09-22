#include <iostream>
#include<cstring>
#include<ctime>
using namespace std;

void randomCharArray(char* arr, int length) {
    const char charset[] = "abcdef ghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQR STUVWXYZ";
    const int charsetSize = sizeof(charset) - 1;

    srand(time(0)); 

    for (int i = 0; i < length; ++i) {
        arr[i] = charset[rand() % charsetSize];
    }

    arr[length] = '\0'; 
}



struct Book {
    char* ime_knjige;
    char* ime_autora;
    int broj_ocjena;
    int* ocjene;

    void unos() {
        ime_knjige = new char[51];
        randomCharArray(ime_knjige, 50);
        ime_autora = new char[51];
        cout << "Unesite ime autora: " << endl;
        cin.ignore();
        cin.getline(ime_autora,51);
        cout << "Unesite broj ocjena: " << endl;
        cin >> broj_ocjena;
        cin.ignore();
        ocjene = new int[broj_ocjena];
        cout << "Unesite ocjene: ";
        for (int i = 0; i < broj_ocjena; i++)
        {
            cin>>*(ocjene + i);
        }
    };
    void ispis() {
        cout << "Ime knjige: " << ime_knjige << endl;
        cout << "Ime autora: " << ime_autora << endl;
        cout << "Broj ocjena: " << broj_ocjena << endl;
        cout << "Ocjene: " << endl;
        for (int i = 0; i < broj_ocjena; i++)
        {
            cout << *(ocjene + i) << " ";
        }
        cout << endl;
    };
    void dealokacija() {
        delete[]ime_autora;
        delete[]ime_knjige;
        delete[]ocjene;
    };
    float prosjek_ocjena() {
        float prosjek=0;
        for (int i = 0; i < broj_ocjena; i++)
        {
            prosjek += *(ocjene + i);
        }
        return prosjek / broj_ocjena;
    }
};
int broj_rijeci(char* imeKnjige) {
    int brojac = 0;
    for (int i = 0; *(imeKnjige+i)!= '\0'; i++)
    {
        if (*(imeKnjige+i)==' ' || *(imeKnjige+i)=='\t' || *(imeKnjige+i)=='\n')
        {
            brojac++;
        }
    }
    return brojac;
}

void najboji_prosjek(Book* niz, int broj_knjiga) {
    float best_prosjek = 0;
    float prosjek=0;
    int indeks=0;
    for (int i = 0; i < broj_knjiga; i++)
    {
        prosjek = (niz + i)->prosjek_ocjena();
        if (prosjek>best_prosjek)
        {
            best_prosjek = prosjek;
            indeks = i;
        }
    }
    cout << "Knjiga s najboljim prosjekom je knjiga br: " << indeks+1 << endl;
}


int main() {
    int vel;
    cout << "Unesite broj knjiga: " << endl;
    cin >> vel;
    Book* knjige = new Book[vel];
    for (int i = 0; i < vel; i++)
    {
        (knjige + i)->unos();
        
    }
    for (int i = 0; i < vel; i++)
    {
        (knjige + i)->ispis();
        int brojanje = broj_rijeci((knjige+i)->ime_knjige);
        cout << "Broj rijeci u knjizi: " << brojanje << endl;
    }
    for (int i = 0; i < vel; i++)
    {
        (knjige + i)->dealokacija();
    }
    najboji_prosjek(knjige, vel);
}
