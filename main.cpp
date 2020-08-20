
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <iostream>

bool Running = false;

using namespace std;

int main()
{
    sf::RenderWindow win(sf::VideoMode(800,480), "SFML Arkanoid by esbva");

    while(win.isOpen()){

      sf::Event  evn;                   //. Receptor de Eventos
      while( win.pollEvent(evn) ) {
         if(evn.type == sf::Event::Closed){
            win.close();
         }

      }



      win.clear(sf::Color(0,0,255));    //. Pinta la Window de Color Azul

      win.display();

    }




    cout << "Hello world!" << endl;
    return 0;
}
