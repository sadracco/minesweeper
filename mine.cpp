#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <string>
#include <math.h>

#include "Cell.cpp"

const short CELL_XN = 18;
const short CELL_YN = 18;
const short CELL_SIZE = 30;
const short WIDTH = CELL_XN * CELL_SIZE;
const short HEIGHT = CELL_YN * CELL_SIZE + 2*CELL_SIZE;

const short COLORS[8][3] = {
    {0, 97, 255},
    {0, 170, 0},
    {204, 0, 0},
    {4, 0, 142},
    {130, 0, 0},
    {49, 142, 135},
    {0, 0, 0},
    {100,100,100}
};

void discover(Cell (*c)[CELL_XN], short i, short j){
    if(j<0 || j>=CELL_XN || i<0 || i>=CELL_YN)
        return;

    if(c[i][j].visited || c[i][j].getState() == 1 || c[i][j].getState() == 2 || c[i][j].getState() == 3 || c[i][j].getState() == 4)
        return;

    if(c[i][j].getNeigh() != 0){
        c[i][j].visited = true;
        c[i][j].setState(2);
        return;
    }
    else{
        c[i][j].visited = true;
        c[i][j].setState(2);
        discover(c, i-1, j);
        discover(c, i+1, j);
        discover(c, i, j-1);
        discover(c, i, j+1);
        discover(c, i-1, j-1);
        discover(c, i+1, j-1);
        discover(c, i-1, j+1);
        discover(c, i+1, j+1);
    }
}

void markBombs(Cell (*c)[CELL_XN]){
    for(short i=0; i<CELL_YN; i++)
        for(short j=0; j<CELL_XN; j++){
            if(c[i][j].getState() == 3)
                c[i][j].setState(7);
            else if(c[i][j].getState() == 4)
                c[i][j].setState(6);
            else if(c[i][j].getState() == 1)
                c[i][j].setState(5);
        }
}

short placeBombs(Cell (*c)[CELL_XN]){
    short bombsNum = 0;
    for(short i=0; i<CELL_YN; i++)
        for(short j=0; j<CELL_XN; j++){
            c[i][j].setPos(j*CELL_SIZE, i*CELL_SIZE);
            if(i%2 != j%2)
                c[i][j].setAlpha(255);
            else
                c[i][j].setAlpha(200);

            if(!(rand() % 8) && bombsNum < 99){
                c[i][j].setState(1);
                bombsNum++;
            }
            else{
                c[i][j].setState(0);
                c[i][j].visited = false;
            }
        }
    return bombsNum;
}

void setNumbers(Cell (*c)[CELL_XN]){
    short neighOff[8][2] = {{-1,-1}, {0,-1}, {1,-1}, {-1,0}, {+1,0}, {-1,+1}, {0,+1}, {+1,+1}};
    for(short i=0; i<CELL_YN; i++)
        for(short j=0; j<CELL_XN; j++){
            short count = 0;
            for(short o=0; o<8; o++){
                short x = neighOff[o][0] + j;
                short y = neighOff[o][1] + i;
                if((0 <= y) && (0 <= x) && (x < CELL_XN) && (y < CELL_YN))
                    if(c[y][x].getState() == 1)
                        count++;
            }
            c[i][j].setNearBombs(count);
        }
}

int main(){
    using namespace sf;
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Minesweeper");
    window.setFramerateLimit(60);
    Cell cells[CELL_YN][CELL_XN];
    short bombsNum = 0;
    short accurateFlags = 0; 
    short flags = 0; 
    short faceState = 0;
    bool play = true;

    Texture flagTex;
    flagTex.loadFromFile("flagTex.png");
    Sprite flagSprite;
    flagSprite.setTexture(flagTex);

    Texture replayTex;
    replayTex.loadFromFile("replay.png");
    Sprite replaySprite;
    replaySprite.setTexture(replayTex);
    replaySprite.setPosition(WIDTH-50, HEIGHT - CELL_SIZE - 17);

    Texture smileTex;
    smileTex.loadFromFile("smileTex.png");
    Sprite smileSprite;
    smileSprite.setTexture(smileTex);
    smileSprite.setPosition(25, HEIGHT - CELL_SIZE - 17);

    Texture glassTex;
    glassTex.loadFromFile("glassTex.png");
    Sprite glassSprite;
    glassSprite.setTexture(glassTex);
    glassSprite.setPosition(25, HEIGHT - CELL_SIZE - 17);

    Texture wooTex;
    wooTex.loadFromFile("wooTex.png");
    Sprite wooSprite;
    wooSprite.setTexture(wooTex);
    wooSprite.setPosition(25, HEIGHT - CELL_SIZE - 17);

    Texture shineTex;
    shineTex.loadFromFile("shineTex.png");
    Sprite shineSprite;
    shineSprite.setTexture(shineTex);
    shineSprite.setPosition(25, HEIGHT - CELL_SIZE - 17);

    Texture mineTex;
    mineTex.loadFromFile("mineTex.png");
    Sprite mineSprite;
    mineSprite.setTexture(mineTex);

    Font f;
    f.loadFromFile("font.ttf");

    RectangleShape menu;
    menu.setSize(Vector2f(WIDTH, CELL_SIZE*2));
    menu.setPosition(Vector2f(0, HEIGHT - 2*CELL_SIZE));
    menu.setFillColor(Color(60, 0, 60));
    
    Text number;
    number.setFont(f);
    number.setCharacterSize(20);
    number.setFillColor(Color::Black);

    Text flagsCap;
    flagsCap.setFont(f);
    flagsCap.setCharacterSize(25);
    flagsCap.setFillColor(Color::White);
    flagsCap.setPosition(WIDTH/2 - 35, HEIGHT - CELL_SIZE - 12);

    srand(time(NULL));

    Event event;

    bombsNum = placeBombs(cells);

    flagsCap.setString(std::to_string(flags) + "/" + std::to_string(bombsNum));

    setNumbers(cells);
    
    while(window.isOpen()){
        while(window.pollEvent(event)){
            switch(event.type){
                case Event::Closed:
                    window.close();
                    return 0;
                    break;

                case Event::MouseButtonPressed:
                    if(event.mouseButton.button == Mouse::Left){
                        short mouseX = Mouse::getPosition(window).x;
                        short mouseY = Mouse::getPosition(window).y;
                        if((0 <= mouseX && mouseX < WIDTH) && (0 <= mouseY && mouseY < HEIGHT - 2 * CELL_SIZE)){
                            if(play)
                                faceState = 1;
                        }
                    }
                    break;
                case Event::MouseButtonReleased:
                    if(event.mouseButton.button == Mouse::Left){
                        short mouseX = Mouse::getPosition(window).x;
                        short mouseY = Mouse::getPosition(window).y;
                        if((0 <= mouseX && mouseX < WIDTH) && (0 <= mouseY && mouseY < HEIGHT)){
                            if((WIDTH-50 <= mouseX && mouseX < WIDTH-20) && (HEIGHT-CELL_SIZE-17 <= mouseY && mouseY < HEIGHT-17)){
                                flags = 0;
                                accurateFlags = 0;
                                play = true;
                                bombsNum = placeBombs(cells);
                                setNumbers(cells);
                                faceState = 0;
                                flagsCap.setString(std::to_string(flags) + "/" + std::to_string(bombsNum));
                                continue;
                            }
                            else if(play){
                                faceState = 0;
                                short x = floor(mouseX/CELL_SIZE);
                                short y = floor(mouseY/CELL_SIZE);
                                if(cells[y][x].getState() == 4){
                                    faceState = 3;
                                    play = false;
                                    markBombs(cells);
                                }

                                else if(cells[y][x].getState() == 3){
                                    flags--;
                                    discover(cells, y, x);
                                    cells[y][x].setState(2);
                                    flagsCap.setString(std::to_string(flags) + "/" + std::to_string(bombsNum));
                                }
                                else if(cells[y][x].getState() == 0){
                                    discover(cells, y, x);
                                    cells[y][x].setState(2);
                                }

                                else if(cells[y][x].getState() == 1){
                                    faceState = 3;
                                    play = false;
                                    markBombs(cells);
                                }
                            }
                        }
                    }
                    else if(event.mouseButton.button == Mouse::Right && play){
                        short mouseX = Mouse::getPosition(window).x;
                        short mouseY = Mouse::getPosition(window).y;
                        if((0 <= mouseX && mouseX < WIDTH) && (0 <= mouseY && mouseY < HEIGHT)){
                            short x = floor(mouseX/CELL_SIZE);
                            short y = floor(mouseY/CELL_SIZE);
                            if(cells[y][x].getState() == 1){
                                if(flags < bombsNum){
                                    cells[y][x].setState(4);
                                    accurateFlags++;
                                    flags++;
                                    if(accurateFlags == bombsNum && flags == bombsNum){
                                        faceState = 2;
                                        markBombs(cells);
                                    }
                                    flagsCap.setString(std::to_string(flags) + "/" + std::to_string(bombsNum));
                                }
                            }
                            else if(cells[y][x].getState() == 0){
                                if(flags < bombsNum){
                                    cells[y][x].setState(3);
                                    flags++;
                                    flagsCap.setString(std::to_string(flags) + "/" + std::to_string(bombsNum));
                                }
                            }
                            else if(cells[y][x].getState() == 3){
                                cells[y][x].setState(0);
                                flags--;
                                flagsCap.setString(std::to_string(flags) + "/" + std::to_string(bombsNum));
                            }
                            else if(cells[y][x].getState() == 4){
                                cells[y][x].setState(1);
                                accurateFlags--;
                                flags--;
                                flagsCap.setString(std::to_string(flags) + "/" + std::to_string(bombsNum));
                            }
                        }
                    }
                    break;
            }
        }

        for(short i=0; i<CELL_YN; i++){
            for(short j=0; j<CELL_XN; j++){
                cells[i][j].draw(window);

                if(cells[i][j].getState() == 2 && cells[i][j].getNeigh() != 0){
                    number.setFillColor(Color(COLORS[cells[i][j].getNeigh()-1][0], COLORS[cells[i][j].getNeigh()-1][1], COLORS[cells[i][j].getNeigh()-1][2]));
                    number.setString(std::to_string(cells[i][j].getNeigh()));
                    number.setPosition(cells[i][j].x+8, cells[i][j].y+3);
                    window.draw(number);
                }
                else if(cells[i][j].getState() == 3 || cells[i][j].getState() == 4){
                    flagSprite.setPosition(cells[i][j].x, cells[i][j].y);
                    window.draw(flagSprite);
                }
                else if(cells[i][j].getState() == 5 || cells[i][j].getState() == 6){
                    mineSprite.setPosition(cells[i][j].x, cells[i][j].y);
                    window.draw(mineSprite);
                }
            }
        }
        window.draw(menu);
        window.draw(replaySprite);
        switch(faceState){
            case 0:
                window.draw(smileSprite);
                break;
            case 1:
                window.draw(wooSprite);
                break;
            case 2:
                window.draw(glassSprite);
                break;
            case 3:
                window.draw(shineSprite);
                break;
        }
        window.draw(flagsCap);

        window.display();
        window.clear();
    }
}
