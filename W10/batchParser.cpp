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
    while(whole != "---------------------------------------------------------------------"){
      getline(myFile, whole);
    }
    getline(myFile, whole);
    // Output so far : "Order            Distance        Weight                          Effort                   Path"
    printf("%s\n", whole.c_str());
    
    int end = 0;
    //parse for coord. until reaches next index
    while(whole.substr(0,5) != "INDEX"){
        getline(myFile, whole);
        size_t pos = whole.find("(");
        size_t comma = whole.find(",");
        int  n = whole.length();
        while(pos != string::npos && pos < n && end < 30){
          end++;
          x = whole.substr(whole.find("(") +1,;
          printf("x = %s\n", x.c_str());
        }
    }
    myFile.close();
  }
  else{
    printf("%s could not be opened.\n",fileName.c_str()); 
    return 2;
  } 
  return 0;
}