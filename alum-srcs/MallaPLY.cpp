// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Implementación de la clase 'MallaPLY'
// **
// *********************************************************************


#include <cassert>
#include <aux.hpp>
#include <tuplasg.hpp>
#include <file_ply_stl.hpp>
#include "MallaPLY.hpp"

using namespace std ;

// *****************************************************************************

MallaPLY::MallaPLY(const std::string& nombre_arch) {
  ponerNombre(string("malla leída del archivo '") + nombre_arch + "'");

  vector<float> vertices;
  vector<int> caras;

  ply::read(nombre_arch.c_str(), vertices, caras);

  for (size_t i = 0; i < vertices.size(); i = i + 3) {
    tabla_cord_vert.push_back(Tupla3f(vertices[i], vertices[i+1], vertices[i+2]));
  }

  for (size_t i = 0; i < caras.size(); i = i + 3) {
    tabla_caras.push_back(Tupla3u(caras[i], caras[i + 1], caras[i + 2]));
  }

  // calcular la tabla de normales
  calcular_normales();
}

// *****************************************************************************
