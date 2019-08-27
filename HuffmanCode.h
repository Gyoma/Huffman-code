#pragma once

#include "CharacterSet.h"
#include <fstream>
#include <vector>
#include <set>

typedef unsigned int ui;

class HuffmanCode
{
public:

	class Node
	{
	public:

		Node(std::string str = "", ui Count = 0, Node * Next = nullptr, Node * Left = nullptr, Node * Right = nullptr);

		Node(const Node& node);

		CharacterSet cset;

		friend class HuffmanCode;
	private:
		Node* left, * right, * next;
		ui count;
	};

	HuffmanCode(std::string in_filename = "");

	void createCodeTree();

	void outCodeTree();

	void loadCodeTree();

	bool alreadyProcessed();

	void setInputFile(std::string in_filename);

	void codeText();

	std::string decodeText();

	void decodeText(std::string in_filename, std::string out_filename);

	~HuffmanCode();

private:

	void createTree();

	void deleteTree(Node* node);

	void outTree(std::ofstream& os, Node* node);

	void loadTree(std::istream& is, Node*& node);

	std::vector<std::pair<char, int> > table;
	std::set<std::string> processedFiles;
	Node* root;

	std::string input_filename;
	int text_size;
};
