// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#include "MallaInd.hpp"  // declaración de 'ContextoVis'
#include <aux.hpp>
#include <tuplasg.hpp>

#include <algorithm>  // usar std::algorithm

// *****************************************************************************
// Funciones auxiliares

// Crea un VBO del tipo especificado enlazándole la matriz correspondiente
// Los VBO (Vertex Buffer Object) son simplemente Buffer Object que almacenan
// información de los vértices de una figura en la GPU.
GLuint crearVBO(GLuint tipo, GLuint tam, GLvoid* puntero) {
  GLuint id_VBO;

  /* void glGenBuffers(GLsizei n, GLuint * buffers)                         */
  /* Devuelve 'n' nombres no utilizados para identificar buffer objects y   */
  /* los almacena en el array 'buffers'.                                    */
  /* No genera ningún tipo de contenido en el VBO                           */
  glGenBuffers(1, &id_VBO);

  /* void glBindBuffers(GLenum target*, GLuint buffer)                      */
  /* Enlaza el buffer object llamado 'buffer' al punto de enlace 'target'.  */
  /* Si es la primera vez que se enlaza el buffer con dicho nombre, se      */
  /* crea un buffer object con ese nombre.                                  */
  glBindBuffer(tipo, id_VBO);

  /* glBufferData(GLenum target, GLsizeiptr size, const GLvoid* data,       */
  /*              GLenum usage)                                             */
  /* Reserva 'size' bytes de memoria para el objeto enlazado con 'target'.  */
  /* Si 'data' no es NULL, copia esa información a dicho espacio. 'usage'   */
  /* se utiliza para indicar al programa cuál es el uso que se le va a dar  */
  /* a los datos.                                                           */
  glBufferData(tipo, tam, puntero, GL_STATIC_DRAW);

  glBindBuffer(tipo, 0);

  return id_VBO;
}

Tupla3f calcularCentroCE(const std::vector<Tupla3f>& p_vertices) {
  Tupla3f maximo = p_vertices[0],
          minimo = p_vertices[0];  // vértices diagonalmente opuestos

  for (auto vertice : p_vertices) {
    maximo = {std::max(vertice(X), maximo(X)), std::max(vertice(Y), maximo(Y)),
              std::max(vertice(Z), maximo(Z))};
    minimo = {std::min(vertice(X), minimo(X)), std::min(vertice(Y), minimo(Y)),
              std::min(vertice(Z), minimo(Z))};
  }

  Tupla3f centro = (maximo + minimo) / 2.0; 
  return centro;
}

// *****************************************************************************
// Métodos de la clase MallaInd.

MallaInd::MallaInd() : MallaInd("malla indexada, anónima") {}

// -----------------------------------------------------------------------------

MallaInd::MallaInd(const std::string& nombreIni) {
  // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
  ponerNombre(nombreIni);

  usar_texturas = false;

  id_VBO_vert = id_VBO_caras = id_VBO_colores = id_VBO_normales =
      id_VBO_texturas = 0;
}

// -----------------------------------------------------------------------------
// calcula las dos tablas de normales
void MallaInd::calcular_normales() {
  if (normales_caras.empty()) {
    // p, q, r := vértices de la cara
    // a := lado q - p
    // b := lado r - p
    Tupla3f a, b, p, q, r, n;
    normales_vertices = std::vector<Tupla3f>(coordenadas_vertices.size(),
                                             Tupla3f{0.0, 0.0, 0.0});

    for (size_t i = 0; i < caras.size(); i++) {
      p = coordenadas_vertices[caras[i](0)];
      q = coordenadas_vertices[caras[i](1)];
      r = coordenadas_vertices[caras[i](2)];

      a = q - p;
      b = r - p;

      // n es el vector perpendicular a los vectores a y b obtenido mediante el
      // producto vectorial
      n = a.cross(b);

      if (n(0) != 0 || n(1) != 0 || n(2) != 0) {
        n = n.normalized();
      }

      normales_caras.push_back(n);

      for (int j = 0; j < 2; j++) {
        normales_vertices[caras[i](j)] = (n + normales_vertices[caras[i](j)]);
        if (normales_vertices[caras[i](j)](0) != 0 ||
            normales_vertices[caras[i](j)](1) != 0 ||
            normales_vertices[caras[i](j)](2) != 0) {
          normales_vertices[caras[i](j)] =
              (normales_vertices[caras[i](j)]).normalized();
        }
      }
    }
  }
}

// -----------------------------------------------------------------------------

// Crea los VBOs correspondientes para los vértices, caras y colores
void MallaInd::inicializarVBOs() {
  if (id_VBO_vert == 0) {
    id_VBO_vert = crearVBO(GL_ARRAY_BUFFER,
                           sizeof(float) * coordenadas_vertices.size() * 3,
                           coordenadas_vertices.data());
  }

  if (id_VBO_caras == 0) {
    id_VBO_caras = crearVBO(GL_ELEMENT_ARRAY_BUFFER,
                            sizeof(unsigned) * 3 * caras.size(), caras.data());
  }

  if (colores.size() > 0) {
    id_VBO_colores = crearVBO(GL_ARRAY_BUFFER,
                              sizeof(float) * coordenadas_vertices.size() * 3,
                              colores.data());
  }

  if (normales_vertices.size() > 0) {
    id_VBO_normales = crearVBO(GL_ARRAY_BUFFER,
                               sizeof(float) * coordenadas_vertices.size() * 3,
                               normales_vertices.data());
  }

  if (coordenadas_texturas.size() > 0) {
    id_VBO_texturas = crearVBO(GL_ARRAY_BUFFER,
                               sizeof(float) * 2L * coordenadas_vertices.size(),
                               coordenadas_texturas.data());
  }
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_MI(ContextoVis& cv) {
  if (usar_texturas) {
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, normales_vertices.data());

    if (coordenadas_texturas.size() > 0) {
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glTexCoordPointer(2, GL_FLOAT, 0, coordenadas_texturas.data());
    }
  } else if (colores.size() > 0 && !cv.modoSeleccionFBO) {
    glColorPointer(3, GL_FLOAT, 0, colores.data());
    glEnableClientState(GL_COLOR_ARRAY);
  }

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
  glVertexPointer(3, GL_FLOAT, 0, coordenadas_vertices.data());

  /* void glDrawArrays(GLenum mode, GLint first, GLsizei count)               */
  /* Construye una secuencia de primitivas geométricas utilizando elementos   */
  /* de array, empezando en 'first' y terminando en 'count' + 1. 'mode'       */
  /* especifica qué tipo de primitivas se van a usar.                         */

  /* void glDrawElements(GLenum mode,                     */
  /*                     GLsizei count,                   */
  /*                     GLenum type,                     */
  /*                     const GLvoid * indices);         */
  /* Renderiza primitivas a partir de datos de un array.  */

  glDrawElements(GL_TRIANGLES, caras.size() * 3, GL_UNSIGNED_INT, caras.data());

  /* void glEnableClientState(GLenum cap)                                     */
  /* Permite desactivar las funcionalidades 'cap' en el cliente.              */
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

// ----------------------------------------------------------------------------

// Función de visualización cuando se usan VBOs
void MallaInd::visualizarDE_VBOs(ContextoVis& cv) {
  inicializarVBOs();

  if (usar_texturas) {
    /* void glBindBuffer(	GLenum  	target, */
    /*                    GLuint  	buffer);                            */
    /* Para poder modificar un objeto de OpenGL es necesario que esté   */
    /* asociado a un contexto, que especifica el comportamiento de este */
    /* objeto                                                           */
    glBindBuffer(GL_ARRAY_BUFFER, id_VBO_normales);
    glNormalPointer(GL_FLOAT, 0, 0);
    glEnableClientState(GL_NORMAL_ARRAY);

    if (coordenadas_texturas.size() > 0) {
      glBindBuffer(GL_ARRAY_BUFFER, id_VBO_texturas);
      glTexCoordPointer(2, GL_FLOAT, 0, 0);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }
  } else if (colores.size() > 0) {  // Colores (puede que el objeto no esté
                                    // coloreado)
    glBindBuffer(GL_ARRAY_BUFFER, id_VBO_colores);

    /* void glColorPointer(GLint  	size,                               */
    /*                     GLenum  	type,                               */
    /*                     GLsizei  	stride,                           */
    /*                     const GLvoid *  	pointer);                   */
    /* Especifica la ubicación el formato de los datos de un array de   */
    /* componentes de color a usar durante el renderizado	              */
    glColorPointer(3, GL_FLOAT, 0, 0);
    glEnableClientState(GL_COLOR_ARRAY);
  }

  // Vértices
  glBindBuffer(GL_ARRAY_BUFFER, id_VBO_vert);

  /* void glVertexPointer(GLint  	size,                               */
  /*                      GLenum  	type,º	                          */
  /*                      GLsizei  	stride,                           */
  /*                      const GLvoid *  	pointer);                 */
  /* Especifica la ubicación el formato de los datos de un array de   */
  /* coordenadas de vértices a usar durante el renderizado	          */
  glVertexPointer(3, GL_FLOAT, 0, NULL);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glEnableClientState(GL_VERTEX_ARRAY);

  // Caras
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_VBO_caras);
  glDrawElements(GL_TRIANGLES, caras.size() * 3, GL_UNSIGNED_INT, NULL);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_MISombreadoPlano(ContextoVis& cv) {
  glBegin(GL_TRIANGLES);

  for (unsigned i = 0; i < caras.size(); i++) {
    glNormal3fv(normales_caras[i]);
    for (unsigned j = 0; j < 3; j++) {
      unsigned indice_vertice = caras[i](j);
      if (coordenadas_texturas.size() > 0)
        glTexCoord2fv(coordenadas_texturas[indice_vertice]);
      glVertex3fv(coordenadas_vertices[indice_vertice]);
    }
  }

  glEnd();
}

// ---------------------------------------------------------------------------

void MallaInd::visualizarGL(ContextoVis& cv) {
  usar_texturas =
      cv.modoVis == modoSombreadoPlano || cv.modoVis == modoSombreadoSuave;

  if (!usar_texturas) {
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
  } else if (normales_vertices.size() == 0) {
    calcular_normales();
  }

  // Modo de selección
  if (cv.modoSeleccionFBO) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_FLAT);
    visualizarDE_MI(cv);
    return;
  }

  /* Cambiamos el modo de visualización según lo indicado en 'cv' */
  switch (cv.modoVis) {
    case modoPuntos:
      glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
      glShadeModel(GL_SMOOTH);
      break;
    case modoSolido:
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      break;
    case modoSombreadoPlano:
       glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
       glShadeModel(GL_FLAT);
       break;
     case modoSombreadoSuave:
       glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
       glShadeModel(GL_SMOOTH);
       break;
    case modoAlambre:
    default:
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      break;
  }

  // Seleccionamos la función a utilizar según vayamos a usar VBOs o no.
  if (cv.usarVBOs) {
    visualizarDE_VBOs(cv);
  } else if (cv.modoVis == modoSombreadoPlano) {
    visualizarDE_MISombreadoPlano(cv);
  } else {
    visualizarDE_MI(cv);
  }
}

void MallaInd::fijarColorNodo(const Tupla3f& color) {
  colores.clear();

  for (size_t i = 0; i < coordenadas_vertices.size(); i++) {
    colores.push_back(color);
  }
}

// ---------------------------------------------------------------------------

// Si 'centro_calculado' es 'false', recalcula el centro del objeto, el
// punto medio de la caja englobante.
void MallaInd::calcularCentroOC() {
  if (!centro_calculado) {
    ponerCentroOC(calcularCentroCE(coordenadas_vertices));
    centro_calculado = true;
  }
}

// ***************************************************************************

// ***************************************************************************

Cubo::Cubo() : MallaInd("malla cubo") {
  coordenadas_vertices = {{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 1.0},
                          {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {1.0, 1.0, 0.0},
                          {1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}};
  caras = {{0, 4, 5}, {0, 1, 5}, {1, 5, 6}, {1, 2, 6}, {2, 6, 7}, {2, 3, 7},
           {3, 7, 4}, {3, 0, 4}, {4, 5, 6}, {4, 7, 6}, {0, 1, 2}, {0, 5, 2}};
}
// *****************************************************************************

Tetraedro::Tetraedro() : MallaInd("malla tetraedro") {
  coordenadas_vertices = {
      {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 1.0, 1.0}};
  caras = {{0, 1, 2}, {0, 1, 3}, {0, 2, 3}, {1, 2, 3}};
}
// *****************************************************************************
