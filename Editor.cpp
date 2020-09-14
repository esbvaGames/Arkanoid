
#include "Globals.h"
#include "Blocks.h"
#include "Circle.h"
#include "Button.h"
#include "Editor.hpp"

Editor::Editor(){}


Editor::Editor(Font *font)
{
    this->font = font;

    rcEdit = Create_Rectangle(46, 310, 466, 158, colorBorde, Color(34,38,38));
    rcCube = Create_Rectangle(100, 342, 32,32, colorBorde);

    idBlock = Create_Label(106, 320, "00", font, 15);
    label_1 = Create_Label(220, 310, "Modo Edicion", font, 20);
    label_2 = Create_Label(150, 340, "Rueda para cambiar Bloque", font, 15);
    label_3 = Create_Label(150, 360, "Click en Numeros para Cambiar-Nivel", font, 15);
    label_4 = Create_Label(150, 390, "Editando Nivel: 00", font, 15);

    Guardar = new BUTTON(310, 386, 70, 25, "Guardar", font, 14);
    Cargar  = new BUTTON(400, 386, 70, 25, "Cargar", font, 14);
    Salir   = new BUTTON(230, 425, 70, 25, "Salir", font, 14);
}

Editor::~Editor()
{
   //dtor
}

void Editor::SetBlock(BLOCK **block, int TOTAL){
   this->block = block;
   this->TOTAL = TOTAL;
}

void Editor::SetLevel(CIRCLE **level, int LEVELS){
   this->level = level;
   this->LEVELS = LEVELS;
}

void Editor::Set_rcGame(RectangleShape *rcGame){
   this->rcGame = rcGame;
}


Color Editor::data_getColors(int idColor){
   return data[idColor].colors;
}

int Editor::data_getGolpes(int idColor){
   return data[idColor].Golpes;
}


void Editor::copyToArray(int level, BLOCK **block, int TOTAL){
  int filas = 0;
  int colms = 0;

  for(int index=0; index < TOTAL; index++){
     colms = index % MAX_COLMS;
     filas = index / MAX_COLMS;
     //cout << "filas(" << filas << " colms(" << colms << ") " << block[index]->get_idColor() << endl;
     ARRAY_LEVEL[level][filas][colms] = block[(TOTAL -1) -index]->get_idColor();
  }
}


void Editor::SaveToTexto(int FILAS){
   ofstream mySave;

   mySave.open(fileSave);

   if(mySave.is_open()){
      mySave << "GameLevel {" << endl;
      for(int level=0; level < MAX_LEVEL; level++){
         mySave << "  Level: " << fillZero(2) << level << " filas: " << fillZero(2) << FILAS << endl;
         for(int filas=0; filas < MAX_FILAS; filas++){
            mySave << "    " << fillZero(2) << filas << " { ";
            for(int colms=0; colms < MAX_COLMS; colms++){
               mySave << ARRAY_LEVEL[level][filas][colms] << " ";
               cout << "level(" << level << ") filas(" << filas << ") colms(" << colms << ")" << endl;
            }
            mySave << " }" << endl;
         }
      }
      mySave << "}";
      mySave.close();
   }
}


void Editor::loadFromFile(){

   fstream myLoad;
   string  buffer;

   int myLevel = 0;
   int myFilas = 0;
   int filas   = 0;
   int colms   = 0;
   int idBlock = 0;

   myLoad.open(fileSave);

   if(myLoad.is_open()){
      while( !myLoad.eof() ){
         myLoad >> buffer;
         if(buffer == "Level:"){
            myLoad >> myLevel;
            continue;
         }
         if(buffer == "filas:"){
            myLoad >> myFilas;
            cout << "Level(" << myLevel << ") filas(" << myFilas << ")" << endl;
            for(filas = 0; filas < MAX_FILAS; filas++){
               myLoad >> filas;
               myLoad >> buffer;   //. La llave {
               for(colms = 0; colms < MAX_COLMS; colms++){
                  myLoad >> idBlock;
                  ARRAY_LEVEL[myLevel][filas][colms] = idBlock;
                  cout << idBlock <<", ";
               }
               myLoad >> buffer;  //. la llave }
               cout << endl;
            }
         }
      }
      myLoad.close();
   }
}


void Editor::CopyFromArray(int level, BLOCK **block, int TOTAL){

   int filas = 0;
   int colms = 0;
   int index = TOTAL -1;
   int idColor = 0;

   for(filas=0; filas < MAX_FILAS; filas++){
      for(colms=0; colms < MAX_COLMS; colms++){
         idColor = ARRAY_LEVEL[level][filas][colms];
         block[index]->set_idColor(idColor, data_getColors(idColor));
         index--;
      }
   }
}

/***** SELECCIONADOR DE LOS NIVELES A EDITAR *******/
bool Editor::isMouseInLevel(RenderWindow *win, CIRCLE **level, int TOTAL, int *indice){
   bool Result = false;

   *indice = -1;
   for(int index=0; index < TOTAL; index++){
      level[index]->Update(&*win);

      if(level[index]->isSelected() ){
         *indice = index;
         Result = true;
      }
   }
   return Result;

}

/****** EDITOR DE BLOQUES DE NIVELES ******/
bool Editor::isMouseInBlock(RenderWindow *win, BLOCK **block, int TOTAL, int *index){
   bool Result = false;

   RectangleShape rcShape;
   Rect<float>    rcBlock;

   int px = Mouse::getPosition(*win).x;    //. posicion relativa a la win
   int py = Mouse::getPosition(*win).y;

   for(int ndx = 0; ndx < TOTAL; ndx++){
      rcShape = block[ndx]->getRectangle();
      rcBlock = Rect<float>(rcShape.getPosition().x - rcShape.getOrigin().x, \
                            rcShape.getPosition().y - rcShape.getOrigin().y, \
                            rcShape.getSize().x, \
                            rcShape.getSize().y);
      if(rcBlock.contains(Vector2f(px, py))){
         *index = ndx;
         Result = true;
         break;
      }
   }

   return Result;
}


void Editor::Process(RenderWindow *win, Event evn){
  if(evn.type == Event::MouseWheelMoved){
      if(Keyboard::isKeyPressed(Keyboard::LControl)){
         iFondo += evn.mouseWheel.delta;
         iFondo  = min(iFondo, 39);
         iFondo  = max(iFondo, 00);
         cout << "Fondo: " << iFondo << endl;
         CambiarFondo(rcGame, iFondo);

      } else {
         Wheel += evn.mouseWheel.delta;
         Wheel  = min( Wheel, NUM_COLORS );
         Wheel  = max( Wheel, 0);
         cout << "Wheel: " << Wheel << endl;
         rcCube.setFillColor( data_getColors(Wheel) );

         ostringstream buffer;
         buffer << setfill('0') << setw(2) << Wheel;
         idBlock.setString(buffer.str());
      }
   }

   //. Mouse en los Bloques
   if(isMouseInBlock(win, block, TOTAL, &index)){
      //. Seleccionar index
      cout << index << endl;

      //. insertar bloque
      if(Mouse::isButtonPressed(Mouse::Left)){
         block[index]->set_idColor(Wheel, data_getColors(Wheel));
      }
      //. eliminar bloque
      if(Mouse::isButtonPressed(Mouse::Right)){
         block[index]->set_idColor(0, (Color)0);
      }

   //. Mouse en los Niveles
   } else if(isMouseInLevel(win, level, LEVELS, &Selecto)){

      if(Mouse::isButtonPressed(Mouse::Left)){
         //. Asegura que el Click Cambia una sola vez.
         if(Selecto != -1){
            if(SELECTO != Selecto){
               //. Desmarca el Anterior
               level[SELECTO]->Set_Normal();
               //. MArca el Actual Seleccionado.
               level[Selecto]->Set_Pressed();
               //. Copia los bloques Actuales al Array
               copyToArray(SELECTO, block, TOTAL);
               SELECTO  = Selecto;
               cout << "Nivel Selecto: " << Selecto << endl;
               //. Copia desde el Array los bloques.-
               CopyFromArray(Selecto, block, TOTAL);

               ostringstream buffer;
               buffer << "Editando Nivel: " << fillZero(2) << Selecto +1;
               label_4.setString(buffer.str());
            } //. Fin, Cambio de Nivel
         } //. Fin, posible Seleccion
      } //. Mouse Pressed
   }

   Guardar->Update(win);
   if(Guardar->isPressed()){
      cout << "Guardar Archivo" << endl;
      copyToArray(0, block, TOTAL);
      SaveToTexto();
   }


   Cargar->Update(win);
   if(Cargar->isPressed()){
      cout << "Cargar Archivo" << endl;
      loadFromFile();
      CopyFromArray(0, block, TOTAL);
   }


   Salir->Update(win);
   if(Salir->isPressed()){
      ModoEditar = false;
   }
}

void Editor::Display(RenderWindow *win){
   win->draw(rcEdit);
   win->draw(rcCube);
   win->draw(label_1);
   win->draw(label_2);
   win->draw(label_3);
   win->draw(label_4);
   win->draw(idBlock);
   Guardar->Display(win);
   Cargar->Display(win);
   Salir->Display(win);
}
