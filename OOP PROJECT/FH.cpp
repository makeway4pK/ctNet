#include <iostream>
#include <fstream>
#define MAX 10		 // no. of max cts (required coz of arrays)
#define NAMESZ 20	// no. of max chars in ct names
using namespace std; // ct means city

/*
typedef struct output_struct{
	int total_cost;
	int total_flightTime;
	stack<string> path;
} output;
*/

typedef struct edge
{
	int cost, time;
} edge;

class Map
{
	edge *g[MAX][MAX];  //array of pointers, so use -> to access cost/time
	bool visited[MAX];  //    ^simply check for existence of pointer to test connection
	int label[MAX], n;  //n= no. of vertices
	string ctName[MAX]; // search to get ct index from ct names

	/*
	map<int, output> cache; // dynamic programming concept coming soon
	*/

  public:
	Map()
	{
		for (int i = 0; i < MAX; i++)
			for (int j = 0; j < MAX; j++)
				g[i][j] = NULL;
	}

	void convert(string str = "")
	{
		if (!str.size())
		{
			cout << "\n Open (text file name): ";
			cin >> str;
			str += ".txt";
		}
		fstream db;

		db.open(str.c_str(), ios::in);

		n = 0;
		string ct;

		// read city names until line "." occurs
		getline(db, ct);
		while (!db.eof() && ct != ".")
		{
			ctName[n] = ct;
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
			g[x][y] = new edge;
			db >> g[x][y]->cost;
			db >> g[x][y]->time;
			g[y][x] = g[x][y];
		}
		db.close();

		// start writing

		int i = 3;
		while (i--)
			str.pop_back();
		str += "bin";

		db.open(str.c_str(), ios::out);
		db.write((char *)&n, sizeof(n)); // no. of cts
		for (int i = 0; i < n; i++)
			db.write((char *)&ctName[i], NAMESZ); //ctName truncated to NAMESZ bytes(chars)

		edge *noEdge = new edge; //if cost/time =0 => not connected
		noEdge->cost = 0;
		noEdge->time = 0;
		for (int i = 0; i < n; i++)
			for (int j = 0; j < i; j++)
			{
				if (!g[i][j])
					g[i][j] = noEdge;
				db.write((char *)g[i][j], sizeof(edge));
			}
		delete noEdge;
		db.close();
	}

	void load(string str = "")
	{
		if (!str.size())
		{
			cout << "\n Open (binary file name):";
			cin >> str;
			str += ".bin";
		}
		ifstream db(str.c_str(), ios::binary);
		db.read((char *)&n, sizeof(n)); // read no. of cts
		for (int i = 0; i < n; i++)
			db.read((char *)&ctName[i], NAMESZ); // read ct names
		edge *e = new edge;						 //if cost/time =0 => not connected
		for (int i = 0; i < n; i++)
			for (int j = 0; j < i; j++)
			{
				db.read((char *)e, sizeof(edge)); //read into e
				if (!e->cost)					  // check if connected
					g[i][j] = g[j][i] = NULL;	 //null pointer if not
				else
				{
					g[i][j] = new edge; // new edge if yes
					g[i][j]->cost = e->cost;
					g[i][j]->time = e->time;
					g[j][i] = g[i][j];
				}
			}
		delete e;
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
						 << i + 1 << " " << j + 1 << " " << g[i][j]->cost << " " << g[i][j]->time;
	}

	/*
	output dijkstraFunName(string src,string dest){}  // for Subhradeep
	*/
};

int main()
{
	Map a, b;
	char ch;
	do
	{
		cout << "\n 1 - convert";
		cout << "\n 2 - load";
		cout << "\n 3 - sho";
		cin >> ch;
		switch (ch)
		{
		case '0':
			return 0;

		case '1':
			// a.convert();
			a.convert("a");
			break;
		case '2':
			// b.load();
			b.load("a");
			break;
		case '3':
			b.sho();
			break;
		}
	} while (1);
}
/*
1 2 3
*/