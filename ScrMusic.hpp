#ifndef SRCMUSIC_HPP
#define SRCMUSIC_HPP


using namespace sf;


class ScrMusic {
   public:

      ScrMusic(){
         THEMAS.push_back("./assets/jazz-01.ogg");
         THEMAS.push_back("./assets/jazz-02.ogg");
         THEMAS.push_back("./assets/jazz-03.ogg");
         THEMAS.push_back("./assets/jazz-04.ogg");
      }

      bool getThema(){
         int index = rand() % THEMAS.size();

         if( !thema.openFromFile(THEMAS.at(index))){
            cout << "Error leyendo Thema: " << THEMAS.at(index) << endl;
            return false;
         }
         Time duracion = thema.getDuration();
         cout << "Thema: " << THEMAS.at(index) << " => " << duracion.asSeconds() << endl;
         return true;
      }

      void play(){
         if( !isPlaying() ){
            if(getThema()){
               thema.play();
            }
         }
      }

      void stop(){
         if( isPlaying() ){
            thema.stop();
         }
      }


      bool isPlaying() {
         return ((thema.getStatus() == SoundStream::Status::Playing) ? true : false);
      }

   private:
      vector<string> THEMAS;
      Music          thema;



};



#endif // SRCMUSIC_HPP_INCLUDED
