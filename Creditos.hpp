//
// Created by patri on 23/04/2021.
//

#ifndef QBERT_CREDITOS_HPP
#define QBERT_CREDITOS_HPP

#include <fstream>
#include <iostream>
#include <list>

class Creditos{
    const int numRegistros = 12; // personas como maximo
    struct Persona{
        std::string nombre;
        int score;
    };

    enum Pantalla {
        REGNOM, HIGHSCORES
    };

    float width, height;
    int timer,
        tiempo; // tiempo para el registro de nombre, 30seg predeterminado
    bool finish;

    Pantalla pant;
    Persona jugador;

    std::list<Persona> personas;
    ALLEGRO_BITMAP *letrasDraw = al_load_bitmap("../sprites/fonts.png");
    ALLEGRO_BITMAP *letrasGrandesDraw = al_load_bitmap("../sprites/youdidit.png");
    ALLEGRO_BITMAP *qbertDraw = al_load_bitmap("../sprites/qbert.png");
    ALLEGRO_BITMAP *patriDraw = al_load_bitmap("../sprites/patri.png");
    ALLEGRO_SAMPLE *teclaSound = al_load_sample("../sounds/qbert-jump.ogg");
    ALLEGRO_SAMPLE *highscoreSound = al_load_sample("../sounds/highscores.ogg");

public:

    Creditos(){
        std::ifstream file("../highscores.txt");
        for(int i=0; !file.eof(); i++) {
            Persona persona;
            std::string nom, punt;
            file >> nom >> punt;
            if(punt != "") { // para cuando el fichero esta vacio
                std::cout << punt << std::endl;
                persona.nombre = nom, persona.score = stoi(punt);
                personas.push_back(persona);
            }
        }
        file.close();
        personas.sort(compare);
    }

    void loadPantalla(int puntuacion, float _width, float _height) {
        width = _width, height = _height;
        finish = false;
        timer = 0, tiempo = 30;
        if(personas.size()==numRegistros){
            if(puntuacion > personas.crbegin()->score){
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
            if(timer%30 == 0) {
                tiempo--;
                if(tiempo == 0) {
                    pant = HIGHSCORES;
                    timer = 0;
                }
            }
        } else {
            if(timer == 0) al_play_sample(highscoreSound, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
            if(timer > 180) finish = true;
        }
        timer++;
    }

    void write(int code){
        if(pant == REGNOM){
            if(code >= 1 && code <= 26 && jugador.nombre.length()<15){ // letras de la A a la Z
                jugador.nombre += (1, char(code+64));
                al_play_sample(teclaSound, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
            } else if(code >= 27 && code <= 36 && jugador.nombre.length()<15){ // numeros del 0 al 9
                jugador.nombre += (1, char(code+21));
                al_play_sample(teclaSound, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
            } else if(code == 63 && jugador.nombre.length()>0){ // delete
                jugador.nombre.pop_back();
            } else if(code == 67){ // ENTER
                if(jugador.nombre.empty()){
                    jugador.nombre = "EMPTY";
                }
                personas.push_back(jugador);
                personas.sort(compare);

                // Meter todos los jugadores de vuelta en el fichero
                std::ofstream file("../highscores.txt");
                int i = 0;
                for (std::_List_iterator<Persona> it = personas.begin(); it != personas.end(); it++) {
                    file << it->nombre << " " << it->score;
                    if(i<personas.size()-1) file << "\n";
                    i++;
                }
                file.close();

                timer = 0;
                pant = HIGHSCORES;
            }
        }
    }

    void drawBitmap() {
        float x, y;
        std::string frase;

        if(pant == REGNOM){

            x = width/2 - 100, y = height/2 - 120;
            al_draw_bitmap_region(letrasGrandesDraw, 0, 0,
                                          27*8, 3*8, x, y, 0);

            x = width/2 - 80, y = height/2 - 80;
            frase = "YOU HAVE USURPED ALL\\OTHERS TO BECOME THE\\\\";
            for (std::string::size_type i = 0; i < frase.size(); i++) {
                if (int(frase[i]) == 92) { // '\' --> \n
                    x = width/2 - 80-8;
                    y += 9;
                } else if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                    al_draw_bitmap_region(letrasDraw, (int(frase[i]) % 65) * 8, 7*8,
                                          8, 8, x, y, 0);
                }
                x += 8;
            }

            x = width/2 - 56;
            frase = "SUPREME NOSER";
            for (std::string::size_type i = 0; i < frase.size(); i++) {
                if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                    al_draw_bitmap_region(letrasDraw, (int(frase[i]) % 65) * 8, 7*8,
                                          8, 8, x, y, 0);
                }
                al_draw_bitmap_region(letrasDraw, 13 * 8, 5*8,
                                      8, 8, x, y+8, 0);
                x += 8;
            }

            x = width/2 - 104, y += 9*3;
            frase = "KINDLY ENTER YOUR INITIALS";
            for (std::string::size_type i = 0; i < frase.size(); i++) {
                if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                    al_draw_bitmap_region(letrasDraw, (int(frase[i]) % 65) * 8, 7*8,
                                          8, 8, x, y, 0);
                }
                x += 8;
            }

            x = width/2 - 56, y += 9;
            frase = "FOR ALL TO SEE";
            for (std::string::size_type i = 0; i < frase.size(); i++) {
                if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                    al_draw_bitmap_region(letrasDraw, (int(frase[i]) % 65) * 8, 7*8,
                                          8, 8, x, y, 0);
                }
                x += 8;
            }

            // Parpadear subrayado
            x = width/2 - jugador.nombre.size()*8/2-4, y = height/2+16;
            for (std::string::size_type i = 0; i < jugador.nombre.size(); i++) {
                if (int(jugador.nombre[i]) >= 48 && int(jugador.nombre[i]) <= 57) { // Numero
                    al_draw_bitmap_region(letrasDraw, (int(jugador.nombre[i]) % 48) * 8 + 9*8, 2*8,
                                          8, 8, x, y, 0);
                } else if (int(jugador.nombre[i]) != 32) { // En caso de ser un espacio no se dibuja
                    al_draw_bitmap_region(letrasDraw, (int(jugador.nombre[i]) % 65) * 8, 3*8,
                                          8, 8, x, y, 0);
                }
                x += 8;
            }
            if(timer%50 >= 0 && timer%50 <= 20){
                y = height/2+20;
                al_draw_bitmap_region(letrasDraw, 13 * 8, 5*8,
                                      8, 8, x, y, 0);
            }

            // TIME
            x = width/2 - 100, y = height/2+100;
            frase = "TIME ";
            for (std::string::size_type i = 0; i < frase.size(); i++) {
                if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                    al_draw_bitmap_region(letrasDraw, (int(frase[i]) % 65) * 8, 7*8,
                                          8, 8, x, y, 0);
                }
                x += 8;
            }

            // tiempo
            frase = std::to_string(tiempo);
            for (std::string::size_type i = 0; i < frase.size(); i++) {
                al_draw_bitmap_region(letrasDraw, (int(frase[i]) % 48) * 8, 0,
                                          8, 8, x, y, 0);
                x += 8;
            }

            x = width/2 - 100, y = height/2+20;
            al_draw_bitmap_region(patriDraw, 0, 0,
                                  27*8, 10*8, x, y, 0);

            //TODO: poner imagen Diego
        }
        else {

            x = width/2 - 74, y = height/2 - 122;
            // QBert izq
            al_draw_bitmap_region(qbertDraw, 6 * 16, 0,
                                  16, 16, x, y, 0);
            // QBert der
            x = width/2 + 56;
            al_draw_bitmap_region(qbertDraw, 4 * 16, 0,
                                  16, 16, x, y, 0);

            x = width/2 - 44, y = height/2 - 120;
            frase = "HIGH SCORES";
            for (std::string::size_type i = 0; i < frase.size(); i++) {
                if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                    al_draw_bitmap_region(letrasDraw, (int(frase[i]) % 65) * 8, 7*8,
                                          8, 8, x, y, 0);
                }
                al_draw_bitmap_region(letrasDraw, 13 * 8, 5*8,
                                      8, 8, x, y+8, 0);
                x += 8;
            }

            // nombre ganador 1º
            frase = personas.begin()->nombre;
            x = width/2 - frase.size()*5, y = height/2 - 90;
            for (std::string::size_type i = 0; i < frase.size(); i++) {
                if (int(frase[i]) >= 48 && int(frase[i]) <= 57) { // Numero
                    al_draw_bitmap_region(letrasDraw, (int(frase[i]) % 48) * 8 + 10*8, 0,
                                          8, 8, x, y, 0);
                } else if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                    al_draw_bitmap_region(letrasDraw, (int(frase[i]) % 65) * 8, 8,
                                          8, 8, x, y, 0);
                }
                x += 8;
            }

            // puntos ganador 1º
            if(timer%50 >= 0 && timer%50 <= 20) {
                x += 10;
                frase = std::to_string(personas.begin()->score);
                for (std::string::size_type i = 0; i < frase.size(); i++) {
                    al_draw_bitmap_region(letrasDraw, (int(frase[i]) % 48) * 8 + 10 * 8, 2 * 8,
                                          8, 8, x, y, 0);
                    x += 8;
                }
            }

            // OTROS JUGADORES (numRegistros)
            x = width/2 - 100, y = height/2 - 60;
            int j = 2;
            for (std::_List_iterator<Persona> it = ++personas.begin(); it != personas.end(); it++, j++) {
                    frase = std::to_string(j) + ")" + it->nombre + " " + std::to_string(it->score);
                    for (std::string::size_type i = 0; i < frase.size(); i++) {
                        if (int(frase[i]) == 41) {  // ')'
                            al_draw_bitmap_region(letrasDraw, 23 * 8, 5 * 8,
                                                  8, 8, x, y, 0);
                        } else if (int(frase[i]) >= 48 && int(frase[i]) <= 57) { // Numero
                            al_draw_bitmap_region(letrasDraw, (int(frase[i]) % 48) * 8, 0,
                                                  8, 8, x, y, 0);
                        } else if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                            al_draw_bitmap_region(letrasDraw, (int(frase[i]) % 65) * 8, 7 * 8,
                                                  8, 8, x, y, 0);
                        }
                        x += 8;
                    }
                    x = width / 2 - 100;
                    y += 9 * 2;
            }
        }
    }

    void resize(float _width, float _height){
        width = _width;
        height = _height;
    }

    void destroy() {
        al_destroy_bitmap(letrasDraw);
        al_destroy_bitmap(letrasGrandesDraw);
        al_destroy_bitmap(qbertDraw);
        al_destroy_bitmap(patriDraw);
        al_destroy_sample(teclaSound);
        al_destroy_sample(highscoreSound);
    }

    // Comparison for sort
    static bool compare (const Persona& first, const Persona& second) {
        return first.score > second.score;
    }


    /*************************
     * GETTER'S AND SETTER'S *
     *************************/

    bool isFinish() const { return finish; }
    void setFinish(bool finish) { Creditos::finish = finish; }

};

#endif //QBERT_CREDITOS_HPP
