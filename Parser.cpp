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
}

void Parser::setUserParam()
{   
    printf("What is the file name?\n");
    cin >> inFile;

    printf("What is the max warehouse width?\n");
    cin >> warehouseWidth;
   	printf("What is the max warehouse height?\n");
    cin >> warehouseHeight;
 
    warehouseHeight *= 2;
    warehouseWidth *= 2;
	
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
					x *= 2; y *= 2;
				
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
		printf("Order\t\t Distance\t Path\n");
		unsigned int tempID = namedItems.front();
		printf("%i\t\t",tempID);
		grid.nextPos(start, grid.getPos(tempID));
		namedItems.pop();	

		position next = grid.getFinalDest();
		int n = namedItems.size();		
		for(int i = 0; i < n ; i++){
			tempID = namedItems.front();
			printf("%i\t\t",tempID);
			grid.nextPos(next,grid.getPos(tempID));
			namedItems.pop();
			next = grid.getFinalDest();			
		} 
		printf("end\t\t");
		grid.nextPos(next,end);
		//last move, otherwise it ends at a neighbor of end
		//printf("(%i,%i)\n",end.x,end.y); //should hop++ 
	}
	printf("Total Distance : %i\n", grid.getDist());
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
	has the most requests. Don't need cluster anymore. Only works for even width and height so far
*/
void Parser::opt(unsigned int granularity)
{
	//can try recursive call for granularity 
	unsigned int in = granularity;
	unsigned int quadNum = 4*in;
	unsigned int xRangeStart[quadNum];// = {warehouseWidth/(2*in) + 1, 0, 0, warehouseWidth/(2*in) + 1};
	unsigned int xRangeEnd[quadNum];// = {warehouseWidth, warehouseWidth/(2*in),
								// warehouseWidth/(2*in), warehouseWidth};
	
	unsigned int yRangeStart[quadNum];// = {warehouseHeight/(2*in) + 1, warehouseHeight/(2*in) + 1, 0, 0};
	unsigned int yRangeEnd[quadNum];// = {warehouseHeight, warehouseHeight,
								// warehouseHeight/(2*in), warehouseHeight/(2*in)};

	queue<unsigned int>  quad[quadNum];

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
	namedItems = optItems;
	/*n = optItems.size();
	for(int i = 0; i < n; i++){
		namedItems.push(optItems.front());
		optItems.pop();
	}*/
}//opt

int Parser::getWidth()
{
    return warehouseWidth;
}
int Parser::getHeight()
{
    return warehouseHeight;
}

