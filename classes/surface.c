#include "php.h"
#include "zend_interfaces.h"

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include "./common.h"
#include "./exceptions.h"
#include "./surface.h"

static zend_object_handlers php_sdl_surface_handlers;

zend_class_entry *sdlSurface_ce;

/* {{{ php_sdl_surface_create */
static zend_object* php_sdl_surface_create(zend_class_entry *ce)
{
  php_sdl_surface_t *st =
    (php_sdl_surface_t*) ecalloc(1, sizeof(php_sdl_surface_t) + zend_object_properties_size(ce));

  zend_object_std_init(&st->std, ce);
  object_properties_init(&st->std, ce);
  st->std.handlers = &php_sdl_surface_handlers;

  return &st->std;
} /* }}} */

/* {{{ php_sdl_surface_dtor_storage */
static void php_sdl_surface_dtor_storage(zend_object *object)
{
	php_sdl_surface_t *st = php_sdl_surface_from_obj(object);

	SDL_FreeSurface(st->surface);
} /* }}} */

/* {{{ php_sdl_surface_free_storage */
static void php_sdl_surface_free_storage(zend_object *object)
{
	php_sdl_surface_t *st = php_sdl_surface_from_obj(object);

	zend_object_std_dtor(&st->std);
	efree(st);
} /* }}} */

/* {{{ proto Surface Surface::__construct(int width, int height[, int depth = 32 [, int format = SDL\PixelFormat::RGBA32]]) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_surface___construct_info, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, width, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, height, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, depth, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, format, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Surface, __construct)
{
	php_sdl_surface_t *st = php_sdl_surface_fetch(getThis());

	zend_long width;
	zend_long height;
	zend_long depth = 32;
	zend_long format = SDL_PIXELFORMAT_RGBA32;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "ll|l", &width, &height, &depth, &format) != SUCCESS) {
		return;
	}

	st->surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, depth, format);

	if (st->surface == NULL) {
		php_sdl_error(SDL_GetError());
	}
}
/* }}} */

/* {{{ proto Surface Surface::loadBMP(string filename) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_surface_loadBMP_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, file, IS_STRING, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Surface, loadBMP)
{
	php_sdl_surface_t *st;

	zend_string *filename;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &filename) != SUCCESS) {
		return;
	}

	object_init_ex(return_value, sdlSurface_ce);
	st = php_sdl_surface_fetch(return_value);

	st->surface = SDL_LoadBMP(ZSTR_VAL(filename));

	if (st->surface == NULL) {
		zval_ptr_dtor(return_value);
		php_sdl_error(SDL_GetError());
	}
}
/* }}} */

/* {{{ php_sdl_surface_methods */
const zend_function_entry php_sdl_surface_methods[] = {
	PHP_ME(Surface, __construct, php_sdl_surface___construct_info, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(Surface, loadBMP, php_sdl_surface_loadBMP_info, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

PHP_MINIT_FUNCTION(SDL_Surface) /* {{{ */
{
  zend_class_entry ce;

  INIT_NS_CLASS_ENTRY(ce, SDL_NS, "Surface", php_sdl_surface_methods);
  sdlSurface_ce = zend_register_internal_class_ex(&ce, NULL);
  sdlSurface_ce->ce_flags |= ZEND_ACC_FINAL;
  sdlSurface_ce->create_object = php_sdl_surface_create;
  sdlSurface_ce->serialize = zend_class_serialize_deny;
  sdlSurface_ce->unserialize = zend_class_unserialize_deny;

  memcpy(&php_sdl_surface_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  php_sdl_surface_handlers.free_obj = php_sdl_surface_free_storage;
  php_sdl_surface_handlers.dtor_obj = php_sdl_surface_dtor_storage;
  php_sdl_surface_handlers.clone_obj = NULL;

  php_sdl_surface_handlers.offset = XtOffsetOf(php_sdl_surface_t, std);

  return SUCCESS;
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
