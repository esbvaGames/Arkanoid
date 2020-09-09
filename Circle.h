#ifndef _CIRCLE_H
#define _CIRCLE_H

struct CIRCLE {
    Font           font;
    Text           prompt;
    CircleShape rcCircle;
    //. Constructor que es una formalidad
    CIRCLE() {}

    CIRCLE(float px, float py, Color myColor = (Color)NULL){
        rcCircle.setPosition(px, py);
        rcCircle.setRadius(15);
        rcCircle.setFillColor(colorRojo_alpha);
        rcCircle.setOutlineColor(colorRojo);
        rcCircle.setOutlineThickness(2);
        this->px = px;
        this->py = py;
        //. La mitad despues del centro de circulo.
        area = Rect<float>(px -4, py -4, 32, 32);
    }

    void Update(RenderWindow *win){
       float mx = Mouse::getPosition(*win).x;
       float my = Mouse::getPosition(*win).y;

       if(area.contains(Vector2f(mx, my))){
          if(state != STATE::Pressed){
             state = STATE::Select;
          }

       } else {
          if(state != STATE::Pressed){
             state = STATE::Normal;
          }
       }
    }

    bool isSelected() {
       return ((state == STATE::Select) ? true : false);
    }
    void Set_Pressed(){  state = STATE::Pressed;  }
    void Set_Normal() {  state = STATE::Normal;   }

    void Display(RenderWindow *win){
        switch(state){
        case STATE::Select:
           rcCircle.setFillColor(colorSelect_alpha);
           rcCircle.setOutlineColor(colorSelect);
           break;

        case STATE::Pressed:
           rcCircle.setFillColor(colorPressed_alpha);
           rcCircle.setOutlineColor(colorPressed);
           break;

        default:
           if(state != STATE::Pressed){
              if(ModoEditar){
                 rcCircle.setFillColor(colorBorde_alpha);
                 rcCircle.setOutlineColor(colorBorde);
              } else {
                 rcCircle.setFillColor(colorRojo_alpha);
                 rcCircle.setOutlineColor(colorRojo);
              }
           }
        }

        win->draw(rcCircle);
        win->draw(prompt);
    }

    void setTexto(string texto, Font font, float Scale = 12){
        this->font = font ;            //. Guarda una copia de la fuente
        prompt = Text(texto, this->font, Scale);
        float bx = (32 - prompt.getLocalBounds().width) / 2.5f;
        float by = (32 - prompt.getLocalBounds().height) / 2.5f;
        prompt.setPosition(px +bx, py +by);
        prompt.setFillColor(colorYellow);
    }
private:
    enum          STATE {Disabled, Normal, Select, Pressed};
    STATE         state = STATE::Normal;
    Rect<float>   area;
    float px, py;

};




#endif // _CIRCLE_H
