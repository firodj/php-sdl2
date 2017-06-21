#include "php.h"
#include "zend_interfaces.h"

#ifdef _WIN32
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
#endif

#include "./common.h"
#include "./exceptions.h"
#include "./renderer.h"
#include "./surface.h"
#include "./texture.h"

static zend_object_handlers php_sdl_texture_handlers;

zend_class_entry *sdlTexture_ce;

/* {{{ php_sdl_texture_create */
static zend_object* php_sdl_texture_create(zend_class_entry *ce)
{
	php_sdl_texture_t *tt = 
		(php_sdl_texture_t*) ecalloc(1, sizeof(php_sdl_texture_t) + zend_object_properties_size(ce));

	zend_object_std_init(&tt->std, ce);
	object_properties_init(&tt->std, ce);
	tt->std.handlers = &php_sdl_texture_handlers;

	return &tt->std;
} /* }}} */

/* {{{ php_sdl_texture_dtor_storage */
static void php_sdl_texture_dtor_storage(zend_object *object)
{
	php_sdl_texture_t *tt = php_sdl_texture_from_obj(object);

	SDL_DestroyTexture(tt->texture);
} /* }}} */

/* {{{ php_sdl_texture_free_storage */
static void php_sdl_texture_free_storage(zend_object *object)
{
	php_sdl_texture_t *tt = php_sdl_texture_from_obj(object);

	zend_object_std_dtor(&tt->std);
	efree(tt);
} /* }}} */

/* {{{ proto Texture Texture::__construct(Renderer renderer, int format, int access, int w, int h) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_texture___construct_info, 0, 0, 5)
	ZEND_ARG_OBJ_INFO(0, renderer, SDL\\Renderer, 0)
	ZEND_ARG_TYPE_INFO(0, format, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, access, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, w, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, h, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Texture, __construct)
{
	php_sdl_texture_t *tt = php_sdl_texture_fetch(getThis());
	php_sdl_renderer_t *rt;

	zval *renderer;
	zend_long format;
	zend_long access;
	zend_long w;
	zend_long h;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "Ollll", &renderer, sdlRenderer_ce, &format, &access, &w, &h) != SUCCESS) {
		return;
	}

	rt = php_sdl_renderer_fetch(renderer);

	tt->texture = SDL_CreateTexture(rt->renderer, format, access, w, h);

	if (tt->texture == NULL) {
		php_sdl_error(SDL_GetError());
	}
} /* }}} */

/* {{{ proto Texture Texture::createFromSurface(renderer, surface) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_texture_createFromSurface_info, 0, 0, 2)
	ZEND_ARG_OBJ_INFO(0, renderer, SDL\\Renderer, 0)
	ZEND_ARG_OBJ_INFO(0, surface, SDL\\Surface, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Texture, createFromSurface)
{
	php_sdl_texture_t *tt;
	php_sdl_renderer_t *rt;
	php_sdl_surface_t *st;
	
	zval *renderer;
	zval *surface;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "OO", &renderer, sdlRenderer_ce, &surface, sdlSurface_ce) != SUCCESS) {
		RETURN_FALSE;
	}

	object_init_ex(return_value, sdlTexture_ce);
	tt = php_sdl_texture_fetch(return_value);
	rt = php_sdl_renderer_fetch(renderer);
	st = php_sdl_surface_fetch(surface);

	tt->texture = SDL_CreateTextureFromSurface(rt->renderer, st->surface);
	
	if (tt->texture == NULL) {
		zval_ptr_dtor(return_value);
		RETURN_FALSE;
	}
}
/* }}} */

/* {{{ php_sdl_texture_methods */
const zend_function_entry php_sdl_texture_methods[] = {
	PHP_ME(Texture, __construct, php_sdl_texture___construct_info, ZEND_ACC_PUBLIC)
	PHP_ME(Texture, createFromSurface, php_sdl_texture_createFromSurface_info, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
}; /* }}} */


PHP_MINIT_FUNCTION(SDL_Texture) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, SDL_NS, "Texture", php_sdl_texture_methods);
	sdlTexture_ce = zend_register_internal_class_ex(&ce, NULL);

	sdlTexture_ce->ce_flags |= ZEND_ACC_FINAL;
	sdlTexture_ce->create_object = php_sdl_texture_create;
	sdlTexture_ce->serialize = zend_class_serialize_deny;
	sdlTexture_ce->unserialize = zend_class_unserialize_deny;

	memcpy(&php_sdl_texture_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_sdl_texture_handlers.free_obj = php_sdl_texture_free_storage;
	php_sdl_texture_handlers.dtor_obj = php_sdl_texture_dtor_storage;
	php_sdl_texture_handlers.clone_obj = NULL;

	php_sdl_texture_handlers.offset = XtOffsetOf(php_sdl_texture_t, std);

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
