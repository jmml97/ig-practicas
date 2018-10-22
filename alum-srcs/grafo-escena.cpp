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
  // COMPLETAR: práctica 3: indicar cuantos parámetros hay
  // ........
}
// -----------------------------------------------------------------------------

// devuelve un puntero al i-ésimo grado de libertad
Parametro* NodoGrafoEscenaParam::leerPtrParametro(unsigned i) {
  // COMPLETAR: práctica 3: devolver puntero al i-ésimo parámetro
  // ........
}
// -----------------------------------------------------------------------------

void NodoGrafoEscenaParam::siguienteCuadro() {
  // COMPLETAR: práctica 3: actualizar todos los parámetros al siguiente cuadro
  // ........
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
  agregar(MAT_Escalado(0.4, 0.4, 0.4));
  agregar(new Esfera);
}

// *********************************************************************
// Articulaciones del Brazo del Brazo Mecánico

Articulaciones::Articulaciones() { 
  agregar(MAT_Traslacion(0, 0.8, 0));
  agregar(new Articulacion);
  agregar(MAT_Traslacion(0, 1.4, 0));
  agregar(new Articulacion);
  agregar(MAT_Traslacion(0, 1.4, 0));
  agregar(new Articulacion);
}

// *********************************************************************
// Brazo del Brazo Mecánico

Brazo::Brazo() {
  agregar(new Articulaciones);
  agregar(new Cilindro(0.2, 0.5, 100, true, true));
  agregar(MAT_Traslacion(0, 0.9, 0));
  agregar(new Cilindro(0.2, 1, 100, true, true));
  agregar(MAT_Traslacion(0, 1.4, 0));
  agregar(new Cilindro(0.2, 1, 100, true, true));
  agregar(MAT_Traslacion(0, 1.4, 0));
  agregar(new Cilindro(0.2, 0.5, 100, true, true));
}

// *********************************************************************
// Pinza de la Cabeza del Brazo Mecánico

Pinza::Pinza() {
  agregar(MAT_Escalado(0.08, 0.6, 0.1));
  agregar(new Cilindro);
}

// *********************************************************************
// Cabeza del Brazo Mecánico

Cabeza::Cabeza() {
  agregar(new Cilindro(0.5, 0.1, 20, true, true));
  agregar(MAT_Traslacion(0.3, 0, 0));
  agregar(new Pinza);
  agregar(MAT_Traslacion(-0.6, 0, 0));
  agregar(new Pinza);
}

// *********************************************************************
// Brazo Mecánico

BrazoMecanico::BrazoMecanico() {
  agregar(new Base);
  agregar(MAT_Traslacion(0, 0.1, 0));
  agregar(new Brazo);
  agregar(MAT_Traslacion(0, 4.2, 0));
  agregar(new Cabeza);

  // agregar(MAT_Traslacion(0, 1, 0));
  // agregar(MAT_Escalado(0.4, 0.4, 0.4));
  // agregar(new Esfera);
  // agregar(MAT_Traslacion(0, 1, 0));
  // agregar(MAT_Escalado(0.4, 0.4, 0.4));
  // agregar(new Esfera); /* Radio 1, centrada en el origen */
}