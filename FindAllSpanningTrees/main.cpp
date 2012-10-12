/*** FILE: main.cpp
**
** ABSTRACT:
**  Enumerates all spanning trees in a directed graph 
**   
**
** DOCUMENTS:
**
** Gabow, Harold N. and Myers, Eugene W. Finding all spanning trees of directed and undirected graphs. 
** SIAM J. of Comput., Vol. 7, No. 3, August 78, pp. 280-287 
**
** Cormen, Thomas H. et.al. Introduction to algorithms. The MIT Press, Cambridge, 1990
**
** AUTHOR:
**   Mikhail Mozolin
**
** CREATION DATE:
**   10/10/2012
**
** NOTES:
**   None
**
** HISTORY:
** 000 - Oct 10, 2012 - M. Mozolin  - Creation
*/


#include <iostream>
#include <iterator>
#include <list>
#include <vector>
#include <algorithm>

using namespace std;


//number of vertices in a graph
const int V = 4;

//represents an edge of a graph
class edge
{
public:
	int from_node; //id of the from node
	int to_node; //id of the to node
	int operator!=(const edge& e) {  return (this->from_node != e.from_node || this->to_node != e.to_node); }
	int operator==(const edge& e) {  return (this->from_node == e.from_node && this->to_node == e.to_node); }
	edge() {from_node=0;to_node=0;}
	edge(int from_node, int to_node):from_node(from_node),to_node(to_node){};
	edge(const edge& e):from_node(e.from_node),to_node(e.to_node){};
};

const int NIL = -1;

enum COLOR {WHITE,GRAY,BLACK};

//represents a vertex in a graph
class Vertex
{
public:
	int d;
	int f;
	int parent; 
	COLOR color;
};

// represents a graph
class graph
{

public:
	//members
	int nverteces_originally_connected; //number of vertices that have edges in graph upon construction
	int root_vertex;
	vector<vector<int>> edge;
	int time;
	Vertex* vertex;
	
	//constructors 
	graph(const int nverteces){edge.assign(nverteces, vector<int>());} 

	//operators
	graph operator=( const graph& gr);


	//member functions
	int ownsvertex(const int ind);
	void removeedge(const int from, const int to);
	void addedge(const int from, const int to);
	void print();
	//traverses a tree in depth first order
	void DFS_Visit(int u);
    int getNconnectedverteces();

};

graph graph::operator=(const graph& gr)
{
	
	nverteces_originally_connected= gr.nverteces_originally_connected;
	
	root_vertex = gr.root_vertex;
	
	time = gr.time;

	edge = gr.edge;

	
	return gr;

}





void graph::DFS_Visit(int u)
{

	vertex[u].color = GRAY;
	time++;
	vertex[u].d = time;

	for (int i = 0; i < (int)edge[u].size(); i++)
	{
		if ( vertex[edge[u][i]].color == WHITE)
		{
			vertex[edge[u][i]].parent = u;
			DFS_Visit(edge[u][i]);
		}

	}

	vertex[u].color = BLACK;
	time++;
	vertex[u].f = time;

}


//does this vertex have inbound or outbound connections?
int graph::ownsvertex(const int ind)
{
	
	//there is at least one outbound connection
	if (edge[ind].size() > 0)
	{
		return 1;

	}


		
	
	for (int i = 0; i <  (int)edge.size(); i ++)
	{
		vector<int>::iterator it;
		it = find(edge[i].begin(),edge[i].end(),ind);
		if (it != edge[i].end())
		{
			return 1;
		
		}
	
	}

	return 0;


}


void graph::removeedge(const int from, const int to)
{

	vector<int>::iterator it;

	it = find(edge[from].begin(),edge[from].end(),to);

	edge[from].erase(it);
	

}

void graph::addedge(const int from, const int to)
{

	
	edge[from].insert(edge[from].begin(), to);


}

void graph::print()
{
	 
	
	cout << "\n";
	for (int i = 0; i < (int)edge.size(); i++)
	{
		cout << i << "\t";

		for (int j = 0; j < (int)edge[i].size(); j ++)
		{
			if ( j != 0) {cout << ",";}
			cout << edge[i][j] ;
		}

		cout << "\n";


	}
	cout << "\n";     
}




//gets number of connected vertices in the graph, if a vertex does not 
//have an incoming or an outgoing edge
//then it is not counted towards the result
int graph::getNconnectedverteces()
{
	vector<int> tally(V, 0);
	
	
	//increase tallies of vertices that have an outgoing edge
	for (int i = 0; i < (int)edge.size(); i ++)
	{
		if (edge[i].size() > 0)
		{
			 tally[i]++;
		}
	}

	//increase tallies of vertices that have an incoming edge
	for (int i = 0; i < (int)edge.size(); i ++)
	{
		for (int j = 0; j < (int)edge[i].size(); j ++)
		{
			tally[edge[i][j]]++;


		}

	}

    int Nconnected = 0;
	for (int i = 0; i < (int)tally.size(); i++)
	{
		if (tally[i] > 0)
		{
			Nconnected++;

		}


	}


	return Nconnected;

}



graph G(V);

graph T(V);

graph L(V);

list<edge> F;

//number of spanning trees found in G
int nspanningtrees = 0;


void GROW()
{

	

    int TnSpannedVertices = T.getNconnectedverteces();
		
	if (TnSpannedVertices == V)
	{
		L = T;
		nspanningtrees++;
		cout << "---------spanning tree " << nspanningtrees << "----------\n";
		L.print();
		cout << "----------------------------------\n";

		if (L.vertex !=  0)
		{
			delete [] L.vertex;
		}
		
	    L.vertex = new Vertex[V];
		
		for(int i = 0; i < V; i++)
		{
			L.vertex[i].color = WHITE;
			L.vertex[i].parent = NIL;
			L.vertex[i].f = NIL;
			L.vertex[i].d = NIL;
		}

		L.time = 0;

		L.DFS_Visit(L.root_vertex);

	}
	else
	{
		list<edge> FF;
		
	
		int b = 1;
		int iterations = 0;
		do 
		
		{
			
			int size = F.size();

			
			edge e = F.front();


			F.pop_front();

			//let e go from T to vertex v, v does not belong to T
			int v = e.to_node;
		
			//add e to T
			T.addedge(e.from_node, v);

			//Debug
			//T.print();
						
			//make a temp copy of F before changing it;
			list<edge> Fcopy(F);
			
			
			
			//push each edge (v, w) in G, where w does not belong to T, onto F
			//Note: this means each edge coming out of v
			for (int i = 0; i < (int)G.edge[v].size(); i++)
			{
				int w = G.edge[v][i];
				if (!T.ownsvertex(w))
				{
					//got an edge
					F.push_front(edge(v,w));

				}


			}

			//remove each edge (w,v) (that is, each edge that is incident to v) where w belongs to T, from F
			for (int w = 0; w < (int)G.edge.size(); w++)
			{
				if (T.ownsvertex(w))
				{
					vector<int>::iterator it;
					it = find(G.edge[w].begin(),G.edge[w].end(),v);
					if (it != G.edge[w].end())
					{
						//an edge incident from w and incident to v has been found
						F.remove(edge(w,v));

					}
				
				}
	
			}

			
			GROW();

			//restore F
			F = Fcopy;
			

			//remove e from T and from G; add e to FF
			 T.removeedge(e.from_node, e.to_node);
			 
			G.removeedge(e.from_node, e.to_node);
			

			FF.push_front(e);

			//bridge test
			b = 1;
			
			//go over all edges in G coming into v
			for (int w = 0; w < (int)G.edge.size(); w++)
			{
				vector<int>::iterator it;
				it = find(G.edge[w].begin(),G.edge[w].end(),v);
				if (it != G.edge[w].end())
				{
					//an edge incident to v is found

					
						if (!(L.vertex[v].d < L.vertex[w].d && L.vertex[w].d < L.vertex[w].f && L.vertex[w].f < L.vertex[v].f))
						{
							//w is NOT a descendant of v
							b = 0;
							break; //for
						}
								
				
				
				}
		

			}

			
		}

		//we want to stop when b is true; this for bridge test
		while (!b);

		
		

		//reconstruct G:
		//pop each edge e from FF, push e onto F, and add e to G

		while (!FF.empty())
		{
			edge e = FF.front();
			FF.pop_front();
			F.push_front(e);
			G.addedge(e.from_node,e.to_node);

		} //end while
     } //end else

}


//S finds all spanning trees rooted at r in a directed graph G rooted at r
void S()
{
	
	G.root_vertex = 0;
	G.addedge(0,1); //edge 1
	G.addedge(0,2); //edge 2
	G.addedge(1,3); //edge 3
	G.addedge(3,2); //edge 4
	G.addedge(1,2); //edge 5
	G.addedge(2,1); //edge 6
	
	cout << "---------Example graph------------\n";
	G.print();
	cout << "----------------------------------\n";

	
	
	

	//discover all descendants of the root vertex in G
	//and initialize list F with them

	for (int i = 0; i < (int)G.edge[G.root_vertex].size(); i++)
	{
		
			F.push_front(edge(G.root_vertex,G.edge[G.root_vertex][i]));
	
	}


	T.root_vertex = 0;
	
	L.root_vertex = 0;
	
	 nspanningtrees = 0;

	 GROW();

	 cout << nspanningtrees << " spanning trees found.\n\n";

}



void main()
{

	S();
	
	//user should hit Return to continue
	//useful during debugging out of Visual Studio.
	system("pause");

}