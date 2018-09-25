// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#include "MallaInd.hpp"  // declaración de 'ContextoVis'
#include <aux.hpp>
#include <tuplasg.hpp>

// *****************************************************************************
// funciones auxiliares

// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd() {
  // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
  ponerNombre("malla indexada, anónima");
}
// -----------------------------------------------------------------------------

MallaInd::MallaInd(const std::string& nombreIni) {
  // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
  ponerNombre(nombreIni);
}
// -----------------------------------------------------------------------------
// calcula las dos tablas de normales

void MallaInd::calcular_normales() {
  // COMPLETAR: en la práctica 2: calculo de las normales de la malla
  // .......
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_MI(ContextoVis& cv) {
  // COMPLETAR: en la práctica 1: visualizar en modo inmediato (glDrawElements)
  // ...........

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, tabla_cord_vert.data());
  glDrawElements(GL_TRIANGLES, tabla_caras.size() * 3, GL_UNSIGNED_INT,
                 tabla_caras.data());
  glDisableClientState(GL_VERTEX_ARRAY);
}

// ----------------------------------------------------------------------------
void MallaInd::visualizarDE_VBOs(ContextoVis& cv) {
  static GLuint id_vbo = 0;

  if (id_vbo == 0) {
    glGenBuffers(1, &id_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, id_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * tabla_cord_vert.size(),
                 tabla_cord_vert.data(), GL_STATIC_DRAW);
  } else {
    glBindBuffer(GL_ARRAY_BUFFER, id_vbo);
  }

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, 0);
  glDrawArrays(GL_TRIANGLES, 0, 3 * tabla_cord_vert.size());
  glDisableClientState(GL_VERTEX_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarGL(ContextoVis& cv) {
  // COMPLETAR: práctica 1: visualizar en modo inmediato o en modo diferido
  // (VBOs), (tener en cuenta el modo de visualización en 'cv' (alambre, sólido,
  // etc..))
  //
  // .............

  switch (cv.modoVis) {
    case modoPuntos:
      glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
      break;
    case modoAlambre:
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      break;
    case modoSolido:
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      break;
    default:
      break;
  }

  if (cv.usarVBOs) {
    visualizarDE_VBOs(cv);
  } else {
    visualizarDE_MI(cv);
  }
}
// *****************************************************************************

// *****************************************************************************

Cubo::Cubo() : MallaInd("malla cubo") {
  tabla_cord_vert = {{0.0, 0.0, 0.0},  {1.0, 0.0, 0.0}, {1.0, 0.0, 1.0},
                     {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {1.0, 1.0, 0.0},
                     {1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}};
  tabla_caras = {{0, 4, 5}, {0, 1, 5}, {1, 5, 6}, {1, 2, 6},
                 {2, 6, 7}, {2, 3, 7}, {3, 7, 4}, {3, 0, 4},
                 {4, 5, 6}, {4, 7, 6}, {0, 1, 2}, {0, 5, 2}};
}
// *****************************************************************************

Tetraedro::Tetraedro() : MallaInd("malla tetraedro") {
  tabla_cord_vert = {{0.0, 0.0, 0.0},  {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0},
                     {0.0, 1.0, 1.0}};
  tabla_caras = {{0, 1, 2}, {0, 1, 3}, {0, 2, 3}, {1, 2, 3}};
}
// *****************************************************************************
