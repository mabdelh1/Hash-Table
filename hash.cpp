// CMSC 341 - Spring 2021 - Project 4
// hash.cpp
// The impleme ntation of the class HashTable
#include "hash.h"
#include "person.h"
#include "math.h"
HashTable::HashTable(unsigned size, hash_fn hash){
  m_hash = hash; //Set the hash function
  m_size = 0; //Initialized size
  m_numDeleted = 0; //initialized number of deleted
  //Seriess of if/else statements to determine capacity
  if(size <= MINPRIME)
    m_capacity = MINPRIME;
  else{
    if(size >= MAXPRIME)
      m_capacity = MAXPRIME;
    else{
      if(!isPrime(size))
	m_capacity = findNextPrime(size);
      else
	m_capacity = size;
    }
  }
  m_table = new Person[MAXPRIME]; //allocate memory for the hashtable
  m_currentProb = LINEAR; //probing is set to linear at the beginning of the program
}

HashTable::~HashTable(){
  delete[] m_table;
}

bool HashTable::insert(Person person){
  bool inserted = false; 
  unsigned int genericIndex = m_hash(person.key());
  unsigned int index = genericIndex % tableSize();
  unsigned int i = 1;
  if(findPerson(person) == 0){ //Only add person if they are not in the table
    if(m_table[index] == EMPTY) //If index bucket is available, declare a succsessful insertion
      inserted = true;
    else{
      if(m_currentProb == LINEAR){ //Otherwise, either do linear quadratic probing
	do{ //do-while loop to keep doing collision handeling until an empty buckets is found
	  index = (genericIndex + i) % tableSize(); //equation for linear probing
	  if(m_table[index] == EMPTY)
	    inserted = true;
	  i = i + 1;
	}while(inserted == false || i > m_capacity);
      }
      else{ //Or Quadratic probing
	do{ //do-while loop to keep doing collision handeling until an empty buckets is found
	  index = ((genericIndex + (i*i)) % m_capacity); //equation for quadratic probing
	  if(m_table[index] == EMPTY)
	    inserted = true;
	  i = i + 1;
	}while(inserted == false || i > m_capacity);
      }
    }
  }
  if(inserted){ //insert perosn into buckets if the previous oberation finds an empty bucket
    m_table[index] = person;
    m_size = m_size + 1;
  }
  if(lambda() > 0.5){ //Check for probbing switch and re-hashing criteria 
    if(m_currentProb == QUADRATIC) //if re-hashin criteria is met, switch probing
      setProbingPolicy(LINEAR);
    else
      setProbingPolicy(QUADRATIC);
  }
  return inserted;
}

bool HashTable::remove(Person person){
  bool deleted = false;
  unsigned int genericIndex = m_hash(person.key());
  unsigned int index = genericIndex % tableSize();
  unsigned int i = 1;
  if(m_table[index] == person) //see if the generic equation located the person
    deleted = true;
  else{
    if(m_currentProb == 0){ //otherwise, do liner probing
      do{
	index = (genericIndex + i) % tableSize();
	if(m_table[index] == person)
	  deleted = true;
	i = i + 1;
      }while(deleted == false || i > m_capacity);
    }
    else{ //or quadratic probing
      index = genericIndex % tableSize();
      do{
	index = (genericIndex + (i*i)) % tableSize();
	if(m_table[index] == person)
	  deleted = true;
	i = i + 1;
      }while(deleted == false || i > m_capacity);
    }
  }
  if(deleted){ //if the person is found, proceed to delete
    m_table[index] = DELETED;
    m_numDeleted = m_numDeleted + 1;
  }
  if(deletedRatio() > 0.25){ //check for probing switching and  re-hashing criteria
    if(m_currentProb == 1)
      setProbingPolicy(LINEAR);
    else
      setProbingPolicy(QUADRATIC);
  }
  return deleted;
}

void HashTable::setProbingPolicy(probing prob){
  unsigned int dataPoints = m_size - m_numDeleted; //calculate number of active buckets
  Person people[dataPoints]; //Array to temporerly hold people
  unsigned int count = 0;
  for(unsigned int i = 0; i < m_capacity; i++){ //copy people to the array
    if(!(m_table[i] == EMPTY || m_table[i] == DELETED)){ //only copy active people 
      people[count] = m_table[i];
      count = count + 1;
    }
  }
  delete[] m_table; //deallocate memory
  m_table = {nullptr};
  unsigned int size = 4 * dataPoints; //resize table
  if(size <= MINPRIME) //Determine new size
    m_capacity = MINPRIME;
  else{
    if(size >= MAXPRIME)
      m_capacity = MAXPRIME;
    else{
      if(!isPrime(size))
        m_capacity = findNextPrime(size);
      else
        m_capacity = size;
    }
  }
  //re-initialize variables
  m_size = 0;
  m_numDeleted = 0;
  m_currentProb = prob; //change probing
  m_table = new Person[MAXPRIME]; //allocate memry
  for(unsigned int c = 0; c < dataPoints; c++) //add people back to trigger re-hashing
    insert(people[c]);
}

float HashTable::lambda() const {
  return (static_cast<float>(m_size) / static_cast<float>(m_capacity));
}

float HashTable::deletedRatio() const {
  if(m_size != 0)
    return (static_cast<float>(m_numDeleted) / static_cast<float>(m_size));
  else
    return 0.0;
}

void HashTable::dump() const {
  for (unsigned int i = 0; i < m_capacity; i++) {
    cout << "[" << i << "] : " << m_table[i] << endl;
  }
}

bool HashTable::isPrime(int number){
  // If number is prime this function returns true
  // otherwise it returns false
  bool result = true;
  for (int i = 2; i <= number / 2; ++i) {
    if (number % i == 0) {
      result = false;
      break;
    }
  }
  return result;
}

int HashTable::findNextPrime(int current){
  //we won't go beyond MAXPRIME
  //the smallest prime would be next after MINPRIME
  if (current < MINPRIME) current = MINPRIME;
  for (int i=current; i<MAXPRIME; i++) { 
    for (int j=2; j*j<=i; j++) {
      if (i % j == 0) 
	break;
      else if (j+1 > sqrt(i) && i != current) {
	return i;
      }
    }
  }
  //if a user tries to go over MAXPRIME
  return MAXPRIME;
}

unsigned int HashTable::findPerson(Person person){
  for (unsigned int i = 0; i < m_capacity; i++){
    if(!(m_table[i] == EMPTY)){
      if(person == m_table[i])
	return i;
    }
  }
  return 0;
}
