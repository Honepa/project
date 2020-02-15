#include <iostream>
#include <ctime>
#include <vector>
using namespace std;
int main()
{
  unsigned int ontime = clock();
  for (int j = 0; j < 1000; j++)
  {
   const int N = 30;
	int lp[N+1];
	vector<int> pr;
 
	for (int i=2; i<=N; ++i) 
	{
		if (lp[i] == 0) 
		{
		lp[i] = i;
		pr.push_back (i);
	  } 
	for (int j=0; j<(int)pr.size() && pr[j]<=lp[i] && i*pr[j]<=N; ++j)
		lp[i * pr[j]] = pr[j];
  }
   }
  
  unsigned int endtime = clock();
  cout << endtime - ontime << endl;
 // cin.get(); cin.get();
} 
