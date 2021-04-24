//
// Created by patri on 23/04/2021.
//

#ifndef QBERT_CREDITOS_HPP
#define QBERT_CREDITOS_HPP

#include <fstream>
#include <iostream>
#include <list>

class Creditos{
    struct Persona{
        std::string nombre;
        int score;
    };

    enum Pantalla {
        REGNOM, HIGHSCORES
    };

    float width, height;
    int timer = 0;
    bool finish;

    Pantalla pant;
    Persona jugador;

    std::list<Persona> personas; // 23 personas
    ALLEGRO_BITMAP *drawInit = al_load_bitmap("../sprites/fonts.png");
    ALLEGRO_SAMPLE *teclaSound = al_load_sample("../sounds/qbert-hello.ogg");

public:

    Creditos(){
        std::ifstream file("../highscores.txt");
        for(int i=0; !file.eof(); i++) {
            Persona persona;
            std::string nom, punt;
            file >> nom >> punt;
            if(punt != "") {
                persona.nombre = nom, persona.score = stoi(punt);
                personas.push_back(persona);
            }
        }
        file.close();
    }

    void loadPantalla(int puntuacion) {
        finish = false;
        if(personas.size()==23){
            if(puntuacion > std::end(personas)->score){
                personas.pop_back(); // elimina el ultimo
                jugador.score = puntuacion;
                jugador.nombre = "";

                pant = REGNOM;
            } else {
                pant = HIGHSCORES;
            }
        } else { // lista vacia
            jugador.score = puntuacion;
            jugador.nombre = "";
            pant = REGNOM;
        }
    }

    void movement() {
        if(pant == REGNOM){

        } else {

        }
    }

    void write(int code){
        if(pant == REGNOM){
            if(code >= 1 && code <= 26){ // letras de la A a la Z
                jugador.nombre += (1, char(code+64));
            } else if(code >= 27 && code <= 36){ // numeros del 0 al 9
                jugador.nombre += (1, char(code+21));
            } else if(code == 63 && jugador.nombre.length()>0){ // delete
                jugador.nombre.pop_back();
            } else if(code == 67){ // ENTER
                personas.push_back(jugador);
                //personas.sort();

                // Meter todos los jugadores de vuelta en el fichero
                std::ofstream file("../highscores.txt");
                int i = 0;
                for (std::_List_iterator<Persona> it = personas.begin(); it != personas.end(); it++) {
                    file << it->nombre << " " << it->score;
                    if(i<personas.size()-1) file << "\n";
                    i++;
                }
                file.close();

                pant = HIGHSCORES;
            }
            //std::cout << jugador.nombre << std::endl;
        }
    }

    void drawBitmap() {
        if(pant == REGNOM){

        } else {

        }
    }

    void resize(float _width, float _height){
        width = _width;
        height = _height;

        if(pant == REGNOM){
            //platillo.setX(width/2+platillo.getXRespectCube());
            //platillo.setY(height/2+platillo.getYRespectCube());
        } else { // HIGHSCORE

        }

    }

    void destroy() {
        if(pant == REGNOM){

        } else {

        }
    }


    /*************************
     * GETTER'S AND SETTER'S *
     *************************/

    bool isFinish() const { return finish; }
    void setFinish(bool finish) { Creditos::finish = finish; }

};

#endif //QBERT_CREDITOS_HPP
