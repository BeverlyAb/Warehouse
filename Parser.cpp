#include "Parser.h"
#include <iostream>

Parser::Parser()
{
    inFile = "";
    outFile = "";
    warehouseWidth = 0;
    warehouseHeight = 0;
    ans = "";
    mutator = 1;

    ID = "";
    xCoord = "";
    yCoord = "";
	grid = Mapper();
}

void Parser::setUserParam()
{   
    printf("What is the file name?\n");
    cin >> inFile;

    printf("What is the max warehouse width?\n");
    cin >> warehouseWidth;
   	printf("What is the max warehouse height?\n");
    cin >> warehouseHeight;
    printf("By what factor should we scale the map?\n");
    cin >> mutator;
    warehouseHeight *= mutator;
    warehouseWidth *= mutator;
	
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
	else if(fileType == STOCK || fileType == ORDER_FILE){
	
		if(myFile.is_open()){
		    getline(myFile,ID,',');
		    while(myFile.good() && !ID.empty()){
		        getline(myFile,xCoord,',');
		        getline(myFile,yCoord,'\n');
				
				if(fileType == STOCK){
					x = atoi(xCoord.c_str()); y = atoi(yCoord.c_str());
					x *= mutator; y *= mutator;
				
					if(x == 0) x++;
					if(y == 0) y++;
				
		        	grid.makeStock(atoi(ID.c_str()), x, y);			
					getline(myFile,ID,',');
				}else if(fileType == ORDER_FILE){
					return;//grid.getOrder(); make sure to form cluster here too
				} 
		      
		    }
		    myFile.close();	
		} 
		else  
		    printf("%s could not be opened\n",inFile.c_str());
	}
	else{
		printf("Something went wrong with fileType\n");
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
		makeCluster(atoi(ans.c_str()));
		cin >> ans;
	}
}

void Parser::getPath()
{
	if(!grid.isValid(start)) {
		printf("Starting position is either out of bounds or starts on a shelf.\n");
		return;
	}else {	
	
		unsigned int tempID = namedItems.front();
		grid.nextPos(start, grid.getPos(tempID));
		namedItems.pop();	

		position next = grid.getFinalDest();
		int n = namedItems.size();		
		for(int i = 0; i < n ; i++){
			tempID = namedItems.front();
			grid.nextPos(next,grid.getPos(tempID));
			namedItems.pop();
			next = grid.getFinalDest();			
		} 
		grid.nextPos(next,end);
		//last move, otherwise it ends at a neighbor of end
		printf("(%i,%i)\n",end.x,end.y); //should hop++ 
	}
}
void Parser::makeCluster(unsigned int ID)
{
	position itemLoc = grid.getPos(ID);
    if(cluster.find(itemLoc) != cluster.end()){
		unsigned int counter = cluster.find(itemLoc)->second + 1;
      	map<position,unsigned int>::iterator it = cluster.find(itemLoc);
		it->second = counter;
    } 
	else{
       	cluster.insert(pair<position, unsigned int>(itemLoc, 1));
	}
}

/*
	So far only splits into 4 quadrants, but orders the request based on which quad.      
	has the most requests. Don't need cluster anymore
*/
void Parser::opt()
{
	unsigned int xRangeStart[4] = {warehouseWidth/2 + 1, 0, 0, warehouseWidth/2 + 1};
	unsigned int xRangeEnd[4] = {warehouseWidth, warehouseWidth/2,
								 warehouseWidth/2, warehouseWidth};
	
	unsigned int yRangeStart[4] = {warehouseHeight/2 + 1, warehouseHeight/2 + 1, 0, 0};
	unsigned int yRangeEnd[4] = {warehouseHeight, warehouseHeight,
								 warehouseHeight/2, warehouseHeight/2};
	position temp = {0,0};
	queue<position> * quad;// = {temp};
	//might be able to use OpenMP if do nested For Loop with i. Will it be worth it?
	//map<position,unsigned int>::iterator it = cluster.begin();
	printf("optItems %i\n",optItems.size());
	for(int l = 0; l < optItems.size(); l++){
		unsigned int tempID = optItems.front();
		optItems.pop();
		if(grid.getPos(tempID).x >= xRangeStart[0] && grid.getPos(tempID).x <= xRangeStart[0]
		&& grid.getPos(tempID).y >= yRangeStart[0] && grid.getPos(tempID).y <= yRangeEnd[0]){
			quad[0].push(grid.getPos(tempID));
			printf("(q 0 %i,%i)\n",grid.getPos(tempID).x,grid.getPos(tempID).y);
		}
		else if(grid.getPos(tempID).x >= xRangeStart[1] && grid.getPos(tempID).x <= xRangeStart[1]
		&& grid.getPos(tempID).y >= yRangeStart[1] && grid.getPos(tempID).y <= yRangeEnd[1]){
			quad[1].push(grid.getPos(tempID));
			printf("(q 1 %i,%i)\n",grid.getPos(tempID).x,grid.getPos(tempID).y);
		}
		else if(grid.getPos(tempID).x >= xRangeStart[2] && grid.getPos(tempID).x <= xRangeStart[2]
		&& grid.getPos(tempID).y >= yRangeStart[2] && grid.getPos(tempID).y <= yRangeEnd[2]){
			quad[2].push(grid.getPos(tempID));
			printf("(q 3 %i,%i)\n",grid.getPos(tempID).x,grid.getPos(tempID).y);
		}
		else if(grid.getPos(tempID).x >= xRangeStart[3] && grid.getPos(tempID).x <= xRangeStart[3]
		&& grid.getPos(tempID).y >= yRangeStart[3] && grid.getPos(tempID).y <= yRangeEnd[3]){
			quad[3].push(grid.getPos(tempID));
			printf("(q 3 %i,%i)\n",grid.getPos(tempID).x,grid.getPos(tempID).y);
		}
		else {
			printf("Something wrong with opt()\n");	
			return;
		}
	}
	int count[4] = {0};
	for(int i = 0; i < 4; i++){
		count[i] = quad[i].size();
		printf("count = %i, size for q[%i] = %i\n",count[i] ,i,quad[i].size());
	}
	sort(count,count + 4);	
	
	for(int i = 3; i >= 0; i--){
		if(quad[0].size() == count[i]){
			for(int j = 0; j < quad[0].size(); j++){
				position tempPos = quad[0].front();
				printf("temp(%i,%i)\n",tempPos.x,tempPos.y);
				//namedItems.push(tempPos);
				//quad[0].pop();
			}
		}
	}
 	
}//opt

int Parser::getWidth()
{
    return warehouseWidth;
}
int Parser::getHeight()
{
    return warehouseHeight;
}

