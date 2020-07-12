/*
Program Name:		Program #4
Description:		A C++ program that performs +,-,*,/, and $ on hexadecimal operands
Programmer Name:	Zachary Doctrove
Date:				11/9/2019
*/

#include "pch.h"
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

int decimalToHex(int operand)
{
	char hexChar = ' ';

	switch (operand)
	{
	case 10:
		hexChar = 'A';
		break;

	case 11:
		hexChar = 'B';
		break;

	case 12:
		hexChar = 'C';
		break;

	case 13:
		hexChar = 'D';
		break;

	case 14:
		hexChar = 'E';
		break;

	case 15:
		hexChar = 'F';
		break;
	}

	return hexChar;
}

int hexToDecimal(char operand)
{
	int integerValue = 0;

	switch (operand)
	{
		case 'A':
			integerValue = 10;
			break;

		case 'B':
			integerValue = 11;
			break;

		case 'C':
			integerValue = 12;
			break;

		case 'D':
			integerValue = 13;
			break;

		case 'E':
			integerValue = 14;
			break;

		case 'F':
			integerValue = 15;
			break;
	}

	return integerValue;
}

// Adds two hexadecimal operands together
string hexAddition(string op1, string op2)
{
	int op1DecValue[40] = { 0 };					
	int op2DecValue[40] = { 0 };
	
	int largerOperand;
	int smallerOperand;
	int operandSizeDiff;

	int sumDecValue[40];
	int carryValue[40] = { 0 };
	
	char finalCarry = { '0' };
	char hexLetter[40] = { 'X' };

	string hexSum = "";

	// Converts operands into a more usable form, starting with the right most values.
	for (int i = op1.length() - 1; i >= 0; i--)
	{
		if (isalpha(op1[i]))
		{
			op1DecValue[i] = hexToDecimal(op1[i]);				
		}
		else
		{
			op1DecValue[i] = op1[i] - '0';							// char - '0' = int
		}
	}

	for (int i = op2.length() - 1; i >= 0; i--)
	{
		if (isalpha(op2[i]))
		{
			op2DecValue[i] = hexToDecimal(op2[i]);
		}
		else
		{
			op2DecValue[i] = op2[i] - '0';
		}
	}

	if (op1.length() > op2.length())
	{
		largerOperand = op1.length();
		smallerOperand = op2.length();
	}
	else if (op1.length() < op2.length())
	{
		largerOperand = op2.length();
		smallerOperand = op1.length();
	}
	else
	{
		largerOperand = op1.length();								// Both operands of equal length, so it doesn't matter
		smallerOperand = op1.length();
	}

		operandSizeDiff = largerOperand - smallerOperand;


	for (int i = largerOperand - 1; i >= 0; i--)
	{	
		if (op1.length() == op2.length())
		{
			sumDecValue[i] = op1DecValue[i] + op2DecValue[i] + carryValue[i];
		}
		
		if (op1.length() == smallerOperand)
		{
			if (i - operandSizeDiff < 0)
			{
				sumDecValue[i] = op2DecValue[i] + carryValue[i];
			}
			else
			{
				sumDecValue[i] = op1DecValue[i - operandSizeDiff] + op2DecValue[i] + carryValue[i];
			}
		}
		
		if (op2.length() == smallerOperand)
		{
			if (i - operandSizeDiff < 0)
			{
				sumDecValue[i] = op1DecValue[i] + carryValue[i];
			}
			else
			{
				sumDecValue[i] = op1DecValue[i] + op2DecValue[i - operandSizeDiff] + carryValue[i];
			}
		}

		if (sumDecValue[i] >= 16)
		{
			sumDecValue[i] = sumDecValue[i] - 16;

			if (i != 0)
			{
				carryValue[i - 1] = 1;
			}
			else
			{
				finalCarry = '1';
			}
		}

		if (sumDecValue[i] >= 10)
		{
			hexLetter[i] = decimalToHex(sumDecValue[i]);
		}
		else
		{
			hexLetter[i] = sumDecValue[i] + '0';
		}

	}

	if (finalCarry == '1')
	{
		hexSum = hexSum + finalCarry;
	}

	finalCarry = '0';

	for (int i = 0; i < largerOperand; i++)
	{
		hexSum = hexSum + hexLetter[i];
	}

	return hexSum;
}

string hexSubtraction(string op1, string op2)
{
	int op1DecValue[40] = { 0 };
	int op2DecValue[40] = { 0 };

	int largerOperand;
	int smallerOperand;
	int operandSizeDiff;

	int diffDecValue[40];
	int carryValue[40] = { 0 };

	char finalCarry = { '0' };
	char hexLetter[40] = { 'X' };

	string hexDiff = "";

	// Converts operands into a more usable form, starting with the right most values.
	for (int i = op1.length() - 1; i >= 0; i--)
	{
		if (isalpha(op1[i]))
		{
			op1DecValue[i] = hexToDecimal(op1[i]);
		}
		else
		{
			op1DecValue[i] = op1[i] - '0';							// char - '0' = int
		}
	}

	for (int i = op2.length() - 1; i >= 0; i--)
	{
		if (isalpha(op2[i]))
		{
			op2DecValue[i] = hexToDecimal(op2[i]);
		}
		else
		{
			op2DecValue[i] = op2[i] - '0';
		}
	}

	if (op1.length() > op2.length())
	{
		largerOperand = op1.length();
		smallerOperand = op2.length();
	}
	else if (op1.length() < op2.length())
	{
		largerOperand = op2.length();
		smallerOperand = op1.length();
	}
	else
	{
		largerOperand = op1.length();								// Both operands of equal length, so it doesn't matter
		smallerOperand = op1.length();
	}

	operandSizeDiff = largerOperand - smallerOperand;

	for (int i = largerOperand - 1; i >= 0; i--)
	{
		if (op1.length() == op2.length())
		{
			diffDecValue[i] = op1DecValue[i] - op2DecValue[i] - carryValue[i];
		}

		if (op1.length() == smallerOperand)
		{
			if (i - operandSizeDiff < 0)
			{
				diffDecValue[i] = op2DecValue[i] - carryValue[i];
			}
			else
			{
				diffDecValue[i] = op1DecValue[i - operandSizeDiff] - op2DecValue[i] - carryValue[i];
			}
		}

		if (op2.length() == smallerOperand)
		{
			if (i - operandSizeDiff < 0)
			{
				diffDecValue[i] = op1DecValue[i] - carryValue[i];
			}
			else
			{
				diffDecValue[i] = op1DecValue[i] - op2DecValue[i - operandSizeDiff] - carryValue[i];
			}
		}

		if (diffDecValue[i] < 0)
		{
			carryValue[i - 1] = 1;									// Take 1 away from next higher value

			diffDecValue[i] = diffDecValue[i] + 16;					// Add that taken value to the lower value
		}

		if (diffDecValue[i] >= 10)
		{
			hexLetter[i] = decimalToHex(diffDecValue[i]);
		}
		else
		{
			hexLetter[i] = diffDecValue[i] + '0';
		}
		
	}

	for (int i = 0; i < largerOperand; i++)
	{
		hexDiff = hexDiff + hexLetter[i];
	}

	return hexDiff;
}


string hexMul(string op1, string op2)
{
	string originalOp1 = op1;
	string originalOp2 = op2;
	bool perfectZero = true;
	bool justOne = false;
	
	while (justOne == false)
	{
		perfectZero = true;

		
		for (int i = 0; i < originalOp2.length(); i++)				// These conditional statements are used to determine op2 equals a hexadecimal 1.
		{
			if (i != originalOp2.length() - 1)
			{
				if (op2[i] != '0')
				{
					perfectZero = false;
				}
			}
			else
			{
				if (op2[originalOp2.length() - 1] == '2' && perfectZero == true)
				{
					justOne = true;
				}
				else
				{
					justOne = false;
				}
			}
		}
		
		op1 = hexAddition(op1, originalOp1);
		op2 = hexSubtraction(op2, "1");
	}

	return op1;
}

string hexPower(string op1, string op2)
{
	string originalOp1 = op1;
	string originalOp2 = op2;
	bool perfectZero = true;
	bool justOne = false;

	while (justOne == false)
	{
		perfectZero = true;


		for (int i = 0; i < originalOp2.length(); i++)				// These conditional statements are used to determine op2 equals a hexadecimal 1.
		{
			if (i != originalOp2.length() - 1)
			{
				if (op2[i] != '0')
				{
					perfectZero = false;
				}
			}
			else
			{
				if (op2[originalOp2.length() - 1] == '2' && perfectZero == true)
				{
					justOne = true;
				}
				else
				{
					justOne = false;
				}
			}
		}

		op1 = hexMul(op1, originalOp1);
		op2 = hexSubtraction(op2, "1");
	}

	return op1;
}

string hexDivision(string op1, string op2)
{
	string quotient = op1;
	string subtractor = op2;
	string counter = "0";

	bool divisible = true;

	int quotientDecValue[40] = { 0 };
	int subtractorDecValue[40] = { 0 };

	int largerOperand;
	int smallerOperand;
	int operandSizeDiff;

	if (op1.length() > op2.length())
	{
		largerOperand = op1.length();
		smallerOperand = op2.length();
	}
	else if (op1.length() < op2.length())
	{
		largerOperand = op2.length();
		smallerOperand = op1.length();
	}
	else
	{
		largerOperand = op1.length();								// Both operands of equal length, so it doesn't matter
		smallerOperand = op1.length();
	}

	operandSizeDiff = largerOperand - smallerOperand;

	

	while (divisible == true)
	{
		if (quotient.length() > subtractor.length())				// Equalizes operand lengths
		{
			for (int i = 0; i < operandSizeDiff; i++)
			{
				subtractor = '0' + subtractor;
			}
		}

		for (int i = 0; i < quotient.length(); i++)
		{
			if (isalpha(quotient[i]))
			{
				quotientDecValue[i] = hexToDecimal(quotient[i]);
			}
			else
			{
				quotientDecValue[i] = quotient[i] - '0';			// char - '0' = int
			}
		}

		for (int i = 0; i < subtractor.length(); i++)
		{
			if (isalpha(subtractor[i]))
			{
				subtractorDecValue[i] = hexToDecimal(subtractor[i]);
			}
			else
			{
				subtractorDecValue[i] = subtractor[i] - '0';
			}
		}

		for (int i = 0; i < largerOperand; i++)
		{
			if (quotientDecValue[i] > subtractorDecValue[i])
			{
				op1 = hexSubtraction(op1, op2);
				quotient = op1;
				counter = hexAddition(counter, "1");
				break;
			}
			else if (quotientDecValue[i] == subtractorDecValue[i])
			{
				int sumZero = 0;
				for (int i = 0; i < largerOperand - 1; i++)
				{
					sumZero = sumZero + quotientDecValue[i];
				}

				if (quotientDecValue[largerOperand - 1] == subtractorDecValue[largerOperand - 1]
					&& sumZero == 0)
				{
						divisible = false;
						break;
				}
			}
			else
			{
				divisible = false;
				break;
			}
		}
	}

	op1 = "quotient " + counter + ", remainder " + op1;

	return op1;
}




int main()
{
	string rawInput;												// Will receive one line of data at a time

	string inputOperand1 = "";										// Each operand can only contain a max. of 40 digits

	char inputOperation;

	string inputOperand2 = "";

	string outputOperand = "";

	ifstream inputData;
	ofstream outputData;
	
	outputData.open("outputData.txt");
	inputData.open("InputData.txt");

	if (inputData.fail())
	{
		cout << "Your input file didn't work" << endl;
	}
	else
	{

	}
	
	
	while (!inputData.eof())										// Inserts input file data into 'rawInput' one line at a time
	{
		getline(inputData, rawInput);

		int i = 0;

		while (isalpha(rawInput[i]) || isdigit(rawInput[i]))		// Compiles all of the digits in the first operand into a string
		{
			inputOperand1 = inputOperand1 + rawInput[i];
			
			i++;
		}

		inputOperation = rawInput[i];								// Assigns the operator between the first and second operand

		i++;

		while (isalpha(rawInput[i]) || isdigit(rawInput[i]))		// Compiles all of the digits in the second operand into a string
		{
			inputOperand2 = inputOperand2 + rawInput[i];
			
			i++;
		}

		if (inputOperation == '+')
		{
			outputOperand = hexAddition(inputOperand1, inputOperand2);
		}

		if (inputOperation == '-')
		{
			outputOperand = hexSubtraction(inputOperand1, inputOperand2);
		}

		if (inputOperation == '*')
		{
			outputOperand = hexMul(inputOperand1, inputOperand2);
		}

		if (inputOperation == '$')
		{
			outputOperand = hexPower(inputOperand1, inputOperand2);
		}

		if (inputOperation == '/')
		{
			outputOperand = hexDivision(inputOperand1, inputOperand2);
		}

		i++;
		
		cout << inputOperand1 << inputOperation << inputOperand2;
		cout << "=";
		cout << outputOperand;

		cout << endl;

		// The following outputs the data into a text file called "outputData"
		outputData << inputOperand1 << inputOperation << inputOperand2;
		outputData << "=";
		outputData << outputOperand;
		outputData << endl;
		
		inputOperand1 = "";											// Clears out all the strings for the next equation in the text file
		inputOperand2 = "";
		outputOperand = "";
	}
	
	system("pause");
	return 0;
}