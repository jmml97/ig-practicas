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

// Llama de forma recursiva a las sucesivas funciones de visualización de 
// los objetos hijos del padre, a la vez que va añadiendo las matrices de
// transformación del grafo de escena
void NodoGrafoEscena::visualizarGL(ContextoVis& cv) {

  bool hay_iluminacion = cv.modoVis == modoSombreadoPlano || cv.modoVis == modoSombreadoSuave;

  cv.pilaMateriales.push();

  /* void glMatrixMode(GLenum mode);                                  */
  /* Especifica sobre qué matrices se deben realizar las operaciones  */
  glMatrixMode(GL_MODELVIEW);

  /* void glPushMatrix(void);                                             */
  /* Hay una pila de matrices para cada MatrixMode. glPushMatrix() añade	*/
  /* matriz a la pila duplicando la actual	                              */
  glPushMatrix();

  for (auto entrada : entradas) {
    if (entrada.tipo == TipoEntNGE::objeto) {
      if (cv.modoSeleccionFBO && leerIdentificador() >= 0) {
        int identificador = leerIdentificador();
        FijarColorIdent(identificador);
      }
      entrada.objeto->visualizarGL(cv);
    } else if (entrada.tipo == TipoEntNGE::material && hay_iluminacion) {
      cv.pilaMateriales.activarMaterial(entrada.material);
    } else if (entrada.tipo == TipoEntNGE::transformacion) {
      glMatrixMode(GL_MODELVIEW);
      /* Multiplica la matriz actual por la especificada */
      glMultMatrixf(*(entrada.matriz));
    }
  }
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  cv.pilaMateriales.pop();
}
// -----------------------------------------------------------------------------

NodoGrafoEscena::NodoGrafoEscena() {
  entradas = {};
}
// -----------------------------------------------------------------------------

// Recorre los nodos hijos del grafo de escena y los cambia al color 
// especificado
void NodoGrafoEscena::fijarColorNodo(const Tupla3f& nuevo_color) {
  for (auto entrada : entradas) {
    if(entrada.tipo == TipoEntNGE::objeto) {
      entrada.objeto->fijarColorNodo(nuevo_color);
    }
  }
}

// Añade una entrada al final del grafo de escena y devuelve el índice
// de la entrada dentro del vector de entradas
unsigned NodoGrafoEscena::agregar(const EntradaNGE& entrada) {
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

// Devuelve el puntero a la matriz en la i-ésima entrada
Matriz4f* NodoGrafoEscena::leerPtrMatriz(unsigned indice) {
  Matriz4f* ptr;
  try {
    ptr = entradas[indice].matriz;
  } catch(std::string e) {
    std::cout << e << std::endl;
  }

  return ptr;
}

// -----------------------------------------------------------------------------
// Si 'centro_calculado' es 'false', recalcula el centro usando los centros
// de los hijos (el punto medio de la caja englobante de los centros de hijos).
void NodoGrafoEscena::calcularCentroOC() {
  if (!centro_calculado) {
    Matriz4f matriz_modelado = MAT_Ident();
    vector<Tupla3f> centros_hijos;

    // Tenemos que ir acumulando las transformaciones del grafo de escena en la
    // matriz de modelado para  calcular correctamente los centros en
    // coordenadas de objeto.
    for (auto& entrada : entradas) {
      if (entrada.tipo == TipoEntNGE::objeto) {
        entrada.objeto->calcularCentroOC();
        centros_hijos.push_back(matriz_modelado *
                                entrada.objeto->leerCentroOC());
      } else if (entrada.tipo == TipoEntNGE::transformacion)
        matriz_modelado = matriz_modelado * (*entrada.matriz);
    }

    ponerCentroOC(calcularCentroCE(centros_hijos));
    centro_calculado = true;
  }
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
  assert(0 < ident_busc);

  // no calculamos los centros de objetos no seleccionables
  if (leerIdentificador() > 0 && !centro_calculado) {
    calcularCentroOC();
  }

  if (leerIdentificador() == ident_busc) {
    centro_wc = mmodelado * leerCentroOC();
    *objeto = this;
    return true;
  } else {
    bool encontrado = false;
    Matriz4f nueva_mmodelado = mmodelado;

    for (unsigned i = 0; i < entradas.size() && !encontrado; i++) {
      if (entradas[i].tipo == TipoEntNGE::transformacion) {
        nueva_mmodelado = nueva_mmodelado * (*entradas[i].matriz);
      } else if (entradas[i].tipo == TipoEntNGE::objeto) {
        encontrado = entradas[i].objeto->buscarObjeto(
            ident_busc, nueva_mmodelado, objeto, centro_wc);
      }
    }
    return encontrado;
  }
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
  fijarColorNodo({0.537, 0.388, 0.184});
}

// *********************************************************************
// Articulacion del Brazo del Brazo Mecánico

Articulacion::Articulacion() {
  agregar(MAT_Escalado(0.4, 0.4, 0.4));
  agregar(new Esfera);
  fijarColorNodo({0.521, 0.521, 0.521});
}

// *********************************************************************
// Pinza de la Cabeza del Brazo Mecánico

Pinza::Pinza() {
  indice_rotacion_pinza = agregar(MAT_Rotacion(0, 0, 0, 1));
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
  agregar(MAT_Traslacion(0.3, 0.1, 0));
  pinza1 = new Pinza;
  agregar(pinza1);
  pinza2 = new Pinza;
  agregar(MAT_Traslacion(-0.6, 0, 0));
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

  // Añadimos la base del brazo
  agregar(new Base);

  // Añadimos el primer cilindro del brazo y la rotación de todo el brazo
  agregar(MAT_Traslacion(0, 0.1, 0));
  unsigned indice_rotacion_brazo = agregar(MAT_Rotacion(0, 0, 1, 0));
  agregar(new Cilindro(0.2, 0.5, 100, true, true));

  // Añadimos las esferas correspondientes a las articulaciones, junto
  // a sus rotaciones
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

  // Añadimos la cabeza del brazo mecánico junto a la rotación del mismo
  agregar(MAT_Traslacion(0, 0.3, 0));
  unsigned indice_rotacion_cabeza = agregar(MAT_Rotacion(0, 0, 1, 0));
  auto cabeza = new Cabeza;
  agregar(cabeza);

  // Creamos todos los parámetros de las rotaciones, empleando para ello
  // los índices que hemos almacenado
  Parametro rotacion_brazo(
    "rotación del brazo",
    leerPtrMatriz(indice_rotacion_brazo),
    [=](float v) {return MAT_Rotacion(v, 0, 1, 0);},
    true,
    0,      /* c: Valor inicial (central para grados acotados)       */
    360,    /* s: Semiamplitud (si acotado) o factor de escala si no */
    0.0005	/* f: Si acotado, frecuencia                             */
  );
  parametros.push_back(rotacion_brazo);

  Parametro rotacion_art1(
    "rotación de articulación 1",
    leerPtrMatriz(indice_rotacion_art1),
    [=](float v) {return MAT_Rotacion(v, 1, 0, 0);},
    true,
    0,
    60,     
    0.001   
  );
  parametros.push_back(rotacion_art1);

  Parametro rotacion_art2(
    "rotación de articulación 2",
    leerPtrMatriz(indice_rotacion_art2),
    [=](float v) {return MAT_Rotacion(v, 1, 0, 0);},
    true,
    0,
    45,
    0.001
  );
  parametros.push_back(rotacion_art2);

  Parametro rotacion_art3(
    "rotación de articulación 3",
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
    360,
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

//******************************************************************************

EscenaP4::Lata::Lata() {
  ponerNombre("lata de cocacola");
  ponerIdentificador(1232353);

  agregar(MAT_Escalado(1.5, 1.5, 1.5));

  agregar(new MaterialTapasLata);
  agregar(new MallaRevol("../plys/lata-psup.ply", 30, false, false, true));
  agregar(new MaterialLata);
  agregar(new MallaRevol("../plys/lata-pcue.ply", 30, false, false, true));
  agregar(new MaterialTapasLata);
  agregar(new MallaRevol("../plys/lata-pinf.ply", 30, false, false, true));
}

EscenaP4::Peones::PeonNegro::PeonNegro() {
  ponerNombre("peón negro");
  ponerIdentificador(21212);
  agregar(new MaterialPeonNegro);
  agregar(new MallaRevol("../plys/peon.ply", 30, true, false, true));
}

EscenaP4::Peones::PeonBlanco::PeonBlanco() {
  ponerNombre("peón blanco");
  ponerIdentificador(323243);
  agregar(new MaterialPeonBlanco);
  agregar(new MallaRevol("../plys/peon.ply", 30, true, false, true));
}

EscenaP4::Peones::PeonMadera::PeonMadera() {
  ponerNombre("peón de madera");
  ponerIdentificador(0);
  agregar(new MaterialPeonMadera);
  agregar(new MallaRevol("../plys/peon.ply", 30, true, false, false));
}

EscenaP4::Peones::Peones() {
  ponerNombre("peones");

  agregar(MAT_Escalado(0.5, 0.5, 0.5));
  agregar(new PeonNegro);
  agregar(MAT_Traslacion(3.0, 0.0, 0.0));
  agregar(new PeonBlanco);
  agregar(MAT_Traslacion(-6.0, 0.0, 0.0));
  agregar(new PeonMadera);
}

EscenaP4::EscenaP4() {
  ponerNombre("escenaP4");

  agregar(new Lata);
  agregar(MAT_Traslacion(0.0, 0.7, 2.0));
  agregar(new Peones);
}
