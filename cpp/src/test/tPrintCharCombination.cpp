#include <iostream>

void printCharCombination(const std::string s_, const std::string s1_)
{
  for(int i = 0; i < s_.length(); ++i)
  {
    std::string t  = s_;
    if (s_.length() <= 1)
      std::cout << s1_ << s_[i] << "\n";
    else
      printCharCombination(t.erase(i,1), s1_ + s_[i]);
  }
}

int main()
{
  printCharCombination("ABCDE", "");
  return(0);
}

