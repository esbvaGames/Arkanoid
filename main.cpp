
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
bool  ModoEditar = false;
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

/** \brief Crea una forma grafica rectangular
 *
 * \param   px: posicion en x
 * \param   py: posicion en y
 * \param   width: ancho
 * \param   height: alto
 * \param   myColor: color de los bordes
 * \param   myRelleno: color de relleno (NULL) Predeterminado
 * \return  RectangleShape
 *
 */
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

   Vector2f getPosition(){
      return Vector2f(px, py);
   }

   void Golpeada(){
      vx = frand() - frand();
      vy = frand() - frand();
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


/** \brief  Crea una etiqueta de texto
 *
 * \param   px: posicion en x
 * \param   py: posicion en y
 * \param   texto: texto de la etiqueta
 * \param   &font: direccion de la fuente
 * \param   Scale: escala del texto
 * \return  Text
 *
 */

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

/***** CONTROL DE ANIMACIONES *****/
struct ANIMATION{
   ANIMATION() {};

   //. Constructor de Copia
   ANIMATION(const ANIMATION *Other){
      *this = Other;
   }

   ANIMATION(Image *image, IntRect area, int filas, int colms){
      texture.loadFromImage(*image, area);
      this->filas = filas;
      this->colms = colms;
      this->height = area.height / filas;
      this->width  = area.width  / colms;
      this->TOTAL  = filas * colms;

      IntRect region(0,0,width,height);

      sprites = (Sprite **) new Sprite[TOTAL];
      for(int index=0; index < TOTAL; index++){
         region.left = (index % colms) * width;
         region.top  = (index / colms) * height;
         region.width = width;
         region.height = height;
         sprites[index] = new Sprite(texture, region);
         //cout << "region:(" << fillZero(3) << region.left <<","<< \
         //                      fillZero(3) << region.top <<")" << endl;
      }
      modo = MODO::Normal;
      setElapsed( 0.15 );
   }

   void setElapsed(float factor){
      this->factor = factor;
      delta = seconds(0.1f);
      lapso = delta.asSeconds() + factor;
   }


   bool Display(RenderWindow *win){
      bool finished = false;     //. Marca si la animacion a finalizado
      if(modo == MODO::Back){
        finished = UpdateBack();
      } else {
        finished = UpdatePlay();
      }
      sprites[index]->setPosition(px, py);
      //. Para tratar la posicion desde el Centro de la Animacion;
      sprites[index]->setOrigin(width / 2.0f, height / 2.0f);
      win->draw(*sprites[index]);
      return finished;
   }

   void setPosition(float px, float py){
      this->px = px;
      this->py = py;
   }

   Vector2f  getPosition(){
      return Vector2f(px, py);
   }

   float getWidth()  { return width;   }
   float getHeight() { return height; }

   enum MODO { Normal, Back};
   void setModo(MODO modo){
      this->modo = modo;
   }

protected:
   bool  isTimeLapse(){
      lapso -= delta.asSeconds();
      if(lapso <= 0.0f){
         lapso = delta.asSeconds() + factor;
         return true;
      }
      return false;
   }


   bool UpdatePlay(){
      if ( !isTimeLapse() ) { return false; }
      index = ((index+1 < TOTAL) ? index +1 : 0);   //. Ciclo hacia adelante
      return  ((index == TOTAL -1) ? true : false);  //. True en el ultimo Frame
   }

   bool UpdateBack(){
      if( !isTimeLapse() ) { return false; }
      index = ((index > 0000) ? index -1 : TOTAL -1); //. Ciclo hacia atras
      return  ((index == 0000) ? true : false);       //. True si llega al 000

   }

private:
   MODO        modo = MODO::Normal;
   float       px = 0, py =0;
   int         index = 0;
   int         TOTAL = 0;
   int         filas, height;
   int         colms, width;
   Texture     texture;
   Sprite      **sprites;        //. Array de Sprites
   Time        delta;
   float       lapso;
   float       factor;

};

map<string, ANIMATION *> TABLA;
map<string, ANIMATION *>::iterator it;

/**** ESTRUCTURA DE ENTIDADES, BONOS Y ENEMIGOS *****/
typedef void (callback) (void *);

struct ENTIDAD {
   ENTIDAD() {}

   virtual void Update() = 0;
   virtual void Display(RenderWindow *win) = 0;

   enum GRUPO {Bonus, Enemy, Efecto};

   void SetVelocity(float vx, float vy, GRUPO idGrupo){
      this->vx = vx;
      this->vy = vy;
      this->idGrupo = idGrupo;
   }

   bool isCollision(RectangleShape spOther){
      Rect<float> rcOther(spOther.getPosition().x - spOther.getOrigin().x, \
                          spOther.getPosition().y - spOther.getOrigin().y, \
                          spOther.getSize().x, \
                          spOther.getSize().y);
      if(area.intersects(rcOther)){
         return true;
      }
      return false;
   }

   void SetCommand(callback *command){
      this->command = command;
   }

   void On_Command(void *data){
      command(data);
   }

   GRUPO get_idGrupo() { return idGrupo; }

   Vector2f  getPosition() {
      return Vector2f(px, py);
   }


protected:
   void SetAnimation(ANIMATION *anim){
      this->anim = anim;      /**< asigna la animacion    */
      SetCollider();          /**< asigna el colisionador */
   }

   void SetCollider(){
     area = Rect<float>(anim->getPosition().x - (anim->getWidth() / 2.0f), \
                        anim->getPosition().y - (anim->getHeight() / 2.0f), \
                        anim->getWidth(), \
                        anim->getHeight());
   }

   void SetPosition(float px, float py){
      this->px = px;
      this->py = py;
      SetCollider();
      anim->setPosition(px, py);
   }

   GRUPO           idGrupo;     //. tipo de Entidad
   float          px, py;     //. Posicion
   float          vx, vy;     //. Velocidad vx,vy;
   Rect<float>    area;       //. Area de Colision
   ANIMATION      *anim;      //. Animacion
   callback       *command;   //. Funcion del Bonus

};

struct BONUS : public ENTIDAD {
   BONUS() {};

   BONUS(float px, float py, ANIMATION *anim){
      SetAnimation(anim);
      SetPosition(px, py);
   }

   void Update(){
      SetPosition(px +vx, py+vy);
   }

   void Display(RenderWindow *win){
      anim->Display(win);
   }
};

struct ENEMY : public ENTIDAD {
   ENEMY() {}

   ENEMY(float px, float py, ANIMATION *anim){
      SetAnimation(anim);
      SetPosition(px, py);
      golpes = (rand()+8) % 16;    //. Minimo 8 golpes max 16.-
   }
   void Display(RenderWindow *win){
      anim->Display(win);
   }
   //. Carga las Referencias de los Colisionadores Principales
   //. Podria ser una funcion friend, pero aqui se evita escritura en
   //. El parrafo central de proceso.-
   void SetColliders(RectangleShape rcTop, RectangleShape rcDown, \
                     RectangleShape rcLeft, RectangleShape rcRight){
      this->rcTop  = rcTop;
      this->rcDown = rcDown;
      this->rcLeft = rcLeft;
      this->rcRight = rcRight;
   }

   //. Comportamiento Diferente
   void Update(){
      if( !Activo ){
         Activo = true;
         vx = frand() - frand();
         vy = frand() - frand();
      }
      SetPosition(px + vx, py + vy);

      if(isCollision(rcTop))  { vy =  frand(); }
      if(isCollision(rcDown)) { vy = -frand(); }
      if(isCollision(rcLeft)) { vx =  frand(); }
      if(isCollision(rcRight)){ vx = -frand(); }

      //. Otros comportamientos segun el Tipo de Enemigo
   }

   //. Si el Enemigo es golpeado por la Bola
   bool isGolpeado(RectangleShape rcBola){
      retardo -= 1;            //. Cuando se crea, siempre esta colisionando
      if(retardo <= 0.0){      //. Este retardador le da tiempo a moverse.-
         if(isCollision(rcBola)){
            golpes -= 1;
            cout << "Enemy Golpeado: " << golpes << endl;
            if(golpes <= 0.0) {  finished = true; }
            return true;
         }
      }
      return false;
   }


   //. Si el enemigo es Golpeado, cambia de direccion
   void Golpeado() {
      vx = frand() - frand();
      vy = frand() - frand();
   }

   bool isFinished()  { return finished; }


protected:
   int golpes    =  0;               //. Golpes de resistencia del Enemigo
   int retardo   = 30;               //. Retardo del detector de colisiones
   bool finished = false;            //. Enemigo ha sido Destruido

   bool Activo = false;
   //. Los enemigos Guardan una Referencia de los Bordes Colisionadores
   RectangleShape   rcTop;
   RectangleShape   rcDown;
   RectangleShape   rcLeft;
   RectangleShape   rcRight;
};

struct EFECTO : public ENTIDAD {
   EFECTO() {}

   EFECTO(float px, float py, ANIMATION *anim){
      SetAnimation(anim);
      SetPosition(px, py);
   }

   void Update(){
      anim->setPosition(px +vx, py +vy);
   }

   void Display(RenderWindow *win){
      finished = anim->Display(win);   //. Retornado desde la Animacion
   }

   bool isFinished() { return finished; }

protected:
   //. Marca si la animación del efecto ha finalizado.-
   bool finished = false;

};



/**** TABLA DE ENTIDADES ******************
Azul     : Hace la nave más larga
Roja     : Proporciona a la nave un cañon que puede disparar
Rosado   : Abre una puerta a la que puedes pasar de nivel
Celeste  : Te propoporciona tres bolas
Plomo    : Aumenta el numero de vidas
Verde    : Atrapa la pelota
Naranja  : Hace que la pelota vaya más lenta
*/

vector<ENTIDAD *> ENTIDADES;
vector<ENTIDAD *>::iterator enty;

//. Crea 2 Cañones de Balas.-
void Command_Canons(void *data){
   PLAYER *player = (PLAYER*)data;
   cout << "Bonus Canons: player(" << player->px << "," << player->py << ")" << endl;
}
//. Abre las Compuertas para pasar de Nivel
void Command_Puertas(void *data){
   cout << " Bonus Compuertas" << endl;
}

//. Multiplica las Bolas x 3
void Command_Bolas_x3(void *data){
   cout << "Bonus 3 Bolas extras" << endl;
}

void Command_Vida_Extra(void *data){
   cout << "Bonus Vida Extra" << endl;
}

void Command_Atrapar_Bola(void *data){
   cout << "Bonus Atrapar Bola" << endl;
}

void Command_Bola_Lenta(void *data){
   cout << "Bonus Bola Lenta" << endl;
}

void Command_Encoger_Tambor(void *data){
   cout << "Encoger Tambor Player" << endl;
}

void Command_Generar_Tornado(void *data){
   cout << "Generar Tornado" << endl;
}

void Create_Canons(Image *imagen, float px, float py){
    ANIMATION *vtmO = new ANIMATION(imagen, IntRect(0, 192, 32, 112), 7, 1);
    vtmO->setElapsed(0.5f);

    BONUS *bonus = new BONUS(px, py, vtmO);

    bonus->SetVelocity(0, frand(), ENTIDAD::GRUPO::Bonus);
    bonus->SetCommand( &Command_Canons);
    ENTIDADES.push_back(bonus);
}
/******** ENUMERACION DE LOS BONOS **************/
enum Bonus { Laser, Enlarge, Slow, Catch, Warp, Duplex, Temp, Ball3, \
             PowerUp, Resizer, Inflate, Magic, Globe, Restore, TopDoor};


map<Bonus, string> Names = {
   { Bonus::Laser,   "Laser" },
   { Bonus::Enlarge, "Enlarge" },
   { Bonus::Slow,    "Slow" },
   { Bonus::Catch,   "Catch" },
   { Bonus::Warp,    "Warp" },
   { Bonus::Duplex,  "Duplex" },
   { Bonus::Temp,    "Temp" },
   { Bonus::Ball3,   "Ball3" },
   { Bonus::PowerUp, "PowerUp" },
   { Bonus::Resizer, "Resizer" },
   { Bonus::Inflate, "Inflate" },
   { Bonus::Magic,   "Magic" },
   { Bonus::Globe,   "Globe" },
   { Bonus::Restore, "Restore" },
   { Bonus::TopDoor, "TopDoor" },
};


void MakeBonus(float px, float py, Bonus tipo){

   string keyName = Names.at(tipo);
   cout << "Bonus: " << keyName << endl;

   ANIMATION *anim = NULL;
   //. No debiera fallar, Si encuentra todas las palabras Clave
   try {
      anim = new ANIMATION( *TABLA.at(keyName) );
   } catch(exception &e){
      //. Podria haber un nombre incorrecto.-
      cout << "Error en: " << e.what() <<" => " << keyName << endl;
      return;   //. se sale.-
   }
   BONUS *bonus = new BONUS(px, py, anim);
   bonus->SetVelocity(0, frand(), ENTIDAD::GRUPO::Bonus);
   bonus->SetCommand( &Command_Canons );
   ENTIDADES.push_back(bonus);
}
//. Overload
void MakeBonus(Vector2f pos, Bonus tipo){
   MakeBonus(pos.x, pos.y, tipo);
}


enum Enemy { Cone, Triangle, GlobeGreen, ExpGreen, GlobeRed, ExpRed, GlobeCyan, \
             ExpCyan, Cube, Sphere, GlobeMix, Orbit, Storm, Arco, Saturn, Magnet, \
             Ruby, Atom};

map<Enemy, string> Enemies = {
   { Enemy::Cone,       "Cone" },
   { Enemy::Triangle,   "Triangle" },
   { Enemy::GlobeGreen, "GlobeGreen" },
   { Enemy::ExpGreen,   "ExpGreen" },
   { Enemy::GlobeRed,   "GlobeRed" },
   { Enemy::ExpRed,     "ExpRed" },
   { Enemy::GlobeCyan,  "GlobeCyan" },
   { Enemy::ExpCyan,    "ExpCyan" },
   { Enemy::Cube,       "Cube" },
   { Enemy::Sphere,     "Sphere" },
   { Enemy::GlobeMix,   "GlobeMix" },
   { Enemy::Orbit,      "Orbit" },
   { Enemy::Storm,      "Storm" },
   { Enemy::Arco,       "Arco" },
   { Enemy::Saturn,     "Saturn" },
   { Enemy::Magnet,     "Magnet" },
   { Enemy::Ruby,       "Ruby" },
   { Enemy::Atom,       "Atom" },
};

ENEMY *MakeEnemy(float px, float py, Enemy tipo){
   //. Ya es comprobado que estan todas las claves.-
   string keyName = Enemies.at(tipo);
   cout << "Enemies: " << keyName << " : " << tipo << endl;

   //. No debiera fallar, si encuentra todas las palabras clave.-
   ANIMATION * anim = NULL;
   try {
      anim = new ANIMATION( *TABLA.at(keyName) );

   } catch(exception &e){
      //. Podria haber un nombre incorrecto.-
      cout << "Error: " << e.what() << " => " << keyName << endl;
      return NULL;   //. Se Sale
   }

   ENEMY *enemy = new ENEMY(px, py, anim);
   enemy->SetVelocity(0,0, ENTIDAD::GRUPO::Enemy);
   enemy->SetCommand( &Command_Canons );
   ENTIDADES.push_back(enemy);

   //. para cargar los Rectangulos de Colision Principales
   return enemy;
}

//.   Overload
ENEMY *MakeEnemy(Vector2f pos, Enemy tipo){
   return MakeEnemy(pos.x, pos.y, tipo);
}

enum Efecto { Colision, Explosion };
map<Efecto, string> NameEfecto = {
   { Efecto::Colision, "Colision"   },
   { Efecto::Explosion, "Explosion" },
};


EFECTO *MakeEfecto(float px, float py, Efecto tipo){
   string keyName = NameEfecto.at(tipo);

   EFECTO *efecto = new EFECTO(px, py, new ANIMATION( *TABLA.at(keyName)));
   if(tipo == Efecto::Colision){
      float vx = ((rand()%80)+10) -40;
      float vy = ((rand()%80)+10) -40;
      efecto->SetVelocity(vx, vy, ENTIDAD::GRUPO::Efecto);
   } else {
      efecto->SetVelocity(0,0, ENTIDAD::GRUPO::Efecto);
   }
   ENTIDADES.push_back(efecto);
   return efecto;
}

EFECTO *MakeEfecto(Vector2f pos, Efecto tipo){
   return MakeEfecto(pos.x, pos.y, tipo);
}


/******** LECTURA DE LETRAS BONUS ***************/
void Load_Bonus(Image *imagen){

   int cc = 40;
   //. Laser
   ANIMATION *Laser = new ANIMATION(imagen, IntRect(0,0,256,16), 1,8);
   Laser->setPosition(cc, 1);
   TABLA.insert(make_pair("Laser", Laser));

   cc += 40;
   //. Enlarge
   ANIMATION *Enlarge= new ANIMATION(imagen, IntRect(0,16,256,16), 1,8);
   Enlarge->setPosition(cc, 1);
   TABLA.insert(make_pair("Enlarge", Enlarge));

    cc += 40;
    //. Slow
    ANIMATION *Slow= new ANIMATION(imagen, IntRect(0,32,256,16), 1,8);
    Slow->setPosition(cc, 1);
    TABLA.insert(make_pair("Slow", Slow));

    cc += 40;
    //. Catch
    ANIMATION *Catch= new ANIMATION(imagen, IntRect(0,48,256,16), 1,8);
    Catch->setPosition(cc, 1);
    TABLA.insert(make_pair("Catch", Catch));

    cc += 40;
    //. Warp
    ANIMATION *Warp= new ANIMATION(imagen, IntRect(0,64,256,16), 1,8);
    Warp->setPosition(cc, 1);
    TABLA.insert(make_pair("Warp", Warp));

    cc += 40;
    //. Duplex
    ANIMATION *Duplex= new ANIMATION(imagen, IntRect(0,80,256,16), 1,8);
    Duplex->setPosition(cc, 1);
    TABLA.insert(make_pair("Duplex", Duplex));

    cc += 40;
    //. Temp
    ANIMATION *Temp= new ANIMATION(imagen, IntRect(0,96,256,16), 1,8);
    Temp->setPosition(cc, 1);
    TABLA.insert(make_pair("Temp", Temp));


    cc += 40;
    //. Ball3
    ANIMATION *Ball3= new ANIMATION(imagen, IntRect(0,112,256,16), 1,8);
    Ball3->setPosition(cc, 1);
    TABLA.insert(make_pair("Ball3", Ball3));

    cc += 40;
    //. PowerUp
    ANIMATION *PowerUp= new ANIMATION(imagen, IntRect(0,128,256,16), 1,8);
    PowerUp->setPosition(cc, 1);
    TABLA.insert(make_pair("PowerUp", PowerUp));

    cc += 40;
    //. Resizer
    ANIMATION *Resizer= new ANIMATION(imagen, IntRect(0,144,256,16), 1,8);
    Resizer->setPosition(cc, 1);
    TABLA.insert(make_pair("Resizer", Resizer));

    cc += 40;
    //. Inflate
    ANIMATION *Inflate= new ANIMATION(imagen, IntRect(0,160,256,32), 1,8);
    Inflate->setPosition(cc, 1);
    TABLA.insert(make_pair("Inflate", Inflate));

    cc += 40;
    //. Magic
    ANIMATION *Magic= new ANIMATION(imagen, IntRect(0,192,256,16), 1,8);
    Magic->setPosition(cc, 1);
    TABLA.insert(make_pair("Magic", Magic));

    cc = 40;
    //. FireBall
    ANIMATION *FireBall= new ANIMATION(imagen, IntRect(0,208,192,32), 1,6);
    FireBall->setPosition(cc, 30);
    TABLA.insert(make_pair("FireBall", FireBall));

    cc += 40;
   //. Globe
   ANIMATION *Globe= new ANIMATION(imagen, IntRect(0,256,256,16), 1,8);
   Globe->setPosition(cc, 30);
   TABLA.insert(make_pair("Globe", Globe));

   cc += 40;
   //. Restore
   ANIMATION *Restore= new ANIMATION(imagen, IntRect(0,272,256,16), 1,8);
   Restore->setPosition(cc, 30);
   TABLA.insert(make_pair("Restore", Restore));

   cc += 40;
   //. TopDoor
   ANIMATION *TopDoor= new ANIMATION(imagen, IntRect(0,288,256,16), 1,8);
   TopDoor->setPosition(cc, 30);
   TABLA.insert(make_pair("TopDoor", TopDoor));

}

void Load_Enemies(Image *imagen){
   int cc = 40;
   //. Cone
   ANIMATION *Cone= new ANIMATION(imagen, IntRect(0,0,256,32), 1,8);
   Cone->setPosition(cc, 70);
   TABLA.insert(make_pair("Cone", Cone));

  cc += 40;
  //. Triangle
  ANIMATION *Triangle= new ANIMATION(imagen, IntRect(256,0,352,32), 1,11);
  Triangle->setPosition(cc, 70);
  TABLA.insert(make_pair("Triangle", Triangle));

  cc += 40;
  //. GlobeGreen
  ANIMATION *GlobeGreen= new ANIMATION(imagen, IntRect(0,32,384,32), 1,12);
  GlobeGreen->setPosition(cc, 70);
  TABLA.insert(make_pair("GlobeGreen", GlobeGreen));
  //cc += 40;
  //. ExpGreen
  ANIMATION *ExpGreen= new ANIMATION(imagen, IntRect(384,32,192,32), 1,6);
  ExpGreen->setPosition(cc, 100);
  TABLA.insert(make_pair("ExpGreen", ExpGreen));

  cc += 40;
  //. GlobeRed
  ANIMATION *GlobeRed= new ANIMATION(imagen, IntRect(0,64,384,32), 1,12);
  GlobeRed->setPosition(cc, 70);
  TABLA.insert(make_pair("GlobeRed", GlobeRed));
  //cc += 40;
  //. ExpRed
  ANIMATION *ExpRed= new ANIMATION(imagen, IntRect(384,64,192,32), 1,6);
  ExpRed->setPosition(cc, 100);
  TABLA.insert(make_pair("ExpRed", ExpRed));


  cc += 40;
  //. GlobeCyan
  ANIMATION *GlobeCyan= new ANIMATION(imagen, IntRect(0,96,384,32), 1,12);
  GlobeCyan->setPosition(cc, 70);
  TABLA.insert(make_pair("GlobeCyan", GlobeCyan));
  //cc += 40;
  //. ExpCyan
  ANIMATION *ExpCyan= new ANIMATION(imagen, IntRect(384,96,192,32), 1,6);
  ExpCyan->setPosition(cc, 100);
  TABLA.insert(make_pair("ExpCyan", ExpCyan));

   cc += 40;
   //. Cube
   ANIMATION *Cube= new ANIMATION(imagen, IntRect(608,0,160,64), 2,5);
   Cube->setPosition(cc, 70);
   TABLA.insert(make_pair("Cube", Cube));

   cc += 40;
   //. Sphere
   ANIMATION *Sphere= new ANIMATION(imagen, IntRect(608,64,160,64), 2,5);
   Sphere->setPosition(cc, 70);
   TABLA.insert(make_pair("Sphere", Sphere));

   cc += 40;
   //. GlobeMix
   ANIMATION *GlobeMix= new ANIMATION(imagen, IntRect(0,128,768,32), 1,24);
   GlobeMix->setPosition(cc, 70);
   TABLA.insert(make_pair("GlobeMix", GlobeMix));

   cc += 40;
   //. Orbit
   ANIMATION *Orbit= new ANIMATION(imagen, IntRect(0,160,384,32), 1,12);
   Orbit->setPosition(cc, 70);
   TABLA.insert(make_pair("Orbit", Orbit));

   cc += 40;
   //. Storm
   ANIMATION *Storm= new ANIMATION(imagen, IntRect(384,160,384,32), 1,12);
   Storm->setPosition(cc, 70);
   TABLA.insert(make_pair("Storm", Storm));

   cc += 40;
   //. Arco
   ANIMATION *Arco= new ANIMATION(imagen, IntRect(0,192,480,32), 1,15);
   Arco->setPosition(cc, 70);
   TABLA.insert(make_pair("Arco", Arco));

   cc += 40;
   //. Saturn
   ANIMATION *Saturn= new ANIMATION(imagen, IntRect(512,192,256,32), 1,8);
   Saturn->setPosition(cc, 70);
   TABLA.insert(make_pair("Saturn", Saturn));

   cc = 40;
   //. Magnet
   ANIMATION *Magnet= new ANIMATION(imagen, IntRect(0,224,768,32), 1,24);
   Magnet->setPosition(cc,140);
   TABLA.insert(make_pair("Magnet", Magnet));

   cc += 40;
   //. Ruby
   ANIMATION *Ruby= new ANIMATION(imagen, IntRect(0,256,736,32), 1,23);
   Ruby->setPosition(cc, 140);
   TABLA.insert(make_pair("Ruby", Ruby));

   cc += 40;
   //. Atom
   ANIMATION *Atom= new ANIMATION(imagen, IntRect(0,288,512,64), 2,16);
   Atom->setPosition(cc, 140);
   TABLA.insert(make_pair("Atom", Atom));

}

void Load_Efectos(Image *efecto){

   ANIMATION *explode = new ANIMATION(efecto, IntRect(0,0, 505, 64), 1,8);
   explode->setPosition(450, 100);
   TABLA.insert(make_pair("Explosion", explode));

   ANIMATION *colision = new ANIMATION(efecto, IntRect(0,64, 280, 40), 1,7);
   colision->setPosition(400, 110);
   TABLA.insert(make_pair("Colision", colision));

}



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

    //. Prueba de Animaciones Bonus ///
    Image imagen;
    if( !imagen.loadFromFile("./PowerUps.png")){
      cout << "Error leyendo imagen: PowerUps.bmp" << endl;
    }
    imagen.createMaskFromColor(Color(0,0,0),0);
    Load_Bonus(&imagen);

    //. Animaciones de Enemigos;
    Image enemies;
    if( !enemies.loadFromFile("./Enemies.png")){
      cout << "Error leyendo Enemies: Enemies.png" << endl;
    }
    Load_Enemies(&enemies);

    //. Explosiones y otros efectos
    Image effect;
    if( !effect.loadFromFile("./Efectos.png")) {
      cout << "Error leyendo Efectos: Efectos.png" << endl;
    }
    Load_Efectos(&effect);



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

         //. Prueba para los Bonos
         if(Keyboard::isKeyPressed(Keyboard::F1)){
            int iTipo = rand() % ((int)Bonus::TopDoor);
            int px = (rand() % 200) + 100;
            MakeBonus(px, 100,(Bonus) iTipo);
         }

         //. Prueba para los enemigos
         if(Keyboard::isKeyPressed(Keyboard::F2)){
            int iTipo = rand() % ((int)Enemy::Atom);
            int px = (rand() % 200) + 100;
            ENEMY *enemy = MakeEnemy(px, 100, (Enemy) iTipo);
            enemy->SetColliders(rcTop, rcDown, rcLeft, rcRight);

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

           //. La probabilidad es de 1 a 10, para Bonos
           if( !(rand() % 10) ){
               int iTipo = rand() % (int)Bonus::TopDoor;
               MakeBonus(rcBola->getPosition(),(Bonus)iTipo);
           }

           //. Probabilidad de 5 para Enemigo a trabajar
           if( !(rand() % 5) ){
              int iTipo = rand() & (int)Enemy::Atom;
              //. Podria aparecer no necesariamente en la posicion de la Bola
              //. porque puede entrar desde las compuertas de arriba.-
              ENEMY *enemy = NULL;
              enemy = MakeEnemy(rcBola->getPosition(), (Enemy)iTipo);
              enemy->SetColliders(rcTop, rcDown, rcLeft, rcRight);
           }

           continue;
        }
        block[index]->Display(&win);
      }

      //. Despliega la tabla de ENTIDADES
Revisar:
      for(enty = ENTIDADES.begin(); enty != ENTIDADES.end(); ++enty){
         //((ENTIDAD*)*enty)->Update();
         //((ENTIDAD*)*enty)->Display(&win);
         ENTIDAD *cosa = (ENTIDAD*)*enty;
         cosa->Update();
         cosa->Display(&win);

         //. Si el Bono Colisiona al Player
         if(cosa->isCollision(rcPlayer->getRectangle())){
            cosa->On_Command(rcPlayer);  /**< Ejecuta el Comando de Este Bono */

            //. Si el player es Golpeado por alguna cosa
            EFECTO *efx = MakeEfecto(cosa->getPosition(), Efecto::Explosion);
            efx->Display(&win);

            enty = ENTIDADES.erase(enty);       /**< y luego lo borra de la tabla */
            goto Revision;
         }

         //. Para los efectos se borran cuando Finalizan
         if(cosa->get_idGrupo() == ENTIDAD::GRUPO::Efecto){
            if( ((EFECTO*)cosa)->isFinished() ){
               //cout << "Terminar el Efecto" << endl;
               enty = ENTIDADES.erase(enty);
               goto Revision;
            }
         }



         //. Enemigo No debe morir en el rcDown
         if(cosa->get_idGrupo() == ENTIDAD::GRUPO::Enemy){
            ENEMY *enemy = (ENEMY*)cosa;
            if(enemy->isFinished()){
               EFECTO *efx = MakeEfecto(enemy->getPosition(), Efecto::Explosion);
               efx->Display(&win);

               enty = ENTIDADES.erase(enty);
               break;
            } else {

               if(enemy->isGolpeado(rcBola->getRectangle() )){
                  rcBola->Golpeada();
                  enemy->Golpeado();
                  //cout << "Crear el Efecto" << endl;
                  EFECTO *efx = MakeEfecto(enemy->getPosition(), Efecto::Colision);
                  efx->Display(&win);

                  break;
               }
            }
            continue;
         }

         //. Si el Bono se Pierde
         if(cosa->isCollision(rcDown)){
            cout << "Bono Perdido: " << cosa->get_idGrupo() << endl;
            enty = ENTIDADES.erase(enty);
         }
Revision:
         //. Si borra algo y el puntero queda en el ultimo se sale.
         if(enty == ENTIDADES.end() ) { break; }

      }


      //. Desplieque de Animaciones
      //for( it = TABLA.begin(); it != TABLA.end(); ++it){
      //   ((ANIMATION*)it->second)->Display(&win);
      //}



      win.display();

    }

    cout << "Hello world!" << endl;
    return 0;
}
