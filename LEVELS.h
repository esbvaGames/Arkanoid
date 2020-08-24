#ifndef LEVELS_H_INCLUDED
#define LEVELS_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#define MAX_LEVEL 50
#define MAX_FILAS 12
#define MAX_COLMS 13


string ARRAY_LEVEL[MAX_LEVEL][MAX_FILAS] = {
   {
      "0,0,0,0,0,1,1,1,1,0,0,0,1",
      "0,1,1,1,1,0,0,0,0,1,1,1,1",
      "1,1,1,1,1,0,0,0,0,0,1,1,1",
      "1,1,1,1,1,0,0,0,0,1,1,1,1",
      "1,1,1,1,1,1,0,0,0,1,1,1,1",
      "2,2,2,2,2,1,1,1,1,2,2,2,2",
      "1,1,1,1,1,2,2,2,2,1,1,1,1",
      "0,0,1,1,0,0,1,1,0,0,1,1,0",
      ".",
   },
   {
      "0,0,0,0,1,1,1,1,0,0,0,0,0",
      "1,1,1,1,0,0,0,0,1,1,1,1,1",
      "1,1,1,1,0,0,0,0,1,1,1,1,1",
      "0,0,0,0,1,1,1,1,0,0,0,0,0",
      "0,0,0,0,1,1,1,1,0,0,0,0,0",
      ".",
   }
};

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

#define NUM_COLORS 9

DATA data[NUM_COLORS] = {
   { DATA(0, Color(  0,  0,  0), 0) },  //. 0 Vacio
   { DATA(1, Color(  1,255,  0), 0) },  //. 1 Verde
   { DATA(2, Color( 20, 98, 56), 1) },  //. 2 Naranja  1 Golpe
   { DATA(3, Color(  0, 62, 44), 2) },  //. 3 Marron   2 Golpes
   { DATA(4, Color(240, 56, 40), 3) },  //. 3 Azul     3 Golpes
   { DATA(5, Color(255, 61, 77), 3) },  //. 3 Rosa     4 Golpes
   // ... Definir mas Colores aca ...
};



#endif // LEVELS_H_INCLUDED
