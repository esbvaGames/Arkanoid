
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <map>

enum CForce { forceLeft=1, forceRight, forceTop, forceDown };

//. Para usar la libreria Standard
using namespace std;
//. Para usar la librería SFML Espacio de Nombres.-
using namespace sf;


//#include "LEVELS.h"

#define MAX_LEVEL 50
#define MAX_FILAS 12    //. Niveles mas altos tienen mas filas
#define MAX_COLMS 13
int ARRAY_LEVEL[MAX_LEVEL][MAX_FILAS][MAX_COLMS] = {};

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

#define NUM_COLORS 10

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

Color data_getColors(int idColor){
   return data[idColor].colors;
}

int data_getGolpes(int idColor){
   return data[idColor].Golpes;
}


int   Puntaje   = 0;
bool  StartGame = false;
bool  ModoEditar = true;
bool  Mouse_in_block = false;

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


#define maxRange 100000 // es 10.1234
#define minRange  20000 // es  2.1234
#define rdMargen   1000

float frand(){
    return (float) (((rand()+rdMargen) % maxRange) + minRange) / 10000;
}

struct BLOCK {
    //. Constructor que no hace nada, es una formalidad
    BLOCK() {};

    //. Si no se especifica el color de relleno, el relleno sera NULL.-
    BLOCK(float px, float py, Color myRelleno = (Color)NULL ){
        rcBlock.setPosition(px, py);
        rcBlock.setSize(Vector2f(32, 16));
        rcBlock.setFillColor(myRelleno);
        rcBlock.setOutlineColor(colorBlock);
        rcBlock.setOutlineThickness(1);
        rcBlock.setOrigin(16, 8);
    }
    //. Dibuja el Bloque
    void Display(RenderWindow *win){
        win->draw(rcBlock);
    }

    void setActivo(bool state){
       Activo = state;
    }
    bool isActivo() { return Activo; }

    RectangleShape getRectangle(){
       return rcBlock;
    }

    void set_idColor(int idColor, Color myColor){
       this->idColor = idColor;
       rcBlock.setFillColor(myColor);
    }

    int get_idColor(){ return idColor; }

protected:
    int  idColor = 0;
    bool Activo  = true;
    RectangleShape rcBlock;
};

//. Un contenedor de Bloques de 10 filas y 13 Columnas
BLOCK **Create_Blocks(int FILAS, int COLMS, int sx, int sy){
    int TOTAL = FILAS * COLMS;
    int index = TOTAL -1;        //. Para que se ordene desde abajo al 1

    BLOCK **block = (BLOCK**) new BLOCK[TOTAL];
    for(int filas = 0; filas < FILAS; filas++){
        for(int colms = 0; colms < COLMS; colms++){
            block[index] = new BLOCK((colms*32) +(colms+1)*sx + 59,
                                     (filas*16) +(filas+1)*sy + 64);
            index--;
        }
    }

    return block;
}

struct CIRCLE {
    Font           font;
    Text           prompt;
    CircleShape rcCircle;
    //. Constructor que es una formalidad
    CIRCLE() {}

    CIRCLE(float px, float py, Color myColor = (Color)NULL){
        rcCircle.setPosition(px, py);
        rcCircle.setRadius(15);
        rcCircle.setFillColor(colorRojo_alpha);
        rcCircle.setOutlineColor(colorRojo);
        rcCircle.setOutlineThickness(2);
        this->px = px;
        this->py = py;
        //. La mitad despues del centro de circulo.
        area = Rect<float>(px -4, py -4, 32, 32);
    }

    void Update(RenderWindow *win){
       float mx = Mouse::getPosition(*win).x;
       float my = Mouse::getPosition(*win).y;

       if(area.contains(Vector2f(mx, my))){
          if(state != STATE::Pressed){
             state = STATE::Select;
          }

       } else {
          if(state != STATE::Pressed){
             state = STATE::Normal;
          }
       }
    }

    bool isSelected() {
       return ((state == STATE::Select) ? true : false);
    }
    void Set_Pressed(){  state = STATE::Pressed;  }
    void Set_Normal() {  state = STATE::Normal;   }

    void Display(RenderWindow *win){
        switch(state){
        case STATE::Select:
           rcCircle.setFillColor(colorSelect_alpha);
           rcCircle.setOutlineColor(colorSelect);
           break;

        case STATE::Pressed:
           rcCircle.setFillColor(colorPressed_alpha);
           rcCircle.setOutlineColor(colorPressed);
           break;

        default:
           if(state != STATE::Pressed){
              if(ModoEditar){
                 rcCircle.setFillColor(colorBorde_alpha);
                 rcCircle.setOutlineColor(colorBorde);
              } else {
                 rcCircle.setFillColor(colorRojo_alpha);
                 rcCircle.setOutlineColor(colorRojo);
              }
           }
        }

        win->draw(rcCircle);
        win->draw(prompt);
    }

    void setTexto(string texto, Font font, float Scale = 12){
        this->font = font ;            //. Guarda una copia de la fuente
        prompt = Text(texto, this->font, Scale);
        float bx = (32 - prompt.getLocalBounds().width) / 2.5f;
        float by = (32 - prompt.getLocalBounds().height) / 2.5f;
        prompt.setPosition(px +bx, py +by);
        prompt.setFillColor(colorYellow);
    }
private:
    enum          STATE {Disabled, Normal, Select, Pressed};
    STATE         state = STATE::Normal;
    Rect<float>   area;
    float px, py;

};

bool isMouseInLevel(RenderWindow *win, CIRCLE **level, int TOTAL, int *indice){
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




//. Contenedor para los botones de Niveles.
//. Rojo nivel Bloqueado
//. Verde nivel Superado
CIRCLE **Create_Levels(int FILAS, int COLMS, Font font){
    int LEVELS = 8*6;
    int index = 0;
    char buffer[4]="";

    CIRCLE **level = (CIRCLE**) new CIRCLE[LEVELS];
    for(int filas = 0; filas < FILAS; filas++){
        for(int colms = 0; colms < COLMS; colms++){
            level[index] = new CIRCLE(colms*32 +(colms+1)*4 + 568,
                                      filas*32 +(filas+1)*4 + 140);
            sprintf(buffer,"%02i", index+1);
            level[index]->setTexto(string(buffer), font);
            index++;
        }
    }
    return level;
}

//. Rectangulo de Menu
//. El color de relleno se sobrepone al color del objeto anterior
//. Dal lo mismo si se rellena con el color de fondo Negro
//. Para anular el color de fondo se usa un parametro a NULL.-
//. El color de relleno sera anulado a menos que se especifique en el
//. ultimo parametro.-

RectangleShape Create_Rectangle(float px, float py, float width, float height, \
                                Color myColor, Color myRelleno = (Color)NULL){
    RectangleShape rcBase;
    rcBase.setPosition(px, py);                     //. Posicion.-
    rcBase.setSize(Vector2f(width, height));        //. Tamaño Ancho Alto
    rcBase.setFillColor(myRelleno);                 //. Color de Relleno
    rcBase.setOutlineColor(myColor);                //. Color del Borde
    rcBase.setOutlineThickness(2);                  //. Grosor

    return rcBase;
}

//. Control de la BOLA
struct BOLA {

   BOLA() {}

   BOLA(float px, float py, float width, float height, Color myColor){
      rcBola = Create_Rectangle(px, py, width, height, myColor);
      rcBola.setOrigin((width / 2.0f) , (height / 2.0f));
      this->px = px;
      this->py = py;
   }

   void Update(){
      if( !StartGame) { return; }
      if( !Activa ){
         Activa = true;
         vx =  frand();
         vy = -frand();
      }
      px += vx;
      py += vy;
      rcBola.setPosition(px, py);
   }

   void reboteLeft()  { vx =  frand(); vy = frand() -frand(); }
   void reboteRight() { vx = -frand(); vy = frand() -frand(); }
   void reboteTop()   { vy =  frand(); vx = frand() -frand(); }
   void reboteDown()  { vy = -frand(); vx = frand() -frand(); }


   void Display(RenderWindow *win){
      win->draw(rcBola);
   }

   RectangleShape getRectangle(){
      return rcBola;
   }

//. Proteccion de datos, solo da acceso a clases heredadas,
//. esta aca, solo porque el editor me muestra un color distinto de las funciones
//. jajajaa.
protected:
   float Activa = false;
   float px, py, vx, vy;   //. Velocidad en x,y
   RectangleShape rcBola;

};


//. CONTROL DEL PLAYER
struct PLAYER{
   float px, py;
   RectangleShape rcPlayer;

   PLAYER() {}

   PLAYER(float px, float py, float width, float height, Color myColor){
      rcPlayer = Create_Rectangle(px, py, width, height, myColor);
      rcPlayer.setOrigin(width / 2.0f, height / 2.0f);
      this->px = px;
      this->py = py;
   }

   void Update() {
      if(Keyboard::isKeyPressed(Keyboard::Space)){
         StartGame = true;
      }

      //. Solo funciona con una tecla presionada una vez;
      if(Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::D)){
         px -= ((px > 76+1) ? 3 : 0);
         rcPlayer.setPosition(px, py);
         //cout << "Left: " << px << endl;
      }

      if(Keyboard::isKeyPressed(Keyboard::D) && !Keyboard::isKeyPressed(Keyboard::A)){
         px += ((px < 480+1) ? 3 : 0);
         rcPlayer.setPosition(px, py);
         //cout << "Right: " << px << endl;
      }

   }

   void Display(RenderWindow *win){
      win->draw(rcPlayer);
   }

   RectangleShape getRectangle(){
      return rcPlayer;
   }

};

//. Calcular los Rectangulos de Colisiones
bool isCollision(RectangleShape rcBola, RectangleShape rcBase, CForce *force){
   Rect<float> myBola = Rect<float>(rcBola.getPosition().x - rcBola.getOrigin().x, \
                                    rcBola.getPosition().y - rcBola.getOrigin().y, \
                                    rcBola.getSize().x, \
                                    rcBola.getSize().y);

   //. Obtiene el punto Central del Rectangulo
   Vector2f  ccBola = Vector2f(myBola.left + rcBola.getOrigin().x, \
                               myBola.top  + rcBola.getOrigin().y);

   //. Obtiene los rectangulos de los Bordes o Bloques
   Rect<float> myBase = Rect<float>(rcBase.getPosition().x -rcBase.getOrigin().x, \
                                    rcBase.getPosition().y -rcBase.getOrigin().y, \
                                    rcBase.getSize().x,\
                                    rcBase.getSize().y);

   //. Obtiene el Centro de Rectangulo a comparar
   Vector2f ccBase = Vector2f(myBase.left + (rcBase.getSize().x / 2.0f), \
                              myBase.top  + (rcBase.getSize().y / 2.0f) );

   if(myBola.intersects(myBase)){
      if(ccBola.x > ccBase.x) { *force = CForce::forceLeft;  };
      if(ccBola.x < ccBase.x) { *force = CForce::forceRight; };
      if(ccBola.y > ccBase.y) { *force = CForce::forceTop;   };
      if(ccBola.y < ccBase.y) { *force = CForce::forceDown;  };
      return true;
   }

   return false;
}

/****** EDITOR DE BLOQUES DE NIVELES ******/
bool isMouseInBlock(RenderWindow *win, BLOCK **block, int TOTAL, int *index){
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

int index = -1;
int INDEX = 0;
int Wheel = 0;
int SELECTO = 0;
int Selecto = 0;

string fileSave = "./GameLevel.txt";

#define fillZero(n)   setfill('0') << setw(n)


void copyToArray(int level, BLOCK **block, int TOTAL){
  int filas = 0;
  int colms = 0;

  for(int index=0; index < TOTAL; index++){
     colms = index % MAX_COLMS;
     filas = index / MAX_COLMS;
     //cout << "filas(" << filas << " colms(" << colms << ") " << block[index]->get_idColor() << endl;
     ARRAY_LEVEL[level][filas][colms] = block[(TOTAL -1) -index]->get_idColor();
  }
}

bool SaveToTexto(int FILAS=15){
   ofstream mySave;

   int level =  0;
   int filas =  0;

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

bool loadFromFile(){

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

void CopyFromArray(int level, BLOCK **block, int TOTAL){

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

/******************************************/

Text Create_Label(float px, float py, string texto, Font *font, float Scale){
   Text label = Text(texto, *font, Scale);
   label.setFillColor(colorYellow);
   label.setPosition(px, py);

   return label;
}

struct BUTTON {
   BUTTON() {};
   BUTTON(float px, float py, float width, float height, \
          string texto, Font *font, float Scale){
      rcButton = Create_Rectangle(px, py, width, height, colorBorde, colorBorde_alpha);
      label    = Create_Label(0,0, texto, &*font, Scale);
      float bx = (width - label.getLocalBounds().width ) / 2.0f;
      float by = (height - label.getLocalBounds().height) / 3.5f;
      label.setPosition(px + bx, py + by);
      label.setFillColor(colorBorde);
      area = Rect<float>(px, py, width, height);
   }
   void Update(RenderWindow *win){
      float mx = Mouse::getPosition(*win).x;
      float my = Mouse::getPosition(*win).y;
      if(area.contains(Vector2f(mx, my))){
         state = STATE::Select;
         if(Mouse::isButtonPressed(Mouse::Left)){
            state = STATE::Pressed;
         }
      } else {
         state = STATE::Normal;
      }
   }

   void Display(RenderWindow *win){
      switch(state){
      case STATE::Select:
         rcButton.setFillColor(colorSelect_alpha);
         rcButton.setOutlineColor(colorSelect);
         label.setFillColor(colorSelect);
         break;

      case STATE::Pressed:
         rcButton.setFillColor(colorPressed_alpha);
         rcButton.setOutlineColor(colorPressed);
         label.setFillColor(colorPressed);
         break;

      default:
         rcButton.setFillColor(colorBorde_alpha);
         rcButton.setOutlineColor(colorBorde);
         label.setFillColor(colorBorde);
      }
      win->draw(rcButton);
      win->draw(label);
   }

   bool isPressed(){
      return (state == STATE::Pressed) ? true : false;
   }

protected:
   enum  STATE {Normal, Select, Pressed};
   STATE state = STATE::Normal;
   Text             label;
   Rect<float>      area;
   RectangleShape   rcButton;
};




int main()
{
    sf::RenderWindow win(sf::VideoMode(800,480), "SFML Arkanoid by esbva");
    //. Solo a 60 cuadros x Segundos
    srand(time(0)); //. Activa el sed random
    win.setFramerateLimit(60);

    //. Rectangulo  de Juego
    RectangleShape rcGame = Create_Rectangle(4,4, 550, 470, colorBorde);

    //. Rectangilos de colisionadores
    RectangleShape rcLeft = Create_Rectangle(20, 8, 20, 460, colorBlock);
    RectangleShape rcRight = Create_Rectangle(518, 8, 20, 460, colorBlock);
    RectangleShape rcTop = Create_Rectangle(46, 8, 466, 20, colorBlock);
    RectangleShape rcDown = Create_Rectangle(46, 448, 466, 20, colorBlock);

    //. Colisionadores de las Compuertas
    RectangleShape rcDoor_L = Create_Rectangle(  14, 380, 32, 64, colorDoors);
    RectangleShape rcDoor_R = Create_Rectangle( 511, 380, 32, 64, colorDoors);
    //RectangleShape rcPlayer = Create_Rectangle( 250, 400, 64, 20, colorPlayer);


    BOLA *rcBola = new BOLA(250, 398, 16,16, Color( 90, 90,255));
    PLAYER *rcPlayer = new PLAYER(250, 420, 64, 20, colorPlayer);

    Font font;
    if( !font.loadFromFile("./acme.ttf")){
        cout << "Error leyendo fuente: Acme.ttf";
    }

    int TOTAL = MAX_FILAS * MAX_COLMS;
    BLOCK **block = Create_Blocks(MAX_FILAS, MAX_COLMS, 4,4);

    int LEVELS = 8*6;
    CIRCLE **level = Create_Levels(8, 6, font);

    //. Rectangulo del Menu
    RectangleShape rcMenu = Create_Rectangle(560,   4, 234, 470, colorBorde);

    //.Requiere la direccion de la fuente, para no copiarla.
    Text titulo = Create_Label(600, 10, "Arkanoid.-", &font, 30);
    Text puntos = Create_Label(600, 60, "Puntos: 000000", &font, 20);
    Text vidas = Create_Label(600, 90, "Vidas ..: 00", &font, 20);

    //. Rectangulo de Editor.
    RectangleShape rcEdit = Create_Rectangle(46, 310, 466, 158, colorBorde, Color(34,38,38));
    RectangleShape rcCube = Create_Rectangle(100, 342, 32,32, colorBorde);

    Text idBlock = Create_Label(106, 320, "00", &font, 15);
    Text label_1 = Create_Label(220, 310, "Modo Edicion", &font, 20);
    Text label_2 = Create_Label(150, 340, "Rueda para cambiar Bloque", &font, 15);
    Text label_3 = Create_Label(150, 360, "Click en Numeros para Cambiar-Nivel", &font, 15);
    Text label_4 = Create_Label(150, 390, "Editando Nivel: 00", &font, 15);

    BUTTON *Guardar = new BUTTON(310, 386, 70, 25, "Guardar", &font, 14);
    BUTTON *Cargar  = new BUTTON(400, 386, 70, 25, "Cargar", &font, 14);
    BUTTON *Salir   = new BUTTON(230, 425, 70, 25, "Salir", &font, 14);

    while(win.isOpen()){

      sf::Event  evn;                   //. Receptor de Eventos
      while( win.pollEvent(evn) ) {
         if(evn.type == sf::Event::Closed){
            win.close();
         }
         //. habilita la tecla Escape para cerrar la Window
         if(Keyboard::isKeyPressed(Keyboard::Escape)){
            win.close();
         }

         //. Activar Modo-Editar
         if(Keyboard::isKeyPressed(Keyboard::F12)){
            ModoEditar = !ModoEditar;   //. Invertir estado (true | false)
            cout << "ModoEditar: " << ModoEditar << endl;
         }


         if(ModoEditar){
            //.Mouse Rueda (Cambiar de Bloque)
            if(evn.type == Event::MouseWheelMoved){
               Wheel += evn.mouseWheel.delta;
               Wheel  = min( Wheel, NUM_COLORS );
               Wheel  = max( Wheel, 0);
               cout << "Wheel: " << Wheel << endl;
               rcCube.setFillColor( data_getColors(Wheel) );

               ostringstream buffer;
               buffer << setfill('0') << setw(2) << Wheel;
               idBlock.setString(buffer.str());
            }

            //. Mouse en los Bloques
            if(isMouseInBlock(&win, block, TOTAL, &index)){
               //. Seleccionar index
               cout << index << endl;

               //. insertar bloque
               if(Mouse::isButtonPressed(Mouse::Left)){
                  block[index]->set_idColor(Wheel, data_getColors(Wheel));
               }
               //. eliminar bloque
               if(Mouse::isButtonPressed(Mouse::Right)){
                  block[index]->set_idColor(0, (Color) NULL);
               }

            //. Mouse en los Niveles
            } else if(isMouseInLevel(&win, level, LEVELS, &Selecto)){

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

            Guardar->Update(&win);
            if(Guardar->isPressed()){
               cout << "Guardar Archivo" << endl;
               copyToArray(0, block, TOTAL);
               SaveToTexto();
            }


            Cargar->Update(&win);
            if(Cargar->isPressed()){
               cout << "Cargar Archivo" << endl;
               loadFromFile();
               CopyFromArray(0, block, TOTAL);
            }


            Salir->Update(&win);
            if(Salir->isPressed()){
               ModoEditar = false;
            }
         } //. End Modo-Editar
      }  //. end Cyclo de Eventos

      win.clear(sf::Color(0,0,0));    //. Pinta la Window de Color Azul

      win.draw(rcGame);
      win.draw(rcMenu);

      win.draw(titulo);
      win.draw(puntos);
      win.draw(vidas);

      win.draw(rcLeft);
      win.draw(rcRight);
      win.draw(rcTop);
      win.draw(rcDown);

      win.draw(rcDoor_L);
      win.draw(rcDoor_R);

      CForce force;
      //. Solo en el modo de edicion
      if(ModoEditar){
         win.draw(rcEdit);
         win.draw(rcCube);
         win.draw(label_1);
         win.draw(label_2);
         win.draw(label_3);
         win.draw(label_4);
         win.draw(idBlock);
         Guardar->Display(&win);
         Cargar->Display(&win);
         Salir->Display(&win);


      } else {
         //. Modo Normal de Juego
         rcBola->Update();

         if(isCollision(rcBola->getRectangle(), rcLeft, &force)){
            cout << "Colision left: " << force << endl;
            rcBola->reboteLeft();
         }

         if(isCollision(rcBola->getRectangle(), rcRight, &force)){
            cout << "Colision right: " << force << endl;
            rcBola->reboteRight();
         }

         if(isCollision(rcBola->getRectangle(), rcTop, &force)){
            cout << "Colision top: " << force << endl;
            rcBola->reboteTop();
         }

         if(isCollision(rcBola->getRectangle(), rcDown, &force)){
            cout << "Colision Down - Bola perdida" << force << endl;
            rcBola->reboteDown();
         }

         rcPlayer->Update();
         rcPlayer->Display(&win);
         if(isCollision(rcBola->getRectangle(), rcPlayer->getRectangle(), &force)){
            if(force == CForce::forceLeft) { rcBola->reboteLeft(); }
            if(force == CForce::forceRight) { rcBola->reboteRight(); }
            rcBola->reboteDown();

            Puntaje +=1;
            cout << "Acierto del Player: " << Puntaje << endl;

            ostringstream buffer;
            buffer << "Puntos: " << setfill('0') << setw(6) << Puntaje;
            puntos.setString(string(buffer.str()));
         }
         rcBola->Display(&win);

      }  //. end Modo-Editar.-



      //. Desplegar los bloques
      //. Aprovechar el Mismo cyclo para Dibujar los Botones de los Niveles
      int indice = 0;
      for(int index = 0; index < TOTAL; index++){
        if(indice < LEVELS){
          level[indice]->Display(&win);
          indice++;
        }
        if( !block[index]->isActivo()) { continue; }
        //. Colision con los Bloques
        if(isCollision(rcBola->getRectangle(), block[index]->getRectangle(), &force)){
           if(force == CForce::forceLeft ) { rcBola->reboteLeft(); }
           if(force == CForce::forceRight) { rcBola->reboteRight(); }
           if(force == CForce::forceTop)   { rcBola->reboteTop(); }
           if(force == CForce::forceDown)  { rcBola->reboteDown(); }
           block[index]->setActivo(false);
           continue;
        }
        block[index]->Display(&win);
      }
      win.display();

    }

    cout << "Hello world!" << endl;
    return 0;
}
