#include <fstream>
#include <algorithm>
#include "HuffmanCode.h"

typedef unsigned int ui;

HuffmanCode::Node::Node(std::string str, ui Count, Node* Next, Node* Left, Node* Right) :
	cset(str.c_str()),
	count(Count),
	left(Left),
	right(Right),
	next(Next)
{}

HuffmanCode::Node::Node(const Node& node)
{
	if (this != &node)
	{
		cset = node.cset;
		count = node.count;
		left = node.left;
		right = node.right;
		next = node.next;
	}
}

HuffmanCode::HuffmanCode(std::string in_filename) :
	root(nullptr),
	text_size(0)
{
	input_filename = in_filename;
}

void HuffmanCode::createCodeTree()
{
	std::ifstream is(input_filename);

	if (!processedFiles.count(input_filename))
		processedFiles.insert(input_filename);

	if (is.is_open())
	{
		table.clear();
		deleteTree(root);
		root = nullptr;
		text_size = 0;

		char ch;
		ch = is.get();

		while (!is.eof())
		{
			auto it = find_if(table.begin(), table.end(), [ch](std::pair<char, int> element) -> bool
				{
					return element.first == ch;
				});

			if (it != table.end())
				it->second++;
			else
				table.push_back({ ch, 1 });

			text_size++;
			ch = is.get();
		}

		root = new Node(std::string(&table[0].first, 1), table[0].second);

		for (int i = table.size() - 1; i > 0; i--)
		{
			Node* node = new Node(std::string(&table[i].first, 1), table[i].second);

			if (node->count <= root->count)
			{
				node->next = root;
				root = node;
			}
			else
			{
				Node* ptr = root;

				for (; ptr->next && table[i].second > ptr->next->count; ptr = ptr->next);

				node->next = ptr->next;
				ptr->next = node;
			}
		}

		createTree();
	}

	is.close();
}

void HuffmanCode::codeText()
{
	std::ifstream is(input_filename);

	if (is.is_open())
	{
		std::ofstream os("out_" + input_filename);
		ui code = 0;
		ui mask = 1 << 31;
		char symbol;

		os << text_size << ' ';
		symbol = is.get();

		while (!is.eof())
		{
			Node* ptr = root;

			while (ptr->left)
			{

				if (ptr->left && ptr->left->cset[symbol])
					ptr = ptr->left;
				else
				{
					code |= mask;
					ptr = ptr->right;
				}

				mask >>= 1;

				if (mask == 0)
				{
					os << code << ' ';
					code = 0;
					mask = 1 << 31;
				}
			}

			symbol = is.get();
		}

		if (code)
			os << code << ' ';
		os.close();
	}
	is.close();
}

void HuffmanCode::outCodeTree()
{
	std::ofstream os("ct_" + input_filename);
	outTree(os, root);
	os.close();
}

void HuffmanCode::loadCodeTree()
{
	deleteTree(root);
	root = nullptr;

	std::ifstream is("ct_" + input_filename);

	if (is.is_open())
		loadTree(is, root);

	is.close();
}

bool HuffmanCode::fileAlreadyProcessed()
{
	return processedFiles.count(input_filename);
}

void HuffmanCode::setInputFile(std::string in_filename)
{
	input_filename = in_filename;
}

std::string HuffmanCode::decodeText()
{
	ui x, mask, count;
	Node* ptr = root;
	std::ifstream is("out_" + input_filename);
	std::string result;

	if (is.is_open())
	{

		is >> count >> x;

		while (!is.eof())
		{
			mask = 1 << 31;

			while (mask && count)
			{
				if (ptr->left)
				{
					if (x & mask)
						ptr = ptr->right;
					else
						ptr = ptr->left;

					mask >>= 1;
				}
				else
				{
					result += ptr->cset.get();
					count--;
					ptr = root;
				}
			}

			is >> x;
		}
	}

	is.close();

	return result;
}

void HuffmanCode::decodeTextTo(std::string out_filename)
{
	ui x, mask, count;
	Node* ptr = root;
	std::ifstream is("out_" + input_filename);
	std::ofstream os(out_filename);
	std::string result;

	if (is.is_open())
	{

		is >> count >> x;

		while (!is.eof())
		{
			mask = 1 << 31;

			while (mask && count)
			{
				if (ptr->left)
				{
					if (x & mask)
						ptr = ptr->right;
					else
						ptr = ptr->left;

					mask >>= 1;
				}
				else
				{
					os << ptr->cset.get();
					count--;
					ptr = root;
				}
			}

			is >> x;
		}
	}

	is.close();
	os.close();
}

HuffmanCode::~HuffmanCode()
{
	deleteTree(root);
}

void HuffmanCode::createTree()
{
	while (root->next)
	{
		Node* node = new Node;
		node->left = root;
		node->right = root->next;
		node->count = node->left->count + node->right->count;
		node->cset = node->left->cset + node->right->cset;

		Node* ptr = root;

		for (; ptr->next && node->count > ptr->next->count; ptr = ptr->next);
		node->next = ptr->next;
		ptr->next = node;

		root = root->next->next;
		node->left->next = nullptr;
		node->right->next = nullptr;
	}
}

void HuffmanCode::deleteTree(Node* node)
{
	if (node)
	{
		if (node->left)
			deleteTree(node->left);
		if (node->right)
			deleteTree(node->right);
	}
	delete node;
}

void HuffmanCode::outTree(std::ofstream& os, Node* node)
{
	if (node)
	{
		os << node->cset << std::endl << "endl" << std::endl;

		if (node->left)
			outTree(os, node->left);
		if (node->right)
			outTree(os, node->right);
	}
}

void HuffmanCode::loadTree(std::istream& is, Node*& node)
{
	std::string str, str_node;

	while (true)
	{
		std::getline(is, str);

		if (str != "endl")
		{
			if (str.empty())
				str_node += '\n';
			else
				str_node += str;

		}
		else
		{
			node = new Node(str_node, str_node.size());

			if (str_node != "\n\n" && str_node.size() > 1)
			{
				loadTree(is, node->left);
				loadTree(is, node->right);
			}

			return;
		}

	}
}
