// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Práctica 2  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica4.hpp"
#include "grafo-escena.hpp"
#include "materiales.hpp"


using namespace std ;

static constexpr float DELTA = 3.0;
static constexpr int NUMERO_OBJETOS_P4 = 1;
static unsigned objeto_activo_p4 = 0;
static NodoGrafoEscena* objetos_p4[NUMERO_OBJETOS_P4] = {nullptr};
static ColeccionFuentesP4* fuentes_luz_p4 = nullptr;
static unsigned angulo_activo_p4 = 0;

// ---------------------------------------------------------------------
// Función para implementar en la práctica 4 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P4_Inicializar(  )
{
   cout << "Creando objetos de la práctica 4 .... " << flush ;

   fuentes_luz_p4 = new ColeccionFuentesP4;
   objetos_p4[0] = new EscenaP4;

   cout << "hecho." << endl << flush ;
}

// ---------------------------------------------------------------------
// Función invocada al pulsar una tecla con la práctica 4 activa:
// (si la tecla no se procesa en el 'main').
//
//  - devuelve 'true' si la tecla se usa en esta práctica para cambiar
//    entre el grado de libertad o incrementar/decrementar valores
//  - devuelve 'false' si la tecla no se usa en esta práctica (no ha
//    cambiado nada)

bool P4_FGE_PulsarTeclaCaracter(unsigned char tecla) {
  bool res = false;  // valor devuelto: es true solo cuando se ha procesado
                     // alguna tecla
  FuenteDireccional* fuente_direccional =
      (FuenteDireccional*)fuentes_luz_p4->ptrFuente(0);

  switch (toupper(tecla)) {
    // Iterar entre los ángulos
    case 'G':
      angulo_activo_p4 = (angulo_activo_p4 + 1) % 2;
      cout << "práctica 4: ángulo_activo " << angulo_activo_p4 << endl;
      res = true;
      break;

    // Incrementar el ángulo activo
    case '>':
      fuente_direccional->variarAngulo(angulo_activo_p4, DELTA);
      cout << "práctica 4: ángulo " << angulo_activo_p4 << " cambiado a "
           << (angulo_activo_p4 ? fuente_direccional->latitud
                                : fuente_direccional->longitud)
           << endl;
      res = true;
      break;

    // Decrementar el ángulo activo
    case '<':
      fuente_direccional->variarAngulo(angulo_activo_p4, -DELTA);
      cout << "práctica 4: ángulo " << angulo_activo_p4 << " cambiado a "
           << (angulo_activo_p4 ? fuente_direccional->latitud
                                : fuente_direccional->longitud)
           << endl;
      res = true;
      break;
    default:
      break;
  }
  return res;
}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 4  para dibujar los objetos
// se debe de usar el modo de dibujo que hay en el parámetro 'cv'
// (se accede con 'cv.modoVisu')

void P4_DibujarObjetos( ContextoVis & cv )
{
   fuentes_luz_p4->activar();
   if (objetos_p4[objeto_activo_p4] != nullptr)
     objetos_p4[objeto_activo_p4]->visualizarGL(cv);

}
