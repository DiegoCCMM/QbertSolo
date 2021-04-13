//
// Created by diego on 13/3/21.
//

enum State{
    GOING_DOWN, AI, COULD_FALL, FALLING
};

#ifndef ALLEGRO5TUTORIAL_COILY_HPP
#define ALLEGRO5TUTORIAL_COILY_HPP
class Coily : public Enemy{

    State state = GOING_DOWN;
    ALLEGRO_BITMAP* secondPhaseDraw;

public:
    Coily(const Piramide &piramide, const std::string &nom, int i, int j, int xRespectCube, int yRespectCube) : Enemy(
            piramide, nom, i, j, xRespectCube, yRespectCube) {
        ALLEGRO_BITMAP *draw = al_load_bitmap("../sprites/coilyEstirado.png");
        must_init(draw, nom.c_str());
        secondPhaseDraw = draw;
    }

    void setState(State s){
        state = s;
    }

    void AIMovement(){
        //TODO HACER LA IA
    }

    void randomMovement(){
        if(state == GOING_DOWN){
            Enemy::randomMovement();
        }else{
            Coily::AIMovement();
        }
    }

    ALLEGRO_BITMAP* getDraw() const override {
        if(state == GOING_DOWN){
            return(draw);
        }else{
            return(secondPhaseDraw);
        }
    }
};
#endif //ALLEGRO5TUTORIAL_COILY_HPP
