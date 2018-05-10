#include "Parse.h"
/*
#define STOCK 0
#define NAME_ITEM 1
#define ORDER_FILE 2
#define BB 3
#define QUAD 4 */
/*
        string inFile;
        unsigned int width, height;
        int execute;
        Layout grid;
*/

//default constructor
Parse::Parse()
{
  inFile = "";
  width = 0; height = 0;
  execute = -1;
  orderIndx = 0;
  //grid = Layout();
}

//constructor with param
Parse::Parse(string file, unsigned int w, unsigned int h)
{
  infile = file;
  width = w;
  height = h;
  orderIndx = 0;
 // grid = Layout(width, height);
}

//used to decide what to execute Stock(0), NAME_ITEM(1), BB(3), QUAD(4)
void Parse::setAction(int act)
{
  execute = act;
}

int Parse::getAction()
{
  return execute;
}

//reads the stock file or order file
void Parse::readFile()
{
	unsigned int x = 0;
	unsigned int y = 0;

  ifstream myFile;
  myFile.open(inFile.c_str());

  if(act == NAME_ITEM){
		getNameItem();
	
  } else if(act == ORDER_FILE){
  		int indx = 0;
		  printf("Name of warehouse order file?\n");
		  cin >> inFile;
      printf("Give an order list number or type '101' to read the entire file\n");
      cin >> indx;
      getOrder(inFile,indx, fileType); //make sure to form cluster here too		
	
  } else if(act == STOCK){
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
		printf("Invalid choice between NAME ITEM(1) or ORDER_FILE (2).\n");
	}
}
void Parse::writeToFile(string)
{

}
//lets user to manually input items
void Parse::getNameItem()
{
	printf("Name an item ID or type 'done'\n");
	cin >> ans;
	//not very robust
	while(ans != "done"){
		namedItems.push(atoi(ans.c_str()));
		optItems.push(atoi(ans.c_str()));
		cin >> ans;
	}
}

void Parser::getOrder(string file, int index)
{
	int orderIndx = index;
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
      } 
      else {
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
}
void Parse::processOrder()
{	//read entire order list if not in range
	if(orderIndx < 0 || orderIndx > ROW || orderIndx == 101) {
		int n = ROW;
    
		for(int j = 0; j < n; j++){

			for(int k = 0; k < orderFile[j].size(); k++){
				namedItems.push(orderFile[j][k]);
				optItems.push(orderFile[j][k]);
			}
		/*	if(execute == QUAD){
				getPath();
				opt();
				getPath();
			} else if (execute == BB){
				branchBound();
			} else{
        printf("Invalid choice. Choose BB(3) or Quad (4). Terminating program\n");
        return;
      }*/
		}
	} else{
		//transfer only a line
		for(int j = 0; j < orderFile[n].size(); j++){
			namedItems.push(orderFile[n][j]);  
			optItems.push(orderFile[n][j]);
		} 
	/*	if(fileType == BOUND){
			getPath();
			branchBound();
			getPath();
		} */
	}
}
void Parse::writeToFile(string outFile)
{
  
}
void Parse::BB()
{

}
void Parse::Quad()
{

}