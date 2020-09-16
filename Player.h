#ifndef _PLAYER_H
#define _PLAYER_H

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
