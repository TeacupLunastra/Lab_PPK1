#include "Bramki_Logiczne.h"
#include <fstream>
#include <sstream>
#include <iostream>


int main(int argc, char* argv[]) {
    std::string strFile, inFile, outFile;
    bool hasU = false, hasI = false, hasO = false;

    // Sprawdzanie argumentów
    if (argc == 1) {
        std::cout << "Brakuje argumentow\n";
        std::cout << "Uzycie: program -u <plik wejsciowy ukladu> -i <plik wartosci na wejsciu> -o <plik wyjsciowy>\n";
        return 1;
    }

    // Przetwarzanie argumentów
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (i + 1 >= argc) {
            std::cerr << "Brak wartosci dla przelacznika " << arg << "\n";
            return 1;
        }
        //szukanie argumentow
        if (arg == "-u") {
            strFile = argv[i + 1];
            hasU = true;
            i++;
        }
        else if (arg == "-i") {
            inFile = argv[i + 1];
            hasI = true;
            i++;
        }
        else if (arg == "-o") {
            outFile = argv[i + 1];
            hasO = true;
            i++;
        }
        else {
            std::cerr << "Nieznany przelacznik: " << arg << "\n";
            return 1;
        }
    }

    // Sprawdzanie czy wszystkie wymagane przełączniki są obecne
    if (!hasU || !hasI || !hasO) {
        std::cerr << "Brakuje wymaganych przelacznikow\n";
        std::cout << "Wymagane przelaczniki: -u, -i, -o\n";
        return 1;
    }

    //do testowanie czy dziala
    //std::cout << "strFile: " << strFile << "\n";
    //std::cout << "inFile: " << inFile << "\n";
    //std::cout << "outFile: " << outFile << "\n";

    std::ifstream plik(strFile);
    if (!plik.is_open()) {
        std::cerr << "Nie mozna otworzyc pliku z ukladem";
    }

    std::string line;
    std::vector<int> inputNodes, outputNodes;
    std::vector<Bramka> gates;
    int maxNode = 0;

    
    std::getline(plik, line);
    //jezeli znajdzie słowo klucz "IN:" to zapisuje do wektora danych wejściowych
    if (line.find("IN:") != std::string::npos) {
        std::istringstream plyk(line.substr(4));
        int node;
        while (plyk >> node) {
            inputNodes.push_back(node);
        }
    }

    
    std::getline(plik, line);
    //to samo ale dla wyjsc
    if (line.find("OUT:") != std::string::npos) {
        std::istringstream plyk(line.substr(5));
        int node;
        while (plyk >> node) {
            outputNodes.push_back(node);
        }
    }

    //po wszystkim uznaje że teraz czas na bramki
    while (std::getline(plik, line)) {
        std::istringstream plik(line);
        Bramka gate;
        plik >> gate.typ;
        //not jest specialny bo uzywa jednego wejścia i jedne wyjscie
        if (gate.typ == "NOT") {
            plik >> gate.input1 >> gate.output;
            gate.input2 = -1; // Bramka NOT nie używa drugiego wejścia a -1 to pusta wartosc
        }
        else {
            plik >> gate.input1 >> gate.input2 >> gate.output;
        }

        gates.push_back(gate);
        maxNode = std::max({ maxNode, gate.input1, gate.input2, gate.output });
    }

    std::vector<int> nodeValues(maxNode + 1, -1);

    // Wczytanie wartości wejściowych
    std::ifstream inputFile(inFile);
    if (!inputFile.is_open()) {
        std::cerr << "Nie można otworzyć pliku z wartościami wejściowymi." << std::endl;
        return 1;
    }

    std::string line2;
    while (std::getline(inputFile, line2)) {
        std::istringstream plik(line2);
        char colon;
        int node, value;

        while (plik >> node >> colon >> value) {
            nodeValues[node] = value;
        }

        // Obliczanie wyjść
        for (int node : outputNodes) {
            while (nodeValues[node] == -1) {
                Licz_wartosci(gates, nodeValues);
            }
        }
        // Zapisanie wyniku
        Zapisz(inputNodes, outputNodes, nodeValues, outFile);

        // Resetowanie węzłów
        Reset(nodeValues);
    }
    return 0;
}
