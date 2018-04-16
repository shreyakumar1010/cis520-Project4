#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>




//load the lines into an array



//substring alg
/*
function LCSubstr(S[1..r], T[1..n])
    L := array(1..r, 1..n)
    z := 0
    ret := {}
    for i := 1..r
        for j := 1..n
            if S[i] == T[j]
                if i == 1 or j == 1
                    L[i,j] := 1
                else
                    L[i,j] := L[i-1,j-1] + 1
                if L[i,j] > z
                    z := L[i,j]
                    ret := {S[i-z+1..i]}
                else
                if L[i,j] == z
                    ret := ret ∪ {S[i-z+1..i]}
            else
                L[i,j] := 0
    return ret
    */
 string longestCommonSubstring(const string &str1, const string &str2)
{
  if(str1.empty() || str2.empty())
  {
    return 0;
  }
  
  int *curr = new int [str2.size()];
  int *prev = new int [str2.size()];
  int *swap = NULL;
  int maxSubstr = 0;
   string longest;
  for(unsigned int i = 0; i < str1.size(); ++i)
  {
    for(unsigned int j = 0; j < str2.size(); ++j)
    {
      if(str1[i] != str2[j])
      {
        curr[j] = 0;
      }
      else
      {
        if(i == 0 || j == 0)
        {
          curr[j] = 1;
        }
        else
        {
          curr[j] = 1 + prev[j-1];
        }
          if(maxSubstr < curr[j])
        {
          maxSubstr = curr[j];
             longest.clear();
        }
          if (maxSubstr == curr[j])
          {
            longest += str1.substr(i - maxSubstr + 1, i + 1);
          }
      }
    }
    swap=curr;
    curr=prev;
    prev=swap;
  }
  delete [] curr;
  delete [] prev;
  return longest.substr(0, maxSubstr);
}
    

//print results

