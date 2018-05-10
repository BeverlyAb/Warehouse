#include "MST.h"
#include <iostream>


MST::MST()
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

}

void MST::setUserParam()
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
}

void MST::readFile(int fileType)
{
	unsigned int x = 0;
	unsigned int y = 0;

    ifstream myFile;
    myFile.open(inFile.c_str());

	if(fileType == ORDER_FILE){ 
		int indx;
		printf("Name of warehouse order file?\n");
		cin >> inFile;
		printf("Give an order list number or type '101' to read the entire file\n");
		cin >> indx;
		getOrder(inFile,indx); //make sure to form cluster here too		
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


void MST::getOrder(string file, int index)
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

		for(int j = 0; j < n; j++){

			for(int k = 0; k < orderFile[j].size(); k++){
				namedItems.push(orderFile[j][k]);
			}
			getPath();
		}
	} else{
		//transfer only a line
		for(int j = 0; j < orderFile[n].size(); j++){
			namedItems.push(orderFile[n][j]);  
		} 
		getPath(); 
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

void MST::getPath()
{
	if(!grid.isValid(start)) {
		printf("Starting position is either out of bounds or starts on a shelf.\n");
		return;
	}else {
		printf("---------------------------------------------------------------------\n");
		printf("Order\t\t Distance\t Path\n");
		if(!namedItems.empty()){

			unsigned int tempID = namedItems.front();
			printf("%i\t\t",tempID);
			grid.nextPos(start, grid.getPos(tempID));
			int totalDist = grid.getTotalDist();
			namedItems.pop();

			position next = grid.getFinalDest();
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
		}
	}
}

/*
	So far only splits into 4 quadrants, but orders the request based on which quad.
	has the most requests. Don't need cluster anymore. Only works for even width and height so far
*/

int MST::getWidth()
{
    return warehouseWidth;
}
int MST::getHeight()
{
    return warehouseHeight;
}
