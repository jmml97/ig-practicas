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
// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaInd : public Objeto3D {
 protected:
  std::vector<Tupla3f> tabla_cord_vert;
  std::vector<Tupla3u> tabla_caras;
  std::vector<Tupla3f> tabla_colores;

  // ID de los VBO de vertices y caras
  GLuint id_VBO_vert, id_VBO_caras, id_VBO_colores;

  // calculo de las normales de esta malla
  void calcular_normales();

  // Inicializar VBOs
  void inicializarVBOs();
  // visualizar con 'draw elements', en modo inmediato
  void visualizarDE_MI(ContextoVis& cv);
  // visualizar con 'draw elements', en modo diferido (con VBOS)
  void visualizarDE_VBOs(ContextoVis& cv);

 public:
  // crea una malla vacía (nombre: "malla indexada nueva vacía")
  MallaInd();
  // crea una malla vacía con un nombre concreto:
  MallaInd(const std::string& nombreIni);
  // visualizar el objeto con OpenGL
  virtual void visualizarGL(ContextoVis& cv);

  void fijarColorNodo(const Tupla3f& color);
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
