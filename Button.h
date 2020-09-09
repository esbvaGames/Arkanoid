#ifndef _BUTTON_H
#define _BUTTON_H

struct BUTTON {
   BUTTON() {};
   BUTTON(float px, float py, float width, float height, \
          string texto, Font *font, float Scale){
      rcButton = Create_Rectangle(px, py, width, height, colorBorde, colorBorde_alpha);
      label    = Create_Label(0,0, texto, &*font, Scale);
      float bx = (width - label.getLocalBounds().width ) / 2.0f;
      float by = (height - label.getLocalBounds().height) / 3.5f;
      label.setPosition(px + bx, py + by);
      label.setFillColor(colorBorde);
      area = Rect<float>(px, py, width, height);
   }
   void Update(RenderWindow *win){
      float mx = Mouse::getPosition(*win).x;
      float my = Mouse::getPosition(*win).y;
      if(area.contains(Vector2f(mx, my))){
         state = STATE::Select;
         if(Mouse::isButtonPressed(Mouse::Left)){
            state = STATE::Pressed;
         }
      } else {
         state = STATE::Normal;
      }
   }

   void Display(RenderWindow *win){
      switch(state){
      case STATE::Select:
         rcButton.setFillColor(colorSelect_alpha);
         rcButton.setOutlineColor(colorSelect);
         label.setFillColor(colorSelect);
         break;

      case STATE::Pressed:
         rcButton.setFillColor(colorPressed_alpha);
         rcButton.setOutlineColor(colorPressed);
         label.setFillColor(colorPressed);
         break;

      default:
         rcButton.setFillColor(colorBorde_alpha);
         rcButton.setOutlineColor(colorBorde);
         label.setFillColor(colorBorde);
      }
      win->draw(rcButton);
      win->draw(label);
   }

   bool isPressed(){
      return (state == STATE::Pressed) ? true : false;
   }

protected:
   enum  STATE {Normal, Select, Pressed};
   STATE state = STATE::Normal;
   Text             label;
   Rect<float>      area;
   RectangleShape   rcButton;
};


#endif // _BUTTON_H
