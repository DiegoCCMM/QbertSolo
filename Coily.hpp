//
// Created by diego on 13/3/21.
//

enum State{
    GOING_DOWN, AI, COULD_FALL, FALLING
};

#ifndef ALLEGRO5TUTORIAL_COILY_HPP
#define ALLEGRO5TUTORIAL_COILY_HPP
class Coily : public Enemy{
    State state;
public:

    void changeState(){
        //TODO CHANGE FROM THE DIFFERENT STATES

        //GOING DOWN
        //sprite in the ball move
        //executing the randomMovement

        //REACHED DOWN, STREACHING INTO AI MODE
        //sprite in snake mode
        //moving in AiMovement


    }
};
#endif //ALLEGRO5TUTORIAL_COILY_HPP
