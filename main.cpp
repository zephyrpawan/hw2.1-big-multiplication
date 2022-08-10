/*
 * Author: Pawan Bhandari
 * Assignment Title: Homework 2.1: Big Multiplication
 * Assignment Description: This program promts user to enter two big integers in the command line and prints their
 *                          product by implementing the recursive Integer multplication from textbook section 2.1
 * Due Date: 05/20/2012 
 * Date Created: 05/18/2022 
 * Date Last Modified:05/20/2022
 */

// Uses C++-2011 standard

/*
 * Data Abstraction:
 *       Integers are stored as an Vectors num1 and num2  with digits as string elements
 *
 * Input:
 *       The program reads input as a string from the user as assigns it to number1 and number2
 *
 * Process:
 *       Input stream is opened to read the two big integers
 *       Vector named num1 is created such that it contains individual digits number1 as its string elements
 *       Vector named num2 is created such that it contains individual digits number2 as its string elements
 *       Product is calculated by calling multiplyRecursively function and is stored as a Vector named productVec with
 *          individual digits of product as its string elements
 *
 *
 * Output:
 *       Product is printed as an integer in the command line
 *
 * Assumptions:
 *      User enters valid numbers only when prompted
 */

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

using namespace std;

//** function declarations
vector<string> multiplyRecursively(vector<string> num1, vector<string> num2, vector<string> productVec);
string add(string a, string b);
string subtract(string a, string b);
vector<string> addStringVec(vector<string> stringVec1, vector<string> stringVec2);
vector<string> substractStringVec(vector<string> vec1, vector<string> vec2);
vector<string> multiplyByTenPowN(vector<string> vec, int n);
void printVector(vector<string> vector);
vector<string> insertZerosAtStart(vector<string> vec, int n);
string add3(string a, string b, string c);
vector<string> removeStartingZeros(vector<string> vec);
//**

int main()
{
    // prompt user to type and enter first Integer and store it as number1
    string number1 = "";
    cout << "Please enter the first integer: ";
    cin >> number1;
    cout << endl;

    // prompt user to type and enter second Integer and store it as number1
    string number2 = "";
    cout << "Please enter the second integer: ";
    cin >> number2;
    cout << endl;

    // Initialize two vectors to store numbers and populate with digits
    vector<string> num1;
    vector<string> num2;

    for(int i = 0; i < number1.length(); i++) {
        num1.push_back(string(1, number1[i]));
    }

    for(int i = 0; i < number2.length(); i++) {
        num2.push_back(string(1, number2[i]));
    }

    vector<string> productVec;
    // call recursive function to multiply numbers
    productVec = multiplyRecursively(num1, num2, productVec);
    cout << "The product of ";
    cout << number1;
    cout << " and ";
    cout << number2;
    cout << " is " << endl;
    // Print the contents of product vector as a single product string/integer in the console
    for(int i = 0; i < productVec.size(); i++) {
        cout << productVec.at(i);
    }
    cout << endl;

    return 0;
}

//*************************************************************************************
// description: implements a recursive integer multiplication pseudocode from textbook 2.1
// return: a vector with single digits as it elements which when combined is a product
//*************************************************************************************

vector<string> multiplyRecursively(vector<string> num1, vector<string> num2, vector<string> productVec)
{
    if(num1.size() > num2.size()) {
        num2 = insertZerosAtStart(num2, num1.size() - num2.size());
    }

    if(num2.size() > num1.size()) {
        num1 = insertZerosAtStart(num1, num2.size() - num1.size());
    }

    int digits = num1.size();

    // base case
    if(digits <= 1) {
        productVec.insert(productVec.begin(), to_string(stoi(num1[0]) * stoi(num2[0])));
        return productVec;
    }

    /* A divide-and-conquer algorithm for integer multiplication
     xL , xR = leftmost dn/2e, rightmost bn/2c bits of x
     yL , yR = leftmost dn/2e, rightmost bn/2c bits of y
         P1  = multiply(xL , yL )
         P2  = multiply(xR , yR )
         P3  = multiply(xL + xR , yL + yR )
    return P1 × 2n + (P3 − P1 − P2 ) × 2n/2 + P2
    */

    // Divide and conquer
    vector<string> num1Left = { num1.begin(), num1.end() - (digits / 2) };
    vector<string> num1Right = { num1.end() - (digits / 2), num1.end() };
    vector<string> num2Left = { num2.begin(), num2.end() - (digits / 2) };
    vector<string> num2Right = { num2.end() - (digits / 2), num2.end() };

    vector<string> p1 = multiplyRecursively(num1Left, num2Left, productVec);
    vector<string> p2 = multiplyRecursively(num1Right, num2Right, productVec);
    vector<string> p3 =
        multiplyRecursively(addStringVec(num1Left, num1Right), addStringVec(num2Left, num2Right), productVec);

    // P1 × 2^n + (P3 − P1 − P2 ) × 2^n/2 + P2

    vector<string> A = multiplyByTenPowN(p1, digits);
    vector<string> B = multiplyByTenPowN(substractStringVec(p3, (addStringVec(p1, p2))), digits / 2);

    return addStringVec(addStringVec(A, B), p2);
}

vector<string> multiplyByTenPowN(vector<string> vec, int n)
{
    while(n > 0) {
        vec.push_back("0");
        n--;
    }
    return vec;
}

vector<string> addStringVec(vector<string> vec1, vector<string> vec2)
{
    if(vec1.size() > vec2.size()) {
        vec2 = insertZerosAtStart(vec2, vec1.size() - vec2.size());
    }
    if(vec2.size() > vec1.size()) {
        vec1 = insertZerosAtStart(vec1, vec2.size() - vec1.size());
    }

    vector<string> sumVec;
    string carry = "0";
    string digitSum = "0";

    for(int i = vec1.size() - 1; i >= 0; i--) {
        digitSum = add3(vec1.at(i), vec2.at(i), carry);

        if(stoi(digitSum) > 9) {
            sumVec.push_back(to_string(stoi(digitSum) % 10));
            carry = to_string(stoi(digitSum) / 10);
        } else {
            sumVec.push_back(digitSum);
            carry = "0";
        }
    }
    sumVec.push_back(carry);
    reverse(sumVec.begin(), sumVec.end());

    return removeStartingZeros(sumVec);
}

vector<string> substractStringVec(vector<string> vec1, vector<string> vec2)
{
    if(vec1.size() > vec2.size()) {
        vec2 = insertZerosAtStart(vec2, vec1.size() - vec2.size());
    }

    if(vec2.size() > vec1.size()) {
        vec1 = insertZerosAtStart(vec1, vec2.size() - vec1.size());
    }
    vector<string> diffVec;
    string borrow = "0";
    int numToSubtractFrom = 0;

    for(int i = vec1.size() - 1; i >= 0; i--) {
        if(stoi(borrow) == 1) {
            numToSubtractFrom = stoi(vec1.at(i)) - 1;
        } else {
            numToSubtractFrom = stoi(vec1.at(i));
        }

        if(numToSubtractFrom >= stoi(vec2.at(i))) {
            diffVec.push_back(to_string(numToSubtractFrom - stoi(vec2.at(i))));
            borrow = "0";
        } else {
            diffVec.push_back(to_string(numToSubtractFrom + 10 - stoi(vec2.at(i))));
            borrow = "1";
        }
    }
    reverse(diffVec.begin(), diffVec.end());

    return removeStartingZeros(diffVec);
}

vector<string> insertZerosAtStart(vector<string> vec, int n)
{
    while(n > 0) {
        vec.insert(vec.begin(), "0");
        n--;
    }
    return vec;
}

vector<string> removeStartingZeros(vector<string> vec)

{
    if(vec.size() == 1) {
        return vec;
    } else {

        for(int i = 0; i < vec.size(); i++) {
            if(stoi(vec.at(0)) > 0) {
                break;
            }
            vec.erase(vec.begin());
        }

        return vec;
    }
}

string add(string a, string b)
{
    return to_string(stoi(a) + stoi(b));
}

string add3(string a, string b, string c)
{
    return to_string(stoi(a) + stoi(b) + stoi(c));
}

string subtract(string a, string b)
{
    return to_string(stoi(a) - stoi(b));
}

void printVector(vector<string> vec)
{
    cout << "{";
    // Iterating vector by using index
    for(int i = 0; i < vec.size(); i++) {
        cout << vec[i] << ", ";
    }
    cout << "}";
}