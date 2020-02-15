#include <iostream>
#include <ctime>
using namespace std;

int main()
{
unsigned int ontime = clock();
for (int j = 0; j < 1000; j++)
{
 int N = 0;
 //cin >> N;
 N = 30;
 for (int i = 2; i <= N; i++)
 {
  if((i % 2 != 0) or (i == 2))
  {
   if((i % 3 != 0) or (i == 3))
   {
    if((i % 5 != 0) or (i == 5))
    {
     if((i % 7 != 0) or (i == 7))
     {
      //cout << i << '\t';
     }
    }
   }
  }
 }
}
 
 //cout << endl;
 unsigned int endtime = clock();
 cout << endtime - ontime << endl;
 return 0;
}
