// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Implementación de la clase 'MallaRevol'
// **
// *********************************************************************

#include "MallaRevol.hpp"
#include <aux.hpp>
#include <file_ply_stl.hpp>
#include <matrices-tr.hpp>
#include <tuplasg.hpp>

using namespace std;

// *****************************************************************************

MallaRevol::MallaRevol(const string& nombre) : MallaInd(nombre) {}

MallaRevol::MallaRevol(const std::string& nombre_arch, const unsigned nperfiles,
                       const bool crear_tapas, const bool cerrar_malla) {
  ponerNombre(
      std::string("malla por revolución del perfil en '" + nombre_arch + "'"));

  vector<float> vertices;

  ply::read_vertices(nombre_arch.c_str(), vertices);
  nvp = vertices.size() / 3;
  nper = nperfiles;

  std::vector<Tupla3f> perfil_original;

  for (size_t i = 0; i < vertices.size(); i += 3) {
    perfil_original.push_back(
        Tupla3f(vertices[i], vertices[i + 1], vertices[i + 2]));
  }

  generarMallaRevol(perfil_original, crear_tapas, cerrar_malla);

  // calcular la tabla de normales
  calcular_normales();
}

void MallaRevol::definirMallaRevol(std::vector<Tupla3f> perfil_original,
                                   const unsigned num_per,
                                   const unsigned num_vert_per,
                                   const bool crear_tapas,
                                   const bool cerrar_malla) {
  nvp = num_vert_per;
  nper = num_per;

  generarMallaRevol(perfil_original, crear_tapas, cerrar_malla);
}

void MallaRevol::generarMallaRevol(std::vector<Tupla3f> perfil_original,
                                   const bool crear_tapas,
                                   const bool cerrar_malla) {
  float angulo_giro = cerrar_malla ? 360.0 / nper : 360.0 / (nper - 1);

  for (size_t i = 0; i <= nper; i++) {
    Matriz4f giro = MAT_Rotacion(i * angulo_giro, 0, 1, 0);

    for (size_t j = 0; j < nvp; j++) {
      tabla_cord_vert.push_back(giro * perfil_original[j]);
    }
  }

  for (size_t i = 0; i < nper; i++) {
    for (size_t j = 0; j < nvp - 1; j++) {
      tabla_caras.push_back(
          Tupla3u(i * nvp + j, i * nvp + j + 1, (i + 1) * nvp + j + 1));
      tabla_caras.push_back(
          Tupla3u(i * nvp + j, (i + 1) * nvp + j, (i + 1) * nvp + j + 1));
    }
  }

  if (crear_tapas) {
    Tupla3f primer_vertice = tabla_cord_vert[0];

    Tupla3f centro_tapa_inferior = Tupla3f(0, primer_vertice[1], 0);
    tabla_cord_vert.push_back(centro_tapa_inferior);

    for (size_t i = 0; i < nper; i++) {
      tabla_caras.push_back(
          Tupla3u(i * nvp, tabla_cord_vert.size() - 1, i * nvp + nvp));
    }

    Tupla3f ultimo_vertice = tabla_cord_vert[0 + nvp - 1];

    Tupla3f centro_tapa_superior = Tupla3f(0, ultimo_vertice[1], 0);
    tabla_cord_vert.push_back(centro_tapa_superior);

    for (size_t i = 0; i < nper; i++) {
      tabla_caras.push_back(Tupla3u(i * nvp + nvp - 1,
                                    tabla_cord_vert.size() - 1,
                                    i * nvp + nvp + nvp - 1));
    }
  }
}

// *****************************************************************************

Cilindro::Cilindro(float radio, float altura, int num_per,
                   const bool crear_tapas, const bool cerrar_malla)
    : MallaRevol("revolución de un cilindro") {
  std::vector<Tupla3f> perfil_original;

  for (int i = -(altura / 2); i <= (altura / 2); i++) {
    perfil_original.push_back(Tupla3f(radio, i, 0));
  }

  definirMallaRevol(perfil_original, num_per, perfil_original.size(),
                    crear_tapas, cerrar_malla);
}

Cono::Cono(int num_per, int num_vert_per, const bool crear_tapas,
           const bool cerrar_malla)
    : MallaRevol("revolución de un cono") {
  std::vector<Tupla3f> perfil_original;

  for (size_t i = 0; i < num_vert_per; i++) {
    /* El número de vértices por perfil determina la distancia entre vértices */
    float x = 1 - (float)i / (num_vert_per - 1);
    /* y = -x + 1 */
    perfil_original.push_back(Tupla3f(x, -x + 1, 0));
  }

  definirMallaRevol(perfil_original, num_per, perfil_original.size(),
                    crear_tapas, cerrar_malla);
}

Esfera::Esfera(int num_per, int num_vert_per, const bool crear_tapas,
               const bool cerrar_malla)
    : MallaRevol("revolución de una esfera") {
  std::vector<Tupla3f> perfil_original;

  for (size_t i = 0; i <= num_vert_per; i++) {
    /* El número de vértices por perfil determina la distancia entre vértices */
    float y = 1 - (float)2 * i / (num_vert_per - 1);
    /* x = sqrt(1 - y^2) */
    perfil_original.push_back(Tupla3f(sqrt(1.0 - y * y), y, 0));
  }

  definirMallaRevol(perfil_original, num_per, perfil_original.size(),
                    crear_tapas, cerrar_malla);
}