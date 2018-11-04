#include "Parametro.hpp"
#include <cassert>
#include <cmath>
#include "Objeto3D.hpp"

// -----------------------------------------------------------------------------


Parametro::Parametro(const std::string& p_descripcion, Matriz4f* p_ptr_mat,
                     TFuncionCMF p_fun_calculo_matriz, bool p_acotado,
                     float p_c, float p_s, float p_f)
    : descripcion(p_descripcion),
      ptr_mat(p_ptr_mat),
      fun_calculo_matriz(p_fun_calculo_matriz),
      acotado(p_acotado),
      c(p_c),
      s(p_s),
      f(p_f)

{
  valor_norm = c;
  velocidad = v0; /* v0 = 1 */
}
// -----------------------------------------------------------------------------

void Parametro::siguiente_cuadro() {
  valor_norm += velocidad;
  *ptr_mat = fun_calculo_matriz(leer_valor_actual());
}
// -----------------------------------------------------------------------------

void Parametro::reset() {
  valor_norm = c;
  velocidad = v0;
  /* Recalculamos la matriz ya que ha variado el valor del parámetro */
  *ptr_mat = fun_calculo_matriz(leer_valor_actual());
}
// -----------------------------------------------------------------------------
void Parametro::incrementar() {
  valor_norm += delta;
  /* Recalculamos la matriz ya que ha variado el valor del parámetro */
  *ptr_mat = fun_calculo_matriz(leer_valor_actual());
}
// -----------------------------------------------------------------------------

void Parametro::decrementar() {
  valor_norm += delta;
  /* Recalculamos la matriz ya que ha variado el valor del parámetro */
  *ptr_mat = fun_calculo_matriz(leer_valor_actual());
}
// -----------------------------------------------------------------------------
void Parametro::acelerar() {
  velocidad += a;
}
// -----------------------------------------------------------------------------
void Parametro::decelerar() {
  velocidad -= a;
  if (velocidad < 0)
    velocidad = 0;
}
// -----------------------------------------------------------------------------

float Parametro::leer_valor_actual() {
  if (acotado) {
    return c + s * std::sin(f * 2.0 * M_PI * valor_norm);
  } else {
    return c + s * valor_norm;
  }
    
}

// -----------------------------------------------------------------------------
float Parametro::leer_velocidad_actual() {
  return velocidad;
}
// -----------------------------------------------------------------------------

std::string Parametro::leer_descripcion() {
  return descripcion;
}
// -----------------------------------------------------------------------------
