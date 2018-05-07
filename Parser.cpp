#include "Parser.h"
#include <iostream>

Parser::Parser()
{
    inFile = "";
    outFile = "";
    warehouseWidth = 0;
    warehouseHeight = 0;
    ans = "";

    ID = "";
    xCoord = "";
    yCoord = "";
	grid = Mapper();
    start.x = 0; start.y = 0;
    end.x = 0; end.y = 0;

	orderFile.resize(ROW*MAX_COL);
	dpIndx = 0;
	/*	for(int i = 0; i < MAX; i++){
			for(int j = 0; j < MAX; j++){
				dp[i][j] = -1;
			//	printf("%i ", dp[i][j]);
			}
		} */
	all = false;
	boundCheck = false;
}

void Parser::setUserParam()

{
    printf("What is the file name?\n");
    cin >> this->inFile;

    printf("What is the max warehouse width?\n");
    cin >> this->warehouseWidth;
   	printf("What is the max warehouse height?\n");
    cin >> this->warehouseHeight;

    this->warehouseHeight *= 2;
    this->warehouseWidth *= 2;

    grid = Mapper(warehouseWidth, warehouseHeight);

	cout << "Starting x-coord?\n";
    cin >> xCoord;
   	cout << "Starting y-coord?\n";
    cin >> yCoord;
    start.x = atoi(xCoord.c_str()); start.y = atoi(yCoord.c_str());

	cout << "Ending x-coord?\n";
    cin >> xCoord;
   	cout << "Ending y-coord?\n";
    cin >> yCoord;
    end.x = atoi(xCoord.c_str()); end.y = atoi(yCoord.c_str());
	dpIndx = 0;
	/*for(int i = 0; i < MAX; i++){
		for(int j = 0; j < MAX; j++){
			dp[i][j] = -1;
		}
	}*/
	all = false;
	boundCheck = false;
}

void Parser::readFile(int fileType)
{
	unsigned int x = 0;
	unsigned int y = 0;

    ifstream myFile;
    myFile.open(inFile.c_str());

	if(fileType == NAME_ITEM){
		getNameItem();
		//already updates cluster
	} 
	else if(fileType == ORDER_FILE || fileType == BOUND){ 
		int indx = 0;
		printf("Name of warehouse order file?\n");
		cin >> inFile;
		printf("Give an order list number or type '101' to read the entire file\n");
		cin >> indx;
		getOrder(inFile,indx, fileType); //make sure to form cluster here too		
	} 
	else if(fileType == STOCK){
		
		if(myFile.is_open()){
		   
		    getline(myFile,ID,',');
		    while(myFile.good() && !ID.empty()){
		        getline(myFile,xCoord,',');
		        getline(myFile,yCoord,'\n');

				x = atoi(xCoord.c_str()); y = atoi(yCoord.c_str());
				x *= 2; y *= 2;

				if(x == 0) x++;
				if(y == 0) y++;

		        grid.makeStock(atoi(ID.c_str()), x, y);
				getline(myFile,ID,',');
			}
		    
			myFile.close();
		}
		else
		    printf("%s could not be opened\n",inFile.c_str());
	}
	else {
		printf("Invalid choice between STOCK, NAMEITEM, and ORDER_FILE\n");
	}
}

void Parser::getNameItem()
{
	printf("Name an item ID or type 'done'\n");
	cin >> ans;
	//not very robust
	while(ans != "done"){
		namedItems.push(atoi(ans.c_str()));
		optItems.push(atoi(ans.c_str()));
		//makeCluster(atoi(ans.c_str()));
		cin >> ans;
	}
}

void Parser::getOrder(string file, int index, int fileType)
{
//	clock_t startTime, end;
//	startTime = clock();
	int n = index;
	int  i = 0;
	int start = 0;
	ifstream myFile;
    myFile.open(file.c_str());
	if(myFile.is_open()){
		myFile >> ID;

       while(myFile.good() && !ID.empty()){
		   	if(ID[ID.length()-1] == '"'){
			   i++;
			   myFile >> ID;
		   	} else {
			//remove trailing front ' " '
		   	if(ID[0]== '"'){
				ID.erase(ID.begin());
		   	}
			
			orderFile[i].push_back(atoi(ID.c_str()));
			myFile >> ID;
			}
        }  
		
		myFile.close(); 
	}
	else
	    printf("%s could not be opened\n",file.c_str()); 

	//read entire order list if not in range
	if(n < 0 || n > ROW || n == 101) {
		n = ROW;
		all = true;

		for(int j = 0; j < n; j++){

			for(int k = 0; k < orderFile[j].size(); k++){
				namedItems.push(orderFile[j][k]);
				optItems.push(orderFile[j][k]);
			}
			if(fileType == ORDER_FILE) {
				getPath();
				opt();
				getPath();
			} else{
				branchBound();
			}
		}
	} else{
		//transfer only a line
		for(int j = 0; j < orderFile[n].size(); j++){
			namedItems.push(orderFile[n][j]);  
			optItems.push(orderFile[n][j]);
		} 
		if(fileType == BOUND){
			getPath();
			branchBound();
			getPath();
		}
	}

/*	//keep for debugging cluster
	map<position, vector<unsigned int> >::iterator it = cluster.begin();
	for(; it != cluster.end(); it++){
		for(int i = 0; i < it->second.size(); i++){
			printf("HERE I AM %i \n", it->second[i]);
		}
	} */

	/*// keep for debugging parsing orderFile
 	n = ROW;
	for(int j = 0; j < n; j++){
		for(int k = 0; k < orderFile[j].size(); k++){
			printf("%i\n", orderFile[j][k]);
		}
	}*/
//	end = clock();
//	int t = difftime(end, startTime);
//	printf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
}

void Parser::getPath()
{
	if(!grid.isValid(start)) {
		printf("Starting position is either out of bounds or starts on a shelf.\n");
		return;
	}else {
		printf("---------------------------------------------------------------------\n");
		printf("Order\t\t Distance\t Path\n");
		if(!namedItems.empty()){

			unsigned int tempID = namedItems.front();
			int totalDist = 0;
			position next = {0,0};
			
			printf("%i\t\t",tempID);
			grid.nextPos(start, grid.getPos(tempID));
			totalDist = grid.getTotalDist();
			namedItems.pop();

			next = grid.getFinalDest();
		
			int n = namedItems.size();
			for(int i = 0; i < n ; i++){
				tempID = namedItems.front();
				printf("%i\t\t",tempID);
				grid.nextPos(next,grid.getPos(tempID));
				totalDist += grid.getTotalDist();
				
				namedItems.pop();
				next = grid.getFinalDest();
			}
			printf("end\t\t\t");
			grid.nextPos(next,end);
			totalDist += grid.getTotalDist();
			printf("Total Distance = %i\n",totalDist);
			//last move, otherwise it ends at a neighbor of end
			//printf("(%i,%i)\n",end.x,end.y); //should hop++
			boundCheck = false;
		}
	}
}
void Parser::makeCluster(unsigned int ID)
{
	position itemLoc = grid.getPos(ID);
    if(cluster.find(itemLoc) != cluster.end()){
		vector<unsigned int> v = cluster.find(itemLoc)->second;
		v.push_back(ID);
		cluster.find(itemLoc)->second = v;
    }
	else{
		vector<unsigned int> v;
		v.push_back(ID);
       	cluster.insert(pair<position, vector<unsigned int> >(itemLoc, v));
	}
}

/*
	So far only splits into 4 quadrants, but orders the request based on which quad.
	has the most requests. Don't need cluster anymore. Only works for even width and height so far
*/
void Parser::opt()
{
	//can try recursive call for granularity
	unsigned int xRangeStart[4] = {warehouseWidth/2 + 1, 0, 0, warehouseWidth/2 + 1};
	unsigned int xRangeEnd[4] = {warehouseWidth, warehouseWidth/2,
								 warehouseWidth/2, warehouseWidth};

	unsigned int yRangeStart[4] = {warehouseHeight/2 + 1, warehouseHeight/2 + 1, 0, 0};
	unsigned int yRangeEnd[4] = {warehouseHeight, warehouseHeight,
								 warehouseHeight/2, warehouseHeight/2};

	queue<unsigned int> quad0;
	queue<unsigned int>  quad1;
	queue<unsigned int> quad2;
	queue<unsigned int> quad3;

	queue<unsigned int> quad[4]= {quad0, quad1, quad2, quad3};

	int n = optItems.size();
	//separate order positions into their quad
	for(int l = 0; l < n; l++){
		unsigned int tempID = optItems.front();

		for(int j = 0; j < 4; j++){

			if(grid.getPos(tempID).x >= xRangeStart[j] && grid.getPos(tempID).x <= xRangeEnd[j]
			&& grid.getPos(tempID).y >= yRangeStart[j] && grid.getPos(tempID).y <= yRangeEnd[j]) {

				quad[j].push(tempID);
				break;
			}
		}
		optItems.pop();
	}
/*
	map<position, vector<unsigned int> >::iterator it = cluster.begin();
	for(; it != cluster.end(); it++){
		int size = it->second.size();
		for(int i = 0; i < size; i++){
			for(int j = 0; j < 4; j++){
				unsigned int tempID = it->second[i];
				//printf("tempID = %i\n", tempID);

					printf("%i = %i (%i,%i)\n", i, it->second[i], grid.getPos(tempID).x, grid.getPos(tempID).y);
				if(grid.getPos(tempID).x >= xRangeStart[j] && grid.getPos(tempID).x <= xRangeEnd[j]
					&& grid.getPos(tempID).y >= yRangeStart[j] && grid.getPos(tempID).y <= yRangeEnd[j]) {
					quad[j].push(tempID);
					break;
				}
			}
		}
	} 
*/

	//get orders in the same quad as start first
	unsigned int startQuad = 0;
	for(int j = 0; j < 4; j++){
		if(start.x >= xRangeStart[j] && start.x <= xRangeEnd[j]
			&& start.y >= yRangeStart[j] && start.y <= yRangeEnd[j]) {
			startQuad = j;
			break;
		}
	}

	for(int i = 0; i < quad[startQuad].size(); i++){
		optItems.push(quad[startQuad].front());
		quad[startQuad].pop();
	}


	while(!quad[0].empty() || !quad[1].empty() || !quad[2].empty()|| !quad[3].empty()) {
		int max = quad[0].size();

		for(int i = 0; i < 4; i++){
			if(quad[i].size() > max){
				max = quad[i].size();
			}

			if(quad[i].size() == max){
				for(int j = 0; j < quad[i].size(); j++){
					optItems.push(quad[i].front());
					quad[i].pop();
				}
			}
		}
	}
	//need to reverse order, do'h!
	 n = optItems.size();
	for(int i = 0; i < n; i++){
		namedItems.push(optItems.front());
		optItems.pop();
	}
}//opt

void Parser::branchBound()
{
	dpIndx = 0;
	boundCheck = true; // add checker for duplicates!
	dpMap.insert(pair<int, position>(dpIndx++,start));
	dpMap.insert(pair<int, position>(dpIndx++,end));
	int n = optItems.size();

	for(int i = 0; i < n; i++){
		position loc = grid.getPos(optItems.front());
		optItems.pop();
	//	dpMap.insert(pair<position, int>(loc, dpIndx++));

		position loc_l = {loc.x - 1, loc.y};
		position loc_r = {loc.x + 1, loc.y};

		//printf("L (%i,%i)\n", loc_l.x, loc_l.y);
		//printf("R (%i,%i)\n", loc_r.x, loc_r.y);
	//	if(dpMap.find(loc_l) == dpMap.end()){
			dpMap.insert(pair<int, position>(dpIndx++,loc));
	//	}
//		if(dpMap.find(loc_r) == dpMap.end()){
		//	dpMap.insert(pair<int, position>(dpIndx++,loc_r));
//		}

	}
/*  //for debugging dpMap
	map<position, int>:: iterator it = dpMap.begin();
	for(; it != dpMap.end(); it++){
		printf("(%i,%i) %i\n", it->second.x, it->second.y, it->first);
	}
*/
	for(int i = 0; i < dpIndx; i++){
		for(int j = 0; j < dpIndx; j++){
			dp[i][j] = -1;
		}
	}
	//grid.nextPosBound(dpMap.find(0)->second,dpMap.find(2)->second);
	//printf("min %i\n", grid.nextPosBound(dpMap.find(0)->second,dpMap.find(2)->second));

	for(int i = 0; i < dpIndx; i++){
		position cur = dpMap.find(i)->second;
		for(int j = 0; j < dpIndx; j++){
			position dest = dpMap.find(j)->second;
			
		//	printf("cur(%i,%i) at %i %i : ", cur.x, cur.y,i,j);
		//	printf("dest(%i,%i) at %i %i\n", dest.x, dest.y,i,j); 
			dp[i][j] = grid.nextPosBound(cur, dest); // 
		}
	} 
	 //for dbg dp
	for(int i = 0; i < dpIndx; i++){
		for(int j = 0; j < dpIndx; j++){
			printf("[%i][%i] = %i ", i,j,dp[i][j]);
		}
		printf("\n");
	}  
	int scr = -1;
	int dest = -1;
	int dist = reduce(scr, dest, true);
	//excludes final destination
	printf("lower bound = %i\n", dist);

	for(int i = 0; i < dpIndx; i++){
		for(int j = 0; j < dpIndx; j++){
			printf("[%i][%i] = %i ", i,j,dp[i][j]);
		}
		printf("\n");
	}   
	scr = 0;
	dest = 0;

	int minCost[dpIndx];
	for(int i = 0; i < dpIndx; i++)
		minCost[i] = 100000;

	for(int s = 0; s < dpIndx; s++){
		scr = dest;
		
		for(int i = 0; i < dpIndx; i++){
			minCost[i] = reduce(scr, i, false);
			printf("minCost %i\n", reduce(scr, i, false));
		}
		
		//find path with least cost at this level
		for(int i = 0; i < dpIndx; i++){
			int min = 10000;
	
			if(minCost[i] < min) {
				min = minCost[i];
				dest = i;
			}
		}
		dist += reduce(scr, dest, true) + dp[scr][dest];  
	}
	for(int i = 0; i < dpIndx; i++){
		for(int j = 0; j < dpIndx; j++){
			printf("[%i][%i] = %i ", i,j,dp[i][j]);
		}
		printf("\n");
	}  
	printf("upper bound = %i\n", dist);
}
int Parser::reduce(int src, int dest, bool update)
{
	int cost= 0;

	//copy dp
	int temp[MAX][MAX];
	for(int i = 0; i < dpIndx; i++){
		for(int j = 0; j < dpIndx; j++){
			temp[i][j] = dp[i][j];
		}
	}

	//scr row == INF
	if(src != -1){
		for(int i = 0; i < dpIndx; i++)
			temp[src][i] = INF;		
	}
	//dest col == INF
	if(dest!= -1){
		for(int i = 0; i < dpIndx; i++)
			temp[i][dest] = INF;		
	}

	//reduce along row
	int min = INF;
	for(int i = 0; i < dpIndx; i++){
		min = INF;
		for(int j = 0; j < dpIndx; j++){
			if(temp[i][j] < min)
				min = temp[i][j];
		}
		cost += min;
		for(int j = 0; j < dpIndx; j++){
			temp[i][j] -= min;
		}
	}

	//reduce along col
	for(int i = 0; i < dpIndx; i++){
		min = INF;
		for(int j = 0; j < dpIndx; j++){
			if(temp[j][i] < min)
				min = temp[j][i];
		}
		cost += min;
		for(int j = 0; j < dpIndx; j++){
			temp[j][i] -= min;
		}
	}

	if(update){
		for(int i = 0; i < dpIndx; i++){
			for(int j = 0; j < dpIndx; j++){
				dp[i][j] = temp[i][j];
			}
		}
	}
	return cost;
}

int Parser::getWidth()
{
    return warehouseWidth;
}
int Parser::getHeight()
{
    return warehouseHeight;
}
bool Parser::getAll()
{
	return all;
}
bool Parser::getBoundCheck()
{
	return boundCheck;
}