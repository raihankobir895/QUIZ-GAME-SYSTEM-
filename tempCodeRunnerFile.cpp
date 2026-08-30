#include<iostream>
#include<cmath>
#include<iomainp>
using namespace std;
double f(double x)
{
    return x*x*x - 2*x + 5;
}
double df(double x)
{
    return 3*x*x - 2;
}
int main (void)
{
    double x0,x1,precision;
    int n;
    cout<<"Enter x0 and the precision and the number of iteration ";
    cin>>x0>>precision>>n;
    cout <<"its x0\t\t x1\n";

     for(int i=1;i<=n;i++)

{
    x1=x0 - f(x0)/df(x0);
    cout<< i << "\t" << setprecision(6);
    
    if(fabs(x1)-(x0)<precsion)
    {
        break;
    }
    x0=x1;

}
cout<<"\n Approx root"<<x1;
cout<<end;


}
