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
				//	printf("stock(%i,%i)\n",grid.getPos(atoi(ID.c_str())).x,grid.getPos(atoi(ID.c_str())).x);			
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
		grid.makeCluster(atoi(ans.c_str()));
		cin >> ans;
	}
}

void Parser::getPath()
{
	if(!grid.isValid(start)) {
		printf("Starting position is either out of bounds or starts on a shelf.\n");
		return;
	}else {	
	/*	position zero = {0,0};
		position one = {2,2};
		position two = {4,2};
		position three = {0,3};

		grid.nextPos(zero,one);
		//grid.printPath(zero,one);
		//one.x = 1;
		grid.nextPos(one,two);
		//two.x = 3;
		grid.nextPos(two,three); */
		unsigned int tempID = namedItems.front();
		grid.nextPos(start, grid.getPos(tempID));
		namedItems.pop();	

		position next = grid.getFinalDest();
	//	printf("final(%i,%i)\n",next.x,next.y);
		int n = namedItems.size();		
		for(int i = 0; i < n ; i++){
			tempID = namedItems.front();
		/*	printf("nameFront %i\n",namedItems.front());
			printf("start (%i,%i)\n",next.x,next.y);
			printf("dest(%i,%i)\n",grid.getPos(tempID).x,grid.getPos(tempID).y);
			next.x = 1; next.y = 2;
			position other = {4,2};
			*/grid.nextPos(next,grid.getPos(tempID));
		//	grid.nextPos(next,other);
			namedItems.pop();
			next = grid.getFinalDest();			
		} 
		grid.nextPos(next,end); 
	}
}

int Parser::getWidth()
{
    return warehouseWidth;
}
int Parser::getHeight()
{
    return warehouseHeight;
}

