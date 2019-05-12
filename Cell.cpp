#include <SFML/Graphics.hpp>
#include <string>

extern const short CELL_XN;
extern const short CELL_YN;
extern const short CELL_SIZE;
extern const short WIDTH;
extern const short HEIGHT;

class Cell{
    private:
        short state = 0;
        short alpha;
        short nearBombs;
        sf::RectangleShape shape;
        sf::Color color;

    public:
        bool visited = false;
        short x;
        short y;

        Cell(){
            shape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            state = 0;
        }

        void draw(sf::RenderWindow &window){
            window.draw(shape);
        }

        void setPos(short xx, short yy){
            x = xx;
            y = yy;
            shape.setPosition(x, y);
        }
        
        void setAlpha(short al){
            alpha = al;
            color = sf::Color(216, 62, 239, alpha);
            shape.setFillColor(color);
        }

        void setCol(short r, short g, short b){
            color = sf::Color(r, g, b, alpha);
            shape.setFillColor(color);
        }

        void setState(short s){
            state = s;
            switch(state){
                case 0: //hidden
                    color = sf::Color(216, 62, 239, alpha);
                    shape.setFillColor(color);
                    break;
                case 1: //hidden mine
                    //color = sf::Color(255, 0, 0, alpha);
                    //shape.setFillColor(color);
                    break;
                case 2: //clear
                    color = sf::Color(251, 226, 255, alpha);
                    shape.setFillColor(color);
                    break;
                case 3: //flagged
                    break;
                case 4: //hidden mine flagged
                    break;
                case 5: //mine
                    color = sf::Color(251, 226, 255, alpha);
                    shape.setFillColor(color);
                    break;
                case 6: //mine accurate flagged
                    color = sf::Color(0, 255, 0, alpha);
                    shape.setFillColor(color);
                    break;
                case 7: //mine not accurate flagged
                    color = sf::Color(255, 0, 0, alpha);
                    shape.setFillColor(color);
                    break;
            }
        }

        void setNearBombs(short n){
            nearBombs = n;
        }

        short getState(){
            return state;
        }

        short getNeigh(){
            return nearBombs;
        }
};
