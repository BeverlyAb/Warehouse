#include "BFS.h"
#define THREADS 1

void BFS::nextPos(position cur, position dest)
{
  bool found = false;
  unsigned int hops = 0;
  unsigned int prevHop = 0;

  if(isValidStop(dest,cur)){
    moveSpace prev = {cur,hops};
    path.insert(pair<position,moveSpace>(cur,prev));
    printSinglePath(cur,finalDest);
    path.clear();
    return;
  }

  //cur, prev with hop
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
	//		printf("prev(%i,%i), cur(%i,%i)\n",next.x,next.y,prev.loc.x,prev.loc.y);

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

      //add to path only if unvisited
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

	finalDest = itHolder->first;
	printSinglePath(cur,finalDest);
	path.clear(); 
	while(!active.empty()) active.pop();
}

bool BFS::isValidStop(position ref, position stop)
{
  if(	(stop.x == ref.x + 1 && stop.y == ref.y) ||
 		(stop.x == ref.x - 1 && stop.y == ref.y))
		return true;
	else
		return false;
}

bool BFS::isValid(position cur)
{
  //out of bounds check
    if(cur.x > width + 1 || cur. y > height + 1  ||
    //obstruction check
    shelf.find(cur) != shelf.end()) {
        return false;
	}
    else
    	return true;
}

void BFS::validNeighbors(position cur)
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

void BFS::printSinglePath(position start, position end)
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

position BFS::getPos(unsigned int ID)
{
	return stock.find(ID)->second;
}

void BFS::getPath()
{
if(!isValid(start)) {
		printf("Starting position is either out of bounds or starts on a shelf.\n");
		return;
	}else {
		printf("---------------------------------------------------------------------\n");
		printf("Order\t\t Distance\t Path\n");
		if(!orgItems.empty()){

			unsigned int tempID = orgItems.front();
			int totalDist = 0;
			position next = {0,0};
			
			printf("%i\t\t",tempID);
			nextPos(start, getPos(tempID));
			totalDist = getTotalDist();
			orgItems.pop();

			next = getFinalDest();
		
			int n = orgItems.size();
			for(int i = 0; i < n ; i++){
				tempID = orgItems.front();
				printf("%i\t\t",tempID);
				nextPos(next, getPos(tempID));
				totalDist +=  getTotalDist();
				
				orgItems.pop();
				next = getFinalDest();
			}
			printf("end\t\t\t");
			nextPos(next,end);
			totalDist += getTotalDist();
			printf("Total Distance = %i\n",totalDist);
			//last move, otherwise it ends at a neighbor of end
			//printf("(%i,%i)\n",end.x,end.y); //should hop++
		}
	}
}
     
void BFS::processSingleOrder(int indx)
{
  int n = indx;
  for(int j = 0; j < orderFile[n].size(); j++){
    orgItems.push(orderFile[n][j]);  
    optItems.push(orderFile[n][j]);
	} 
  getPath();
}

int BFS::getTotalDist()
{
  return totalDist;
}

position BFS::getFinalDest()
{
  return finalDest;
}

void BFS::hopOnly(position cur, position dest, bool clear)
{
  bool found = false;
  unsigned int hops = 0;
  unsigned int prevHop = 0;
	
	int srcID = dpRef.find(cur)->second;
	int destID = dpRef.find(dest)->second;

	if(cur == dest){
		dp[srcID][destID] = INF;
	}
  //cur, prev with hop
	moveSpace prev = {cur,hops++};

  queue<position> active;
	active.push(cur);
	
	path.insert(pair<position,moveSpace>(prev.loc,prev));
	moveSpace filler = {prev.loc, INF};

  while(!active.empty()) {

		position next = active.front();
		
		if(dest == next)
			found = true;
		
		active.pop();

		if(found){
			finalDest = next;
		//	printf("prev(%i,%i), cur(%i,%i)\n",next.x,next.y,prev.loc.x,prev.loc.y);
			prevHop = path.find(next)->second.hop;
			dp[srcID][destID] = prevHop;

			if(clear)
				path.clear();
		}
		//create new list of neighbors
    validNeighbors(next);
    int n = neighbors.size();
    for(int i = 0; i < n; i++){
      position temp = neighbors.front();

      //add to path only if unvisited
			if(path.find(temp) == path.end()){
				prevHop = path.find(next)->second.hop;
				
			//	printf("prevHop = %i\n", path.find(next)->second.hop);
				
				prev.loc = next;
				prev.hop = hops + prevHop;
        //printf("hop = %i, prev(%i,%i)\n",prevHop,prev.loc.x,prev.loc.y);
				path.insert(pair<position,moveSpace>(temp,prev));
				active.push(temp);
			}
			neighbors.pop();
    }
  }
}

//maps positions with index
void BFS::makeRefDP()
{
	int count = 0;
	map<position, unsigned int> ::iterator it2 = shelf.begin();
	//insert start
	dpRef.insert(pair<position, int>(start,count++));

	for(; it2 != shelf.end(); it2++){
		position left = {it2->first.x - 1, it2->first.y};
		position right = {it2->first.x + 1, it2->first.y};

		//update ref library
		if(dpRef.find(left) == dpRef.end() && left.x > 0 && shelf.find(left) == shelf.end()){
			dpRef.insert(pair<position, int>(left,count++));
		}

		if(dpRef.find(right) == dpRef.end() && right.x < width && shelf.find(right) == shelf.end()){
			dpRef.insert(pair<position, int>(right,count++));
		}
	}

	map<position, int> ::iterator it3 = dpRef.begin();
	for(; it3 != dpRef.end(); it3++){
//		printf("(%i,%i)\n", it3->first.x, it3->first.y);
	}
}


void BFS::preProcess(bool readFromFile, string file)
{
	int i = 0; 	
	bool clear = false;
	map<position, int> ::iterator itClear = dpRef.end();
	itClear--;
	map<position, int> ::iterator it3 = dpRef.begin();


	for(; it3 != dpRef.end(); it3++){
		int j = 0;
		map<position, int> ::iterator it4 = dpRef.begin();
	
		for(; it4 != dpRef.end(); it4++){	
				if(it4 == itClear)
					clear = true;
				hopOnly(it3->first, it4->first, clear);
				clear = false;
				printf("start (%i,%i) dest (%i,%i)\n", it3->first.x, it3->first.y, it4->first.x, it4->first.y);
			//	printf("here %i\n", hopOnly(it3->first, it4->first,clear));
			}
		}
	i++;

	i = 0; 
	int n = shelf.size();
	for(; i < n; i++){
		for(int k = 0; k < n; k++){
			printf("%i, ", dp[i][k]);
		}
		printf("\n");
	}
}