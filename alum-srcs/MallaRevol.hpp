// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase MallaRevol
// **
// *********************************************************************

#ifndef IG_MALLAREVOL_HPP
#define IG_MALLAREVOL_HPP

#include <string>
#include <vector>  // usar std::vector

#include "MallaInd.hpp"  // declaración de 'Objeto3D'
// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaRevol : public MallaInd {
 private:
  unsigned nper,  // numero de perfiles
      nvp;        // numero de vertices por perfil

 public:
  MallaRevol(const std::string& nombre);
  // crea una malla de revolucion
  MallaRevol(const std::string& nombre_arch, const unsigned nperfiles,
             const bool crear_tapas, const bool cerrar_malla,
             const bool crear_texturas);
  MallaRevol(const std::string& nombre_arch, const unsigned nperfiles,
             const bool crear_tapas, const bool cerrar_malla)
      : MallaRevol(nombre_arch, nperfiles, crear_tapas, cerrar_malla, false){};
  void definirMallaRevol(std::vector<Tupla3f> perfil_original,
                         const unsigned num_per, const unsigned num_vert_per,
                         const bool crear_tapas, const bool cerrar_malla,
                         const bool crear_texturas = false);
  void generarMallaRevol(std::vector<Tupla3f> perfil_original,
                         const bool crear_tapas, const bool cerrar_malla);
  void definirCoordenadasTexturas();
};

#endif

class Cilindro : public MallaRevol {
 public:
  Cilindro(float radio, float altura, int num_per, const bool crear_tapas,
           const bool cerrar_malla);
  Cilindro() : Cilindro(1, 1, 20, true, true) {};
};

class Cono : public MallaRevol {
 public:
  Cono(int num_vert_per, int num_per, const bool crear_tapas,
       const bool cerrar_malla);
};

class Esfera : public MallaRevol {
 public:
  Esfera(int num_per, int num_vert_per, const bool crear_tapas,
         const bool cerrar_malla);
  Esfera() : Esfera(100, 100, true, true){};
};