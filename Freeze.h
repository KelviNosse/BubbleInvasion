#ifndef FREEZE_H
#define FREEZE_H
#include "Burbuja.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

class Freeze : public Burbuja
{
    public:
        Freeze(SDL_Surface* surface, SDL_Surface* screen, int x, int y);
        virtual ~Freeze();
    protected:
    private:
};

#endif // FREEZE_H
