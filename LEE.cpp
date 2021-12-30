/*
Found limitations:
	- max 58 operations
	- max 10 atoms
	- normal form limited

	To do:
	- non table comparison
	- create macros
*/

#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;

#define MAX_STRING 4096
#define MAX_FILE 512

int VERBOSE_LEVEL = 1; //mutiple levels 0(none)-4(all)
bool ALLOW_RELAXED_SYNTAX = true, ALLOW_TABLE_EVAL = true, ALLOW_SIMPLIFICATION = false, fileOutput = false, fileInput = false;
char OUT_FILE[MAX_FILE] = { 0 }, IN_FILE[MAX_FILE] = { 0 };
ifstream fin;
ofstream fout;

int toInt(char s[])
{
	int n = 0;
	for (int i = 0; s[i]; i++)
	{
		if (s[i] >= '0' && s[i] <= '9')
			n = n * 10 + s[i] - '0';
	}
	return n;
}

bool insertChar(char s[], const char c, const int pos)
{
	if (pos > strlen(s) + 1 || pos < 0)
		return false;
	for (int i = strlen(s); i >= pos; i--)
		s[i + 1] = s[i];
	s[pos] = c;
	if (VERBOSE_LEVEL > 3) cout << "Inserted: '" << c << "' at " << pos << " : " << s << ".\n";
	return true;
}
bool popChar(char s[], const int pos)
{
	if (pos >= strlen(s) || pos < 0)
		return false;
	if (VERBOSE_LEVEL > 3) cout << "Popped: '" << s[pos] << "' at " << pos << " : " << s << ".\n";
	for (int i = pos; s[i]; i++)
		s[i] = s[i + 1];
	return true;
}

void getStrongExp(char s[])
{ //a naive implementation
	bool isGood = true;
	int len = strlen(s);

	if (VERBOSE_LEVEL > 1) cout << "Adding missing parenthesis\n";

	for (int i = len - 1; i >= 0; i--)
	{ // negations
		if (s[i] == '!')
		{ //find first atom or end of parentheses after
			int j = i + 1, paranth = 0;
			while (s[j] && (((s[j] < 'A' || s[j] > 'Z') && (s[j] != 'f' && s[j] != 't')) || paranth != 0) && (s[j] != ')' || paranth != 1))
			{
				if (s[j] == '(') paranth++;
				else if (s[j] == ')') paranth--;
				j++;
			}
			insertChar(s, ')', j + 1);
			insertChar(s, '(', i);
			if (VERBOSE_LEVEL > 2) cout << " - Added parenthesis for: '" << s[i] << "' at " << i << " : " << s << ".\n";
			len += 2;
		}
	}

	for (int i = len - 1; i >= 0; i--)
	{ // ^ and v
		if (s[i] == 'v' || s[i] == '^' || s[i] == '|' || s[i] == '*')
		{ //find first atom or end of parentheses
			int j = i + 1, paranth = 0;
			while (s[j] && (((s[j] < 'A' || s[j] > 'Z') && (s[j] != 'f' && s[j] != 't')) || paranth != 0) && (s[j] != ')' || paranth != 1))
			{
				if (s[j] == '(') paranth++;
				else if (s[j] == ')') paranth--;
				j++;
			}
			insertChar(s, ')', j + 1);

			j = i - 1; paranth = 0;

			while (s[j] && (((s[j] < 'A' || s[j] > 'Z') && (s[j] != 'f' && s[j] != 't')) || paranth != 0) && (s[j] != '(' || paranth != 1))
			{
				if (s[j] == ')') paranth++;
				else if (s[j] == '(') paranth--;
				j--;
			}
			insertChar(s, '(', j);
			if (VERBOSE_LEVEL > 2) cout << " - Added parenthesis for: '" << s[i] << "' at " << i << " : " << s << ".\n";
			len += 2;
		}
	}

	for (int i = len - 1; i >= 0; i--)
	{ // for >
		if (s[i] == '>')
		{ //find first atom or end of parentheses
			int j = i + 1, paranth = 0;
			while (s[j] && (((s[j] < 'A' || s[j] > 'Z') && (s[j] != 'f' && s[j] != 't')) || paranth != 0) && (s[j] != ')' || paranth != 1))
			{
				if (s[j] == '(') paranth++;
				else if (s[j] == ')') paranth--;
				j++;
			}
			insertChar(s, ')', j + 1);

			j = i - 1; paranth = 0;

			while (s[j] && (((s[j] < 'A' || s[j] > 'Z') && (s[j] != 'f' && s[j] != 't')) || paranth != 0) && (s[j] != '(' || paranth != 1))
			{
				if (s[j] == ')') paranth++;
				else if (s[j] == '(') paranth--;
				j--;
			}
			insertChar(s, '(', j);
			if (VERBOSE_LEVEL > 2) cout << " - Added parenthesis for: '" << s[i] << "' at " << i << " : " << s << ".\n";
			len += 2;
		}
	}

	for (int i = len - 1; i >= 0; i--)
	{ // for =
		if (s[i] == '=')
		{ //find first atom or end of parentheses
			int j = i + 1, paranth = 0;
			while (s[j] && (((s[j] < 'A' || s[j] > 'Z') && (s[j] != 'f' && s[j] != 't')) || paranth != 0) && (s[j] != ')' || paranth != 1))
			{
				if (s[j] == '(') paranth++;
				else if (s[j] == ')') paranth--;
				j++;
			}
			insertChar(s, ')', j + 1);

			j = i - 1; paranth = 0;

			while (s[j] && (((s[j] < 'A' || s[j] > 'Z') && (s[j] != 'f' && s[j] != 't')) || paranth != 0) && (s[j] != '(' || paranth != 1))
			{
				if (s[j] == ')') paranth++;
				else if (s[j] == '(') paranth--;
				j--;
			}
			insertChar(s, '(', j);
			if (VERBOSE_LEVEL > 2) cout << " - Added parenthesis for: '" << s[i] << "' at " << i << " : " << s << ".\n";
			len += 2;
		}
	}

	//remove excess parentheses
	if (VERBOSE_LEVEL > 1) cout << "Removing excess parenthesis\n";
	for (int i = 0; s[i + 1]; i++)
	{
		if (s[i] == '(' && s[i + 1] == '(')
		{ //find the pair of s[i + 1]
			int j = i + 2, paranth = 1;
			while (s[j] && paranth != 0)
			{
				if (s[j] == '(') paranth++;
				else if (s[j] == ')') paranth--;
				j++;
			}
			if (s[j] == ')')
			{
				popChar(s, j - 1);
				popChar(s, i + 1);
				if (VERBOSE_LEVEL > 2) cout << " - Removed excess parenthesis: " << s << ".\n";
				i--;
			}
		}
	}
}

struct TableColumn
{
	char exp[MAX_STRING] = { 0 };
	int valueNumber;
	bool values[4096];

	void deconstruct()
	{
		memset(exp, 0, MAX_STRING);
		for (int i = 0; i < 4096; i++)
			values[i] = 0;
	}
};
struct Node
{
	char symbol = 0;
	int father = -1, leaf[2] = { -1 };
	bool value = false, isInterpreted = false;

	void construct(const char s, const int f, const int l1, const int l2)
	{
		symbol = s;
		father = f;
		leaf[0] = l1;
		leaf[1] = l2;
	}
	void clearInterpretation()
	{
		value = false;
		isInterpreted = false;
	}
	void deconstruct()
	{
		symbol = 0;
		father = -1;
		leaf[0] = -1;
		leaf[1] = -1;
		value = false;
		isInterpreted = false;
	}
	void addInterpret(bool b)
	{
		isInterpreted = true;
		value = b;
	}
	bool append(const char s)
	{
		if (symbol != 0)
			return 0;
		symbol = s;
		return 1;
	}
};
struct Tree
{
	Node node[1000];
	TableColumn table[60];
	int nrNodes = 0;
	int atoms[26][1000]; //hold positions for atoms

	//general functions
	int getFreeNode()
	{ //return first unused node from the list
		for (int i = 1; i < 1000; i++)
		{
			if (node[i].symbol == 0 && node[i].father == -1)
				return i;
		}
		return -1;
	}
	void clearInterpretations()
	{
		for (int i = 0; i < 1000; i++)
			node[i].clearInterpretation();
		if (VERBOSE_LEVEL > 3)
			cout << "Values cleared\n";
	}
	void clearAtomsList()
	{
		for (int i = 0; i < 26; i++)
			for (int j = 0; j < 1000; j++)
				atoms[i][j] = -1;
		if (VERBOSE_LEVEL > 3)
			cout << "Atom list cleared\n";
	}
	void updateAtomsList(int pos)
	{
		if (node[pos].symbol >= 'A' && node[pos].symbol <= 'Z')
		{
			for (int i = 0; i < 1000; i++)
				if (atoms[node[pos].symbol - 65][i] == -1)
				{
					atoms[node[pos].symbol - 65][i] = pos;
					break;
				}
		}
		else if (node[pos].symbol == '!')
		{
			updateAtomsList(node[pos].leaf[0]);
		}
		else if (node[pos].symbol == '^' || node[pos].symbol == 'v' || node[pos].symbol == '>' || node[pos].symbol == '=' || node[pos].symbol == '*' || node[pos].symbol == '|')
		{
			updateAtomsList(node[pos].leaf[0]);
			updateAtomsList(node[pos].leaf[1]);
		}
		if (VERBOSE_LEVEL > 3)
			cout << "Atom list updated\n";
	}
	void getTreeList()
	{
		for (int i = 0; i < nrNodes; i++)
			if (node[i].symbol != 0)
			{
				cout << "Node " << i << ": '" << node[i].symbol << "'   leafs: " << node[i].leaf[0] << ',' << node[i].leaf[1] << "   father:   " << node[i].father << '\n';
				if (fileOutput) fout<< "Node " << i << ": '" << node[i].symbol << "'   leafs: " << node[i].leaf[0] << ',' << node[i].leaf[1] << "   father:   " << node[i].father << '\n';
			}
	}
	int getAtomCount()
	{
		int c = 0;
		for (int i = 0; i < 26; i++)
			if (atoms[i][0] != -1)
				c++;
		return c;
	}
	void getAllInterpretations(const int atomCnt, char s[])
	{ //generate all interpretations for n atoms
		int n = 1;
		for (int i = 0; i < atomCnt; i++)
			n *= 2;

		for (int i = 0; i < n; i++)
		{
			insertChar(s, '{', strlen(s));
			int b = i, a = 0, ai = 25;
			while (a < atomCnt)
			{
				if (b % 2 == 0)
					insertChar(s, '!', strlen(s));

				//find next atom
				while (atoms[ai][0] == -1) ai--;

				insertChar(s, ai + 'A', strlen(s));
				b /= 2;
				ai--;
				a++;
			}
			insertChar(s, '}', strlen(s));
		}
	}
	void defragmentTree(int pos)
	{
		int free;
		if (node[0].symbol == 0)
			free = 0;
		else
			free = getFreeNode();
		if (pos > free)
		{
			if (pos == node[node[pos].father].leaf[1])
				node[node[pos].father].leaf[1] = free;
			else
				node[node[pos].father].leaf[0] = free;

			node[free].construct(node[pos].symbol, node[pos].father, node[pos].leaf[0], node[pos].leaf[1]);

			node[node[pos].leaf[0]].father = free;
			node[node[pos].leaf[1]].father = free;

			node[pos].deconstruct();
			pos = free;
		}

		//parsing
		if (node[pos].symbol == '!')
		{
			defragmentTree(node[pos].leaf[0]);
		}
		else if (node[pos].symbol == '^' || node[pos].symbol == 'v' || node[pos].symbol == '>' || node[pos].symbol == '=' || node[pos].symbol == '*' || node[pos].symbol == '|')
		{
			defragmentTree(node[pos].leaf[0]);
			defragmentTree(node[pos].leaf[1]);
		}
	}

	//tree building
	bool getTree(char s[])
	{
		node[0].construct(0, -1, -1, -1);
		if (VERBOSE_LEVEL > 2) cout << " - node 0 constructed\n";
		nrNodes++;

		int pos = 0, i = 0;
		while (s[i] != 0)
		{
			if (s[i] == '(')
			{ //add new node, connect it to previous node
				node[pos].leaf[0] = pos + 1;
				pos++;
				node[pos].construct(0, pos - 1, -1, -1);
				nrNodes++;
				if (VERBOSE_LEVEL > 2) cout << i << ": '('      New node (" << pos << ") constructed, position set to " << pos << ".\n";
			}
			else if (s[i] == ')')
			{ //move back one node
				if (node[pos].symbol == 0)
				{
					if (VERBOSE_LEVEL > 2) cout << i << ": ')'      Error: A proposition was expected.\n";
					nrNodes = 0;
					return false;
				}
				else
				{
					pos = node[pos].father;
					if (VERBOSE_LEVEL > 2) cout << i << ": ')'      Position set to " << pos << ".\n";
				}
			}
			else if (s[i] == '!')
			{ //append '!' to past node
				node[pos].leaf[0] = pos + 1;
				if (!node[pos - 1].append('!'))
				{
					if (VERBOSE_LEVEL > 2) cout << i << ": '!'      Error: Node already appended.\n";
					nrNodes = 0;
					return false;
				}
				else
				{
					if (VERBOSE_LEVEL > 2) cout << i << ": '!'      Symbol appended to node (" << pos - 1 << ").\n";
				}
			}
			else if (s[i] == 'v' || s[i] == '^' || s[i] == '>' || s[i] == '=' || s[i] == '|' || s[i] == '*')
			{ //create a new leaf on the current node and move to it
				if (node[pos].leaf[0] == -1)
				{
					if (VERBOSE_LEVEL > 2) cout << i << ": '" << s[i] << "'      Error: A proposition was expected.\n";
					nrNodes = 0;
					return false;
				}
				else
				{
					int auxPos = getFreeNode();
					if (!node[pos].append(s[i]))
					{
						if (VERBOSE_LEVEL > 2) cout << i << ": '" << s[i] << "'      Error: Node already appended.\n";
						nrNodes = 0;
						return false;
					}
					else
					{
						node[pos].leaf[1] = auxPos;
						node[auxPos].construct(0, pos, -1, -1);
						nrNodes++;
						pos = auxPos;
						if (VERBOSE_LEVEL > 2) cout << i << ": '" << s[i] << "'      New node (" << pos << ") constructed, position set to " << pos << ".\n";
					}
				}
			}
			else if (s[i] >= 'A' && s[i] <= 'Z')
			{ //append atomic proposition and move to previous node
				if (!node[pos].append(s[i]))
				{
					if (VERBOSE_LEVEL > 2) cout << i << ": '" << s[i] << "'      Error: Node already appended.\n";
					nrNodes = 0;
					return false;
				}
				else
				{
					//save the nodes containing atoms for interpretation
					int k = 0; while (k < 1000 && atoms[s[i] - 'A'][k] != -1) k++;
					atoms[s[i] - 'A'][k] = pos;

					node[pos].leaf[0] = -1; node[pos].leaf[1] = -1; //why is this needed? sometimes atoms get leafs out of nowhere...memory leaks maybe??
					pos = node[pos].father;
					if (VERBOSE_LEVEL > 2) cout << i << ": '" << s[i] << "'      Position set to " << pos << " leaf1: " << node[pos].leaf[0] << " leaf2: " << node[pos].leaf[1] << ".\n";
				}
			}
			else if (s[i] == 'f')
			{ //append FALSE and move to previous node
				if (!node[pos].append(s[i]))
				{
					if (VERBOSE_LEVEL > 2) cout << i << ": '" << s[i] << "'      Error: Node already appended.\n";
					nrNodes = 0;
					return false;
				}
				else
				{
					node[pos].value = false; node[pos].isInterpreted = true;
					node[pos].leaf[0] = -1; node[pos].leaf[1] = -1; //why is this needed? sometimes atoms get leafs out of nowhere...memory leaks maybe??
					pos = node[pos].father;
					if (VERBOSE_LEVEL > 2) cout << i << ": 'FALSE'  Position set to " << pos << ".\n";
				}
			}
			else if (s[i] == 't')
			{ //append FALSE and move to previous node
				if (!node[pos].append(s[i]))
				{
					if (VERBOSE_LEVEL > 2) cout << i << ": '" << s[i] << "'      Error: Node already appended.\n";
					nrNodes = 0;
					return false;
				}
				else
				{
					node[pos].value = true; node[pos].isInterpreted = true;
					pos = node[pos].father;
					if (VERBOSE_LEVEL > 2) cout << i << ": 'TRUE'   Position set to " << pos << ".\n";
				}
			}
			else if (s[i] != ' ')
			{
				if (VERBOSE_LEVEL > 2) cout << i << ": '" << s[i] << "'      Error: Unknown syntax.\n";
				nrNodes = 0;
				return false;
			}
			i++;
		}


		//evaluation output
		if (pos != -1)
		{
			nrNodes = 0;
			return false;
		}

		return true;
	}
	bool construct(char s[])
	{
		if (ALLOW_RELAXED_SYNTAX)
		{
			getStrongExp(s);
			cout << "Strong expression generated: " << s << '\n';
		}
		for (int i = 0; i < 26; i++)
			for (int j = 0; j < 1000; j++)
				atoms[i][j] = -1;
		return getTree(s);
	}

	//table stuff
	bool recInt(int pos)
	{
		if (node[pos].symbol == '!')
		{
			bool ls = recInt(node[pos].leaf[0]);
			node[pos].value = !ls;
			node[pos].isInterpreted = true;
			if (VERBOSE_LEVEL > 2)
			{
				cout <<" - Node "<< pos << ": '" << node[pos].symbol << "' is " << !ls << "\n";
				//char aux[MAX_STRING] = { 0 }; recTableValueExp(0, 0, aux); cout << aux << '\n';
			}
			return !ls;
		}
		else if (node[pos].symbol == '^')
		{
			bool ls = recInt(node[pos].leaf[0]), rs = recInt(node[pos].leaf[1]);
			node[pos].value = ls * rs;
			node[pos].isInterpreted = true;
			if (VERBOSE_LEVEL > 2)
			{
				cout << " - Node " << pos << ": '" << node[pos].symbol << "' is " << ls * rs << "\n";
				//char aux[MAX_STRING] = { 0 }; recTableValueExp(0, 0, aux); cout << aux << '\n';
			}
			return ls * rs;
		}
		else if (node[pos].symbol == 'v')
		{
			bool ls = recInt(node[pos].leaf[0]), rs = recInt(node[pos].leaf[1]);
			node[pos].value = ls + rs;
			node[pos].isInterpreted = true;
			if (VERBOSE_LEVEL > 2)
			{
				cout << " - Node " << pos << ": '" << node[pos].symbol << "' is " << bool(ls + rs) << "\n";
				//char aux[MAX_STRING] = { 0 }; recTableValueExp(0, 0, aux); cout << aux << '\n';
			}
			return ls + rs;
		}
		else if (node[pos].symbol == '|')
		{
			bool ls = recInt(node[pos].leaf[0]), rs = recInt(node[pos].leaf[1]);
			node[pos].value = !(ls * rs);
			node[pos].isInterpreted = true;
			if (VERBOSE_LEVEL > 2)
			{
				cout << " - Node " << pos << ": '" << node[pos].symbol << "' is " << ls * rs << "\n";
				//char aux[MAX_STRING] = { 0 }; recTableValueExp(0, 0, aux); cout << aux << '\n';
			}
			return !(ls * rs);
		}
		else if (node[pos].symbol == '*')
		{
			bool ls = recInt(node[pos].leaf[0]), rs = recInt(node[pos].leaf[1]);
			node[pos].value = !(ls + rs);
			node[pos].isInterpreted = true;
			if (VERBOSE_LEVEL > 2)
			{
				cout << " - Node " << pos << ": '" << node[pos].symbol << "' is " << bool(ls + rs) << "\n";
				//char aux[MAX_STRING] = { 0 }; recTableValueExp(0, 0, aux); cout << aux << '\n';
			}
			return !(ls + rs);
		}
		else if (node[pos].symbol == '>')
		{
			bool ls = recInt(node[pos].leaf[0]), rs = recInt(node[pos].leaf[1]);
			node[pos].isInterpreted = true;
			if (ls && !rs)
			{
				node[pos].value = false;
				if (VERBOSE_LEVEL > 2)
				{
					cout << " - Node " << pos << ": '" << node[pos].symbol << "' is 0\n";
					//char aux[MAX_STRING] = { 0 }; recTableValueExp(0, 0, aux); cout << aux << '\n';
				}
				return false;
			}
			node[pos].value = true;
			if (VERBOSE_LEVEL > 2)
			{
				cout << " - Node " << pos << ": '" << node[pos].symbol << "' is 1\n";
				//char aux[MAX_STRING] = { 0 }; recTableValueExp(0, 0, aux); cout << aux << '\n';
			}
			return true;
		}
		else if (node[pos].symbol == '=')
		{
			bool ls = recInt(node[pos].leaf[0]), rs = recInt(node[pos].leaf[1]);
			node[pos].isInterpreted = true;
			node[pos].value = false;
			if (ls != rs)
			{
				if (VERBOSE_LEVEL > 2)
				{
					cout << " - Node " << pos << ": '" << node[pos].symbol << "' is 0\n";
					//char aux[MAX_STRING] = { 0 }; recTableValueExp(0, 0, aux); cout << aux << '\n';
				}
				return false;
			}
			node[pos].value = true;
			if (VERBOSE_LEVEL > 2)
			{
				cout << " - Node " << pos << ": '" << node[pos].symbol << "' is 0\n";
				//char aux[MAX_STRING] = { 0 }; recTableValueExp(0, 0, aux); cout << aux << '\n';
			}
			return true;
		}
		else if (node[pos].symbol >= 'A' && node[pos].symbol <= 'Z')
		{
			if (VERBOSE_LEVEL > 2)
			{
				if (!node[pos].value)
					cout << " - Node " << pos << ": '" << node[pos].symbol << "' is 0 (interpreted atomic)\n";
				else
					cout << " - Node " << pos << ": '" << node[pos].symbol << "' is 1 (interpreted atomic)\n";
			}
			return node[pos].value;
		}
		else
		{
			if (VERBOSE_LEVEL > 2)
			{
				if (!node[pos].value)
					cout << " - Node " << pos << ": '" << node[pos].symbol << "' is 0 (contradiction)\n";
				else
					cout << " - Node " << pos << ": '" << node[pos].symbol << "' is 1 (tautology)\n";
			}
			return node[pos].value;
		}
	}
	void loadIntToTable(int i, int pos, int &tablePos)
	{ //doesnt load atoms
		if (node[pos].symbol == '!')
		{
			table[tablePos--].values[i] = node[pos].value;

			loadIntToTable(i, node[pos].leaf[0], tablePos);
		}
		else if (node[pos].symbol == '^' || node[pos].symbol == 'v' || node[pos].symbol == '>' || node[pos].symbol == '=' || node[pos].symbol == '|' || node[pos].symbol == '*')
		{
			table[tablePos--].values[i] = node[pos].value;

			loadIntToTable(i, node[pos].leaf[0], tablePos);
			loadIntToTable(i, node[pos].leaf[1], tablePos);
		}
	}
	int readInterpretation(char s[])
	{
		int i = 0, ind = 0, startPos;
		while (s[i] != 0)// read interpretations
		{
			if (s[i] == '{')
			{
				bool b = true;
				startPos = i;
				clearInterpretations();
				while (s[i] && s[i] != '}') //reading an interpretation
				{
					i++;
					if (s[i] == '!')
						b = false;
					else if (s[i] >= 'A' && s[i] <= 'Z')
					{
						if (atoms[s[i] - 'A'][0] != -1)
						{
							int k = 0;
							while (k < 1000 && atoms[s[i] - 'A'][k] != -1)
							{
								node[atoms[s[i] - 'A'][k]].addInterpret(b);
								k++;
							}
							b = true;
						}
						else
							cout << "Warning: '" << s[i] << "' is not an atom in the given proposition\n";
					}
				}
				bool ok = true;
				for (int j = 0; j < 1000; j++) //check that all are interpreted
					if (node[j].symbol >= 'A' && node[j].symbol <= 'Z' && !node[j].isInterpreted)
					{
						ok = false;
						break;
					}
				if (ok)
				{
					if (VERBOSE_LEVEL > 2)
					{
						char aux[MAX_STRING] = { 0 };
						strncpy_s(aux, MAX_STRING, s + startPos + 1, i - startPos - 1);
						cout << "For interpretation: {" << aux << "}:\n";
					}
					recInt(0);
					//load subtree values
					int aux = getFreeNode() - 1;
					loadIntToTable(ind, 0, aux);
					//load atom value
					aux = 0;
					for (int i = 0; i < 26; i++)
						if (atoms[i][0] != -1)
							table[aux++].values[ind] = node[atoms[i][0]].value;
					ind++;
				}
				else
				{
					cout << "Warning: Some atoms lack interpretation\n";
					return -1;
				}
			}
			i++;
		}
		return ind;
	}
	void recTableExp(int pos, int ins, char s[])
	{ // truns a tree or subtree back into a strong expression
		if (node[pos].symbol == '!')
		{
			insertChar(s, '!', ins);
			insertChar(s, ')', ins + 1);
			insertChar(s, '(', ins);
			recTableExp(node[pos].leaf[0], ins + 2, s);
		}
		else if (node[pos].symbol == '^' || node[pos].symbol == 'v' || node[pos].symbol == '>' || node[pos].symbol == '=' || node[pos].symbol == '|' || node[pos].symbol == '*')
		{
			insertChar(s, node[pos].symbol, ins);
			insertChar(s, ')', ins + 1);
			insertChar(s, '(', ins);

			recTableExp(node[pos].leaf[1], ins + 2, s);
			recTableExp(node[pos].leaf[0], ins + 1, s);
		}
		else if (node[pos].symbol >= 'A' && node[pos].symbol <= 'Z' || node[pos].symbol == 'f' || node[pos].symbol == 't')
			insertChar(s, node[pos].symbol, ins);
	}
	void recTableValueExp(int pos, int ins, char s[])
	{ // truns a tree or subtree back into a strong expression, stoping at values -- UNUSED
		if (node[pos].symbol == '!')
		{
			if (node[pos].isInterpreted)
			{
				if (node[pos].value)
					insertChar(s, '1', ins);
				else
					insertChar(s, '0', ins);
			}
			else
			{
				insertChar(s, '!', ins);
				insertChar(s, ')', ins + 1);
				insertChar(s, '(', ins);
			}
			recTableExp(node[pos].leaf[0], ins + 2, s);
		}
		else if (node[pos].symbol == '^' || node[pos].symbol == 'v' || node[pos].symbol == '>' || node[pos].symbol == '=' || node[pos].symbol == '|' || node[pos].symbol == '*')
		{
			if (node[pos].isInterpreted)
			{
				if (node[pos].value)
					insertChar(s, '1', ins);
				else
					insertChar(s, '0', ins);
			}
			else
			{
				insertChar(s, node[pos].symbol, ins);
				insertChar(s, ')', ins + 1);
				insertChar(s, '(', ins);

			}
			recTableExp(node[pos].leaf[1], ins + 2, s);
			recTableExp(node[pos].leaf[0], ins + 1, s);
		}
		else if (node[pos].symbol >= 'A' && node[pos].symbol <= 'Z' || node[pos].symbol == 'f' || node[pos].symbol == 't')
		{
			if (node[pos].isInterpreted)
			{
				if (node[pos].value)
					insertChar(s, '1', ins);
				else
					insertChar(s, '0', ins);
			}
			else
				insertChar(s, node[pos].symbol, ins);
		}
	}
	void getTableHeader(int pos, int &tablePos)
	{ //fills the table header, doesnt add atoms
		if (node[pos].symbol == '!')
		{
			table[tablePos].exp[0] = 0;
			recTableExp(pos, 0, table[tablePos--].exp);

			getTableHeader(node[pos].leaf[0], tablePos);
		}
		else if (node[pos].symbol == '^' || node[pos].symbol == 'v' || node[pos].symbol == '>' || node[pos].symbol == '=' || node[pos].symbol == '|' || node[pos].symbol == '*')
		{
			table[tablePos].exp[0] = 0;
			recTableExp(pos, 0, table[tablePos--].exp);

			getTableHeader(node[pos].leaf[0], tablePos);
			getTableHeader(node[pos].leaf[1], tablePos);
		}
	}
	int buildTable()
	{ //builds a table by getting the header (axcepting atoms) then adding atoms from atom list and removing spaces(if an atom si used twice a space is wrongfully created)
		int treeLen = getFreeNode() - 1, l = 0;
		getTableHeader(0, treeLen);

		for (int i = 0; i < 26; i++)
			if (atoms[i][0] != -1)
				table[l++].exp[0] = i + 'A';

		treeLen = getFreeNode();
		int tableLen = treeLen;
		for (int i = 0; i < treeLen; i++)
			if (table[i].exp[0] == 0)
			{
				for (int j = i; j + 1 < treeLen; j++)
					table[j] = table[j + 1];
				i--;
				tableLen--;
			}
		return tableLen;
	}
	void printTable(int tableLen, int interpretationCnt)
	{ //tedious low level formating...
		//header
		cout << 'É';
		for (int i = 0; i < tableLen - 1; i++)
		{
			for (int j = 0; table[i].exp[j]; j++)
				cout << 'Í';
			cout << 'Ñ';
		}
		for (int j = 0; table[tableLen - 1].exp[j]; j++)
			cout << 'Í';
		cout << "»\nº";
		for (int i = 0; i < tableLen - 1; i++)
			cout << table[i].exp << '³';
		cout << table[tableLen - 1].exp << "º\nÌ";
		for (int i = 0; i < tableLen - 1; i++)
		{
			for (int j = 0; table[i].exp[j]; j++)
				cout << 'Í';
			cout << 'Ø';
		}
		for (int j = 0; table[tableLen - 1].exp[j]; j++)
			cout << 'Í';
		cout << "¹\n";

		for (int i = 0; i < interpretationCnt - 1; i++)
		{
			//value line
			cout << 'º';
			for (int j = 0; j < tableLen - 1; j++)
			{
				for (int k = 0; k < strlen(table[j].exp) - 1; k++)
					cout << ' ';
				cout << table[j].values[i] << '³';
			}
			for (int k = 0; k < strlen(table[tableLen - 1].exp) - 1; k++)
				cout << ' ';
			cout << table[tableLen - 1].values[i] << "º\n";
			//table line
			cout << 'Ç';
			for (int j = 0; j < tableLen - 1; j++)
			{
				for (int k = 0; k < strlen(table[j].exp); k++)
					cout << 'Ä';
				cout << 'Å';
			}
			for (int k = 0; k < strlen(table[tableLen - 1].exp); k++)
				cout << 'Ä';
			cout << "¶\n";
		}
		//final value line
		cout << 'º';
		for (int j = 0; j < tableLen - 1; j++)
		{
			for (int k = 0; k < strlen(table[j].exp) - 1; k++)
				cout << ' ';
			cout << table[j].values[interpretationCnt - 1] << '³';
		}
		for (int k = 0; k < strlen(table[tableLen - 1].exp) - 1; k++)
			cout << ' ';
		cout << table[tableLen - 1].values[interpretationCnt - 1] << "º\n";
		//final table line
		cout << 'È';
		for (int j = 0; j < tableLen - 1; j++)
		{
			for (int k = 0; k < strlen(table[j].exp); k++)
				cout << 'Í';
			cout << 'Ï';
		}
		for (int k = 0; k < strlen(table[tableLen - 1].exp); k++)
			cout << 'Í';
		cout << "¼\n";
	}

	//normal form stuff
	void deleteTree(int pos)
	{
		if (node[pos].symbol == '!')
			deleteTree(node[pos].leaf[0]);
		else if (node[pos].symbol == '^' || node[pos].symbol == 'v' || node[pos].symbol == '>' || node[pos].symbol == '=' || node[pos].symbol == '|' || node[pos].symbol == '*')
		{
			deleteTree(node[pos].leaf[0]);
			deleteTree(node[pos].leaf[1]);
		}
		node[pos].deconstruct();
	}
	bool compareValues(int posTree1, int posTree2)
	{
		char s[MAX_STRING] = { 0 };
		getAllInterpretations(getAtomCount(), s);

		bool isEqual = true;
		int i = 0;
		while (s[i] != 0)// read interpretations
		{
			while (s[i] == ' ') i++; //ignore spaces

			if (s[i] == '{')
			{
				bool b = true;
				while (s[i] && s[i] != '}') //reading an interpretation
				{
					i++;
					if (s[i] == '!')
						b = false;
					else if (s[i] >= 'A' && s[i] <= 'Z')
					{
						if (atoms[s[i] - 'A'][0] != -1)
						{
							int k = 0;
							while (k < 100 && atoms[s[i] - 'A'][k] != -1)
							{
								node[atoms[s[i] - 'A'][k]].addInterpret(b);
								k++;
							}
							b = true;
						}

					}
				}
				if (recInt(posTree1) != recInt(posTree2))
				{
					isEqual = false;
					break;
				}
			}
			i++;
		}
		return isEqual;
	}
	bool reverseCompareValues(int posTree1, int posTree2)
	{
		char s[MAX_STRING] = { 0 };
		getAllInterpretations(getAtomCount(), s);

		bool isEqual = true;
		int i = 0;
		while (s[i] != 0)// read interpretations
		{
			while (s[i] == ' ') i++; //ignore spaces

			if (s[i] == '{')
			{
				bool b = true;
				while (s[i] && s[i] != '}') //reading an interpretation
				{
					i++;
					if (s[i] == '!')
						b = false;
					else if (s[i] >= 'A' && s[i] <= 'Z')
					{
						if (atoms[s[i] - 'A'][0] != -1)
						{
							int k = 0;
							while (k < 100 && atoms[s[i] - 'A'][k] != -1)
							{
								node[atoms[s[i] - 'A'][k]].addInterpret(b);
								k++;
							}
							b = true;
						}
					}
				}
				if (recInt(posTree1) == recInt(posTree2))
				{
					isEqual = false;
					break;
				}
			}
			i++;
		}
		return isEqual;
	}
	bool isTautology(int posTree1)
	{
		char s[MAX_STRING] = { 0 };

		getAllInterpretations(getAtomCount(), s);

		bool isEqual = true;
		int i = 0;
		while (s[i] != 0)// read interpretations
		{
			while (s[i] == ' ') i++; //ignore spaces

			if (s[i] == '{')
			{
				bool b = true;
				while (s[i] && s[i] != '}') //reading an interpretation
				{
					i++;
					if (s[i] == '!')
						b = false;
					else if (s[i] >= 'A' && s[i] <= 'Z')
					{
						if (atoms[s[i] - 'A'][0] != -1)
						{
							int k = 0;
							while (k < 100 && atoms[s[i] - 'A'][k] != -1)
							{
								node[atoms[s[i] - 'A'][k]].addInterpret(b);
								k++;
							}
							b = true;
						}
					}
				}
				if (recInt(posTree1) == 0)
				{
					isEqual = false;
					break;
				}
			}
			i++;
		}
		return isEqual;
	}
	bool isContradiction(int posTree1)
	{
		char s[MAX_STRING] = { 0 };
		getAllInterpretations(getAtomCount(), s);

		bool isEqual = true;
		int i = 0;
		while (s[i] != 0)// read interpretations
		{
			while (s[i] == ' ') i++; //ignore spaces

			if (s[i] == '{')
			{
				bool b = true;
				while (s[i] && s[i] != '}') //reading an interpretation
				{
					i++;
					if (s[i] == '!')
						b = false;
					else if (s[i] >= 'A' && s[i] <= 'Z')
					{
						if (atoms[s[i] - 'A'][0] != -1)
						{
							int k = 0;
							while (k < 100 && atoms[s[i] - 'A'][k] != -1)
							{
								node[atoms[s[i] - 'A'][k]].addInterpret(b);
								k++;
							}
							b = true;
						}
					}
				}
				if (recInt(posTree1) == 1)
				{
					isEqual = false;
					break;
				}
			}
			i++;
		}
		return isEqual;
	}
	void simplify(int pos, int &startPos, bool &isChange)
	{
		bool verbose = false;
		if (VERBOSE_LEVEL > 1)
			verbose = true;

		if (node[pos].symbol == '^')
		{
			if (reverseCompareValues(node[pos].leaf[0], node[pos].leaf[1])) //annihilation
			{
				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(node[pos].leaf[0], 0, exp);
					cout << "Annihilation: " << exp << " ^ ";
					memset(exp, 0, 1001);
					recTableExp(node[pos].leaf[1], 0, exp);
					cout << exp << " -> ";
				}

				node[pos].symbol = 'f';
				deleteTree(node[pos].leaf[0]);
				deleteTree(node[pos].leaf[1]);
				node[pos].leaf[0] = -1;
				node[pos].leaf[1] = -1;
				isChange = true;

				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(pos, 0, exp);
					cout << exp << '\n';
				}
			}
			else if (node[node[pos].leaf[0]].symbol == 't' || isTautology(node[pos].leaf[0])) // conjunction with tautology 1
			{
				if (verbose)
				{
					char exp[1000] = { 0 };
					recTableExp(node[pos].leaf[0], 0, exp);
					cout << "Conjunction with tautology: " << exp << " ^ ";
					memset(exp, 0, 1000);
					recTableExp(node[pos].leaf[1], 0, exp);
					cout << exp << " -> ";
				}

				int endPos = node[pos].leaf[1];
				node[endPos].father = node[pos].father;
				if (node[pos].father == -1)
					startPos = endPos;
				if (node[pos].father != -1)
				{
					if (pos == node[node[pos].father].leaf[1])
						node[node[pos].father].leaf[1] = endPos;
					else
						node[node[pos].father].leaf[0] = endPos;
					deleteTree(node[pos].leaf[0]);
				}
				else
				{
					deleteTree(node[pos].leaf[0]);
					node[pos].deconstruct();
				}
				pos = endPos;
				isChange = true;

				if (verbose)
				{
					char exp[1000] = { 0 };
					recTableExp(pos, 0, exp);
					cout << exp << '\n';
				}
			}
			else if (node[node[pos].leaf[1]].symbol == 't' || isTautology(node[pos].leaf[1])) // conjunction with tautology 2
			{
				if (verbose)
				{
					char exp[1000] = { 0 };
					recTableExp(node[pos].leaf[0], 0, exp);
					cout << "Conjunction with tautology: " << exp << " ^ ";
					memset(exp, 0, 1000);
					recTableExp(node[pos].leaf[1], 0, exp);
					cout << exp << " -> ";
				}
				int endPos = node[pos].leaf[0];
				node[endPos].father = node[pos].father;
				if (node[pos].father == -1)
					startPos = endPos;
				if (node[pos].father != -1)
				{
					if (pos == node[node[pos].father].leaf[1])
						node[node[pos].father].leaf[1] = endPos;
					else
						node[node[pos].father].leaf[0] = endPos;
					deleteTree(node[pos].leaf[1]);
				}
				else
				{
					deleteTree(node[pos].leaf[1]);
					node[pos].deconstruct();
				}
				pos = endPos;
				isChange = true;

				if (verbose)
				{
					char exp[1000] = { 0 };
					recTableExp(pos, 0, exp);
					cout << exp << '\n';
				}
			}
			else if (node[node[pos].leaf[0]].symbol == 'f' || node[node[pos].leaf[1]].symbol == 'f' || isContradiction(node[pos].leaf[0]) || isContradiction(node[pos].leaf[1])) // conjunction with contradiction
			{
				if (verbose)
				{
					char exp[1000] = { 0 };
					recTableExp(node[pos].leaf[0], 0, exp);
					cout << "Conjunction with contradiction: " << exp << " ^ ";
					memset(exp, 0, 1000);
					recTableExp(node[pos].leaf[1], 0, exp);
					cout << exp << " -> ";
				}
				node[pos].symbol = 'f';
				node[node[pos].leaf[0]].deconstruct();
				node[node[pos].leaf[1]].deconstruct();
				node[pos].leaf[0] = -1;
				node[pos].leaf[1] = -1;

				isChange = true;

				if (verbose)
				{
					char exp[1000] = { 0 };
					recTableExp(pos, 0, exp);
					cout << exp << '\n';
				}
			}
			else if (compareValues(node[pos].leaf[0], node[pos].leaf[1])) //idempocy simplification
			{
				if (verbose)
				{
					char exp[1000] = { 0 };
					recTableExp(node[pos].leaf[0], 0, exp);
					cout << "Idempocy: " << exp << " ^ ";
					memset(exp, 0, 1000);
					recTableExp(node[pos].leaf[1], 0, exp);
					cout << exp << " -> ";
				}
				int endPos = node[pos].leaf[0];
				node[endPos].father = node[pos].father;
				if (node[pos].father == -1)
					startPos = endPos;
				if (node[pos].father != -1)
				{
					if (pos == node[node[pos].father].leaf[1])
						node[node[pos].father].leaf[1] = endPos;
					else
						node[node[pos].father].leaf[0] = endPos;
					deleteTree(node[pos].leaf[1]);
				}
				else
				{
					deleteTree(node[pos].leaf[1]);
					node[pos].deconstruct();
				}
				pos = endPos;
				isChange = true;

				if (verbose)
				{
					char exp[1000] = { 0 };
					recTableExp(pos, 0, exp);
					cout << exp << '\n';
				}
			}
			else if (node[node[pos].leaf[0]].symbol == 'v') //absorbtion 1
			{
				if (compareValues(node[pos].leaf[1], node[node[pos].leaf[0]].leaf[0])) //side 1A
				{
					if (verbose)
					{
						char exp[1000] = { 0 };
						recTableExp(node[pos].leaf[0], 0, exp);
						cout << "Absorbtion: " << exp << " ^ ";
						memset(exp, 0, 1000);
						recTableExp(node[pos].leaf[1], 0, exp);
						cout << exp << " -> ";
					}

					int endPos = node[pos].leaf[1];
					node[endPos].father = node[pos].father;
					if (node[pos].father == -1)
						startPos = endPos;
					if (node[pos].father != -1)
					{
						if (pos == node[node[pos].father].leaf[1])
							node[node[pos].father].leaf[1] = endPos;
						else
							node[node[pos].father].leaf[0] = endPos;
						deleteTree(node[pos].leaf[0]);
					}
					else
					{
						deleteTree(node[pos].leaf[0]);
						node[pos].deconstruct();
					}
					pos = endPos;
					isChange = true;

					if (verbose)
					{
						char exp[1000] = { 0 };
						recTableExp(pos, 0, exp);
						cout << exp << '\n';
					}
				}
				else if (compareValues(node[pos].leaf[1], node[node[pos].leaf[0]].leaf[1])) //side 1B
				{
					if (verbose)
					{
						char exp[1000] = { 0 };
						recTableExp(node[pos].leaf[0], 0, exp);
						cout << "Absorbtion: " << exp << " ^ ";
						memset(exp, 0, 1000);
						recTableExp(node[pos].leaf[1], 0, exp);
						cout << exp << " -> ";
					}

					int endPos = node[pos].leaf[1];
					node[endPos].father = node[pos].father;
					if (node[pos].father == -1)
						startPos = endPos;
					if (node[pos].father != -1)
					{
						if (pos == node[node[pos].father].leaf[1])
							node[node[pos].father].leaf[1] = endPos;
						else
							node[node[pos].father].leaf[0] = endPos;
						deleteTree(node[pos].leaf[0]);
					}
					else
					{
						deleteTree(node[pos].leaf[0]);
						node[pos].deconstruct();
					}
					pos = endPos;
					isChange = true;

					if (verbose)
					{
						char exp[1000] = { 0 };
						recTableExp(pos, 0, exp);
						cout << exp << '\n';
					}
				}
			}
			else if (node[node[pos].leaf[1]].symbol == 'v') // absorbtion 2
			{
				if (compareValues(node[pos].leaf[0], node[node[pos].leaf[1]].leaf[0])) //side 2A
				{
					if (verbose)
					{
						char exp[1000] = { 0 };
						recTableExp(node[pos].leaf[0], 0, exp);
						cout << "Absorbtion: " << exp << " ^ ";
						memset(exp, 0, 1000);
						recTableExp(node[pos].leaf[1], 0, exp);
						cout << exp << " -> ";
					}

					int endPos = node[pos].leaf[0];
					node[endPos].father = node[pos].father;
					if (node[pos].father == -1)
						startPos = endPos;
					if (node[pos].father != -1)
					{
						if (pos == node[node[pos].father].leaf[1])
							node[node[pos].father].leaf[1] = endPos;
						else
							node[node[pos].father].leaf[0] = endPos;
						deleteTree(node[pos].leaf[1]);
					}
					else
					{
						deleteTree(node[pos].leaf[1]);
						node[pos].deconstruct();
					}
					pos = endPos;
					isChange = true;

					if (verbose)
					{
						char exp[1000] = { 0 };
						recTableExp(pos, 0, exp);
						cout << exp << '\n';
					}
				}
				else if (compareValues(node[pos].leaf[0], node[node[pos].leaf[1]].leaf[1])) //side 2B
				{
					if (verbose)
					{
						char exp[1000] = { 0 };
						recTableExp(node[pos].leaf[0], 0, exp);
						cout << "Absorbtion: " << exp << " ^ ";
						memset(exp, 0, 1000);
						recTableExp(node[pos].leaf[1], 0, exp);
						cout << exp << " -> ";
					}

					int endPos = node[pos].leaf[0];
					node[endPos].father = node[pos].father;
					if (node[pos].father == -1)
						startPos = endPos;
					if (node[pos].father != -1)
					{
						if (pos == node[node[pos].father].leaf[1])
							node[node[pos].father].leaf[1] = endPos;
						else
							node[node[pos].father].leaf[0] = endPos;
						deleteTree(node[pos].leaf[1]);
					}
					else
					{
						deleteTree(node[pos].leaf[1]);
						node[pos].deconstruct();
					}
					pos = endPos;
					isChange = true;

					if (verbose)
					{
						char exp[1000] = { 0 };
						recTableExp(pos, 0, exp);
						cout << exp << '\n';
					}
				}
			}
		}

		if (node[pos].symbol == 'v')
		{
			if (reverseCompareValues(node[pos].leaf[0], node[pos].leaf[1]))
			{
				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(node[pos].leaf[0], 0, exp);
					cout << "Annihilation: " << exp << " v ";
					memset(exp, 0, 1001);
					recTableExp(node[pos].leaf[1], 0, exp);
					cout << exp << " -> ";
				}

				node[pos].symbol = 't';
				deleteTree(node[pos].leaf[0]);
				deleteTree(node[pos].leaf[1]);
				node[pos].leaf[0] = -1;
				node[pos].leaf[1] = -1;
				isChange = true;

				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(pos, 0, exp);
					cout << exp << '\n';
				}
			}
			else if (node[node[pos].leaf[0]].symbol == 'f' || isContradiction(node[pos].leaf[0])) // disjunction with contradiction 1
			{
				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(node[pos].leaf[0], 0, exp);
					cout << "Disjunction with contradiction: " << exp << " v ";
					memset(exp, 0, 1001);
					recTableExp(node[pos].leaf[1], 0, exp);
					cout << exp << " -> ";
				}

				int endPos = node[pos].leaf[1];
				node[endPos].father = node[pos].father;
				if (node[pos].father == -1)
					startPos = endPos;
				if (node[pos].father != -1)
				{
					if (pos == node[node[pos].father].leaf[1])
						node[node[pos].father].leaf[1] = endPos;
					else
						node[node[pos].father].leaf[0] = endPos;
					deleteTree(node[pos].leaf[0]);
				}
				else
				{
					deleteTree(node[pos].leaf[0]);
					node[pos].deconstruct();
				}
				pos = endPos;
				isChange = true;

				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(pos, 0, exp);
					cout << exp << '\n';
				}
			}
			else if (node[node[pos].leaf[1]].symbol == 'f' || isContradiction(node[pos].leaf[1])) // conjunction with contradiction 2
			{
				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(node[pos].leaf[0], 0, exp);
					cout << "Disjunction with contradiction: " << exp << " v ";
					memset(exp, 0, 1001);
					recTableExp(node[pos].leaf[1], 0, exp);
					cout << exp << " -> ";
				}
				int endPos = node[pos].leaf[0];
				node[endPos].father = node[pos].father;
				if (node[pos].father == -1)
					startPos = endPos;
				if (node[pos].father != -1)
				{
					if (pos == node[node[pos].father].leaf[1])
						node[node[pos].father].leaf[1] = endPos;
					else
						node[node[pos].father].leaf[0] = endPos;
					deleteTree(node[pos].leaf[1]);
				}
				else
				{
					deleteTree(node[pos].leaf[1]);
					node[pos].deconstruct();
				}
				pos = endPos;
				isChange = true;

				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(pos, 0, exp);
					cout << exp << '\n';
				}
			}
			else if (node[node[pos].leaf[0]].symbol == 't' || node[node[pos].leaf[1]].symbol == 't' || isTautology(node[pos].leaf[0]) || isTautology(node[pos].leaf[1])) // disjunction with contradiction
			{
				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(node[pos].leaf[0], 0, exp);
					cout << "Disjunction with tautology: " << exp << " v ";
					memset(exp, 0, 1001);
					recTableExp(node[pos].leaf[1], 0, exp);
					cout << exp << " -> ";
				}

				node[pos].symbol = 't';
				node[node[pos].leaf[0]].deconstruct();
				node[node[pos].leaf[1]].deconstruct();
				node[pos].leaf[0] = -1;
				node[pos].leaf[1] = -1;

				isChange = true;

				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(pos, 0, exp);
					cout << exp << '\n';
				}
			}
			else if (compareValues(node[pos].leaf[0], node[pos].leaf[1])) //idempocy simplification
			{
				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(node[pos].leaf[0], 0, exp);
					cout << "Idempocy: " << exp << " v ";
					memset(exp, 0, 1001);
					recTableExp(node[pos].leaf[1], 0, exp);
					cout << exp << " -> ";
				}

				int endPos = node[pos].leaf[0];
				node[endPos].father = node[pos].father;
				if (node[pos].father == -1)
					startPos = endPos;
				if (node[pos].father != -1)
				{
					if (pos == node[node[pos].father].leaf[1])
						node[node[pos].father].leaf[1] = endPos;
					else
						node[node[pos].father].leaf[0] = endPos;
					deleteTree(node[pos].leaf[1]);
				}
				else
				{
					deleteTree(node[pos].leaf[1]);
					node[pos].deconstruct();
				}
				pos = endPos;
				isChange = true;

				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(pos, 0, exp);
					cout << exp << '\n';
				}
			}
			else if (node[node[pos].leaf[0]].symbol == '^') //absorbtion 1
			{
				if (compareValues(node[pos].leaf[1], node[node[pos].leaf[0]].leaf[0])) //side 1A
				{
					if (verbose)
					{
						char exp[1001] = { 0 };
						recTableExp(node[pos].leaf[0], 0, exp);
						cout << "Absorbtion: " << exp << " v ";
						memset(exp, 0, 1001);
						recTableExp(node[pos].leaf[1], 0, exp);
						cout << exp << " -> ";
					}

					int endPos = node[pos].leaf[1];
					node[endPos].father = node[pos].father;
					if (node[pos].father == -1)
						startPos = endPos;
					if (node[pos].father != -1)
					{
						if (pos == node[node[pos].father].leaf[1])
							node[node[pos].father].leaf[1] = endPos;
						else
							node[node[pos].father].leaf[0] = endPos;
						deleteTree(node[pos].leaf[0]);
					}
					else
					{
						deleteTree(node[pos].leaf[0]);
						node[pos].deconstruct();
					}
					pos = endPos;
					isChange = true;

					if (verbose)
					{
						char exp[1001] = { 0 };
						recTableExp(pos, 0, exp);
						cout << exp << '\n';
					}
				}
				else if (compareValues(node[pos].leaf[1], node[node[pos].leaf[0]].leaf[1])) //side 1B
				{
					if (verbose)
					{
						char exp[1001] = { 0 };
						recTableExp(node[pos].leaf[0], 0, exp);
						cout << "Absorbtion: " << exp << " v ";
						memset(exp, 0, 1001);
						recTableExp(node[pos].leaf[1], 0, exp);
						cout << exp << " -> ";
					}

					int endPos = node[pos].leaf[1];
					node[endPos].father = node[pos].father;
					if (node[pos].father == -1)
						startPos = endPos;
					if (node[pos].father != -1)
					{
						if (pos == node[node[pos].father].leaf[1])
							node[node[pos].father].leaf[1] = endPos;
						else
							node[node[pos].father].leaf[0] = endPos;
						deleteTree(node[pos].leaf[0]);
					}
					else
					{
						deleteTree(node[pos].leaf[0]);
						node[pos].deconstruct();
					}
					pos = endPos;
					isChange = true;

					if (verbose)
					{
						char exp[1001] = { 0 };
						recTableExp(pos, 0, exp);
						cout << exp << '\n';
					}
				}
			}
			else if (node[node[pos].leaf[1]].symbol == '^') // absorbtion 2
			{
				if (compareValues(node[pos].leaf[0], node[node[pos].leaf[1]].leaf[0])) //side 2A
				{
					if (verbose)
					{
						char exp[1001] = { 0 };
						recTableExp(node[pos].leaf[0], 0, exp);
						cout << "Absorbtion: " << exp << " v ";
						memset(exp, 0, 1001);
						recTableExp(node[pos].leaf[1], 0, exp);
						cout << exp << " -> ";
					}

					int endPos = node[pos].leaf[0];
					node[endPos].father = node[pos].father;
					if (node[pos].father == -1)
						startPos = endPos;
					if (node[pos].father != -1)
					{
						if (pos == node[node[pos].father].leaf[1])
							node[node[pos].father].leaf[1] = endPos;
						else
							node[node[pos].father].leaf[0] = endPos;
						deleteTree(node[pos].leaf[1]);
					}
					else
					{
						deleteTree(node[pos].leaf[1]);
						node[pos].deconstruct();
					}
					pos = endPos;
					isChange = true;

					if (verbose)
					{
						char exp[1001] = { 0 };
						recTableExp(pos, 0, exp);
						cout << exp << '\n';
					}
				}
				else if (compareValues(node[pos].leaf[0], node[node[pos].leaf[1]].leaf[1])) //side 2B
				{
					if (verbose)
					{
						char exp[1001] = { 0 };
						recTableExp(node[pos].leaf[0], 0, exp);
						cout << "Absorbtion: " << exp << " v ";
						memset(exp, 0, 1001);
						recTableExp(node[pos].leaf[1], 0, exp);
						cout << exp << " -> ";
					}

					int endPos = node[pos].leaf[0];
					node[endPos].father = node[pos].father;
					if (node[pos].father == -1)
						startPos = endPos;
					if (node[pos].father != -1)
					{
						if (pos == node[node[pos].father].leaf[1])
							node[node[pos].father].leaf[1] = endPos;
						else
							node[node[pos].father].leaf[0] = endPos;
						deleteTree(node[pos].leaf[1]);
					}
					else
					{
						deleteTree(node[pos].leaf[1]);
						node[pos].deconstruct();
					}
					pos = endPos;
					isChange = true;

					if (verbose)
					{
						char exp[1001] = { 0 };
						recTableExp(pos, 0, exp);
						cout << exp << '\n';
					}
				}
			}
		}

		if (node[pos].symbol == '=')
		{
			if (compareValues(node[pos].leaf[0], node[pos].leaf[1]))
			{
				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(node[pos].leaf[0], 0, exp);
					cout << "Annihilation: " << exp << " = ";
					memset(exp, 0, 1001);
					recTableExp(node[pos].leaf[1], 0, exp);
					cout << exp << " -> ";
				}

				node[pos].symbol = 't';
				deleteTree(node[pos].leaf[0]);
				deleteTree(node[pos].leaf[1]);
				node[pos].leaf[0] = -1;
				node[pos].leaf[1] = -1;

				isChange = true;

				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(pos, 0, exp);
					cout << exp << '\n';
				}
			}
			else if (node[node[pos].leaf[0]].symbol == '^') //other 1
			{
				if (compareValues(node[pos].leaf[1], node[node[pos].leaf[0]].leaf[0])) //side 1A
				{
					if (verbose)
					{
						char exp[1001] = { 0 };
						recTableExp(node[pos].leaf[0], 0, exp);
						cout << "Other: " << exp << " = ";
						memset(exp, 0, 1001);
						recTableExp(node[pos].leaf[1], 0, exp);
						cout << exp << " -> ";
					}

					node[pos].symbol = '>';
					int endPos = node[node[pos].leaf[0]].leaf[1];
					deleteTree(node[node[pos].leaf[0]].leaf[0]);
					node[node[pos].leaf[0]].deconstruct();

					node[pos].leaf[0] = node[pos].leaf[1];
					node[pos].leaf[1] = endPos;
					node[endPos].father = pos;
					startPos = pos;
					isChange = true;

					if (verbose)
					{
						char exp[1001] = { 0 };
						recTableExp(pos, 0, exp);
						cout << exp << '\n';
					}
				}
				else if (compareValues(node[pos].leaf[1], node[node[pos].leaf[0]].leaf[1])) //side 1B
				{
					if (verbose)
					{
						char exp[1001] = { 0 };
						recTableExp(node[pos].leaf[0], 0, exp);
						cout << "Other: " << exp << " = ";
						memset(exp, 0, 1001);
						recTableExp(node[pos].leaf[1], 0, exp);
						cout << exp << " -> ";
					}

					node[pos].symbol = '>';
					int endPos = node[node[pos].leaf[0]].leaf[0];
					deleteTree(node[node[pos].leaf[0]].leaf[1]);
					node[node[pos].leaf[0]].deconstruct();

					node[pos].leaf[0] = node[pos].leaf[1];
					node[pos].leaf[1] = endPos;
					node[endPos].father = pos;
					startPos = pos;
					isChange = true;

					if (verbose)
					{
						char exp[1001] = { 0 };
						recTableExp(pos, 0, exp);
						cout << exp << '\n';
					}
				}
			}
			else if (node[node[pos].leaf[1]].symbol == '^') //other 2
			{
				if (compareValues(node[pos].leaf[0], node[node[pos].leaf[1]].leaf[0])) //side 2A
				{
					if (verbose)
					{
						char exp[1001] = { 0 };
						recTableExp(node[pos].leaf[0], 0, exp);
						cout << "Other: " << exp << " = ";
						memset(exp, 0, 1001);
						recTableExp(node[pos].leaf[1], 0, exp);
						cout << exp << " -> ";
					}

					node[pos].symbol = '>';
					int endPos = node[node[pos].leaf[1]].leaf[1];
					deleteTree(node[node[pos].leaf[1]].leaf[0]);
					node[node[pos].leaf[1]].deconstruct();

					node[pos].leaf[1] = endPos;
					node[endPos].father = pos;
					startPos = pos;
					isChange = true;

					if (verbose)
					{
						char exp[1001] = { 0 };
						recTableExp(pos, 0, exp);
						cout << exp << '\n';
					}
				}
				else if (compareValues(node[pos].leaf[0], node[node[pos].leaf[1]].leaf[1])) //side 2B
				{
					if (verbose)
					{
						char exp[1001] = { 0 };
						recTableExp(node[pos].leaf[0], 0, exp);
						cout << "Other: " << exp << " = ";
						memset(exp, 0, 1001);
						recTableExp(node[pos].leaf[1], 0, exp);
						cout << exp << " -> ";
					}

					node[pos].symbol = '>';
					int endPos = node[node[pos].leaf[1]].leaf[0];
					deleteTree(node[node[pos].leaf[1]].leaf[1]);
					node[node[pos].leaf[1]].deconstruct();

					node[pos].leaf[1] = endPos;
					node[endPos].father = pos;
					startPos = pos;
					isChange = true;

					if (verbose)
					{
						char exp[1001] = { 0 };
						recTableExp(pos, 0, exp);
						cout << exp << '\n';
					}
				}
			}
			else if (node[node[pos].leaf[0]].symbol == 'v') //other 1
			{
				if (compareValues(node[pos].leaf[1], node[node[pos].leaf[0]].leaf[0])) //side 1A
				{
					if (verbose)
					{
						char exp[1001] = { 0 };
						recTableExp(node[pos].leaf[0], 0, exp);
						cout << "Other: " << exp << " = ";
						memset(exp, 0, 1001);
						recTableExp(node[pos].leaf[1], 0, exp);
						cout << exp << " -> ";
					}

					node[pos].symbol = '>';
					int endPos = node[node[pos].leaf[0]].leaf[1];
					deleteTree(node[node[pos].leaf[0]].leaf[0]);
					node[node[pos].leaf[0]].deconstruct();

					node[pos].leaf[0] = endPos;
					node[endPos].father = pos;
					startPos = pos;
					isChange = true;

					if (verbose)
					{
						char exp[1001] = { 0 };
						recTableExp(pos, 0, exp);
						cout << exp << '\n';
					}
				}
				else if (compareValues(node[pos].leaf[1], node[node[pos].leaf[0]].leaf[1])) //side 1B
				{
					if (verbose)
					{
						char exp[1001] = { 0 };
						recTableExp(node[pos].leaf[0], 0, exp);
						cout << "Other: " << exp << " = ";
						memset(exp, 0, 1001);
						recTableExp(node[pos].leaf[1], 0, exp);
						cout << exp << " -> ";
					}

					node[pos].symbol = '>';
					int endPos = node[node[pos].leaf[0]].leaf[0];
					deleteTree(node[node[pos].leaf[0]].leaf[1]);
					node[node[pos].leaf[0]].deconstruct();

					node[pos].leaf[0] = endPos;
					node[endPos].father = pos;
					startPos = pos;
					isChange = true;

					if (verbose)
					{
						char exp[1001] = { 0 };
						recTableExp(pos, 0, exp);
						cout << exp << '\n';
					}
				}
			}
			else if (node[node[pos].leaf[1]].symbol == 'v') //other 1
			{
				if (compareValues(node[pos].leaf[0], node[node[pos].leaf[1]].leaf[0])) //side 1A
				{
					if (verbose)
					{
						char exp[1001] = { 0 };
						recTableExp(node[pos].leaf[0], 0, exp);
						cout << "Other: " << exp << " = ";
						memset(exp, 0, 1001);
						recTableExp(node[pos].leaf[1], 0, exp);
						cout << exp << " -> ";
					}

					node[pos].symbol = '>';
					int endPos = node[node[pos].leaf[1]].leaf[1];
					deleteTree(node[node[pos].leaf[1]].leaf[0]);
					node[node[pos].leaf[1]].deconstruct();

					node[pos].leaf[1] = node[pos].leaf[0];
					node[pos].leaf[0] = endPos;
					node[endPos].father = pos;
					startPos = pos;
					isChange = true;

					if (verbose)
					{
						char exp[1001] = { 0 };
						recTableExp(pos, 0, exp);
						cout << exp << '\n';
					}
				}
				else if (compareValues(node[pos].leaf[0], node[node[pos].leaf[1]].leaf[1])) //side 1B
				{
					if (verbose)
					{
						char exp[1001] = { 0 };
						recTableExp(node[pos].leaf[0], 0, exp);
						cout << "Other: " << exp << " = ";
						memset(exp, 0, 1001);
						recTableExp(node[pos].leaf[1], 0, exp);
						cout << exp << " -> ";
					}

					node[pos].symbol = '>';
					int endPos = node[node[pos].leaf[1]].leaf[0];
					deleteTree(node[node[pos].leaf[1]].leaf[1]);
					node[node[pos].leaf[1]].deconstruct();

					node[pos].leaf[1] = node[pos].leaf[0];
					node[pos].leaf[0] = endPos;
					node[endPos].father = pos;
					startPos = pos;
					isChange = true;

					if (verbose)
					{
						char exp[1001] = { 0 };
						recTableExp(pos, 0, exp);
						cout << exp << '\n';
					}
				}
			}
		}

		if (node[pos].symbol == '>')
		{
			if (node[node[pos].leaf[0]].symbol == 'f' || isContradiction(node[pos].leaf[0]))
			{
				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(node[pos].leaf[0], 0, exp);
					cout << "Contradiction implies: " << exp << " > ";
					memset(exp, 0, 1001);
					recTableExp(node[pos].leaf[1], 0, exp);
					cout << exp << " -> ";
				}

				node[pos].symbol = 't';
				node[node[pos].leaf[0]].deconstruct();
				node[node[pos].leaf[1]].deconstruct();
				node[pos].leaf[0] = -1;
				node[pos].leaf[1] = -1;

				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(pos, 0, exp);
					cout << exp << '\n';
				}
			}
			else if (node[node[pos].leaf[1]].symbol == 't' || isTautology(node[pos].leaf[1]))
			{
				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(node[pos].leaf[0], 0, exp);
					cout << "Tautology is implied: " << exp << " > ";
					memset(exp, 0, 1001);
					recTableExp(node[pos].leaf[1], 0, exp);
					cout << exp << " -> ";
				}

				node[pos].symbol = 't';
				node[node[pos].leaf[0]].deconstruct();
				node[node[pos].leaf[1]].deconstruct();
				node[pos].leaf[0] = -1;
				node[pos].leaf[1] = -1;

				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(pos, 0, exp);
					cout << exp << '\n';
				}
			}
			else if (compareValues(node[pos].leaf[0], node[pos].leaf[1]))
			{
				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(node[pos].leaf[0], 0, exp);
					cout << "Annihilation: " << exp << " > ";
					memset(exp, 0, 1001);
					recTableExp(node[pos].leaf[1], 0, exp);
					cout << exp << " -> ";
				}

				node[pos].symbol = 't';
				deleteTree(node[pos].leaf[0]);
				deleteTree(node[pos].leaf[1]);
				node[pos].leaf[0] = -1;
				node[pos].leaf[1] = -1;

				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(pos, 0, exp);
					cout << exp << '\n';
				}
			}
		}

		if (node[pos].symbol == '!')
		{
			if (node[node[pos].leaf[0]].symbol == '!') //double negation
			{
				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(pos, 0, exp);
					cout << "Double negation: " << exp << " -> ";
				}

				int endPos = node[node[pos].leaf[0]].leaf[0];
				node[endPos].father = node[pos].father;
				if (node[pos].father != -1)
				{
					if (pos == node[node[pos].father].leaf[1])
						node[node[pos].father].leaf[1] = endPos;
					else
						node[node[pos].father].leaf[0] = endPos;
				}
				else
					startPos = endPos;
				node[node[pos].leaf[0]].deconstruct();
				node[pos].deconstruct();
				pos = endPos;
				isChange = true;

				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(pos, 0, exp);
					cout << exp << '\n';
				}
			}
			else if (node[node[pos].leaf[0]].symbol == 't' || isTautology(node[pos].leaf[0]))
			{
				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(pos, 0, exp);
					cout << "Negation of tautology: " << exp << " -> f";
				}

				node[pos].symbol = 'f';
				deleteTree(node[pos].leaf[0]);
				node[pos].leaf[0] = -1;

				isChange = true;
			}
			else if (node[node[pos].leaf[0]].symbol == 'f' || isContradiction(node[pos].leaf[0]))
			{
				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(pos, 0, exp);
					cout << "Negation of contradiction: " << exp << " -> t";
				}

				node[pos].symbol = 't';
				deleteTree(node[pos].leaf[0]);
				node[pos].leaf[0] = -1;

				isChange = true;
			}
		}

		//parsing
		if (node[pos].symbol == '!')
		{
			simplify(node[pos].leaf[0], startPos, isChange);
		}
		else if (node[pos].symbol == '^' || node[pos].symbol == 'v' || node[pos].symbol == '>' || node[pos].symbol == '=' || node[pos].symbol == '*' || node[pos].symbol == '|')
		{
			simplify(node[pos].leaf[0], startPos, isChange);
			simplify(node[pos].leaf[1], startPos, isChange);
		}
	}
	void copyTree(int pos, int &newPos)
	{
		int free = getFreeNode();
		if (newPos == -1) //only do this first time
			newPos = free;
		if (free != -1)
			node[free].construct(node[pos].symbol, node[pos].father, node[pos].leaf[0], node[pos].leaf[1]);

		if (node[pos].symbol == '!')
			copyTree(node[pos].leaf[0], newPos);
		else if (node[pos].symbol == '^' || node[pos].symbol == 'v' || node[pos].symbol == '>' || node[pos].symbol == '=' || node[pos].symbol == '*' || node[pos].symbol == '|')
		{
			copyTree(node[pos].leaf[0], newPos);
			copyTree(node[pos].leaf[1], newPos);
		}
	}
	void eliminateEquiv(int pos, bool &isChange)
	{
		bool verbose = false;
		if (VERBOSE_LEVEL > 1)
			verbose = true;

		if (node[pos].symbol == '=')
		{
			if (verbose)
			{
				char exp[1001] = { 0 };
				recTableExp(node[pos].leaf[0], 0, exp);
				cout << "Reduction: " << exp << " = ";
				memset(exp, 0, 1001);
				recTableExp(node[pos].leaf[1], 0, exp);
				cout << exp << " -> ";
			}

			node[pos].symbol = '^';
			int newStartPos1 = -1, newStartPos2 = -1; //must initialise with -1 for copytree
			copyTree(node[pos].leaf[0], newStartPos1);
			copyTree(node[pos].leaf[1], newStartPos2);

			int impl1 = getFreeNode();
			node[impl1].construct('>', pos, node[pos].leaf[0], node[pos].leaf[1]);
			node[node[pos].leaf[0]].father = impl1;
			node[node[pos].leaf[1]].father = impl1;

			int impl2 = getFreeNode();
			node[impl2].construct('>', pos, newStartPos2, newStartPos1);
			node[newStartPos1].father = impl2;
			node[newStartPos2].father = impl2;

			node[pos].leaf[0] = impl1;
			node[pos].leaf[1] = impl2;

			isChange = true;

			if (verbose)
			{
				char exp[1001] = { 0 };
				recTableExp(pos, 0, exp);
				cout << exp << '\n';
			}
		}

		//parsing
		if (node[pos].symbol == '!')
		{
			eliminateEquiv(node[pos].leaf[0], isChange);
		}
		else if (node[pos].symbol == '^' || node[pos].symbol == 'v' || node[pos].symbol == '>' || node[pos].symbol == '=' || node[pos].symbol == '*' || node[pos].symbol == '|')
		{
			eliminateEquiv(node[pos].leaf[0], isChange);
			eliminateEquiv(node[pos].leaf[1], isChange);
		}
	}
	void eliminateImpl(int pos, bool &isChange)
	{
		bool verbose = false;
		if (VERBOSE_LEVEL > 1)
			verbose = true;


		if (node[pos].symbol == '>')
		{
			if (verbose)
			{
				char exp[1001] = { 0 };
				recTableExp(node[pos].leaf[0], 0, exp);
				cout << "Reduction: " << exp << " > ";
				memset(exp, 0, 1001);
				recTableExp(node[pos].leaf[1], 0, exp);
				cout << exp << " -> ";
			}

			node[pos].symbol = 'v';

			int newPos = getFreeNode();
			node[newPos].construct('!', pos, node[pos].leaf[0], -1);
			node[node[pos].leaf[0]].father = newPos;
			node[pos].leaf[0] = newPos;

			isChange = true;

			if (verbose)
			{
				char exp[1001] = { 0 };
				recTableExp(pos, 0, exp);
				cout << exp << '\n';
			}
		}

		//parsing
		if (node[pos].symbol == '!')
		{
			eliminateImpl(node[pos].leaf[0], isChange);
		}
		else if (node[pos].symbol == '^' || node[pos].symbol == 'v' || node[pos].symbol == '>' || node[pos].symbol == '=' || node[pos].symbol == '*' || node[pos].symbol == '|')
		{
			eliminateImpl(node[pos].leaf[0], isChange);
			eliminateImpl(node[pos].leaf[1], isChange);
		}
	}
	void eliminateBigNeg(int pos, int &startPos, bool &isChange)
	{
		bool verbose = false;
		if (VERBOSE_LEVEL > 1)
			verbose = true;

		if (node[pos].symbol == '!')
		{
			if (node[node[pos].leaf[0]].symbol == 'v')
			{
				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(pos, 0, exp);
					cout << "De Morgan: " << exp << " -> ";
				}

				node[pos].symbol = '^';

				int newPos = getFreeNode();
				node[newPos].construct('!', pos, node[node[pos].leaf[0]].leaf[1], -1);
				node[pos].leaf[1] = newPos;
				node[node[node[pos].leaf[0]].leaf[1]].father = newPos;

				node[node[pos].leaf[0]].symbol = '!';
				node[node[pos].leaf[0]].leaf[1] = -1;

				isChange = true;

				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(pos, 0, exp);
					cout << exp << '\n';
				}
			}
			else if (node[node[pos].leaf[0]].symbol == '^')
			{
				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(pos, 0, exp);
					cout << "De Morgan: " << exp << " -> ";
				}

				node[pos].symbol = 'v';

				int newPos = getFreeNode();
				node[newPos].construct('!', pos, node[node[pos].leaf[0]].leaf[1], -1);
				node[pos].leaf[1] = newPos;
				node[node[node[pos].leaf[0]].leaf[1]].father = newPos;

				node[node[pos].leaf[0]].symbol = '!';
				node[node[pos].leaf[0]].leaf[1] = -1;

				isChange = true;

				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(pos, 0, exp);
					cout << exp << '\n';
				}
			}
			else if (node[node[pos].leaf[0]].symbol == '!') //double negation
			{
				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(pos, 0, exp);
					cout << "Double Negation: " << exp << " -> ";
				}

				int endPos = node[node[pos].leaf[0]].leaf[0];
				node[endPos].father = node[pos].father;
				if (node[pos].father != -1)
				{
					if (pos == node[node[pos].father].leaf[1])
						node[node[pos].father].leaf[1] = endPos;
					else
						node[node[pos].father].leaf[0] = endPos;
				}
				if (node[pos].father == -1)
					startPos = endPos;
				node[node[pos].leaf[0]].deconstruct();
				node[pos].deconstruct();
				pos = endPos;

				isChange = true;

				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(pos, 0, exp);
					cout << exp << '\n';
				}
			}
		}

		//parsing
		if (node[pos].symbol == '!')
		{
			eliminateBigNeg(node[pos].leaf[0], startPos, isChange);
		}
		else if (node[pos].symbol == '^' || node[pos].symbol == 'v' || node[pos].symbol == '>' || node[pos].symbol == '=' || node[pos].symbol == '*' || node[pos].symbol == '|')
		{
			eliminateBigNeg(node[pos].leaf[0], startPos, isChange);
			eliminateBigNeg(node[pos].leaf[1], startPos, isChange);
		}
	}
	void getNNF(char exp[])
	{
		int startPos = 0; bool change = false, s_change = false;

		if (ALLOW_SIMPLIFICATION)
		{
			simplify(0, startPos, s_change);
			while (s_change)
			{
				s_change = false;
				simplify(startPos, startPos, s_change);
			}
		}

		eliminateEquiv(startPos, change);
		clearAtomsList();
		updateAtomsList(startPos);

		if (ALLOW_SIMPLIFICATION)
		{
			simplify(0, startPos, s_change);
			while (s_change)
			{
				s_change = false;
				simplify(startPos, startPos, s_change);
			}
		}

		while (change)
		{
			change = false;
			eliminateEquiv(startPos, change);
			clearAtomsList();
			updateAtomsList(startPos);

			if (ALLOW_SIMPLIFICATION)
			{
				simplify(0, startPos, s_change);
				while (s_change)
				{
					s_change = false;
					simplify(startPos, startPos, s_change);
				}
			}
		}

		eliminateImpl(startPos, change);

		if (ALLOW_SIMPLIFICATION)
		{
			simplify(0, startPos, s_change);
			while (s_change)
			{
				s_change = false;
				simplify(startPos, startPos, s_change);
			}
		}

		while (change)
		{
			change = false;
			eliminateImpl(startPos, change);
			if (ALLOW_SIMPLIFICATION)
			{
				simplify(0, startPos, s_change);
				while (s_change)
				{
					s_change = false;
					simplify(startPos, startPos, s_change);
				}
			}
		}

		eliminateBigNeg(startPos, startPos, change);
		if (ALLOW_SIMPLIFICATION)
		{
			simplify(0, startPos, s_change);
			while (s_change)
			{
				s_change = false;
				simplify(startPos, startPos, s_change);
			}
		}

		while (change)
		{
			change = false;
			eliminateBigNeg(startPos, startPos, change);
			if (ALLOW_SIMPLIFICATION)
			{
				simplify(0, startPos, s_change);
				while (s_change)
				{
					s_change = false;
					simplify(startPos, startPos, s_change);
				}
			}
		}

		defragmentTree(startPos);
		exp[0] = 0;
		recTableExp(0, 0, exp);
	}

	void eliminateBigConj(int pos, bool &isChange)
	{
		bool verbose = false;
		if (VERBOSE_LEVEL > 1)
			verbose = true;

		if (node[pos].symbol == '^')
		{
			if (node[node[pos].leaf[0]].symbol == 'v')
			{
				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(node[pos].leaf[0], 0, exp);
					cout << "Distribution: " << exp << " ^ ";
					memset(exp, 0, 1001);
					recTableExp(node[pos].leaf[1], 0, exp);
					cout << exp << " -> ";
				}

				node[pos].symbol = 'v';
				node[node[pos].leaf[0]].symbol = '^';

				int newPos1 = -1; //must initialise with -1 for copytree
				copyTree(node[pos].leaf[1], newPos1);

				int savePos = node[pos].leaf[1];

				int newPos2 = getFreeNode();
				node[newPos2].construct('^', pos, newPos1, node[node[pos].leaf[0]].leaf[0]);
				node[newPos1].father = newPos2;
				node[node[node[pos].leaf[0]].leaf[0]].father = newPos2;
				node[pos].leaf[1] = newPos2;

				node[node[pos].leaf[0]].leaf[0] = savePos;
				node[savePos].father = node[pos].leaf[0];

				isChange = true;

				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(pos, 0, exp);
					cout << exp << '\n';
				}
			}
			if (node[node[pos].leaf[1]].symbol == 'v')
			{
				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(node[pos].leaf[0], 0, exp);
					cout << "Distribution: " << exp << " ^ ";
					memset(exp, 0, 1001);
					recTableExp(node[pos].leaf[1], 0, exp);
					cout << exp << " -> ";
				}

				node[pos].symbol = 'v';
				node[node[pos].leaf[1]].symbol = '^';

				int newPos1 = -1; //must initialise with -1 for copytree
				copyTree(node[pos].leaf[0], newPos1);

				int savePos = node[pos].leaf[0];

				int newPos2 = getFreeNode();
				node[newPos2].construct('^', pos, newPos1, node[node[pos].leaf[1]].leaf[0]);
				node[newPos1].father = newPos2;
				node[node[node[pos].leaf[1]].leaf[0]].father = newPos2;
				node[pos].leaf[0] = newPos2;

				node[node[pos].leaf[1]].leaf[0] = savePos;
				node[savePos].father = node[pos].leaf[0];

				isChange = true;

				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(pos, 0, exp);
					cout << exp << '\n';
				}
			}
		}

		//parsing
		if (node[pos].symbol == '!')
		{
			eliminateBigConj(node[pos].leaf[0], isChange);
		}
		else if (node[pos].symbol == '^' || node[pos].symbol == 'v' || node[pos].symbol == '>' || node[pos].symbol == '=' || node[pos].symbol == '*' || node[pos].symbol == '|')
		{
			eliminateBigConj(node[pos].leaf[0], isChange);
			eliminateBigConj(node[pos].leaf[1], isChange);
		}
	}
	void getDNF(char exp[])
	{
		int startPos = 0; bool change = false, s_change = false;

		if (ALLOW_SIMPLIFICATION)
		{
			simplify(0, startPos, s_change);
			while (s_change)
			{
				s_change = false;
				simplify(startPos, startPos, s_change);
			}
		}

		eliminateBigConj(startPos, change);
		clearAtomsList();
		updateAtomsList(startPos);
		if (ALLOW_SIMPLIFICATION)
		{
			simplify(0, startPos, s_change);
			while (s_change)
			{
				s_change = false;
				simplify(startPos, startPos, s_change);
			}
		}

		while (change)
		{
			change = false;
			eliminateBigConj(startPos, change);
			clearAtomsList();
			updateAtomsList(startPos);
			if (ALLOW_SIMPLIFICATION)
			{
				simplify(0, startPos, s_change);
				while (s_change)
				{
					s_change = false;
					simplify(startPos, startPos, s_change);
				}
			}
		}

		defragmentTree(startPos);
		exp[0] = 0;
		recTableExp(0, 0, exp);
	}

	void eliminateBigDisj(int pos, bool &isChange)
	{
		bool verbose = false;
		if (VERBOSE_LEVEL > 1)
			verbose = true;

		if (node[pos].symbol == 'v')
		{
			if (node[node[pos].leaf[0]].symbol == '^')
			{
				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(node[pos].leaf[0], 0, exp);
					cout << "Distribution: " << exp << " v ";
					memset(exp, 0, 1001);
					recTableExp(node[pos].leaf[1], 0, exp);
					cout << exp << " -> ";
				}

				node[pos].symbol = '^';
				node[node[pos].leaf[0]].symbol = 'v';

				int newPos1 = -1; //must initialise with -1 for copytree
				copyTree(node[pos].leaf[1], newPos1);

				int savePos = node[pos].leaf[1];

				int newPos2 = getFreeNode();
				node[newPos2].construct('v', pos, newPos1, node[node[pos].leaf[0]].leaf[0]);
				node[newPos1].father = newPos2;
				node[node[node[pos].leaf[0]].leaf[0]].father = newPos2;
				node[pos].leaf[1] = newPos2;

				node[node[pos].leaf[0]].leaf[0] = savePos;
				node[savePos].father = node[pos].leaf[0];

				isChange = true;

				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(pos, 0, exp);
					cout << exp << '\n';
				}
			}
			if (node[node[pos].leaf[1]].symbol == '^')
			{
				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(node[pos].leaf[0], 0, exp);
					cout << "Distribution: " << exp << " v ";
					memset(exp, 0, 1001);
					recTableExp(node[pos].leaf[1], 0, exp);
					cout << exp << " -> ";
				}

				node[pos].symbol = '^';
				node[node[pos].leaf[1]].symbol = 'v';

				int newPos1 = -1; //must initialise with -1 for copytree
				copyTree(node[pos].leaf[0], newPos1);

				int savePos = node[pos].leaf[0];

				int newPos2 = getFreeNode();
				node[newPos2].construct('v', pos, newPos1, node[node[pos].leaf[1]].leaf[0]);
				node[newPos1].father = newPos2;
				node[node[node[pos].leaf[1]].leaf[0]].father = newPos2;
				node[pos].leaf[0] = newPos2;

				node[node[pos].leaf[1]].leaf[0] = savePos;
				node[savePos].father = node[pos].leaf[0];

				isChange = true;

				if (verbose)
				{
					char exp[1001] = { 0 };
					recTableExp(pos, 0, exp);
					cout << exp << '\n';
				}
			}
		}

		//parsing
		if (node[pos].symbol == '!')
		{
			eliminateBigDisj(node[pos].leaf[0], isChange);
		}
		else if (node[pos].symbol == '^' || node[pos].symbol == 'v' || node[pos].symbol == '>' || node[pos].symbol == '=' || node[pos].symbol == '*' || node[pos].symbol == '|')
		{
			eliminateBigDisj(node[pos].leaf[0], isChange);
			eliminateBigDisj(node[pos].leaf[1], isChange);
		}
	}
	void getCNF(char exp[])
	{
		int startPos = 0; bool change = false, s_change = false;

		if (ALLOW_SIMPLIFICATION)
		{
			simplify(0, startPos, s_change);
			while (s_change)
			{
				s_change = false;
				simplify(startPos, startPos, s_change);
			}
		}

		eliminateBigDisj(startPos, change);
		clearAtomsList();
		updateAtomsList(startPos);

		if (ALLOW_SIMPLIFICATION)
		{
			simplify(0, startPos, s_change);
			while (s_change)
			{
				s_change = false;
				simplify(startPos, startPos, s_change);
			}
		}
		while (change)
		{
			change = false;
			eliminateBigDisj(startPos, change);
			clearAtomsList();
			updateAtomsList(startPos);
			if (ALLOW_SIMPLIFICATION)
			{
				simplify(0, startPos, s_change);
				while (s_change)
				{
					s_change = false;
					simplify(startPos, startPos, s_change);
				}
			}
		}

		defragmentTree(startPos);
		exp[0] = 0;
		recTableExp(0, 0, exp);
	}

	void getTreeDepth(int pos, int currentD, int &maxD)
	{  // UNUSED
		if (currentD > maxD)
			maxD = currentD;
		if (node[pos].symbol == '!')
			getTreeDepth(node[pos].leaf[0], currentD + 1, maxD);
		else if (node[pos].symbol == '^' || node[pos].symbol == 'v' || node[pos].symbol == '>' || node[pos].symbol == '=' || node[pos].symbol == '|' || node[pos].symbol == '*')
		{
			getTreeDepth(node[pos].leaf[0], currentD + 1, maxD);
			getTreeDepth(node[pos].leaf[1], currentD + 1, maxD);
		}
	}
	void getTreeLeaves(int pos, int &leaves)
	{
		if (node[pos].symbol >= 'A' && node[pos].symbol <= 'Z' || node[pos].symbol == 'f' || node[pos].symbol == 't')
			leaves++;
		else if (node[pos].symbol == '!')
			getTreeLeaves(node[pos].leaf[0], leaves);
		else if (node[pos].symbol == '^' || node[pos].symbol == 'v' || node[pos].symbol == '>' || node[pos].symbol == '=' || node[pos].symbol == '|' || node[pos].symbol == '*')
		{
			getTreeLeaves(node[pos].leaf[0], leaves);
			getTreeLeaves(node[pos].leaf[1], leaves);
		}
	}
	void recTreeString(char treeString[256][256], int pos, int x, int y)
	{
		if (x < 254 && y < 254)
		{
			if (node[pos].symbol == '!')
			{
				treeString[y][x] = '!';

				int depth = -1;
				getTreeLeaves(pos, depth);

				for (int i = 0; i < depth + 1; i++)
					treeString[y + i + 1][x - i - 1] = '/';
				recTreeString(treeString, node[pos].leaf[0], x - depth - 2, y + depth + 2);
			}
			else if (node[pos].symbol == '^' || node[pos].symbol == 'v' || node[pos].symbol == '>' || node[pos].symbol == '=' || node[pos].symbol == '|' || node[pos].symbol == '*')
			{
				treeString[y][x] = node[pos].symbol;

				int depth = -1;
				getTreeLeaves(pos, depth);

				for (int i = 0; i < depth; i++)
				{
					treeString[y + i + 1][x - i - 1] = '/';
					treeString[y + i + 1][x + i + 1] = '\\';
				}
				recTreeString(treeString, node[pos].leaf[0], x - depth - 1, y + depth + 1);
				recTreeString(treeString, node[pos].leaf[1], x + depth + 1, y + depth + 1);
			}
			else if (node[pos].symbol >= 'A' && node[pos].symbol <= 'Z' || node[pos].symbol == 'f' || node[pos].symbol == 't')
			{
				treeString[y][x] = node[pos].symbol;
			}
		}
	}
	
	void display()
	{
		char treeString[256][256];
		for (int i = 0; i < 256; i++)
		{ //create empty char matrix
			for (int j = 0; j < 255; j++)
				treeString[i][j] = ' ';
			treeString[i][255] = 0;
		}

		//start as close to 0 0 as posible 
		int allLeaves = 0;
		getTreeLeaves(0, allLeaves);
		recTreeString(treeString, 0, allLeaves * 2 + 1, 0);

		//delete useless spaces
		for (int i = 0; i < 256; i++)
		{
			for (int j = 254; j >= 0; j--)
				if (treeString[i][j] != ' ')
				{
					treeString[i][j + 1] = 0;
					break;
				}
			treeString[i][255] = 0;
		}

		cout << '\n';
		for (int i = 0; i < 256; i++)
		{ //print matrix
			bool ok = false;
			for (int j = 0; j < 255; j++)
				if (treeString[i][j] != ' ')
				{
					ok = true;
					break;
				}
			if (!ok)
				break;
			cout << treeString[i]<<'\n';
		}
		cout << '\n';
	}

	void deconstruct()
	{
		for (int i = 0; i < 1000; i++)
			node[i].deconstruct();
		for (int i = 0; i < 26; i++)
			for (int j = 0; j < 1000; j++)
				atoms[i][j] = -1;
		for (int i = 0; i < 60; i++)
			table[i].deconstruct();
	}
};

struct Clause
{
	int nrLiterals = 0;
	char atoms[101] = { 0 };
	bool negated[100] = { false };

	void construct(int cnt, char a[], bool n[])
	{
		nrLiterals = cnt;
		for (int i = 0; i < cnt; i++)
		{
			atoms[i] = a[i];
			negated[i] = n[i];
		}
	}
	void append(const char a, bool n)
	{
		if (VERBOSE_LEVEL > 3)
		{
			if (n) cout << "Appended: \"!" << a << "\" at " << nrLiterals << '\n';
			else cout << "Appended: '" << a << "' at " << nrLiterals << '\n';
		}
		atoms[nrLiterals] = a;
		negated[nrLiterals++] = n;
	}
	void popLiteral(const char c, bool n)
	{
		for (int i = 0; i < nrLiterals; i++)
			if (c == atoms[i] && n == negated[i])
			{
				for (int j = i; j < nrLiterals - 1; j++)
				{
					atoms[j] = atoms[j + 1];
					negated[j] = negated[j + 1];
				}
				nrLiterals--;
				i--;
			}
	}
	void decontruct()
	{
		memset(atoms, 0, 101);
		memset(negated, false, 100);
	}
	bool isComplementary()
	{ //check if one literal complements another
		if (nrLiterals == 0)
			return 0;
		for (int i = 0; i < nrLiterals; i++)
		{
			bool ok = true;
			for (int j = 0; j < nrLiterals; j++)
				if (atoms[j] == atoms[i] && negated[i] != negated[j])
				{
					ok = false;
					break;
				}
			if (ok)
				return 0;
		}
		return 1;
	}
	bool contains(const char a, bool n)
	{
		for (int i = 0; i < nrLiterals; i++)
			if (a == atoms[i] && n == negated[i])
				return 1;
		return 0;
	}
	void getStr(char s[])
	{
		s[0] = '{';
		int sPos = 1;
		if (nrLiterals)
		{
			for (int i = 0; i < nrLiterals - 1; i++)
			{
				if (negated[i])
					s[sPos++] = '!';
				s[sPos++] = atoms[i];
				s[sPos++] = ',';
			}
			if (negated[nrLiterals - 1])
				s[sPos++] = '!';
			s[sPos++] = atoms[nrLiterals - 1];
		}
		s[sPos++] = '}';
		s[sPos] = 0;
	}

	void setFromCNF(Tree &tree, int pos)
	{
		if (tree.node[pos].symbol == 'v')
		{
			if (tree.node[tree.node[pos].leaf[0]].symbol == 'v')
			{
				setFromCNF(tree, tree.node[pos].leaf[0]);
			}
			else
			{
				char a = 0; bool n = false;
				if (tree.node[tree.node[pos].leaf[0]].symbol == '!')
				{
					n = true;
					a = tree.node[tree.node[tree.node[pos].leaf[0]].leaf[0]].symbol;
				}
				else
					a = tree.node[tree.node[pos].leaf[0]].symbol;
				if(!contains(a, n))
					append(a, n);
			}

			if (tree.node[tree.node[pos].leaf[1]].symbol == 'v')
			{
				setFromCNF(tree, tree.node[pos].leaf[1]);
			}
			else
			{
				char a = 0; bool n = false;
				if (tree.node[tree.node[pos].leaf[1]].symbol == '!')
				{
					n = true;
					a = tree.node[tree.node[tree.node[pos].leaf[1]].leaf[0]].symbol;
				}
				else
					a = tree.node[tree.node[pos].leaf[1]].symbol;
				if (!contains(a, n))
					append(a, n);
			}
		}
		else
		{
			char a = 0; bool n = false;
			if (tree.node[pos].symbol == '!')
			{
				n = true;
				a = tree.node[tree.node[pos].leaf[0]].symbol;
			}
			else
				a = tree.node[pos].symbol;
			if (!contains(a, n))
				append(a, n);
		}
	}
};
struct ClauseSet
{
	Clause set[100];
	int nrClauses = 0;

	void append(Clause c)
	{
		if (VERBOSE_LEVEL > 3)
		{
			char aux[MAX_STRING] = { 0 }; c.getStr(aux);
			cout << "Appended: \"" << aux << "\" at " << nrClauses << '\n';
		}
		set[nrClauses++] = c;
	}
	void pop(int pos)
	{
		nrClauses--;
		for (int i = pos; i < nrClauses; i++)
			set[i] = set[i + 1];
		set[nrClauses].decontruct();
	}
	int construct(const char s[])
	{
		if (s[0] != '{')
			return 0;
		int i = 1;
		int paranth = 1, cnt = 0;
		bool neg[100] = { false };
		char atoms[101] = { 0 };

		while (s[i] && (s[i] != '}' || paranth != 1))
		{
			if (s[i] == '{')
			{
				paranth++;
				if (paranth > 2) //syntax error
					return i;
			}
			else if (s[i] == '!')
			{
				neg[cnt] = true;
			}
			else if (s[i] >= 'A' && s[i] <= 'Z')
			{
				atoms[cnt++] = s[i];
			}
			else if (s[i] == '}')
			{
				paranth--;
				Clause aux; aux.construct(cnt, atoms, neg);
				memset(atoms, 0, 101);
				memset(neg, false, 100);
				cnt = 0;
				append(aux);
			}
			else if (s[i] != ' ' && s[i] != ',') //syntax error
				return i;
			i++;
		}
		if (paranth != 1) //syntax error, final } not found
			return -2;
		return -1;

	}
	void deconstruct()
	{
		for (int i = 0; i < nrClauses; i++)
			set[i].decontruct();
		nrClauses = 0;
	}
	void getStr(char s[])
	{
		int sPos = 1;
		s[0] = '{';
		if (nrClauses)
		{
			for (int i = 0; i < nrClauses - 1; i++)
			{
				s[sPos++] = '{';
				for (int j = 0; j < set[i].nrLiterals - 1; j++)
				{
					if (set[i].negated[j])
						s[sPos++] = '!';
					s[sPos++] = set[i].atoms[j];
					s[sPos++] = ',';
				}
				if (set[i].negated[set[i].nrLiterals - 1])
					s[sPos++] = '!';
				s[sPos++] = set[i].atoms[set[i].nrLiterals - 1];
				s[sPos++] = '}';
				s[sPos++] = ',';
				s[sPos++] = ' ';
			}
			s[sPos++] = '{';
			for (int j = 0; j < set[nrClauses - 1].nrLiterals - 1; j++)
			{
				if (set[nrClauses - 1].negated[j])
					s[sPos++] = '!';
				s[sPos++] = set[nrClauses - 1].atoms[j];
				s[sPos++] = ',';
			}
			if (set[nrClauses - 1].negated[set[nrClauses - 1].nrLiterals - 1])
				s[sPos++] = '!';
			s[sPos++] = set[nrClauses - 1].atoms[set[nrClauses - 1].nrLiterals - 1];
			s[sPos++] = '}';
		}
		s[sPos++] = '}';
		s[sPos] = 0;
	}
	void getExpression(char s[])
	{
		int sPos = 0;
		if (nrClauses)
		{
			for (int i = 0; i < nrClauses - 1; i++)
			{
				s[sPos++] = '(';
				for (int j = 0; j < set[i].nrLiterals - 1; j++)
				{
					if (set[i].negated[j])
						s[sPos++] = '!';
					s[sPos++] = set[i].atoms[j];
					s[sPos++] = 'v';
				}
				if (set[i].negated[set[i].nrLiterals - 1])
					s[sPos++] = '!';
				s[sPos++] = set[i].atoms[set[i].nrLiterals - 1];
				s[sPos++] = ')';
				s[sPos++] = '^';
			}
			s[sPos++] = '(';
			for (int j = 0; j < set[nrClauses - 1].nrLiterals - 1; j++)
			{
				if (set[nrClauses - 1].negated[j])
					s[sPos++] = '!';
				s[sPos++] = set[nrClauses - 1].atoms[j];
				s[sPos++] = 'v';
			}
			if (set[nrClauses - 1].negated[set[nrClauses - 1].nrLiterals - 1])
				s[sPos++] = '!';
			s[sPos++] = set[nrClauses - 1].atoms[set[nrClauses - 1].nrLiterals - 1];
			s[sPos++] = ')';
		}
		s[sPos++] = 0;
	}
	bool contains(Clause c)
	{
		if (c.nrLiterals == 0)
			return 0;
		for (int i = 0; i < nrClauses; i++)
		{
			if (set[i].nrLiterals == c.nrLiterals)
			{
				bool ok = true;
				for (int j = 0; j < c.nrLiterals; j++)
					if (!set[i].contains(c.atoms[j], c.negated[j]))
					{
						ok = false;
						break;
					}
				if (ok)
					return 1;
			}
		}
		return 0;
	}
	bool findResolvent(int pos1, int pos2, Clause &c)
	{
		for (int i = 0; i < set[pos1].nrLiterals; i++)
		{
			for (int j = 0; j < set[pos2].nrLiterals; j++)
				if (set[pos1].atoms[i] == set[pos2].atoms[j] && set[pos1].negated[i] != set[pos2].negated[j])
				{
					for (int k = 0; k < set[pos1].nrLiterals; k++) //append from 1
						if (k != i && !c.contains(set[pos1].atoms[k], set[pos1].negated[k]))
							c.append(set[pos1].atoms[k], set[pos1].negated[k]);
					for (int k = 0; k < set[pos2].nrLiterals; k++) //append from 2
						if (k != j && !c.contains(set[pos2].atoms[k], set[pos2].negated[k]))
							c.append(set[pos2].atoms[k], set[pos2].negated[k]);
					//char auxs[1000]; c.getStr(auxs); cout <<"tried: "<< auxs<< '\n';
					return 1;
				}
		}
		return 0;
	}
	bool isPure(const char a, bool n)
	{
		for (int i = 0; i < nrClauses; i++)
			for (int j = 0; j < set[i].nrLiterals; j++)
				if (a == set[i].atoms[j] && n != set[i].negated[j])
					return 0;
		return 1;
	}
	int createResolvent()
	{
		for (int i = 0; i < nrClauses; i++)
			for (int j = 0; j < nrClauses; j++)
			{
				Clause aux;
				if (findResolvent(i, j, aux) && !aux.isComplementary() && !contains(aux))
				{
					char auxs[1000]; aux.getStr(auxs); cout << auxs;
					memset(auxs, 0, 1000); set[i].getStr(auxs); cout << " from: (" << i << ") " << auxs;
					memset(auxs, 0, 1000); set[j].getStr(auxs); cout << ", (" << j << ") " << auxs << '\n';
					if (aux.nrLiterals == 0) //empty clause found!
						return 2;
					append(aux);
					return 1;
				}
			}
		return 0;
	}
	void sort()
	{
		for (int i = 0; i < nrClauses - 1; i++)
			for (int j = i + 1; j < nrClauses; j++)
				if (set[i].nrLiterals > set[j].nrLiterals)
					swap(set[i], set[j]);
	}

	bool classicResolution()
	{
		int res = 0;
		do
		{
			sort();
			res = createResolvent();
			if (res == 2)
			{
				char a[1000]; getStr(a); cout << "Unsatisfiable\n" << a << '\n';
				return 0;
			}
		} while (res == 1);
		char a[1000]; getStr(a); cout << "Satisfiable\n" << a << '\n';
		return 1;
	}
	bool DPResolution()
	{
		int res = 0;
		do
		{
			bool again;
			do //step 1
			{
				again = false;
				for (int i = 0; i < nrClauses; i++)
					if (set[i].nrLiterals == 1)
					{
						char sAtom = set[i].atoms[0]; bool sNeg = set[i].negated[0];
						for (int j = 0; j < nrClauses; j++)
						{
							if (set[j].contains(sAtom, sNeg))
							{
								char a[1000]; set[j].getStr(a); cout << "Poped: " << a << '\n';
								pop(j);
								again = true;
								i--;
							}
							else if (set[j].contains(sAtom, !sNeg))
							{
								cout << "Poped literal: "; if (!sNeg) cout << '!'; cout << sAtom << " from " << j << '\n';
								if (set[j].nrLiterals == 1)
									return 0; //unsatisf, created empty clause
								set[j].popLiteral(sAtom, !sNeg);
								again = true;
								i--;
							}
						}
					}
				if (nrClauses == 0)
					return 1;
			} while (again);

			do //step 2
			{
				again = false;
				for (int i = 0; i < nrClauses; i++)
					for (int j = 0; j < set[i].nrLiterals; j++)
						if (isPure(set[i].atoms[j], set[i].negated[j]))
						{
							char a[1000]; set[i].getStr(a); cout << "Poped: " << a << '\n';
							pop(i);
							i--;
							again = true;
							break;
						}
				if (nrClauses == 0)
					return 1;
			} while (again);

			sort();
			res =createResolvent();
			if (res == 2)
				return 0;
		} while (res == 1);
		return 1;
	}

	void setFromCNF(Tree &tree, int pos)
	{
		if (tree.node[pos].symbol == '^')
		{
			if (tree.node[tree.node[pos].leaf[0]].symbol == '^')
			{
				setFromCNF(tree, tree.node[pos].leaf[0]);
			}
			else
			{
				Clause aux;
				aux.setFromCNF(tree, tree.node[pos].leaf[0]);
				if (!contains(aux))
					append(aux);
			}

			if (tree.node[tree.node[pos].leaf[1]].symbol == '^')
			{
				setFromCNF(tree, tree.node[pos].leaf[1]);
			}
			else
			{
				Clause aux;
				aux.setFromCNF(tree, tree.node[pos].leaf[1]);
				if (!contains(aux))
					append(aux);
			}
		}
	}
};



int main()
{
	cout << "LEE 2022 [version 1.1 64-bit]\nMade for the LCS exam by FA.\n__________________________________________________________________________\n         __                _________         _________\n        /\\\\\\              /\\\\\\\\\\\\\\\\\\\\       /\\\\\\\\\\\\\\\\\\\\\n        \\ \\\\\\             \\ \\\\\\_____/       \\ \\\\\\_____/\n         \\ \\\\\\             \\ \\\\\\___          \\ \\\\\\___\n          \\ \\\\\\             \\ \\\\\\\\\\\\          \\ \\\\\\\\\\\\\n           \\ \\\\\\             \\ \\\\\\_/           \\ \\\\\\_/\n            \\ \\\\\\_______   __ \\ \\\\\\_______   __ \\ \\\\\\_______   __\n             \\ \\\\\\\\\\\\\\\\\\\\ /\\_\\ \\ \\\\\\\\\\\\\\\\\\\\ /\\_\\ \\ \\\\\\\\\\\\\\\\\\\\ /\\_\\\n              \\/________/ \\/_/  \\/________/ \\/_/  \\/________/ \\/_/\n                 Type HELP for the command menu\n__________________________________________________________________________\n";
	Tree tree;
	char cInput[MAX_STRING] = { 0 };
	char commands[10][261] = { 0 };
	char inpString[MAX_STRING] = { 0 }, saveString[MAX_STRING] = { 0 }, inpClause[MAX_STRING] = { 0 };
	char inpInterpetation[MAX_STRING] = { 0 };
	ClauseSet cSet;

	while (true)
	{
		if (fileInput)
		{
			if (!fin.getline(cInput, MAX_STRING - 1))
			{
				cout << "Reached EOF in: \"" << IN_FILE << "\"\n";
				fin.close();
				fileInput = false;
				memset(IN_FILE, 0, MAX_FILE);
			}
		}
		else
		{
			cout << "|> ";
			cin.getline(cInput, MAX_STRING - 1);
		}

		int i, j = 0, k = 0;
		for (i = 0; i < 10; i++)
			memset(commands[i], 0, 261);
		//parsing
		i = 0;
		bool inName = false;
		while (cInput[i] && j < 10)
		{
			if (!inName)
			{
				if (cInput[i] == '\"') inName = true;
				else if (cInput[i] == '#')
				{
					break;
				}
				else if (cInput[i] != ' ') commands[j][k] = cInput[i], k++;
				else
				{
					if (commands[j][0]) j++;
					k = 0;
				}
			}
			else
			{
				while (cInput[i] == ' ') i++; //escape spaces
				if (cInput[i] == '\"') inName = false;
				else
				{
					commands[j][k] = cInput[i];
					k++;
				}
			}
			i++;
		}

		//interpreting
		if (inName)
		{
			cout << "Warning: A quote was started but not ended\n";
			if(fileOutput) fout << "Warning: A quote was started but not ended\n";
		}
		else if (!_strcmpi("SET", commands[0]))
		{ //macros
			if (commands[1][0] == 0 || commands[2][0] == 0)
			{
				cout << "Incorrect syntax: missing argument\n";
				if(fileOutput) fout << "Incorrect syntax: missing argument\n";
			}
			else if (commands[3][0] != 0)
			{
				cout << "Incorrect syntax: too many arguments\n";
				if (fileOutput) fout << "Incorrect syntax: too many arguments\n";
			}
			else if (!strcmp("VERBOSE_LEVEL", commands[1]))
			{
				VERBOSE_LEVEL = toInt(commands[2]);
				if (VERBOSE_LEVEL > 2)
				{
					cout << "VERBOSE_LEVEL set to: " << VERBOSE_LEVEL << '\n';
					if (fileOutput) fout << "VERBOSE_LEVEL set to: " << VERBOSE_LEVEL << '\n';
				}
			}
			else if (!strcmp("ALLOW_RELAXED_SYNTAX", commands[1]))
			{
				if (!_strcmpi("true", commands[2]))
					ALLOW_RELAXED_SYNTAX = true;
				else if (!_strcmpi("false", commands[2]))
					ALLOW_RELAXED_SYNTAX = false;
				else
					ALLOW_RELAXED_SYNTAX = (bool)toInt(commands[2]);

				if (VERBOSE_LEVEL > 2)
				{
					cout << "ALLOW_RELAXED_SYNTAX set to: " << ALLOW_RELAXED_SYNTAX << '\n';
					if (fileOutput) fout << "ALLOW_RELAXED_SYNTAX set to: " << ALLOW_RELAXED_SYNTAX << '\n';
				}
			}
			else if (!strcmp("ALLOW_TABLE_EVAL", commands[1]))
			{
				if (!_strcmpi("true", commands[2]))
					ALLOW_TABLE_EVAL = true;
				else if (!_strcmpi("false", commands[2]))
					ALLOW_TABLE_EVAL = false;
				else
					ALLOW_TABLE_EVAL = (bool)toInt(commands[2]);

				if (VERBOSE_LEVEL > 2)
				{
					cout << "ALLOW_TABLE_EVAL set to: " << ALLOW_TABLE_EVAL << '\n';
					if (fileOutput) fout << "ALLOW_TABLE_EVAL set to: " << ALLOW_TABLE_EVAL << '\n';
				}
			}
			else if (!strcmp("STRING", commands[1]))
			{
				if (!strcmp("$STRONGFORM$", commands[2]))
				{
					if (inpString[0])
					{
						getStrongExp(inpString);
						if (VERBOSE_LEVEL > 1)
						{
							cout << "STRING set to: \"" << inpString << "\"\n";
							if (fileOutput) fout << "STRING set to: \"" << inpString << "\"\n";
						}
					}
					else
					{
						cout << "Warning: STRING variable is empty\n";
						if (fileOutput) fout << "Warning: STRING variable is empty\n";
					}
				}
				else if (!strcmp("$TREE$", commands[2]))
				{
					if (!tree.nrNodes)
					{
						cout << "Warning: TREE object is empty\n";
						if (fileOutput) fout << "Warning: TREE object is empty\n";
					}
					else
					{
						memset(inpString, 0, MAX_STRING);
						tree.recTableExp(0, 0, inpString);
						if (VERBOSE_LEVEL > 2)
						{
							cout << "TREE loaded into STRING\n";
							if (fileOutput) fout << "TREE loaded into STRING\n";
						}
					}
				}
				else if (!strcmp("$SAVE$", commands[2]))
				{
					if (saveString[0])
					{
						strcpy_s(inpString, MAX_STRING, saveString);
						if (VERBOSE_LEVEL > 2)
						{
							cout << "SAVE loaded into STRING\n";
							if (fileOutput) fout << "SAVE loaded into STRING\n";
						}
					}
					else
					{
						cout << "Warning: SAVE variable is empty\n";
						if (fileOutput) fout << "Warning: SAVE variable is empty\n";
					}
				}
				if (!strcmp("$CLAUSESET$", commands[2]))
				{
					if (cSet.nrClauses)
					{
						cSet.getExpression(inpString);
						if (VERBOSE_LEVEL > 1)
						{
							cout << "STRING set to: \"" << inpString << "\"\n";
							if (fileOutput) fout << "STRING set to: \"" << inpString << "\"\n";
						}
					}
					else
					{
						cout << "Warning: CLAUSESET object is empty\n";
						if (fileOutput) fout << "Warning: CLAUSESET object is empty\n";
					}
				}
				else
				{
					strcpy_s(inpString, MAX_STRING, commands[2]);
					if (VERBOSE_LEVEL > 2)
					{
						cout << "STRING set to: \"" << inpString << "\"\n";
						if (fileOutput) fout << "STRING set to: \"" << inpString << "\"\n";
					}
				}
			}
			else if (!strcmp("CLAUSES", commands[1]))
			{
				if (!strcmp("$SAVE$", commands[2]))
				{
					if (saveString[0])
					{
						strcpy_s(inpString, MAX_STRING, saveString);
						if (VERBOSE_LEVEL > 2)
						{
							cout << "SAVE loaded into CLAUSES\n";
							if (fileOutput) fout << "SAVE loaded into CLAUSES\n";
						}
					}
					else
					{
						cout << "Warning: SAVE variable is empty\n";
						if (fileOutput) fout << "Warning: SAVE variable is empty\n";
					}
				}
				else
				{
					strcpy_s(inpClause, MAX_STRING, commands[2]);
					if (VERBOSE_LEVEL > 2)
					{
						cout << "CLAUSES set to: \"" << inpClause << "\"\n";
						if (fileOutput) fout << "CLAUSES set to: \"" << inpClause << "\"\n";
					}
				}
			}
			else if (!strcmp("INTERPRETATION", commands[1]))
			{
				strcpy_s(inpInterpetation, MAX_STRING, commands[2]);
				if (VERBOSE_LEVEL > 2)
				{
					cout << "INTERPRETATION set to: \"" << inpInterpetation << "\"\n";
					if (fileOutput) fout << "INTERPRETATION set to: \"" << inpInterpetation << "\"\n";
				}
			}
			else if (!strcmp("OUT_FILE", commands[1]))
			{
				if (!strcmp("STDOUT", commands[2]))
				{
					fileOutput = false;
					fout.close();
					if(VERBOSE_LEVEL > 1) cout << "Output stream changed to: STDOUT\n";
				}
				else
				{
					fout.open(commands[2]);
					if (!fout)
						cout << "Warning: Cannot open file\n";
					else
					{
						strcpy_s(OUT_FILE, MAX_FILE, commands[2]);
						fileOutput = true;
						if (VERBOSE_LEVEL > 1) cout << "Output stream changed to: \"" << OUT_FILE << "\"\n";
					}
				}
			}
			else
			{
				cout << "Incorrect syntax: unknown argument \"" << commands[1] << "\"\n";
				if (fileOutput) fout << "Incorrect syntax: unknown argument \"" << commands[1] << "\"\n";
			}
		}
		else if (!_strcmpi("GET", commands[0]))
		{
			if (commands[1][0] == 0)
			{
				cout << "Incorrect syntax: missing argument\n";
				if (fileOutput) fout << "Incorrect syntax: missing argument\n";
			}
			else if (commands[2][0] != 0)
			{
				cout << "Incorrect syntax: too many arguments\n";
				if (fileOutput) fout << "Incorrect syntax: too many arguments\n";
			}
			else if (!strcmp("STRING", commands[1]))
			{
				if (!inpString[0])
				{
					cout << "Warning: STRING variable is empty\n";
					if (fileOutput) fout << "Warning: STRING variable is empty\n";
				}
				else
				{
					cout << "STRING: \"" << inpString << "\"\n";
					if (fileOutput) fout << "STRING: \"" << inpString << "\"\n";
				}
			}
			else if (!strcmp("CLAUSES", commands[1]))
			{
				if (!inpClause[0])
				{
					cout << "Warning: CLAUSES variable is empty\n";
					if (fileOutput) fout << "Warning: CLAUSES variable is empty\n";
				}
				else
				{
					cout << "CLAUSES: \"" << inpClause << "\"\n";
					if (fileOutput) fout << "CLAUSES: \"" << inpClause << "\"\n";
				}
			}
			else if (!strcmp("INTERPRETATION", commands[1]))
			{
				if (!inpInterpetation[0])
				{
					cout << "Warning: INTERPRETATION variable is empty\n";
					if (fileOutput) fout << "Warning: INTERPRETATION variable is empty\n";
				}
				else
				{
					cout << "INTERPRETATION: \"" << inpInterpetation << "\"\n";
					if (fileOutput) fout << "INTERPRETATION: \"" << inpInterpetation << "\"\n";
				}
			}
			else if (!strcmp("STRONGFORM", commands[1]))
			{
				char aux[MAX_STRING]; strcpy_s(aux, MAX_STRING, inpString);
				getStrongExp(aux);
				cout << "Strong expression generated: \"" << aux << "\"\n";
				if (fileOutput) fout << "Strong expression generated: \"" << aux << "\"\n";
			}
			else if (!strcmp("TREE", commands[1]))
			{
				if (tree.nrNodes == 0)
				{
					cout << "Warning: TREE object is empty\n";
					if (fileOutput) fout << "Warning: TREE object is empty\n";
				}
				else
				{
					char aux[MAX_STRING] = { 0 }; tree.recTableExp(0, 0, aux); cout << aux << '\n';
					tree.getTreeList();
				}
			}
			else if (!strcmp("CLAUSESET", commands[1]))
			{
				if (cSet.nrClauses == 0)
				{
					cout << "Warning: CLAUSESET structure is empty\n";
					if (fileOutput) fout << "Warning: CLAUSESET structure is empty\n";
				}
				else
				{
					char aux[MAX_STRING] = { 0 }; cSet.getStr(aux); cout << aux << '\n';
				}
			}
			else
			{
				cout << "Incorrect syntax: unknown argument \"" << commands[1] << "\"\n";
				if (fileOutput) fout << "Incorrect syntax: unknown argument \"" << commands[1] << "\"\n";
			}
		}
		else if (!_strcmpi("BUILD", commands[0]))
		{
			if (commands[1][0] == 0)
			{
				cout << "Incorrect syntax: missing argument\n";
				if (fileOutput) fout << "Incorrect syntax: missing argument\n";
			}
			else if (commands[2][0] != 0)
			{
				cout << "Incorrect syntax: too many arguments\n";
				if (fileOutput) fout << "Incorrect syntax: too many arguments\n";
			}
			else if (!strcmp("TREE", commands[1]))
			{
				if (!inpString[0])
				{
					cout << "Warning: STRING variable is empty\n";
					if (fileOutput) fout << "Warning: STRING variable is empty\n";
				}
				else
				{
					tree.deconstruct();
					if (tree.construct(inpString))
					{
						cout << "TREE built successfully\n";
						if (fileOutput) fout << "TREE built successfully\n";
					}
					else
					{
						cout << "Warning: An error occurred while building TREE, STRING might not be well-formed\n";
						if (fileOutput) fout << "Warning: An error occurred while building TREE, STRING might not be well-formed\n";
					}
				}
			}
			else if (!strcmp("TABLE", commands[1]))
			{
				if (!tree.nrNodes)
				{
					cout << "Warning: TREE object is empty\n";
					if (fileOutput) fout << "Warning: TREE object is empty\n";
				}
				else
				{
					if (!inpInterpetation[0] || inpInterpetation[0] == '@') //all intepretations
					{
						char allInterp[MAX_STRING] = { 0 };
						tree.getAllInterpretations(tree.getAtomCount(), allInterp);
						if (VERBOSE_LEVEL > 0)
						{
							cout << "Generated all interpretations: \"" << allInterp << "\"\n";
							if (fileOutput) fout << "Generated all interpretations: \"" << allInterp << "\"\n";
						}
						//build table for all interp
						int interpretationCnt = tree.readInterpretation(allInterp);
						if (interpretationCnt != -1)
						{
							int tableLen = tree.buildTable();
							tree.printTable(tableLen, interpretationCnt);
							cout << "TABLE built successfully\n";
							if (fileOutput) fout << "TABLE built successfully\n";
						}
						else
						{
							cout << "Warning: An error occurred while building TABLE\n";
							if (fileOutput) fout << "Warning: An error occurred while building TABLE\n";
						}
					}
					else
					{
						//build table
						int interpretationCnt = tree.readInterpretation(inpInterpetation);
						if (interpretationCnt != -1)
						{
							int tableLen = tree.buildTable();
							cout << "TABLE built successfully\n";
							if (fileOutput) fout << "TABLE built successfully\n";
							tree.printTable(tableLen, interpretationCnt);
						}
						else
						{
							cout << "Warning: An error occurred while building TABLE\n";
							if (fileOutput) fout << "Warning: An error occurred while building TABLE\n";
						}
					}
				}
			}
			else if (!strcmp("NNF", commands[1]))
			{
				if (!tree.nrNodes)
				{
					cout << "Warning: TREE object is empty\n";
					if (fileOutput) fout << "Warning: TREE object is empty\n";
				}
				else
				{
					if (VERBOSE_LEVEL > 2)
					{
						cout << "Applied rules:\n";
					}
					char NNFexp[MAX_STRING] = { 0 };
					tree.getNNF(NNFexp);
					int sVerbose = VERBOSE_LEVEL; VERBOSE_LEVEL = 0;
					tree.deconstruct();
					tree.getTree(NNFexp);
					VERBOSE_LEVEL = sVerbose;
					if (VERBOSE_LEVEL > 0)
					{
						cout << "NNF built successfully and loaded into TREE\nNNF: \""<<NNFexp<<"\"\n";
						if (fileOutput) fout << "NNF built successfully and loaded into TREE\nNNF: \"" << NNFexp << "\"\n";
					}
				}
			}
			else if (!strcmp("DNF", commands[1]))
			{
				if (!tree.nrNodes)
				{
					cout << "Warning: TREE object is empty\n";
					if (fileOutput) fout << "Warning: TREE object is empty\n";
				}
				else
				{
					if (VERBOSE_LEVEL > 2)
					{
						cout << "Applied rules:\n";
					}
					char exp[MAX_STRING] = { 0 };
					tree.getNNF(exp);
					int sVerbose = VERBOSE_LEVEL; VERBOSE_LEVEL = 0;
					tree.deconstruct();
					tree.getTree(exp);
					VERBOSE_LEVEL = sVerbose;
					if (VERBOSE_LEVEL > 0)
					{
						cout << "NNF built successfully and loaded into TREE\nNNF: \"" << exp << "\"\n";
						if (fileOutput) fout << "NNF built successfully and loaded into TREE\nNNF: \"" << exp << "\"\n";
					}
					if (VERBOSE_LEVEL > 2)
					{
						cout << "Applied rules:\n";
					}
					memset(exp, 0, MAX_STRING);
					tree.getDNF(exp);
					sVerbose = VERBOSE_LEVEL; VERBOSE_LEVEL = 0;
					tree.deconstruct();
					tree.getTree(exp);
					VERBOSE_LEVEL = sVerbose;
					if (VERBOSE_LEVEL > 0)
					{
						cout << "DNF built successfully and loaded into TREE\nDNF: \"" << exp << "\"\n";
						if (fileOutput) fout << "DNF built successfully and loaded into TREE\nDNF: \"" << exp << "\"\n";
					}
				}
			}
			else if (!strcmp("CNF", commands[1]))
			{
				if (!tree.nrNodes)
				{
					cout << "Warning: TREE object is empty\n";
					if (fileOutput) fout << "Warning: TREE object is empty\n";
				}
				else
				{
					if (VERBOSE_LEVEL > 2)
					{
						cout << "Applied rules:\n";
					}
					char exp[MAX_STRING] = { 0 };
					tree.getNNF(exp);
					int sVerbose = VERBOSE_LEVEL; VERBOSE_LEVEL = 0;
					tree.deconstruct();
					tree.getTree(exp);
					VERBOSE_LEVEL = sVerbose;
					if (VERBOSE_LEVEL > 0)
					{
						cout << "NNF built successfully and loaded into TREE\nNNF: \"" << exp << "\"\n";
						if (fileOutput) fout << "NNF built successfully and loaded into TREE\nNNF: \"" << exp << "\"\n";
					}

					if (VERBOSE_LEVEL > 2)
					{
						cout << "Applied rules:\n";
					}
					memset(exp, 0, MAX_STRING);
					tree.getCNF(exp);
					sVerbose = VERBOSE_LEVEL; VERBOSE_LEVEL = 0;
					tree.deconstruct();
					tree.getTree(exp);
					VERBOSE_LEVEL = sVerbose;
					if (VERBOSE_LEVEL > 0)
					{
						cout << "CNF built successfully and loaded into TREE\nCNF: \"" << exp << "\"\n";
						if (fileOutput) fout << "CNF built successfully and loaded into TREE\nCNF: \"" << exp << "\"\n";
					}

					cSet.deconstruct();
					cSet.setFromCNF(tree, 0);
					memset(exp, 0, MAX_STRING);
					cSet.getStr(exp);

					if (VERBOSE_LEVEL > 0)
					{
						cout << "CLAUSESET built successfully: \"" << exp << "\"\n";
						if (fileOutput) fout << "CLAUSESET built successfully: \"" << exp << "\"\n";
					}
				}
			}
			else if (!strcmp("CLAUSESET", commands[1]))
			{
				if (!inpClause[0])
				{
					cout << "Warning: CLAUSES variable is empty\n";
					if (fileOutput) fout << "Warning: CLAUSES variable is empty\n";
				}
				else
				{
					cSet.deconstruct();
					int err = cSet.construct(inpClause);
					if (err == -1)
					{
						cout << "CLAUSESET built successfully\n";
						if (fileOutput) fout << "CLAUSESET built successfully\n";
					}
					else if (err == -2)
					{
						cout << "Warning: Missing ending '}'\n";
						if (fileOutput) fout << "Warning: Missing ending '}'\n";
					}
					else
					{
						cout << "Warning: Syntax error at position: " << err << " '" << inpClause[err]<<"'\n";
						if (fileOutput) fout << "Warning: Syntax error at position: " << err << " '" << inpClause[err] << "'\n";
					}
				}
			}
			else
			{
				cout << "Incorrect syntax: unknown argument \"" << commands[1] << "\"\n";
				if (fileOutput) fout << "Incorrect syntax: unknown argument \"" << commands[1] << "\"\n";
			}
		}
		else if (!_strcmpi("APPLY", commands[0]))
		{
			if (commands[1][0] == 0)
			{
				cout << "Incorrect syntax: missing argument\n";
				if (fileOutput) fout << "Incorrect syntax: missing argument\n";
			}
			else if (commands[2][0] != 0)
			{
				cout << "Incorrect syntax: too many arguments\n";
				if (fileOutput) fout << "Incorrect syntax: too many arguments\n";
			}
			else if (!strcmp("RESOLUTION", commands[1]))
			{
				if (!cSet.nrClauses)
				{
					cout << "Warning: CLAUSESET object is empty\n";
					if (fileOutput) fout << "Warning: CLAUSESET object is empty\n";
				}
				else
				{
					cSet.classicResolution();
				}
			}
			else if (!strcmp("DAVIS_PUTNAM", commands[1]))
			{
				if (!cSet.nrClauses)
				{
					cout << "Warning: CLAUSESET object is empty\n";
					if (fileOutput) fout << "Warning: CLAUSESET object is empty\n";
				}
				else
				{
					int err = cSet.DPResolution();
					if(err)
						cout << "Satisfiable\n";
					else
						cout << "Unsatifiable\n";
				}
			}
			else
			{
				cout << "Incorrect syntax: unknown argument \"" << commands[1] << "\"\n";
				if (fileOutput) fout << "Incorrect syntax: unknown argument \"" << commands[1] << "\"\n";
			}
		}
		else if (!_strcmpi("RUN", commands[0]))
		{
			if (commands[1][0] == 0)
			{
				cout << "Incorrect syntax: missing argument\n";
				if (fileOutput) fout << "Incorrect syntax: missing argument\n";
			}
			else if (commands[2][0] != 0)
			{
				cout << "Incorrect syntax: too many arguments\n";
				if (fileOutput) fout << "Incorrect syntax: too many arguments\n";
			}
			else
			{
				fin.open(commands[1]);
				if (!fout)
					cout << "Warning: Cannot open file\n";
				else
				{
					strcpy_s(IN_FILE, MAX_FILE, commands[1]);
					fileInput = true;
					if (VERBOSE_LEVEL > 1) cout << "Running: \"" << IN_FILE << "\"\n";
				}
			}
		}
		else if (!_strcmpi("DISPLAY", commands[0]))
		{
			if (commands[1][0] == 0)
			{
				cout << "Incorrect syntax: missing argument\n";
				if (fileOutput) fout << "Incorrect syntax: missing argument\n";
			}
			else if (commands[2][0] != 0)
			{
				cout << "Incorrect syntax: too many arguments\n";
				if (fileOutput) fout << "Incorrect syntax: too many arguments\n";
			}
			else if (!strcmp("TREE", commands[1]))
			{
				if (!tree.nrNodes)
				{
					cout << "Warning: TREE object is empty\n";
					if (fileOutput) fout << "Warning: TREE object is empty\n";
				}
				else
				{
					tree.display();
				}
			}
			else
			{
				cout << "Incorrect syntax: unknown argument \"" << commands[1] << "\"\n";
				if (fileOutput) fout << "Incorrect syntax: unknown argument \"" << commands[1] << "\"\n";
			}
		}
		else if (!_strcmpi("SAVE", commands[0]))
		{
			if (commands[1][0] == 0)
			{
				cout << "Incorrect syntax: missing argument\n";
				if (fileOutput) fout << "Incorrect syntax: missing argument\n";
			}
			else if (commands[2][0] != 0)
			{
				cout << "Incorrect syntax: too many arguments\n";
				if (fileOutput) fout << "Incorrect syntax: too many arguments\n";
			}
			else if (!strcmp("STRING", commands[1]))
			{
				if (!inpString[0])
				{
					cout << "Warning: STRING variable is empty\n";
					if (fileOutput) fout << "Warning: STRING variable is empty\n";
				}
				else
				{
					strcpy_s(saveString, MAX_STRING, inpString);
					cout << "STRING variable saved\n";
					if (fileOutput) fout << "STRING variable saved\n";
				}
			}
			else if (!strcmp("CLAUSES", commands[1]))
			{
				if (!inpString[0])
				{
					cout << "Warning: CLAUSES variable is empty\n";
					if (fileOutput) fout << "Warning: CLAUSES variable is empty\n";
				}
				else
				{
					strcpy_s(saveString, MAX_STRING, inpClause);
					cout << "CLAUSES variable saved\n";
					if (fileOutput) fout << "CLAUSES variable saved\n";
				}
			}
			else
			{
				cout << "Incorrect syntax: unknown argument \"" << commands[1] << "\"\n";
				if (fileOutput) fout << "Incorrect syntax: unknown argument \"" << commands[1] << "\"\n";
			}
		}
		else if (!_strcmpi("SEP", commands[0]))
		{
			cout << "__________________________________________________________________________\n\n";
		}
		else if (!_strcmpi("HELP", commands[0]))
		{
			cout << "LEE 2022 [version 1.1 64-bit]\n\n      ---Commands---\nBUILD   [TREE|TABLE|NNF|DNF|CNF|SIMPLIFIEDFORM]   Computes the given object\n HELP                                             Shows this page";
		}
		else if (!_strcmpi("PAUSE", commands[0]))
		{
			cout << "Press enter to continue\n";
			getchar();
		}
		else if (!_strcmpi("EXIT", commands[0]))
		{
			if(fileInput)
				cout << "Reached EXIT in: \"" << IN_FILE << "\"\n";
			fin.close();
			fout.close();
			break;
		}
		else if (commands[0][0])
		{
			cout << "Unknown syntax: \"" << commands[0] << "\"\n";
			if (fileOutput) fout << "Unknown syntax: \"" << commands[0] << "\"\n";
		}
	}

	getchar();
}