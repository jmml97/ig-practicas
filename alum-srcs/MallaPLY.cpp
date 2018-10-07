// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Implementación de la clase 'MallaPLY'
// **
// *********************************************************************

#include "MallaPLY.hpp"
#include <aux.hpp>
#include <cassert>
#include <file_ply_stl.hpp>
#include <tuplasg.hpp>

using namespace std;

// *****************************************************************************

/** MallaPLY::MallaPLY() ******************************************************/
/** Genera un objeto centrado en el origen a partir de la lectura de los     **/
/** vértices especificados en el archivo                                     **/
/******************************************************************************/
MallaPLY::MallaPLY(const std::string& nombre_arch) {
  ponerNombre(string("malla leída del archivo '") + nombre_arch + "'");

  vector<float> vertices;
  vector<int> caras;

  ply::read(nombre_arch.c_str(), vertices, caras);

  /* Por cada 3 números en el archivo, creamos un vértice */
  for (size_t i = 0; i < vertices.size(); i = i + 3) {
    tabla_cord_vert.push_back(
        Tupla3f(vertices[i], vertices[i + 1], vertices[i + 2]));
  }

  /* Por cada tres índices en el archivo, creamos una cara */
  for (size_t i = 0; i < caras.size(); i = i + 3) {
    tabla_caras.push_back(Tupla3u(caras[i], caras[i + 1], caras[i + 2]));
  }

  // calcular la tabla de normales
  calcular_normales();
}

// *****************************************************************************
