#include "Mapper.h"

Mapper::Mapper()
{
    start = {0,0};
    end = {0,0}; // empty
    queue <position> path;
}

map<unsigned int,position> Mapper::makeMap(unsigned int ID, unsigned int xCoord, unsigned int yCoord)
{
    map<unsigned int, position> table;
    position itemLoc = {xCoord, yCoord};
    table.emplace(ID, itemLoc);
    return table;
}
//will return a grid made of map<loc ,map<ID,position> > 
position * makeGrid(position * grid, map<unsigned int,position> a, unsigned int width, unsigned int height)
{
    // +2 on to give outer walkable layer
    int dimension = (width + 2) * (height + 2);
    position *  grid  = (position *) malloc(dimension * sizeof(position));

    return grid;
}
    //use Dijkstra or BFS use queue, 
position Mapper::nextPos(position cur, product item)
{

}
void Mapper::printPath()
{

}
