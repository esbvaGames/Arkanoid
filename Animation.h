#ifndef _ANIMATION_H_
#define _ANIMATION_H_

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
         //cout << "region:(" << fillZero(3) << region.left <<","<<
         //                      fillZero(3) << region.top <<")" << endl;

         //. Para tratar la posicion desde el Centro de la Animacion;
         sprites[index]->setOrigin(width / 2.0f, height / 2.0f);
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
      if(modo != MODO::Stop) {   //. Para animacion detenida
         if(modo == MODO::Back){
           finished = UpdateBack();
         } else {
           finished = UpdatePlay();
         }
      }
      sprites[index]->setPosition(px, py);
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

   enum MODO { Normal, Back, Stop};
   void setModo(MODO modo, int index = 0){
      this->modo = modo;
      this->index = index;
   }

   void IndexReset(int index){
      this->index = index;
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





#endif // _ANIMATION_H_
