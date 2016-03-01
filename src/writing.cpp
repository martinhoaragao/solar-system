/*
 * Example of outputing to a file in c++.
 * compile with 'g++ writing.cpp'.
 *
 * 2902105
 */

#include <fstream>
#include <iostream>

using namespace std;

int main() {
  ofstream file ("writing.3d");

  file << "plane\n";
  file << "-1.0, 0.0, 1.0\n";
  file << "-1.0, 0.0, -1.0\n";
  file << "1.0 , 0.0, 1.0\n";
  file << "1.0 , 0.0, -1.0\n";
  file << "1.0 , 0.0, 1.0\n";
  file << "-1.0, 0.0, -1.0\n";

  file.close();
}
