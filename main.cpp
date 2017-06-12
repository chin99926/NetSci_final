/*************************************
	file:	main.cpp
	repo:	NetSci/final/src
*************************************/

#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "myUtil.h"
using namespace std;

#define	USAGE	"final inputfile"
#define _n	123345

/* ==================================
	final inputfile
================================== */

double findIntersect(const vector<int>& x, const vector<int>& y, vector<int>& z);
double calcAdamic(const vector<int>& z, vector<int>* list[]);

int main(int argc, char **argv) {
   if(!myCheckArgc(argc, 2, USAGE)) return 1;

   ifstream ifile;
   ifile.open(argv[1], ios::in);
   if(!ifile.is_open()) {
      cerr << "Error: file \'" << argv[1] << "\' failed to open!!" << endl;
      return 1;
   }

   vector<int>* list[_n];
   for(int i = 0; i < _n; i++) list[i] = new vector<int>;

   string ln;
   vector<int> nums;

   while(ifile.good()) {
      ln.clear(); nums.clear();
      getline(ifile, ln);
      if(!parse2Int(ln, nums, ' ')) continue;

      int s = nums[0] - 1, d = nums[1] - 1;
      list[s]->push_back(d); list[d]->push_back(s);
   }

   int real_n = 0, max_d = 0;
   float avg_deg = 0;

   for(int i = 0; i < _n; i++) {
      if(list[i]->empty()) continue;

      int tmp_l = int(list[i]->size());
      sort(list[i]->begin(), list[i]->end());
      avg_deg += float(tmp_l); real_n++;
      if(max_d < tmp_l) max_d = tmp_l;
   }

/* simple analysis */
   cout << "=== simple analysis ==="
        << "\nnum. of nodes: " << real_n
        << "\nAvg degree: " << avg_deg
        << "\nMax degree: " << max_d << '\n' << endl;

   int i_J, j_J, i_A, j_A;
   size_t union_num;
   double tmp_a, tmp_j, max_Adamic = 0, max_Jaccard = 0;
   vector<int> intersect;

/*   string tkn;

// iteratively prompt cmd
   do {
      ln.clear(); tkn.clear();
      cout << "demo >> ";
      getline(cin, ln);

      size_t n = myStrGetTok(ln, tkn, 0, ' ');

   } while(
*/

/* record link prediction metric */
   for(int i = 0; i < _n; i++) {
      if(list[i]->empty()) continue;

      max_Adamic = max_Jaccard = 0;

      for(int j = i + 1; j < _n; j++) {
         if(list[j]->empty()) continue;

         intersect.clear();
         tmp_j = findIntersect((*list[i]), (*list[j]), intersect);
         if(max_Jaccard < tmp_j) {
            max_Jaccard = tmp_j;
            i_J = i; j_J = j;
         }

         tmp_a = calcAdamic(intersect, list);
         if(max_Adamic < tmp_a) {
            max_Adamic = tmp_a;
            i_A = i; j_A = j;
         }

      }
cout << "finish processing node " << i << "...\n"
     << "A = " << max_Adamic << " at " << j_A
     << ", J = " << max_Jaccard << " at " << j_J << endl;
   }

   cout << "Largest Adamic coefficient: " << max_Adamic
        << "\nbetween " << i_A << " and " << j_A << endl;
   cout << "Largest Jaccard coefficient: " << max_Adamic
        << "\nbetween " << i_J << " and " << j_J << endl;

   return 0;
}

double
findIntersect(const vector<int>& x, const vector<int>& y,
		vector<int>& z) {
   z.clear();
   size_t i = 0, j = 0;
   double in = 0, un = 0;

   while(i < x.size() || j < y.size()) {
      if(i == x.size()) i--; if(j == y.size()) j--;

      if(x[i] < y[j]) {
         un++;
         if(i == x.size() - 1) break;
         else if(i < x.size() - 1) i++;
      }
      else if(x[i] == y[j]) {
         z.push_back(x[i]); un++;
         in++;
         i++; j++;
      }
      else {
         un++;
         if(j == y.size() - 1) break;
         else if(j < y.size() - 1) j++;
      }
   }

   return in / un;
}

double
calcAdamic(const vector<int>& z, vector<int>* list[]) {
   double ada = 0;

   for(size_t i = 0; i < z.size(); i++) {
      if(list[z[i]]->size() <= 1) continue;
      ada += (1 / myLog2(list[z[i]]->size()));
   }

   return ada;
}
