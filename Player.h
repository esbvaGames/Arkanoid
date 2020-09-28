#ifndef _PLAYER_H
#define _PLAYER_H

#include <math.h>
using namespace sf;

#define TIMER_FUEGO   250
#define TIMER_PLASMA  500

//. Control de la BOLA
struct BOLA {
   enum TIPO { Normal, Mini, Fuego, Plasma};

   BOLA() {}

   BOLA(const BOLA *Other){   //. Llamado como:  new BOLA( *other )
      *this = *Other;
   }
   BOLA(const BOLA &Other){   //. Llamado como:  new BOLA(  other )
      *this = Other;
   }

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
         vx =  frand() - frand();
         vy = -frand();
      }
      px += vx;
      py += vy;
      rcBola.setPosition(px, py);

      animFuego->setPosition(px, py);
      animPlasma->setPosition(px, py);

      if(tipo == TIPO::Fuego){
         timerFuego -= 1;
         if(timerFuego < 0){ ChangeImage(TIPO::Normal); }
      }
      if(tipo == TIPO::Plasma){
         timerPlasma -=1;
         if(timerPlasma < 0){ ChangeImage(TIPO::Normal); }
      }

   }

   void reboteLeft()  {
      vx =  frand();
      vy =((fabs(vy) < 1.0f) ? frand() -frand() : vy);
   }
   void reboteRight() {
      vx = -frand();
      vy =((fabs(vy) < 1.0f) ? frand() -frand() : vy);
   }

   void reboteTop() {
       vy =  frand();
       vx = ((fabs(vx) < 0.0f) ? vx : frand() -frand());
   }
   void reboteDown(float diffx = 0.0f){
      vy = -frand();
      vx = ((diffx != 0.0f) ? diffx : frand() -frand());
   }

   void Display(RenderWindow *win){

      switch(tipo){
      case TIPO::Fuego:       animFuego->Display(win); break;
      case TIPO::Plasma:      animPlasma->Display(win); break;
      default:
         win->draw(rcBola);
      }
   }

   RectangleShape getRectangle(){
      return rcBola;
   }

   Vector2f getPosition(){
      return Vector2f(px, py);
   }

   void setPosition(float px, float py){
      this->px = px;  vx = 0;
      this->py = py;  vy = 0;
      rcBola.setPosition(px, py);
      animFuego->setPosition(px, py);
      animPlasma->setPosition(px, py);
   }


   bool isActiva() { return Activa; }
   void setActiva(bool state) { Activa = state; }

   void Golpeada(){
      vx = frand() - frand();
      vy = frand() - frand();
   }

   void SetImagenes(Image *image){
      Color NegroAplpha = Color(0,0,0, 0);

      ball_Normal.create(10,10, NegroAplpha);
      ball_Normal.copy(*image, 0,0, IntRect(435,275, 10,10), true);

      ball_Mini.create(6,6, NegroAplpha);
      ball_Mini.copy(*image, 0,0, IntRect(453,277, 6,6), true);

      animFuego = new ANIMATION(image, IntRect(464,272,20,10), 1,2);
      animPlasma = new ANIMATION(image, IntRect(496, 269, 80,20), 1,4);


   }

   void ChangeImage(TIPO tipo){
      this->tipo = tipo;

      Texture *mmm = new Texture();

      switch(tipo){
      case TIPO::Fuego:
      case TIPO::Plasma:
      case TIPO::Normal:  mmm->loadFromImage(ball_Normal); break;
      case TIPO::Mini:    mmm->loadFromImage(ball_Mini);   break;
      }

      rcBola.setFillColor(Color(255,255,255, 255));
      rcBola.setOutlineColor(colorSelect);
      rcBola.setOutlineThickness( 1 );
      rcBola.setOrigin( mmm->getSize().x / 2.0f, mmm->getSize().y / 2.0f);
      rcBola.setSize(Vector2f(mmm->getSize().x, mmm->getSize().y));
      rcBola.setTexture(mmm, true);
      rcBola.setPosition(px, py);

      timerFuego = TIMER_FUEGO;
      timerPlasma = TIMER_PLASMA;
   }


   TIPO get_idTipo() { return tipo; }

   void ReStart(){
      px = 250;
      py = 410;
      ChangeImage(TIPO::Normal);
      Activa    = false;
      StartGame = false;
   }


protected:
   float Activa = false;
   float px, py, vx, vy;   //. Velocidad en x,y
   RectangleShape rcBola;

   TIPO           tipo = TIPO::Normal;
   Image          ball_Normal;
   Image          ball_Mini;
   ANIMATION      *animFuego;
   ANIMATION      *animPlasma;

private:
   int timerFuego  = TIMER_FUEGO;
   int timerPlasma = TIMER_PLASMA;
};

//. CONTROL DEL LASER

struct LASER {
   LASER() {}

   LASER(float px, float py, float width, float height, Color myColor){
      rcBullet = Create_Rectangle(px, py, width, height, myColor);
      rcBullet.setOrigin(width / 2.0f, height / 2.0f);
      rcBullet.setPosition(px, py);
      rcBullet.setOutlineThickness( 0 );
      this->px = px;
      this->py = py;
   }

   void SetImagen(Image *image){
      Texture *mmm = new Texture();
      mmm->loadFromImage(*image);

      rcBullet.setFillColor(Color(255,255,255, 255));
      rcBullet.setTexture(mmm, true);
   }

   void Update(){
      py -= 5;
      rcBullet.setPosition(px, py);
      if(py < 35){ finished = true; }
   }

   void Display(RenderWindow *win){
      win->draw(rcBullet);
   }

   Vector2f getPosition() { return rcBullet.getPosition(); }

   RectangleShape getRectangle() { return rcBullet; }

   bool isFinished() { return finished; }
   void setFinished(bool state){ finished = state; }


protected:
   bool                 finished = false;
   float                px, py;
   RectangleShape       rcBullet;

};





//. CONTROL DEL PLAYER
struct PLAYER{
   float px, py;
   RectangleShape rcPlayer;
   enum TIPO {Normal, Canon, Small, Mini, Big, \
              DuplexNormal, DuplexSmall, DuplexMini, DuplexBig};

   friend void Create_Bullets(float px, float py, Image *image);

   PLAYER() {}

   PLAYER(float px, float py, float width, float height, Color myColor){
      rcPlayer = Create_Rectangle(px, py, width, height, myColor);
      rcPlayer.setOrigin(width / 2.0f, height / 2.0f);
      this->px = px;
      this->py = py;
   }

   void Update() {
      if(Keyboard::isKeyPressed(Keyboard::Space)){
         if( !OpenLevel ){
            StartGame = true;
            OpenLevel = false;
         }
      }

      float mx = rcPlayer.getOrigin().x;
      //. Solo funciona con una tecla presionada una vez;
      if(Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::D)){
         px -= ((px > 42+mx) ? 4 : 0);
         rcPlayer.setPosition(px, py);
         //cout << "Left: " << px << endl;
      }

      if(Keyboard::isKeyPressed(Keyboard::D) && !Keyboard::isKeyPressed(Keyboard::A)){
         px += ((px < 516-mx) ? 4 : 0);
         rcPlayer.setPosition(px, py);
         //cout << "Right: " << px << endl;
      }

      if(Mouse::isButtonPressed(Mouse::Left) || Keyboard::isKeyPressed(Keyboard::Space)){
         if(tipo != TIPO::Canon){ return; }

         steps -= 0.95;
         if(steps < 0.0f){
            steps = STEPS;
            Create_Bullets(px -(mx -5), py -12, &bullet);
            Create_Bullets(px +(mx +5), py -12, &bullet);
         }
      }

      exp_Normal->setPosition(px, py);
      exp_Canon->setPosition(px, py);
      exp_Small->setPosition(px, py);
      exp_Mini->setPosition(px, py);
      exp_Big->setPosition(px, py);
   }

   bool isFinished() { return finished; }
   bool setFinished(bool state) { return finished = state; }

   void Display(RenderWindow *win){
      if(finished){
         if(exploted){
            ReStart();
         } else {
             switch(tipo){
             case TIPO::Normal:
             case TIPO::DuplexNormal:
               exploted = exp_Normal->Display(win);
               break;
             case TIPO::Canon:
               exploted = exp_Canon->Display(win);
               break;
             case TIPO::Small:
             case TIPO::DuplexSmall:
               exploted = exp_Small->Display(win);
               break;
             case TIPO::Mini:
             case TIPO::DuplexMini:
               exploted = exp_Mini->Display(win);
               break;
             case TIPO::Big:
             case TIPO::DuplexBig:
               exploted = exp_Big->Display(win);
               break;
             }
         }
      } else {
        if(inflated){
           ax += (px - paddle_0->getPosition().x) * 0.062;
           paddle_0->setPosition(ax, py);
           win->draw(*paddle_0);
           win->draw( getCollider(paddle_0));

           bx += (ax - paddle_1->getPosition().x) * 0.062;
           paddle_1->setPosition(bx, py);
           win->draw(*paddle_1);
           win->draw( getCollider(paddle_1));

           cx += (bx - paddle_2->getPosition().x) * 0.062;
           paddle_2->setPosition(cx, py);
           win->draw(*paddle_2);
           win->draw( getCollider(paddle_2));

        }

        win->draw(rcPlayer);
      }

   }



   Rect<float> GetCollider(Sprite *paddle){
      Rect<float> rcPaddle = \
                  Rect<float>(paddle->getPosition().x - paddle->getOrigin().x, \
                              paddle->getPosition().y - paddle->getOrigin().y, \
                              paddle->getTextureRect().width, \
                              paddle->getTextureRect().height);

      return rcPaddle;
   }



   bool isInflatedCollision(RectangleShape rcOther){
      Rect<float> myOther = \
                  Rect<float>(rcOther.getPosition().x - rcOther.getOrigin().x, \
                              rcOther.getPosition().y - rcOther.getOrigin().y, \
                              rcOther.getSize().x, \
                              rcOther.getSize().y);

      Rect<float> myPlayer = \
                  Rect<float>(rcPlayer.getPosition().x - rcPlayer.getOrigin().x, \
                              rcPlayer.getPosition().y - rcPlayer.getOrigin().y, \
                              rcPlayer.getSize().x, \
                              rcPlayer.getSize().y);

      if(myPlayer.intersects(myOther)){
         diffx = myOther.getPosition().x - px;
         diffx = min(diffx,  2.5f);
         diffx = max(diffx, -2.5f);
         return true;
      }

      if(inflated){
         if(myOther.intersects(GetCollider( paddle_0)) || \
            myOther.intersects(GetCollider( paddle_1)) || \
            myOther.intersects(GetCollider( paddle_2))) {
            return true;
         }
      }
      return false;
   }

   float get_Diffx() { return diffx; }

   RectangleShape getCollider(Sprite *paddle){
      RectangleShape shape;
      shape.setFillColor( (Color)0 );
      shape.setOutlineColor(colorBorde);
      shape.setOutlineThickness(1);

      shape.setPosition( paddle->getPosition() );
      shape.setSize( Vector2f( paddle->getTextureRect().width, \
                               paddle->getTextureRect().height ));
      shape.setOrigin( paddle->getOrigin() );
      return shape;
   }


   RectangleShape getRectangle(){
      return rcPlayer;
   }

   void SetImagenes(Image *image){
      Color NegroAlpha = Color(0,0,0, 255);

      pad_Normal.create(48, 12, NegroAlpha);
      pad_Normal.copy(*image,0,0, IntRect(296, 276, 48,12), false);

      pad_Canon.create(48, 20, NegroAlpha);
      pad_Canon.copy(*image, 0,0, IntRect(296, 308, 48,20), false);

      pad_Small.create(32, 12, NegroAlpha);
      pad_Small.copy(*image, 0,0, IntRect(352, 308, 32,12), false);

      pad_Mini.create(20, 8);
      pad_Mini.copy(*image, 0,0, IntRect(496, 292, 20,8), false);

      pad_Big.create(56, 12, NegroAlpha);
      pad_Big.copy(*image, 0,0, IntRect(292, 356, 56,12), false);

      //. PAD, Duplicados
      Duplex_Normal.create(96,12, NegroAlpha);
      Duplex_Normal.copy(*image, 0,0, IntRect(248,276,96,12), false);

      Duplex_Small.create(64,12, NegroAlpha);
      Duplex_Small.copy(*image, 0,0, IntRect(352,308,64,12), false);

      Duplex_Mini.create(40, 8, NegroAlpha);
      Duplex_Mini.copy(*image, 0,0, IntRect(496,292,40,8), false);

      Duplex_Big.create(112,12, NegroAlpha);
      Duplex_Big.copy(*image, 0,0, IntRect(292,356,112,12), false);

      exp_Normal = new ANIMATION(image, IntRect(296,292,192,12), 1,4);
      exp_Normal->setElapsed(0.75);
      exp_Canon = new ANIMATION(image, IntRect(296,332,192,20), 1,4);
      exp_Canon->setElapsed(0.75);
      exp_Small = new ANIMATION(image, IntRect(352,320, 128,12), 1,4);
      exp_Small->setElapsed(0.75);
      exp_Mini = new ANIMATION(image, IntRect(496,304,80,8), 1,4);
      exp_Mini->setElapsed(0.75);
      exp_Big = new ANIMATION(image, IntRect(392,372,216,12), 1,4);
      exp_Big->setElapsed(0.75);

      bullet.create(6,14, Color(0,0,0, 0));
      bullet.copy(*image, 0,0, IntRect(425,272, 6,14), true);
   }

   void ChangeImage(TIPO tipo){
      this->tipo = tipo;

      Texture *mmm = new Texture;

      switch(tipo){
      case TIPO::Normal:
         mmm->loadFromImage(pad_Normal);
         break;
      case TIPO::Canon:
         mmm->loadFromImage(pad_Canon);
         break;
      case TIPO::Small:
         mmm->loadFromImage(pad_Small);
         break;
      case TIPO::Mini:
         mmm->loadFromImage(pad_Mini);
         break;
      case TIPO::Big:
         mmm->loadFromImage(pad_Big);
         break;

      case TIPO::DuplexNormal:
         inflated = false;
         mmm->loadFromImage(Duplex_Normal);
         break;
      case TIPO::DuplexSmall:
         inflated = false;
         mmm->loadFromImage(Duplex_Small);
         break;
      case TIPO::DuplexMini:
         inflated = false;
         mmm->loadFromImage(Duplex_Mini);
         break;
      case TIPO::DuplexBig:
         inflated = false;
         mmm->loadFromImage(Duplex_Big);
         break;
      }

      rcPlayer.setSize( Vector2f(mmm->getSize().x, mmm->getSize().y));
      rcPlayer.setOrigin(mmm->getSize().x / 2.0f, mmm->getSize().y / 2.0f);
      rcPlayer.setOutlineThickness( 1 );
      rcPlayer.setFillColor(Color(255,255,255, 255));
      rcPlayer.setTexture(mmm, true);

      if(inflated){
         SetInflate( true );
      }
   }

   TIPO get_idImage() { return tipo; }

   void SetInflate( bool inflated ) {
      this->inflated = inflated;

      Texture *mmm = new Texture();
      if(tipo == TIPO::Canon){
         mmm->loadFromImage(pad_Normal);
      } else {
         mmm = (Texture *) rcPlayer.getTexture();
      }

      float width  = mmm->getSize().x / 2.0f;
      float height = mmm->getSize().y / 2.0f;

      paddle_0 = new Sprite();
      paddle_0->setTexture(*mmm);
      paddle_0->setOrigin(width, height);
      paddle_0->setPosition(px, py);

      paddle_1 = new Sprite();
      paddle_1->setTexture(*mmm);
      paddle_1->setOrigin(width, height);
      paddle_1->setPosition(px, py);

      paddle_2 = new Sprite();
      paddle_2->setTexture(*mmm);
      paddle_2->setOrigin(width, height);
      paddle_2->setPosition(px, py);

   }

   void ReStart() {
      px = 250;
      exploted = false;
      finished = false;
      rcPlayer.setPosition(px, py);
      ChangeImage(TIPO::Normal);

      exp_Normal->IndexReset( 0 );
      exp_Canon->IndexReset( 0 );
      exp_Small->IndexReset( 0 );
      exp_Mini->IndexReset( 0 );
      exp_Big->IndexReset( 0 );
   }

   bool isCaptured()  { return capturar; }
   void setCaptura(bool state) { this->capturar = state; }

   int  get_Lifes() { return lifes; }
   void add_Lifes(int pp = 1) { lifes += pp; }

   int  get_Score()  { return score; }
   void add_Score(int pp = 1) { score += pp;}

protected:
   bool              capturar = false;

   bool              inflated = false;
   bool              exploted = false;
   bool              finished = false;
   TIPO              tipo = TIPO::Normal;

   Image             pad_Normal;
   Image             pad_Canon;
   Image             pad_Small;
   Image             pad_Mini;
   Image             pad_Big;

   Image             Duplex_Normal;
   Image             Duplex_Small;
   Image             Duplex_Mini;
   Image             Duplex_Big;

   Image             bullet;
   float             STEPS = 10.0f;
   float             steps = 0.0f;
   float             diffx = 0.0f;

   ANIMATION         *exp_Normal;
   ANIMATION         *exp_Canon;
   ANIMATION         *exp_Small;
   ANIMATION         *exp_Mini;
   ANIMATION         *exp_Big;

private:
   int   score =  0;
   int   lifes = 20;

   float ax, bx, cx;
   Sprite *paddle_0 = NULL;
   Sprite *paddle_1 = NULL;
   Sprite *paddle_2 = NULL;

};


struct PUERTA {
   PUERTA() {}

   PUERTA(float px, float py, float width, float height, \
          Color myColor, Color myRelleno = (Color)0) {

     rcDoor = Create_Rectangle(px, py, width, height, myColor, myRelleno);
     rcDoor.setOrigin(width / 2.0f, height / 2.0f);
     area = Rect<float>(px -(width/2.0f), py -(height/2.0f), width, height);
     this->px = px;
     this->py = py;
   }

   void SetAnimation(ANIMATION *anim, ANIMATION *rayo){
      this->anim = anim;
      this->anim->setPosition(px, py);
      this->anim->setModo(ANIMATION::MODO::Stop, 0);

      this->rayo = rayo;
      this->rayo->setPosition(px, py);
      this->rayo->setModo(ANIMATION::MODO::Stop, 0);
   }

   void Display(RenderWindow *win){
      //win->draw(rcDoor);

      switch(modo){
      case MODO::Cerrando:
         if(anim->Display(win)){
            modo = MODO::Cerrada;
            anim->setModo(ANIMATION::MODO::Stop, 0);
            rayo->setModo(ANIMATION::MODO::Stop);
         }
         break;

      case MODO::Abierta:
         rayo->Display(win);
         break;

      case MODO::Abriendo:
         if(anim->Display(win)){
            modo = MODO::Abierta;
            anim->setModo(ANIMATION::MODO::Stop);
            rayo->setModo(ANIMATION::MODO::Normal);
         }
         break;

      default:
         if(anim->Display(win)){
            modo = MODO::Abriendo;
            anim->setModo(ANIMATION::MODO::Normal);
         }
      }
   }

   enum MODO { Cerrada, Abriendo, Abierta, Cerrando };
   void setModo(MODO modo){
      this->modo = modo;
      if(modo == MODO::Abriendo){
         anim->setModo(ANIMATION::MODO::Normal);
      }
      if(modo == MODO::Cerrando){
         anim->setModo(ANIMATION::MODO::Back, 7);
      }
   }

   MODO getModo() { return modo; }

   RectangleShape getRectangle() { return rcDoor; }


protected:
   MODO  modo = MODO::Cerrada;
   float px   = 0;
   float py   = 0;
   Rect<float>     area;
   ANIMATION       *anim;
   ANIMATION       *rayo;
   RectangleShape  rcDoor;

};



#endif // _PLAYER_H
