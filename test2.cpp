#include <iostream>
using namespace std;

int count[10];
string paths[] = {"main calls square", "main calls f1 calls square", "main calls f2 calls square", "", "main calls cube", "main calls f1 calls cube", "main calls f2 calls cube", "", "", ""};
int r;
int square(int x);
int f1(int a, int b);
int cube(int x);
int f2 (int a, int b);

int main()  {
  for (int k = 0; k < 10; k++)
    count[k] = 0;
  r = 0;
  for (int i = 1; i < 100; i++)
  {
    int s = square(i);
    cout << i << " " << s;
    int c = cube(i);
    cout << " " << c << endl;
  }
  for (int i = 1; i < 50; i++)
  {
    int s = square(i);
    cout << i << " " << s << endl;
  }
  for (int j = 1; j < 50; j++)
  {
    f1(j, j + 1);
    if (j > 6)
      f2(j, j + 1);
  }
  cout << "\nResults" << endl;
  for (int k = 0; k < 10; k++)
    cout << k << " " << paths[k] << " " << count[k] << endl;

}
int square(int x) {
  count[r]++;
  return x * x;
}

int cube(int x) {
  r = r + 4;
  count[r]++;
  r = r - 4;
  return x * x * x;
}

int f1(int a, int b) {
  r = r + 1;
  for (int i = 1; i <= 5; i++)
  {
    int s1;
    s1 = square(i);
    int c1;
    c1 = cube(i);
    cout << s1 + c1;
  }
  r = r - 1;
  return 0;

}

int f2(int a , int b) {
  r = r + 2;
  for (int i = 1; i <= 5; i++)
  {
    int s1 = square(i);
    int c1 = cube(i);
    int c2 = cube(i);
    cout << s1 + c1 + c2;
  }
  r = r - 2;
  return 0;
}