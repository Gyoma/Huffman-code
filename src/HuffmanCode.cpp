#include <include/HuffmanCode.h>
#include <iostream>
#include <fstream>

typedef unsigned int ui;

//конструктор узла дерева Хаффмана
HuffmanCode::Node::Node(std::string str, ui Count, Node* Next, Node* Left, Node* Right) :
    cset(str.c_str()),
    count(Count),
    left(Left),
    right(Right),
    next(Next)
{}

//конструктор копирования
HuffmanCode::Node::Node(const Node& node) :
    count(0),
    left(nullptr),
    right(nullptr),
    next(nullptr)
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

//установить входной файл
HuffmanCode::HuffmanCode(std::string in_filename) :
    root(nullptr),
    text_size(0)
{
    input_filename = in_filename;
}

//построить дерево Хаффмана
bool HuffmanCode::createCodeTree()
{
    //входной файл
    std::ifstream is(input_filename);

    if (is.is_open())
    {
        //если файл еще не обработан
        if (!processed_files.count(input_filename))
            processed_files.insert(input_filename);

        //очистить таблицу частот
        table.clear();
        //очистить дерево
        deleteTree(root);
        root = nullptr;
        text_size = 0;

        //считываем текст
        char ch;
        ch = is.get();

        //пока файл не пуст
        while (!is.eof())
        {
            //ищем символ в таблице частот
            int index = -1;
            for (size_t i = 0; i < table.size(); ++i)
                if (table[i].first == ch)
                {
                    index = i;
                    break;
                }

            //если такой символ уже есть в таблице
            if (index != -1)
                table[index].second++;
            else//иначе создаем его в таблице
                table.push_back({ ch, 1 });

            //увеличиваем размер текста
            text_size++;
            //считываем следующий символ
            ch = is.get();
        }

        //создаем корень
        root = new Node(std::string(&table[0].first, 1), table[0].second);

        //строим упорядоченный по частотам список
        for (int i = table.size() - 1; i > 0; i--)
        {
            //в узле содержится буква (или множество букв) и ее частота
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

        //строим дерево по упорядоченному списку
        //строим дерево пока в списке больше одного элемента
        while (root->next)
        {
            //создаем узел
            Node* node = new Node;
            //левый ребенок - головный элемент в списке
            node->left = root;
            //левый ребенок - следующий элемент после головного элемента в списке
            node->right = root->next;
            //складываем частоты
            node->count = node->left->count + node->right->count;
            //объеединение буквенных множеств
            node->cset = node->left->cset + node->right->cset;


            Node* ptr = root;

            //упорядочим список по частотам
            for (; ptr->next && node->count > ptr->next->count; ptr = ptr->next);
            node->next = ptr->next;
            ptr->next = node;

            //обновляем корень
            root = root->next->next;
            //обрываем связи
            node->left->next = nullptr;
            node->right->next = nullptr;
        }
    }
    else
    {
        std::cout << "file not found" << std::endl;
        return false;
    }

    is.close();
    return true;
}

void HuffmanCode::codeText()
{
    //открываем файл
    std::ifstream is(input_filename);

    if (is.is_open())
    {
        //открываем выходной файл. Его имя "out_" + имя входного файла
        std::ofstream os("out_" + input_filename);
        //в переменную code побитово записываеюся коды символом
        ui code = 0;
        //маска для прохода по битам
        ui mask = 1 << 31;
        //символ, который будет кодироваться
        char symbol;

        //выписывается количество символов в тексте (нужно для декодирования)
        os << text_size << ' ';
        //считываем символ из файла
        symbol = is.get();

        //пока файл не пуст
        while (!is.eof())
        {
            Node* ptr = root;

            //опускаемся по дереву от корня пока не достигли листа
            while (ptr->left)
            {
                //если символ в левом узле
                if (ptr->left && ptr->left->cset.find(symbol) != -1)
                    ptr = ptr->left;
                else //если в правом
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

            //считываем сл. символ
            symbol = is.get();
        }

        if (code)
            os << code << ' ';
        os.close();
    }
    is.close();
}

//сохранение дерева в файл
void HuffmanCode::saveCodeTree()
{
    std::ofstream os("ct_" + input_filename);
    saveTree(os, root);
    os.close();
}

//загрузить дерево
void HuffmanCode::loadCodeTree()
{
    //очищаем дерево
    deleteTree(root);
    root = nullptr;

    std::ifstream is("ct_" + input_filename);

    //если дерево было сохранено
    if (is.is_open())
        loadTree(is, root);

    is.close();
}

//обработан ли уже файл
bool HuffmanCode::fileAlreadyProcessed()
{
    return processed_files.count(input_filename);
}

//установить имя файла
void HuffmanCode::setInputFile(std::string in_filename)
{
    input_filename = in_filename;
}

//раскодировать текст
std::string HuffmanCode::decodeText()
{
    ui code, mask, text_size;
    Node* ptr = root;
    std::ifstream is("out_" + input_filename);
    std::string result;

    if (is.is_open())
    {
        //считываем количество символом в исходном тексте и первую часть кода
        is >> text_size >> code;

        while (!is.eof())
        {
            mask = 1 << 31;

            //если не все расшифровали
            while (mask && text_size)
            {
                //если не лист
                if (ptr->left)
                {
                    if (code & mask)
                        ptr = ptr->right;
                    else
                        ptr = ptr->left;

                    mask >>= 1;
                }
                else
                {
                    result += ptr->cset;
                    text_size--;
                    ptr = root;
                }
            }

            //считываем сл. часть кода
            is >> code;
        }
    }
    else
        result = "file not found";

    is.close();

    return result;
}

//расшифровать текст и вывести его в файл
void HuffmanCode::decodeTextTo(std::string out_filename)
{
    ui code, mask, text_size;
    Node* ptr = root;
    std::ifstream is("out_" + input_filename);
    std::ofstream os(out_filename);
    std::string result;

    if (is.is_open())
    {
        //считываем количество символом в исходном тексте и первую часть кода
        is >> text_size >> code;

        while (!is.eof())
        {
            mask = 1 << 31;

            while (mask && text_size)
            {
                if (ptr->left)
                {
                    if (code & mask)
                        ptr = ptr->right;
                    else
                        ptr = ptr->left;

                    mask >>= 1;
                }
                else
                {
                    os << ptr->cset;
                    text_size--;
                    ptr = root;
                }
            }

            is >> code;
        }
    }

    is.close();
    os.close();
}

HuffmanCode::~HuffmanCode()
{
    deleteTree(root);
}

//очистить дерево
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

//вывести дерево в файл
void HuffmanCode::saveTree(std::ofstream& os, Node* node)
{
    if (node)
    {
        //выводим узел и отделяем его строкой "=="
        os << node->cset << std::endl << "==" << std::endl;

        if (node->left)
            saveTree(os, node->left);
        if (node->right)
            saveTree(os, node->right);
    }
}

//загрузить дерево из файла
void HuffmanCode::loadTree(std::istream& is, Node*& node)
{
    //считываемая строка и строка узла
    std::string str, str_node;
    //конец узла
    bool node_end = false;

    while (!node_end)
    {
        //считываем очередную строку
        std::getline(is, str);

        //если не достигли конца строки
        if (str != "==")
        {
            if (str.empty())
            {
                if (str_node.find('\n') == -1)
                    str_node += '\n';
            }
            else
                str_node += str;
        }
        else
        {
            //создаем узел со считанным текстом
            node = new Node(str_node, str_node.size());

            //если не лист
            if (str_node != "\n\n" && str_node.size() > 1)
            {
                //загружаем левое поддерево
                loadTree(is, node->left);
                //загружаем правое поддерево
                loadTree(is, node->right);
            }

            //конец узла
            node_end = true;
        }
    }
}
