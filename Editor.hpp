#ifndef EDITOR_H
#define EDITOR_H

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
   { DATA(0, (Color)0,        0) },     //. 0 Vacio
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
#else
   extern int ARRAY_LEVEL[MAX_LEVEL][MAX_FILAS][MAX_COLMS];
   extern DATA data[NUM_COLORS];

#endif // _MAIN_


class Editor
{
   public:
      Editor();
      Editor(Font *font);
      virtual ~Editor();

   void SetBlock(BLOCK **block, int TOTAL);
   void SetFonts(Font *font);

   void SetLevel(CIRCLE **level, int LEVELS);

   //. Para recibir el rcGame
   void Set_rcGame(RectangleShape *rcGame);


   bool isMouseInLevel(RenderWindow *win, CIRCLE **level, int TOTAL, int *indice);
   bool isMouseInBlock(RenderWindow *win, BLOCK **block, int TOTAL, int *index);
   void Process(RenderWindow *win, Event evn);
   void Display(RenderWindow *win);

   Color data_getColors(int idColor);
   int data_getGolpes(int idColor);
   void copyToArray(int level, BLOCK **block, int TOTAL);
   void SaveToTexto(int FILAS=15);
   void loadFromFile();
   void CopyFromArray(int level, BLOCK **block, int TOTAL);

protected:
      int iFondo = 0;
      int LEVELS = 0;
      int TOTAL = 0;
      int index = -1;
      int INDEX = 0;
      int Wheel = 0;
      int SELECTO = 0;
      int Selecto = 0;
      string fileSave = "./GameLevel.txt";

private:
   RectangleShape *rcGame;

   RectangleShape rcEdit;
   RectangleShape rcCube;

   Text idBlock;
   Text label_1;
   Text label_2;
   Text label_3;
   Text label_4;

   BUTTON *Guardar = NULL;
   BUTTON *Cargar  = NULL;
   BUTTON *Salir   = NULL;

   Font   *font;
   BLOCK  **block;
   CIRCLE **level;

};

#endif // EDITOR_H
