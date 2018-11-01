#include <functional>
#include <matrizg.hpp>
#include <string>

#ifndef GRADO_LIBERTAD_HPP
#define GRADO_LIBERTAD_HPP

class Objeto3D;

// tipo usado para las funciones que calculan una matriz a partir de un flotante
typedef std::function<Matriz4f(float)> TFuncionCMF;

// -----------------------------------------------------------------------------

class Parametro {
 private:
  // COMPLETAR: práctica 3: definir variables de instancia privadas de un
  // parámetro

  const std::string descripcion;        /* Descripción del grado de libertad */
  const bool acotado;                   /* true: el valor oscila entre dos   */ 
                                        /* valores                           */
  const TFuncionCMF fun_calculo_matriz; /* Función que produce una nueva     */
                                        /* matriz a partir de un valor float */
  
  const float c, /* Valor inicial (central para grados acotados)       */
      s,         /* Semiamplitud (si acotado) o factor de escala si no */
      f;         /* Si acotado, frecuencia                             */

  Matriz4f* const ptr_mat; /* Puntero a la matriz dentro del modelo */
  float valor_norm, /* Valor actual, normalizado y no acotado (crece desde 0) */
      velocidad;    /* Velocidad actual (se suma a valor_norm) */

  static constexpr float v0 = 1;
  static constexpr float delta = 1;
  static constexpr float a = 1;

 public:
  // crear grado de libertad no acotado
  Parametro(const std::string& p_descripcion, Matriz4f* p_ptr_mat,
            TFuncionCMF p_fun_calculo_matriz, bool p_acotado, float p_c,
            float p_s, float p_f);

  void siguiente_cuadro();  // actualizar valor y matriz al siguiente frame
  void reset();             // vuelve al estado inicial
  void incrementar();       // incrementar el valor
  void decrementar();       // decrementar el valor
  void acelerar();          // acelerar (aumentar velocidad normalizada)
  void decelerar();         // decelerar (disminuir la velocidad normalizada)
  float
  leer_valor_actual();  // devuelve el valor actual (escalado, no normalizado)
  float leer_velocidad_actual();  // devuelve velocidad o frecuencia actual
  std::string leer_descripcion();
};

#endif
