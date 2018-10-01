// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Implementación de la clase 'MallaRevol'
// **
// *********************************************************************

#include <aux.hpp>
#include <tuplasg.hpp>
#include <file_ply_stl.hpp>
#include <matrices-tr.hpp>
#include "MallaRevol.hpp"

using namespace std ;

// *****************************************************************************

MallaRevol::MallaRevol(const std::string& nombre_arch, const unsigned nperfiles,
                       const bool crear_tapas, const bool cerrar_malla) {
  ponerNombre(
      std::string("malla por revolución del perfil en '" + nombre_arch + "'"));

  vector<float> vertices;

  ply::read_vertices(nombre_arch.c_str(), vertices);
  nvp = vertices.size()/3;

  if (cerrar_malla) {
    nper = nperfiles;
  } else {
    nper = nperfiles - 1;
  }

  std::cout << "nper:" << nper << '\n';

  for (size_t i = 0; i <= nper; i++) {
    Matriz4f giro = MAT_Rotacion(i * 360 / nper, 0, 1, 0);
    std::cout << "i: " << i << '\n';

    for (size_t j = 0; j < vertices.size(); j = j + 3) {
      tabla_cord_vert.push_back(
          giro * Tupla3f(vertices[j], vertices[j + 1], vertices[j + 2]));
    }
  }

  std::cout << "tabla_cord_vert.size(): " << tabla_cord_vert.size() << '\n';

  for (size_t i = 0; i < nper; i++) {
    for (size_t j = 0; j < nvp - 1; j++) {
      tabla_caras.push_back(Tupla3u(i*nvp + j, i*nvp + j +1, (i+1)*nvp + j + 1));
      tabla_caras.push_back(Tupla3u(i*nvp + j, (i+1)*nvp + j, (i+1)*nvp + j + 1));
    }
  }
  
  // calcular la tabla de normales
  calcular_normales();
}

// *****************************************************************************
