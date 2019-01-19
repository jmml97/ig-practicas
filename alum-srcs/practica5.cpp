// *********************************************************************
// **
// ** Informática Gráfica
// ** Práctica 5  (implementaciones)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica5.hpp"

#include "CamaraInter.hpp"
#include "grafo-escena.hpp"

using namespace std;

// Declaración de variables de la práctica 5 (static) (escena, viewport, el
// vector de camaras, y las variables que sirven para gestionar el modo
// arrastrar).

// número de cámaras.
static constexpr int NUMERO_CAMARAS = 4;
// vector de cámaras con vista como mínimo de alzado, planta y perfil.
static CamaraInteractiva* camaras[NUMERO_CAMARAS] = {nullptr, nullptr, nullptr,
                                                     nullptr};
static int camara_activa = 0;

// viewport actual (se recalcula al inicializar y al fijar las matrices).
static Viewport viewport;

// true si se está en modo arrastrar, false si no.
static bool modo_arrastrar = false;

// grafo de escena.
static constexpr int NUMERO_OBJETOS_P5 = 1;
static unsigned objeto_activo_p5 = 0;
static NodoGrafoEscena* objetos_p5[NUMERO_OBJETOS_P5] = {nullptr};
static ColeccionFuentesP4* fuentes_luz_p5 = nullptr;

// Desplazamiento de la cámara.
static const float DELTA = 5.0;

// Posiciones anteriores del ratón.
static int x_anterior, y_anterior;

// ---------------------------------------------------------------------

void P5_Inicializar(int vp_ancho, int vp_alto) {
  cout << "Creando objetos de la práctica 5 .... " << flush;
  // Inicialización de las variables de la práctica 5 (incluyendo el viewport).
  fuentes_luz_p5 = new ColeccionFuentesP4;
  objetos_p5[0] = new EscenaP4;
  viewport = Viewport(0, 0, vp_ancho, vp_alto);
  float ratio = (float)vp_alto / vp_ancho;
  // Alzado
  camaras[0] =
      new CamaraInteractiva(true, ratio, 0, 0, {0, 0, 0}, true, 80.0, 1.8);
  // Perfil
  camaras[1] =
      new CamaraInteractiva(true, ratio, 90, 0, {0, 0, 0}, true, 80.0, 2.0);
  // Planta
  camaras[2] =
      new CamaraInteractiva(true, ratio, 0, 90, {0, 0, 0}, true, 80.0, 4.0);
  // Alzado ortográfica
  camaras[3] =
      new CamaraInteractiva(true, ratio, 0, 0, {0, 0, 0}, false, 80.0, 3.2);

  cout << "hecho." << endl << flush;
}
// ---------------------------------------------------------------------

void P5_FijarMVPOpenGL(int vp_ancho, int vp_alto) {
  // Actualizar viewport, actualizar y activar la camara actual (en base a las
  // dimensiones del viewport)

  viewport = Viewport(0, 0, vp_ancho, vp_alto);
  viewport.fijarGlViewport();

  camaras[camara_activa]->ratio_yx_vp = viewport.ratio_yx;
  camaras[camara_activa]->calcularViewfrustum();  // recalcular m_proy
  camaras[camara_activa]->activar();
}
// ---------------------------------------------------------------------

void P5_DibujarObjetos(ContextoVis& cv) {
  // Activar las fuentes de luz y visualizar la escena (se supone que la camara
  // actual ya está activada)
  fuentes_luz_p5->activar();
  if (objetos_p5[objeto_activo_p5] != nullptr) {
    objetos_p5[objeto_activo_p5]->visualizarGL(cv);
  }
}

// ---------------------------------------------------------------------

bool P5_FGE_PulsarTeclaCaracter(unsigned char tecla) {
  bool result = true;

  bool examinar;

  switch (toupper(tecla)) {
    case 'C':
      // Activar siguiente cámara.
      camara_activa = (camara_activa + 1) % NUMERO_CAMARAS;
      cout << "práctica 5: la nueva cámara activa es " << camara_activa << endl;
      break;

    case 'V':
      // Conmutar la cámara actual entre modo examinar y el modo primera
      // persona.
      examinar = camaras[camara_activa]->examinar;
      if (examinar) {
        camaras[camara_activa]->modoPrimeraPersona();
      } else {
        camaras[camara_activa]->modoExaminar();
      }
      cout << "práctica 5: el modo de la cámara " << camara_activa
           << " ha cambiado a " << (examinar ? "primera persona" : "examinar")
           << endl;
      break;

    case '-':
      // Desplazamiento en Z de la cámara actual (positivo) (desplaZ).
      camaras[camara_activa]->desplaZ(DELTA);
      cout << "práctica 5: desplazamiento en el eje Z (acercar)" << endl;
      break;

    case '+':
      // Desplazamiento en Z de la cámara actual (negativo) (desplaZ).
      camaras[camara_activa]->desplaZ(-DELTA);
      cout << "práctica 5: desplazamiento en el eje Z (alejar)" << endl;
      break;

    default:
      result = false;
      break;
  }
  return result;
}
// ---------------------------------------------------------------------

bool P5_FGE_PulsarTeclaEspecial(int tecla) {
  bool result = true;

  switch (tecla) {
    case GLFW_KEY_LEFT:
      // Desplazamiento/rotación hacia la izquierda (moverHV).
      camaras[camara_activa]->moverHV(-DELTA, 0);
      cout << "práctica 5: desplazamiento hacia la izquierda" << endl;
      break;
    case GLFW_KEY_RIGHT:
      // Desplazamiento/rotación hacia la derecha (moverHV).
      camaras[camara_activa]->moverHV(DELTA, 0);
      cout << "práctica 5: desplazamiento hacia la derecha" << endl;
      break;
    case GLFW_KEY_UP:
      // Desplazamiento/rotación hacia arriba (moverHV).
      camaras[camara_activa]->moverHV(0, DELTA);
      cout << "práctica 5: desplazamiento hacia arriba" << endl;
      break;
    case GLFW_KEY_DOWN:
      // Desplazamiento/rotación hacia abajo (moverHV).
      camaras[camara_activa]->moverHV(0, -DELTA);
      cout << "práctica 5: desplazamiento hacia abajo" << endl;
      break;
    default:
      result = false;
      break;
  }

  return result;
}
// ---------------------------------------------------------------------
// se llama al hacer click con el botón izquierdo

void P5_ClickIzquierdo( int x, int y )
{

   // COMPLETAR: práctica 5: visualizar escena en modo selección y leer el color del pixel en (x,y)


   // 1. crear un 'contextovis' apropiado
   // .....

   // 2. visualizar en modo selección (sobre el backbuffer)
   // ....

   // 3. leer el color del pixel, si es 0 no se hace nada
   // .....

   // 4. buscar el objeto en el grafo de escena e informar del mismo
   // .....

}
// ---------------------------------------------------------------------
// se llama al mover el botón en modo arrastrar

void P5_InicioModoArrastrar(int x, int y) {
  // Activar bool de modo arrastrar, registrar (x,y) de inicio del modo
  // arrastrar
  modo_arrastrar = true;
  x_anterior = x;
  y_anterior = y;
}
// ---------------------------------------------------------------------
// se llama al subir el botón derecho del ratón

void P5_FinModoArrastrar()
{
   // Desactivar bool del modo arrastrar
   modo_arrastrar = false;
}
// ---------------------------------------------------------------------
// se llama durante el modo arrastrar

void P5_RatonArrastradoHasta(int x, int y) {
  // Calcular desplazamiento desde inicio de modo arrastrar, actualizar la
  // camara (moverHV) mover cámara
  camaras[camara_activa]->moverHV(x - x_anterior, y - y_anterior);

  // registrar última posición
  x_anterior = x;
  y_anterior = y;
}
// ---------------------------------------------------------------------
// pulsar/levantar botón del ratón, específico de la práctica 5

bool P5_FGE_ClickRaton( int boton, int estado, int x, int y )
{
   //cout << "P5_FGE_ClickRaton" << endl ;
   if ( estado == GLFW_PRESS && boton == GLFW_MOUSE_BUTTON_LEFT  )
      P5_ClickIzquierdo( x, viewport.alto-y );
   else if ( estado == GLFW_PRESS && boton == GLFW_MOUSE_BUTTON_RIGHT )
      P5_InicioModoArrastrar( x, viewport.alto-y );
   else if ( estado == GLFW_RELEASE && boton == GLFW_MOUSE_BUTTON_RIGHT )
      P5_FinModoArrastrar();
   else
      return false ;

   return true ;
}
// ---------------------------------------------------------------------
bool P5_FGE_RatonMovidoPulsado( int x, int y )
{
   if ( modo_arrastrar )
   {
      P5_RatonArrastradoHasta( x, viewport.alto-y );
      return true ;
   }
   else
      return false ;
}
// ---------------------------------------------------------------------

bool P5_FGE_Scroll(int direction) {
  // Acercar/alejar la camara (desplaZ)
  camaras[camara_activa]->desplaZ(direction * DELTA);
  return true;
}
// ---------------------------------------------------------------------

// 0 ≤ ident < 2^24
void FijarColorIdent(const int ident) {
  // Fijar color actual de OpenGL usando 'ident' (glColor3ub).
  if (ident >= 0) {
    const unsigned char byte_R = ident % 0x100U,
                        byte_G = (ident / 0x100U) % 0x100U,
                        byte_B = (ident / 0x10000U) % 0x100U;

    glColor3ub(byte_R, byte_G, byte_B);
  }
}
//---------------

int LeerIdentEnPixel(int xpix, int ypix) {
  // Leer el identificador codificado en el color del píxel (x,y).
  unsigned char bytes[3];
  // Leer los 3 bytes del framebuffer (píxel = 1×1).
  glReadPixels(xpix, ypix, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void*)bytes);
  // Reconstruir el indentificador y devolverlo.
  return bytes[0] + (0x100U * bytes[1]) + (0x10000U * bytes[2]);
}
//---------------
