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

  vector<float> datos_vertices;
  vector<int> datos_caras;

  ply::read(nombre_arch.c_str(), datos_vertices, datos_caras);

  /* Por cada 3 números en el archivo, creamos un vértice */
  for (size_t i = 0; i < datos_vertices.size(); i = i + 3) {
    coordenadas_vertices.push_back(Tupla3f(
        datos_vertices[i], datos_vertices[i + 1], datos_vertices[i + 2]));
  }

  /* Por cada tres índices en el archivo, creamos una cara */
  for (size_t i = 0; i < datos_caras.size(); i = i + 3) {
    caras.push_back(
        Tupla3u(datos_caras[i], datos_caras[i + 1], datos_caras[i + 2]));
  }

  // calcular la tabla de normales
  calcular_normales();
}

// *****************************************************************************
