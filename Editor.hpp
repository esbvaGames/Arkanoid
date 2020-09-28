#ifndef EDITOR_H
#define EDITOR_H


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
