// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Práctica 2  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica3.hpp"
#include "grafo-escena.hpp"



using namespace std ;

static unsigned grado_libertad_actual = 0;
static bool animaciones_activadas = false;
static BrazoMecanico *objeto;

// ---------------------------------------------------------------------
// Función para implementar en la práctica 1 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P3_Inicializar() {
  cout << "Creando objetos de la práctica 3 .... " << flush;
  objeto = new BrazoMecanico();
  cout << "hecho." << endl << flush;
}

// ---------------------------------------------------------------------
// Función invocada al pulsar una tecla con la práctica 1 activa:
// (si la tecla no se procesa en el 'main').
//
//  - devuelve 'true' si la tecla se usa en esta práctica para cambiar
//    entre el grado de libertad o incrementar/decrementar valores
//  - devuelve 'false' si la tecla no se usa en esta práctica (no ha
//    cambiado nada)

bool P3_FGE_PulsarTeclaCaracter(unsigned char tecla) {
  bool res = false;  // valor devuelto (true si se ha procesado tecla)

  switch (toupper(tecla)) {
    case 'O':
      // Solo hay un objeto
      break;

    case 'A':
      // práctica 3: activar o desactivar animaciones
      animaciones_activadas = !animaciones_activadas;

      if (animaciones_activadas) {
        FijarFuncDesocupado(FGE_Desocupado);
      }

      res = true;
      cout << "animaciones "
           << (animaciones_activadas ? "activadas" : "desactivadas") << endl;

      break;

    case 'G':

      // Activar el siguiente parámetro

      if (objeto != nullptr) {
        grado_libertad_actual =
            (grado_libertad_actual + 1) % objeto->numParametros();

        cout
            << "el grado de libertad activo es: "
            << objeto->leerPtrParametro(grado_libertad_actual)->leer_descripcion()
            << endl;

        res = true;
      }

      break;

    case 'R':
      // práctica 3: reset de animaciones
      animaciones_activadas = false;

      if (objeto != nullptr) {
        for (size_t i = 0; i < objeto->numParametros(); i++) {
          objeto->leerPtrParametro(i)->reset();
        }
      }

      cout << "animaciones desactivadas y parámetros reiniciados" << endl;
      res = true;

      break;

    case '>':
      // práctica 3: acelerar o incrementar parámetro

      if (objeto != nullptr) {
        Parametro* parametro_actual =
            objeto->leerPtrParametro(grado_libertad_actual);

        if (animaciones_activadas) {
          parametro_actual->acelerar();
          cout << "parámetro acelerado" << endl;
        } else {
          parametro_actual->incrementar();
          cout << "parámetro incrementado" << endl;
        }

        res = true;
      }

      break;

    case '<':
      // práctica 3: decelerar o decrementar parámetro

      if (objeto != nullptr) {
        Parametro* parametro_actual =
            objeto->leerPtrParametro(grado_libertad_actual);

        if (animaciones_activadas) {
          parametro_actual->decelerar();
          cout << "parámetro decelerado" << endl;
        } else {
          parametro_actual->decrementar();
          cout << "parámetro decrementado" << endl;
        }

        res = true;
      }

      break;
    default:
      break;
  }
  return res;
}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 3  para dibujar los objetos
// se debe de usar el modo de dibujo que hay en el parámetro 'cv'
// (se accede con 'cv.modoVisu')

void P3_DibujarObjetos(ContextoVis& cv) {
  // práctica 3: visualizar el objeto de la práctica 3
  objeto->visualizarGL(cv);
}

// Si las animaciones están activadas, actualizar el objeto 
// activo, forzar redibujado, devolver true.
// Si las animaciones están desactivadas, devolver false.
bool P3_FGE_Desocupado() {
  if (!animaciones_activadas) return false;

  if (objeto != nullptr) objeto->siguienteCuadro();

  redibujar_ventana = true;
  return true;
}
