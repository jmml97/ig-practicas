// *********************************************************************
// **
// ** Gestión de una grafo de escena (implementación)
// ** Copyright (C) 2016 Carlos Ureña
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

#include "grafo-escena.hpp"
#include "aux.hpp"
#include "matrices-tr.hpp"
#include "shaders.hpp"

using namespace std;

// *********************************************************************
// Entrada del nodo del Grafo de Escena

// ---------------------------------------------------------------------
// Constructor para entrada de tipo sub-objeto

EntradaNGE::EntradaNGE(Objeto3D* pObjeto) {
  assert(pObjeto != NULL);
  tipo = TipoEntNGE::objeto;
  objeto = pObjeto;
}
// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE(const Matriz4f& pMatriz) {
  tipo = TipoEntNGE::transformacion;
  matriz = new Matriz4f();  // matriz en el heap, puntero propietario
  *matriz = pMatriz;
}

// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE(Material* pMaterial) {
  assert(pMaterial != NULL);
  tipo = TipoEntNGE::material;
  material = pMaterial;
}

// -----------------------------------------------------------------------------
// Destructor de una entrada

EntradaNGE::~EntradaNGE() {
  /**  no fnciona debido a que se hacen copias (duplicados) de punteros
  if ( tipo == TipoEntNGE::transformacion )
  {
     assert( matriz != NULL );
     delete matriz ;
     matriz = NULL ;
  }
  * **/
}

// *****************************************************************************
// Nodo del grafo de escena: contiene una lista de entradas
// *****************************************************************************

// -----------------------------------------------------------------------------
// Visualiza usando OpenGL

void NodoGrafoEscena::visualizarGL(ContextoVis& cv) {
  // COMPLETAR: práctica 3: recorrer las entradas y visualizar el nodo
  // ........
  glMatrixMode(GL_MODELVIEW); /* Operamos sobre la modelview */
  glPushMatrix();

  for (auto entrada : entradas) {
    if (entrada.tipo == TipoEntNGE::objeto) {
      entrada.objeto->visualizarGL(cv);
    } else {
      glMatrixMode(GL_MODELVIEW);
      glMultMatrixf(*(entrada.matriz));
    }
  }
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}
// -----------------------------------------------------------------------------

NodoGrafoEscena::NodoGrafoEscena() {
  // COMPLETAR: práctica 3: inicializar un nodo vacío (sin entradas)
  // ........
}
// -----------------------------------------------------------------------------

void NodoGrafoEscena::fijarColorNodo(const Tupla3f& nuevo_color) {
  // COMPLETAR: práctica 3: asignarle un color plano al nodo, distinto del padre
  // ........
  for (auto entrada : entradas) {
    if(entrada.tipo == TipoEntNGE::objeto) {
      entrada.objeto->fijarColorNodo(nuevo_color);
    }
  }
}

// -----------------------------------------------------------------------------
// Añadir una entrada (al final).
// genérica

unsigned NodoGrafoEscena::agregar(const EntradaNGE& entrada) {
  // COMPLETAR: práctica 3: agregar la entrada al nodo, devolver índice de la
  // entrada
  // ........
  entradas.push_back(entrada);
  return entradas.size() - 1;
}
// -----------------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// objeto (copia solo puntero)

unsigned NodoGrafoEscena::agregar(Objeto3D* pObjeto) {
  return agregar(EntradaNGE(pObjeto));
}
// ---------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// matriz (copia objeto)

unsigned NodoGrafoEscena::agregar(const Matriz4f& pMatriz) {
  return agregar(EntradaNGE(pMatriz));
}
// ---------------------------------------------------------------------
// material (copia solo puntero)
unsigned NodoGrafoEscena::agregar(Material* pMaterial) {
  return agregar(EntradaNGE(pMaterial));
}

// devuelve el puntero a la matriz en la i-ésima entrada
Matriz4f* NodoGrafoEscena::leerPtrMatriz(unsigned indice) {
  // COMPLETAR: práctica 3: devolver puntero la matriz en ese índice
  //   (debe de dar error y abortar si no hay una matriz en esa entrada)
  // ........
  Matriz4f* ptr;
  try {
    ptr = entradas[indice].matriz;
  } catch(std::string e) {
    std::cout << e << std::endl;
  }

  return ptr;
}
// -----------------------------------------------------------------------------
// si 'centro_calculado' es 'false', recalcula el centro usando los centros
// de los hijos (el punto medio de la caja englobante de los centros de hijos)

void NodoGrafoEscena::calcularCentroOC() {
  // COMPLETAR: práctica 5: calcular y guardar el centro del nodo
  //    en coordenadas de objeto (hay que hacerlo recursivamente)
  //   (si el centro ya ha sido calculado, no volver a hacerlo)
  // ........
}
// -----------------------------------------------------------------------------
// método para buscar un objeto con un identificador y devolver un puntero al
// mismo

bool NodoGrafoEscena::buscarObjeto(
    const int ident_busc,       // identificador a buscar
    const Matriz4f& mmodelado,  // matriz de modelado
    Objeto3D** objeto,          // (salida) puntero al puntero al objeto
    Tupla3f& centro_wc  // (salida) centro del objeto en coordenadas del mundo
) {
  // COMPLETAR: práctica 5: buscar un sub-objeto con un identificador
  // ........
}

// *****************************************************************************
// Nodo del grafo de escena, con una lista añadida de parámetros
// *****************************************************************************

// -----------------------------------------------------------------------------
// devuelve el numero de grados de libertad
int NodoGrafoEscenaParam::numParametros() {
  return parametros.size();
}
// -----------------------------------------------------------------------------

// devuelve un puntero al i-ésimo grado de libertad
Parametro* NodoGrafoEscenaParam::leerPtrParametro(unsigned i) {
  return &parametros[i];
}
// -----------------------------------------------------------------------------

void NodoGrafoEscenaParam::siguienteCuadro() {
  for (auto &p : parametros) {
    p.siguiente_cuadro();
  }
}

// *********************************************************************
// Base del Brazo Mecánico

Base::Base() {
  agregar(MAT_Escalado(2, 0.1, 2));
  agregar(new Cilindro);
}

// *********************************************************************
// Articulacion del Brazo del Brazo Mecánico

Articulacion::Articulacion() {
  agregar(MAT_Rotacion(0, 0, 0, 1));
  agregar(MAT_Escalado(0.4, 0.4, 0.4));
  agregar(new Esfera);
  fijarColorNodo({0.521, 0.521, 0.521});
}

// *********************************************************************
// Pinza de la Cabeza del Brazo Mecánico

Pinza::Pinza() {
  indice_rotacion_pinza = agregar(MAT_Rotacion(0, 0, 1, 0));
  agregar(MAT_Escalado(0.08, 0.6, 0.1));
  agregar(new Cilindro);
  fijarColorNodo({0.188, 0.188, 0.188});
}

Matriz4f* Pinza::matrizPinza() {
  return leerPtrMatriz(indice_rotacion_pinza);
}

// *********************************************************************
// Cabeza del Brazo Mecánico

Cabeza::Cabeza() {
  agregar(new Cilindro(0.5, 0.1, 20, true, true));
  fijarColorNodo({0.737, 0.219, 0.219});
  indice_pinza1 = agregar(MAT_Traslacion(0.3, 0, 0));
  pinza1 = new Pinza;
  agregar(pinza1);
  pinza2 = new Pinza;
  indice_pinza2 = agregar(MAT_Traslacion(-0.6, 0, 0));
  agregar(pinza2);
}

Matriz4f* Cabeza::matrizPinza(int i) {
  if (i == 1) {
    return pinza1->matrizPinza();
  } else {
    return pinza2->matrizPinza();
  }
}

// *********************************************************************
// Brazo Mecánico

BrazoMecanico::BrazoMecanico() {
  agregar(new Base);
  agregar(MAT_Traslacion(0, 0.1, 0));

  unsigned indice_rotacion_brazo = agregar(MAT_Rotacion(0, 0, 1, 0));
  agregar(new Cilindro(0.2, 0.5, 100, true, true));
  agregar(MAT_Traslacion(0, 0.8, 0));
  unsigned indice_rotacion_art1 = agregar(MAT_Rotacion(0, 1, 0, 0));
  agregar(new Articulacion);
  agregar(MAT_Traslacion(0, 0.3, 0));
  agregar(new Cilindro(0.2, 1, 100, true, true));
  agregar(MAT_Traslacion(0, 1.3, 0));
  unsigned indice_rotacion_art2 = agregar(MAT_Rotacion(0, 1, 0, 0));
  agregar(new Articulacion);
  agregar(MAT_Traslacion(0, 0.3, 0));
  agregar(new Cilindro(0.2, 1, 100, true, true));
  agregar(MAT_Traslacion(0, 1.3, 0));
  unsigned indice_rotacion_art3 = agregar(MAT_Rotacion(0, 1, 0, 0));
  agregar(new Articulacion);
  agregar(MAT_Traslacion(0, 0.3, 0));
  agregar(new Cilindro(0.2, 0.3, 100, true, true));

  agregar(MAT_Traslacion(0, 0.3, 0));
  unsigned indice_rotacion_cabeza = agregar(MAT_Rotacion(0, 0, 1, 0));
  auto cabeza = new Cabeza;
  agregar(cabeza);

  Parametro rotacion_brazo(
    "rotación del brazo",
    leerPtrMatriz(indice_rotacion_brazo),
    [=](float v) {return MAT_Rotacion(v, 0, 1, 0);},
    true,
    0,
    360,
    0.0005
  );
  parametros.push_back(rotacion_brazo);

  Parametro rotacion_art1(
    "rotación del brazo",
    leerPtrMatriz(indice_rotacion_art1),
    [=](float v) {return MAT_Rotacion(v, 1, 0, 0);},
    true,
    0,   /* c: Valor inicial (central para grados acotados)       */
    60,  /* s: Semiamplitud (si acotado) o factor de escala si no */
    0.001    /* f: Si acotado, frecuencia                             */
  );
  parametros.push_back(rotacion_art1);

  Parametro rotacion_art2(
    "rotación del brazo",
    leerPtrMatriz(indice_rotacion_art2),
    [=](float v) {return MAT_Rotacion(v, 1, 0, 0);},
    true,
    0,
    45,
    0.001
  );
  parametros.push_back(rotacion_art2);

  Parametro rotacion_art3(
    "rotación del brazo",
    leerPtrMatriz(indice_rotacion_art3),
    [=](float v) {return MAT_Rotacion(v, 1, 0, 0);},
    true,
    0,
    30,
    0.001
  );
  parametros.push_back(rotacion_art3);

  Parametro rotacion_cabeza(
    "rotación de la cabeza",
    leerPtrMatriz(indice_rotacion_cabeza),
    [=](float v) {return MAT_Rotacion(v, 0, 1, 0);},
    true,
    0,
    180,
    0.001
  );
  parametros.push_back(rotacion_cabeza);

  Parametro rotacion_pinza1(
    "rotación de las pinzas",
    cabeza->matrizPinza(1),
    [=](float v) {return MAT_Rotacion(v, 0, 0, 1);},
    true,
    5,
    -5,
    0.01
  );
  parametros.push_back(rotacion_pinza1);
  
  Parametro rotacion_pinza2(
    "rotación de las pinzas",
    cabeza->matrizPinza(2),
    [=](float v) {return MAT_Rotacion(v, 0, 0, 1);},
    true,
    -5,
    5,
    0.01
  );
  parametros.push_back(rotacion_pinza2);
}
