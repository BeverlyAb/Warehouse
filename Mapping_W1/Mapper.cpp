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
    order.insert(pair<unsigned int, position>(ID,itemLoc));

    if(!shelf.empty()){
        //is shelf already in use?
        if(shelf.find(itemLoc) != shelf.end()){
            unsigned int counter = shelf.find(itemLoc)->second + 1;
            shelf.insert(pair<position, unsigned int>(itemLoc, counter));
        }
    } else
        shelf.insert(pair<position, unsigned int>(itemLoc, 0));
}

position * Mapper::makeGrid()
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
void Mapper::nextPos(position cur, unsigned int item)
{
	//fix later, no idea why map starts at 0 instead of 1 for test_modified.csv
	//if(item == 1) 
	//	item++;
    map<unsigned int, position>::iterator it = order.begin();
	it = order.find(item);
    position dest = {it->second.x, it->second.y};
	//for (it = order.begin(); it != order.end(); it++)
	//	printf("Order %i : (%i,%i)\n",it->first,it->second.x,it->second.y);
	

    bool found = false;
	//printf("dest %i,%i\n",dest.x,dest.y);
    map<position,bool> visited;
    visited.insert(pair<position,bool>(cur,true));

    position next = cur;
	validNeighbors(next);
	path.push(next);
	visited.find(next)->second = true;

	bool notVisited = true;

    while(!path.empty() && !found) {
		next = path.front();	
		found = isValidStop(dest,next);	
		visited.find(next)->second = true;
		printf("(%i, %i, %d)\n",path.front().x, path.front().y, found);
		path.pop();	

		//create new list of neighbors	
        validNeighbors(next);		

	//	printf("Valid neighbors for (%i,%i)\n", next.x,next.y); 
       	while(!neighbors.empty()){//for(int i = 0; i < neighbors.size(); i++){
            position temp = neighbors.front(); 
		//	printf("are (%i %i)\n", temp.x,temp.y);		    

			if	(/*visited.find(temp)->second == false || */
				visited.find(temp) == visited.end()){
				visited.insert(pair<position,bool>(temp,false));
				path.push(temp);
			}	
			neighbors.pop();
        }
    }
}

int Mapper::shortest(int label[], bool visited[], position * grid, position cur)
{
 /*  unsigned int min = INF;
   unsigned int minIndx; 
   unsigned int n = (width + 2) * (height + 2) - order.size();
  
   for (unsigned int i = 0; i < n; i++){
     if (visited[i] == false && label[i] <= min && isValidNeighbor(grid[i]))
        min = label[i];
        minIndx = i;
   }

   return minIndx; */
	return 0;
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
bool Mapper::isValidStop(position ref, position stop)
{
	//printf("(Ref %i,%i Stop %i, %i )\n",ref.x,ref.y, stop.x, stop.y);
    if(	(stop.x == ref.x + 1 && stop.y == ref.y) ||
 		(stop.x == ref.x - 1 && stop.y == ref.y))
		return true;
	else 
		return false;
}
//no diagonals allowed!
 bool Mapper::isValidNeighbor(position next)
 {
	//obstruction check until shelf works..
	map<unsigned int, position>::iterator it = order.begin();
	for (it = order.begin(); it != order.end(); it++){
		if(it->second.x == next.x && it->second.y ==next.y){
			//printf("obstruct: %i,%i\n", it->second.x,it->second.y); 
			return false;
		}
    }
	//out of bounds check
    if(next.x < 0 || next.x > width + 1 ||
        next.y < 0 || next. y > height + 1 ){
    //obstruction check 
    //(shelf.find(next) != shelf.end()))
        return false;
	}
    else 
    	return true;
 }

void Mapper::validNeighbors(position cur)
 {
    position right = {cur.x + 1, cur.y};
    position left = {cur.x - 1, cur.y};
    position up = {cur.x, cur.y + 1};
    position down = {cur.x, cur.y - 1};

    position arr[ADJ_SIZE] = {right, left, up, down};
	//clear just in case
	for(int i = 0; i < neighbors.size(); i++)
		neighbors.pop();	

    for(int i = 0; i < ADJ_SIZE; i++){
        if(isValidNeighbor(arr[i])){
            neighbors.push(arr[i]);
		//	printf("GOOD %i %i\n",arr[i].x,arr[i].y);
		}
    }
 }
bool operator==(const position & left, const position & right)
{
    return (left.x == right.x && left.y == right.y);
}

bool operator<(const position & l, const position & r) {
     return (l.x<r.x || (l.x==r.x && l.y<r.y));
}