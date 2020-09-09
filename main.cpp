
#define _MAIN_

#include "globals.h"
#include "Blocks.h"
#include "Circle.h"
#include "Player.h"
#include "Button.h"
#include "LEVELS.h"
#include "Animation.h"
#include "Entidad.h"

map<string, ANIMATION *> TABLA;
map<string, ANIMATION *>::iterator it;

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
//vector<ENTIDAD *>::iterator enty;  //. Usar Iterador automativo auto_ptr

#define maxRange 100000 // es 10.1234
#define minRange  20000 // es  2.1234
#define rdMargen   1000

float frand(){
    return (float) (((rand()+rdMargen) % maxRange) + minRange) / 10000;
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

/******* ENUMERACION DE LOS TIPOS DE ENEMIGOS *********/
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

/******* ENUMERACION PARA LOS EFECTOS **************/
enum Efecto { Colision, Explosion };
map<Efecto, string> NameEfecto = {
   { Efecto::Colision, "Colision"   },
   { Efecto::Explosion, "Explosion" },
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
                                Color myColor, Color myRelleno){
    RectangleShape rcBase;
    rcBase.setPosition(px, py);                     //. Posicion.-
    rcBase.setSize(Vector2f(width, height));        //. Tamaño Ancho Alto
    rcBase.setFillColor(myRelleno);                 //. Color de Relleno
    rcBase.setOutlineColor(myColor);                //. Color del Borde
    rcBase.setOutlineThickness(2);                  //. Grosor

    return rcBase;
}

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




//. *** COMANDOS GLOBALES (Pendientes) ***


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


//. *** INSTANCIAR BONUS ***
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


//. *** INSTANCIAR ENEMIGOS ***
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




//. *** INSTANCIAR EFECTOS ***
EFECTO *MakeEfecto(float px, float py, Efecto tipo){
   string keyName = NameEfecto.at(tipo);

   EFECTO *efecto = new EFECTO(px, py, new ANIMATION( *TABLA.at(keyName)));
   if(tipo == Efecto::Colision){
      float vx = ((rand()%50)+10) -25;
      float vy = ((rand()%50)+10) -25;
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

         //. Prueba de efectos
         if(Keyboard::isKeyPressed(Keyboard::F3)){
            EFECTO *efx = MakeEfecto(Vector2f(250,200), Efecto::Colision);
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
      //. auto_ptr,,, very fast.-
      for(auto enty = ENTIDADES.begin(); enty != ENTIDADES.end(); ++enty){
         //((ENTIDAD*)*enty)->Update();
         //((ENTIDAD*)*enty)->Display(&win);
         ENTIDAD *cosa = (ENTIDAD*)*enty;
         cosa->Update();
         cosa->Display(&win);

         //. Si el Bono Colisiona al Player
         if(cosa->isCollision(rcPlayer->getRectangle())){
            cosa->On_Command(rcPlayer);     /**< Ejecuta el Comando de Este Bono */

            //. Si el player es Golpeado por alguna cosa
            EFECTO *efx = MakeEfecto(cosa->getPosition(), Efecto::Explosion);
            efx->Display(&win);

            enty = ENTIDADES.erase(enty);   /**< y luego lo borra de la tabla */
            goto Revision;
            //break;
         }

         //. Para los efectos se borran cuando Finalizan
         if(cosa->get_idGrupo() == ENTIDAD::GRUPO::Efecto){
            if( ((EFECTO*)cosa)->isFinished() ){
               //cout << "Terminar el Efecto" << endl;
               enty = ENTIDADES.erase(enty);
               goto Revision;
               //break;
            }
         }



         //. Enemigo No debe morir en el rcDown
         if(cosa->get_idGrupo() == ENTIDAD::GRUPO::Enemy){
            ENEMY *enemy = (ENEMY*)cosa;
            if(enemy->isFinished()){
               EFECTO *efx = MakeEfecto(enemy->getPosition(), Efecto::Explosion);
               efx->Display(&win);
               enty = ENTIDADES.erase(enty);
               goto Revision;
               //break;
            } else {

               if(enemy->isGolpeado(rcBola->getRectangle() )){
                  rcBola->Golpeada();
                  enemy->Golpeado();
                  //cout << "Crear el Efecto" << endl;
                  EFECTO *efx = MakeEfecto(enemy->getPosition(), Efecto::Colision);
                  efx->Display(&win);
                  goto Revision;
                  //break;
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
