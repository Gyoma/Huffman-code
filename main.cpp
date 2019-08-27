#include "HuffmanCode.h"
#include <iostream>

using namespace std;

int main()
{
	HuffmanCode code;

	code.setInputFile("in.txt");
	code.createCodeTree(); //it's neсessary to procces data
	code.codeText();

	code.outCodeTree(); //save code tree for the processed data

	if (code.fileAlreadyProcessed())
	{
		code.setInputFile("lol.txt");
		code.createCodeTree();
		code.codeText();
		code.outCodeTree();
	}

	code.setInputFile("in.txt");

	code.loadCodeTree();
	cout << code.decodeText();

	system("pause");
	return 0;
}
