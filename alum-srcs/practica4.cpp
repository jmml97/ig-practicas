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
static constexpr int numObjetos4 = 2;
static unsigned objetoActivo4 = 0;
static NodoGrafoEscena * objetos4[numObjetos4] = {nullptr, nullptr};
static ColeccionFuentesP4 * cf4 = nullptr;
static unsigned angulo4 = 0;

// ---------------------------------------------------------------------
// Función para implementar en la práctica 4 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P4_Inicializar(  )
{
   cout << "Creando objetos de la práctica 4 .... " << flush ;

   cf4 = new ColeccionFuentesP4;
   objetos4[0] = new EscenaP4;

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

bool P4_FGE_PulsarTeclaCaracter( unsigned char tecla )
{
   bool res = false  ; // valor devuelto: es true solo cuando se ha procesado alguna tecla

   switch ( toupper( tecla ) )
   {
      case 'G' :
         // COMPLETAR: práctica 4: activar el siguiente ángulo (longitud o latitud)
         // ....

         break ;

      case '>' :
         // COMPLETAR: práctica 4: incrementar el ángulo activo
         // ....

         break ;

      case '<' :
         // COMPLETAR: práctica 4: decrementar el ángulo activo
         // ....

         break ;
      default :
         break ;
   }
   return res ;

}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 4  para dibujar los objetos
// se debe de usar el modo de dibujo que hay en el parámetro 'cv'
// (se accede con 'cv.modoVisu')

void P4_DibujarObjetos( ContextoVis & cv )
{
   cf4->activar();
   if (objetos4[objetoActivo4] != nullptr)
     objetos4[objetoActivo4]->visualizarGL(cv);

}
