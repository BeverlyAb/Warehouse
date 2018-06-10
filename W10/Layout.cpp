#include "Layout.h"
#include <iostream>
#include <sstream>
 /*  
    string infile;
    unsigned int width, height;
    map<unsigned int, position> stock; //reads via ID
    map<position, unsigned int> shelf; //reads via position and # of items in shelf
  	
    position start,end;
  	queue<unsigned int> orgItems;
  	queue<unsigned int> optItems;    
    */

Layout::Layout()
{
  infile = "";
  width = 0; height  = 0;
  start.x = 0; start.y = 0;
  end.x = 0; end.y = 0;
  orderFile.resize(ROW*COL);
}
Layout::Layout(unsigned int w, unsigned int h, string in, position s, position e)
{
  infile = in;
  width = w * 2; //double the scale to allow for walkways
  height = h * 2;
  start = s;
  end = e;
  orderFile.resize(ROW*COL);
}
void Layout::readFile(int fileType, string in)
{
  infile = in;
  if(fileType == STOCK)
		makeStock(infile);
	
  else if(fileType == NAME_ITEM){
		getNameItems();
  }
	else if(fileType == ORDER_FILE){ 
		int indx = 0;
		printf("Give an order list number or type '101' to read the entire file\n");
		cin >> indx;

		getListItems(infile,indx); 	
	} 
  else 
		printf("Invalid choice between STOCK(0), NAME_ITEM(1), and ORDER_FILE(2)\n");
}
void Layout::makeStock(string in)
{
  unsigned int x = 0;
	unsigned int y = 0;
  
  string ID, xCoord, yCoord;
  ID = ""; xCoord = ""; yCoord = "";
  infile = in;
  ifstream myFile;
  myFile.open(infile.c_str());
	
  if(myFile.is_open()){
    getline(myFile,ID,',');
		
    while(myFile.good() && !ID.empty()){
		  getline(myFile,xCoord,',');
		  getline(myFile,yCoord,'\n');
    
			x = atoi(xCoord.c_str()); y = atoi(yCoord.c_str());
			x *= 2; y *= 2;

			if(x == 0) x++;
			if(y == 0) y++;
      
		  stockSingleItem(atoi(ID.c_str()), x, y);
			getline(myFile,ID,',');
		}
		    
	  myFile.close();
  }
  else{
    printf("%s could not be opened\n",infile.c_str());
    return;
  }
}
void Layout::stockSingleItem(unsigned int ID, unsigned int xCoord, unsigned int yCoord)
{
  position itemLoc = {xCoord, yCoord};
  stock.insert(pair<unsigned int, position>(ID,itemLoc));

  /* //is shelf already in use?
  if(shelf.find(itemLoc) != shelf.end()){
		
    unsigned int counter = shelf.find(itemLoc)->second + 1;
    map<position,unsigned int>::iterator it = shelf.find(itemLoc);
		it->second = counter;
  }
	else*/
    shelf.insert(pair<position, bool>(itemLoc, false));
}
void Layout::getNameItems()
{
  string ans;
	printf("Name an item ID or type 'done'\n");
	cin >> ans;
	//not very robust
	while(ans != "done"){
		orgItems.push(atoi(ans.c_str()));
		optItems.push(atoi(ans.c_str()));
		//makeCluster(atoi(ans.c_str()));
		cin >> ans;
	}
}
void Layout::getListItems(string in, int index)
{
  int i = 0;
  int n = index;
  string IDs = ""; // change to ID for warehouse.csv
  infile = in;
  ifstream myFile;
  myFile.open(infile.c_str());

  //works for bigWarehouse.txt
  if(myFile.is_open()){
   while(myFile.good() && getline(myFile, IDs, '\n')){
    std::string ID;                 // Have a buffer string
    std::stringstream ss(IDs);       // Insert the string into a stream
    while (ss >> ID)
      orderFile[i].push_back(atoi(ID.c_str()));
    i++;
    myFile.close();
    }
  }	else{
    printf("%s could not be opened.\n",infile.c_str()); 
    return;
  }  

  //works for warehouse.csv text formatting issues
  // if(myFile.is_open()){
	// 	myFile >> ID;
  //   while(myFile.good() && !ID.empty()){
  //     if(ID[ID.length()-1] == '"'){
  //       i++;
  //       myFile >> ID;
  //     }
  //     else {
  //     //remove trailing front ' " '
  //       if(ID[0]== '"'){
  //         ID.erase(ID.begin());
  //       }
  //       orderFile[i].push_back(atoi(ID.c_str()));
  //       myFile >> ID;
  //     }
  //   }
  //   myFile.close();
  // }	else{
	//   printf("%s could not be opened.\n",infile.c_str()); 
  //   return;
  // }  
 // printf("size = %i \n",orderFile.size());
    //  for(int j = 0; j < 2500; j++){
    //   printf("item %i at %i\n",orderFile[j][1],j);  
    // } 
} 
void Layout::processSingleOrder(int indx)
{
  int n = indx;
  for(int j = 0; j < orderFile[n].size(); j++){
    orgItems.push(orderFile[n][j]);  
    optItems.push(orderFile[n][j]);
	} 
  /*call 
  printPath();
  algorithm(), which sorts the order
  printPath();*/
  
}
void Layout::printStock()
{
 /* map<unsigned int, position> ::iterator it = stock.begin();
  for(; it != stock.end(); it++)
    printf("%i\n", it->first);

  printf("size %lu\n", stock.size()); */
  map<position, bool> ::iterator it = shelf.begin();
  for(; it != shelf.end(); it++)
    printf("shelf(%i, %i)\n", it->first.x, it->first.y);

  printf("size %lu\n", shelf.size());
}

  void Layout::resetShelf()
  {
    map<position, bool>::iterator it = shelf.begin();
    for(; it != shelf.end(); it++)
      it->second = false;

  }