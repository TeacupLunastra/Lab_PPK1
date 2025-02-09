#pragma once

#include <string>
#include <vector>

// Struktura opisuj�ca pojedyncz� bramk� logiczn�
struct Bramka {
    std::string typ;
    int input1;
    int input2;
    int output;
};

// Funkcje logiczne
bool Not(bool in);
bool And(bool in1, bool in2);
bool Or(bool in1, bool in2);
bool Nand(bool in1, bool in2);
bool Nor(bool in1, bool in2);
bool Xor(bool in1, bool in2);
bool Xnor(bool in1, bool in2);

// Obliczenie warto�ci dla bramki na podstawie typu
bool Wartosc_na_bramce(Bramka calc, bool var1, bool var2);

// Funkcje operuj�ce na bramkach i w�z�ach
void Licz_wartosci(std::vector<Bramka> gates, std::vector<int>& nodeValues);
void Reset(std::vector<int>& nodeValues);
void Zapisz(const std::vector<int>& inputNodes, const std::vector<int>& outputNodes, const std::vector<int>& nodeValues, const std::string& filename);

