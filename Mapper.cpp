#include "Mapper.h"

Mapper::Mapper()
{   
    width = 0;
    height = 0;
	finalDest.x = 0; finalDest.y = 0;
}

 Mapper::Mapper(unsigned int w, unsigned int h)
 {
    width = w;
    height = h;
	finalDest.x = 0; finalDest.y = 0;
 }

void Mapper::makeStock(unsigned int ID, unsigned int xCoord, unsigned int yCoord)
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

position Mapper::getPos(unsigned int ID)
{
	return stock.find(ID)->second;
}

void Mapper::makeCluster(unsigned int ID)
{
	position itemLoc = getPos(ID);
   //is shelf already in use?
    if(cluster.find(itemLoc) != cluster.end()){
		unsigned int counter = cluster.find(itemLoc)->second + 1;
        cluster.insert(pair<position, unsigned int>(itemLoc,counter));	
    } 
	else{
       	cluster.insert(pair<position, unsigned int>(itemLoc, 0));
	}
}

//BFS single weight
void Mapper::nextPos(position cur, position dest)
{
    bool found = false;
	unsigned int hops = 0;

	//cur, prev with hop
	moveSpace prev = {cur,hops++};
	
	position curStatic = cur;
	queue<position> active;	
	active.push(cur);

    while(!active.empty()) {

		position next = active.front();					
		found = isValidStop(dest,next);	
		active.pop();

		if(found){
			finalDest = next;
			map<position,moveSpace> onlyFound;
			onlyFound.insert(pair<position,moveSpace>(next,prev));
				
		
			unsigned int prevHop = path.find(next)->second.hop;
			prev.loc = next;
			prev.hop = hops + prevHop;
			path.insert(pair<position,moveSpace>(next,prev));
			
			map<position,moveSpace>::iterator it = onlyFound.begin();
			map<position,moveSpace>::iterator itHolder = onlyFound.begin();
			unsigned int min = path.begin()->second.hop;			
			for(; it != onlyFound.end(); it++){
				if(min > it->second.hop){
					min = it->second.hop;
					itHolder = it;
				}
			}
			finalDest = itHolder->first; //or second?
			printPath(cur,finalDest);
		}
		//create new list of neighbors	
        validNeighbors(next);		
	
       	while(!neighbors.empty()){
            position temp = neighbors.front(); 		    
			
			if(path.find(temp) == path.end()){
				unsigned int prevHop = path.find(next)->second.hop;
				prev.loc = next;
				prev.hop = hops + prevHop;
				path.insert(pair<position,moveSpace>(temp,prev));
				active.push(temp);
			}	
			neighbors.pop();
        }
    }
}
void Mapper::printPath(position start, position end)
{
	unsigned int hop = path.find(end)->second.hop;

	position reverse[hop];
	position temp = path.find(end)->first;
	reverse[hop] = end;
	
	printf("Distance %i\n",hop);
	//OpenMP
	for(unsigned int i = 1; i <= hop; i++){
		temp = path.find(temp)->second.loc;
		reverse[hop-i] = temp;
	}
	
	for(unsigned int i = 0; i <= hop; i++)
		printf("(%i,%i)\n", reverse[i].x, reverse[i].y);

}

//stops at either left or right of shelf 
bool Mapper::isValidStop(position ref, position stop)
{
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
		//	printf("neigh(%i,%i)\n",arr[i].x,arr[i].y);
		}
    }
 }

position Mapper::getFinalDest()
{
	return finalDest;
}


