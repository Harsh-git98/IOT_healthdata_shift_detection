#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <fstream>
#include <sstream>

#define BETA 0.1
#define lamda 0.08
#define zeta 0.01
#define L 3
#define m 20

using namespace std;

struct CSVRow {
    int column1;
    int column2;
    int column3;
};

CSVRow parseCSV(const string& line) {
    CSVRow row;
    stringstream ss(line);
    char comma;
    ss >> row.column1 >> comma >> row.column2 >> comma >> row.column3;
    return row;
}

double calculateMannWhitneyU(const vector<double>& sample1, const vector<double>& sample2) {
    // Combine the two samples
    vector<double> combinedSample;
    combinedSample.reserve(sample1.size() + sample2.size());
    combinedSample.insert(combinedSample.end(), sample1.begin(), sample1.end());
    combinedSample.insert(combinedSample.end(), sample2.begin(), sample2.end());

    // Rank the combined sample
    vector<double> ranks(combinedSample.size());
    iota(ranks.begin(), ranks.end(), 1);
    sort(ranks.begin(), ranks.end(), [&](int i, int j) {
        return combinedSample[i - 1] < combinedSample[j - 1];
    });

    // Calculate U statistic for Mann-Whitney U test
    double sumRankSample1 = accumulate(ranks.begin(), ranks.begin() + sample1.size(), 0.0);
    double U = sumRankSample1 - (sample1.size() * (sample1.size() + 1) / 2.0);

    return U;
}

int main() {
    string filename = "data.csv";
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }

    double time[200] = {0};
    vector<double> heartbeat(200, 0.0); // Initialize with size and default value
    double num[200] = {0};

    string line;
    int p = 0;
    while (getline(file, line)) {
        CSVRow row = parseCSV(line);
        num[p] = row.column1;
        time[p] = row.column2;
        heartbeat[p] = row.column3;
        p++;
    }

    file.close();

    // Additional initialization and calculations
    double z[200] = {0};
    double error[200] = {0};
    double var[200] = {0};
    double ucl[200] = {0};
    double lcl[200] = {0};

    // Perform the required calculations and checks
    // training phase for 50
    double mean=0;
    for(int i=0;i<5;i++)
    {
        mean=heartbeat[i]+mean;

    }
    mean=mean/5;
   
    z[0]=mean;
    
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


    
    var[5]=variance;
    

    for (int i = 6; i < 200; i++) {
        z[i]=lamda*heartbeat[i]+(1-lamda)*z[i-1];
        error[i]=abs(heartbeat[i]-z[i-1]);
        var[i]=abs(zeta*error[i]*error[i]+(1-zeta)*var[i-1]);
        ucl[i]=z[i-1]+L*abs((abs(var[i-1])));
        lcl[i]=z[i-1]-L*abs((abs(var[i-1])));

        if (heartbeat[i] > lcl[i] && heartbeat[i] < ucl[i]) {
            continue;
        } else {
            vector<double> f1;
            vector<double> f2;
            f1.reserve(m);
            f2.reserve(m);
            for(int j=i-(m-1);j<=i;j++)
            {
                f1.push_back(heartbeat[j]);
            }
            for(int j=i+1;j<=i+m;j++)
            {
                f2.push_back(heartbeat[j]);
            }
            sort(f1.begin(),f1.end());
            sort(f2.begin(),f2.end());

            double U_statistic = calculateMannWhitneyU(f1, f2);

            // Set your significance level (alpha)
            double alpha = 0.05;

            // Calculate critical value for Mann-Whitney U test
            double critical_value = sqrt((m * (m + 1) * (2 * m + 1)) / 6.0);

            if (abs(U_statistic) <= critical_value) {
                cout << "No significant difference detected at time: " << time[i] << endl;
            } else {
                cout << "Significant difference detected at time: " << time[i] << endl;
                z[i]=z[i-1];
                var[i]=var[i-1];
                error[i]=error[i-1];
            }
        }
    }
    for(int i=0;i<200;i++)
    {
        cout<<time[i]<<" "<<heartbeat[i]<<" "<<error[i]<<" "<<var[i]<<" "<<z[i]<<" "<<ucl[i]<<" "<<lcl[i]<<" "<<endl;
    }

    return 0;
}