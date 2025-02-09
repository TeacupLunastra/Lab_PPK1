#include "Bramki_Logiczne.h"
#include <fstream>
#include <iostream>

// Funkcje logiczne dziala na podstawie tego ze 0=false a 1=true w cpp
bool Not(bool in) {
    return !in;
}
bool And(bool in1, bool in2) {
    return in1 && in2;
}
bool Or(bool in1, bool in2) {
    return in1 || in2;
}
bool Nand(bool in1, bool in2) {
    return !And(in1, in2);
}
bool Nor(bool in1, bool in2) {
    return !Or(in1, in2);
}
bool Xor(bool in1, bool in2) {
    return in1 != in2;
}
bool Xnor(bool in1, bool in2) {
    return !Xor(in1, in2);
}
// Obliczenie wartoœci dla bramki na podstawie typu
bool Wartosc_na_bramce(Bramka calc, bool var1, bool var2) {
    if (calc.typ == "AND") {
        return And(var1, var2);
    }
    else if (calc.typ == "NAND") {
        return Nand(var1, var2);
    }
    else if (calc.typ == "OR") {
        return Or(var1, var2);
    }
    else if (calc.typ == "NOR") {
        return Nor(var1, var2);
    }
    else if (calc.typ == "XOR") {
        return Xor(var1, var2);
    }
    else if (calc.typ == "XNOR") {
        return Xnor(var1, var2);
    }
    else if (calc.typ == "NOT") {
        return Not(var1);
    }
    return false;
}

// Liczenie wartoœci na wêz³ach ogarnac
void Licz_wartosci(std::vector<Bramka> gates, std::vector<int>& nodeValues) {
    for (const auto& gate : gates) {
        if (gate.typ == "NOT") {//sprawdzanie czy to bramka NOT
            if (nodeValues[gate.input1] != -1) {//sprawdzenie czy wartosc jest obliczona na bramce
                nodeValues[gate.output] = Wartosc_na_bramce(gate, nodeValues[gate.input1], 0);//liczenie wartosci tylko z jednym wejœciem
            }
        }
        else {
            if (nodeValues[gate.input1] != -1 && nodeValues[gate.input2] != -1) {
                nodeValues[gate.output] = Wartosc_na_bramce(gate, nodeValues[gate.input1], nodeValues[gate.input2]);//Liczenie ale z dwoma wejsciami
            }
        }
    }
}

// Resetowanie wartoœci wêz³ów
void Reset(std::vector<int>& nodeValues) {
    for (int& value : nodeValues) {
        value = -1;
    }
}

// Zapisanie wyniku do pliku ogarnij
void Zapisz(const std::vector<int>& inputNodes, const std::vector<int>& outputNodes,
    const std::vector<int>& nodeValues, const std::string& filename) {
    std::ofstream outFile(filename,std::ios::app);
    if (!outFile.is_open()) {
        std::cerr << "Nie mo¿na otworzyæ pliku do zapisu wyniku." << std::endl;
        return;
    }

    // Wypisanie wartoœci wejœciowych
    outFile << "IN: ";
    for (int node : inputNodes) {
        outFile << node << ":" << nodeValues[node] << " ";
    }

    // Wypisanie wartoœci wyjœciowych
    outFile << "OUT: ";
    for (int node : outputNodes) {
        outFile << node << ":" << nodeValues[node] << " ";
    }

    outFile << std::endl;//czysto kosmetyczna
}
