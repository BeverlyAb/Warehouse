#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
// grep -n "INDEX 1000" bigBatchProcess.txt | cut -d : -f 1
// 53787 = 1000
// 233 = 10

//prints coord. to make lines
void lineMaker(const vector<int> x, const vector<int> y)
{
  for(int i = 0; i < x.size(); i++){
    printf("%i %i\n", x[i], y[i]);
  }
}

int main(int argc, char *argv[])
{
  if(argc != 3){
    printf("input file, ind.txt\n");
    return 1;
  }
  
  string fileName =  argv[1];
  string indName = argv[2];
  
  string ans = "";
  // printf("Index?\n");
  // cin >> ans;
  // printf("%s\n",ans.c_str());
  // int lineNum = atoi(ans.c_str());
  ifstream indFile;
  indFile.open(indName.c_str());
  indFile >> ans;

  int lineNum = atoi(ans.c_str());
  vector<int> xPt;
  vector<int> yPt;

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
     while(whole.substr(0,5) != "Total"){
      getline(myFile, whole);
    //   printf("%s\n",whole.substr(0,5).c_str());
    }
   
    
  //   //parse for coord. until reaches next index
    while(whole.substr(0,5) != "INDEX" && myFile.good()){
        getline(myFile, whole); 
        int pos = 0;
        int n = whole.size();

        while(pos < n){
          if(whole[pos] == '('){
            x = whole[pos + 1];
            
            if(whole[pos + 2] != ',')
              x += whole[pos +2];

            xPt.push_back(atoi(x.c_str()));
          //  printf("%s ", x.c_str());
          }
          if(whole[pos] == ','){
            y = whole[pos + 1];
            
            if(whole[pos + 2] != ')')
              y += whole[pos +2];
            yPt.push_back(atoi(y.c_str()));
          //  printf("%s\n", y.c_str());
          }
          pos++;
        }
    }
    myFile.close();
  }
  else{
    printf("%s could not be opened.\n",fileName.c_str()); 
    return 2;
  } 

  lineMaker(xPt, yPt);
  return 0;
}