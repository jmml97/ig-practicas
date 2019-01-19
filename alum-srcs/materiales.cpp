// *********************************************************************
// **
// ** Gestión de materiales y texturas (implementación)
// **
// ** Copyright (C) 2014 Carlos Ureña
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

#include "materiales.hpp"
#include "matrices-tr.hpp"

using namespace std;

const bool trazam = false;

//*********************************************************************

PilaMateriales::PilaMateriales() { actual = nullptr; }
// ----------------------------------------------------------------------------

void PilaMateriales::activarMaterial(Material* material) {
  if (material != actual) {
    actual = material;
    if (actual != nullptr) actual->activar();
  }
}

// ----------------------------------------------------------------------------

void PilaMateriales::activarActual() {
  if (actual != nullptr) actual->activar();
}

// ----------------------------------------------------------------------------

void PilaMateriales::push() { pila.push_back(actual); }

// ----------------------------------------------------------------------------

void PilaMateriales::pop() {
  if (pila.size() == 0) {
    cout << "error: intento de hacer 'pop' de un material en una pila de "
            "materiales vacía."
         << endl
         << flush;
    exit(1);
  }

  Material* anterior = pila[pila.size() - 1];
  pila.pop_back();
  activarMaterial(anterior);  // cambia 'actual'
}

//*****************************************************************************

Textura::Textura(const std::string& nombreArchivoJPG) {
  enviada = false;
  modo_gen_ct = mgct_desactivada;  // desactivamos las coordenadas automáticas
  imagen = new jpg::Imagen(nombreArchivoJPG);
}

//-----------------------------------------------------------------------------

void Textura::enviar() {
  if (modo_gen_ct == mgct_coords_objeto) {
    cout << "Enviar textura: Modo objeto" << endl;
    glTexGenfv(GL_S, GL_OBJECT_PLANE, coefs_s);
    glTexGenfv(GL_T, GL_OBJECT_PLANE, coefs_t);
  }

  if (modo_gen_ct == mgct_coords_ojo) {
    cout << "Enviar textura: modo ojo" << endl;
    glTexGenfv(GL_S, GL_EYE_PLANE, coefs_s);
    glTexGenfv(GL_T, GL_EYE_PLANE, coefs_t);
  }

  /* The glGenTextures function generates texture names. The generated textures
   * assume the dimensionality of the texture target to which they are first
   * bound with the glBindTexture function. */
  glGenTextures(1, &ident_textura);

  /* The glBindTexture function enables the creation of a named texture that is
   * bound to a texture target. When a texture is bound to a target, the
   * previous binding for that target is no longer in effect. */
  glBindTexture(GL_TEXTURE_2D, ident_textura);

  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, (GLsizei)imagen->tamX(),
                    (GLsizei)imagen->tamY(), GL_RGB, GL_UNSIGNED_BYTE,
                    imagen->leerPixels());

  enviada = true;
}

//----------------------------------------------------------------------

Textura::~Textura() {
  using namespace std;
  cout << "destruyendo textura...imagen ==" << imagen << endl;
  if (imagen != NULL) delete imagen;

  imagen = NULL;
  cout << "hecho (no hecho!)" << endl << flush;
}

//----------------------------------------------------------------------
// por ahora, se asume la unidad de texturas #0

void Textura::activar() {
  glEnable(GL_TEXTURE_2D);

  // Enviar a la GPU la primera vez
  if (!enviada) {
    enviar();
  } else {
    glBindBuffer(GL_TEXTURE_2D, ident_textura);
  }; 

  // Generación automática de coordenadas
  if (modo_gen_ct == mgct_desactivada) {
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
  } else {
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);

    if (modo_gen_ct == mgct_coords_objeto) {
      glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
      glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

      glTexGenfv(GL_S, GL_OBJECT_PLANE, coefs_s);
      glTexGenfv(GL_T, GL_OBJECT_PLANE, coefs_t);
    } else {
      glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
      glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);

      glTexGenfv(GL_S, GL_EYE_PLANE, coefs_s);
      glTexGenfv(GL_T, GL_EYE_PLANE, coefs_t);
    }
  }

  glBindTexture(GL_TEXTURE_2D, ident_textura);
}

// *********************************************************************

TexturaXY::TexturaXY(const std::string& nom) : Textura(nom) {
  modo_gen_ct = mgct_coords_ojo;

  coefs_s[0] = 1.0;
  for (unsigned i = 1; i < 4; i++) coefs_s[i] = 0.0;

  coefs_t[1] = 1.0;
  for (unsigned i = 0; i < 4; i++)
    if (i != 1) coefs_t[i] = 0.0;
}

// *********************************************************************

Material::Material() {
  iluminacion = false;
  tex = NULL;
  coloresCero();
}

// ---------------------------------------------------------------------

Material::Material(const std::string& nombreArchivoJPG) {
  iluminacion = true;
  tex = new Textura(nombreArchivoJPG);

  coloresCero();

  del.emision = VectorRGB(0.0, 0.0, 0.0, 1.0);
  del.ambiente = VectorRGB(0.0, 0.0, 0.0, 1.0);
  del.difusa = VectorRGB(0.5, 0.5, 0.5, 1.0);
  del.especular = VectorRGB(1.0, 1.0, 1.0, 1.0);

  tra.emision = VectorRGB(0.0, 0.0, 0.0, 1.0);
  tra.ambiente = VectorRGB(0.0, 0.0, 0.0, 1.0);
  tra.difusa = VectorRGB(0.2, 0.2, 0.2, 1.0);
  tra.especular = VectorRGB(0.2, 0.2, 0.2, 1.0);

  del.exp_brillo = 1.0;
  tra.exp_brillo = 1.0;
}

// ---------------------------------------------------------------------

// crea un material usando textura y coeficientes: ka,kd,ks y exponente
// (la textura puede ser NULL, la ilum. queda activada)
Material::Material(Textura* text, float ka, float kd, float ks, float exp)
    : Material() {
  iluminacion = true;
  tex = text;

  del.emision = tra.emision = {0.0, 0.0, 0.0, 1.0};
  del.ambiente = tra.ambiente = {ka, ka, ka, 1.0};
  del.difusa = tra.difusa = {kd, kd, kd, 1.0};
  del.especular = tra.especular = {ks, ks, ks, 1.0};
  del.exp_brillo = tra.exp_brillo = exp;

  ponerNombre("material con textura e iluminación");
}

// ---------------------------------------------------------------------

// crea un material con un color único para las componentes ambiental y difusa
// en el lugar de textura (textura == NULL)
Material::Material(const Tupla3f& colorAmbDif, float ka, float kd, float ks,
                   float exp) {
  VectorRGB color = {colorAmbDif(R), colorAmbDif(G), colorAmbDif(B), 1.0};

  iluminacion = true;
  tex = NULL;

  del.emision   = VectorRGB(0.0,0.0,0.0, 1.0);
  del.ambiente  = ka*color;
  del.difusa    = kd*color;
  del.especular = ks*color;

  tra.emision   = VectorRGB(0.0,0.0,0.0, 1.0);
  tra.ambiente  = ka*color;
  tra.difusa    = kd*color;
  tra.especular = ks*color;

  del.exp_brillo = tra.exp_brillo = exp;

  ponerNombre("material color plano con iluminación");
}

// -----------------------------------------------------------------------------

Material::Material(const float r, const float g, const float b) {
  iluminacion = false;
  tex = NULL;
  color = {r, g, b, 1.0};

  ponerNombre("material color plano sin iluminación");
}

//------------------------------------------------------------------------------

void Material::coloresCero() {
  const VectorRGB ceroRGBopaco(0.0, 0.0, 0.0, 1.0);

  color =

  del.emision = 
  del.ambiente = 
  del.difusa = 
  del.especular =

  tra.emision = 
  tra.ambiente = 
  tra.difusa = 
  tra.especular = ceroRGBopaco;

  del.exp_brillo = tra.exp_brillo = 1.0;
}

//------------------------------------------------------------------------------

Material::~Material() {
  if (tex != nullptr) {
    delete tex;
    tex = nullptr;
  }
}

//------------------------------------------------------------------------------

void Material::ponerNombre(const std::string& nuevo_nombre) {
  nombre_mat = nuevo_nombre;
}

//------------------------------------------------------------------------------

std::string Material::nombre() const { return nombre_mat; }

//------------------------------------------------------------------------------

void Material::activar() {
  // Activar textura
  if (tex == NULL)
    glDisable(GL_TEXTURE_2D);
  else
    tex->activar();

  if (iluminacion) {
    // Cara delantera
    glMaterialfv(GL_FRONT, GL_EMISSION, del.emision);     // M_E
    glMaterialfv(GL_FRONT, GL_AMBIENT, del.ambiente);     // M_A
    glMaterialfv(GL_FRONT, GL_DIFFUSE, del.difusa);       // M_D
    glMaterialfv(GL_FRONT, GL_SPECULAR, del.especular);   // M_S
    glMaterialf(GL_FRONT, GL_SHININESS, del.exp_brillo);  // e

    // Cara trasera
    glMaterialfv(GL_BACK, GL_EMISSION, tra.emision);     // M_E
    glMaterialfv(GL_BACK, GL_AMBIENT, tra.ambiente);     // M_A
    glMaterialfv(GL_BACK, GL_DIFFUSE, tra.difusa);       // M_D
    glMaterialfv(GL_BACK, GL_SPECULAR, tra.especular);   // M_S
    glMaterialf(GL_BACK, GL_SHININESS, tra.exp_brillo);  // e
  } else {
    glDisable(GL_LIGHTING);
    glColor4fv(color);
  }

  glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
}

//*****************************************************************************

FuenteLuz::FuenteLuz(const VectorRGB& p_color) {
  if (trazam) cout << "creando fuente de luz." << endl << flush;

  color_ambiente = p_color;
  color_difuso = p_color;
  color_especular = p_color;

  indice_fuente = -1;  // la marca como no activable hasta que no se le asigne
                       // índice
}

//-----------------------------------------------------------------------------

FuenteLuz::~FuenteLuz() {}

//-----------------------------------------------------------------------------

void FuenteLuz::activar() {}

//-----------------------------------------------------------------------------

void FuenteLuz::preActivar() {
  glEnable(GL_LIGHT0 + indice_fuente);

  // Fijamos los colores de la fuente de luz
  glLightfv(GL_LIGHT0 + indice_fuente, GL_AMBIENT, color_ambiente);
  glLightfv(GL_LIGHT0 + indice_fuente, GL_DIFFUSE, color_difuso);
  glLightfv(GL_LIGHT0 + indice_fuente, GL_SPECULAR, color_especular);
}

//*****************************************************************************

FuenteDireccional::FuenteDireccional(GLfloat p_longitud_inicial,
                                     GLfloat p_latitud_inicial,
                                     const VectorRGB& p_color)
    : FuenteLuz(p_color) {
  longitud = longitud_inicial = p_latitud_inicial;
  latitud = latitud_inicial = p_latitud_inicial;
}

//-----------------------------------------------------------------------------

void FuenteDireccional::activar() {
  if (indice_fuente == -1) return;  // La fuente no se puede activar

  Tupla4f posicion_eje_z = {0.0, 0.0, 1.0, 0.0};
  preActivar();

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  glLoadIdentity();
  glRotatef(longitud, 0.0, 1.0, 0.0);
  glRotatef(latitud, -1.0, 0.0, 0.0);
  glLightfv(GL_LIGHT0 + indice_fuente, GL_POSITION, posicion_eje_z);

  glPopMatrix();
}

//-----------------------------------------------------------------------------

void FuenteDireccional::variarAngulo(unsigned angulo, float incremento) {
  if (angulo == 0)
    longitud += incremento;
  else {
    if (incremento > 0)
      latitud = std::min(latitud + incremento, 90.0f);
    else
      latitud = std::max(latitud + incremento, -90.0f);
  }
}

//-----------------------------------------------------------------------------

bool FuenteDireccional::gestionarEventoTeclaEspecial(int key) {
  bool actualizar = true;
  const float incremento = 3.0f;

  switch (key) {
    case GLFW_KEY_RIGHT:
      longitud = longitud + incremento;
      break;
    case GLFW_KEY_LEFT:
      longitud = longitud - incremento;
      break;
    case GLFW_KEY_UP:
      latitud = std::min(latitud + incremento, 90.0f);
      break;
    case GLFW_KEY_DOWN:
      latitud = std::max(latitud - incremento, -90.0f);
      break;
    case GLFW_KEY_HOME:
      latitud = latitud_inicial;
      longitud = longitud_inicial;
      break;
    default:
      actualizar = false;
      cout << "tecla no usable para la fuente de luz." << endl << flush;
  }

  if (actualizar) {
    cout << "fuente de luz cambiada: longitud == " << longitud
         << ", latitud == " << latitud << endl
         << flush;
  }

  return actualizar;
}

//*****************************************************************************

FuentePosicional::FuentePosicional(const Tupla3f& p_posicion,
                                   const VectorRGB& p_color)
    : FuenteLuz(p_color) {
  posicion = p_posicion;
}

//-----------------------------------------------------------------------------

void FuentePosicional::activar() {
  if (indice_fuente == -1) return;  // La fuente no se puede activar

  Tupla4f posicion_homogenea = {posicion(X), posicion(Y), posicion(Z), 1.0};

  preActivar();
  glLightfv(GL_LIGHT0 + indice_fuente, GL_POSITION, posicion_homogenea);
}

//*****************************************************************************

ColFuentesLuz::ColFuentesLuz() { max_num_fuentes = 8; }

//-----------------------------------------------------------------------------

void ColFuentesLuz::insertar(FuenteLuz* pf)  // inserta una nueva
{
  assert(pf != nullptr);

  pf->indice_fuente = vpf.size();
  vpf.push_back(pf);
}

//------------------------------------------------------------------------------

// activa una colección de fuentes de luz
void ColFuentesLuz::activar() {
  glEnable(GL_LIGHTING);

  for (unsigned i = 0; i < vpf.size(); i++) vpf[i]->activar();

  for (unsigned i = vpf.size(); i < max_num_fuentes; i++)
    glDisable(GL_LIGHT0 + i);
}

//------------------------------------------------------------------------------

FuenteLuz* ColFuentesLuz::ptrFuente(unsigned i) {
  assert(i < vpf.size());
  return vpf[i];
}

//------------------------------------------------------------------------------

ColFuentesLuz::~ColFuentesLuz() {
  for (unsigned i = 0; i < vpf.size(); i++) {
    assert(vpf[i] != NULL);
    delete vpf[i];
    vpf[i] = NULL;
  }
}

//******************************************************************************

MaterialLata::MaterialLata()
    : Material(new Textura("../imgs/lata-coke.jpg"), 0.2, 3.0, 2.0, 5.0) {}

MaterialTapasLata::MaterialTapasLata() : Material(Tupla3f{0.7,0.7,0.7}, 0.2, 2.0, 2.5, 5.0) {}

MaterialPeonNegro::MaterialPeonNegro() : Material(Tupla3f{0.05,0.05,0.05}, 0.1, 0.1, 0.4, 5.0) {}

MaterialPeonBlanco::MaterialPeonBlanco() : Material(Tupla3f{0.8,0.8,0.8}, 0.1, 0.1, 0.4, 5.0) {}

MaterialPeonMadera::MaterialPeonMadera()
    : Material(new TexturaXY("../imgs/text-madera.jpg"), 0.2, 1.0, 0.4, 1.0) {}

ColeccionFuentesP4::ColeccionFuentesP4() {
  const VectorRGB color = {0.4, 0.4, 0.4, 1.0};

  insertar(new FuenteDireccional(-10.0, 30.0, color));
  insertar(new FuentePosicional({0.0, 3.0, 3.0}, color));
}
