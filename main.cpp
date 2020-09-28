
#define _MAIN_

#include "Globals.h"
#include "Animation.h"
#include "Blocks.h"
#include "Circle.h"
#include "Player.h"
#include "Button.h"
#include "Entidad.h"

#include "Editor.hpp"

map<string, ANIMATION *> TABLA;

using namespace std;
using namespace sf;

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

#define maxRange 100000 // es 10.1234
#define minRange  20000 // es  2.1234
#define rdMargen   1000

float frand(){
    return (float) (((rand()+rdMargen) % maxRange) + minRange) / 10000;
}

/******** ENUMERACION DE LOS BONOS **************/
enum Bonus { Laser, Enlarge, Slow, Catch, Warp, Duplex, Temp, Ball3, \
             PowerUp, Resizer, Inflate, Magic, FireBall, Globe, Restore, TopDoor};


map<Bonus, string> NameBonus = {
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
   { Bonus::FireBall,"FireBall"},
   { Bonus::Globe,   "Globe" },
   { Bonus::Restore, "Restore" },
   { Bonus::TopDoor, "TopDoor" },
};

/******* ENUMERACION DE LOS TIPOS DE ENEMIGOS *********/
enum Enemy { Cone, Triangle, GlobeGreen, ExpGreen, GlobeRed, ExpRed, GlobeCyan, \
             ExpCyan, Cube, Sphere, GlobeMix, Orbit, Storm, Arco, Saturn, Magnet, \
             Ruby, Atom};

map<Enemy, string> NameEnemy = {
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

/******* Cache de Efectos y Balas *****************/
//. Win7 and XP, Buggs in table asignacion, no se pueden crear objetos
//. Dentro del mismo ciclo de la tabla ENTIDADES

struct Chispas {
   Vector2f   posicion;
   Efecto     tipo;

   Chispas() {}
   Chispas(Vector2f posicion, Efecto tipo){
      this->posicion = posicion;
      this->tipo = tipo;
   }
};
//. Cache para acumular los Efectos.
vector <Chispas> CHISPAS;

vector <BOLA *> BOLAS;

vector <LASER*> BULLETS;
#define MAXBULLETS   32

/******* Creacion de las BALAS desde el Player ****/
void Create_Bullets(float px, float py, Image *image){
   if(BULLETS.size() > MAXBULLETS){ return; }
   LASER *bullet = new LASER(px, py, 5,13, colorSelect);
   bullet->SetImagen(image);
   BULLETS.push_back(bullet);
}

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


//. *** COMANDOS GLOBALES (afecto Player y Bola) ***
void Command_Bonus(int iTipo, PLAYER *player, vector<BOLA*>*balls){

   cout << "Command Bonus: " << NameBonus.at((Bonus)iTipo) << endl;

   player->setCaptura( false );

   switch ((Bonus)iTipo) {
   case Bonus::Restore:
      player->ChangeImage(PLAYER::TIPO::Normal);
      break;
   case Bonus::Enlarge:
      player->ChangeImage(PLAYER::TIPO::Big);
      break;
   case Bonus::Resizer:
      player->ChangeImage(PLAYER::TIPO::Small);
      break;
   case Bonus::Inflate:
      player->SetInflate( true );
      break;
   case Bonus::Temp:
      player->SetInflate( false );
      break;
   case Bonus::Catch:
      player->setCaptura( true );
      break;
   case Bonus::Laser:
      player->ChangeImage(PLAYER::TIPO::Canon);
      break;
   case Bonus::Duplex:
      if(player->get_idImage() == PLAYER::TIPO::Normal){
         player->ChangeImage(PLAYER::TIPO::DuplexNormal);
      }
      if(player->get_idImage() == PLAYER::TIPO::Small){
         player->ChangeImage(PLAYER::TIPO::DuplexSmall);
      }
      if(player->get_idImage() == PLAYER::TIPO::Mini){
         player->ChangeImage(PLAYER::TIPO::DuplexMini);
      }
      if(player->get_idImage() == PLAYER::TIPO::Big){
         player->ChangeImage(PLAYER::TIPO::DuplexBig);
      }
      break;

   case Bonus::PowerUp:
      player->add_Lifes(1);
      break;

   case Bonus::Magic:
      for(auto enty = ENTIDADES.begin(); enty!=ENTIDADES.end(); ++enty){
         ENTIDAD *cosa = (ENTIDAD*)*enty;
         if(cosa->get_idGrupo() == ENTIDAD::GRUPO::Enemy){
            cosa->SetFinished( true );
            CHISPAS.push_back(Chispas(cosa->getPosition(), Efecto::Colision));
         }
      }
      player->add_Lifes(5);
      break;

   case Bonus::Slow:
      break;

   case Bonus::Warp:
      player->add_Lifes(3);
      OpenLevel = true;
      break;

   case Bonus::Ball3:
      for(int index=0; index < 3; index++){
         BOLA *bbb = new BOLA( balls->at(0) );
         bbb->Update();
         bbb->reboteDown();
         balls->push_back(bbb);
      }
      break;

   case Bonus::FireBall:
      for(auto bb=balls->begin(); bb!=balls->end(); ++bb){
         BOLA *bola = (BOLA*)*bb;
         bola->ChangeImage(BOLA::TIPO::Fuego);
      }
      break;

   case Bonus::Globe:
      for(auto bb=balls->begin(); bb!=balls->end(); ++bb){
         BOLA *bola = (BOLA*)*bb;
         bola->ChangeImage(BOLA::TIPO::Plasma);
      }
      break;

   case Bonus::TopDoor:
      break;
   }
}

void Command_Enemy(int iTipo, PLAYER *rcPlayer, vector<ENTIDAD*>*entity){

   cout << "Comandos de Enemigos: " << NameEnemy.at((Enemy)iTipo) << endl;

   switch((Enemy)iTipo){
   case Enemy::Cone:
      break;
   case Enemy::Triangle:
      break;
   case Enemy::GlobeGreen:
      break;
   case Enemy::ExpGreen:
      break;
   case Enemy::GlobeRed:
      break;
   case Enemy::ExpRed:
      break;
   case Enemy::GlobeCyan:
      break;
   case Enemy::ExpCyan:
      break;
   case Enemy::Cube:
      break;
   case Enemy::Sphere:
      break;
   case Enemy::GlobeMix:
      break;
   case Enemy::Orbit:
      break;
   case Enemy::Storm:
      break;
   case Enemy::Arco:
      break;
   case Enemy::Saturn:
      break;
   case Enemy::Magnet:
      break;
   case Enemy::Ruby:
      break;
   case Enemy::Atom:
      break;
   }
}




//. *** INSTANCIAR BONUS ***
void MakeBonus(float px, float py, Bonus tipo){

   string keyName = NameBonus.at(tipo);
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
   bonus->set_BonusTipo(tipo);
   ENTIDADES.push_back(bonus);
}
//. Overload
void MakeBonus(Vector2f pos, Bonus tipo){
   MakeBonus(pos.x, pos.y, tipo);
}


//. *** INSTANCIAR ENEMIGOS ***
ENEMY *MakeEnemy(float px, float py, Enemy tipo){
   //. Ya es comprobado que estan todas las claves.-
   string keyName = NameEnemy.at(tipo);
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
   enemy->set_EnemyTipo(tipo);
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

/******** Recorta los Fondos y los carga en la tabla FONDOS *****/
map<int, Image> FONDOS;

void Load_Fondos(Image *image, int filas, int colms, int patron = 64){

   int index = 0;

   for(int f = 0; f < filas; f++){
      for(int c = 0; c < colms; c++){
         Image fondo;
         fondo.create(patron, patron, Color(255,255,255, 255));
         fondo.copy(*image, 0, 0, IntRect(c*64, f*64, 64,64), false);
         FONDOS.insert(make_pair(index, fondo));
         cout << "FONDOS[" << index << "] : {" \
                           << c*64 << ", " \
                           << f*64 << "}" << endl;
         index++;
      }
   }

   data[0] = DATA(0, (Color)0);
   data[1] = DATA(1, Color(240,240,240));
   data[1].SetImagen(image, IntRect(256,368, 32,16), 40, 0); // White 50pts

   data[2] = DATA(2, Color(181,181,181));
   data[2].SetImagen(image, IntRect(224,368, 32,16), 50, 0); // Silver 50pts

   data[3] = DATA(3, Color(252,116,96));
   data[3].SetImagen(image, IntRect(224,336, 32,16), 60, 0); // Orange 60pts

   data[4] = DATA(4, Color(60,188,252));
   data[4].SetImagen(image, IntRect(256,320, 32,16), 70, 0); // Cyan   70pts

   data[5] = DATA(5, Color(128,208,16));
   data[5].SetImagen(image, IntRect(224,320, 32,16), 80, 0); // Green 80pts

   data[6] = DATA(6, Color(216,40,0));
   data[6].SetImagen(image, IntRect(224,352, 32,16), 90, 1); // Red  90pts 1 Golpe

   data[7] = DATA(7, Color(0,112,236));
   data[7].SetImagen(image, IntRect(224,304, 32,26), 100, 2); // Blue 100pts 2 Golpes

   data[8] = DATA(8, Color(252,116,180));
   data[8].SetImagen(image, IntRect(256,336, 32,16), 110, 3); // Pink 110pts 3 Golpes

   data[9] = DATA(9, Color(252,152,56));
   data[9].SetImagen(image, IntRect(256,352, 32,16), 120, 4); // Yellow 120pts 4 Golpes

   data[10] = DATA(10, Color(240,188,60));
   data[10].SetImagen(image, IntRect(256,304, 32,16), 130, 5); // Gold 130pts 5 Golpes

}


/******** Recorta las cañerias y tubos para hacer los bordes ****/

enum Borde { TopL, TopR, DownL, DownR, TubeVL, TubeVR, CoplaVL, CoplaVR, \
            CoplaHS, CoplaHN };

map<Borde, Image> BORDES;

void Load_Bordes(Image *image){

   Color NegroAplha = Color(0,0,0, 0);

   Image cornerTopL;
   cornerTopL.create(16, 16, NegroAplha);
   cornerTopL.copy(*image, 0,0, IntRect(240, 400, 16,16), false);
   BORDES.insert(make_pair(Borde::TopL, cornerTopL));

   Image cornerTopR;
   cornerTopR.create(16, 16, NegroAplha);
   cornerTopR.copy(*image, 0,0, IntRect(256, 400, 16,16), false);
   BORDES.insert(make_pair(Borde::TopR, cornerTopR));

   Image cornerDownL;
   cornerDownL.create(16, 16, NegroAplha);
   cornerDownL.copy(*image, 0,0, IntRect(240, 416, 16,16), false);
   BORDES.insert(make_pair(Borde::DownL, cornerDownL));

   Image cornerDownR;
   cornerDownR.create(16, 16, NegroAplha);
   cornerDownR.copy(*image, 0,0, IntRect(256, 416, 16,16), false);
   BORDES.insert(make_pair(Borde::DownR, cornerDownR));

   Image tuboVL;
   tuboVL.create(16, 32, NegroAplha);
   tuboVL.copy(*image, 0,0, IntRect(144, 336, 16,32), false);
   BORDES.insert(make_pair(Borde::TubeVL, tuboVL));

   Image tuboVR;
   tuboVR.create(16, 32, NegroAplha);
   tuboVR.copy(*image, 0,0, IntRect(192, 336, 16,32), false);
   BORDES.insert(make_pair(Borde::TubeVR, tuboVR));

   Image coplaVL;
   coplaVL.create(16, 32, NegroAplha);
   coplaVL.copy(*image, 0,0, IntRect(144, 384, 16,32), false);
   BORDES.insert(make_pair(Borde::CoplaVL, coplaVL));

   Image coplaVR;
   coplaVR.create(16, 32, NegroAplha);
   coplaVR.copy(*image, 0,0, IntRect(192, 384, 16,32), false);
   BORDES.insert(make_pair(Borde::CoplaVR, coplaVR));

   Image coplaHS;
   coplaHS.create(32, 16, NegroAplha);
   coplaHS.copy(*image, 0,0, IntRect(160, 416, 32,16), false);
   BORDES.insert(make_pair(Borde::CoplaHS, coplaHS));

   Image coplaHN;
   coplaHN.create(32, 16, NegroAplha);
   coplaHN.copy(*image, 0,0, IntRect(192, 416, 32,16), false);
   BORDES.insert(make_pair(Borde::CoplaHN, coplaHN));

   //. Graba las imagenes, para comprobar los recortes
   /*
   int index = 0;
   for(auto img = BORDES.begin(); img != BORDES.end(); img++){
       ostringstream keyName;
       keyName << "./Borde_" << fillZero(2) << index << ".png";
       ((Image)img->second).saveToFile(keyName.str());
       index++;
   }
   */


}

void Load_Puertas(Image *image){
   //. Carga las animaciones de las Compuertas
   image->createMaskFromColor(Color(0,0,0, 0));

   ANIMATION *Opening_L = new ANIMATION(image, IntRect(0, 0, 180,42), 1,10);
   Opening_L->setElapsed(0.25);
   TABLA.insert(make_pair("Opening_L", Opening_L));

   ANIMATION *Opening_R = new ANIMATION(image, IntRect(0, 0, 180,42), 1,10);
   Opening_R->setElapsed(0.25);
   TABLA.insert(make_pair("Opening_R", Opening_R));

   ANIMATION *OpenDoor_L = new ANIMATION(image, IntRect(180, 0, 270,42), 1,15);
   TABLA.insert(make_pair("OpenDoor_L", OpenDoor_L));

   ANIMATION *OpenDoor_R = new ANIMATION(image, IntRect(180, 0, 270,42), 1,15);
   TABLA.insert(make_pair("OpenDoor_R", OpenDoor_R));
}


void CambiarFondo(RectangleShape *myShape, int index, int patron){

   //. Toma el ancho y alto del retangle de juego
   int width = myShape->getSize().x;
   int height = myShape->getSize().y;

   Image tileFondo;
   tileFondo.create(width, height, Color(255,255,255, 0));

   int colms = (width / patron);
   int filas = (height / patron);
   int mx = (width % patron)  / 2;  //. Margen para centrar en X
   int my = (height % patron) / 2;  //. Margen para centrar en Y

   for(int f = 0; f < filas; f++){
      for(int c = 0; c < colms; c++){
         tileFondo.copy( FONDOS.at(index), (c*patron)+mx, (f*patron)+my, \
                         IntRect(0,0, patron, patron), false);
      }
   }
   //. Pegar los tubos y Cañerias sobre el tileFondo
   colms = 15;
   for(int c = 0; c < colms; c++){
      if(c % 2){   //. intercalados, uno por medio
         tileFondo.copy(BORDES.at(Borde::CoplaHN), (c*32)+mx +16, my, \
                        IntRect(0,0, 32, 16), true);
         tileFondo.copy(BORDES.at(Borde::CoplaHN), (c*32)+mx +16, (27*16) + my, \
                        IntRect(0,0, 32, 16), true);
      } else {
         tileFondo.copy(BORDES.at(Borde::CoplaHS), (c*32)+mx +16, my, \
                        IntRect(0,0, 32, 16), true);
         tileFondo.copy(BORDES.at(Borde::CoplaHS), (c*32)+mx +16, (27*16) + my, \
                        IntRect(0,0, 32, 16), true);
      }
   }

   filas = 27;
   for(int f = 1; f < filas -1; f++){
      if(f < 25 && (f % 3)){      //. Pone un tubo liso al final
         tileFondo.copy(BORDES.at(Borde::CoplaVL), mx+(00000), (f*16)+my, \
                        IntRect(0,0, 16,32), true);
         tileFondo.copy(BORDES.at(Borde::CoplaVR), mx+(16*31), (f*16)+my, \
                        IntRect(0,0, 16,32), true);
         f++;

      } else {
         tileFondo.copy(BORDES.at(Borde::TubeVL), mx+(00000), (f*16)+my, \
                        IntRect(0,0, 16,32), true);
         tileFondo.copy(BORDES.at(Borde::TubeVR), mx+(16*31), (f*16)+my, \
                        IntRect(0,0, 16,32), true);
      }
   }

   //. Pone las esquinas
   tileFondo.copy( BORDES.at(Borde::TopL), mx + (00000), my, \
                   IntRect(0,0, 16,16), true);
   tileFondo.copy( BORDES.at(Borde::TopR), mx + (31*16), my, \
                   IntRect(0,0, 16,16), true);

   tileFondo.copy( BORDES.at(Borde::DownL), mx + (00000),(27*16)+ my, \
                   IntRect(0,0, 16,16), true);
   tileFondo.copy( BORDES.at(Borde::DownR), mx + (31*16),(27*16)+ my, \
                   IntRect(0,0, 16,16), true);

   //tileFondo.saveToFile("./TileFondo.png");

   //. Crea la textura del Fondo y  la fija al Rectangulo
   Texture *mmmm = new Texture;
   mmmm->loadFromImage(tileFondo);
   myShape->setFillColor(Color(255,255,255, 255));
   myShape->setTexture( mmmm );

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


void UpdateScore(Text *puntos, int Puntaje){
   ostringstream buffer;
   buffer << "Puntos: " << setfill('0') << setw(6) << Puntaje;
   puntos->setString(string(buffer.str()));
}

void UpdateLifes(Text *Lifes, int vidas){
   ostringstream buffer;
   buffer << "Vidas.: " << setfill('0') << setw(2) << vidas;
   Lifes->setString(string(buffer.str()));
}


int main()
{
    sf::RenderWindow win(sf::VideoMode(800,480), "SFML Arkanoid by esbva");
    //. Solo a 60 cuadros x Segundos
    srand(time(0)); //. Activa el sed random
    win.setFramerateLimit(60);

    #ifdef RELEASED
     Time  delta;
     int   nivel = 0;
     float tiempo  = 20.0f;
     float retardo =  0.0f;
     int   LIFES   = 0;
     int   SCORE   = 0;

     delta = seconds(0.1f);
     retardo = delta.asSeconds() + tiempo;
    #endif // RELEASED

    //. Rectangulo  de Juego
    RectangleShape rcGame = Create_Rectangle(4,4, 550, 470, colorBorde);

    //. Rectangilos de colisionadores
    RectangleShape rcLeft = Create_Rectangle(20, 8, 20, 460, colorBlock);
    RectangleShape rcRight = Create_Rectangle(518, 8, 20, 460, colorBlock);
    RectangleShape rcTop = Create_Rectangle(46, 8, 466, 20, colorBlock);
    RectangleShape rcDown = Create_Rectangle(46, 448, 466, 20, colorBlock);

    //. Colisionadores de las Compuertas
    //RectangleShape rcDoor_L = Create_Rectangle(  14, 380, 32, 64, colorDoors);
    //RectangleShape rcDoor_R = Create_Rectangle( 511, 380, 32, 64, colorDoors);
    //RectangleShape rcPlayer = Create_Rectangle( 250, 400, 64, 20, colorPlayer);

    PUERTA *rcDoor_L = new PUERTA( 32, 420, 16,32, colorDoors, (Color)0 );
    PUERTA *rcDoor_R = new PUERTA(527, 420, 16,32, colorDoors, (Color)0 );


    //BOLA *rcBola = new BOLA(250, 398, 16,16, Color( 90, 90,255));
    BOLA *myBola = new BOLA(250, 420, 16,16, Color(90,90,255));
    PLAYER *rcPlayer = new PLAYER(250, 420, 64, 20, colorPlayer);
    BOLAS.push_back(myBola);

    Font font;
    if( !font.loadFromFile("./acme.ttf")){
        cout << "Error leyendo fuente: Acme.ttf";
    }

    int TOTAL = MAX_FILAS * MAX_COLMS;
    BLOCK **block = Create_Blocks(MAX_FILAS, MAX_COLMS, 4,4);

    int LEVELS = 8*6;
    CIRCLE **level = Create_Levels(8, 6, font);

    //. Rectangulo de Editor.
    Editor *editor = new Editor(&font);
    editor->SetBlock(block, TOTAL);
    editor->SetLevel(level, LEVELS);
    editor->Set_rcGame(&rcGame);

    //. Rectangulo del Menu
    RectangleShape rcMenu = Create_Rectangle(560,   4, 234, 470, colorBorde);

    //.Requiere la direccion de la fuente, para no copiarla.
    Text titulo   = Create_Label(600, 10, "Arkanoid.-", &font, 30);
    Text labScore = Create_Label(600, 60, "Puntos: 000000", &font, 20);
    Text labLifes = Create_Label(600, 90, "Vidas ..: 00", &font, 20);

    //. Lectura de los Fondos y Bordes.
    Image fondos;
    if( !fondos.loadFromFile("./arinoid.png")){
       cout << "Error leyendo Fondos: arinoid.png" << endl;
    }
    Load_Fondos(&fondos, 4, 10, 64);
    Load_Bordes(&fondos);
    CambiarFondo(&rcGame, 0);

    myBola->SetImagenes(&fondos);
    myBola->ChangeImage(BOLA::TIPO::Normal);

    rcPlayer->SetImagenes(&fondos);
    rcPlayer->ChangeImage(PLAYER::TIPO::Normal);
    UpdateLifes( &labLifes, rcPlayer->get_Lifes() );


    Image puertas;
    if( !puertas.loadFromFile("./Doors_18x42.png")){
       cout << "Error leyendo Doors_18x42.png" << endl;
    }
    Load_Puertas(&puertas);
    //. Asigna las Animaciones a las Compuertas.
    rcDoor_L->SetAnimation(new ANIMATION(*TABLA.at("Opening_L")), \
                           new ANIMATION(*TABLA.at("OpenDoor_L")));

    rcDoor_R->SetAnimation(new ANIMATION(*TABLA.at("Opening_R")), \
                           new ANIMATION(*TABLA.at("OpenDoor_R")));



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

    #ifdef RELEASED
    editor->loadFromFile();
    editor->CopyFromArray(nivel, block, TOTAL);
    level[nivel]->Set_Pressed();
    #endif // RELEASED



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

         if(Keyboard::isKeyPressed(Keyboard::F4)){
            if(rcDoor_L->getModo() == PUERTA::MODO::Cerrada){
               rcDoor_L->setModo(PUERTA::MODO::Abriendo);
               rcDoor_R->setModo(PUERTA::MODO::Abriendo);
            }
            if(rcDoor_L->getModo() == PUERTA::MODO::Abierta){
               rcDoor_L->setModo(PUERTA::MODO::Cerrando);
               rcDoor_R->setModo(PUERTA::MODO::Cerrando);
            }
         }

         if(Keyboard::isKeyPressed(Keyboard::F5)){
            BOLA *bbb = new BOLA( myBola );
            bbb->Update();
            bbb->reboteDown();
            BOLAS.push_back(bbb);
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
            efx->Display(&win);
         }

         if(ModoEditar){
            //.Mouse Rueda (Cambiar de Bloque)
            editor->Process(&win, evn);

         } //. End Modo-Editar
      }  //. end Cyclo de Eventos

      win.clear(sf::Color(0,0,0));    //. Pinta la Window de Color Azul

      win.draw(rcGame);
      win.draw(rcMenu);

      win.draw(titulo);

      #ifdef RELEASED
         if(SCORE != rcPlayer->get_Score()){
            UpdateScore(&labScore, rcPlayer->get_Score());
         }
         if(LIFES != rcPlayer->get_Lifes()){
            UpdateLifes(&labLifes, rcPlayer->get_Lifes());
         }
      #else
         win.draw(rcLeft);
         win.draw(rcRight);
         win.draw(rcTop);
         win.draw(rcDown);
      #endif // RELEASED
      win.draw(labScore);
      win.draw(labLifes);

      rcDoor_L->Display(&win);
      rcDoor_R->Display(&win);

      CForce force;
      //. Solo en el modo de edicion
      if(ModoEditar){
         editor->Display(&win);

      } else {
         //. Modo Normal de Juego
         rcPlayer->Update();

         for(auto bb=BOLAS.begin(); bb!=BOLAS.end(); ++bb){
            BOLA *rcBola = (BOLA*)*bb;
            if( !rcBola->isActiva() ){
               rcBola->setPosition(rcPlayer->px, rcPlayer->py -12);
            }
            rcBola->Update();

            if(isCollision(rcBola->getRectangle(), rcLeft, &force)){
               rcBola->reboteLeft();
            }
            if(isCollision(rcBola->getRectangle(), rcRight, &force)){
               rcBola->reboteRight();
            }
            if(isCollision(rcBola->getRectangle(), rcTop, &force)){
               rcBola->reboteTop();
            }
            if(isCollision(rcBola->getRectangle(), rcDown, &force)){
               rcBola->reboteDown();
               //. Elimina las BOLAS, pero dejara 1 sola
               if(BOLAS.size() > 1){
                  cout << "Bola Perdida: " << BOLAS.size() << endl;
                  BOLAS.erase(bb);
                  advance(bb, -1);
                  continue;
               } else {
                  //. Pierde Vidas cuando hay 1 sola BOLA
                  rcPlayer->add_Lifes(-1);
               }
            }

            if(rcPlayer->isInflatedCollision(rcBola->getRectangle())){
               if(rcPlayer->isCaptured()){
                  rcBola->setActiva( false );
                  StartGame = false;
               } else {
                  rcBola->reboteDown( rcPlayer->get_Diffx() );
                  if(StartGame){
                     rcPlayer->add_Score( 1 );
                     //. Solo 1 punto por tocar la bola.
                  }
               }
            } //. Colisionador con la (Bola/Player)
            rcBola->Display(&win);
         }
         rcPlayer->Display(&win);
      }  //. end Modo-Editar.-



      //. Desplegar los bloques
      //. Aprovechar el Mismo cyclo para Dibujar los Botones de los Niveles
      int indice  = 0;
      int activos = 0;

      CHISPAS.clear();    //. Limpia el Cache de Chispas.-

      for(int index = 0; index < TOTAL; index++){
        if(indice < LEVELS){
          level[indice]->Display(&win);
          indice++;
        }
        #ifdef RELEASED
        if( !ModoEditar ){
          if(!block[index]->get_idColor() ) { continue; }
        }
        #endif // RELEASED

        if( !block[index]->isActivo()) { continue; }

        //. Collisiones de los Lasers
        for(auto bala=BULLETS.begin(); bala!=BULLETS.end(); ++bala){
           LASER *laser = (LASER*)*bala;
           if(laser->isFinished()){ continue; }
           if(isCollision(laser->getRectangle(), block[index]->getRectangle(), &force)){
              laser->setFinished(true);
              block[index]->setActivo(false);
              CHISPAS.push_back(Chispas(laser->getPosition(), Efecto::Colision));
           }
        }

        //. Colision con los Bloques
        bool Seguir = false;
        for(auto bb=BOLAS.begin(); bb!=BOLAS.end(); ++bb){
           BOLA *rcBola = (BOLA*)*bb;

           if(isCollision(rcBola->getRectangle(), block[index]->getRectangle(), &force)){
              if(rcBola->get_idTipo() != BOLA::TIPO::Fuego){
                if(force == CForce::forceLeft ) { rcBola->reboteLeft(); }
                if(force == CForce::forceRight) { rcBola->reboteRight(); }
                if(force == CForce::forceTop)   { rcBola->reboteTop(); }
                if(force == CForce::forceDown)  { rcBola->reboteDown(); }
              }
              #ifdef RELEASED
              if(block[index]->get_Golpes() > 0) { Seguir = true; break; }
              rcPlayer->add_Score( block[index]->get_Scores() );
              #endif // RELEASED

              block[index]->setActivo(false);

              //. La probabilidad es de 1 a 10, para Bonos
              if( !(rand() % 5) ){
                  int iTipo = rand() % (int)Bonus::TopDoor;
                  MakeBonus(rcBola->getPosition(),(Bonus)iTipo);
              }

              //. Probabilidad de 5 para Enemigo a trabajar
              if( !(rand() % 8) ){
                 int iTipo = rand() & (int)Enemy::Atom;
                 //. Podria aparecer no necesariamente en la posicion de la Bola
                 //. porque puede entrar desde las compuertas de arriba.-
                 ENEMY *enemy = NULL;
                 enemy = MakeEnemy(rcBola->getPosition(), (Enemy)iTipo);
                 enemy->SetColliders(rcTop, rcDown, rcLeft, rcRight);
              }
           }
        }
        if(Seguir){ continue; }
        block[index]->Display(&win);
        activos++;
      }
      #ifdef RELEASED
      if(!activos || OpenLevel){
         StartGame = false;
         if(rcDoor_L->getModo() == PUERTA::MODO::Cerrada){
            rcDoor_L->setModo(PUERTA::MODO::Abriendo);
            rcDoor_R->setModo(PUERTA::MODO::Abriendo);
         }
         retardo -= delta.asSeconds();
         if(retardo < 0.0f){
            OpenLevel = false;
            retardo = delta.asSeconds() + tiempo;

            ENTIDADES.clear();
            CambiarFondo(&rcGame, rand() % 40);

            level[nivel]->Set_Select();
            nivel++;
            level[nivel]->Set_Pressed();

            editor->CopyFromArray(nivel, block, TOTAL);
            rcDoor_L->setModo(PUERTA::MODO::Cerrando);
            rcDoor_R->setModo(PUERTA::MODO::Cerrando);
            rcPlayer->ReStart();
            BOLAS.at(0)->ReStart();
         }
      }
      #endif // RELEASED



      //. Despliega la tabla de ENTIDADES

      for(auto enty = ENTIDADES.begin(); enty != ENTIDADES.end(); ++enty){
         //((ENTIDAD*)*enty)->Update();
         //((ENTIDAD*)*enty)->Display(&win);
         ENTIDAD *cosa = (ENTIDAD*)*enty;

         if(cosa->isFinished()){
            enty = ENTIDADES.erase(enty);
            //. si el puntero queda al final se Sale
            if(enty == ENTIDADES.end()  ) { break; }
            //. si hay mas de una cosa, retrocede
            if(enty != ENTIDADES.begin()) { advance(enty, -1); }
            continue;
         }

         cosa->Update();
         cosa->Display(&win);

         //. Si el Bono / Enemigo Colisiona al Player
         if(cosa->get_idGrupo() != ENTIDAD::GRUPO::Efecto){
            if(rcPlayer->isInflatedCollision(cosa->GetCollider())){
               /**< Ejecuta el Comando de Este Bono */
               if(cosa->get_idGrupo() == ENTIDAD::GRUPO::Bonus){
                  Command_Bonus(((BONUS*)cosa)->get_BonusTipo(), rcPlayer, &BOLAS);
               }

               if(cosa->get_idGrupo() == ENTIDAD::GRUPO::Enemy){
                 Command_Enemy(((ENEMY*)cosa)->get_EnemyTipo(), rcPlayer, &ENTIDADES);
               }

               //. Si el player es Golpeado por alguna cosa
               //. Agrega los efectos en el Cache de Chispas
               CHISPAS.push_back(Chispas(cosa->getPosition(), Efecto::Explosion));
               cosa->SetFinished( true );
            }
         }



         //. Enemigo No debe morir en el rcDown
         if(cosa->get_idGrupo() == ENTIDAD::GRUPO::Enemy){
            ENEMY *enemy = (ENEMY*)cosa;
            if(enemy->isFinished()){
               //. Agrega los efectos en el Cache de Chispas
               CHISPAS.push_back(Chispas(cosa->getPosition(), Efecto::Explosion));

            } else {
               for(auto bb=BOLAS.begin(); bb!=BOLAS.end(); ++bb){
                  BOLA *rcBola = (BOLA*)*bb;
                  if(enemy->isGolpeado(rcBola->getRectangle() )){
                     enemy->Golpeado();
                     if(rcBola->get_idTipo() == BOLA::TIPO::Plasma){
                        enemy->SetFinished(true);
                     } else {
                        rcBola->Golpeada();
                     }
                     //. Agrega los efectos en el Cache de Chispas
                     CHISPAS.push_back(Chispas(cosa->getPosition(), Efecto::Explosion));
                  }
               }
            }
            continue;
         }

         //. Si el Bono se Pierde
         if(cosa->isCollision(rcDown)){
            cout << "Bono Perdido: " << cosa->get_idGrupo() << endl;
            cosa->SetFinished( true );
         }
      }

      //. Segun el Cache de Chispas, crea los efectos.-
      for(auto mmm = CHISPAS.begin(); mmm != CHISPAS.end(); mmm++){
         Chispas *dta = &*mmm;
         MakeEfecto(dta->posicion, dta->tipo);
      }

      //. Actualizar las BULLETS
      for(auto bala=BULLETS.begin(); bala!=BULLETS.end(); ++bala){
         LASER *laser = (LASER*)*bala;
         if(laser->isFinished()){
            bala = BULLETS.erase(bala);
            if(bala == BULLETS.end())    { break; }
            if(bala != BULLETS.begin()) { advance(bala, -1); }
         }
         laser->Update();
         laser->Display(&win);
      }

      win.display();

    }

    cout << "Hello world!" << endl;
    return 0;
}
