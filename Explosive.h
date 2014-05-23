#ifndef EXPLOSIVE_H
#define EXPLOSIVE_H
#include "Burbuja.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

class Explosive : public Burbuja
{    public:
        Explosive(SDL_Surface* surface, SDL_Surface* screen, int x, int y);
        virtual ~Explosive();
    protected:
    private:
};

#endif // EXPLOSIVE_H
