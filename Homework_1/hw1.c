/*--------------------------------------------------------------------
Name: Matthew Schlatter
Date: 1/29/2024
Course: CSCE 336 Embedded Systems (Spring 2024)
File: hw1.c
HW/Lab: Homework 1, programming component

Purp:   A brief description of what this program does and 
        the general solution strategy. 

Doc: 

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor 
code. I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
--------------------------------------------------------------------*/

/*
Look for comments that include "Student Code" to indicate where you 
should fill in code.
*/

#include <stdio.h>
#include <string.h>

char name[] = "Matthew Schlatter";

/** 
* Return your user name 
**/
char* getFullName(void){ 
  ////////////////////////// Student Code ////////////////////////// 
  // Write code to return a pointer to a _global_ variable that 
  // contains your full name. 
  char* fullName = name; 
    
  return fullName;
}

////////////////////////// Student Code //////////////////////////
//Write a function called getLastName() that returns a pointer to the
// same _global_ variable that contains your full name, but that
// starts at your last name.//
char* getLastName(void){ 
  char* lastName = name;

  while (*lastName != ' ' && *lastName != '\0') { 
     lastName++; 
  }

  lastName++;

  return lastName;
}
/** Gets the ith character of the user 
 *  name or returns '!' if the 
 *  name * is not that long. **/
char getFullNameChar(int i){ 
  char *name = getFullName(); 

  ////////////////////////// Student Code //////////////////////////
  // Write code to return the ith character of name or '!' if an out 
  // of bounds index is requested. Do _not_ use strlen or equivalent 
  // functions to determine the length of the string, instead, write 
  // your own code to determine the length of the string (look for 
  // the null termination of the name string: '\0').

  if (name[i] == '\0') { 
    return '!'; 
  } else { 
     return name[i]; 
  }
}

/** * Print the results of the string operation tests **/
void stringOperations(void){ 
  char *name = getFullName();

  printf("Name: %s\n",name); 
 ////////////////////////// Student Code ////////////////////////// 
 // Uncomment the following code once you create the getLastName() function

  printf("LastName: %s\n",getLastName());

  printf("Name[0]: %c\n",getFullNameChar(0)); 
  printf("Name[4]: %c\n",getFullNameChar(4)); 
  printf("Name[%d] (last): %c\n",(int)strlen(name)-1, 
         getFullNameChar(strlen(name)-1)); 
  printf("Name[%d] (oob): %c\n",(int)strlen(name), 
         getFullNameChar(strlen(name))); 
  printf("Name[%d] (oob): %c\n",(int)strlen(name)+10, 
         getFullNameChar(strlen(name)+10));

  printf("\n");
}

/** * Print the output of various bit operations **/
void bitOperations(void){ 
  int varA = 0x8191C; 
  int varB = 0x10336;

  ////////////////////////// Student Code ////////////////////////// 
  // Complete the following print statements by ouputing the value 
  //after doing the bit operations indicated in the comments. You 
  // should not periminately modifiy the value of the variables. 
  // You also should _not_ hard code the result.

  // Output the value (1 or 0) of bit 3 (zero referenced) in varA and varB 
  printf("Bit 3 in varA: %#x\n", 
            ((varA >> 3) & 1)); 
  printf("Bit 3 in varB: %#x\n", 
            ((varB >> 3) & 1));

  // Output the value of varA after setting bit 5 (zero referenced) to 1 
  printf("Bit 5 set to 1 varA: %#x\n", 
            (varA | (1 << 5)));

  // Output the value of varB after setting bit 8 (zero referenced) to 0 
  printf("Bit 8 set to 0 varB: %#x\n", 
            (varB & ~(1 << 8))); 

  // Output the value of the varA after setting the 3-5 bits 
  // (inclusive) of varA to the lower 3 bits of varB. 
  printf("varA after setting the 3-5 bits to lower 3 bits of varB: %#x\n", 
            ((varA & ~0b111000) | ((varB & 0b111) << 3))); 

  // Output the value of the varA and varB after all bitwise operations 
  // varA and varB should be unchanged. 
  printf("varA after...: %#x\n", 
            varA); 
  printf("varB after...: %#x\n", 
            varB);
}

/** 
 * Main function that calls the corresponding functions 
 **/
int main(){ 
    printf("CSCE 336 HW1 Output\n"); 
    printf("-------------------\n"); 
    stringOperations(); 
    bitOperations();

    return(0);
 }