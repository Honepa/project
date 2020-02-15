#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

int main()
{
unsigned int ontime = clock();
for (int j = 0; j < 1000; j++)
{
	int n;
 	n = 30;
	vector<bool> prime (n + 1, true);
	prime[0] = prime[1] = false;
	for (int i = 2; i * i <= n; ++i)   // valid for n < 46340^2 = 2147395600
  	  if (prime[i])
  	      for (int j = i * i; j <= n; j += i)
  	          prime[j] = false;
}
 
unsigned int endtime = clock();
cout << endtime - ontime << endl;
}
