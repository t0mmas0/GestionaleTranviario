#include <iostream>
#include "LineaFerroviaria.h"

int main() {
    LineaFerroviaria linea{ "C:\\Users\\Tommaso\\GitProjects\\GestionaleTranviario\\timetables.txt", "C:\\Users\\Tommaso\\GitProjects\\GestionaleTranviario\\line_description.txt" };
    linea.esegui();
}
