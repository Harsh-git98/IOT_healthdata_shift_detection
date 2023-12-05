#include<iostream>
#include<vector>
#include <cmath>
#include <fstream>
#include <sstream>

using namespace std;
# define BETA 0.05
# define lamda 0.95

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


double KT(double xt, double xtmean,double sigmat)
{
    double kt=xt-xtmean;
    kt=kt/sigmat;
    return kt;
    

}
double PT(int kt)
{
    double n= (-(kt*kt)/2);
    double ans=exp(n)/(sqrt(2*M_1_PI));
    
    return ans;
    

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

    double PEWMA[200];
    double xtmean=heartbeat[0];
    double sigmat=0; 
    int settime =time[50];
    double s1=heartbeat[0];
    double s2=heartbeat[0];
    for(int t=0;t<200;t++)
    {
        double kt=KT(heartbeat[t],xtmean,sigmat);
        double pt=PT(kt);
        
        double lamdat=0;
        if(t<=settime)
        {
            lamdat=1-(1/time[t]);
        }
        else{
            lamdat=(1- BETA*pt)*lamda;
            if(abs(heartbeat[t]-PEWMA[t-1])>10)
            {
                cout<<"anamoly detected at time:"<<time[t]<<endl;
            }
            
        }
        
        double s1=lamdat*heartbeat[t] + (1-lamdat)*s1;
        double s2=(lamdat*heartbeat[t] + (1-lamdat)*s2*s2);
        xtmean=s1;
        PEWMA[t] = s1;
        sigmat=sqrt(abs(s2-(s1*s1)));
        
    }

    for(int i=0;i<200;i++)
    {
        cout<<time[i]<<" "<<heartbeat[i]<<" "<<PEWMA[i]<<endl;
    }


}
