#include <iostream>
#include "LineaFerroviaria.h"

int main() {
    LineaFerroviaria linea{ "../../../timetables.txt", "../../../line_description.txt" };
    linea.esegui();
}
