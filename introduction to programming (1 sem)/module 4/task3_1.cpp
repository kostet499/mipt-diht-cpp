/*
 *
 *  Дано N кубиков. Требуется определить
 *  каким количеством способов можно выстроить
 *  из этих кубиков пирамиду.
 *
 */


#include <assert.h>
#include <iostream>



using namespace std;



long long solve(int& cubes);

int main()
{
  int cubes = -1;
  cin >> cubes;
  assert(cubes > 0);
  cout << solve(cubes) << endl;
  return 0;
}

long long solve(int& cubes)
{
  long long** dp = new long long*[cubes + 1];
  for (int i = 0; i <= cubes; i++)
    dp[i] = new long long[cubes + 1];
  for (int i = 1; i <= cubes; i++)
  {
    for (int j = cubes; j >= 1; j--) {
      if (i == j)
        dp[i][j] = 1LL;
      else if (i < j)
        dp[i][j] = 0LL;
      else
        dp[i][j] = dp[i - j][j] + (j < cubes ? dp[i][j + 1] : 0LL);

    }
  }
  long long answer = dp[cubes][1];
  for (int i = 0; i <= cubes; i++)
    delete[] dp[i];
  delete[] dp;
  return answer;
}
