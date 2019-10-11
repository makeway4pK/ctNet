#include <iostream>
#include <fstream>
#include <limits.h>
#include <stack>
#include <map>
#define FILE "India"
#define MAX 30		 // no. of max cts (required coz of arrays)
#define NAMESZ 20	// no. of max chars in ct names
using namespace std; // ct means city

typedef struct output_struct
{
	int total_flightTime;
	stack<int> path;
} output;

class Map
{
	int *g[MAX][MAX]; //array of pointers
					  //    ^simply check for existence of pointer to test connection
	int n;			  //n= no. of vertices
	map<int, output> cache;

	/*
	map<int, output> cache; // dynamic programming concept coming soon
	*/

  public:
	char ctName[MAX][NAMESZ]; // search to get ct index from ct names
	Map()
	{
		n = 0;
		for (int i = 0; i < MAX; i++)
		{
			for (int j = 0; j < MAX; j++)
				g[i][j] = NULL;
		}
	}

	void convert(string str = "")
	{
		if (!str.size())
		{
			cout << "\n Open (text file name): ";
			cin >> str;
		}
		str += ".txt";
		fstream db;

		db.open(str.c_str(), ios::in);

		n = 0;
		string ct;

		// read city names until line "." occurs
		getline(db, ct);
		while (db && ct != ".")
		{
			ctName[n][ct.copy(ctName[n], ct.size(), 0)] = '\0';
			getline(db, ct);
			n++;
		}
		ct = "";
		int x, y;
		//read edge coordinates, cost, time(minutes) on each line
		while (db >> x >> y) //natural no.s
		{

			x--;
			y--;
			g[x][y] = new int;
			db >> *g[x][y];
			g[y][x] = g[x][y];
		}
		db.close();

		// start writing

		int i = 3;
		while (i--)
			str.pop_back();
		str += "bin";

		db.open(str.c_str(), ios::out);
		db.write((char *)&n, sizeof(n));			 // no. of cts
		db.write((char *)&ctName, (n + 1) * NAMESZ); //ctName truncated to NAMESZ bytes(chars)

		int *no = new int;
		*no = 0;
		for (int i = 0; i < n; i++)
			for (int j = 0; j < i; j++)
			{
				if (!g[i][j])
					g[i][j] = no; //if cost/time =0 => not connected
				db.write((char *)g[i][j], sizeof(int));
			}
		db.close();
	}

	void load(string str = "")
	{
		if (!str.size())
		{
			cout << "\n Open (binary file name):";
			cin >> str;
		}
		str += ".bin";
		ifstream db(str.c_str(), ios::binary);

		db.read((char *)&n, sizeof(n));				// read no. of cts
		db.read((char *)&ctName, (n + 1) * NAMESZ); // read ct names
		int e = 0;									//if time =0 => not connected
		for (int i = 0; i < n; i++)
			for (int j = 0; j < i; j++)
			{
				db.read((char *)&e, sizeof(int)); //read into e
				if (!e)							  // check if connected
					g[i][j] = g[j][i] = NULL;	 //null pointer if not
				else
				{
					g[i][j] = new int; // new edge if yes
					*g[i][j] = e;
					g[j][i] = g[i][j];
				}
			}
		db.close();
	}

	void sho()
	{
		cout << "\n No.(Cities):" << n;
		cout << "\n City names:";
		for (int i = 0; i < n; i++)
			cout << "\n " << i + 1 << ".\t" << ctName[i];
		cout << "\n Adjacency Matrix:";
		for (int i = 0; i < n; i++)
		{
			cout << endl;
			for (int j = 0; j < n; j++)
				cout << " " << (g[i][j] ? "1" : ".");
		}
		cout << "\n Raw edge data:";
		for (int i = 0; i < n; i++)
			for (int j = 0; j < i; j++)
				if (g[i][j])
					cout << endl
						 << i + 1 << " " << j + 1 << " " << *g[i][j];
	}

	output skip(int i, int j) // Dynamic programming check
	{
		int t;
		output o;
		try
		{
			o = cache.at(MAX * min(i, j) + max(i, j)); // will it work?
			if (i > j)
			{
				stack<int> s = o.path;
				while (!o.path.empty())
					o.path.pop();
				while (!s.empty())
				{
					o.path.push(s.top());
					s.pop();
				}
				cout << "\n Skipped from " << ctName[i] << " to " << ctName[j] << endl;
				return o;
			}
		}
		catch (out_of_range)
		{
			o.total_flightTime = 0;
			return o;
		}
	}

	output dijkstra(int source, int destination)
	{
		source--;
		destination--;
		output o;
		o.total_flightTime = 0;

		o = skip(source, destination);
		if (o.total_flightTime)
			return o;

		int prev_Node[MAX], label[MAX], mini, min_index, source1, destination1;
		bool visited[MAX];
		for (int i = 0; i < MAX; i++)
			prev_Node[i] = source, label[i] = INT_MAX, visited[i] = false;
		label[source] = 0, visited[source] = true;

		source1 = source;
		destination1 = destination;
		while (source != destination)
		{
			mini = INT_MAX;
			for (int i = 0; i < MAX; i++)
				if (g[source][i] && !visited[i] && label[i] >= label[source] + *g[source][i])
				{
					label[i] = label[source] + *g[source][i];
					prev_Node[i] = source;
				}
			for (int i = 0; i < MAX; i++)
				if (!visited[i] && mini >= label[i])
				{
					mini = label[i];
					min_index = i;
				}

			o = skip(min_index, destination1);
			if (o.total_flightTime)
				break;

			visited[min_index] = true;
			source = min_index;
		}

		o.total_flightTime += label[source];
		while (source != source1)
		{
			o.path.push(source + 1);
			source = prev_Node[source];
		}
		o.path.push(source + 1);

		cache.insert(pair<int, output>(MAX * min(source1, destination1) + max(source1, destination1), o));
		return o;
	}
};

int main()
{
	Map b;
	char ch;
	int s, d;
	output o;
	do
	{
		cout << "\n 1 - convert";
		cout << "\n 2 - load";
		cout << "\n 3 - sho";
		cout << "\n 4 - path\n";
		cin >> ch;
		switch (ch)
		{
		case '0':
			return 0;

		case '1':
			// a.convert();
			b.convert(FILE);
			break;
		case '2':
			// b.load();
			b.load(FILE);
			break;
		case '3':
			b.sho();
			break;
		case '4':
			cout << "\n Enter source:";
			cin >> s;
			cout << "\n Enter destination:";
			cin >> d;
			o = b.dijkstra(s, d);
			cout << " T=" << o.total_flightTime << "\n Path: ";
			while (!o.path.empty())
			{
				cout << o.path.top() << " -> ";
				o.path.pop();
			}
		}
	} while (1);
}
/*

*/
