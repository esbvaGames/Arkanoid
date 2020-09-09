#ifndef LEVELS_H_
#define LEVELS_H_

#define MAX_LEVEL 50
#define MAX_FILAS 12    //. Niveles mas altos tienen mas filas
#define MAX_COLMS 13
#define NUM_COLORS 10

struct DATA {
   int   indice;
   Color colors;
   int   Golpes;
   DATA() {};
   DATA(int indice, Color colors, int Golpes){
      this->indice = indice;
      this->colors = colors;
      this->Golpes = Golpes;
   }
};


#ifdef _MAIN_


int ARRAY_LEVEL[MAX_LEVEL][MAX_FILAS][MAX_COLMS] = {};



DATA data[NUM_COLORS] = {
   { DATA(0, (Color)NULL,        0) },  //. 0 Vacio
   { DATA(1, Color(  1,155,  0), 0) },  //. 1 Verde
   { DATA(2, Color(  0,255,  0), 1) },  //. 2 Verde LT 1 Golpe
   { DATA(3, Color(255,255,  0), 2) },  //. 3 Amarillo 2 Golpes
   { DATA(4, Color(255,111,  0), 3) },  //. 4 Naranja  3 Golpes
   { DATA(5, Color(255,  0,  0), 4) },  //. 5 Rojo     4 Golpes
   { DATA(6, Color(255,  0,255), 5) },  //. 6 Rosa     5 Golpes
   { DATA(7, Color(154,  0,255), 6) },  //. 7 fuccia   6 Golpes
   { DATA(8, Color(  3,145,130), 7) },  //. 8 purple   7 Golpes
   { DATA(9, Color( 74,134,232), 8) },  //. 9 cyanDK   8 Golpes
   // ... Definir mas Colores aca ...
};

int index = -1;
int INDEX = 0;
int Wheel = 0;
int SELECTO = 0;
int Selecto = 0;

string fileSave = "./GameLevel.txt";

#else
   extern int ARRAY_LEVEL[MAX_LEVEL][MAX_FILAS][MAX_COLMS];
   extern DATA data[NUM_COLORS];
   extern int index;
   extern int INDEX;
   extern int Wheel;
   extern int SELECTO;
   extern int Selecto;
   extern string fileSave;

#endif // _MAIN_


//. Funciones del Modulo

Color data_getColors(int idColor);
int data_getGolpes(int idColor);
void copyToArray(int level, BLOCK **block, int TOTAL);
bool SaveToTexto(int FILAS=15);
bool loadFromFile();
void CopyFromArray(int level, BLOCK **block, int TOTAL);


#endif // LEVELS_H_INCLUDED
