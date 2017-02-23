/**
 *  Universidad Simon Bolivar
 *  CI-5651 - Diseño de Algoritmos I
 *  @author : Salvador Di Renzo 10-10296
 *  @author : Stefani Castellanos 11-11394
 *
 *  Tools for reading the file and creating the graph
 */


#ifndef READER_HPP_
#define READER_HPP_

#include "Graph.hpp"
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
using namespace std;


//get_int. Given a string, extracts the only integer value
int get_int(string line){

	char * token; //Allows to save each sub-string from the variable line

	token = strtok((char*)line.c_str()," :");

	while (token != NULL){

		if(isdigit(token[0])) { return atoi(token); }

		else{ token = strtok(NULL," :");}

	}

	return 0;
}


//get_edge. Given a string of 4 integers, returns an edge with those values
Edge get_edge(string line){

	char * token; //Allows to save each sub-string from the variable line
	int v1,v2,cost,benefit; //Each of the edge values (origin - destiny - cost - benefit)

	token = strtok((char*)line.c_str()," ");
	v1 = atoi(token);
	token = strtok(NULL," ");
	v2 = atoi(token);
	token = strtok(NULL," ");;
	cost = atoi(token);
	token = strtok(NULL," ");
	benefit = atoi(token);

	//cout << v1 << " " << v2 << " " << cost << " " << benefit << endl;

	Edge result(v1,v2,cost,benefit); //Structure that contains the edge's info

	return result;

}

//ReadFile creates the Grpah's edges with the read values
Graph readFile(const char* path){

	int size = 0;    //Number of vertex
	int r_edges = 0; //Number of Required edges
	int n_edges = 0; //Number of Non Required edges
	string line; //Value of the read line
	vector<Edge> twice_benefit; // List of positive beneffit edges after crossing them twice
	vector<Edge> positive_benefit; // List of positive benefit edges;
	vector<Edge> negative_benefit; // List of negative benefit edges;
	ifstream sourcefile(path); //File to be read

	//Make sure the file exists
	if(sourcefile.is_open()){

		//Getting the number of nodes
		getline(sourcefile,line);
		size = get_int(line);
		//cout << size << endl;

		//Getting the number of required edges
		getline(sourcefile,line);
		r_edges = get_int(line);
		//cout << r_edges << endl;

		//Creating each of the required edges
		for (int i = 0; i < r_edges; i = i+1){

			getline(sourcefile,line);
			if(get_edge(line).get_benefit() > 2*get_edge(line).get_cost()){
				twice_benefit.push_back(get_edge(line));
			}
			else {
				positive_benefit.push_back(get_edge(line));}
		}

		//Getting the number of the non required edges
		getline(sourcefile,line);
		n_edges = get_int(line);
		//cout << n_edges << endl;

		//Creating each of the non required edges
		for (int i = 0; i < n_edges; i = i+1){

			getline(sourcefile,line);
			negative_benefit.push_back(get_edge(line));
		}

		sourcefile.close();

	}

	else { cout << "Unable to read specified file..." << endl; }

	Graph result(size,r_edges,n_edges,twice_benefit,positive_benefit,negative_benefit); //Structure that contains the resulting Graph
	return result;
}

#endif // READER_HPP_