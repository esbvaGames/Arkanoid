#ifndef SCR_SCORES_HPP
#define SCR_SCORES_HPP

using namespace sf;

#define fillChar(c,n)   setfill(c) << setw(n)

struct RECORD {
   int      index   = 0;
   int      Nivel   = 0;
   int      Score   = 0;
   long     SECONDS = 0;
   string   tiempo  ="";

   RECORD() {}

   RECORD(const RECORD *Other){
      *this = *Other;
   }

   RECORD(int Nivel, int Score, long Segs = 0){
      this->Nivel  = Nivel;
      this->Score  = Score;
      this->SECONDS = Segs;
   }

   string getRecord(int ndx = 0){
      ostringstream buffer;
      switch(ndx){
      case 1:   buffer << fillZero(2) << index; break;
      case 2:   buffer << fillZero(2) << Nivel; break;
      case 3:   buffer << fillZero(5) << Score; break;
      case 4:   buffer << calcTimer(SECONDS); break;
      }
      return buffer.str();
   }

   string calcTimer(long TotalSegs){
      if(TotalSegs == 0){
         return string("00:00:00");
      }

      ostringstream buffer;

      int horas    = TotalSegs / 3600;
      int minutos  = (TotalSegs / 60) % 60;
      int segundos = TotalSegs % 60;

      buffer << fillZero(2) << horas   << ":" \
             << fillZero(2) << minutos << ":" \
             << fillZero(2) << segundos;

      return buffer.str();
   }
};

bool Compare(pair<string, RECORD> a, pair<string, RECORD> b){
 if(a.second.Score > b.second.Score){
    return true;
 }
 return false;
}


#ifdef _MAIN_
  map<string, RECORD> JUGADOR;
  vector<pair<string, RECORD> > REGISTER;

#else
  extern map<string, RECORD> > JUGADOR;
  extern vector<pair<string, RECORD> > REGISTER;

#endif // _MAIN_


class ScrScore
{
   public:
      ScrScore() {};
      virtual ~ScrScore() {};

      ScrScore(float px, float py, float width, float height, \
             Color myColor, Color myRelleno, Font *font) {

         this->font = *font;

         rcDialog = Create_Rectangle(px, py, width, height, myColor, myRelleno);
         rcDialog.setOrigin(width / 2.0f, height / 2.0f);

         rcSelect = Create_Rectangle(px, py, width-4, 15, colorRojo, Color(255,90,90, 128));
         rcSelect.setOrigin((width-4)/2.0f, 16/2.55f);

         Titulo = Create_Texto(px, py -136,"MEJORES PUNTAJES", font, 20);
         SubTit = Create_Texto(px, py -116,\
         "Nro   Nombre             Puntaje    Nivel    Tiempo", font, 16);

         Pausa  = Create_Texto(px, py +134, "Pausado (5) seg.", font, 15);
         SetPausa();

         this->px = px;
         this->py = py;

         if(!LoadScores()) {
            Player_Nuevo("Jeannise", RECORD(rand()%40, rand()%50000));
            Player_Nuevo("Naomi"   , RECORD(rand()%40, rand()%50000));
            Player_Nuevo("Kevin"   , RECORD(rand()%40, rand()%50000));
            Player_Nuevo("Samuel"  , RECORD(rand()%40, rand()%50000));
            Player_Nuevo("Camila"  , RECORD(rand()%40, rand()%50000));
            Player_Nuevo("Gabriela", RECORD(rand()%40, rand()%50000));
            Player_Nuevo("Nayarett", RECORD(rand()%40, rand()%50000));
            Player_Nuevo("Miriam"  , RECORD(rand()%40, rand()%50000));
            Player_Nuevo("Raquel"  , RECORD(rand()%40, rand()%50000));
            Player_Nuevo("Kathlyn" , RECORD(rand()%40, rand()%50000));
            Player_Nuevo("Felipe"  , RECORD(rand()%40, rand()%50000));
            Player_Nuevo("Yaiden"  , RECORD(rand()%40, rand()%50000));
            Player_Nuevo("Damian"  , RECORD(rand()%40, rand()%50000));
         }
         SaveScores();
      }

      void SetActiva(bool state){ Activa = state; }
      bool isActiva() { return Activa; }

      void SetKeyName(string KeyName){
         this->KEYNAME = KeyName;
      }
      string GetKeyName() { return KEYNAME; }

      bool inRegister(string KeyName){
         auto it = JUGADOR.find(KeyName);
         if(it == JUGADOR.end()){
            return false;
         }
         return true;
      }


      bool Player_Nuevo(string KeyName, RECORD reg){
         if(!inRegister(KeyName)) {
            JUGADOR.insert(make_pair(KeyName, RECORD(&reg)));
            return true;
         }
         return false;
      }

      void Player_Update(string KeyName, RECORD reg){
         auto it = JUGADOR.find(KeyName);
         if(it == JUGADOR.end() ){
            cout << KeyName << ": No Existe" << endl;
            return;
         }
         time(&finish);
         reg.SECONDS = difftime(finish, start);
         JUGADOR.at(KeyName) = RECORD(&reg);
      }

      void Clock_Reiniciar(){
         time(&start);
      }

      void Record_Orden() {
         for(auto it = JUGADOR.begin(); it != JUGADOR.end(); ++it){
            REGISTER.push_back(make_pair(it->first, it->second));
         }
         sort(REGISTER.begin(), REGISTER.end(), Compare);

         int index = 0;
         for(auto it = REGISTER.begin(); it != REGISTER.end(); ++it){
            RECORD *reg = (RECORD*)&it->second;
            reg->index = index;
            index++;
            int len = ((string)it->first).size();

            cout << it->first   << fillChar(' ', 10 -len) << " " << \
                    fillZero(2) << reg->getRecord(1) << " " << \
                    fillZero(2) << reg->getRecord(2) << " " << \
                    fillZero(5) << reg->getRecord(3) << " " << \
                    reg->getRecord(4) << endl;
         }
      }

      void Display(RenderWindow *win) {
          win->draw(rcDialog);
          win->draw(Titulo);
          win->draw(SubTit);

          int index = 0;
          int TOP   =96;

          for(auto it = REGISTER.begin(); it != REGISTER.end(); ++it){
             //cout << ((RECORD)*it).getRecord() << endl;
             if(index > 13){ continue; }

             string data1 = ((RECORD)it->second).getRecord(1);
             string data2 = (string)it->first;
             string data3 = ((RECORD)it->second).getRecord(2);
             string data4 = ((RECORD)it->second).getRecord(3);
             string data5 = ((RECORD)it->second).getRecord(4);

             if(data2 == KEYNAME){
                rcSelect.setPosition(px, (py+(index*16)-TOP+2));
                win->draw(rcSelect);
             }

             Text Line1 = Create_Texto(px -160, (py+(index*16)) -TOP, data1, &font, 14, Aling::Right);
             win->draw(Line1);
             Text Line2 = Create_Texto(px -135, (py+(index*16)) -TOP, data2, &font, 14, Aling::left);
             win->draw(Line2);
             Text Line3 = Create_Texto(px +040, (py+(index*16)) -TOP, data4, &font, 14, Aling::Right);
             win->draw(Line3);
             Text Line4 = Create_Texto(px + 90, (py+(index*16)) -TOP, data3, &font, 14, Aling::Right);
             win->draw(Line4);
             Text Line5 = Create_Texto(px +180, (py+(index*16)) -TOP, data5, &font, 14, Aling::Right);
             win->draw(Line5);

             index++;
          }


          if(!isTimeLapse() ){
            Pausa.setString( msg_Lapso() );
            win->draw(Pausa);
          } else {
            SetActiva(false);
          }
      }
      bool LoadScores(){
         ifstream   myFile;

         string linea;
         string KeyName;

         myFile.open(fileName);

         if(myFile.is_open()){
            myFile >> linea;
            cout << linea << endl;

            do {
               RECORD reg;

               myFile >> KeyName;
               if(KeyName.empty()) { break; }

               myFile >> reg.Score;
               myFile >> reg.Nivel;
               myFile >> reg.SECONDS;

               if(Player_Nuevo(KeyName, RECORD(&reg))) {
                  int len = KeyName.size();
                  cout << reg.getRecord(1) << " " \
                       << KeyName << fillChar(' ', 10 -len) << " " \
                       << reg.getRecord(2) << " " \
                       << reg.getRecord(3) << " " \
                       << reg.getRecord(4) << endl;
               }
            } while( !myFile.eof() );

            myFile.close();
            return true;
         }
         return false;
      }

      bool SaveScores(){
         ofstream  myFile;

         myFile.open(fileName);

         if(myFile.is_open()){
            myFile << "PUNTAJES:" << endl;
            for(auto it = JUGADOR.begin(); it != JUGADOR.end(); ++it){
               RECORD data = it->second;
               int len = ((string)it->first).size();
               myFile << it->first    << fillChar(' ',10 -len) << " " \
                      << fillZero(5)  << data.Score  << " " \
                      << fillZero(2)  << data.Nivel  << " " \
                      << data.SECONDS << endl;
            }
            myFile.close();
         }
         return false;
      }
   protected:
      enum Aling { left, Center, Right };

      Text Create_Texto(float px, float py, string texto, Font *font, \
                        float Scale = 30, Aling aling = Aling::Center){
         Text txt;
         txt = Text(texto, *font, Scale);
         txt.setPosition(px, py);
         txt.setFillColor(colorYellow);
         float centroX = txt.getLocalBounds().width / 2.0f;
         float centroY = txt.getLocalBounds().height / 2.0f;

         if(aling == Aling::Right){
            txt.setOrigin(txt.getLocalBounds().width, centroY);
         }
         if(aling == Aling::left){
            txt.setOrigin(0, centroY);
         }
         if(aling == Aling::Center){
            txt.setOrigin(centroX, centroY);
         }
         return txt;
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
         buffer << "Pausado (" << ceil(lapso) << ") seg";
         return buffer.str();
      }

   private:
      time_t            start, finish;

      string            KEYNAME = "Kevin";
      Time              delta;
      float             lapso;
      float             PAUSA = 5.0f;
      string            fileName = "./GameScore.txt";

      float             px, py;
      bool              Activa = false;
      Font              font;
      Text              Titulo;
      Text              SubTit;
      Text              Pausa;
      RectangleShape    rcDialog;
      RectangleShape    rcSelect;
};

#endif // SCREEN_H
