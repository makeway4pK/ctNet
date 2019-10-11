#include <iostream>
#include <fstream>
#include <limits.h>
#include <stack>
#define MAX 10		// no. of max cts (required coz of arrays)
#define NAMESZ 20	// no. of max chars in ct names
using namespace std; 	// ct means city


typedef struct output_struct{
	int total_flightTime;
	stack<int> path;
} output;

class Map
{
	int *g[MAX][MAX];  //array of pointers, so use -> to access cost/time
	bool visited[MAX];  //    ^simply check for existence of pointer to test connection
	int label[MAX], n;  //n= no. of vertices


	/*
	map<int, output> cache; // dynamic programming concept coming soon
	*/

  public:
  	string ctName[MAX]; // search to get ct index from ct names
	Map()
	{
		n=0;
		for (int i = 0; i < MAX; i++){
			label[i]=INT_MAX;
			visited[i]=false;
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
			g[x][y] = new int;
			db >> g[x][y];
			db >> g[x][y];
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

		int *noEdge = new int; //if cost/time =0 => not connected
		noEdge = 0;
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
		int *e = new int;						 //if cost/time =0 => not connected
		for (int i = 0; i < n; i++)
			for (int j = 0; j < i; j++)
			{
				db.read((char *)e, sizeof(int)); //read into e
				if (!e)					  // check if connected
					g[i][j] = g[j][i] = NULL;	 //null pointer if not
				else
				{
					g[i][j] = new int; // new edge if yes
					g[i][j]->time = e;
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
						 << i + 1 << " " << j + 1 << " " << *g[i][j];
	}

	output dijkstraFunName(int source,int destination){
		output o;
		o.total_cost = 0;
		stack<int> s;
		int prev_Node[MAX], label[MAX], min, min_index, source1;
		bool visited[MAX];
		for (int i = 0; i < MAX; i++)
			prev_Node[i] = source, label[i] = INT_MAX, visited[i] = false;
		label[source] = 0, visited[source] = true;
		source1 = source;
		while (source != destination) {
			min = INT_MAX;
			for (int i = 0; i < MAX; i++)
				if (g[source][i] && !visited[i] && label[i] >= label[source] + g[source][i]->time) {
					label[i] = label[source] + *g[source][i];
					prev_Node[i] = source;
				}
			for (int i = 0; i < MAX; i++)
				if (!visited[i] && min >= label[i]) {
					min = label[i];
					min_index = i;
				}
			visited[min_index] = true;
			source = min_index;
		}
		while (destination != source1) {
			o.path.push(destination+1);
			destination = prev_Node[destination];
		}
		o.path.push(destination+1);
		o.total_flightTime=label[destination];
		return o;
	}
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
