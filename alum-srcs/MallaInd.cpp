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
  /* void glEnableClientState(GLenum cap)                                     */
  /* Permite activar las funcionalidades 'cap' en el cliente.                 */
  /* GL_VERTEX_ARRAY: Si está activada, el array de vértices está activado    */
  /* para la escritura y usado durante el renderizado.                        */
  glEnableClientState(GL_VERTEX_ARRAY);

  /* void glVertexPointer(GLint size, GLenum type, GLsizei stride,            */
  /*                      const GLvoid* pointer)                              */
  /* Define la localización y el formato de los datos de un array de          */
  /* coordenadas vértices a usar cuando se renderice. 'size' especifica el    */
  /* número de coordenadas por vértice, 'type', el tipo de cada coordenada, y */
  /* 'stride', el incremento de bytes en el array de un vértice a otro        */
  glVertexPointer(3, GL_FLOAT, 0, tabla_cord_vert.data());

  /* void glDrawArrays(GLenum mode, GLint first, GLsizei count)               */
  /* Construye una secuencia de primitivas geométricas utilizando elementos   */
  /* de array, empezando en 'first' y terminando en 'count' + 1. 'mode'       */
  /* especifica qué tipo de primitivas se van a usar.                         */
  glDrawElements(GL_TRIANGLES, tabla_caras.size() * 3, GL_UNSIGNED_INT,
                 tabla_caras.data());

  /* void glEnableClientState(GLenum cap)                                     */
  /* Permite desactivar las funcionalidades 'cap' en el cliente.              */
  glDisableClientState(GL_VERTEX_ARRAY);
}

// ----------------------------------------------------------------------------
void MallaInd::visualizarDE_VBOs(ContextoVis& cv) {
  static GLuint id_buffer_vert, id_buffer_caras;

  /* Primero vamos a crear el buffer object para los vértices                 */
  if (id_buffer_vert == 0) {
    /* void glGenBuffers(GLsizei n, GLuint * buffers)                         */
    /* Devuelve 'n' nombres no utilizados para identificar buffer objects y   */
    /* los almacena en el array 'buffers'.                                    */
    glGenBuffers(1, &id_buffer_vert);

    /* void glBindBuffers(GLenum target*, GLuint buffer)                      */
    /* Enlaza el buffer object llamado 'buffer' al punto de enlace 'target'.  */
    /* Si es la primera vez que se enlaza el buffer con dicho nombre, se      */
    /* crea un buffer object con ese nombre.                                  */
    glBindBuffer(GL_ARRAY_BUFFER, id_buffer_vert);

    /* glBufferData(GLenum target, GLsizeiptr size, const GLvoid* data,       */
    /*              GLenum usage)                                             */
    /* Reserva 'size' bytes de memoria para el objeto enlazado con 'target'.  */
    /* Si 'data' no es NULL, copia esa información a dicho espacio. 'usage'   */
    /* se utiliza para indicar al programa cuál es el uso que se le va a dar  */
    /* a los datos.                                                           */
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tabla_cord_vert.size() * 3,
                 tabla_cord_vert.data(), GL_STATIC_DRAW);
  } else {
    glBindBuffer(GL_ARRAY_BUFFER, id_buffer_vert);
  }

  /* Ahora vamos a crear el buffer object para las caras siguiendo el mismo   */
  /* proceso.                                                                 */
  if (id_buffer_caras == 0) {
    glGenBuffers(1, &id_buffer_caras);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_buffer_caras);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(unsigned) * 3 * tabla_caras.size(), tabla_caras.data(),
                 GL_STATIC_DRAW);
  } else {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_buffer_caras);
  }

  glEnableClientState(GL_VERTEX_ARRAY);

  /* El puntero relativo al VBO es NULL. */
  glVertexPointer(3, GL_FLOAT, 0, NULL);
  /* De nuevo, el puntero relativo al VBO es NULL. */
  glDrawElements(GL_TRIANGLES, tabla_caras.size() * 3, GL_UNSIGNED_INT, NULL);

  glDisableClientState(GL_VERTEX_ARRAY);
  /* Deshacemos los enlaces a los buffer objects. */
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarGL(ContextoVis& cv) {
  /* Cambiamos el modo de visualización según lo indicado en 'cv' */
  switch (cv.modoVis) {
    case modoPuntos:
      glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
      break;
    case modoSolido:
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      break;
    case modoAlambre:
    default:
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      break;
  }

  /* Seleccionamos la función a utilizar según vayamos a usar VBOs o no. */
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
