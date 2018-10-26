#include "Parametro.hpp"
#include <cassert>
#include <cmath>
#include "Objeto3D.hpp"

// -----------------------------------------------------------------------------

// crear grado de libertad no acotado

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
  // COMPLETAR: práctica 3: inicializar un parámetro
  // ....
  valor_norm = 0;
  velocidad = 1; /* v0 = 1 */
}
// -----------------------------------------------------------------------------

void Parametro::siguiente_cuadro() {
  // COMPLETAR: práctica 3: actualizar el valor y la matriz para el siguiente
  // cuadro
  // ....
  valor_norm += velocidad;
}
// -----------------------------------------------------------------------------

void Parametro::reset() {
  // COMPLETAR: práctica 3: hace reset de valor y velocidad
  // ....
  valor_norm = c;
  velocidad = v0;
  /* Recalculamos la matriz ya que ha variado el valor del parámetro */
  *ptr_mat = fun_calculo_matriz(valor_norm);
}
// -----------------------------------------------------------------------------
void Parametro::incrementar() {
  // COMPLETAR: práctica 3: incrementa el valor
  // ....
  valor_norm += delta;
  /* Recalculamos la matriz ya que ha variado el valor del parámetro */
  *ptr_mat = fun_calculo_matriz(valor_norm);
}
// -----------------------------------------------------------------------------

void Parametro::decrementar() {
  // COMPLETAR: práctica 3: decrementa el valor
  // ....
  valor_norm += delta;
  /* Recalculamos la matriz ya que ha variado el valor del parámetro */
  *ptr_mat = fun_calculo_matriz(valor_norm);
}
// -----------------------------------------------------------------------------
void Parametro::acelerar() {
  // COMPLETAR: práctica 3: incrementa la velocidad
  // ....
  velocidad += a;
}
// -----------------------------------------------------------------------------
void Parametro::decelerar() {
  // COMPLETAR: práctica 3: decrementa la velocidad
  // ....
  velocidad += a;
}
// -----------------------------------------------------------------------------

float Parametro::leer_valor_actual() {
  // COMPLETAR: práctica 3: devuelve el valor actual
  // ....
  if (acotado)
    return c + s * sin(f * 2 * M_PI * valor_norm);
  else
    return c + s * valor_norm;
}

// -----------------------------------------------------------------------------
float Parametro::leer_velocidad_actual() {
  // COMPLETAR: práctica 3: devuelve la velocidad actual
  // ....
  return velocidad;
}
// -----------------------------------------------------------------------------

std::string Parametro::leer_descripcion() {
  // COMPLETAR: práctica 3: devuelve la descripción
  // ....
  return descripcion;
}
// -----------------------------------------------------------------------------
