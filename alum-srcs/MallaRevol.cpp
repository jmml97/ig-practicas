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

/** MallaRevol::MallaRevol() ***********************************************/
/** Lee los vértices del archivo especificado y los almacena en el vector **/
/** correspondiente, actualizando las variables nvp y nper                **/
/***************************************************************************/
MallaRevol::MallaRevol(const std::string& nombre_arch, const unsigned nperfiles,
                       const bool crear_tapas, const bool cerrar_malla) {
  ponerNombre(
      std::string("malla por revolución del perfil en '" + nombre_arch + "'"));

  vector<float> vertices;

  /* El archivo a leer contiene únicamente la información de los vértices del */
  /* perfil original                                                          */
  ply::read_vertices(nombre_arch.c_str(), vertices);
  nvp = vertices.size() / 3;
  nper = nperfiles;

  /* Recibimos los vértices como una lista de 3-tuplas de floats y queremos */
  /* convertir esta información a un vector<Tupla3f>, que es como           */
  /* almacenamos la información de los vértices                             */
  std::vector<Tupla3f> perfil_original;

  /* Para cada 3-tupla de floats, creamos un vértice */
  for (size_t i = 0; i < vertices.size(); i += 3) {
    perfil_original.push_back(
        Tupla3f(vertices[i], vertices[i + 1], vertices[i + 2]));
  }

  /* La malla de triángulos para las caras las creamos en otra función */
  generarMallaRevol(perfil_original, crear_tapas, cerrar_malla);

  // calcular la tabla de normales
  calcular_normales();
}

/** MallaRevol::definirMallaRevol() *******************************************/
/** Nos permite actualizar los valores nvp y nper, privados, desde clases    **/
/** derivadas de esta                                                        **/
/******************************************************************************/
void MallaRevol::definirMallaRevol(std::vector<Tupla3f> perfil_original,
                                   const unsigned num_per,
                                   const unsigned num_vert_per,
                                   const bool crear_tapas,
                                   const bool cerrar_malla) {
  nvp = num_vert_per;
  nper = num_per;

  generarMallaRevol(perfil_original, crear_tapas, cerrar_malla);
}

/** MallaRevol::generarMallaRevol() *******************************************/
/** Genera las mallas de triángulos que forman las caras a partir del perfil **/
/** original que se le proporcione                                           **/
/******************************************************************************/
void MallaRevol::generarMallaRevol(std::vector<Tupla3f> perfil_original,
                                   const bool crear_tapas,
                                   const bool cerrar_malla) {
  /* Cerrar la malla es evitar que se superponga el último perfil al primero */
  float angulo_giro = cerrar_malla ? 360.0 / nper : 360.0 / (nper - 1);

  /* Insertamos los vértices del perfil original */
  for (size_t i = 0; i < nvp; i++) {
    tabla_cord_vert.push_back(perfil_original[i]);
  }

  /* Para el resto de perfiles, rotamos el perfil y añadimos los vértices  */
  /* También unimos los vértices de perfiles consecutivos para crear caras */
  for (size_t i = 0; i < nper; i++) {
    Matriz4f giro = MAT_Rotacion(i * angulo_giro, 0, 1, 0);

    for (size_t j = 0; j < nvp; j++) {
      tabla_cord_vert.push_back(giro * perfil_original[j]);
    }

    /* Unimos los vértices del perfil actual con los del perfil anterior */
    for (size_t j = 0; j < nvp - 1; j++) {
      int k_1 = (i - 1) * nvp + j;
      int k_2 = (i - 1) * nvp + j + 1;
      int k_3 = i * nvp + j;
      int k_4 = i * nvp + j + 1;

      tabla_caras.push_back(
          Tupla3u(k_1, k_2, k_4));
      tabla_caras.push_back(
          Tupla3u(k_1, k_3, k_4));
    }
  }

  /* Si se cierra la malla, unimos los vértices del último perfil con los de */
  /* el perfil inicial                                                       */
  if (cerrar_malla) {
    for (size_t j = 0; j < nvp - 1; j++) {
      tabla_caras.push_back(
          Tupla3u((nper - 1) * nvp + j, (nper - 1) * nvp + j + 1, j + 1));
      tabla_caras.push_back(Tupla3u((nper - 1) * nvp + j, j, j + 1));
    }
  }

  if (crear_tapas) {
    /* Obtenemos el primer vértice del perfil original */
    Tupla3f primer_vertice = tabla_cord_vert[0];

    /* Centramos el vértice en el eje de giro */
    Tupla3f centro_tapa_inferior = Tupla3f(0, primer_vertice[1], 0);

    /* Añadimos el vértice a la tabla y generamos las caras uniendo los */
    /* primeros vértices de cada perfil con él                          */
    tabla_cord_vert.push_back(centro_tapa_inferior);
    for (size_t i = 0; i < nper; i++) {
      tabla_caras.push_back(
          Tupla3u(i * nvp, tabla_cord_vert.size() - 1, (i * nvp + nvp) % (tabla_cord_vert.size() - 1)));
    }

    /* Obtenemos el último vértice del perfil original */
    Tupla3f ultimo_vertice = tabla_cord_vert[tabla_cord_vert.size() - 2];

    /* Centramos el vértice en el eje de giro */
    Tupla3f centro_tapa_superior = Tupla3f(0, ultimo_vertice[1], 0);

    /* Añadimos el vértice a la tabla y generamos las caras uniendo los */
    /* primeros vértices de cada perfil con él                          */
    tabla_cord_vert.push_back(centro_tapa_superior);
    for (size_t i = 0; i < nper; i++) {
      tabla_caras.push_back(Tupla3u(i * nvp + nvp - 1,
                                    tabla_cord_vert.size() - 1,
                                    i * nvp + nvp + nvp - 1));
    }

    /* Última cara */
    tabla_caras.push_back(Tupla3u(nper * nvp - 1, tabla_cord_vert.size() - 1, nvp - 1));
  }
}

// *****************************************************************************

/** Cilindro::Cilindro() ******************************************************/
/** Genera un cilindro centrado en el origen según el radio y la altura      **/
/** especificadas.                                                           **/
/******************************************************************************/
Cilindro::Cilindro(float radio, float altura, int num_per,
                   const bool crear_tapas, const bool cerrar_malla)
    : MallaRevol("revolución de un cilindro") {
  std::vector<Tupla3f> perfil_original;

  // Construir perfil
  for (unsigned i = 0; i < num_per; i++) {
    perfil_original.push_back({radio, altura * ((float)i / (num_per - 1)), 0.0});
  }

  definirMallaRevol(perfil_original, num_per, perfil_original.size(),
                    crear_tapas, cerrar_malla);
}

/** Cono::Cono() **************************************************************/
/** Genera un cono centrado en el origen según el número de perfiles y el    **/
/** número de vértices por perfil                                            **/
/******************************************************************************/
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

/** Esfera::Esfera() **********************************************************/
/** Genera una esfera centrada en el origen según el número de perfiles y el **/
/** número de vértices por perfil                                            **/
/******************************************************************************/
Esfera::Esfera(int num_per, int num_vert_per, const bool crear_tapas,
               const bool cerrar_malla)
    : MallaRevol("revolución de una esfera") {
  std::vector<Tupla3f> perfil_original;

  for (size_t i = 0; i < num_vert_per; i++) {
    /* El número de vértices por perfil determina la distancia entre vértices */
    float y = -1 + (float)2 * i / (num_vert_per - 1);
    /* x = sqrt(1 - y^2) */
    perfil_original.push_back(Tupla3f(sqrt(1.0 - y * y), y, 0.0));
  }

  definirMallaRevol(perfil_original, num_per, perfil_original.size(),
                    crear_tapas, cerrar_malla);
}