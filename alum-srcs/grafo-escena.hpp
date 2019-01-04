// *********************************************************************
// **
// ** Gestión de una grafo de escena (declaraciones)
// ** Copyright (C) 2014-2017 Carlos Ureña
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// *********************************************************************

#ifndef GRAFO_ESCENA_HPP
#define GRAFO_ESCENA_HPP

//#include <obj-gra.hpp>
//#include <materiales.hpp>

#include "Objeto3D.hpp"
#include "materiales.hpp"
#include "matrices-tr.hpp"
#include "MallaRevol.hpp"

// *********************************************************************
// declaración adelantada de estructura para un nodo del grafo de escena

class NodoGrafoEscena;

// *********************************************************************
// tipo enumerado con los tipos de entradas del nodo del grafo de escena

enum class TipoEntNGE { objeto, transformacion, material, noInicializado };

// *********************************************************************
// Entrada del nodo del Grafo de Escena

struct EntradaNGE {
  TipoEntNGE tipo;  // objeto, transformacion, material
  union {
    Objeto3D* objeto;    // ptr. a un objeto (no propietario)
    Matriz4f* matriz;    // ptr. a matriz 4x4 transf. (propietario)
    Material* material;  // ptr. a material (no propietario)
  };
  // constructores (uno por tipo)
  EntradaNGE(Objeto3D* pObjeto);        // (copia solo puntero)
  EntradaNGE(const Matriz4f& pMatriz);  // (crea copia en el heap)
  EntradaNGE(Material* pMaterial);      // (copia solo puntero)
  ~EntradaNGE();
};

// *********************************************************************
// Nodo del grafo de escena: es un objeto 3D parametrizado, que contiene una
// lista de entradas

class NodoGrafoEscena : public Objeto3D {
 protected:
  std::vector<EntradaNGE> entradas;   /* Vector de entradas */

 public:
  NodoGrafoEscena();

  // Llama de forma recursiva a las sucesivas funciones de visualización de 
  // los objetos hijos del padre, a la vez que va añadiendo las matrices de
  // transformación del grafo de escena
  virtual void visualizarGL(ContextoVis& cv);

  // Recorre los nodos hijos del grafo de escena y los cambia al color 
  // especificado
  void fijarColorNodo(const Tupla3f& nuevo_color);

  // calcular y obtener la caja englobante
  // virtual CajaEngf cajaEnglobante() ;

  // Añade una entrada al final del grafo de escena y devuelve el índice
  // de la entrada dentro del vector de entradas
  unsigned agregar(const EntradaNGE& entrada);

  // construir una entrada y añadirla (al final)
  unsigned agregar(Objeto3D* pObjeto);        // objeto (copia solo puntero)
  unsigned agregar(const Matriz4f& pMatriz);  // matriz (copia objeto)
  unsigned agregar(Material* pMaterial);      // material (copia solo puntero)

  // Devuelve el puntero a la matriz en la i-ésima entrada
  Matriz4f* leerPtrMatriz(unsigned iEnt);

  // método para buscar un objeto con un identificador
  virtual bool buscarObjeto(const int ident_busc, const Matriz4f& mmodelado,
                            Objeto3D** objeto, Tupla3f& centro_wc);

  // si 'centro_calculado' es 'false', recalcula el centro usando los centros
  // de los hijos (el punto medio de la caja englobante de los centros de hijos)
  virtual void calcularCentroOC();
};

// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos que tienen además un vector de
// parámetros

class NodoGrafoEscenaParam : public NodoGrafoEscena {
 protected:
  std::vector<Parametro> parametros;

 public:
  // Devuelve el número de parámetros
  int numParametros();

  // Devuelve un puntero al i-ésimo parámetro (i < numParametros())
  Parametro* leerPtrParametro(unsigned i);

  // Actualiza el objeto para ir al siguiente cuadro. Se usa cuando están 
  // activadas las animaciones, una vez antes de cada frame
  void siguienteCuadro();
};

// *********************************************************************
// Base del Brazo Mecánico

class Base : public NodoGrafoEscena {
  public:
   Base();
};

// *********************************************************************
// Articulaciones del Brazo del Brazo Mecánico

class Articulacion : public NodoGrafoEscena {
 public:
  Articulacion();
};

// *********************************************************************
// Pinza de la Cabeza del Brazo Mecánico

class Pinza : public NodoGrafoEscena {
  public:
   Pinza();
   unsigned indice_rotacion_pinza;
   Matriz4f* matrizPinza();
};

// *********************************************************************
// Cabeza del Brazo Mecánico

class Cabeza : public NodoGrafoEscena {
 public:
  Pinza* pinza1;
  Pinza* pinza2;

  Cabeza();
  Matriz4f* matrizPinza(int i);
  unsigned indice_pinza1, indice_pinza2;
};

// *********************************************************************
// Brazo Mecánico

class BrazoMecanico : public NodoGrafoEscenaParam {
  protected:
  unsigned indice_brazo;
  public:
   BrazoMecanico();
};

//******************************************************************************
// Clases de la práctica 4
// -----------------------
// Clases que se van a utilizar para el grafo de escena de la práctica 4

class Lata : public NodoGrafoEscena {
 public:
  Lata();
};

class Peones : public NodoGrafoEscena {
 protected:
  class PeonNegro : public NodoGrafoEscena {
   public:
    PeonNegro();
  };

  class PeonBlanco : public NodoGrafoEscena {
   public:
    PeonBlanco();
  };

  class PeonMadera : public NodoGrafoEscena {
   public:
    PeonMadera();
  };

 public:
  Peones();
};

#endif  // GRAFO_ESCENA_HPP