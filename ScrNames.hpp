#ifndef SCR_NAMES_HPP
#define SCR_NAMES_HPP

using namespace sf;

class NameBox {
   public:
      enum STATE {Normal, Select, Pressed };

      NameBox() {}

      NameBox(float px, float py, float width, float height, \
              Color myColor, Color myRelleno, Font *font){
         rcDialog = Create_Rectangle(px, py, width, height, myColor, myRelleno);
         rcDialog.setOrigin(width / 2.0f, height / 2.0f);
         rcDialog.setPosition(px, py);

         tittle = Create_Label(px, py, "Nombre Jugador", font, 20);
         tittle.setOrigin(tittle.getLocalBounds().width / 2.0f, \
                          tittle.getLocalBounds().height / 2.0f);
         tittle.setPosition(px, py -40);


         rcEditor = Create_Rectangle(px, py, width -10, 26, myColor, colorBorde_alpha);
         rcEditor.setOrigin((width -10)/2.0f, 13);
         rcEditor.setPosition(px, py-6);

         textbox = Create_Label(px -90, py-15, "Sin-Nombre", font, 18);


         btnOkey   = new BUTTON(px-42, py+20, 32, 20, "Si", font, 16);
         btnCancel = new BUTTON(px+20, py+20, 32, 20, "No", font, 16);
      }

      bool isVisible() { return visible; }
      void setVisible(bool state) { visible = state; }

      void SetString(string buffer){
         this->buffer = buffer;
         textbox.setString(buffer);

         modo = STATE::Normal;
         btnOkey->SetState(BUTTON::STATE::Normal);
         btnCancel->SetState(BUTTON::STATE::Normal);
         Okey = false;
         finish = false;
      }
      string GetString() { return buffer; }


      bool isMouseInEditor(RenderWindow *win){
         int mx = Mouse::getPosition(*win).x;
         int my = Mouse::getPosition(*win).y;

         Rect<float> myEditor = \
                     Rect<float>(rcEditor.getPosition(). x - rcEditor.getOrigin().x, \
                                 rcEditor.getPosition(). y - rcEditor.getOrigin().y, \
                                 rcEditor.getSize().x, \
                                 rcEditor.getSize().y);

         if(myEditor.contains(mx, my)){
            return true;
         }
         return false;
      }

      void Display(RenderWindow *win){
         win->draw(rcDialog);
         win->draw(tittle);

         switch(modo){
         case STATE::Pressed:
            rcEditor.setOutlineColor(colorRojo);
            rcEditor.setFillColor(colorRojo_alpha);
            break;
         case STATE::Select:
            rcEditor.setOutlineColor(colorSelect);
            rcEditor.setFillColor(colorSelect_alpha);
            break;
         default:
            rcEditor.setOutlineColor(colorBorde);
            rcEditor.setFillColor(colorBorde_alpha);
         }
         win->draw(rcEditor);
         win->draw(textbox);

         btnOkey->Display(win);
         btnCancel->Display(win);
      }

      //. Edita el Nombre cuando el NameBox fue Presionado.-
      void Update(RenderWindow *win, Event *evn){
         if(isMouseInEditor(win)){
           if(Mouse::isButtonPressed(Mouse::Left)){
              if(modo != STATE::Pressed){
                 modo = STATE::Pressed;
              } else {
                 modo = STATE::Select;
              }
           } else {
              if(modo != STATE::Pressed){
                 modo = STATE::Select;
              }
           }
         } else {
           if(modo != STATE::Pressed){
              modo = STATE::Normal;
           }
         }

         if(modo == STATE::Pressed){
           if(evn->type == Event::TextEntered){
              if(Keyboard::isKeyPressed(Keyboard::Enter)){
                 modo = STATE::Normal;
              } else {
                 if(Keyboard::isKeyPressed(Keyboard::Backspace)){
                    if(buffer.length() >= 1){
                      buffer = buffer.erase(buffer.length()-1, 1);
                    }
                 } else {
                   if(evn->text.unicode < 128 && buffer.length() < 16 ){
                     buffer += evn->text.unicode;
                   }
                 }
                 textbox.setString(buffer);
              }
           }
         }

         btnOkey->Update(win);
         if(btnOkey->isPressed()){
            Okey = true;
            finish = true;
         }
         btnCancel->Update(win);
         if(btnCancel->isPressed()){
            Okey = false;
            finish = true;
         }
      }

      bool isOkey()   { return Okey; }
      bool isFinish() { return finish; }


   protected:

   private:
      bool                 Okey   = false;
      bool                 finish = false;
      STATE                modo = STATE::Normal;
      bool                 visible = false;
      string               buffer  = "Sin-Nombre";
      Text                 tittle;
      Text                 textbox;
      RectangleShape       rcDialog;
      RectangleShape       rcEditor;
      BUTTON               *btnOkey;
      BUTTON               *btnCancel;
};


#endif // NAMEBOX_HPP
