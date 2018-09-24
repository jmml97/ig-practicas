// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#include <aux.hpp>
#include <tuplasg.hpp>
#include "MallaInd.hpp"   // declaración de 'ContextoVis' 

// *****************************************************************************
// funciones auxiliares


// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd()
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre("malla indexada, anónima");

}
// -----------------------------------------------------------------------------

MallaInd::MallaInd( const std::string & nombreIni )
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre(nombreIni) ;

}
// -----------------------------------------------------------------------------
// calcula las dos tablas de normales

void MallaInd::calcular_normales()
{
   // COMPLETAR: en la práctica 2: calculo de las normales de la malla
   // .......


}


// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_MI(ContextoVis& cv) {
  // COMPLETAR: en la práctica 1: visualizar en modo inmediato (glDrawElements)
  // ...........

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, tabla_cord_vert.data());
  glDrawElements(GL_TRIANGLES, tabla_indices.size(), GL_UNSIGNED_INT,
                 tabla_indices.data());
  glDisableClientState(GL_VERTEX_ARRAY);
}

// ----------------------------------------------------------------------------
void MallaInd::visualizarDE_VBOs( ContextoVis & cv )
{
   // COMPLETAR: práctica 1: visualizar en modo diferido,
   //                        usando VBOs (Vertex Buffer Objects)
   // ..........

}

// -----------------------------------------------------------------------------

void MallaInd::visualizarGL(ContextoVis& cv) {
  // COMPLETAR: práctica 1: visualizar en modo inmediato o en modo diferido
  // (VBOs), (tener en cuenta el modo de visualización en 'cv' (alambre, sólido,
  // etc..))
  //
  // .............

  switch (cv.modoVis) {
    case modoPuntos:
      glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
      break;
    case modoAlambre:
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      break;
    case modoSolido:
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      break;
    default:
      break;
  }

  if (cv.usarVBOs) {
    visualizarDE_VBOs(cv);
  } else {
    visualizarDE_MI(cv);
  }
}
// *****************************************************************************

// *****************************************************************************

Cubo::Cubo()
:  MallaInd( "malla cubo" )
{

}
// *****************************************************************************

Tetraedro::Tetraedro()
:  MallaInd( "malla tetraedro")
{

}
// *****************************************************************************



