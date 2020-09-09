
#include "globals.h"
#include "blocks.h"
#include "levels.h"


Color data_getColors(int idColor){
   return data[idColor].colors;
}

int data_getGolpes(int idColor){
   return data[idColor].Golpes;
}

void copyToArray(int level, BLOCK **block, int TOTAL){
  int filas = 0;
  int colms = 0;

  for(int index=0; index < TOTAL; index++){
     colms = index % MAX_COLMS;
     filas = index / MAX_COLMS;
     //cout << "filas(" << filas << " colms(" << colms << ") " << block[index]->get_idColor() << endl;
     ARRAY_LEVEL[level][filas][colms] = block[(TOTAL -1) -index]->get_idColor();
  }
}

bool SaveToTexto(int FILAS){
   ofstream mySave;

   int level =  0;
   int filas =  0;

   mySave.open(fileSave);

   if(mySave.is_open()){
      mySave << "GameLevel {" << endl;
      for(int level=0; level < MAX_LEVEL; level++){
         mySave << "  Level: " << fillZero(2) << level << " filas: " << fillZero(2) << FILAS << endl;
         for(int filas=0; filas < MAX_FILAS; filas++){
            mySave << "    " << fillZero(2) << filas << " { ";
            for(int colms=0; colms < MAX_COLMS; colms++){
               mySave << ARRAY_LEVEL[level][filas][colms] << " ";
               cout << "level(" << level << ") filas(" << filas << ") colms(" << colms << ")" << endl;
            }
            mySave << " }" << endl;
         }
      }
      mySave << "}";
      mySave.close();
   }
}

bool loadFromFile(){

   fstream myLoad;
   string  buffer;

   int myLevel = 0;
   int myFilas = 0;
   int filas   = 0;
   int colms   = 0;
   int idBlock = 0;

   myLoad.open(fileSave);

   if(myLoad.is_open()){
      while( !myLoad.eof() ){
         myLoad >> buffer;
         if(buffer == "Level:"){
            myLoad >> myLevel;
            continue;
         }
         if(buffer == "filas:"){
            myLoad >> myFilas;
            cout << "Level(" << myLevel << ") filas(" << myFilas << ")" << endl;
            for(filas = 0; filas < MAX_FILAS; filas++){
               myLoad >> filas;
               myLoad >> buffer;   //. La llave {
               for(colms = 0; colms < MAX_COLMS; colms++){
                  myLoad >> idBlock;
                  ARRAY_LEVEL[myLevel][filas][colms] = idBlock;
                  cout << idBlock <<", ";
               }
               myLoad >> buffer;  //. la llave }
               cout << endl;
            }
         }
      }
      myLoad.close();
   }
}

void CopyFromArray(int level, BLOCK **block, int TOTAL){

   int filas = 0;
   int colms = 0;
   int index = TOTAL -1;
   int idColor = 0;

   for(filas=0; filas < MAX_FILAS; filas++){
      for(colms=0; colms < MAX_COLMS; colms++){
         idColor = ARRAY_LEVEL[level][filas][colms];
         block[index]->set_idColor(idColor, data_getColors(idColor));
         index--;
      }
   }
}
