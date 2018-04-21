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

	/*vector<unsigned int> row;
	for(int j = 0; j < ROW; j++){
		for(int i = 0; i < MAX_COL; i++)
			row.push_back(0);

		orderFile.push_back(row);
	}*/
	orderFile.resize(ROW*MAX_COL);

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
	else if(fileType == ORDER_FILE){ 
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

void Parser::getNameItem()
{
	printf("Name an item ID or type 'done'\n");
	cin >> ans;
	int i = 0;
	//not very robust
	while(ans != "done"){
		namedItems.push(atoi(ans.c_str()));
		optItems.push(atoi(ans.c_str()));
		makeCluster(atoi(ans.c_str()));
		cin >> ans;
	}
}

void Parser::getOrder(string file, int index)
{
	int n = index;
	int  i = 0;
	int start = 0;
	ifstream myFile;
    myFile.open(file.c_str());
	if(myFile.is_open()){
		getline(myFile,ID,' ');
		printf("%s",ID.c_str()); 

       while(myFile.good()){
	
		/*	int pos = ID.find(' ');
			i++;
			while(pos != string::npos){
				string sub = ID.substr(start,pos);
				orderFile[i].push_back(atoi(sub.c_str()));

				getline(myFile,ID,' ');
				//printf("%i %i\n",i, orderFile[0][i]); 
				start = pos;
				ID = ID.substr(pos);
				pos = ID.find(' ');
			}
        }  */
			getline(myFile,ID,' ');
			printf("%s", ID.c_str());
        } 

/*		getline(myFile,ID);
		int i = 0;
		int stop = 0;
		while(myFile.good() && !ID.empty()){
			int pos = ID.find("\t");
			int start = 1;
			i++;
			int n = ID.length() -1;
			while(pos < ID.length() && stop < 10){
				stop++;
				string sub = ID.substr(start,pos);
				ID = ID.substr(start, n);

				orderFile[i].push_back(atoi(sub.c_str()));
				printf("%i %s\n", i, ID.substr(start,pos).c_str());
				start = pos;
				pos = ID.find("\t"); */
				
			/*	if(getline(myFile,ID,'\n')) {
					i++;
					getline(myFile,ID,'\n');
				} 
				else
					getline(myFile,ID,'\t'); */
		
		myFile.close(); 
	}
	else
	    printf("%s could not be opened\n",file.c_str()); 
/*
	//read entire order list if not in range
	if(n < 0 || n > orderFile.size() ) {
		n = orderFile.size();
		for(int j = 0; j < n; j++){

			for(int k = 0; k < orderFile[j].size(); k++){
				namedItems.push(orderFile[j][k]);
				optItems.push(orderFile[j][k]);
			}
		}
	}
*/	//transfer only a line
/*	for(int j = 0; j < orderFile[n].size(); j++){
		namedItems.push(orderFile[n][j]);  
		optItems.push(orderFile[n][j]);
	} */
}

void Parser::getPath()
{
	if(!grid.isValid(start)) {
		printf("Starting position is either out of bounds or starts on a shelf.\n");
		return;
	}else {
		printf("Order\t\t Distance\t Path\n");
		if(!namedItems.empty()){
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
	queue<unsigned int> quad1;
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

int Parser::getWidth()
{
    return warehouseWidth;
}
int Parser::getHeight()
{
    return warehouseHeight;
}
