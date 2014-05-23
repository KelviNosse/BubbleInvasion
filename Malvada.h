#ifndef MALVADA_H
#define MALVADA_H
#include "Burbuja.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

class Malvada : public  Burbuja
{
    public:
        Malvada(SDL_Surface* surface, SDL_Surface* screen, int x, int );

        virtual ~Malvada();
    protected:
    private:
};

#endif // MALVADA_H
