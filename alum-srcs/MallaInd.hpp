// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#ifndef IG_MALLAIND_HPP
#define IG_MALLAIND_HPP

#include <vector>  // usar std::vector

#include "Objeto3D.hpp"  // declaración de 'Objeto3D'

Tupla3f calcularCentroCE(const std::vector<Tupla3f>& p_vertices) {};

// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaInd : public Objeto3D {
 protected:
  std::vector<Tupla3f> coordenadas_vertices;
  std::vector<Tupla3u> caras;
  std::vector<Tupla3f> colores;

  std::vector<Tupla3f> normales_vertices;
  std::vector<Tupla3f> normales_caras;

  std::vector<Tupla2f> coordenadas_texturas;

  GLuint id_VBO_vert, id_VBO_caras, id_VBO_colores, id_VBO_normales,
      id_VBO_texturas;

  bool usar_texturas;

  // calculo de las normales de esta malla
  void calcular_normales();

  // Inicializar VBOs
  void inicializarVBOs();
  // visualizar con 'draw elements', en modo inmediato
  void visualizarDE_MI(ContextoVis& cv);
  // visualizar con 'draw elements', en modo diferido (con VBOS)
  void visualizarDE_VBOs(ContextoVis& cv);
  // Visualizar con glBegin/glEnd en modo inmediato
  // Es la única forma de poder asignar una normal (o cualquier otro atributo) 
  // a un triángulo
  void visualizarDE_MISombreadoPlano(ContextoVis & cv);

 public:
  // crea una malla vacía (nombre: "malla indexada nueva vacía")
  MallaInd();
  // crea una malla vacía con un nombre concreto:
  MallaInd(const std::string& nombreIni);
  // visualizar el objeto con OpenGL
  virtual void visualizarGL(ContextoVis& cv);

  void fijarColorNodo(const Tupla3f& color);

  // Si 'centro_calculado' es 'false', recalcula el centro del objeto, el
  // punto medio de la caja englobante.
  virtual void calcularCentroOC();
};
// ---------------------------------------------------------------------

class Cubo : public MallaInd {
 public:
  Cubo();
};
// ---------------------------------------------------------------------

class Tetraedro : public MallaInd {
 public:
  Tetraedro();
};

#endif
