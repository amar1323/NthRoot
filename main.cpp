#include <cmath>
#include <iostream>
#include <chrono>
#include <random>
#include <functional>
//#include <thread>

//GetNthRoot1: binary search solution
//GetNthRoot: Newton solution, faster using derivative of nthRoot
//To do: maybe multithread Power

using namespace std;
const double PRECISION = 1.e-16;
const double PRECISION_NEWTON = 0.000000001;
//divide and conquer algorithm to compute the power of x to the nth in o(log(n))
double Power(double x, int n){
    if(n==1)
        return x;
    double ret=Power(x,n/2);
    if( n%2==0){
        return ret*ret;
    }   
    return ret*ret*x;
    
}

//binary search algorithm to find the nth root of number, time complexity o(log^2(n))
double GetNthRoot1(double number, int n){
    if(n==1) return number;
    double start = 1, end = number;
    double guess_0 =-2;
    double guess_1 = -1;
    double mid=-1;
    do{
        mid = 0.5*(start+end);
        if(guess_1 != std::numeric_limits<double>::infinity())
            guess_0 = guess_1;
        guess_1 = Power(mid,n);

        if(guess_1>number) 
            end = mid;
        else
            start = mid;
    } while(abs(guess_1-number)>PRECISION && (abs(guess_1-guess_0)>PRECISION));
    return mid;
}
//binary search algorithm to find the nth root of number, time complexity o(log^2(n))
double GetNthRoot(double number, int n){
    if(n==1) return number;
    double guess_0 =number/n;
    double guess_1 = -1.;
    double residu=1.;
    do{
        guess_1 = ((n-1.)*guess_0+number/Power(guess_0,n-1))/n;
        residu = guess_1-guess_0;
        if(guess_1 != std::numeric_limits<double>::infinity())
            guess_0 = guess_1;
    } while(abs(residu)>PRECISION_NEWTON);
    return guess_1;
}


void TestFunction(function<double(double, int)> testFunc) {
     //compare the proposed solution, for 100 number values of x and n,with cmath power in term of time and precision
    random_device rndDevice;
    mt19937 mersenneEngine{ rndDevice() };
    uniform_int_distribution<double> numberDdist{ 100, 20000000 };
    uniform_int_distribution<int> nDdist{ 1, 100 };
    int numTest = 100;
    vector<pair<double,int>> inputs(numTest);
    vector<double> cmathSolution(numTest);
    vector<double> mySolution(numTest);
    
    generate(inputs.begin(), inputs.end(), [&numberDdist, &nDdist, &mersenneEngine]() { 
        return make_pair(numberDdist(mersenneEngine),nDdist(mersenneEngine)); 
    });

    cout.precision(32);

    auto cmathStart = chrono::steady_clock::now();
    transform(inputs.begin(), inputs.end(), cmathSolution.begin(), [](pair<double,int>& x){
        return pow(x.first, 1./x.second);
    });
    auto cmathEnd = chrono::steady_clock::now();

    auto mySolStart = chrono::steady_clock::now();
    transform(inputs.begin(), inputs.end(), mySolution.begin(), [&](pair<double,int>& x){
        return testFunc(x.first,x.second);
    });
    auto mySolEnd = chrono::steady_clock::now();

    chrono::duration<double> mySolTime = cmathEnd-cmathStart;
    chrono::duration<double> cmathTime = mySolEnd-mySolStart;
    cout << "Elapsed time cmath vs myS: "<< cmathTime.count() << "   "<< mySolTime.count()<<endl;
    double residuMoy=0.;
    double residuMax=numeric_limits<double>::min();
    for(int i = 0; i < numTest;i++){
        residuMoy+=abs(mySolution[i]-cmathSolution[i]);
        residuMax = max(residuMax, abs(mySolution[i]-cmathSolution[i]));
    }
    residuMoy/=numTest;
    cout << "Residu max:   "<< residuMax<< "   "<<"  and residu moy:   "<<residuMoy<<endl;
}


//I think my solution is faster because cmath power is more general and can compute for n in R
int main(){
    auto xx = Power(3,15);
    cout<<"test binary search solution"<<endl;
    TestFunction(GetNthRoot);
    cout<<"test Newton solution"<<endl;
    TestFunction(GetNthRoot1);
    return 0;
}