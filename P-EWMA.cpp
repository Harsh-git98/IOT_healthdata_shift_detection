#include<iostream>
#include<vector>
#include <cmath>

using namespace std;
# define BETA 0.5
# define lamda 0.9
double KT(double xt, double xtmean,double sigmat)
{
    double kt=xt-xtmean;
    kt=kt/sigmat;
    return kt;

}
double PT(int kt)
{
    double n= -(kt*kt)/2;
    double ans=exp(n)/(sqrt(2)*M_1_PI);
    return ans;

}

int main()
{
    int time[200];
    int heartbeat[200];
    double PEWMA[200];
    double xtmean=heartbeat[0];
    double sigmat=1; 
    int settime =time[50];
    int s1=heartbeat[0];
    int s2=heartbeat[0];
    for(int t=0;t<time[200];t++)
    {
        double kt=KT(heartbeat[t],xtmean,sigmat);
        double pt=PT(kt);
        PEWMA[t]=pt;
        double lamdat=0;
        if(t<=settime)
        {
            lamdat=1-1/time[t];
        }
        else{
            lamdat=(1- BETA*pt)*lamda;
            if(heartbeat[t]>PEWMA[t-1])
            {
                cout<<"anamoly detected at time:"<<time[t]<<endl;
            }
        }

        double s1=lamdat*heartbeat[t] + (1-lamdat)*s1;
        double s2=lamdat*heartbeat[t] + (1-lamdat)*s2*s2;

        xtmean=s1;
        sigmat=sqrt((s2)-(s1*s1));

    }

    cout<<endl<<endl<<endl<<endl;
    for(int i=0;i<time[200];i++)
    {
        cout<<time[i]<<" "<<heartbeat[i]<<" "<<PEWMA[i]<<endl;

    }


}
