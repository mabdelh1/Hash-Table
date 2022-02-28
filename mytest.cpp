#include "hash.h"
#include <random>
// Using a fixed seed value generates always the same sequence                      
// of pseudorandom numbers, e.g. reproducing scientific experiments                 
// here it helps us with testing since the same sequence repeats                    
std::mt19937 generator(10);// 10 is the fixed seed value                            

// We distribute the pseudorandom numbers uniformaly within                         
// the range [1000,99999]                                                           
std::uniform_int_distribution<> distribution(1000, 99999);

// The hash function used by HashTable class                                        
unsigned int hashCode(const string str);

// the function to generate random serial numbers                                   
unsigned int vaccineSerial() {return distribution(generator);}

class Tester{ // Tester class to implement test functions            
public:
  Tester(){}
  ~Tester(){}
private:  
};

int main(){
  HashTable aTable2(101, hashCode);
  int savedPeople[105];
  int savedSerial[105];
  int savedIndex = 0;
  int temp = 0;
  string names[100] = {"Moe", "Sam", "Mohamed", "Yassin", "Hala", "Ashnell", "Kachi", "Denis", "Penelope",
			 "Leonard", "Rashid", "Sawsan", "Anwar", "Twanda", "Aiya", "Sajed", "Osamn", "Khalid",
			 "Hind", "Amani", "Marwan", "Ahmed", "Abudi", "Saley", "Sahee", "Chandler", "Rachel",
			 "Chris", "Matthew", "Abraham", "Aidan", "Ross", "Joseph", "Michelle", "Ryan", "Mazin",
			 "Marlene", "Palestine", "Sudan", "Kamal", "Adrian", "Abdelhameed", "Tarig", "David",
			 "Henry", "Mansoor", "Daniel", "Casey", "Austin", "Devan", "Eric", "Jennifer", "Joesf",
			 "Bernie", "Wornock", "John", "Justin", "Kenny", "Kevin", "Johnosn", "Mason", "Fatima",
			 "Murtada", "Robert", "Ian", "Scott", "Thomas", "Tyler", "Wilson", "Ayana", "Andrew",
			 "Jake", "Monica", "NewYork", "Boston", "Chicago", "California", "Maryland", "Jayan",
			 "Jill", "Isabella", "Jacob", "Abdelmomin", "Abdelmoniem", "Abualizz", "Ramadan", "Eid",
			 "Dubai", "London", "Paris", "Jawan", "Rawan", "Hardy", "Hamza", "Dixon", "Sabrina",
			 "Samuel", "Sergio", "Newsome", "Mitchell"};

  //Insert just enough to avoid switching probing methods and rehashing
    cout << "Test Normal Insertion Operation -- Insert up to 49% of capacity" << endl;
    cout << "Expectations:" << endl;
    cout << "Linear probing should be used" << endl;
    cout << "Size of table should be 101  & number of deleted should be 0" << endl;
    for(int v = 0; v < 50; v++){
      temp = vaccineSerial();
      if (v%2 == 0){
	savedPeople[savedIndex] = v;
	savedSerial[savedIndex] = temp;
	savedIndex++;
      }
      aTable2.insert(Person(names[v], temp));
    }
    aTable2.dump();

    //Insert more to trigger a probing method switch and re-hashing
    cout << "Test Complex Insertion Operation -- Insert over 50% capacity" << endl;
    cout << "Expectations:" << endl;
    cout << "Table should be resized" << endl;
    cout << "Probing should change to quadratic" << endl;
    cout << "A re-hashing should be triggered" << endl;
    for(int n = 50; n < 100; n++){
      temp = vaccineSerial();
      if (n%2 == 0){
	savedPeople[savedIndex] = n;
	savedSerial[savedIndex] =  temp;
	savedIndex++;
      }
      aTable2.insert(Person(names[n], temp));
    }
    aTable2.dump();

    //Remove just enough to avoid switching probing methods and re-hashing 
    cout << "Test Normal Removal Operation -- Keep ration of deleted buckets to occupied buckets less than 25%" << endl;
    cout << "Expectations:" << endl;
    cout << "Probing should stay as quadratic" << endl;
    cout << "25 buckets should be marked as deleted" << endl;
    for(int x = 0; x < 25; x++)
      aTable2.remove(Person(names[savedPeople[x]], savedSerial[x]));
    aTable2.dump();

    //Remove more to trigger a probing methods switch and re-hashing
    cout << "Test Complex Removal Operation -- Exceed ration of 25% of deleted buckets to occupied buckets" << endl;
    cout << "Expectations:" << endl;
    cout << "Table should be resized" << endl;
    cout << "Probing should change back to linear" << endl;
    cout << "A re-hashing should be triggered" << endl;
    cout << "All deleted buckets must be removed, no bucket should be market as deleted" << endl;
    aTable2.remove(Person(names[savedPeople[25]], savedSerial[25]));
    aTable2.dump();
    
}
unsigned int hashCode(const string str) {
  unsigned int val = 0 ;
  const unsigned int thirtyThree = 33 ;  // magic number from textbook  
  for ( int i = 0 ; i < str.length(); i++)
    val = val * thirtyThree + str[i] ;
  return val ;
}

