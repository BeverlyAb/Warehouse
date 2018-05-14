#include "DFS.h"
void DFS::nextPos(position cur, position dest)
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

bool DFS::isValidStop(position ref, position stop)
{
  if(	(stop.x == ref.x + 1 && stop.y == ref.y) ||
 		(stop.x == ref.x - 1 && stop.y == ref.y))
		return true;
	else
		return false;
}

bool DFS::isValid(position cur)
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

void DFS::validNeighbors(position cur)
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

void DFS::printSinglePath(position start, position end)
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

position DFS::getPos(unsigned int ID)
{
	return stock.find(ID)->second;
}

void DFS::getPath()
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
     
void DFS::processSingleOrder(int indx)
{
  int n = indx;
  for(int j = 0; j < orderFile[n].size(); j++){
    orgItems.push(orderFile[n][j]);  
    optItems.push(orderFile[n][j]);
	} 
  getPath();
}

int DFS::getTotalDist()
{
  return totalDist;
}

position DFS::getFinalDest()
{
  return finalDest;
}

unsigned int DFS::pathOnly(position cur, position dest)
{
	bool found = false;
  unsigned int hops = 0;
  unsigned int prevHop = 0;

  if(dest == cur){
    return INF;
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
	//printSinglePath(cur,finalDest);
	path.clear(); 
	while(!active.empty()) active.pop();
	return itHolder->second.hop;
}


void DFS::preProcess()// fix later
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

		if(dpRef.find(right) == dpRef.end() && right.x < width && shelf.find(left) == shelf.end()){
			dpRef.insert(pair<position, int>(right,count++));
		}
	}
	printf("COUNT %i", count);
	position t1 = {0,0};
	position t2 = {10,1};
/*	printf("WHYYY %i\n", pathOnly(t1, t2)); 
	printf("width %i, height %i\n", width, height);
	int i = 0; 	
	map<position, int> ::iterator it3 = dpRef.begin();
	for(; it3 != dpRef.end(); it3++){
		int j = 0;
		map<position, int> ::iterator it4 = dpRef.begin();
		
		for(; it4 != dpRef.end(); it4++){
			//nullify path if start = end
			if(i == j)
				dp[i][j++] = INF;
			else{
			//	dp[i][j++] = pathOnly(it3->first, it4->first);
			//	printf("left (%i,%i) right (%i,%i)\n", it3->first.x, it3->first.y, it4->first.x, it4->first.y);
			//	printf("here %i\n", pathOnly(it3->first, it4->first));
			}
		}
	}*/
/*	i =0; int k = 0;
	int n = MAX_ROW * MAX_COL;
	for(; i < n; i++){
		for(; k < n; k++){
			printf("why %i\n", dp[i][k]);
		}
	}*/
}