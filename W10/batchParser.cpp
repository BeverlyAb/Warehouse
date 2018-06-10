#include <iostream>
#include <fstream>
#include <string>

using namespace std;
// grep -n "INDEX 1000" bigBatchProcess.txt | cut -d : -f 1
// 53787 = 1000
// 233 = 10
int main(int argc, char *argv[])
{
  if(argc != 3){
    printf("input file, line number\n");
    return 1;
  }
  
  string fileName =  argv[1];
  int lineNum = atoi(argv[2]);
  
  ifstream myFile;
  myFile.open(fileName.c_str());
  
  string whole ="";
  string x = ""; string y = "";

  if(myFile.is_open()){
    //jump to line with index
    for(int i = 0; i < lineNum; i++ ){
      getline(myFile, whole);
    } 
    // skips first "---------------------------------------------------------------------"
    getline(myFile, whole);

    getline(myFile, whole);
    while(whole == "---------------------------------------------------------------------"){
      getline(myFile, whole);
    }
    // Output so far : "Order            Distance        Weight                          Effort                   Path"
    
    //parse for coord. until reaches next index
   // while(whole.substr(0,5) != "INDEX"){
      getline(myFile, whole);
     
      //'find' doesn't work for char? :/
      int pos = 0;
      int n = whole.length();
      while(pos < n){
        if(whole[pos] == '('{
          x = whole[pos+1];
          y = whole[pos+3];
        }
        pos++;
        printf("x = %s y = %s\n", x.c_str(), y.c_str());
        string temp = whole.substr(pos);
        //printf("%s\n", temp.c_str());
       //couldn't simply offset y by 3 
     // }
      }
        
     // y = whole.substr(pos + 2, pos + 3);
     // printf("%s\n", whole.c_str());
      
  //  }

    myFile.close();
  }
  else{
    printf("%s could not be opened.\n",fileName.c_str()); 
    return 2;
  } 
  return 0;
}