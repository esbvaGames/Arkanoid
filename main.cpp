
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <iostream>

enum CForce { forceLeft=1, forceRight, forceTop, forceDown };

//. Para usar la libreria Standard
using namespace std;
//. Para usar la librería SFML Espacio de Nombres.-
using namespace sf;

int   Puntaje   = 0;
bool  StartGame = false;

Color colorBorde = Color( 74, 134, 232);
Color colorBlock = Color(255,  90,  90);
Color colorYellow = Color(255, 255, 0);
Color colorPlayer = Color(90, 255, 90);
Color colorDoors  = Color( 0, 255, 255);

Color colorRojo  = Color(255, 0, 0);
Color colorVerde = Color(0, 255, 0);


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

protected:
    bool Activo = true;
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
        rcCircle.setRadius(16);
        rcCircle.setFillColor(colorRojo);
        rcCircle.setOutlineColor(colorVerde);
        rcCircle.setOutlineThickness(1);
        this->px = px;
        this->py = py;
    }

    void Display(RenderWindow *win){
        win->draw(rcCircle);
        win->draw(prompt);
    }

    void setTexto(string texto, Font font, float Scale = 12){
        this->font = font ;            //. Guarda una copia de la fuente
        prompt = Text(texto, this->font, Scale);
        prompt.setPosition(px +8, py +8);
        prompt.setFillColor(colorYellow);
    }
private:
    float px, py;

};


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
         vx =  (rand() % 8) -4;
         vy = -(rand() % 8);
      }
      px += vx;
      py += vy;
      rcBola.setPosition(px, py);
   }

   void reboteLeft()  { vx =  ((rand() % 8) -4) +2; }
   void reboteRight() { vx = -((rand() % 8) -4) -2; }
   void reboteTop()   { vy =  ((rand() % 8) -4) +2; vx = ((rand() % 8) -4) +2; }
   void reboteDown()  { vy = -((rand() % 8) -4) -2; vx = ((rand() % 8) -4) -2; }


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
   Rect<float> myBola = Rect<float>(rcBola.getPosition().x, \
                                    rcBola.getPosition().y, \
                                    rcBola.getSize().x, \
                                    rcBola.getSize().y);

   //. Obtiene el punto Central del Rectangulo
   Vector2f  ccBola = Vector2f(myBola.left + rcBola.getOrigin().x, \
                               myBola.top  + rcBola.getOrigin().y);

   //. Obtiene los rectangulos de los Bordes o Bloques
   Rect<float> myBase = Rect<float>(rcBase.getPosition().x, \
                                    rcBase.getPosition().y, \
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




int main()
{
    sf::RenderWindow win(sf::VideoMode(800,480), "SFML Arkanoid by esbva");
    //. Solo a 60 cuadros x Segundos
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

    int TOTAL = 10*13;
    BLOCK **block = Create_Blocks(10, 13, 4,4);

    int LEVELS = 8*6;
    CIRCLE **level = Create_Levels(8, 6, font);

    //. Rectangulo del Menu
    RectangleShape rcMenu = Create_Rectangle(560, 4, 234, 470, colorBorde);


    Text titulo;
    titulo.setFont(font);
    titulo.setString("Arkanoid.-");
    titulo.setPosition(600, 10);
    titulo.setFillColor(colorYellow);

    Text puntos;
    puntos.setFont(font);
    puntos.setString("Puntos: 000000");
    puntos.setPosition(600, 60);
    puntos.setScale(Vector2f(0.6, 0.6));
    puntos.setFillColor(colorYellow);

    Text vidas;
    vidas.setFont(font);
    vidas.setString("Vidas .: 00");
    vidas.setPosition(600, 90);
    vidas.setScale(Vector2f(0.6, 0.6));
    vidas.setFillColor(colorYellow);


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

      }

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


      rcBola->Update();

         CForce force;
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
            char buffer[5]="";
            sprintf(buffer, " Puntos: %-6i", Puntaje);
            puntos.setString(string(buffer));
         }

      rcBola->Display(&win);



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
