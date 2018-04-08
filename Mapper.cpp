#include "Mapper.h"

Mapper::Mapper()
{   
    width = 0;
    height = 0;
  //  queue <position> path;
}

 Mapper::Mapper(unsigned int w, unsigned int h)
 {
    width = w;
    height = h;
   // queue <position> path;
 }

void Mapper::makeMap(unsigned int ID, unsigned int xCoord, unsigned int yCoord)
{
    position itemLoc = {xCoord, yCoord};
    order.emplace(ID, itemLoc);
  
    if(!shelf.empty){
        //is shelf already in use?
        if(shelf.find(itemLoc) != shelf.end()){
            unsigned int counter = shelf.find(itemLoc)->second + 1;
            shelf.emplace(itemLoc, counter);
        }
    } else
        shelf.emplace(itemLoc, 0);
}

position * Mapper::makeGrid(position * grid)
{
    // +2 on to give walkable outer layer
    unsigned int dimension = (width + 2) * (height + 2);
    position *  grid  = (position *) malloc(dimension * sizeof(position));
    
    //populate grid row-wise first
    int k = 0;
    for(int i = 0; i < height + 2; i++){
        for(int j = 0; j < width + 2; j++){
            position temp = {j, i};
            grid[k++] = temp;
        }
    }
    return grid;
}
/*    //use Dijkstra 
void Mapper::nextPos(position cur, product item, position * grid)
{
    //paths exclude shelves
    unsigned int n = (width + 2) * (height + 2) - order.size();
    bool visited[n];
    int label[n];
    for (unsigned int i = 0; i < n; i++){
        visited[i] = false;
        label[i] = INF;
    }
    
    //find where cur is in grid
    unsigned int curIndx = 0;

    position temp = grid[curIndx];
    while(!(cur == temp)){
        temp = grid[curIndx++];
    }
    
    //start has 0 distance from itself
    label[curIndx] = 0;

    //iterate through vertices
    for(unsigned int i = 0; i < n - 1; i++){
        unsigned int next = shortest(label,visited,grid,grid[curIndx]);
        visited[next] = true;

        //iterate through neighbors
        for(int j = 0; j < neighbors.size(); j++){
            if  (!visited[j] && label[curIndx] != INF && 
                label[curIndx] < label[j])
                label[j] = label[next];
        }
    }
}
*/
//BFS single weight
void Mapper::nextPos(position cur, product item, position * grid)
{
    //paths exclude shelves
    map<unsigned int, position>::iterator it = order.find(item.ID);
    position dest = it->second;

    unsigned int n = (width + 2) * (height + 2) - order.size();
    map<position,bool> visited;
    visited.emplace(cur, true);
    path.push(cur);
    while(!visited.empty() || next!= dest){
        position next = visited.front()->first;
        visited.erase(cur);
        
        validNeighbors(cur);
        for(int i = 0; i < neighbors.size(); i++){
            position temp = neighbors.front();
            neighbors.pop();
            if(visited.find(temp) != visited.end()){
                visited.emplace(temp,true);
            }
        }
    }
}

int Mapper::shortest(int label[], bool visited[], position * grid, position cur)
{
   unsigned int min = INF;
   unsigned int minIndx; 
   unsigned int n = (width + 2) * (height + 2) - order.size();
  
   for (unsigned int i = 0; i < n; i++){
     if (visited[i] == false && label[i] <= min && isValidNeighbor(grid[i]))
        min = label[i];
        minIndx = i;
   }

   return minIndx;
}
void Mapper::printPath()
{
    printf("(x,y)\t\n");
    for(int i = 0; i < path.size(); i++){
        position out = path.front();
        path.pop();
        printf("(%i,%i)\n",out.x,out.y);
    }
}


//stops at either left or right of shelf 
bool Mapper::isValidStop(product package, position stop)
{
    return (stop.x == package.loc.x + 1 || stop.x == package.loc.x - 1);
}
//no diagonals allowed!
 bool Mapper::isValidNeighbor(position next)
 {
     //out of bounds check
    if(next.x < 0 || next.x > width + 2 ||
        next.y < 0 || next. y > height + 2)
        return false;
    //obstruction check 
    if(shelf.find(next) != shelf.end())
        return false;
    
    return true;
 }

void Mapper::validNeighbors(position cur)
 {
    position right = {cur.x + 1, cur.y};
    position left = {cur.x - 1, cur.y};
    position up = {cur.x, cur.y + 1};
    position down = {cur.x, cur.y - 1};

    position arr[ADJ_SIZE] = {right, left, up, down};
    for(int i = 0; i < ADJ_SIZE; i++){
        if(isValidNeighbor(arr[i]))
            neighbors.push(arr[i]);
    }
 }

bool operator==(const position & left, const position & right)
{
    return (left.x == right.x && left.y == right.y);
}