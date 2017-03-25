/**
 *  Universidad Simon Bolivar
 *  CI-5651 - Diseño de Algoritmos I
 *  @author : Salvador Di Renzo 10-10296
 *  @author : Stefani Castellanos 11-11394
 *
 *  Information of the graph
 */

#include <algorithm>
#include "Reader.hpp"
#include "DGraph.hpp"

std::vector< vector<DEdge> > solParcial;
std::vector< vector<DEdge> > mejorSol;
int beneficioDisponible = 12; //obtener_max_beneficio(); // benefit of the greedy algorithm

int benefit(std::vector< vector<DEdge> > graph) {
    int benefit = 0;
    int vOut = 0; // source

    while (!(graph[vOut]).empty() && vOut < graph.size()) {
        DEdge edge = graph[vOut][0]; // First element
        (graph[vOut]).erase((graph[vOut]).begin());
        benefit += edge.get_benefit() - edge.get_cost();
        vOut = edge.get_vIn();
    }
    return benefit;
}

void printSol(std::vector< vector<DEdge> > sol) {
    int vOut = 0; // source
    while (!sol[vOut].empty() && vOut < sol.size()) {
        DEdge edge = sol[vOut][0]; // First element
        // std::cout << vOut + 1 << " - ";
        std::cout << edge << std::endl;
        sol[vOut].erase(sol[vOut].begin());
        vOut = edge.get_vIn();
    }
    // std::cout << vOut + 1 << std::endl;
    std::cout << std::endl;
}
void agregar_lado(int v, DEdge e, std::vector< vector<DEdge> >& solParcial){
    solParcial[v].push_back(e);
}

DEdge eliminar_ultimo_lado(std::vector< vector<DEdge> > sol, std::vector< vector<DEdge> >& solParcial) {
    int vOut = 0; // source
    int vToDelete = 0;
    int be = 0;
    int ce = 0;
    while (!sol[vOut].empty() && vOut < sol.size()) {
        DEdge edge = sol[vOut][0]; // First element
        sol[vOut].erase(sol[vOut].begin());
        be = edge.get_benefit();
        ce = edge.get_cost();
        vToDelete = vOut;
        vOut = edge.get_vIn();
    }
    std::cout << "edge to delete"<< vToDelete << vOut << std::endl;
    if (solParcial[vToDelete].empty()){
        solParcial[vToDelete].erase(solParcial[vToDelete].begin() + vOut);
    }
    DEdge e(vOut, ce, be);
    return e;
}

bool ciclo_negativo(int v, DEdge e, std::vector< vector<DEdge> > solParcial) {
    // inserte su codigo aqui
    return false;
}

// func esta-lado-en-sol-parcial(e: arista, solParcial: Secuencia de aristas)
//     begin
//     if la arista e no se encuentra en solParcial then
//         return Falso
//     else if la arista e se encuentra una vez en solParcial then
//         if be = 0 then // La arista se recorre por segunda vez
//             return Falso
//         else
//             return Verdadero
//     else
//         return Verdadero

bool esta_lado_en_sol_parcial(int v, DEdge e, std::vector< vector<DEdge> > solParcial) {

    for(std::vector<DEdge>::iterator edge = solParcial[v].begin(); edge != solParcial[v].end(); ++edge) {
        // Edge was found
        if (e == *edge) {
            std::cout << "esta lado " << v+1 << " - " << e << std::endl;
            return true;
        }
    }

    for(std::vector<DEdge>::iterator edge = solParcial[e.get_vIn()].begin(); edge != solParcial[e.get_vIn()].end(); ++edge) {
        // Edge was found
        if (edge -> get_vIn() == v && edge -> get_benefit() == e.get_benefit()) {
            // if (e.get_benefit() == 0) {
            //     std::cout << "beneficio 0" << std::endl;
            //     return false;
            // }
            // else {
                std::cout << "esta lado " << v+1 << " - " << e << std::endl;
                return true;
            // }
        }
    }
    std::cout << "noooooooooo" <<  v+1 << " - " << e.get_vIn() + 1 << std::endl;
    return false;
}

bool repite_ciclo(int v, DEdge e, std::vector< vector<DEdge> > solParcial) {
    // inserte su codigo aqui
    return false;
}

// func cumple-acotamiento(e: arista, solParcial: Secuencia de aristas)
//     begin
//     beneficioE ← be − ce
//     beneficioSolParcial ← beneficio(solParcial) + beneficioE
//     maxBeneficio ← beneficioDisponible − max(0, beneficioE) + beneficioSolParcial
//     if (maxBeneficio ≤ beneficio(mejorSol)) then return Falso
//     return Verdadero
bool cumple_acotamiento(int v, DEdge e, std::vector< vector<DEdge> > solParcial) {
    int beneficioE = e.get_benefit() - e.get_cost();
    int beneficioSolParcial = benefit(solParcial) + beneficioE;
    int maxBeneficio = beneficioDisponible - max(0, beneficioE) + beneficioSolParcial;
    if (maxBeneficio <= benefit(mejorSol)){
        std::cout << " no cumple_acotamiento" << std::endl;
        return false;
    }
    std::cout << "cumple_acotamiento" << std::endl;
    return true;
}

void DFS(int v, DGraph& grafo) {
    std::cout << "benefit " << benefit(solParcial) << " " << benefit(mejorSol) << std::endl;
    std::cout << "v" << v << std::endl;
    if (v == 0 && (benefit(solParcial) > benefit(mejorSol))) {
        mejorSol = solParcial; //clone
    }
    std::vector<DEdge> L = grafo.get_successor_list(v);
    for (int i = 0; i < L.size(); i++) {
        DEdge e = L[i];
        int vj = e.get_vIn();
        int be = e.get_benefit();
        int ce = e.get_cost();
        if (!ciclo_negativo(v, e, solParcial) &&
            !esta_lado_en_sol_parcial(v, e, solParcial) &&
            !repite_ciclo(v, e, solParcial) &&
            cumple_acotamiento(v, e, solParcial)
            ) {
                std::cout << "agregando lado.." << v+1 << e << std::endl;
                agregar_lado(v, e, solParcial);
                printSol(solParcial);
                int nrows;
                cin >> nrows;
                beneficioDisponible = beneficioDisponible - max(0, be - ce);
                std::cout << "b" << std::endl;
                DFS(vj, grafo);
            }
    }

    DEdge e = eliminar_ultimo_lado(solParcial, solParcial);
    int be = e.get_benefit();
    int ce = e.get_cost();
    beneficioDisponible = beneficioDisponible + max(0, be - ce);
}

// Input: Un grafo G = (V, E) y un ciclo solInicial soluci´on de PRPP
// Output: Un ciclo C , soluci´on ´optima de PRPP
// begin
//      solParcial ←< d > // Variable Global
//      mejorSol ←solInicial // Variable Global
//      beneficioDisponible ← obtener-max-beneficio(G) // Variable Global
//      busqueda-en-Profundidad()

// busqueda-en-Profundidad()
// begin
//      v ← vertice-mas-externo(solParcial)
//      // Vemos si se encuentra una mejor soluci´on factible
//       if (v = d) then
//          if (beneficio(solParcial) > beneficio(mejorSol)) then
//          mejorSol ← solParcial
//       L(v) ← obtener-lista-de-sucesores(v) // Lista de aristas
//       foreach e ∈ L(v) do
//           if (¬ciclo-negativo(e, solParcial) ∧
//          ¬esta-lado-en-sol-parcial(e, solParcial) ∧
//          ¬repite-ciclo(L(v), e, solParcial) ∧
//          cumple-acotamiento(e, solParcial)) then
//           agregar-lado(e, solParcial)
//           beneficioDisponible = beneficioDisponible − max(0, be − ce)
//           busqueda-en-Profundidad()
//       e = eliminar-ultimo-lado(solParcial)
//       beneficioDisponible = beneficioDisponible + max(0, be − ce)

// func obtener-lista-de-sucesores(v: v´ertice)
// begin
//     L(v) ← ∅
//     foreach arista e incidente a v do
//     L(v) ← agregar una arista e1 con beneficio be y costo ce
//     L(v) ← agregar una arista e2 con beneficio 0 y costo ce
//     Ordenar de manera descendente a L(v) seg´un su beneficio
//     return L(v)
// func ciclo-negativo(e: arista, solParcial: Secuencia de aristas)
//      begin
//      if al agregar la arista e a la soluci´on parcial solParcial se forma un ciclo con beneficio negativo then
//         return Verdadero
//      return Falso
//

// func repite-ciclo(L(v): Lista de Aristas, e: arista, solParcial: Secuencia de aristas)
//     begin
//     if al agregar e en solParcial se forma un ciclo con una arista e' then
//     if (e' ∈ solParcial ∧ ((be − ce) < (be' − ce'))) then
//         return Falso
//     else
//         return Verdadero
//     return Falso

int main(int argc, char **argv) {
    DGraph grafo = readFile2(argv[1]);
    for (int i = 0; i < grafo.get_n_vertex(); i++) {
        std::vector<DEdge> adjacent;
        solParcial.push_back(adjacent);
        mejorSol.push_back(adjacent);
    }
    DFS(0, grafo);
    printSol(mejorSol);
    std::cout << benefit(mejorSol) << std::endl;
}