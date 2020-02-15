#include <iostream>
#include <ctime>
#include <vector>
using namespace std;

int main()
{
  unsigned int ontime = clock();
  for (int j = 0; j < 1000; j++)
  {
   int n;
  n = 30;
  vector<char> prime (n+1, true);
	prime[0] = prime[1] = false;
	for (int i=2; i<=n; ++i)
	{
		if (prime[i])
		{
			if (i * 1ll * i <= n)
			{
			 for (int j=i*i; j<=n; j+=i)
					prime[j] = false;
			} //1ll для перевода в long long
				
		}
			
	}
		
  }
  
	unsigned int endtime = clock();
	cout << endtime - ontime << endl;
	return 0;
}
