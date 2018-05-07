#include "Layout.h"
/*
    unsigned int width;
    unsigned int height;
    int totalDist;

    queue <position> neighbors;
	  map<position,moveSpace> path;

    map<unsigned int, position> stock; //reads via ID
    map<position, unsigned int> shelf; //reads via position and # of items in shelf
	  position finalDest;
*/
//default constructor
Layout::Layout()
{
  width = 0, height = 0, totalDist = 0;
  finalDest.x = 0; finalDest.y = 0;
}

//constructor with param
Layout::Layout(unsigned int w, unsigned int h)
{
  width = w;
  height = h;
  totalDist = 0;
  finalDest.x = 0; finalDest.y = 0;
}

//make map of items and their positions
void Layout::makeStock(unsigned int ID, unsigned int xCoord, unsigned int yCoord)
{
  position itemLoc = {xCoord, yCoord};
  stock.insert(pair<unsigned int, position>(ID,itemLoc));

  //is shelf already in use?
  if(shelf.find(itemLoc) != shelf.end()){
		unsigned int counter = shelf.find(itemLoc)->second + 1;
    map<position,unsigned int>::iterator it = shelf.find(itemLoc);
	  it->second = counter;
  }
	else{
    shelf.insert(pair<position, unsigned int>(itemLoc, 1));
	}
}
//returns the position of an item
position Layout::getPos(unsigned int)
{
  return stock.find(ID)->second;
}
void Layout::nextPos(position cur, position dest)
{
  bool found = false;
  unsigned int hops = 0;
  unsigned int prevHop = 0;

  if(isValidStop(dest,cur)){
    moveSpace prev = {cur,hops};
    path.insert(pair<position,moveSpace>(cur,prev));
    return;
  }

  moveSpace prev = {cur,hops++};
  queue<position> active;
  map<position,moveSpace> onlyFound;
	active.push(cur);

  while(!active.empty()) {

		position next = active.front();
		found = isValidStop(dest,next);
		active.pop();

    if(found){
			finalDest = next;
      onlyFound.insert(pair<position,moveSpace>(next,prev));
	//	printf("prev(%i,%i), cur(%i,%i)\n",next.x,next.y,prev.loc.x,prev.loc.y);

			prevHop = path.find(next)->second.hop;
			prev.loc = next;
			prev.hop = hops + prevHop;
			path.insert(pair<position,moveSpace>(next,prev));
		}

    //create new list of neighbors
    validNeighbors(next);
    int n = neighbors.size();
    for(int i = 0; i < n; i++){
      position temp = neighbors.front();

			if(path.find(temp) == path.end()){
				prevHop = path.find(next)->second.hop;
        if(prevHop > 30000) //debug issue
          prevHop = 0;
				prev.loc = next;
				prev.hop = hops + prevHop;
        //printf("hop = %i, prev(%i,%i)\n",prevHop,prev.loc.x,prev.loc.y);
				path.insert(pair<position,moveSpace>(temp,prev));
				active.push(temp);
			}
			neighbors.pop();
    }
  }

  map<position,moveSpace>::iterator it = onlyFound.begin();
	map<position,moveSpace>::iterator itHolder = onlyFound.begin();
	unsigned int min = path.begin()->second.hop;
	for(; it != onlyFound.end(); it++){
		if(min > it->second.hop){
			min = it->second.hop;
			itHolder = it;
		}
	}
}

//stops at either left or right of shelf
bool Layout::isValidStop(position ref, position stop)
{
  if(	(stop.x == ref.x + 1 && stop.y == ref.y) ||
 		(stop.x == ref.x - 1 && stop.y == ref.y))
		return true;
	else
		return false;
}

//checks if positions are within layout
bool Layout::isValid(position cur)
{
	//out of bounds check
    if(cur.x > width + 1 || cur. y > height + 1  ||
    //obstruction check
    shelf.find(cur) != shelf.end()) {
        return false;
	}
    else
    	return true;
}//isValid

//no diagonals allowed! Allows cross movement of worker
void validNeighbors(position cur)
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
  //	printf("neigh(%i,%i)\n",arr[i].x,arr[i].y);
    }
  }
}//validNeighbors

//prints paths of after visiting nodes in certain order
void Layout::printPath(position start, position end)
{
	unsigned int hop = path.find(end)->second.hop;
	totalDist = 0;

  position reverse[hop];
	position temp = path.find(end)->first;
	reverse[hop] = end;

	//OpenMP
	for(unsigned int i = 1; i <= hop; i++){
		temp = path.find(temp)->second.loc;

		reverse[hop-i] = temp;
	}
	
	totalDist += hop;
  printf(" %i\t\t",hop);
	for(unsigned int i = 0; i <= hop; i++)
		printf("(%i,%i)\t", reverse[i].x, reverse[i].y);

  printf("\n"); // bound
}

//returns last position after reaching item dest.
position Layout::getFinalDest()
{
  return finalDest;
}//getFinalDest

//returns distance for 1 - 1 traversal
int Layout::getTotalDist()
{
  return totalDist;
}//totalDist

//map<position,moveSpace> getPath();