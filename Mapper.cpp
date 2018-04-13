#include "Mapper.h"

Mapper::Mapper()
{   
    width = 0;
    height = 0;
}

 Mapper::Mapper(unsigned int w, unsigned int h)
 {
    width = w;
    height = h;
 }

void Mapper::makeMap(unsigned int ID, unsigned int xCoord, unsigned int yCoord)
{
    position itemLoc = {xCoord, yCoord};
    stock.insert(pair<unsigned int, position>(ID,itemLoc));

    //is shelf already in use?
    if(shelf.find(itemLoc) != shelf.end()){
		unsigned int counter = shelf.find(itemLoc)->second + 1;
        shelf.insert(pair<position, unsigned int>(itemLoc,counter));	
    } 
	else{
       	shelf.insert(pair<position, unsigned int>(itemLoc, 0));
	}
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
    unsigned int n = (width + 2) * (height + 2) - stock.size();
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
void Mapper::nextPos(position cur, unsigned int ID)
{
    map<unsigned int, position>::iterator it = stock.begin();
	it = stock.find(ID);
    position dest = {it->second.x, it->second.y};

    bool found = false;
	unsigned int hops = 0;

	//cur, prev with hop
	moveSpace prev = {cur,hops++};
    //path.insert(pair<position,moveSpace>(cur,prev));
	
	printf("pos 1(%i,%i) prev(%i,%i)\n", path.find(cur)->first.x,
														path.find(cur)->first.y,
														path.find(cur)->second.loc.x,
														path.find(cur)->second.loc.y);
	
	position curStatic = cur;
	queue<position> active;	
	active.push(cur);

	position finalDest = {0,0};

    while(!active.empty() && !found) {

		position next = active.front();					
		found = isValidStop(dest,next);	
		active.pop();

		if(found){
			printf("FOUND_-----------------------\n");
			finalDest = next;

			unsigned int prevHop = path.find(next)->second.hop;
			prev = {next, hops+prevHop};
			path.insert(pair<position,moveSpace>(next,prev));
		
			printf("FOUND pos(%i,%i) prev(%i,%i)\n", path.find(next)->first.x,
														path.find(next)->first.y,
														path.find(next)->second.loc.x,
														path.find(next)->second.loc.y); 		
			
			printPath(cur,finalDest);
		}
		//create new list of neighbors	
        validNeighbors(next);		
	
       	while(!neighbors.empty()){
            position temp = neighbors.front(); 		    
			
			if(path.find(temp) == path.end()){
				unsigned int prevHop = path.find(next)->second.hop;
				prev = {next, hops+prevHop};
				path.insert(pair<position,moveSpace>(temp,prev));
			//	printf("temp pos(%i,%i) visit hop = %i\n",temp.x,temp.y,path.find(temp)->second.hop);
				printf("pos(%i,%i) prev(%i,%i)\n", path.find(temp)->first.x,
														path.find(temp)->first.y,
														path.find(temp)->second.loc.x,
														path.find(temp)->second.loc.y);  
				active.push(temp);
			}	
			neighbors.pop();
        }
    }
	/*printf("path begin: pos(%i,%i) prev(%i,%i)\n", path.begin()->first.x,
														path.begin()->first.y,
														path.begin()->second.x,
														path.begin()->second.y); */
	
}

int Mapper::shortest(int label[], bool visited[], position * grid, position cur)
{
 /*  unsigned int min = INF;
   unsigned int minIndx; 
   unsigned int n = (width + 2) * (height + 2) - stock.size();
  
   for (unsigned int i = 0; i < n; i++){
     if (visited[i] == false && label[i] <= min && isValidNeighbor(grid[i]))
        min = label[i];
        minIndx = i;
   }

   return minIndx; */
	return 0;
} 
void Mapper::printPath(position start, position end)
{
    printf("(x,y)\t\n");
	printf("start (%i,%i)\t\n",start.x, start.y);
	printf("end (%i,%i)\t\n",end.x, end.y);
	unsigned int hop = path.find(end)->second.hop;

	position reverse[hop];
	position temp = path.find(end)->first;
	reverse[hop] = end;
	for(unsigned int i = 1; i <= hop; i++){
		temp = path.find(temp)->second.loc;
		printf("path :(%i,%i)\n", temp.x, temp.y);
		reverse[hop-i] = temp;
	}	

	//use OpenMP here	
	for(unsigned int i = 0; i <= hop; i++)
		printf("rvrs :(%i,%i)\n", reverse[i].x, reverse[i].y);

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
 bool Mapper::isValid(position cur)
 {
	//out of bounds check
    if(cur.x < 0 || cur.x > width + 1 ||
        cur.y < 0 || cur. y > height + 1  ||
    //obstruction check 
    shelf.find(cur) != shelf.end()) {
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
        if(isValid(arr[i])){
            neighbors.push(arr[i]);
		}
    }
 }
/*
bool Mapper::lessVal(const pair<position,unsigned int> & left,
			 const pair<position,unsigned int> & right)
{
	return (left.second < right.second);
}
*/
/*position Mapper::minVal(const map<position, unsigned int> & in)  
{	
	printf("%i", position(in.begin(),in.end(),& Mapper::lessVal).x);
	position temp = {0,0};
	return temp;
}*/
/*bool operator==(const position & left, const position & right)
{
    return (left.x == right.x && left.y == right.y);
}*/

bool operator<(const position & left, const position & right) 
{
	return ((left.y < right.y) || (left.y == right.y &&  left.x < right.x));
}


