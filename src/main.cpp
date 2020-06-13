#include <include/HuffmanCode.h>
#include <iostream>

int main()
{
	//EXAMPLE
	HuffmanCode code;

	code.setInputFile("input_text");
	code.createCodeTree();
	code.codeText();

	code.saveCodeTree();

	if (!code.fileAlreadyProcessed())
	{
		code.setInputFile("another_text");
		code.createCodeTree();
		code.codeText();
		code.saveCodeTree();
	}

	code.setInputFile("input_text");

	if (code.fileAlreadyProcessed())
	{
		code.loadCodeTree();
		std::cout << code.decodeText();
	}

	return 0;
}
