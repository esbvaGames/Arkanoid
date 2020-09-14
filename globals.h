
#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <map>


//. Para usar la libreria Standard
using namespace std;
//. Para usar la librería SFML Espacio de Nombres.-
using namespace sf;

enum CForce { forceLeft=1, forceRight, forceTop, forceDown };

#define fillZero(n)   setfill('0') << setw(n)

#ifdef _MAIN_
   Color colorBorde = Color( 74, 134, 232);
   Color colorBlock = Color(255,  90,  90);
   Color colorYellow = Color(255, 255, 0);
   Color colorPlayer = Color(90, 255, 90);
   Color colorDoors  = Color( 0, 255, 255);

   Color colorRojo  = Color(255, 0, 0);
   Color colorRojo_alpha = Color(255,0,0, 40);
   Color colorVerde = Color(0, 255, 0);

   Color colorBorde_alpha = Color(74, 134, 232, 40);
   Color colorSelect = Color(0,255,0);
   Color colorSelect_alpha = Color(0,255,0, 40);
   Color colorPressed = Color(0,255,255);
   Color colorPressed_alpha = Color(0,255,255,40);

   int   Puntaje   = 0;
   bool  StartGame = false;
   bool  ModoEditar = false;
   bool  Mouse_in_block = false;


#else
   extern Color colorBorde;
   extern Color colorBlock;
   extern Color colorYellow;
   extern Color colorPlayer;
   extern Color colorDoors;
   extern Color colorRojo;
   extern Color colorRojo_alpha;
   extern Color colorVerde;
   extern Color colorBorde_alpha;
   extern Color colorSelect;
   extern Color colorSelect_alpha;
   extern Color colorPressed;
   extern Color colorPressed_alpha;

   extern int Puntaje;
   extern bool StartGame;
   extern bool ModoEditar;
   extern bool Mouse_in_block;


#endif // _MAIN_


#endif // _GLOBALS_H_

float frand();
Text Create_Label(float px, float py, string texto, Font *font, float Scale);
RectangleShape Create_Rectangle(float px, float py, float width, float height, \
                                Color myColor, Color myRelleno = (Color)0);

void Command_Canons(void *data);
void Command_Puertas(void *data);
void Command_Bolas_x3(void *data);
void Command_Vida_Extra(void *data);
void Command_Atrapar_Bola(void *data);
void Command_Bola_Lenta(void *data);
void Command_Encoger_Tambor(void *data);
void Command_Generar_Tornado(void *data);
void Create_Canons(Image *imagen, float px, float py);

//. Funcion para cambiar el fondo desde el Editor
void CambiarFondo(RectangleShape *myShape, int index, int patron = 64);






