//The headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include "Malvada.h"
#include "Freeze.h"
#include "Explosive.h"
#include "Burbuja.h"
#include "Ship.h"
#include <list>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;


//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;
const int FRAMES_PER_SECOND = 60;

int scores;


//The surfaces
SDL_Surface *background = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *play = NULL;
SDL_Surface *info = NULL;
SDL_Surface *exito = NULL;
SDL_Surface *pts = NULL;
SDL_Surface *escores = NULL;

//The event structure
SDL_Event event;

//The font
TTF_Font *font = NULL;
TTF_Font *fuente = NULL;
TTF_Font *big = NULL;

Mix_Chunk* navig = NULL;
Mix_Chunk* enter = NULL;
Mix_Chunk* music = NULL;
Mix_Chunk* intro = NULL;
Mix_Chunk* danger = NULL;
Mix_Chunk* clicked = NULL;
Mix_Chunk* freezed = NULL;
Mix_Chunk* nitro = NULL;


//The color of the font
SDL_Color textColor = { 255, 255, 255 };


class Timer
{
private:
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;

public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    int get_ticks();

    //Checks the status of the timer
    bool is_started();
    bool is_paused();
};



SDL_Surface *load_image( std::string filename )
{

    return IMG_Load(filename.c_str());
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        return false;
    }

    //Iniciando SDL_Mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "BubbleInvasion v1.0", NULL );

    //If everything initialized fine
    return true;
}

bool load_files()
{
    //Load the background image
    background = load_image( "background.png" );

    navig = Mix_LoadWAV("navig.wav");
    enter = Mix_LoadWAV("enter.wav");
    danger = Mix_LoadWAV("hited.wav");
    clicked = Mix_LoadWAV("clicked.wav");
    freezed = Mix_LoadWAV("freezing.wav");
    nitro = Mix_LoadWAV("explode.wav");

    //Open the font
    font = TTF_OpenFont( "lazy.ttf", 72 );
    fuente = TTF_OpenFont("nano.ttf" , 42);
    big = TTF_OpenFont("nano.ttf", 72);

    intro = Mix_LoadWAV("Sound/waak.wav");


    //If there was a problem in loading the background
    if( background == NULL )
    {
        return false;
    }



    //If there was an error in loading the font
    if( font == NULL )
    {
        return false;
    }

    //If everything loaded fine
    return true;
}

void clean_up()
{
    //Free the surfaces
    SDL_FreeSurface( background );

    //Close the font
    TTF_CloseFont( font );

    //Quit SDL_ttf
    TTF_Quit();

    Mix_CloseAudio();

    //Quit SDL
    SDL_Quit();
}

class Punto
{
public:
    int x;
    int y;

    Punto(int x, int y)
    {
        this->x = x;
        this->y = y;

    }

    Punto()
    {

    }

};

class Rectangulo
{

public:
    Punto p;
    int w;
    int h;
    Rectangulo(Punto p, int w, int h)
    {
        this->p = p;
        this->w = w;
        this->h = h;

    }

};

bool puntodentrorect(Punto p, Rectangulo r)
{
    if(p.x>=r.p.x && p.x<=r.p.x+r.w && p.y>=r.p.y &&p.y<=r.p.y+r.h)
    {
        return true;
    }
    return false;

}


bool colision(int x1 , int y1 , int w1, int h1,int x2, int y2 , int w2, int h2)
{
    if(puntodentrorect(Punto(x2, y2), Rectangulo(Punto(x1,y1),w1,h1)))
    {
        return true;
    }
    if(puntodentrorect(Punto(x2+w2, y2), Rectangulo(Punto(x1,y1),w1,h1)))
    {
        return true;
    }
    if(puntodentrorect(Punto(x2, y2+h2), Rectangulo(Punto(x1,y1),w1,h1)))
    {
        return true;
    }
    if(puntodentrorect(Punto(x2+w2, y2+h2), Rectangulo(Punto(x1,y1),w1,h1)))
    {
        return true;
    }

    return false;


}

bool wayToSort(int i, int j){


return i>j;


}

void failed(){
    int frame = 0;
    bool cap = true;
    bool quit = false;
    Timer fps;

    SDL_Surface* f = NULL;

    while(quit == false)
    {
        fps.start();
        cap = (!cap);
        f = TTF_RenderText_Solid(big, "GAME OVER", textColor);
        apply_surface(0,0,background, screen);
        apply_surface(120, 200, f, screen);

        while(SDL_PollEvent(&event))
        {

            if(event.type == SDL_QUIT)
            {

                quit = true;
            }
            if(event.type == SDL_KEYDOWN)
            {

                if(event.key.keysym.sym == SDLK_RETURN){


                return;

                }


            }

        }
        if(SDL_Flip(screen) == -1)
        {

            return;
        }
        frame++;
        if( ( cap == true ) && ( fps.get_ticks() < 1000 / FRAMES_PER_SECOND ) )
        {
            //Sleep the remaining frame time
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }


    }



}
void theGame()
{

    bool running = true;

    scores = 0;
    bool quit = false;
    int frames = 0;
    int frame = 0;
    bool cap = true;
    Timer fps;
    int posshipx = 0;
    int posshipy = 0;
    int posbubblex = 0;
    int posbubbley = 0;
    int velocidad = 3;
    int cont = 0;
    int vida = 0;
    SDL_Surface* life[5];
    SDL_Surface* hited[2];
    SDL_Surface* explosion;
    explosion = load_image("explode.png");
    life[0] = load_image("life.png");
    life[1] = load_image("life2.png");
    life[2] = load_image("life3.png");
    life[3] = load_image("life4.png");
    life[4] = load_image("life5.png");
    hited[0] = load_image("hit.png");
    hited[1] = load_image("hit2.png");



    srand(time(NULL));

    list<Burbuja*>burbujas;



    //While the user hasn't quit
    while( quit == false )
    {

        fps.start();
        cap = ( !cap );
        frames++;


//         int px = posshipx;
//         int py = posshipy;
//         int pw = 161;
//         int ph = 166;
//
//         int bx = (*f)->x;
//         int by = (*f)->y;
//         int bw = 64;
//         int bh = 64;

        if(scores < 0){
            scores = 0;
        }


        if(vida == 4){

            if(scores>30){
             vector<int> highs;
                int t;
                ifstream i("scores.txt");
                while(i>>t){
                    highs.push_back(t);

                }
                highs.push_back(scores);
                sort(highs.begin(),highs.end(),wayToSort);


                i.close();

                ofstream o("scores.txt");
                for(int i = 0; i< highs.size() ; i++){
                    o<<highs[i]<<endl;

                }


                }

            failed();
            return;
        }


        if(frames%100 == 0)
        {
            burbujas.push_back(new Burbuja(load_image("square.png"), screen, 0, rand()%300+110));
        }
        if(frames%120 == 0)
        {
            burbujas.push_back(new Malvada(load_image("bad.png"), screen, 0, rand()%300+110));
        }
        if(frames%130 == 0)
        {
            burbujas.push_back(new Freeze(load_image("timer.png"), screen, 0, rand()%300+110));
        }
        if(frames%110 == 0)
        {
            burbujas.push_back(new Explosive(load_image("explosive.png"), screen, 0, rand()%300+110));
        }

//        apply_surface(400, 200, space, screen);

        int clickx =-1;
        int clicky =-1;

//         if(colision(px+pw/2, py+ph/2, pw/4,ph/4, bx+bw/2, by+bh/2, bw/4, bh/4)){
//                    exit(0);
//
//             }

        //While there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {

                quit = true;
            }
        }

        //Apply the background
        apply_surface( 0, 0, background, screen );

        apply_surface(0,0 ,pts, screen);
        apply_surface(0,0 ,life[vida], screen);


        //If a mouse button was pressed
        if( event.type == SDL_MOUSEBUTTONDOWN )   //If the left mouse button was pressed
        {
            if( event.button.button == SDL_BUTTON_LEFT )
            {

                clickx =event.button.x;
                clicky =event.button.y;



            }


        }

        list<Burbuja*>::iterator i= burbujas.begin();

        while(i!= burbujas.end())
        {
//            if((*i)->x == 635){
//                vida++;
//            }

            if((*i)->x > SCREEN_WIDTH ){
                apply_surface(0,0,hited[0], screen);
                vida++;
                Mix_PlayChannel( -1, danger, 0 );
                Mix_VolumeChunk(danger, 60);

                Burbuja*e = (*i);
                i = burbujas.erase(i);
                delete e;


            }else if((*i)->borrar)
            {

                Burbuja*temp = (*i);

                if((*i)->tipo == "Father"){
                        scores++;
                        Mix_PlayChannel( -1, clicked, 0 );
                        Mix_VolumeChunk(clicked, 50);

                        }

                if((*i)->tipo == "Explosive"){
                    apply_surface(0,0,explosion, screen);
                    if((*i)->tipo == "Freeze" ||(*i)->tipo == "Malvada" || (*i)->tipo == "Father"){
                        apply_surface(0,0,explosion, screen);
                    }
                    Mix_PlayChannel( -1, nitro, 0 );
                    Mix_VolumeChunk(nitro, 50);
                    list<Burbuja*>::iterator d = burbujas.begin();
                    while(d!= burbujas.end()){

                            (*d)->borrar = true;
                            d++;
                    }


                }

                if((*i)->tipo == "Freeze"){
                        Mix_PlayChannel( -1, freezed, 0 );
                        Mix_VolumeChunk(freezed, 50);
                        scores++;
                        velocidad = 1;


                }

                if((*i)->tipo == "Malvada"){
                        Mix_PlayChannel( -1, clicked, 0 );
                        Mix_VolumeChunk(clicked, 50);
                        scores--;
                }


                i = burbujas.erase(i);

                delete temp;


            }
            else
            {
                (*i)->render();
                (*i)->logic(clickx, clicky, velocidad);

            }
            i++;

        }

        if(velocidad == 1){

            cont++;
            if(cont == 180){
                velocidad = 3;
                cont = 0;
            }
        }

        std::stringstream escore;
        escore <<"Score: " << scores;
        pts = TTF_RenderText_Solid(fuente, escore.str().c_str(), textColor);


        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return;
        }

        frame++;

        if( ( cap == true ) && ( fps.get_ticks() < 1000 / FRAMES_PER_SECOND ) )
        {

            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }

    }

}

void instructions()
{
    int frame = 0;
    bool cap = true;
    bool quit = false;
    Timer fps;
    SDL_Surface* bg = IMG_Load("instructions.png");


    while(quit == false)
    {
        fps.start();
        cap = (!cap);
        while(SDL_PollEvent(&event))
        {

            if(event.type == SDL_QUIT)
            {

                quit = true;
            }

            if(event.type == SDL_KEYDOWN)
            {

                if(event.key.keysym.sym == SDLK_RETURN){


                return;

                }


            }

        }
        if(SDL_Flip(screen) == -1)
        {

            return;
        }
        frame++;
        if( ( cap == true ) && ( fps.get_ticks() < 1000 / FRAMES_PER_SECOND ) )
        {
            //Sleep the remaining frame time
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }

        apply_surface(0,0,  bg, screen);

    }






}

void HighScores(){

    int frame = 0;
    int i = 0;
    bool cap = true;
    bool quit = false;
    Timer fps;
    SDL_Surface* highs = NULL;
    SDL_Surface* nums1 = NULL;
    SDL_Surface* nums2 = NULL;
    SDL_Surface* nums3 = NULL;


    SDL_Surface* anim[3];
    anim[0] = load_image("rect0.png");
    anim[1]= load_image("rect1.png");
    anim[2] = load_image("rect2.png");

    ifstream getting("scores.txt");

    if(getting == NULL){
        return;
    }
    int higher1;
    int higher2;
    int higher3;
    getting >> higher1 >> higher2 >> higher3;
    if(higher3 == NULL || higher1 == NULL || higher2 == NULL){
        return;
    }

    std::stringstream highscore1;
    std::stringstream highscore2;
    std::stringstream highscore3;
    highscore1<<"First Place: "<<higher1;
    highscore2<<"Second Place: "<<higher2;
    highscore3<<"Third Place: "<<higher3;

    int scoresy1 = 0;
    int scoresy2 = 0;
    int scoresy3 = 0;

     while(quit == false)
    {
        fps.start();
        cap = ( !cap );

        highs = TTF_RenderText_Solid(fuente, "High Scores", textColor);

        nums1 = TTF_RenderText_Solid(fuente, highscore1.str().c_str(), textColor);
        nums2 = TTF_RenderText_Solid(fuente, highscore2.str().c_str(), textColor);
        nums3 = TTF_RenderText_Solid(fuente, highscore3.str().c_str(), textColor);

        apply_surface(0,0,background, screen);
        apply_surface(200, 0, highs, screen);
        apply_surface(150, scoresy1, nums1,screen);
        apply_surface(150, scoresy2, nums2,screen);
        apply_surface(150, scoresy3, nums3,screen);
        apply_surface(0,0,anim[frame], screen);

        while(SDL_PollEvent(&event))
        {

            if(event.type == SDL_QUIT)
            {

                quit = true;
            }

                if(event.type == SDL_KEYDOWN)
            {

                if(event.key.keysym.sym == SDLK_RETURN){


                return;

                }


            }

            }




        i++;
        scoresy1++;
        scoresy2+=2;
        scoresy3 = scoresy3+3;

        if(scoresy1 == 140){
            scoresy1--;

                }

        if(scoresy2 == 240){
            scoresy2-=2;

                }

        if(scoresy3==345){
            scoresy3-=3;
        }


        if(i%10 == 0){
            frame++;
            if(frame>3){

                frame = 2;
            }


        }
        if(SDL_Flip(screen) == -1)
        {

            return;
        }


        if( ( cap == true ) && ( fps.get_ticks() < 1000 / FRAMES_PER_SECOND ) )
        {
            //Sleep the remaining frame time
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }
    }

}

void menu2()
{
    int frame = 0;
    bool cap = true;
    Timer fps;
    int menux = 0;
    int menuy = 0;
    int groundx = 0;
    int groundy = 0;
    int movex = 0;
    int movey = -518;
    bool quit = false;
    int selecting = 1;


    SDL_Surface* menu_bg = load_image("menu.png");
    SDL_Surface* ground = load_image("ground.png");
    SDL_Surface* tip = load_image("tipy.png");
    SDL_Surface* bg = load_image("stretch.png");

    music = Mix_LoadWAV("Sound/musiqui.wav");

    play = TTF_RenderText_Solid( fuente, "Jugar", textColor );
    info = TTF_RenderText_Solid(fuente, "Instrucciones", textColor);
    exito = TTF_RenderText_Solid(fuente, "Salir" , textColor);
    escores = TTF_RenderText_Solid(fuente, "Scores", textColor);

    Mix_PlayChannel( -1, music, 0 );

    while(quit == false)
    {
        fps.start();
        cap = ( !cap );

        while(SDL_PollEvent(&event))
        {

            if(event.type == SDL_QUIT)
            {


                exit(0);
            }



            if(event.type == SDL_KEYDOWN )
            {


                if(event.key.keysym.sym == SDLK_UP)
                {
                    Mix_PlayChannel( -1, navig, 0 );
                    Mix_VolumeChunk(navig, 50);
                    if(selecting>1)
                    {
                        selecting--;
                        break;
                    }
                }

                if(event.key.keysym.sym == SDLK_DOWN)
                {
                    Mix_PlayChannel( -1, navig, 0 );
                    Mix_VolumeChunk(navig, 50);
                    if(selecting<4)
                    {
                        selecting++;
                        break;
                    }
                }

                if(event.key.keysym.sym == SDLK_RETURN)
                {
                    Mix_PlayChannel( -1, enter, 0 );
                    Mix_VolumeChunk(enter, 50);
                    if(selecting == 1)
                    {
                        theGame();
                    }
                    if(selecting == 2)
                    {
                        instructions();
                    }
                    if(selecting == 3)
                    {
                        HighScores();

                    }

                    if(selecting == 4){

                        exit(0);
                    }
                }

            }




        }


        apply_surface(menux,menuy,menu_bg, screen);



        apply_surface(255, 130, play, screen);
        apply_surface(170, 205, info, screen);
        apply_surface(260, 350, exito, screen);
        apply_surface(240, 279, escores, screen);

        if(selecting == 1)
        {

            apply_surface(165, 84, tip, screen);

        }

        if(selecting == 2)
        {

            apply_surface(75, 162, tip, screen);

        }

        if(selecting == 3)
        {

            apply_surface(145, 230, tip, screen);


        }

        if(selecting == 4){


            apply_surface(155, 305, tip, screen);
        }



        if(SDL_Flip(screen) == -1)
        {

            return;
        }

        frame++;

        if( ( cap == true ) && ( fps.get_ticks() < 1000 / FRAMES_PER_SECOND ) )
        {
            //Sleep the remaining frame time
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }
    }



}

void menu()
{

    int frame = 0;
    bool cap = true;
    Timer fps;
    int menux = 0;
    int menuy = 0;
    int groundx = 0;
    int groundy = 0;
    int movex = 0;
    int movey = -518;
    bool quit = false;
    SDL_Surface* menu_bg = load_image("menu.png");
    SDL_Surface* ground = load_image("ground.png");
    SDL_Surface* stretch = load_image("stretch.png");

    Mix_PlayChannel( -1, intro, 0 );
    Mix_VolumeChunk(intro, 60);

    while(quit == false)
    {
        fps.start();
        cap = ( !cap );

        apply_surface(menux,menuy,menu_bg, screen);
        movey+=2;
        apply_surface(movex, movey, stretch, screen);

        if(movey == 0)
        {

            menu2();


        }

        if(SDL_Flip(screen) == -1)
        {

            return;
        }

        frame++;

        if( ( cap == true ) && ( fps.get_ticks() < 1000 / FRAMES_PER_SECOND ) )
        {
            //Sleep the remaining frame time
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }
    }

}







Timer::Timer()
{

    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start()
{



    started = true;

    paused = false;

    startTicks = SDL_GetTicks();



}


void Timer::stop()
{
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}

void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}


void Timer::unpause()
{
    //If the timer is paused
    if( paused == true )
    {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    //If the timer is running
    if( started == true )
    {
        //If the timer is paused
        if( paused == true )
        {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
    return 0;
}

bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}


int main( int argc, char* args[] )
{
    //Quit flag
    int framesd = 0;

    bool cap = true;


    bool quit = false;

    //Initialize
    if( init() == false )
    {
        return 1;
    }

    //Load the files
    if( load_files() == false )
    {
        return 1;
    }




    menu();

    //Clean up
    clean_up();

    return 0;
}
