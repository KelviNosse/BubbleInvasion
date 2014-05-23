#include "Explosive.h"

Explosive::Explosive(SDL_Surface* surface, SDL_Surface* screen, int x, int y)
{
    this->surface = surface;
    this->screen = screen;
    this->x = x;
    this->y = y;
    this->borrar = false;

    this->tipo = "Explosive";
}

Explosive::~Explosive()
{
    //dtor
}
