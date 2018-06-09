#include "BFS.h"
#define THREADS 4

void BFS::nextPos(position cur, position dest, unsigned int ID)
{
  bool found = false;
  unsigned int hops = 0;
  unsigned int prevHop = 0;

	double weight = 0;
	
	if(weights.find(ID) != weights.end()){
		weight = weights.find(ID)->second;
	}
	
	double prevEffort = 0;
	double effort = 0;

  if(isValidStop(dest,cur)){
    moveSpace prev = {cur,hops};
    path.insert(pair<position,moveSpace>(cur,prev));
    printSinglePath(cur,finalDest);
    path.clear();
    return;
  }

  //cur, prev with hop
	moveSpace prev = {cur,hops++, weight, effort};

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
			prevEffort = path.find(next)->second.effort;

			prev.loc = next;
			prev.hop = hops + prevHop;
			prev.effort = (weight * hops);
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
				prevEffort = path.find(next)->second.effort;
        if(prevHop > 30000) //debug issue
          prevHop = 0;
				prev.loc = next;
				prev.hop = hops + prevHop;
				prev.effort = (weight * (prevHop+1));
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

void BFS::hopOnlyNeighbors(position cur)
{
    position right = {cur.x + 1, cur.y};
    position left = {cur.x - 1, cur.y};
    position up = {cur.x, cur.y + 1};
    position down = {cur.x, cur.y - 1};

    position arr[ADJ_SIZE] = {right, left, up, down};
	//clear just in case
	for(int i = 0; i < neighbors.size(); i++)
		neighbors.pop();

//	#pragma omp parallel for schedule(static) num_threads(THREADS)
  for(int i = 0; i < ADJ_SIZE; i++){
		if(shelf.find(arr[i])->second == false || shelf.find(arr[i]) == shelf.end())
    	neighbors.push(arr[i]);
    }
}

void BFS::printSinglePath(position start, position end)
{
	unsigned int hop = path.find(end)->second.hop;
	double weight = path.find(end)->second.weight;
	double effort = path.find(end)->second.effort;


	totalDist = 0;
	totalEffort = 0;

  position reverse[hop];
	position temp = path.find(end)->first;
	reverse[hop] = end;

	//OpenMP
	for(unsigned int i = 1; i <= hop; i++){
		temp = path.find(temp)->second.loc;

		reverse[hop-i] = temp;
	}
	
	totalDist += hop;
	totalEffort += effort;
	printf(" %i\t\t\t\t %f\t\t\t %f\t\t",hop, weight, effort);

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
	queue<unsigned int> temp = orgItems;
	if(!isValid(start)) {
		printf("Starting position is either out of bounds or starts on a shelf.\n");
		return;
	}else {
		printf("---------------------------------------------------------------------\n");
		printf("Order\t\t Distance\t Weight\t\t\t\t Effort\t\t\t  Path\n");
		if(!temp.empty()){

			unsigned int tempID = temp.front();
			int totalDist = 0;
			double totalEffort = 0;
			position next = {0,0};
			
			printf("%i\t\t",tempID);
			nextPos(start, getPos(tempID), tempID);
			totalDist = getTotalDist();
			totalEffort = getTotalEffort();
			temp.pop();

			next = getFinalDest();
		
			int n = temp.size();
			for(int i = 0; i < n ; i++){
				tempID = temp.front();
				printf("%i\t\t",tempID);
				nextPos(next, getPos(tempID), tempID);
				totalDist +=  getTotalDist();
				totalEffort +=  getTotalEffort();
				
				temp.pop();
				next = getFinalDest();
			}
			printf("end\t\t\t");
			nextPos(next,end, tempID);
			totalDist += getTotalDist();
			totalEffort += getTotalEffort();
			printf("Total Distance = %i\t Total Effort = %f\n",totalDist, totalEffort);
			//last move, otherwise it ends at a neighbor of end
			//printf("(%i,%i)\n",end.x,end.y); //should hop++
		}
	}
}
     
void BFS::processSingleOrder(int indx)
{
  int n = indx;
	while(!orgItems.empty())
		orgItems.pop();

	while(!optItems.empty())
		optItems.pop();	

  for(int j = 0; j < orderFile[n].size(); j++){
    orgItems.push(orderFile[n][j]);  
    optItems.push(orderFile[n][j]);
	} 
}

int BFS::getTotalDist()
{
  return totalDist;
}

double BFS::getTotalEffort()
{
  return totalEffort;
}

position BFS::getFinalDest()
{
  return finalDest;
}

void BFS::hopOnly(position cur, int newRowLeft)
{
  bool found = false;
  unsigned int hops = 0;
  unsigned int prevHop = 0;
	
	int terminate = 0;
	int srcID = dpRef.find(cur)->second;

  //cur, prev with hop
	moveSpace prev = {cur,hops++};

  queue<position> active;
	active.push(cur);
	
	path.insert(pair<position,moveSpace>(prev.loc,prev));
	moveSpace filler = {prev.loc, INF};

  while(!active.empty() && terminate < newRowLeft) {
		position next = active.front();

		if(dpRef.find(next) != dpRef.end())
			found = true;
		
		active.pop();

		if(found){
			finalDest = next;
			
			//printf("prev(%i,%i), cur(%i,%i)\n",next.x,next.y,prev.loc.x,prev.loc.y);
			prevHop = path.find(next)->second.hop;
			int destID = dpRef.find(finalDest)->second;
			//visited shelf already, treat it as obstruction
			if(shelf.find(next) != shelf.end()) 
				shelf.find(next)->second = true; 

			if(dp2[srcID][destID] == 0){
			
				if(cur == next){
					dp2[srcID][srcID] = INF;
					terminate++;
				}
				else {			
					dp2[srcID][destID] = prevHop;
					dp2[destID][srcID] = prevHop; //get compliment
					terminate++;
				}
			} 
		
			found = false;
		}
	
		//create new list of neighbors
    hopOnlyNeighbors(next);

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
	path.clear();
	return;
}

//maps positions with index
void BFS::makeRefDP()
{
	int count = 0;
	map<position, bool> ::iterator it2 = shelf.begin();
	//insert start
	dpRef.insert(pair<position, int>(start,count++));

	for(; it2 != shelf.end(); it2++){
		position cur = it2->first;
		
		if(dpRef.find(cur) == dpRef.end()){
			dpRef.insert(pair<position, int>(cur,count++));
		}
	} 
	//insert end (exlucde for now)
//	dpRef.insert(pair<position, int>(end,count));
	
/*	map<position, int> ::iterator it3 = dpRef.begin();
	for(; it3 != dpRef.end(); it3++){
		printf("(%i,%i)\n", it3->first.x, it3->first.y);
	} */
}


void BFS::preProcess()
{
	map<position, int> ::iterator it3 = dpRef.begin();

	int n = dpRef.size(); //includes start (include end later)
	//printf("dpRefsize = %i\n", dpRef.size());
	position cur;

	//#pragma omp parallel for schedule(static) num_threads(THREADS)
	for(; it3 != dpRef.end(); it3++){
		cur = it3->first;
		hopOnly(cur, n--);
		resetShelf(); // turn all to unvisited
		//	printf("start (%i,%i) dest (%i,%i)\n", cur.x, cur.y, dest.x, dest.y);
		//	printf("here %i\n", hopOnly(it3->first, it4->first,clear));
	}

/*	n = shelf.size(); //excludes start and end
	//printf("shelf size %i \n", n);
	for(int i = 0; i < n; i++){
		for(int k = 0; k < n; k++){
			printf("%i, ", dp2[i][k]);
		}
		printf("\n");
	} */
}

int ** BFS::makeSubDP()
{
	queue<unsigned int> temp = orgItems;
	int n = temp.size() + 1;
//	printf("arr size %i\n", n);
	int arr[n];

	arr[0] = dpRef.find(start)->second;
	for(int i = 0; i < n; i++){
		arr[i] = temp.front();
		//printf("ID %i ", arr[i]);
		temp.pop();
	}
	int s[n][n];
	int ** subDP = 0;


	for(int i = 0; i < n; i++){
		position pos = getPos(arr[i]);
		int srcIndx = dpRef.find(pos)->second;

		for(int j = i; j < n; j++){
			if(i == j)
				s[i][j] = INF;
			
			else{
				pos = getPos(arr[j]);
				int destIndx = dpRef.find(pos)->second;
				s[i][j] = dp2[srcIndx][destIndx];
				s[j][i] = dp2[srcIndx][destIndx];
			}
		}
	}
	
	subDP = new int * [n];
	
	for(int i = 0; i < n; i++){
		subDP[i] = new int[n];
		for(int j = 0; j < n; j++){
			subDP[i][j] = s[i][j];
			}
	}
	

		for(int i = 0; i < n; i++){
			for(int j = 0; j < n; j++){
				printf("%i ", subDP[i][j]);
			}
			printf("\n");
		}
	return subDP; 
}

void BFS::readWeight(string in)
{
  string ID, weight; 
  ID = ""; weight = ""; 
  infile = in;
  ifstream myFile;
  myFile.open(infile.c_str());
	
  if(myFile.is_open()){
    getline(myFile, ID,',');
		
    while(myFile.good() && !ID.empty()){
		  getline(myFile, weight,'\n');

			string tens = weight.substr(0, weight.find("."));
	
		  weights.insert(pair<unsigned int, double>(atoi(ID.c_str()), stod(weight) ));
			getline(myFile,ID,',');
		}
		    
	  myFile.close();
  }
  else{
    printf("%s could not be opened\n",infile.c_str());
    return;
  }

	/*map<unsigned int, double>::iterator it = weights.begin();

	for(; it != weights.end(); it++){
		printf("ID %i weight %f\n",it->first, it->second);
	} */
}

void BFS::setOpt(queue<unsigned int> in)
{
	int n = orgItems.size();
	for(int i = 0; i < n; i ++)
		orgItems.pop();

	n = in.size();
	
	for(int i = 0; i < n; i ++){
		orgItems.push(in.front());
		in.pop();
	}
	return;
}
int BFS::getItemSize()
{
	return orgItems.size();
}
queue<unsigned int> BFS::getItems()
{
	return orgItems;
}
int BFS::getDPRef(position want)
{
	if(dpRef.find(want) != dpRef.end())
		return dpRef.find(want)->second;
	else 
		return -1;
}

int BFS::getDPRef(unsigned int ID)
{
//	printf("I read %i\n", ID);
	position want = getPos(ID);
	//printf("(%i, %i)\n", want.x, want.y);
	if(dpRef.find(want) != dpRef.end())
		return dpRef.find(want)->second;
	else 
		return -1;
}

