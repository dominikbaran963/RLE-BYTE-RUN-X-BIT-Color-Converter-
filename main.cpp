#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include <SDL/SDL.h>
#include<iostream>
#include<vector>
#include<fstream>
#include <string>
#include <string.h>
#include<windows.h>
#include <cstdlib>
#include <algorithm>







using namespace std;


///////////// Ustawienia ///////////////

 char identyfikator[] = "G2";
 string rozszerzenie = "meme";


///////////////////////////////////////

SDL_Surface* screen;
SDL_Rect* dstrect = new SDL_Rect;
SDL_Surface* oryginal_surface;
vector<SDL_Color> oryginal;
vector<int> aktualny_obraz;
int tryb = 0; //numer trybu to numer wywolujacego go przycisku
int width = 0;
int height = 0;
int przesuniecie_x=0;
int przesuniecie_y=0;

vector<vector<SDL_Color> > sectors;
vector<SDL_Color> palette;
enum Type{r,g,b};
int amountOfColors = 32;


struct tab_kolor{
    SDL_Color kolor;
    int licznik=0;
};
tab_kolor *kolory;
int r_kolory=0;
SDL_Color *dedykowana;
//paleta dedykowana
SDL_Color* paleta = new SDL_Color[32];
int palette_counter = 0;

SDL_Color standardowa[] = {
    {0,0,0},
    {255, 255, 255},
    {255,0,0},
    {250,128,114},
    {255,192,203},
    {199,21,133},
    {255,165,0},
    {255,127,80},
    {255,255,0},
    {240,230,140},
    {238,130,238},
    {148,0,211},
    {255,0,255},
    {75,0,130},
    {0,255,0},
    {34,139,34},
    {128,128,0},
    {0,139,139},
    {0,255,255},
    {70,130,180},
    {25,25,0112},
    {128,0,0},
    {245,222,179},
    {128,128,128},
    {47,79,79},
    {220,220,220},
    {240,255,255},
    {210,105,30},
    {255,215,0},
    {128,0,128},
    {0,100,0},
    {0,128,128}
};


SDL_Color szarosci[] = {

        {0 , 0 , 0},
        {8 , 8 , 8},
        {16 , 16 , 16},
        {24 , 24 , 24},
        {32 , 32 , 32},
        {40 , 40 , 40},
        {48 , 48 , 48},
        {56 , 56 , 56},
        {64 , 64 , 64},
        {72 , 72 , 72},
        {80 , 80 , 80},
        {88 , 88 , 88},
        {96 , 96 , 96},
        {104 , 104 , 104},
        {112 , 112 , 112},
        {120 , 120 , 120},
        {128 , 128 , 128},
        {136 , 136 , 136},
        {144 , 144 , 144},
        {152 , 152 , 152},
        {160 , 160 , 160},
        {168 , 168 , 168},
        {176 , 176 , 176},
        {184 , 184 , 184},
        {192 , 192 , 192},
        {200 , 200 , 200},
        {208 , 208 , 208},
        {216 , 216 , 216},
        {224 , 224 , 224},
        {232 , 232 , 232},
        {245 , 245 , 245},
        {255 , 255 , 255}
    };




void otworz_ze_sciezki(string sciezka);
void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);
SDL_Color getPixel(int x, int y);
SDL_Color getPixelSurface(int x, int y, SDL_Surface *surface);
void odczytaj_z_pliku(string sciezka);
vector<uint8_t> CompressWithByteRun(vector<int> source,int s_width,int s_height);
vector<int>* DecompressByteRun(vector<int> source);
void czyscEkran(Uint8 R, Uint8 G, Uint8 B);
void wyswietl();
void surface_na_wektor();
void wyswietl_oryginal();
void wyswietl_opcje_obrazu();
void wyswietl_opcje_obrazu(string sciezka,bool ismeme = false);
void zapisz_do_bmp();
vector<uint8_t> RLE_compress(vector<int> obraz);
vector<int> RLE_decompress(vector<int> obraz);
void wczytaj();
////
bool porownajKolory_dedykowana(SDL_Color k1);
void sprawdzKolor_dedykowane();
void sortowanie_babelkowe(tab_kolor tab[],int n);
void wybierz();
int najblizszy_kolor(SDL_Color k);
void wyswietl_dedykowany();
void szarosc(int BW , int x, int y);
void wyswietl_szary();
int najblizszy_kolor_standardowa(SDL_Color k);
void wyswietl_standard();
void medianCut();

void menu()
{
    cout<<"Program do obslugi formatu ."<<rozszerzenie<<endl<<endl;
    cout<<"Wybierz jeden z ponizszych obrazow lub wczytaj swoj plik"<<endl<<endl;
    cout<<"[1] star wars "<<endl;   //obrazek1.bmp
    cout<<"[2] sonic "<<endl;       //obrazek2.bmp
    cout<<"[3] mario "<<endl;       //obrazek3.bmp
    cout<<"[4] wczytaj plik .bmp lub ."<<rozszerzenie<<endl;

    string wybor;
    bool petla = true;
    while(petla){
      cout<<endl<<"Wybrana opcja: ";
      std::getline(std::cin, wybor);

        if(wybor == "1"){
                otworz_ze_sciezki("obrazek1.bmp");
                petla=false;
                break;
        }
        else if(wybor == "2"){
                otworz_ze_sciezki("obrazek2.bmp");
                petla=false;
                break;
        }
        else if(wybor == "3"){
                otworz_ze_sciezki("obrazek3.bmp");
                petla=false;
                break;
        }
        else if(wybor == "4"){
                wczytaj();
                petla=false;
                break;
        }
        else{
                string roz;
                if(wybor[0]=='"'){
                    wybor = wybor.substr(1,wybor.size()-2);
                    cout<<endl<<wybor<<endl<<endl;
                }
                int k = -1;
                for(int i=0; i<wybor.size() ;i++){
                    if(wybor[i]=='.')
                        k = i;
                }
                if(k>0){
                    roz = wybor.substr(k+1,wybor.size());
                }
                else{
                   // cout<<endl<<"Podano nieprawidlowa nazwe lub sciezke! Sprobuj ponownie."<<endl<<endl;
                }

                if(roz=="bmp" || roz== rozszerzenie || roz== rozszerzenie || roz=="BMP" || roz=="Bmp"){
                    otworz_ze_sciezki(wybor);
                    petla=false;
                }
                else{
                    wybor.clear();
                }

        }


      }
    }


void menu2()
{
    oryginal.clear();
    aktualny_obraz.clear();
    tryb = 0; //numer trybu to numer wywolujacego go przycisku
    width = 0;
    height = 0;
    przesuniecie_x=0;
    przesuniecie_y=0;
    SDL_FreeSurface(oryginal_surface);

    palette_counter = 0;
    r_kolory=0;

    delete[] dedykowana;
    delete[] kolory;
    delete[] paleta;

    sectors.clear();
    palette.clear();
    amountOfColors = 32;

    paleta = new SDL_Color[32];

    cout<<"Program do obslugi formatu ."<<rozszerzenie<<endl<<endl;
    cout<<"Wybierz jeden z ponizszych obrazow lub wczytaj swoj plik"<<endl<<endl;
    cout<<"[1] star wars "<<endl;   //obrazek1.bmp
    cout<<"[2] sonic "<<endl;       //obrazek2.bmp
    cout<<"[3] mario "<<endl;       //obrazek3.bmp
    cout<<"[4] wczytaj plik .bmp lub ."<<rozszerzenie<<endl;

    string wybor;
    bool petla = true;
    while(petla){
      cout<<endl<<"Wybrana opcja: ";
      std::getline(std::cin, wybor);

        if(wybor == "1"){
                otworz_ze_sciezki("obrazek1.bmp");
                petla=false;
                break;
        }
        else if(wybor == "2"){
                otworz_ze_sciezki("obrazek2.bmp");
                petla=false;
                break;
        }
        else if(wybor == "3"){
                otworz_ze_sciezki("obrazek3.bmp");
                petla=false;
                break;
        }
        else if(wybor == "4"){
                wczytaj();
                petla=false;
                break;
        }
        else{
                string roz;
                if(wybor[0]=='"'){
                    wybor = wybor.substr(1,wybor.size()-2);
                    cout<<endl<<wybor<<endl<<endl;
                }
                int k = -1;
                for(int i=0; i<wybor.size() ;i++){
                    if(wybor[i]=='.')
                        k = i;
                }
                if(k>0){
                    roz = wybor.substr(k+1,wybor.size());
                }
                else{
                   // cout<<endl<<"Podano nieprawidlowa nazwe lub sciezke! Sprobuj ponownie."<<endl<<endl;
                }

                if(roz=="bmp" || roz== rozszerzenie || roz=="BMP" || roz=="Bmp"){
                    otworz_ze_sciezki(wybor);
                    petla=false;
                }
                else{
                    wybor.clear();
                }

        }


      }
    }

void wczytaj()
{
    string sciezka;
    cout<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<endl;
    cout<<"Wczytaj plik .bmp lub ."<<rozszerzenie<<endl<<endl;
    cout<<"   ______________________________"<<endl;
    cout<<" / \\                             \\."<<endl;
    cout<<"|   |                            |."<<endl;
    cout<<"\\_  |                            |."<<endl;
    cout<<"    |                            |."<<endl;
    cout<<"    |       Przeciagnij plik     |."<<endl;
    cout<<"    |        i upusc tutaj       |."<<endl;
    cout<<"    |                            |."<<endl;
    cout<<"    |  lub podaj sciezke ponizej |."<<endl;
    cout<<"    |                            |."<<endl;
    cout<<"    |                            |."<<endl;
    cout<<"    |                            |."<<endl;
    cout<<"    |                            |."<<endl;
    cout<<"    |                            |."<<endl;
    cout<<"    |                            |."<<endl;
    cout<<"    |   _________________________|___"<<endl;
    cout<<"    |  /                            /."<<endl;
    cout<<"    \\_/____________________________/."<<endl;
    cout<<endl;
    cout<<"Sciezka do pliku: ";
    cin.clear();
    std::getline(std::cin, sciezka);
    otworz_ze_sciezki(sciezka);
}


void otworz_ze_sciezki(string sciezka)
{
    char const* tytul = "GKiM Projekt";
    cout<<endl<<sciezka<<endl<<endl;
    string roz;
    if(sciezka[0]=='"'){
        sciezka = sciezka.substr(1,sciezka.size()-2);
        cout<<endl<<sciezka<<endl<<endl;
    }
    int k = -1;
        for(int i=0; i<sciezka.size() ;i++){
            if(sciezka[i]=='.')
                k = i;
        }
        if(k>0){
            roz = sciezka.substr(k+1,sciezka.size());
        }
        else{
            cout<<endl<<"Podano nieprawidlowa nazwe lub sciezke! Sprobuj ponownie."<<endl<<endl;
        }
    cout<<endl<<roz<<endl;


    if(roz=="bmp" || roz=="BMP" || roz=="Bmp"){
        //wczytywanie obrazka z pliku
        const char *s = sciezka.data();
        oryginal_surface = SDL_LoadBMP(s);

        width = oryginal_surface->w;
        height = oryginal_surface->h;

        cout<<"width: "<<width<<endl;
        cout<<"height: "<<height<<endl;



            //tworzenie okna o wymiarach obrazu
            screen = SDL_SetVideoMode(width, height, 16,SDL_HWSURFACE|SDL_DOUBLEBUF);
            SDL_WM_SetCaption( tytul , NULL );
            //przesuniecie
            przesuniecie_x = 0;
            przesuniecie_y = 0;

        kolory = new tab_kolor[width*height];
        //czyszczenie poprzedniego obrazu z pamieci
        oryginal.clear();
        //przepisanie bitmapy z surface do wektora
        surface_na_wektor();
        //narysuj obrazek na ekranie
        wyswietl_oryginal();

        wyswietl_opcje_obrazu();

    }
    else if(roz == rozszerzenie){
    //wczytaj format utworzony pzez nas
        //czyszczenie poprzedniego obrazu z pamieci
        oryginal.clear();
        //odczytanie z dekompresją
        odczytaj_z_pliku(sciezka);

        cout<<"width: "<<width<<endl;
        cout<<"height: "<<height<<endl;
            //tworzenie okna o wymiarach obrazu
            screen = SDL_SetVideoMode(width, height, 16,SDL_HWSURFACE|SDL_DOUBLEBUF);
            SDL_WM_SetCaption( tytul , NULL );
            //przesuniecie
            przesuniecie_x = 0;
            przesuniecie_y = 0;

         kolory = new tab_kolor[width*height];

        wyswietl_oryginal();

        wyswietl_opcje_obrazu();

    }
    else{
        cout<<endl<<endl<<endl<<"Podano nieprawidlowa sciezke lub nieobslugiwany typ pliku"<<endl;
    }

}


void odczytaj_z_pliku(string sciezka)
{
    uint8_t wartosc;
    SDL_Color kolor;
    uint16_t szerokosc ;
    uint16_t wysokosc;
    Uint8 ilosc_kolorow ;
    SDL_Color* paleta_z_pliku;
    vector<int> obraz;
    char id[] = "  ";
    int kompresja = 0;
    uint8_t t;
    int t2; //tryb w jakim został zapisany obraz

    std::ifstream wejscie(sciezka.c_str(), ios::binary);
    wejscie.read((char*)&id, sizeof(char)*2);
    if((string)id == (string)identyfikator){
        wejscie.read((char*)&wysokosc  , sizeof(uint16_t));
        wejscie.read((char*)&szerokosc , sizeof(uint16_t));
        wejscie.read((char*)&ilosc_kolorow , sizeof(uint8_t));
        wejscie.read((char*)&kompresja, sizeof(uint8_t));
        wejscie.read((char*)&t, sizeof(uint8_t));
    t2 = (int)t;
        switch(t2){
            case 1:
            case 2:{
                    paleta_z_pliku = new SDL_Color[ilosc_kolorow];
                    //odczytywanie palety kolorów zapisanej w pliku
                    for(int i =0; i < ilosc_kolorow ; i++){
                        wejscie.read((char*)&paleta_z_pliku[i] , sizeof(SDL_Color));
                    }
                    break;
            }
            case 3:
            case 4:{
                    paleta_z_pliku = szarosci;
                    break;
            }
            case 5:
            case 6:{
                    paleta_z_pliku = standardowa;
                    break;
            }

        }

        //wypisanie podstawowych parametrow obrazu
        cout<<"PARAMETRY"<<endl;
        cout<<"identyfikator: " << identyfikator <<endl;
        cout<<"szerokosc: " << szerokosc <<endl;
        cout<<"wysokosc: " << wysokosc <<endl;
        cout<<"kolory: " << (Uint16)ilosc_kolorow <<endl;
        cout<<"tryb: " << (int)t <<endl;
        cout<<"kompresja: " << (int)kompresja <<endl;

        //odczytanie wartości koloru pikseli z obrazu
         for(int i = 0 ; i < (wysokosc * szerokosc) ; i++){
                wejscie.read((char*)&wartosc, sizeof(uint8_t));
                //cout<<(int)wartosc<<endl;
                obraz.push_back((int)wartosc);
        }

        //dekompresja
        if(kompresja == 1){
            obraz = *DecompressByteRun(obraz);
        }
        else if(kompresja == 2){
            obraz = RLE_decompress(obraz);
        }




        //zapisywanie obrazu do wektora
        int j=0;    //pomocnicza zmienna
        for(int y = 0 ; y < wysokosc ; y++){
                for(int x =0 ; x < szerokosc ; x++ ){
                        kolor = paleta_z_pliku[obraz[j]];
                        if((int)obraz[j]>32)
                            cout<<(int)obraz[j]<<endl;
                        j++;
                        oryginal.push_back(kolor);
                }
        }

        j=0;
        width = (int)szerokosc;
        height = (int)wysokosc;
        }
    else{
        cout<<"Nieobslugiwany typ pliku!"<<endl;
    }
}





vector<uint8_t> CompressWithByteRun(vector<int> source,int s_width,int s_height)
{
    vector<uint8_t> result;  //deklaracja wektora który bêdzie przechowywa³ wynik kompresji

    int i =0;
    int size = s_width * s_height; //oblicza iloœæ pikseli w obrazie

    while(i<size){

        //sprawdzenie czy natrafiono na sekwencjê conajmniej 2 powtarzaj¹cych siê po sobie kolorów
        //jeœli i=size-1 czyli zosta³ tylko jeden piksel to nie ma sensu sprawdzaæ czy jest powtarzaj¹ca siê sekwencja
        if((i<size-1)&&(source[i]==source[i+1])){
            int lenghtOfSameCombination = 0;

            //oblicza d³ugoœæ powtarzaj¹cej siê sekwencji kolorów
            while((i+lenghtOfSameCombination < size - 1)&&(lenghtOfSameCombination<127)&&
            (source[i+lenghtOfSameCombination]==source[i+lenghtOfSameCombination+1])){
                lenghtOfSameCombination++;
            }

            //Ustawiony typ to Uint8 wiêc nie obs³uguje wartoœci ujemnych, jako wartoœæ 0 przyjmujê liczbê 128, poni¿ej tego zapisujê wartoœci ujemne
            result.push_back(128-lenghtOfSameCombination-1);
            //dopisanie koloru
            result.push_back(source[i+lenghtOfSameCombination]);
            //przesuñ wskaŸnik o d³ugoœæ znalezionej sekwencji
            i = i + lenghtOfSameCombination + 1;
        }


        //gdy nie znaleziono takiej samej sekwencji
        else{
            int lenghtOfDifferentCombination = 0;

            //oblicza d³ugoœæ niepowtarzaj¹cej siê sekwencji kolorów
            while((i+lenghtOfDifferentCombination < size )&&(lenghtOfDifferentCombination<127)&&
            (source[i+lenghtOfDifferentCombination]!=source[i+lenghtOfDifferentCombination+1])){
                lenghtOfDifferentCombination++;
            }

            //Wpisz d³ugoœæ sekwencji
            result.push_back(128+lenghtOfDifferentCombination);
            //Wpisz wartoœci kolorów
            for(int j=0;j<lenghtOfDifferentCombination;j++){
                result.push_back(source[i+j]);
            }
            //przesuñ wskaŸnik o d³ugoœæ znalezionej sekwencji
            i = i + lenghtOfDifferentCombination;

        }
    }
    //zwróæ wektor z wynikiem
    return result;
}


vector<int>* DecompressByteRun(vector<int> source)
{
    int size = source.size();
    int i=0;
    vector<int>* result = new vector<int>;

    int temp;
    while(i<size){
        //pobierz symbol sterujący
        temp = (int)source[i];
        temp = temp - 128;

        //gdy napotka na ujemną wartość należy wpisać 1 sybol kilka razy (tyle ile wynosi ta wartość na minusie)
        if(temp<0){
            temp = abs(temp);
            for(int j=0;j < (int)temp;j++){
                result->push_back(source[i+1]);
        }
            i = i + 2;
        }
        //gdy napotka na dodatnią wartość należy przepisać x wartości bez zmiany
        else{
            for(int j=0;j<(int)temp;j++){
                result->push_back(source[i+1+j]);
        }
            i = i + abs((int)temp)+1;
        }
    }
    return result;
}


void wyswietl_oryginal(){
         SDL_Color kolor;
        // clear screen
        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));
        // draw bitmap
        aktualny_obraz.clear();
        tryb=0;
        int j = 0;
         for(int y = 0 ; y < height ; y++){
                for(int x =0 ; x < width ; x++ ){
                        kolor = oryginal[j];
                        setPixel(x + przesuniecie_x, y + przesuniecie_y , kolor.r , kolor.g, kolor.b);
                        j++;
                }
        }
        SDL_Flip(screen);
}


void surface_na_wektor()
{
    SDL_Color kolor;
    for(int y = 0 ; y < height ; y++){
                for(int x =0 ; x < width ; x++ ){
                        kolor = getPixelSurface(x,y,oryginal_surface);
                        oryginal.push_back(kolor);
                }
        }
}

 void zapisz_do_pliku()
{
    string sciezka;
    std::vector<uint8_t> obraz_po_kompresji;

        while(true){
        //zapytanie o nadanie wlasnej nazwy i rozszerzenia dla pliku
        cout<<endl<<endl<<"Podaj nazwe lub pelna sciezke do zapisania obrazu: ";
        string temp;
        cin.clear();

        std::getline(std::cin, temp);
        int k = -1;
        for(int i=0; i<temp.size() ;i++){
            if(temp[i]=='.')
                k = i;
        }
        if(k>0){
            if(temp.substr(k+1,temp.size())==rozszerzenie){
                sciezka = temp.substr(0,k);
                break;
            }
            else{
                cout<<"Podano nieprawidlowa nazwe lub sciezke! Sprobuj ponownie."<<endl<<endl;
            }
        }
        else{
            sciezka = temp;
            break;
        }
    }



    uint8_t wartosc = 0;
    uint8_t kompresja = 0;
    int ilosc_kolorow;
    switch(tryb){
            case 1:
            case 2:{
                    ilosc_kolorow = palette.size();
                break;
            }
            case 3:
            case 4:{
                    ilosc_kolorow = 32;
                break;
            }
            case 5:
            case 6:{
                    ilosc_kolorow = 32;
                break;
            }
            default:{
                cout<<"Blad, nie mozna zapisac obrazu w takiej postaci! Wybierz jedna z opcji modyfikacji obrazu (2-7) i spróbuj wybrac opcje zapisu ponownie"<<endl;
                return;
            }
        }

    uint16_t wys = (uint16_t)height;
    uint16_t szer = (uint16_t)width;


    //kompresja
    std::vector<uint8_t> obraz_po_kompresji_byterun;
    std::vector<uint8_t> obraz_po_kompresji_rle;

    //ByteRun
    obraz_po_kompresji_byterun = CompressWithByteRun(aktualny_obraz,width,height);

    //RLE
    obraz_po_kompresji_rle = RLE_compress(aktualny_obraz);

    //do testow zapisywanie obrazu bez kompresji
    //obraz_po_kompresji = aktualny_obraz;
    cout<<"ByteRun: "<<obraz_po_kompresji_byterun.size()<<endl;
    cout<<"RLE: "<<obraz_po_kompresji_rle.size()<<endl;

    if(obraz_po_kompresji_byterun.size() < obraz_po_kompresji_rle.size()){
        obraz_po_kompresji = obraz_po_kompresji_byterun;
        kompresja = 1;
    }
    else{
        obraz_po_kompresji = obraz_po_kompresji_rle;
        kompresja = 2;
    }
    uint8_t t = tryb;
    cout<<endl<<"tryb: "<<(int)t<<endl;

    //zapisywanie podstawowych parametrów do pliku
    ofstream wyjscie( (sciezka+"."+rozszerzenie).c_str(), ios::binary); // ustawiamy typ danych na dane binarne
    wyjscie.write((char*)&identyfikator, sizeof(char)*2);
    wyjscie.write((char*)&wys  , sizeof(uint16_t));
    wyjscie.write((char*)&szer , sizeof(uint16_t));
    wyjscie.write((char*)&ilosc_kolorow  , sizeof(uint8_t));
    wyjscie.write((char*)&kompresja, sizeof(uint8_t));
    wyjscie.write((char*)&t,sizeof(uint8_t));



    //zapisywanie palety kolorów do pliku
    switch(tryb){
            case 1:
            case 2:{
                for(int i =0; i < ilosc_kolorow ; i++){
                     wyjscie.write((char*)&palette[i] , sizeof(SDL_Color));

                }
                break;
            }
            case 3:
            case 4:{

                break;
            }
            case 5:
            case 6:{

                break;
            }
            default:{
                cout<<"Blad, nie mozna zapisac obrazu w takiej postaci! Wybierz jedna z opcji modyfikacji obrazu (2-7) i sprobuj wybrac opcje zapisu ponownie"<<endl;
                return;
            }
        }



    //zapisywanie wartości kolorów pikseli do pliku
     for(int i =0; i < obraz_po_kompresji.size() ; i++){
        wartosc = obraz_po_kompresji[i];
       // cout<<(int)wartosc<<endl;
        wyjscie.write((char*)&wartosc,sizeof(uint8_t));
    }
    wyswietl_opcje_obrazu(sciezka,true);
}

void wyswietl_opcje_obrazu()
{
    //cout<<system("cls"); // nie działa
    for(int i=0;i<50;i++)
        cout<<endl;

    cout<<endl<<"Opcje obrazu: "<<endl;
    cout<<"[1] Wyswietl oryginal "<<endl;
    cout<<"[2] Paleta dedykowana "<<endl;
    cout<<"[3] Skala szrosci "<<endl;
    cout<<"[4] Paleta standardowa "<<endl;
    cout<<"[5] Paleta dedykowana z ditheringiem "<<endl;
    cout<<"[6] Skala szrosci z digheringiem"<<endl;
    cout<<"[7] Paleta standardowa z ditheringiem"<<endl;
    cout<<endl;
    cout<<endl;
    cout<<"[8] Zapisz jako plik .bmp "<<endl;
    cout<<"[9] Zapisz jako plik ."<<rozszerzenie<<endl;
    cout<<endl;
    cout<<"[Esc] Wyjscie z programu"<<endl;
    cout<<"[Space] Wczytaj inny obraz"<<endl;
}


void wyswietl_opcje_obrazu(string sciezka,bool ismeme)
{
    //cout<<system("cls"); // nie działa
    for(int i=0;i<50;i++)
        cout<<endl;

    if(ismeme)
        cout<<endl<<"Zapisano obraz: "<<sciezka<<".meme"<<endl<<endl<<endl<<endl;
    else
        cout<<endl<<"Zapisano obraz: "<<sciezka<<endl<<endl<<endl<<endl;

   cout<<endl<<"Opcje obrazu: "<<endl;
    cout<<"[1] Wyswietl oryginal "<<endl;
    cout<<"[2] Paleta dedykowana "<<endl;
    cout<<"[3] Skala szrosci "<<endl;
    cout<<"[4] Paleta standardowa "<<endl;
    cout<<"[5] Paleta dedykowana z ditheringiem "<<endl;
    cout<<"[6] Skala szrosci z digheringiem"<<endl;
    cout<<"[7] Paleta standardowa z ditheringiem"<<endl;
    cout<<endl;
    cout<<endl;
    cout<<"[8] Zapisz jako plik .bmp "<<endl;
    cout<<"[9] Zapisz jako plik ."<<rozszerzenie<<endl;
    cout<<endl;
    cout<<"[Esc] Wyjscie z programu"<<endl;
    cout<<"[Space] Wczytaj inny obraz"<<endl;

}




void zapisz_do_bmp()
{

        string sciezka;

        while(true){
        //zapytanie o nadanie wlasnej nazwy i rozszerzenia dla pliku
        cout<<endl<<endl<<"Podaj nazwe lub pelna sciezke do zapisania obrazu: ";
        string temp;
        cin.clear();
        std::getline(std::cin, temp);
        int k = -1;
        for(int i=0; i<temp.size() ;i++){
            if(temp[i]=='.')
                k = i;
        }
        if(k>0){
            if(temp.substr(k+1,temp.size())== "bmp"){
                sciezka = temp.substr(0,k);
                break;
            }
            else{
                cout<<"Podano nieprawidlowa nazwe lub sciezke! Sprobuj ponownie."<<endl<<endl;
            }
        }
        else{
            sciezka = temp;
            break;
        }
    }
    sciezka = sciezka + ".bmp";
    const char *s = sciezka.data();
    SDL_SaveBMP(screen,s);
    wyswietl_opcje_obrazu(sciezka,false);

}


vector<uint8_t> RLE_compress(vector<int> obraz){

    vector<uint8_t> skompresowany;
    int n = obraz.size();

    for(int i=0; i < n; i++){

        int licznik = 1;

        while (i < n - 1 && obraz[i] == obraz[i + 1]) {
            licznik++;
            i++;
        }

        if(licznik==1){
            skompresowany.push_back(obraz[i]);
        }
        else{
            skompresowany.push_back(obraz[i]);
            skompresowany.push_back(255);
            skompresowany.push_back(licznik);
        }
    }
    return skompresowany;
}


vector<int> RLE_decompress(vector<int> obraz){

    vector<int> dekompresja;
    int n = obraz.size();

    for(int i=0; i < n;){

        if( i < (n-1) && obraz[i+1]==255){
            for(int j=0;j<obraz[i+2];j++){
                dekompresja.push_back(obraz[i]);
            }
            i = i + 3;
        }
        else{
            dekompresja.push_back(obraz[i]);
            i++;
        }
    }
    return dekompresja;
}

void dedykowana_start()
{
    medianCut();
    wyswietl_dedykowany();
}

pR(const SDL_Color rgb, const SDL_Color rhs) {
    return rgb.r>rhs.r;
}

pG(const SDL_Color rgb, const SDL_Color rhs) {
    return rgb.g>rhs.g;
}

pB(const SDL_Color rgb, const SDL_Color rhs) {
    return rgb.b>rhs.b;
}

Type getType(vector<SDL_Color> sector) {
    int rR, rG, rB;
    int minR=255, maxR=0, minG=255, maxG=0, minB=255, maxB=0;
    SDL_Color rgb;
    for(int i=0; i<sector.size(); i++){
        rgb=sector[i];
        if(rgb.r<minR)
            minR=rgb.r;
        if(rgb.r>maxR)
            maxR=rgb.r;
        if(rgb.g<minG)
            minG=rgb.g;
        if(rgb.g>maxG)
            maxG=rgb.g;
        if(rgb.b<minB)
            minB=rgb.b;
        if(rgb.b>maxB)
            maxB=rgb.b;
    }
    rR=maxR-minR;
    rG=maxG-minG;
    rB=maxB-minB;
    if(rR==max(rR,max(rG,rB)))
        return r;
    else if(rG==max(rR,max(rG,rB)))
        return g;
    else
        return b;
}

void sortSector(vector<SDL_Color> &s, Type t) { // dzieli sektor
    switch(t){
    case r:
        sort(s.begin(), s.end(), pR);
        break;
    case g:
        sort(s.begin(), s.end(), pG);
        break;
    case b:
        sort(s.begin(), s.end(), pB);
        break;
    }
}

void MediaCutStart(){
    SDL_Color rgb;
    Type t;
    palette.clear();
    sectors.clear();
    vector<SDL_Color> sector;
    vector<SDL_Color> sector2;
    int med;
    int j=0;
    for(int y=0; y<height; y++){
        for(int x=0; x<width; x++){
            rgb=oryginal[j];
            j++;
            sector.push_back(rgb);
        }
    }
    t=getType(sector);
    sortSector(sector, t);

    med = sector.size()/2;
    for(int i=sector.size()-1; i>=med; i--) {
        sector2.push_back(sector[i]);
        sector.pop_back();
    }
    sectors.push_back(sector);
    sectors.push_back(sector2);
}

vector<vector<SDL_Color> > divideSector(vector<SDL_Color> sector) {
    int med;
    Type t=getType(sector);
    vector<SDL_Color> sector2;
    vector<vector<SDL_Color> > temp;
    sortSector(sector, t);
    med = sector.size()/2;
    for(int i=sector.size()-1; i>=med; i--){
        sector2.push_back(sector[i]);
        sector.pop_back();
    }
    temp.push_back(sector);
    temp.push_back(sector2);
    return temp;
}

SDL_Color AverageColor(vector<SDL_Color> s){
    unsigned int r=0,g=0,b=0;
    SDL_Color color;

    for(int i=0; i<s.size(); i++){
        r+=s[i].r;
        g+=s[i].g;
        b+=s[i].b;
    }

    color.r=(double)r/s.size();
    color.g=(double)g/s.size();
    color.b=(double)b/s.size();

    return color;
}

void medianCut(){
    vector<vector<SDL_Color> > temp;
    vector<vector<SDL_Color> > holder;
    MediaCutStart();

    while(sectors.size()!=amountOfColors){

        for(int i=sectors.size()-1; i>=0; i--){
            temp=divideSector(sectors[i]);
            sectors.pop_back();
            for(int j=0; j<temp.size(); j++){
                holder.push_back(temp[j]);
            }
        }
        for(int i=0; i<holder.size(); i++){
            sectors.push_back(holder[i]);
        }
        holder.clear();
    }

    for(int i=0; i<sectors.size(); i++){
        palette.push_back(AverageColor(sectors[i]));
    }
}

void Dithering_Standardowa() //Dithering
{
    SDL_Color kolor;
    SDL_Color nowykolor;
    float **bledyR;
    float **bledyG;
    float **bledyB;
    bledyR = new float*[(width+2)];
    bledyG = new float*[(width+2)];
    bledyB = new float*[(width+2)];

    for(int i=0;i<width+2;i++){
        bledyR[i] = new float[height+1];
        bledyG[i] = new float[height+1];
        bledyB[i] = new float[height+1];
    }



    for(int x = 0 ; x < width + 2 ; x++){
            for(int y = 0 ; y < height + 1; y++){
                bledyR[x][y]=0;
                bledyG[x][y]=0;
                bledyB[x][y]=0;
            }
        }
    int j = 0;
    aktualny_obraz.clear();
    tryb = 6;
    int bladR, bladG, bladB, R, G, B;
    int przesuniecie=1;
    int p;
    for(int y=0; y<height; y++)
    {
        for(int x=0; x<width; x++)
        {
            kolor = oryginal[j];
            kolor.r= (uint8_t) min(max(kolor.r + bledyR[x + przesuniecie][y] , (float) 0) , (float)255);
            kolor.g= (uint8_t) min(max(kolor.g + bledyG[x + przesuniecie][y] , (float) 0) , (float)255);
            kolor.b= (uint8_t) min(max(kolor.b + bledyB[x + przesuniecie][y] , (float) 0) , (float)255);

            p = najblizszy_kolor_standardowa(kolor);
            aktualny_obraz.push_back(p);
            nowykolor = standardowa[p];

            bladR = (double)kolor.r - (double)nowykolor.r;
            bladG = (double)kolor.g - (double)nowykolor.g;
            bladB = (double)kolor.b - (double)nowykolor.b;

            bledyR[x+przesuniecie+1][y  ]+=(bladR*7.0/16.0);
            bledyR[x+przesuniecie+1][y+1]+=(bladR*1.0/16.0);
            bledyR[x+przesuniecie  ][y+1]+=(bladR*5.0/16.0);
            bledyR[x+przesuniecie-1][y+1]+=(bladR*3.0/16.0);

            bledyG[x+przesuniecie+1][y  ]+=(bladG*7.0/16.0);
            bledyG[x+przesuniecie+1][y+1]+=(bladG*1.0/16.0);
            bledyG[x+przesuniecie  ][y+1]+=(bladG*5.0/16.0);
            bledyG[x+przesuniecie-1][y+1]+=(bladG*3.0/16.0);

            bledyB[x+przesuniecie+1][y  ]+=(bladB*7.0/16.0);
            bledyB[x+przesuniecie+1][y+1]+=(bladB*1.0/16.0);
            bledyB[x+przesuniecie  ][y+1]+=(bladB*5.0/16.0);
            bledyB[x+przesuniecie-1][y+1]+=(bladB*3.0/16.0);

            setPixel( x  , y  , nowykolor.r , nowykolor.g , nowykolor.b);
            j++;
        }

    }
        j=0;
        for(int i=0;i<width+2;i++){
            for(int j=0;j<height + 1;j++){
                bledyR[i][j] = 0;
                bledyG[i][j] = 0;
                bledyB[i][j] = 0;

            }
        }
        bladR = 0;
        bladG = 0;
        bladB = 0;
        SDL_Flip(screen);

        for(int i=0;i<width+2;i++){
                delete[] bledyR[i];
                delete[] bledyG[i];
                delete[] bledyB[i];
        }
        delete[] bledyR;
        delete[] bledyG;
        delete[] bledyB;
}

void Dithering_Dedykowana() //Dithering
{
    SDL_Color kolor;
    SDL_Color nowykolor;
    float **bledyR;
    float **bledyG;
    float **bledyB;
    bledyR = new float*[(width+2)];
    bledyG = new float*[(width+2)];
    bledyB = new float*[(width+2)];

    for(int i=0;i<width+2;i++){
        bledyR[i] = new float[height+1];
        bledyG[i] = new float[height+1];
        bledyB[i] = new float[height+1];
    }
        for(int x = 0 ; x < width + 2 ; x++){
            for(int y = 0 ; y < height + 1 ; y++){
                bledyR[x][y]=0;
                bledyG[x][y]=0;
                bledyB[x][y]=0;
            }
        }

    int j = 0;
    //pobierz_palete_dedykowana();
    j=0;
    aktualny_obraz.clear();
    tryb = 2;
    int bladR, bladG, bladB, R, G, B;
    int przesuniecie=1;
    int p;
    //setPictureTab();
   // mediancut(0,width*height, 0);
    medianCut();
    for(int y=0; y<height; y++)
    {
        for(int x=0; x<width; x++)
        {
            kolor = oryginal[j];
            kolor.r = (uint8_t) min(max(kolor.r + bledyR[x + przesuniecie][y] , (float) 0) , (float)255);
            kolor.g = (uint8_t) min(max(kolor.g + bledyG[x + przesuniecie][y] , (float) 0) , (float)255);
            kolor.b = (uint8_t) min(max(kolor.b + bledyB[x + przesuniecie][y] , (float) 0) , (float)255);



            p = najblizszy_kolor(kolor);
            aktualny_obraz.push_back(p);
            nowykolor = palette[p];

            bladR = (double)kolor.r - (double)nowykolor.r;
            bladG = (double)kolor.g - (double)nowykolor.g;
            bladB = (double)kolor.b - (double)nowykolor.b;

            bledyR[x+przesuniecie+1][y  ]+=(bladR*7.0/16.0);
            bledyR[x+przesuniecie+1][y+1]+=(bladR*1.0/16.0);
            bledyR[x+przesuniecie  ][y+1]+=(bladR*5.0/16.0);
            bledyR[x+przesuniecie-1][y+1]+=(bladR*3.0/16.0);

            bledyG[x+przesuniecie+1][y  ]+=(bladG*7.0/16.0);
            bledyG[x+przesuniecie+1][y+1]+=(bladG*1.0/16.0);
            bledyG[x+przesuniecie  ][y+1]+=(bladG*5.0/16.0);
            bledyG[x+przesuniecie-1][y+1]+=(bladG*3.0/16.0);

            bledyB[x+przesuniecie+1][y  ]+=(bladB*7.0/16.0);
            bledyB[x+przesuniecie+1][y+1]+=(bladB*1.0/16.0);
            bledyB[x+przesuniecie  ][y+1]+=(bladB*5.0/16.0);
            bledyB[x+przesuniecie-1][y+1]+=(bladB*3.0/16.0);

            setPixel( x  , y  , nowykolor.r , nowykolor.g , nowykolor.b);
            j++;
        }

    }
        j=0;
        bladR = 0;
        bladG = 0;
        bladB = 0;
         for(int i=0;i<width+2;i++){
                delete[] bledyR[i];
                delete[] bledyG[i];
                delete[] bledyB[i];
        }
        delete[] bledyR;
        delete[] bledyG;
        delete[] bledyB;
        SDL_Flip(screen);
}

void Dithering_Szary() //Dithering
{
    SDL_Color kolor;
    SDL_Color nowykolor;
    float** bledy;
    bledy = new float*[width+2];
    for(int i=0;i<width+2;i++){
        bledy[i] = new float[height+1];
    }

        for(int x = 0 ; x < width +2 ; x++){
            for(int y = 0 ; y < height +1 ; y++){
                bledy[x][y]=0;
            }
        }

    int j = 0;
    aktualny_obraz.clear();
    tryb = 4;
    int przesuniecie=1;
    int BW , BWNEW;
    SDL_Color BWC;
    int blad;
    for(int y=0; y<height; y++)
    {
        for(int x=0; x<width; x++)
        {
            kolor = oryginal[j];
            BW=0.299*kolor.r+0.587*kolor.g+0.114*kolor.b;
            BW+=bledy[x+przesuniecie][y];

            if(BW>255)
                BW=255;
            if(BW<0)
                BW=0;

            szarosc(BW , x  ,y );
            BWC = szarosci[aktualny_obraz[j]];
            BWNEW = 0.299*BWC.r+0.587*BWC.g+0.114*BWC.b;
            blad = BW - BWNEW;

            if(blad>255){
                blad = 255;
            }

            bledy[x+przesuniecie+1][y  ]+=(blad*7.0/16.0);
            bledy[x+przesuniecie+1][y+1]+=(blad*1.0/16.0);
            bledy[x+przesuniecie  ][y+1]+=(blad*5.0/16.0);
            bledy[x+przesuniecie-1][y+1]+=(blad*3.0/16.0);
            setPixel(x,y,BWC.r,BWC.g,BWC.b);
            j++;
        }
    }
        for(int x = 0 ; x < width/2+2 ; x++){
            for(int y = 0 ; y < height/2+1 ; y++){
                bledy[x][y]=0;
            }
        }
        j=0;
        SDL_Flip(screen);
}


bool porownajKolory_dedykowana(SDL_Color k1){ // true jeżeli koloru nie ma w tablicy
    for(int i = 0; i < r_kolory ; i++){
        if(k1.r == kolory[i].kolor.r && k1.g == kolory[i].kolor.g && k1.b == kolory[i].kolor.b){
            return true;
        }
    }
    return false;
}

void sprawdzKolor_dedykowane(){ // sprawdza jakie kolory ile razy wystapily i zapisuje je do tablicy kolorow
    SDL_Color kolor;
    int j=0;
    for(int y = 0 ; y< height ; y++){
        for(int x =0 ; x < width; x++ ){
            kolor = oryginal[j];
            j++;
            if(porownajKolory_dedykowana(kolor) == false){
                kolory[r_kolory].kolor = kolor;
                kolory[r_kolory].licznik++;
                r_kolory++;
            }
            else{
                kolory[r_kolory].licznik++;
            }
        }
    }
}

void sortowanie_babelkowe(tab_kolor tab[],int n)
{
    for(int i=0;i<n;i++)
        for(int j=1;j<n-i;j++) //pętla wewnętrzna
        if(tab[j-1].licznik<tab[j].licznik)
            swap(tab[j-1], tab[j]);
}

void wybierz(){
    dedykowana = new SDL_Color [32];
    sortowanie_babelkowe(kolory , r_kolory);
    for(int i = 0 ; i < 32  ; i++){
        dedykowana[i] = kolory[i].kolor;
    }
}

void pobierz_palete_dedykowana() {
    kolory = new tab_kolor[width*height];
    r_kolory=0;
    sprawdzKolor_dedykowane();
    wybierz();
}

int najblizszy_kolor(SDL_Color k){
    int nr=0;
    int najmniejszy = 10000;
    int temp;
    for(int i = 0 ; i < 32 ; i++){
        temp=abs(k.r - palette[i].r) + abs(k.g - palette[i].g) + abs(k.b - palette[i].b);
        if(temp < najmniejszy ){
         najmniejszy = temp;
         nr = i;
        }
    }
    return nr;
}

void wyswietl_dedykowany() {
    SDL_Color kolor;
    int l;
    int j=0;
    aktualny_obraz.clear();
    tryb=1;
    for(int y = 0 ; y < height ; y++){
        for(int x = 0 ; x < width ; x++){
            kolor = oryginal[j];
            l = najblizszy_kolor(kolor);
            aktualny_obraz.push_back(l);
            setPixel( x + przesuniecie_x , y + przesuniecie_y , palette[l].r , palette[l].g , palette[l].b);
            j++;
        }
    }
    SDL_Flip(screen);
}
void szarosc(int BW , int x, int y){
    SDL_Color temp;
    if(BW < 8){
        setPixel(x , y , 0 , 0 , 0);
        aktualny_obraz.push_back(0);
        temp  = {0,0,0};
    }
    else if(BW >= 8 && BW < 16){
        setPixel(x , y , 8 , 8 , 8);
        aktualny_obraz.push_back(1);
         temp  = {8,8,8};
    }
    else if(BW >= 16 && BW < 24){
        setPixel(x , y , 16 , 16 , 16);
        aktualny_obraz.push_back(2);
        temp  = {16,16,16};
    }
    else if(BW >= 24 && BW < 32){
        setPixel(x , y , 24 , 24 , 24);
        aktualny_obraz.push_back(3);
        temp  = {24,24,24};
    }
    else if(BW >= 32 && BW < 40){
        setPixel(x , y , 32 , 32 , 32);
        aktualny_obraz.push_back(4);
        temp  = {24,24,24};
    }
    else if(BW >= 40 && BW < 48){
        setPixel(x , y , 40 , 40 , 40);
        aktualny_obraz.push_back(5);
    }
    else if(BW >= 48 && BW < 56){
        setPixel(x , y , 48 , 48 , 48);
        aktualny_obraz.push_back(6);
    }
    else if(BW >= 56 && BW < 64){
        setPixel(x , y , 56 , 56 , 56);
        aktualny_obraz.push_back(7);
    }
    else if(BW >= 64 && BW < 72){
        setPixel(x , y , 64 , 64 , 64);
        aktualny_obraz.push_back(8);
    }
    else if(BW >= 72 && BW < 80){
        setPixel(x , y , 72 , 72 , 72);
        aktualny_obraz.push_back(9);
    }
    else if(BW >= 80 && BW < 88){
        setPixel(x , y , 80 , 80 , 80);
        aktualny_obraz.push_back(10);
    }
    else if(BW >= 88 && BW < 96){
        setPixel(x , y , 88 , 88 , 88);
        aktualny_obraz.push_back(11);
    }
    else if(BW >= 96 && BW < 104){
        setPixel(x , y , 96 , 96 , 96);
        aktualny_obraz.push_back(12);
    }
    else if(BW >= 104 && BW < 112){
        setPixel(x , y , 104 , 104 , 104);
        aktualny_obraz.push_back(13);
    }
    else if(BW >= 112 && BW < 120){
        setPixel(x , y , 112 , 112 , 112);
        aktualny_obraz.push_back(14);
    }
    else if(BW >= 120 && BW < 128){
        setPixel(x , y , 120 , 120 , 120);
        aktualny_obraz.push_back(15);
    }
    else if(BW >= 128 && BW < 136){
        setPixel(x , y , 128 , 128 , 128);
        aktualny_obraz.push_back(16);
    }
    else if(BW >= 136 && BW < 144){
        setPixel(x , y , 136 , 136 , 136);
        aktualny_obraz.push_back(17);
    }
    else if(BW >= 144 && BW < 152){
        setPixel(x , y , 144 , 144 , 144);
        aktualny_obraz.push_back(18);
    }
    else if(BW >= 152 && BW < 160){
        setPixel(x , y , 152 , 152 , 152);
        aktualny_obraz.push_back(19);
    }
    else if(BW >= 160 && BW < 168){
        setPixel(x , y , 160 , 160 , 160);
        aktualny_obraz.push_back(20);
    }
    else if(BW >= 168 && BW < 176){
        setPixel(x , y , 168 , 168 , 168);
        aktualny_obraz.push_back(21);
    }
    else if(BW >= 176 && BW < 184){
        setPixel(x , y , 176 , 176 , 176);
        aktualny_obraz.push_back(22);
    }
    else if(BW >= 184 && BW < 192){
        setPixel(x , y , 184 , 184 , 184);
        aktualny_obraz.push_back(23);
    }
    else if(BW >= 192 && BW < 200){
        setPixel(x , y , 192 , 192 , 192);
        aktualny_obraz.push_back(24);
    }
    else if(BW >= 200 && BW < 208){
        setPixel(x , y , 200 , 200 , 200);
        aktualny_obraz.push_back(25);
    }
    else if(BW >= 208 && BW < 216){
        setPixel(x , y , 208 , 208 , 208);
        aktualny_obraz.push_back(26);
    }
    else if(BW >= 216 && BW < 224){
        setPixel(x , y , 216 , 216 , 216);
        aktualny_obraz.push_back(27);
    }
    else if(BW >= 224 && BW < 232){
        setPixel(x , y , 224 , 224 , 224);
        aktualny_obraz.push_back(28);
    }
    else if(BW >= 232 && BW < 240){
        setPixel(x , y , 232 , 232 , 232);
        aktualny_obraz.push_back(29);
    }
    else if(BW >= 240 && BW < 250){
        setPixel(x , y , 245 , 245 , 245);
        aktualny_obraz.push_back(30);
    }
    else if(BW >= 250 && BW < 256){
        setPixel(x , y , 255 , 255 , 255);
        aktualny_obraz.push_back(31);
    }
}
void wyswietl_szary() {
    SDL_Color kolor;
    int BW;
    aktualny_obraz.clear();
    tryb=3;
    int j=0;
    for(int y = 0 ; y < height ; y++){
        for(int x = 0 ; x < width ; x++){
            kolor = oryginal[j];
             BW=0.299*kolor.r+0.587*kolor.g+0.114*kolor.b;
             szarosc(BW , x + przesuniecie_x ,y + przesuniecie_y);
             j++;
        }
    }
SDL_Flip(screen);
}
int najblizszy_kolor_standardowa(SDL_Color k){
    int nr=0;
    int najmniejszy = 10000;
    int temp;
    for(int i = 0 ; i < 32 ; i++){
        temp=abs(k.r - standardowa[i].r) + abs(k.g - standardowa[i].g) + abs(k.b - standardowa[i].b);

        if(temp < najmniejszy ){
         najmniejszy = temp;
         nr = i;
        }
    }
    return nr;
}
void wyswietl_standard() { // tablica standardowych kolorów
        SDL_Color kolor;
    int l ;
    aktualny_obraz.clear();
    tryb = 5;
    int j=0;
    for(int y = 0 ; y < height ; y++){
        for(int x = 0 ; x < width ; x++){
            kolor = oryginal[j];
            l = najblizszy_kolor_standardowa(kolor);
            aktualny_obraz.push_back(l);
            setPixel( x + przesuniecie_x , y + przesuniecie_y , standardowa[l].r , standardowa[l].g , standardowa[l].b);
            j++;
        }
    }
    SDL_Flip(screen);
}





   int main ( int argc, char** argv )
{

    // console output
    freopen( "CON", "wt", stdout );
    freopen( "CON", "wt", stderr );

    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    bool bez_obrazu = true;
    while(bez_obrazu){
    menu();
    if(width > 0 && height > 0)
        bez_obrazu = false;
    }


    //SDL_WM_SetCaption( tytul , NULL );
    // program main loop
    bool done = false;
    while (!done)
    {
        // message processing loop
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // check for messages
            switch (event.type)
            {
                // exit if the window is closed
            case SDL_QUIT:
                done = true;
                break;

                // check for keypresses
            case SDL_KEYDOWN:
                {
                    // exit if ESCAPE is pressed
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                    if (event.key.keysym.sym == SDLK_1)
                        wyswietl_oryginal();
                    if (event.key.keysym.sym == SDLK_2)
                        dedykowana_start();
                    if (event.key.keysym.sym == SDLK_3)
                        wyswietl_szary();
                    if (event.key.keysym.sym == SDLK_4)
                        wyswietl_standard();
                    if (event.key.keysym.sym == SDLK_5)
                        Dithering_Dedykowana();
                    if (event.key.keysym.sym == SDLK_6)
                        Dithering_Szary();
                    if (event.key.keysym.sym == SDLK_7)
                        Dithering_Standardowa();
                    if (event.key.keysym.sym == SDLK_8)
                        zapisz_do_bmp();
                    if (event.key.keysym.sym == SDLK_9)
                        zapisz_do_pliku();
                  /*  if (event.key.keysym.sym == SDLK_7)
                        Funkcja7();
                    */

                    if (event.key.keysym.sym == SDLK_SPACE){
                        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));
                        SDL_Flip(screen);

                        for(int i=0;i<50;i++)
                            cout<<endl;

                        menu2();
                    }
                    break;

                    if (event.key.keysym.sym == SDLK_b)
                        czyscEkran(0, 0, 0);          break;
                     }
            } // end switch
        } // end of message processing

    } // end main loop


    return 0;
}


void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
  if ((x>=0) && (x<screen->w) && (y>=0) && (y<screen->h))
  {
    /* Zamieniamy poszczególne składowe koloru na format koloru piksela */
    Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);

    /* Pobieramy informację ile bajtów zajmuje jeden piksel */
    int bpp = screen->format->BytesPerPixel;

    /* Obliczamy adres piksela */
    Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

    /* Ustawiamy wartość piksela, w zależności od formatu powierzchni*/
    switch(bpp)
    {
        case 1: //8-bit
            *p = pixel;
            break;

        case 2: //16-bit
            *(Uint16 *)p = pixel;
            break;

        case 3: //24-bit
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4: //32-bit
            *(Uint32 *)p = pixel;
            break;

    }
         /* ewentualna aktualizacja obrazu (aka double buffering) */
  }
}


void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B,SDL_Surface* screen)
{
  if ((x>=0) && (y>=0))
  {
    /* Zamieniamy poszczególne składowe koloru na format koloru piksela */
    Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);

    /* Pobieramy informację ile bajtów zajmuje jeden piksel */
    int bpp = screen->format->BytesPerPixel;

    /* Obliczamy adres piksela */
    Uint8 *p1 = (Uint8 *)screen->pixels + (y*2) * screen->pitch + (x*2) * bpp;
    Uint8 *p2 = (Uint8 *)screen->pixels + (y*2+1) * screen->pitch + (x*2) * bpp;
    Uint8 *p3 = (Uint8 *)screen->pixels + (y*2) * screen->pitch + (x*2+1) * bpp;
    Uint8 *p4 = (Uint8 *)screen->pixels + (y*2+1) * screen->pitch + (x*2+1) * bpp;

    /* Ustawiamy wartość piksela, w zależności od formatu powierzchni*/
    switch(bpp)
    {
        case 1: //8-bit
            *p1 = pixel;
            *p2 = pixel;
            *p3 = pixel;
            *p4 = pixel;
            break;

        case 2: //16-bit
            *(Uint16 *)p1 = pixel;
            *(Uint16 *)p2 = pixel;
            *(Uint16 *)p3 = pixel;
            *(Uint16 *)p4 = pixel;
            break;

        case 3: //24-bit
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p1[0] = (pixel >> 16) & 0xff;
                p1[1] = (pixel >> 8) & 0xff;
                p1[2] = pixel & 0xff;
                p2[0] = (pixel >> 16) & 0xff;
                p2[1] = (pixel >> 8) & 0xff;
                p2[2] = pixel & 0xff;
                p3[0] = (pixel >> 16) & 0xff;
                p3[1] = (pixel >> 8) & 0xff;
                p3[2] = pixel & 0xff;
                p4[0] = (pixel >> 16) & 0xff;
                p4[1] = (pixel >> 8) & 0xff;
                p4[2] = pixel & 0xff;
            } else {
                p1[0] = pixel & 0xff;
                p1[1] = (pixel >> 8) & 0xff;
                p1[2] = (pixel >> 16) & 0xff;
                p2[0] = pixel & 0xff;
                p2[1] = (pixel >> 8) & 0xff;
                p2[2] = (pixel >> 16) & 0xff;
                p3[0] = pixel & 0xff;
                p3[1] = (pixel >> 8) & 0xff;
                p3[2] = (pixel >> 16) & 0xff;
                p4[0] = pixel & 0xff;
                p4[1] = (pixel >> 8) & 0xff;
                p4[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4: //32-bit
            *(Uint32 *)p1 = pixel;
            *(Uint32 *)p2 = pixel;
            *(Uint32 *)p3 = pixel;
            *(Uint32 *)p4 = pixel;
            break;

    }
         /* ewentualna aktualizacja obrazu (aka double buffering) */
  }
}




SDL_Color getPixel(int x, int y) {
    SDL_Color color ;
    Uint32 col = 0 ;
    if ((x>=0) && (y>=0)) {
        //określamy pozycję
        char* pPosition=(char*)screen->pixels ;
        //przesunięcie względem y
        pPosition+=(screen->pitch*y*2) ;
        //przesunięcie względem x
        pPosition+=(screen->format->BytesPerPixel*x*2);
        //kopiujemy dane piksela
        memcpy(&col, pPosition, screen->format->BytesPerPixel);
        //konwertujemy kolor
        SDL_GetRGB(col, screen->format, &color.r, &color.g, &color.b);
    }
    return ( color ) ;
}

SDL_Color getPixelSurface(int x, int y, SDL_Surface *surface) {
    SDL_Color color ;
    Uint32 col = 0 ;
    //określamy pozycję
    char* pPosition=(char*)surface->pixels ;
    //przesunięcie względem y
    pPosition+=(surface->pitch*y) ;
    //przesunięcie względem x
    pPosition+=(surface->format->BytesPerPixel*x);
    //kopiujemy dane piksela
    memcpy(&col, pPosition, surface->format->BytesPerPixel);
    //konwertujemy kolor
    SDL_GetRGB(col, surface->format, &color.r, &color.g, &color.b);
    return ( color ) ;
}

void czyscEkran(Uint8 R, Uint8 G, Uint8 B)
{
    SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, R, G, B));
    SDL_Flip(screen);

}
