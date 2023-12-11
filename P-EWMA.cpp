#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>

using namespace std;
#define BETA 0.1
#define lamda 0.08

struct CSVRow
{
    int column1;
    int column2;
    int column3;
};

// 55 90 170

CSVRow parseCSV(const string &line)
{
    CSVRow row;
    stringstream ss(line);
    char comma; // to read the comma
    ss >> row.column1 >> comma >> row.column2 >> comma >> row.column3;
    return row;
}


double KT(double xt, double xtmean, double sigmat)
{
    double kt = abs(xt - xtmean);
    kt = kt / sigmat;

    return kt;
}

double PT(double kt)
{
    double n = (-(kt * kt) / 2);
    double ans = exp(n) / (sqrt(2 * M_1_PI));

    return ans;
}

int main()
{
    string filename = "data.csv";

    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }

    double time[200] = {0};
    double heartbeat[200] = {0};
    double num[200] = {0};
    

    string line;
    int p = 0;
    while (getline(file, line))
    {

        CSVRow row = parseCSV(line);
        num[p] = row.column1;
        time[p] = row.column2;
        heartbeat[p] = row.column3;
        p++;
    }

    file.close();

    double PEWMA[200];
    double xtmean = 0;
    double sigmat = 1;
    double s2sq[200] = {0};
    double ptr[200]={0};
    int settime = time[5];
    double lam[200];
    double ktt[200];
    double s1 = heartbeat[0];
    double sigma[200] = {1};
    double s2 = heartbeat[0];
    for (int t = 0; t < 200; t++)
    {
        double kt = KT(heartbeat[t], xtmean, sigmat);
        ktt[t] = kt;
        double pt = PT(kt);
        ptr[t] = pt;
        double lamdat ;

        if (t <= settime)
        {
            lamdat = 1 - (1 / time[t]);

            s1 = lamdat * heartbeat[t] + (1 - lamdat) * s1;
            s2 = (lamdat * heartbeat[t] * heartbeat[t] + (1 - lamdat) * s1 * s1);
            xtmean = s1;
            PEWMA[t] = s1;
            s2sq[t] = s2;
            sigmat = sqrt(abs(s2 - (s1 * s1)));
            sigma[t] = sigmat;
        }

        else
        {
            lamdat = (1 - BETA * pt) * lamda;
            lam[t] = lamdat;
            double g = abs(heartbeat[t] - PEWMA[t - 1]);
            if (g >= 3 * (sigma[t - 1]))
            {
                cout << "anamoly detected at time:" << time[t] << endl;
            }

            s1 = lamdat * heartbeat[t] + (1 - lamdat) * s1;
            s2 = (lamdat * heartbeat[t] * heartbeat[t] + (1 - lamdat) * s1 * s1);
            xtmean = s1;
            PEWMA[t] = s1;
            s2sq[t] = s2;
            sigmat = sqrt(abs(s2 - (s1 * s1)));
            sigma[t] = sigmat;
        }
    }

    for (int i = 0; i < 200; i++)
    {
        cout << time[i] << " " << heartbeat[i] << " " << PEWMA[i] <</* " --- " << ktt[i] << " " << ptr[i] << " " << lam[i] << */" " << sigma[i] << endl;
    }
}
