#ifndef __BLOCKS_H_
#define __BLOCKS_H_

#define MAX_LEVEL  50
#define MAX_FILAS  12    //. Niveles mas altos tienen mas filas
#define MAX_COLMS  13
#define NUM_COLORS 11

using namespace sf;

struct DATA {
   DATA() {};
   DATA(int indice, Color colors){
      this->indice = indice;
      this->colors = colors;
      this->Golpes = Golpes;
   }

   void SetImagen(Image *fondo, IntRect area, int Scores, int Golpes){
       imagen.create(32,16, Color(0,0,0, 0));
       imagen.copy(*fondo, 0,0, area, true);
       this->Scores = Scores;
       this->Golpes = Golpes;
   }

   Texture *GetImagen(){
       Texture *mmm = new Texture();
       mmm->loadFromImage(imagen);
       return mmm;
   }

   Color    colors;     //. Color de este blocque
   int      Golpes;     //. Golpes que puede recibir
   int      Scores;     //. Puntaje entregado

protected:
   int      indice;     //. indice

private:
    Image   imagen;      //. Textura del bloque.
};



#ifdef _MAIN_

int ARRAY_LEVEL[MAX_LEVEL][MAX_FILAS][MAX_COLMS] = {};
DATA data[NUM_COLORS] = {};

#else
   extern int ARRAY_LEVEL[MAX_LEVEL][MAX_FILAS][MAX_COLMS];
   extern DATA data[NUM_COLORS];

#endif // _MAIN_


struct BLOCK {
    //. Constructor que no hace nada, es una formalidad
    BLOCK() {};

    //. Si no se especifica el color de relleno, el relleno sera NULL.-
    BLOCK(float px, float py, Color myRelleno = (Color)0 ){
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
       this->Golpes  = 0;
       this->Scores  = 0;
       this->idColor = idColor;
       rcBlock.setFillColor(myColor);
       if(idColor != 0){
         Scores = data[idColor].Scores;
         Golpes = data[idColor].Golpes;
         rcBlock.setFillColor( Color(255,255,255, 255));
         rcBlock.setTexture( data[idColor].GetImagen() );
       }
    }

    int get_idColor(){ return idColor; }
    int get_Scores() { return Scores; }
    int get_Golpes() { Golpes -= 1; return Golpes; }

protected:
    int  Scores  = 0;
    int  Golpes  = 0;
    int  idColor = 0;
    bool Activo  = true;
    RectangleShape rcBlock;
};

#endif // __BLOCKS_H_
