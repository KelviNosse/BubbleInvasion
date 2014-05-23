#include "Malvada.h"

Malvada::Malvada(SDL_Surface* surface, SDL_Surface* screen, int x, int y)
{
    this->surface = surface;
    this->screen = screen;
    this->x = x;
    this->y = y;
    this->borrar = false;

    this->tipo = "Malvada";

}

Malvada::~Malvada()
{

}
