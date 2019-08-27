#include "HuffmanCode.h"
#include <iostream>

using namespace std;

int main()
{
	HuffmanCode code;

	code.setInputFile("in.txt");
	code.createCodeTree();
	code.codeText();

	code.outCodeTree();

	if (code.alreadyProcessed())
	{
		code.setInputFile("lol.txt");
		code.createCodeTree();
		code.codeText();
		code.outCodeTree();
	}

	code.setInputFile("in.txt");

	if (code.alreadyProcessed())
	{
		code.loadCodeTree();
		cout << code.decodeText();
	}

	code.setInputFile("lol.txt");

	if (code.alreadyProcessed())
	{
		code.loadCodeTree();
		cout << code.decodeText();
	}

	system("pause");
	return 0;
}
