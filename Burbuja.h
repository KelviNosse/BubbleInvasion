#ifndef BURBUJA_H
#define BURBUJA_H
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <list>
#include <iostream>
#include <string>
using namespace std;


class Burbuja
{
    public:

            SDL_Surface* surface;
            int x = 0;
            int y = 0;
            string tipo;
            double velocidad;
            SDL_Surface* screen;
            bool borrar;

            void render();
            void logic(int clickx , int clicky, double velocidad);

        Burbuja();
        Burbuja(SDL_Surface* surface, SDL_Surface* screen, int x, int y);

        virtual ~Burbuja();
    protected:
    private:
};

#endif // BURBUJA_H
