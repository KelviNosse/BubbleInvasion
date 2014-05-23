#ifndef SHIP_H
#define SHIP_H
#include "Burbuja.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"


class Ship : public Burbuja
{
    public:
        Ship(SDL_Surface* surface, SDL_Surface* screen, int x, int y);
        virtual ~Ship();
    protected:
    private:
};

#endif // SHIP_H
