#ifndef SCRFINAL_HPP
#define SCRFINAL_HPP

using namespace sf;

struct ScrFinal {

     ScrFinal() {}

     ScrFinal(float px, float py, float width, float height, \
              Color myColor, Color myRelleno, Font *font){

        rcFondo = Create_Rectangle(px, py, width, height, myColor, myRelleno);
        rcFondo.setOrigin(width / 2.0f, height / 2.0f);
        rcFondo.setPosition(px, py);

        tittle = Create_Label(px, py, "JUEGO TERMINADO", font, 20);
        tittle.setOrigin(calcOrigen(tittle));
        tittle.setPosition(px, py-40);

        texto = Create_Label(px, py, "¡¡¡ Felicitaciones !!!", font, 16);
        texto.setOrigin(calcOrigen(texto));
        texto.setPosition(px, py);

        Pausa = Create_Label(px, py, "Reinciando el Juego en (5) seg.", font, 16);
        Pausa.setOrigin( calcOrigen(Pausa));
        Pausa.setPosition(px, py+36);

        SetPausa(3.0f);
     }

     void SetPausa(float pausa = 5.0f){
        this->PAUSA = pausa;
        delta = seconds(0.01f);
        lapso = delta.asSeconds() + PAUSA;
     }

     bool isTimeLapse(){
        lapso -= delta.asSeconds();
        if(lapso < 0.0f){
           lapso = delta.asSeconds() + PAUSA;
           return true;
        }
        return false;
     }

     string msg_Lapso(){
        ostringstream buffer;
        buffer << "Reiniciando el Juego en (" << ceil(lapso) << ") seg";
        return buffer.str();
     }

     bool isVisible() { return visible; }
     void setVisible(bool state) {visible = state; }

     void Display(RenderWindow *win){
        win->draw(rcFondo);
        win->draw(tittle);
        win->draw(texto);

        if( !isTimeLapse() ){
           Pausa.setString( msg_Lapso() );
           win->draw(Pausa);
        } else {
           visible = false;
        }
     }

protected:
   Vector2f calcOrigen(Text txt){
      return Vector2f(txt.getLocalBounds().width/2.0f, \
                      txt.getLocalBounds().height/2.0f);
   }


private:
   bool                 visible = false;
   Time                 delta;
   float                lapso;
   float                PAUSA;

   RectangleShape       rcFondo;
   Text                 tittle;
   Text                 texto;
   Text                 Pausa;



};


#endif // SCRFINAL_HPP
