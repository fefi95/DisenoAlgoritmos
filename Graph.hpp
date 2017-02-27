/**
 *  Universidad Simon Bolivar
 *  CI-5651 - Diseño de Algoritmos I
 *  @author : Salvador Di Renzo 10-10296
 *  @author : Stefani Castellanos 11-11394
 *
 *  Information of the graph
 */

#ifndef _GRAPH_HPP
#define _GRAPH_HPP

#include <vector>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Union-find.hpp"

using namespace std;

/**************************************************************************//**
 *  Edge. Information of an edge of the graph, it's benefit and cost.
 *****************************************************************************/
 class Edge {
     private:
         int v1, v2;  // Edge of the graph
         int be;      // Edge's benefit
         int ce;      // Edge's cost
         bool two_way; // Defines if the edge is a two way path
         int set;

     public:
         Edge(int, int, int, int);

         //Prints the current Edge Info
         void print(std::ostream &os);
         int get_v1() { return this -> v1; }
         int get_v2() { return this -> v2; }
         int get_cost() {return this -> ce;}
         int get_benefit() {return this -> be;}
         int get_set() {return this->set;}
         void set_set(int set) {this ->set = set;}
         bool is_two_way() {return (this->be > (2*(this->ce)));}
         void set_two_way() {this->two_way = true;}
         bool get_two_way() {return this->two_way;}
         Edge invert();
         // bool operator< (const Edge &edge) const {
         //     return this -> crossE2 < edge.crossE2;
         // }
 };


 //Create the inverse of a determinated Edge
 Edge Edge::invert(){
	 Edge edge(this->v2,this->v1,this->ce,this->be);
	 return edge;
 }

 // Constructor
 Edge::Edge (int v1, int v2, int ce, int be) {
   this -> v1 = v1;
   this -> v2 = v2;
   this -> be = be;
   this -> ce = ce;
   this -> two_way  = false;
   this->set = -1;
 }

inline void Edge::print(std::ostream &os)  {

	os << "Start: " << this->v1 << " End: " << this->v2 << " Cost: " <<
	this->ce << " Benefit: "<< this->be << endl;

}

//Aux function to specify the order to sort a list
//Does not form part of the namesapce Edge
bool comp (Edge a, Edge b) {

	return ((a.get_benefit() - a.get_cost()) > (b.get_benefit() - b.get_cost()));

}

inline std::ostream& operator<<(std::ostream &os, Edge &edge) {
    edge.print(os);
    return os;
}

void printEdges(std::ostream &os, std::vector<Edge> edges) {
    for(std::vector<Edge>::iterator it= edges.begin(); it != edges.end(); ++it) {
		it -> print(os);
	}
}

/**
 *  Checks if an vector of edges is connected. Any connected graph
 *  has at most 2 vertexes with only one edge incident.
 *  @param n_vertex: number of vertexes of the graph
 *  @param edges: vector of edges (graph) to be checked
 *  @return true/false
 */
bool isConnected(int n_vertex, std::vector<Edge> edges) {
    int n_edges [n_vertex]; // array for the number of edges incident in every node
    int v1, v2; // Edge

    // Initialize array of vertexes
    for (int i = 0; i < n_vertex; ++i) {
        n_edges[i] = 0;
    }
    for(std::vector<Edge>::iterator edge = edges.begin(); edge != edges.end(); ++edge) {
        v1 = edge -> get_v1() - 1;
        v2 = edge -> get_v2() - 1;
        ++n_edges[v1];
        ++n_edges[v2];
    }

    int count = 0;
    for (int i = 0; i < n_vertex; ++i) {
        if (n_edges[i] == 1) { // A vertex does not have an even number of edges
            ++count;
            if (count > 2) {
                return false;
            }
        }
    }
    return true;
}

void assignSet(std::vector<Edge> &graph, int start, int set, std::vector<int> succs[], std::vector<int> ancs[], std::vector<int> visited){

	if((start == 1) || (visited[start-1] == 1)){return;}

	//For each edge of the graph whose v1 is equal to start, we assign the set it belongs
	for(std::vector<Edge>::iterator it = graph.begin(); it != graph.end(); ++it){


		if(it->get_v1() == start){
			if(it->get_set() == -1){ it->set_set(set);}
		}
	}

	//after being assigned all the set for v1 edge's, we proceed to do the same with v1 ancestors and successors
	visited[start-1] = 1;

	for(std::vector<int>::iterator i = succs[start-1].begin(); i != succs[start-1].end(); ++i){

		assignSet(graph,*i,set,succs,ancs,visited);
	}

	for(std::vector<int>::iterator j = ancs[start-1].begin(); j != ancs[start-1].end(); ++j){

		assignSet(graph,*j,set,succs,ancs,visited);

	}

}

/**
 *  Add edges to a connected graph so it becames an eurelian cycle
 *  @param n_vertex: number of vertexes of the graph
 *  @param graph: vector of edges. Initial graph
 *  @param edges: vector of edges to be added to the graph
 *  @return true/false
 */

std::vector<Edge> makeEurelian(int n_vertex, std::vector<Edge> graph, std::vector<Edge> extras){

    std::vector<Edge> eurelian; // Variable for the returning of the functions
    std::vector<Edge> origins; //Variable for getting the edges with the origin
    std::vector<Edge> inverted;//Variable for the inverse graph
    std::vector<Edge>::iterator extraIt; //Iterator for the extra edges list
    std::vector<int> successors [n_vertex]; //array for the successors of each node
    std::vector<int> ancestors [n_vertex]; //array for the ancestors of each node
    std::vector<int> visited(n_vertex);
    int even [n_vertex] = {0}; // array to check if a vertex is already an origin
    int v1; //Origin of the edge
    int v2; //Destiny of the edge
    int set; // Variable to specify which set belongs the edge
    bool found; //Variable for the founded edge for completing the graph

    //We proceed to set even property for each node
    for (std::vector<Edge>::iterator it = graph.begin(); it != graph.end(); ++it){

    	v1 = it->get_v1() -1 ;
    	if (v1 == 0) { origins.push_back(*it); }
    	v2 = it->get_v2() - 1;
    	even[v1] = (even[v1] + 1) % 2;
    	even[v2] = (even[v2] + 1) % 2;
    	successors[v1].push_back(v2+1);
    	ancestors[v2].push_back(v1+1);
    	eurelian.push_back(*it);

    }

    for (std::vector<Edge>::iterator it = graph.begin(); it != graph.end(); ++it){

    	v1 = it->get_v1() - 1;
    	//For each vertex, we must see if is has already a pair grade, if not
    	//Also, is always recommended to add the edges with the start node
    	if ((even[v1] != 0) || (v1 == 0)){
    		extraIt = extras.begin();
    		found = false;

    		while((!found) && (extraIt != extras.end())){
    			found = (v1+1) == (extraIt->get_v1());
    			v2 = extraIt->get_v2()-1;
    			//If adding the new edge, sets both vertexes grade even, then
    			if(found && ((even[v2]+1)%2 == 0)){
    				if(extraIt->get_v1() == 1) {origins.push_back(*extraIt);}
    				even[v1] = (even[v1] + 1) % 2;
    				even[v2] = (even[v2] + 1) % 2;
    				successors[v1].push_back(v2+1);
    				ancestors[v2].push_back(v1+1);
    				eurelian.push_back(*extraIt);
    			}
    			++extraIt;
    		}
    	}
    }
    //We proceed now to add the inverted edges to create a non directed graph
    //for(int i = 0; i < graph_size; i++){

    	//v1 = eurelian[i].get_v1();
    	//v2 = eurelian[i].get_v2()-1;
    	//Edge edge = eurelian[i].invert();
    	//eurelian.push_back(edge);
    	//successors[v2].push_back(v1);
    //}
    set = 1;
    for (std::vector<Edge>::iterator startNode = eurelian.begin(); startNode != eurelian.end(); ++startNode){

    	if(startNode->get_v1() == 1) {
			for(int i = 0; i < n_vertex; ++i){
				visited[i] = 0;
			}
			int begin = startNode->get_v2();
			startNode->set_set(set);
			cout << *startNode;
			cout << "set perteneciente: "<< startNode->get_set() << endl;
			assignSet(eurelian,begin,set,successors,ancestors,visited);
			set = set+1;
		}
    }
    return eurelian;
}

std::pair<vector<int>,int> getPath(std::vector<Edge> graph){

	vector<int> path;
	int benefit = 0;

	std::pair<vector<int>,int> result(path,benefit);
	return result;
}

/**************************************************************************//**
 *  Graph. vector of the edges and it's benefits and costs
 *****************************************************************************/
class Graph {
    private:
		int n_vertex;  // Number of vertex of the Graph
        int r2_size;  // Number of edges that get benefit after crossing them twice
		int r_size;   // Number of required that get benefit after crossing them once
		int n_size;   // Number of non required edges of the Graph
		std::vector<Edge> r2_edges; //List of required edges that get benefit after crossing them twice
        std::vector<Edge> r_edges; //List of required edges
        std::vector<Edge> n_edges; //List of non required edges

    public:
        Graph(int n_vertex, int r2_size, int r_size, int n_size,std::vector<Edge> &r2_edges, std::vector<Edge> &r_edges, std::vector<Edge> &n_edges);
        int get_n_vertex()   { return this -> n_vertex; }
        int get_r2_size()  { return this -> r2_size; }
        int get_r_size()   { return this -> r_size; }
        int get_n_size()   { return this -> n_size; }
        std::vector<Edge> get_r2_edges() { return this -> r2_edges; }
        std::vector<Edge> get_r_edges()  { return this -> r_edges; }
        std::vector<Edge> get_n_edges()  { return this -> n_edges; }

        void print(std::ostream &os);

        bool isEulerian();
        std::pair < std::vector<Edge>,std::vector<Edge> > MST();
        void improveSolution();
        std::vector<Edge> solvePRPP();
};

inline std::ostream& operator<<(std::ostream &os, Graph &graph) {
    graph.print(os);
    return os;
}

// Constructor
Graph::Graph (int n_vertex, int r2_size, int r_size, int n_size, std::vector<Edge> &r2_edges, std::vector<Edge> &r_edges, std::vector<Edge> &n_edges) {

	this -> n_vertex = n_vertex;
    this -> r2_size = r2_size;
	this -> r_size = r_size;
	this -> n_size = n_size;
	this -> r2_edges = r2_edges;
	this -> r_edges = r_edges;
	this -> n_edges = n_edges;

    //we proceed to sort the lists for easy work later
	std::sort (this -> r_edges.begin(), this -> r_edges.end(), comp);
	std::sort (this -> n_edges.begin(), this -> n_edges.end(), comp);
}

//Prints the current Graph Info
inline void Graph::print(std::ostream &os)  {

	os << "Number of vertex: " << this->n_vertex << endl;
	os << "Required edges: " << this-> r2_size + r_size << endl;
    printEdges(os, this -> r2_edges);
    printEdges(os, this -> r_edges);
	os << "Non Required edges: " << this->n_size << endl;
    printEdges(os, this -> n_edges);
}

/**
 *  Checks if an vector of edges is an eulerian cycle
 *  @param n_vertex: number of vertexes of the graph
 *  @param edges: vector of edges (graph) to be checked
 *  @return true/false
 */
bool Graph::isEulerian() {

    int even [n_vertex]; // array to check whether a vertex has an even number of edges
    int v1, v2; // Edge

    // Initialize array of vertexes
    for (int i = 0; i < n_vertex; i++) {
        even[i] = 0;
    }
    for(std::vector<Edge>::iterator edge = r_edges.begin(); edge != r_edges.end(); ++edge) {
        v1 = edge -> get_v1() - 1;
        v2 = edge -> get_v2() - 1;
        even[v1] = (even[v1] + 1) % 2;
        even[v2] = (even[v2] + 1) % 2;
    }
    for (int i = 0; i < n_vertex; i++) {
        if (even[i] == 1) { // A vertex does not have an even number of edges
            return false;
        }
    }
    return true;
}

/**
 * Creates the Minimum Spanning Tree (Variation Of Kruskall) for non-required
 * edges connecting isolated connected components
 */
std::pair < std::vector<Edge>,std::vector<Edge> > Graph::MST(){

	std::vector<Edge> mst; // MST for the original graph
	std::vector<Edge> nonMst; // Edges that are not in the MST
    Edge edge(0,0,0,0);
    int v1, v2; // Vertexes that belong to an edge
    Union_find uf(n_vertex); // Disjoint set

    // connected components
    for(std::vector<Edge>::iterator edge = this -> r2_edges.begin(); edge != this -> r2_edges.end(); ++edge) {
        v1 = edge -> get_v1() - 1;
        v2 = edge -> get_v2() - 1;
        if(!uf.connected(v1, v2)){
            uf.set_union(v1, v2);
        }
        mst.push_back(*edge);
    }

    for(std::vector<Edge>::iterator edge = this -> r_edges.begin(); edge != this -> r_edges.end(); ++edge) {
        v1 = edge -> get_v1() - 1;
        v2 = edge -> get_v2() - 1;
        if(!uf.connected(v1, v2)) {
            mst.push_back(*edge);
            uf.set_union(v1, v2);
        }

        else {
        	nonMst.push_back(*edge);
        }
    }

    // All vertexes belong to the same tree
    if (uf.count() > 1) {
        for(std::vector<Edge>::iterator edge = this -> n_edges.begin(); edge != this -> n_edges.end(); ++edge) {
            v1 = edge -> get_v1() - 1;
            v2 = edge -> get_v2() - 1;
            if(!uf.connected(v1, v2)) {
                mst.push_back(*edge);
                uf.set_union(v1, v2);
            }

            else {
            	nonMst.push_back(*edge);
            }
        }
    }
    std::pair < std::vector<Edge>,std::vector<Edge> > Edges(mst,nonMst);
    return Edges;
}

void Graph::improveSolution(){

    //
    // foreach par de lados duplicados e1 = (i;j)y e2 = (i;j) enGsol do
    //     G2sol Eliminar los ladose1 y e2 de Gsol
    //     if G2 sol es un grafo euleriano con mayor beneficio then
    //     //G 0 sol es conexo y todos sus vertices tienen grado par
    //         Gsol G2sol
    //         C Obtener un ciclo euleriano de Gsol

    // Parte 2: Eliminacion de Componente Conexa foreachpar de lados duplicados e1= (i;j)ye2= (i;j) enGsol
    // do G 0 solEliminar los lados e1 y e2 de Gsol
    //     if G 0 sol es un grafo desconectado then
    //         Gcc Se crea un grafo con la componente conexa de G2sol que contiene al deposito
    //         Ccc obtener un ciclo euleriano de Gcc
    //         if beneficio(Ccc)>beneficio(C) then
    //             C Ccc
    //             Gsol Gcc
    // return C
}

std::vector<Edge> Graph::solvePRPP(){

	std::vector<Edge> solution;

    if (this -> isEulerian()){
        return this -> r_edges;
    }
    else {
        //std::vector<Edge> mst = this -> MST();
        // solution = makeEurelian(..., mst);
    }
    return solution;
}


#endif // _GRAPH_HPP
