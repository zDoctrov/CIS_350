/*
Program Name:		Program #5
Description:		A C++ program that simulates a 2-pass assembler
					(Similar to how the assembler in previous assignments worked internally)
Programmer Name:	Zachary Doctrove
Date:				11/29/2019
*/

#include "pch.h"
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;


char decimalToHex(int operand)
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
			op1DecValue[i] = op1[i] - '0';								// char - '0' = int
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
		largerOperand = op1.length();									// Both operands of equal length, so it doesn't matter
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



int whiteSpaceSkip(string codeLine, int charPosition)					// Skips whitespace and tab uses
{
	charPosition++;

	while (codeLine[charPosition] == ' ')
	{
		charPosition++;
	}

	return charPosition;
}

string firstCharRemover(string operand)
{
	operand = operand.erase(0, 1);

	return operand;
}

string sizeOfInstructions(string instruction, string operand1, string operand2)
{
	int sumOfSize = 0;
	string finalSum;

	if (instruction == "ORG" || instruction == "END" || instruction == "TRAP")
	{
		// All instructions, except "ORG"/"END"/"TRAP", take up two bytes
	}
	else
	{
		sumOfSize = sumOfSize + 2;
	}

	if (operand1[0] == '#')
	{
		sumOfSize = sumOfSize + 2;
	}
	else if (operand1[0] == '$')
	{
		sumOfSize = sumOfSize + 4;
	}

	if (operand2[0] == '#')
	{
		sumOfSize = sumOfSize + 2;
	}
	else if (operand2[0] == '$')
	{
		sumOfSize = sumOfSize + 4;
	}

	finalSum = sumOfSize + '0';

	return finalSum;
}

struct assemblerCode
{
	string loop;
	string instruction;
	string operand1;
	string operand2 = "";
	string LC;															// Location counter: Stores the current memory location value
	string lengthOfCode;
};

struct machineCode
{
	string instruction;
	string operand1;
	string operand2;
};

// Assigns each instruction its hex. value, based on its operands
string machineCodeInstruction(string instruction, string operand1, string operand2 = "empty")
{
	string machineCode = "N/A";

	if (instruction == "MOVE")
	{
		if (operand1[0] == '#' && operand2 == "DO")
		{
			machineCode = "303C";
		}
		if (operand1[0] == '#' && operand2[0] == '$')
		{
			machineCode = "33FC";
		}
		if (operand1[0] == '$' && operand2 == "DO")
		{
			machineCode = "3039";
		}
		if (operand1 == "DO" && operand2[0] == '$')
		{
			machineCode = "33C0";
		}
	}
	else if (instruction == "TRAP")
	{
		if (operand1 == "#0")
		{
			machineCode = "4E40";
		}
		if (operand1 == "#1")
		{
			machineCode = "4E41";
		}
		if (operand1 == "#2")
		{
			machineCode = "4E42";
		}
	}
	else if (instruction == "ADD")
	{
		if (operand1[0] == '$' && operand2 == "DO")
		{
			machineCode = "D079";
		}
		if (operand2[0] == '$' && operand1 == "DO")
		{
			machineCode = "D179";
		}
	}
	else if (instruction == "ADDI")
	{
		if (operand1[0] == '#' && operand2 == "DO")
		{
			machineCode = "0640";
		}
		if (operand1[0] == '#' && operand2[0] == '$')
		{
			machineCode = "0679";
		}
	}
	else
	{
		machineCode = " ";
	}

	return machineCode;
}

string decimalToHexFull(string operand)
{
	string hexOperand;
	int quotient;
	char sQuotient[10];

	operand = firstCharRemover(operand);
	
	int operandValue = stoi(operand);
	
	if (operandValue < 16)
	{
		hexOperand = decimalToHex(operandValue);
	}
	else
	{
		quotient = operandValue / 16;
		hexOperand = (quotient + '0');
		hexOperand = hexOperand + decimalToHex(operandValue % 16);
	}

	return hexOperand;
}

// Assigns an operand its hex. value
string operandMachineCode(string operand)
{
	string machineOperand = " ";

	if (operand == "DO")
	{
		machineOperand = " ";
	}
	else if (operand[0] == '#')
	{
		machineOperand = decimalToHexFull(operand);
	}
	else if (operand[0] == '$')
	{
		machineOperand = firstCharRemover(operand);
	}

	while (machineOperand.length() < 4 && machineOperand != " ")
	{
		machineOperand = '0' + machineOperand;
	}

		return machineOperand;
}

int main()
{
	string inputLine;

	ifstream inputData;
	ofstream outputData;

	bool labelPresent;

	assemblerCode lineOfCode[11];
	machineCode lineOfCode2[11];

	outputData.open("output.txt");
	inputData.open("input.txt");

	if (inputData.fail())
	{
		cout << "Your input file didn't work" << endl;
	}
	else
	{

	}
	

	int currentLine = 0;												// Tells you which line of code you are on.

	// Pass One, creates symbol table and allocates memory locations for each instruction line.
	while (!inputData.eof())											// Inserts input file data into 'rawInput' one line at a time
	{
		int i = 0;														// Counts the location of the char in a single line of code.
		int j = 0;														// counts the number inputted words (except loops)
	
		labelPresent = false;

		getline(inputData, inputLine);
		
		if (inputLine[0] == 'L' || inputLine[1] == 'L')					// Locates and stores loops
		{
			lineOfCode[currentLine].loop = inputLine[0];
			i++;

			while (isspace(inputLine[i]) == false)
			{
				lineOfCode[currentLine].loop = lineOfCode[currentLine].loop + inputLine[i];
				i++;
			}
		}
		


		i = whiteSpaceSkip(inputLine, i);								// Skips whitespace
		
		while (isspace(inputLine[i]) == false)							// Stores the instructions
		{
			lineOfCode[currentLine].instruction = lineOfCode[currentLine].instruction + inputLine[i];
			i++;
		}

		i = whiteSpaceSkip(inputLine, i);								// Skips whitespace

		
		while (isspace(inputLine[i]) == false)							// Stores the first operand.
		{
			if (inputLine[i] == ',')
			{
				labelPresent = true;
				i++;
				break;
			}
	
				lineOfCode[currentLine].operand1 = lineOfCode[currentLine].operand1 + inputLine[i];
				i++;
		}

		i = whiteSpaceSkip(inputLine, i);

		while (isspace(inputLine[i]) == false && labelPresent == true)	// Stores second operand (if applicable)
		{
			lineOfCode[currentLine].operand2 = lineOfCode[currentLine].operand2 + inputLine[i];
			i++;
		}

		i = whiteSpaceSkip(inputLine, i);								// Skips whitespace

		if (labelPresent == false)										// The line of code doesn't contain a label (no comma found)
		{
			if (lineOfCode[currentLine].instruction == "ORG")
			{
				lineOfCode[currentLine].LC = lineOfCode[currentLine].operand1;	// Set currentLine to the memory address of the given line of code.
				lineOfCode[currentLine].LC = firstCharRemover(lineOfCode[currentLine].LC);
			}
			else if (lineOfCode[currentLine].instruction == "END")		// Ends the first pass, starts the second pass.
			{
				break;																					
			}
			else if (lineOfCode[currentLine].instruction == "TRAP")
			{
				lineOfCode[currentLine].lengthOfCode = sizeOfInstructions(lineOfCode[currentLine].instruction,
					lineOfCode[currentLine].operand1, lineOfCode[currentLine].operand2);

				lineOfCode[currentLine].LC = hexAddition(lineOfCode[currentLine - 1].LC,
					lineOfCode[currentLine].lengthOfCode);
			}
		}
		else
		{
			// When there is a label, store the LC found on the previous line THEN increment it

			// ***Use the database to find out the size of the current line of code, so you can find the next line's address
			// Need two functions: Old--hexadecimal addition, New--database, identifier
			
			lineOfCode[currentLine].lengthOfCode = sizeOfInstructions(lineOfCode[currentLine].instruction,
								lineOfCode[currentLine].operand1, lineOfCode[currentLine].operand2);
			
			lineOfCode[currentLine].LC = hexAddition(lineOfCode[currentLine - 1].LC,
													lineOfCode[currentLine].lengthOfCode);
		}

		currentLine++;
	}

	// Pass 2, Translate the Assembler language into machine code (hexadecimal)
	for (int i = 0; i < 11; i++)
	{
		lineOfCode2[i].instruction = machineCodeInstruction(lineOfCode[i].instruction, 
															lineOfCode[i].operand1, lineOfCode[i].operand2);
		lineOfCode2[i].operand1 = operandMachineCode(lineOfCode[i].operand1);
		lineOfCode2[i].operand2 = operandMachineCode(lineOfCode[i].operand2);
	}

	// Symbol table and Machine code version of inputs are outputted to the screen and a file
	cout << left << setw(11) << "Address";
	cout << setw(16) << "Machine Code";
	cout << setw(16) << "Operands";
	cout << "Instructions" << endl;

	outputData << left << setw(11) << "Address";
	outputData << setw(16) << "Machine Code";
	outputData << setw(16) << "Operands";
	outputData << "Instructions" << endl;

	for (int i = 1; i < 10; i++)
	{
		cout << left << setw(11) << lineOfCode[i - 1].LC;
		cout << setw(16) << lineOfCode2[i].instruction;

		outputData << left << setw(11) << lineOfCode[i - 1].LC;
		outputData << setw(16) << lineOfCode2[i].instruction;
		
		if (lineOfCode[i].operand1 != "DO")
		{
			cout << setw(16) << lineOfCode2[i].operand1 + " " + lineOfCode2[i].operand2;

			outputData << setw(16) << lineOfCode2[i].operand1 + " " + lineOfCode2[i].operand2;
		}
		else
		{
			cout << setw(16) << lineOfCode2[i].operand2;

			outputData << setw(16) << lineOfCode2[i].operand2;
		}

		cout << setw(8) << lineOfCode[i].instruction << lineOfCode[i].operand1;

		outputData << setw(8) << lineOfCode[i].instruction << lineOfCode[i].operand1;

		if (lineOfCode[i].operand2.length() > 0)
		{
			cout << ", " << lineOfCode[i].operand2 << endl;

			outputData << ", " << lineOfCode[i].operand2 << endl;
		}
		else
		{
			cout << lineOfCode[i].operand2 << endl;

			outputData << lineOfCode[i].operand2 << endl;
		}
	}

	// Identifies and displays loops within the assembler code
	cout << endl;
	cout << "Symbol Table" << endl;

	outputData << endl;
	outputData << "Symbol Table" << endl;

	for (int i = 1; i < 10; i++)
	{
		if (lineOfCode[i].loop.length() > 0)
		{
			cout << lineOfCode[i].loop << "   " << lineOfCode[i - 1].LC << endl;

			outputData << lineOfCode[i].loop << "   " << lineOfCode[i - 1].LC << endl;
		}
	}
}