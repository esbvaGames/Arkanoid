#ifndef _ENTIDAD_H
#define _ENTIDAD_H

using namespace sf;

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

   //. Para preguntar si el Efecto, Enemygo se ha destruido
   bool isFinished()  { return finished; }

   //. Para marcarlo que ha sido destruido
   void SetFinished( bool state ){
      finished = state;
   }

   RectangleShape GetCollider(){
      shape.setFillColor((Color)0);
      shape.setOutlineColor(colorSelect);
      shape.setOutlineThickness(1);

      shape.setPosition(px, py);
      shape.setOrigin(anim->getWidth()/2.0f, anim->getHeight()/2.0f);
      shape.setSize(Vector2f(anim->getWidth(), anim->getHeight()));

      return shape;
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

   GRUPO          idGrupo;      //. tipo de Entidad
   float          px, py;       //. Posicion
   float          vx, vy;       //. Velocidad vx,vy;
   Rect<float>    area;         //. Area de Colision
   ANIMATION      *anim;        //. Animacion
   callback       *command;     //. Funcion del Bonus
   bool finished = false;       //. Enemigo ha sido Destruido
   RectangleShape  shape;       //. Entrega el Colisionador de la Entidad
};

struct BONUS : public ENTIDAD {
   BONUS() {};

   BONUS(float px, float py, ANIMATION *anim){
      SetAnimation(anim);
      SetPosition(px, py);
   }

   void Update(){
      if( !StartGame ) { return; }
      SetPosition(px +vx, py+vy);
   }

   void Display(RenderWindow *win){
      anim->Display(win);
   }

   void set_BonusTipo( int idTipo){
      this->idTipo = idTipo;
   }

   int get_BonusTipo() { return idTipo; }

protected:
   int   idTipo;      //. Identificador del Tipo de Bonus.
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
      win->draw( GetCollider() );
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
      if( !StartGame ){ return; }
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

   void set_EnemyTipo(int idTipo){
      this->idTipo = idTipo;
   }
   int get_EnemyTipo(){ return idTipo; }

   //. Si el Enemigo es golpeado por la Bola
   bool isGolpeado(RectangleShape rcBola){
      retardo -= 1;            //. Cuando se crea, siempre esta colisionando
      if(retardo <= 0.0){      //. Este retardador le da tiempo a moverse.-
         if(isCollision(rcBola)){
            golpes -= 1; retardo = 8;
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

protected:
   int idTipo    =  0;               //. Identificador del Tipo de Enemigo.
   int golpes    =  0;               //. Golpes de resistencia del Enemigo
   int retardo   = 30;               //. Retardo del detector de colisiones


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
      if( !StartGame ){ return;  }
      anim->setPosition(px +vx, py +vy);
   }

   void Display(RenderWindow *win){
      finished = anim->Display(win);   //. Retornado desde la Animacion
   }

protected:
   //. El efecto ahora usara el finalizador Generico de la clase Base.

};


#endif // _ENTIDAD_H
