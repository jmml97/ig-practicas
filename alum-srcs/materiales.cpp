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


#include "matrices-tr.hpp"
#include "materiales.hpp"

using namespace std ;

const bool trazam = false ;

//*********************************************************************

PilaMateriales::PilaMateriales()
{
   actual = nullptr ;
}
// -----------------------------------------------------------------------------

void PilaMateriales::activarMaterial( Material * material )
{
   if ( material != actual )
   {
      actual = material ;
      if ( actual != nullptr )
         actual->activar();
   }
}
// -----------------------------------------------------------------------------

void PilaMateriales::activarActual()
{
   if ( actual != nullptr )
      actual->activar() ;
}
// -----------------------------------------------------------------------------

void PilaMateriales::push(  )
{
   pila.push_back( actual );
}
// -----------------------------------------------------------------------------

void PilaMateriales::pop(  )
{
   if ( pila.size() == 0 )
   {
      cout << "error: intento de hacer 'pop' de un material en una pila de materiales vacía." << endl << flush ;
      exit(1);
   }

   Material * anterior = pila[pila.size()-1] ;
   pila.pop_back();
   activarMaterial( anterior );  // cambia 'actual'
}

//**********************************************************************

Textura::Textura( const std::string & nombreArchivoJPG )
{
   // COMPLETAR: práctica 4: inicializar todas las variables
   // .....

}

// ---------------------------------------------------------------------

//----------------------------------------------------------------------

void Textura::enviar()
{
   // COMPLETAR: práctica 4: enviar la imagen de textura a la GPU
   // .......

}

//----------------------------------------------------------------------

Textura::~Textura( )
{
   using namespace std ;
   cout << "destruyendo textura...imagen ==" << imagen << endl ;
   if ( imagen != NULL )
      delete imagen ;

   imagen = NULL ;
   cout << "hecho (no hecho!)" << endl << flush ;
}

//----------------------------------------------------------------------
// por ahora, se asume la unidad de texturas #0

void Textura::activar(  )
{
   // COMPLETAR: práctica 4: enviar la textura a la GPU (solo la primera vez) y activarla
   // .......

}
// *********************************************************************

Material::Material()
{
   iluminacion = false ;
   tex = NULL ;
   coloresCero() ;
}
// ---------------------------------------------------------------------

Material::Material( const std::string & nombreArchivoJPG )
{
   iluminacion    = false ;
   tex            = new Textura( nombreArchivoJPG ) ;

   coloresCero();

   del.emision   = VectorRGB(0.0,0.0,0.0,1.0);
   del.ambiente  = VectorRGB( 0.0, 0.0, 0.0, 1.0);
   del.difusa    = VectorRGB( 0.5, 0.5, 0.5, 1.0 );
   del.especular = VectorRGB( 1.0, 1.0, 1.0, 1.0 );

   del.emision   = VectorRGB(0.0,0.0,0.0,1.0);
   del.ambiente  = VectorRGB( 0.0, 0.0, 0.0, 1.0);
   tra.difusa    = VectorRGB( 0.2, 0.2, 0.2, 1.0 );
   tra.especular = VectorRGB( 0.2, 0.2, 0.2, 1.0 );

}

// ---------------------------------------------------------------------
// crea un material usando textura y coeficientes: ka,kd,ks y exponente
// (la textura puede ser NULL, la ilum. queda activada)

Material::Material( Textura * text, float ka, float kd, float ks, float exp )
:  Material()
{
   // COMPLETAR: práctica 4: inicializar material usando text,ka,kd,ks,exp
   // .....

   ponerNombre("material con textura e iluminación") ;

 }

// ---------------------------------------------------------------------
// crea un material con un color único para las componentes ambiental y difusa
// en el lugar de textura (textura == NULL)
Material::Material( const Tupla3f & colorAmbDif, float ka, float kd, float ks, float exp )
{
   // COMPLETAR: práctica 4: inicializar material usando colorAmbDif,ka,kd,ks,exp
   // .....

   ponerNombre("material color plano, ilum.") ;
}
// ---------------------------------------------------------------------

Material::Material( const float r, const float g, const float b )
{
   // COMPLETAR: práctica 4: inicializar material usando un color plano sin iluminación
   // .....

}

//----------------------------------------------------------------------

void Material::coloresCero()
{
   const VectorRGB ceroRGBopaco(0.0,0.0,0.0,1.0);

   color         =

   del.emision   =
   del.ambiente  =
   del.difusa    =
   del.especular =

   tra.emision   =
   tra.ambiente  =
   tra.difusa    =
   tra.especular = ceroRGBopaco ;

   del.exp_brillo =
   tra.exp_brillo = 1.0 ;
}
//----------------------------------------------------------------------

Material::~Material()
{
   if ( tex != nullptr )
   {  delete tex ;
      tex = nullptr ;
   }
}
//----------------------------------------------------------------------

void Material::ponerNombre( const std::string & nuevo_nombre )
{
   nombre_mat = nuevo_nombre ;
}
//----------------------------------------------------------------------

std::string Material::nombre() const
{
   return nombre_mat ;
}
//----------------------------------------------------------------------

void Material::activar(  )
{
   

}

//**********************************************************************

FuenteLuz::FuenteLuz(const VectorRGB& p_color) {
  // CError();

  if (trazam) cout << "creando fuente de luz." << endl << flush;

  color_ambiente = p_color;
  color_difuso = p_color;
  color_especular = p_color;

  indice_fuente = -1;   // la marca como no activable hasta que no se le asigne
                     // índice

  // CError();
}

//----------------------------------------------------------------------

void FuenteLuz::preActivar() {

   glEnable(GL_LIGHT0 + indice_fuente);

   // Fijamos los colores de la fuente de luz
   glLightfv(GL_LIGHT0 + indice_fuente, GL_AMBIENT, color_ambiente);
   glLightfv(GL_LIGHT0 + indice_fuente, GL_DIFFUSE, color_difuso);
   glLightfv(GL_LIGHT0 + indice_fuente, GL_SPECULAR, color_especular);
   
}

//----------------------------------------------------------------------

FuenteDireccional::FuenteDireccional(GLfloat p_longitud_inicial,
                                     GLfloat p_latitud_inicial,
                                     const VectorRGB& p_color)
    : FuenteLuz(p_color) {
  longitud = longitud_inicial = p_latitud_inicial;
  latitud = latitud_inicial = p_latitud_inicial;
}

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

  // if ( actualizar )
  //   cout << "fuente de luz cambiada: longi == " << longi << ", lati == " <<
  //   lati << endl << flush ;
  return actualizar;
}

FuentePosicional::FuentePosicional(const Tupla3f& p_posicion,
                                   const VectorRGB& p_color)
    : FuenteLuz(p_color) {
  posicion = p_posicion;
}

void FuentePosicional::activar() {
  if (indice_fuente == -1) return;  // La fuente no se puede activar

  Tupla4f posicion_homogenea = {posicion(X), posicion(Y), posicion(Z), 1.0};

  preActivar();
  glLightfv(GL_LIGHT0 + indice_fuente, GL_POSITION, posicion_homogenea);
}

//**********************************************************************

ColFuentesLuz::ColFuentesLuz()
{
   max_num_fuentes = -1 ;
}
//----------------------------------------------------------------------

void ColFuentesLuz::insertar( FuenteLuz * pf )  // inserta una nueva
{
   assert( pf != nullptr );

   pf->indice_fuente = vpf.size() ;
   vpf.push_back( pf ) ;
}
//----------------------------------------------------------------------
// activa una colección de fuentes de luz

void ColFuentesLuz::activar(unsigned id_prog) {
  glEnable(GL_LIGHTING);

  for (unsigned i = 0; i < vpf.size(); i++) vpf[i]->activar();

  for (unsigned i = vpf.size(); i < max_num_fuentes; i++)
    glDisable(GL_LIGHT0 + i);
}
//----------------------------------------------------------------------
FuenteLuz * ColFuentesLuz::ptrFuente( unsigned i )
{
   assert(i < vpf.size()) ;
   return vpf[i] ;
}
//----------------------------------------------------------------------
ColFuentesLuz::~ColFuentesLuz()
{
   for( unsigned i = 0 ; i < vpf.size() ; i++ )
   {
      assert( vpf[i] != NULL );
      delete vpf[i] ;
      vpf[i] = NULL ;
   }
}
