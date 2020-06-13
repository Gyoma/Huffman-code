#pragma once

#include <fstream>
#include <vector>
#include <unordered_set>
#include <string>

typedef unsigned int ui;

class HuffmanCode
{
public:

	class Node //узел в дереве Хаффмана
	{
	public:

		Node(std::string str = "", ui Count = 0, Node * Next = nullptr, Node * Left = nullptr, Node * Right = nullptr);
		Node(const Node& node);

		//ћножество символов
		std::string cset;

		friend class HuffmanCode;
	private:
		Node* left, * right, * next;
		//количество этого символа в тексте
		ui count;
	};

	//передаетс¤ входной файл в конструктор
	HuffmanCode(std::string in_filename = "");

	//создать дерево ’аффмана
	bool createCodeTree();

	//сохранить дерево ’аффмана
	void saveCodeTree();

	//загрузить дерево ’аффмана
	void loadCodeTree();

	//создавалось ли дл¤ этого файла дерево ’аффмана
	bool fileAlreadyProcessed();

	//установить новый файл дл¤ кодировани¤
	void setInputFile(std::string in_filename);

	//кодировать текст
	void codeText();

	//декодировать текст
	std::string decodeText();

	//декодировать текст в файл
	void decodeTextTo(std::string out_filename);

	~HuffmanCode();

private:
	void deleteTree(Node* node);
	void saveTree(std::ofstream& os, Node* node);
	void loadTree(std::istream& is, Node*& node);

	//таблица чатсот символов
	std::vector<std::pair<char, int> > table;
	//множество обработанных файлов
	std::unordered_set<std::string> processed_files;
	//корень дерева ’аффмана
	Node* root;
	
	//текущий файл
	std::string input_filename;
	//размер текста
	int text_size;
};
