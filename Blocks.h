#ifndef __BLOCKS_H_
#define __BLOCKS_H_

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
       this->idColor = idColor;
       rcBlock.setFillColor(myColor);
    }

    int get_idColor(){ return idColor; }

protected:
    int  idColor = 0;
    bool Activo  = true;
    RectangleShape rcBlock;
};





#endif // __BLOCKS_H_
