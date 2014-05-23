#include "Ship.h"

Ship::Ship(SDL_Surface* surface, SDL_Surface* screen, int x, int y)
{
    this->surface = surface;
    this->screen = screen;
    this->x = x;
    this->y = y;
    this->borrar = true;

    this->tipo = "Ship";
}

Ship::~Ship()
{
    //dtor
}

