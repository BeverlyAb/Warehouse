#include "Parser.h"
#include "Mapper.h"

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

	printf("Does this file need calibration? Y or N?\n");
   	 	cin >> ans; 
	if(ans == "Y") {
        outFile = inFile.substr(0,inFile.size()-4) + "_modified.csv";
        printf("A file with these parameters has been created as %s\n",outFile.c_str());
    } 
    else if(ans == "N")
        return;
    else {   
        printf("Please answer with Y or N after retyping the file name.\n");
        setUserParam();
    }
}
void Parser::readFile()
{
    if (ans == "Y")
    {
        ifstream myFile;
        ofstream newFile;
		int x = 0, y = 0;
        myFile.open(inFile.c_str());
        newFile.open(outFile.c_str());
        if(myFile.is_open() && newFile.is_open()){
            
            getline(myFile,ID,',');
            while(myFile.good() && !ID.empty()){
                getline(myFile,xCoord,',');
                getline(myFile,yCoord,'\n');
                
                //change to stod
                x = atoi(xCoord.c_str());
                y = atoi(yCoord.c_str());
				x *= mutator;
				y *= mutator;

                //move to shelving platform of (0,0)
                if(x == 0)
                    x += 1; 
                if(y == 0)   
                    y += 1;

                newFile << ID << ',' << x  << ',' << y  << '\n';
                getline(myFile,ID,',');
            }
            myFile.close();
            newFile.close();
			
			inFile = outFile;					
			ans = "N";
			readFile();
        }
        else  
            printf("Files could not be opened\n");
    } else{ //file is ready to be mapped as is
        ifstream myFile;
        myFile.open(inFile.c_str());

        Mapper grid = Mapper(warehouseWidth, warehouseHeight);

        if(myFile.is_open()){
            getline(myFile,ID,',');
            while(myFile.good()){
                getline(myFile,xCoord,',');
                getline(myFile,yCoord,'\n');
                grid.makeMap(atoi(ID.c_str()), atoi(xCoord.c_str()), atoi(yCoord.c_str()));

                getline(myFile,ID,',');
            }
            myFile.close();
            
            unsigned int x = 0;
            unsigned int y = 0;
            unsigned int ID = 0;

            cout << "Starting x-coord?\n";
            cin >> x;
            cout << "Starting y-coord?\n";
            cin >> y;
            position start = {x,y};

            cout << "Product ID?\n";
            cin >> ID;
			
			if(grid.isValidStart(start))
            	grid.nextPos(start, ID);
			else{
				printf("Starting position is either out of bounds or starts on a shelf.\n");
				printf("Check with the 'modified' version of the file. Ending program.\n");
				return;
			}			
       //     grid.printPath();
        }
        else  
            printf("%s could not be opened\n",inFile.c_str());
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

