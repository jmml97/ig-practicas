#include <aux.hpp>
#include "CamaraInter.hpp"

using namespace std ;


// -----------------------------------------------------------------------------
// constructor de cámaras interactivas, los parámetros son:
//
//    * examinar_ini: fija modo examinar (true) o modo primera persona (false)
//    * ratio_yx_vp_ini: ratio del viewport (alto/ancho)
//    * longi_ini_grad, lati_ini_grad: longitud y latitud iniciales (en grados)
//    * aten_ini: punto de atención inicial
//    * pers_ini: fija proy. perspectiva (true) u ortográfica (false)
//

CamaraInteractiva::CamaraInteractiva( bool examinar_ini, float ratio_yx_vp_ini,
                                      float longi_ini_grad, float lati_ini_grad,
                                      const Tupla3f & aten_ini, bool pers_ini,
                                      float hfov_grad_ini, float dist_ini  )
{
   using namespace std ;

   examinar    = examinar_ini ;
   longi       = longi_ini_grad ;
   lati        = lati_ini_grad ;
   dist        = dist_ini ;
   aten        = aten_ini ;
   ratio_yx_vp = ratio_yx_vp_ini; //cout << "ratio_yx_vp_ini == " << ratio_yx_vp_ini << endl ;
   perspectiva = pers_ini ;
   hfov_grad   = hfov_grad_ini ;
   dx          = 0.0 ;
   dy          = 0.0 ;

   calcularViewfrustum(  );  // lee: dist, ratio_yx_vp, perspectica
   calcularMarcoCamara();    // lee: longi, lati, dist, aten
}


// ----------------------------------------------------------------------------
// calcula view-frustum (la matriz de proyección) usando:
//    perspectiva, dist, ratio_yx_vp
constexpr float n = 0.01 ; // valor 'near' en proy. persp

void CamaraInteractiva::calcularViewfrustum() {
  float aspect_ratio = (float)1 / ratio_yx_vp;
  if (perspectiva) {
    // calcular los parámetros del view frustum (vf), y
    // actualiza la matriz de proyección (vf.matrizProy) caso perspectiva: usar
    // hfov_grad, n, ratio_yx_vp, dist, función MAT_Frustum
    vf = ViewFrustum(hfov_grad, aspect_ratio, n, dist + 200);
  } else {
    // calcular los parámetros del view frustum (vf), y
    // actualiza la matriz de proyección (vf.matrizProy) caso ortográfica: usar
    // ratio_yx_vp, dist, función MAT_Ortografica
    aspect_ratio = ratio_yx_vp;

    vf.persp = false;
    vf.left = -dist * aspect_ratio;
    vf.right = dist * aspect_ratio;
    vf.top = dist;
    vf.bottom = -dist;
    vf.near = n;
    vf.far = dist + 200;

    vf.matrizProy =
        MAT_Ortografica(vf.left, vf.right, vf.bottom, vf.top, vf.near, vf.far);
  }
}

//-----------------------------------------------------------------------
// calcula el marco de referencia de la camara (y la matriz de vista), usando:
//     longi, lati, dist, aten
void CamaraInteractiva::calcularMarcoCamara() {
  // Recalcular 'mcv.matrizVista' y 'mcv.matriVistaInv'
  //    (1) Matriz = Trasl( aten )*Rotacion( longi, Y )*Rotacion( -lati, X )*
  //    Traslacion( (0,0,dist) )
  //    (2) ejes mcv = ejes mcv * matriz
  //    (3) recalcular matrices marco camara

  Matriz4f m = MAT_Traslacion(aten) * MAT_Rotacion(longi, 0, 1, 0) *
               MAT_Rotacion(-lati, 1, 0, 0) * MAT_Traslacion(0, 0, dist);

  mcv.eje[0] = m * Tupla4f(1, 0, 0, 0);
  mcv.eje[1] = m * Tupla4f(0, 1, 0, 0);
  mcv.eje[2] = m * Tupla4f(0, 0, 1, 0);
  // Origen
  mcv.org = m * Tupla4f(0, 0, 0, 1);

  recalcularMatrMCV();
}

//-----------------------------------------------------------------------
// actualiza las matrices que hay en 'cam.mcv'
// a patir de 'cam.mcv.org' y 'cam.mcv.ejes'
void CamaraInteractiva::recalcularMatrMCV() {
  // Recalcular 'mcv.matrizVista' y 'mcv.matriVistaInv'
  // en 'mcv' usando 'mcv.eje[X/Y/Z]' y 'mcv.org'
  mcv.matrizVista = MAT_Vista(mcv.eje, mcv.org);
  mcv.matrizVistaInv = MAT_Vista_inv(mcv.eje, mcv.org);
}

// -----------------------------------------------------------------------------
//  métodos para manipular (desplazar o rotar) la cámara
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// desplazar o rotar la cámara en horizontal/vertical
constexpr float urot = 1,  // unidad de rotación (1 grado)
    udesp = 0.01;          // unidad de desplazamiento

void CamaraInteractiva::moverHV(float nh, float nv) {
  if (examinar) {
    // Modo examinar.
    // Actualizar 'longi' y 'lati' y recalcular marco de cámara.
    longi += nh;
    lati += nv;
    calcularMarcoCamara();
  } else {
    // Modo primera persona.
    // Desplazar 'mcv.org' en X e Y y recalcular la matriz de vista (y
    // movimiento solidario del punto de atención)
    mcv.org(X) += nh * udesp;
    mcv.org(Y) += nv * udesp;

    aten(X) += nh * udesp;
    aten(Y) += nv * udesp;
    recalcularMatrMCV();
  }
}

// -----------------------------------------------------------------------------
// desplazar en el eje Z de la cámara (hacia adelante o hacia detrás)
constexpr float dmin = 2.0 * n,  // distancia minima (2*near)
    porc = 2;                    // porcentaje de cambio (1%)

void CamaraInteractiva::desplaZ(float nz) {
  if (examinar) {
    // Modo examinar.
    // Actualizar 'dist' usando 'nz', 'dmin' y 'porc' y recalcular marco de
    // cámara.
    dist = dmin + (dist - dmin) * (1.0 - nz * porc / 100.0);
    calcularMarcoCamara();
  } else {
    // Modo primera persona.
    // Desplazar 'mcv.org' en Z, y recalcular la matriz de vista (y movimiento
    // solidario del punto de atención).
    mcv.org(Z) -= nz * udesp;
    aten(Z) -= nz * udesp;
    recalcularMatrMCV();
  }
}
// -----------------------------------------------------------------------------



// -----------------------------------------------------------------------------
// métodos para cambiar de modo
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// fija punt. aten. y activa modo examinar
void CamaraInteractiva::modoExaminar(const Tupla3f& pAten) {
  // Fija punt. aten. y activa modo examinar, recalcula marco de cámara.
  examinar = true;
  aten = pAten;
  calcularMarcoCamara();
}

// -----------------------------------------------------------------------------
// pasa a modo examinar (mantiene p.aten.)
void CamaraInteractiva::modoExaminar() {
  // Activa modo examinar (no es necesario recalcular el marco de cámara).
  examinar = true;
}

// -----------------------------------------------------------------------------
// pasa al modo primera persona
void CamaraInteractiva::modoPrimeraPersona() {
  // Activa modo primera persona (no es necesario recalcular el marco de cámara)
  examinar = false;
}
