#include<cstdlib>
#include<fstream>
#include<iostream>
#include<iomanip>
#include<string>
using namespace std;
#define inFile "data.txt"

void Process(ifstream&);

int main()
{
    ifstream ins;
    ins.open(inFile);
    if (ins.fail())
    {
        cerr << "***ERROR: Cannot open " << inFile << " for input." << endl;
        return EXIT_FAILURE;
    }
    Process(ins);
    ins.close();
    return 0;
}

void Process(ifstream& ins)
{
    int A, BB, B, CC, C, DD, D, F;
    int score;
    int n; 
    float average;
    float sum;
    string course;
    cout << fixed << setprecision(2);
    cout << "Course" << setw(6) << "A" << setw(5) << "B+" << setw(5) << "B" << setw(5) << "C+" << setw(5) << "C" << setw(5) << "D+" << setw(5) << "D" << setw(5) << "F" << setw(9) << "Average" << setw(6) << "total" << endl;
    cout << "-------------------------------------------------" << endl;
    ins >> course;
    while (!ins.eof())
    {
        A = 0; BB = 0; B = 0; CC = 0; C = 0; DD = 0; D = 0; F = 0;
        average = 0.0;
        sum = 0.0;
        n = 0;
        ins >> score;
        while (score != -1)
        {
            if (score < 50)
                F++;
            else if (score < 55)
                D++;
            else if (score < 60)
                DD++;
            else if (score < 65)
                C++;
            else if (score < 70)
                CC++;
            else if (score < 75)
                B++;
            else if (score < 80)
                BB++;
            else
                A++;
            sum = sum + score;
            n++;
            ins >> score;
        }
        average = sum / n;
        cout << course << setw(5) << A << setw(5) << BB << setw(5) << B << setw(5) << CC << setw(5) << C << setw(5) << DD << setw(5) << D << setw(5) << F << setw(8) << average << setw(6) << n << endl;
        ins >> course;
    }
    cout << "-------------------------------------------------" << endl;
}