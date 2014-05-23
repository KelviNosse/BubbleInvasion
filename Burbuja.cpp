#include "Burbuja.h"

Burbuja::Burbuja()
{

}

Burbuja::Burbuja(SDL_Surface* surface, SDL_Surface* screen, int x, int y)
{
    this->surface = surface;
    this->x = x;
    this->y = y;
    this->screen = screen;
    this->borrar = false;
    this->tipo = "Father";
}

void Burbuja::render()
{

    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( surface, NULL, screen, &offset );

}

void Burbuja::logic(int clickx , int clicky, double velocidad)
{

    x+=velocidad;


    if(clickx >= this->x && clickx <= this->x+ surface->w && clicky >= this->y && clicky <= this->y + surface->h)
    {

        borrar = true;

    }


}

Burbuja::~Burbuja()
{
    //dtor
}
