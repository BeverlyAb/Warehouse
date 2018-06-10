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

    while(whole != "---------------------------------------------------------------------"){
      getline(myFile, whole);
    }
    getline(myFile, whole);
    // Output so far : "Order            Distance        Weight                          Effort                   Path"
    printf("%s\n", whole.c_str());
    
    //parse for coord. until reaches next index
   // while(whole.substr(0,5) != "INDEX"){
        getline(myFile, whole); 
        printf("whole %s\n", whole.c_str());
        int pos = 0;
        int n = whole.size();
        printf("size = %i\n", n);
        int end = 0;
        while(pos < n && end < 15){
          end++;
          if(whole[pos] == '('){
            x = whole[pos + 1];
            
            if(whole[pos + 2] != ','){
              x += whole[pos + 2];
              y = whole[pos +3];
              
              if(whole[pos + 4] != ')')
                y += whole[pos +4];
            }
            else{
              y = whole[pos + 3];
              
              if(whole[pos + 4] != ')'){
                y += whole[pos +4];
                pos += 4;
              } 
              else
                pos += 3;
            }

            printf("x = %s y = %s\n", x.c_str(), y.c_str());
          }
        }
        //printf("pos = %i\n", pos);
   // }
    myFile.close();
  }
  else{
    printf("%s could not be opened.\n",fileName.c_str()); 
    return 2;
  } 
  return 0;
}