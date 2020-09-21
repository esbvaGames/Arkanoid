#ifndef _PLAYER_H
#define _PLAYER_H

using namespace sf;

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
   enum TIPO {Normal, Canon, Small, Mini, Big};

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
      //. Configuracion de teclado Solo Pruebas

      if(Keyboard::isKeyPressed(Keyboard::Num1)){
         ChangeImage(TIPO::Normal);
      }
      if(Keyboard::isKeyPressed(Keyboard::Num2)){
         ChangeImage(TIPO::Canon);
      }
      if(Keyboard::isKeyPressed(Keyboard::Num3)){
         ChangeImage(TIPO::Small);
      }
      if(Keyboard::isKeyPressed(Keyboard::Num4)){
         ChangeImage(TIPO::Mini);
      }
      if(Keyboard::isKeyPressed(Keyboard::Num5)){
         ChangeImage(TIPO::Big);
      }
      if(Keyboard::isKeyPressed(Keyboard::Num6)){
         finished = true;
      }
      if(Keyboard::isKeyPressed(Keyboard::Num7)){
         SetInflate();
         inflated = true;
      }


      exp_Normal->setPosition(px, py);
      exp_Canon->setPosition(px, py);
      exp_Small->setPosition(px, py);
      exp_Mini->setPosition(px, py);
      exp_Big->setPosition(px, py);
   }

   void Display(RenderWindow *win){
      if(finished){
         if(exploted){
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

         } else {
             switch(tipo){
             case TIPO::Normal:
               exploted = exp_Normal->Display(win);
               break;
             case TIPO::Canon:
               exploted = exp_Canon->Display(win);
               break;
             case TIPO::Small:
               exploted = exp_Small->Display(win);
               break;
             case TIPO::Mini:
               exploted = exp_Mini->Display(win);
               break;
             case TIPO::Big:
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
         return true;
      }

      if(inflated){
         if(myOther.intersects(GetCollider( paddle_0)) || \
            myOther.intersects(GetCollider( paddle_1)) || \
            myOther.intersects(GetCollider( paddle_1))) {

            return true;
         }
      }


      return false;
   }

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

      pad_Small.create(32, 12);
      pad_Small.copy(*image, 0,0, IntRect(352, 308, 32,12), false);

      pad_Mini.create(20, 8);
      pad_Mini.copy(*image, 0,0, IntRect(496, 292, 20,8), false);

      pad_Big.create(56, 12, NegroAlpha);
      pad_Big.copy(*image, 0,0, IntRect(292, 356, 56,12), false);

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
      }

      rcPlayer.setSize( Vector2f(mmm->getSize().x, mmm->getSize().y));
      rcPlayer.setOrigin(mmm->getSize().x / 2.0f, mmm->getSize().y / 2.0f);
      rcPlayer.setOutlineThickness( 1 );
      rcPlayer.setFillColor(Color(255,255,255, 255));
      rcPlayer.setTexture(mmm, true);
   }

   void SetInflate() {
      Texture *mmm = (Texture*) rcPlayer.getTexture();

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




protected:
   bool              inflated = false;
   bool              exploted = false;
   bool              finished = false;
   TIPO              tipo = TIPO::Normal;

   Image             pad_Normal;
   Image             pad_Canon;
   Image             pad_Small;
   Image             pad_Mini;
   Image             pad_Big;

   ANIMATION         *exp_Normal;
   ANIMATION         *exp_Canon;
   ANIMATION         *exp_Small;
   ANIMATION         *exp_Mini;
   ANIMATION         *exp_Big;

private:
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
