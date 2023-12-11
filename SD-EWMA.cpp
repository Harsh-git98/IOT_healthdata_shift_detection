#include<iostream>
#include<vector>
#include <cmath>
#include <fstream>
#include <sstream>

using namespace std;
# define BETA 0.1
# define lamda 0.08
# define zeta 0.01
#define L 3
struct CSVRow {
    int column1;
    int column2;
    int column3;
};
// 55 90 170 

CSVRow parseCSV(const string& line) {
    CSVRow row;
    stringstream ss(line);
    char comma; // to read the comma
    ss >> row.column1 >> comma >> row.column2 >> comma >> row.column3;
    return row;
}


int main()
{
   string filename = "data.csv";

    ifstream file(filename);

    
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return 1; 
    }

    double time[200]={0};
    double heartbeat[200]={0};
    double num[200]={0};

    string line;
    int p=0;
    while (getline(file, line)) {
        
        CSVRow row = parseCSV(line);
        num[p]=row.column1;
        time[p]=row.column2;
        heartbeat[p]=row.column3;
        p++;
        
    }

    
    file.close();
    

    // training phase for 50
    double mean=0;
    for(int i=0;i<5;i++)
    {
        mean=heartbeat[i]+mean;

    }
    mean=mean/5;
    double z[200]={0};
    z[0]=mean;
    double error[200]={0};
    for(int i=1;i<=5;i++)
    {
        z[i]=lamda*heartbeat[i]+(1-lamda)*z[i-1];
        error[i]=heartbeat[i]-z[i-1];
    }
    

    double variance=0;

    for(int i=1;i<5;i++)
    {
        variance=variance+(error[i]*error[i])/5;
    }


    double var[200]={0};
    var[5]=variance;
    double ucl[200]={0};
    double lcl[200]={0};
  
    for(int i=6;i<200;i++)
    {
        z[i]=lamda*heartbeat[i]+(1-lamda)*z[i-1];
        error[i]=abs(heartbeat[i]-z[i-1]);
        var[i]=abs(zeta*error[i]*error[i]+(1-zeta)*var[i-1]);
        ucl[i]=z[i-1]+L*abs((abs(var[i-1])));
        lcl[i]=z[i-1]-L*abs((abs(var[i-1])));

        if(heartbeat[i]>lcl[i] && heartbeat[i]<ucl[i])
        {
            continue;
        }
        else{
            cout<<"covariate shift is found at time: "<<time[i]<<endl;
        }
    }
    for(int i=0;i<200;i++)
    {
        cout<<time[i]<<" "<<heartbeat[i]<<" "<<error[i]<<" "<<var[i]<<" "<<z[i]<<" "<<ucl[i]<<" "<<lcl[i]<<" "<<endl;
    }


    


}