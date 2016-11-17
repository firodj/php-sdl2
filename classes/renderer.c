#ifndef HAVE_PHP_SDL_WINDOW
#define HAVE_PHP_SDL_WINDOW

#include "php.h"
#include "zend_interfaces.h"

#include <SDL2/SDL.h>

#include <classes/common.h>
#include <classes/exceptions.h>
#include <classes/renderer.h>
#include <classes/window.h>

static zend_object_handlers php_sdl_renderer_handlers;

zend_class_entry *sdlRenderer_ce;

 /* {{{ php_sdl_renderer_create */
static zend_object* php_sdl_renderer_create(zend_class_entry *ce)
{
	php_sdl_renderer_t *rt = 
		(php_sdl_renderer_t*) ecalloc(1, sizeof(php_sdl_renderer_t) + zend_object_properties_size(ce));

	zend_object_std_init(&rt->std, ce);
	object_properties_init(&rt->std, ce);
	rt->std.handlers = &php_sdl_renderer_handlers;

	return &rt->std;
} /* }}} */

 /* {{{ php_sdl_renderer_dtor_storage */
static void php_sdl_renderer_dtor_storage(zend_object *object)
{
	php_sdl_renderer_t *rt = php_sdl_renderer_from(object);

	SDL_DestroyRenderer(rt->renderer);
} /* }}} */

/* {{{ php_sdl_renderer_free_storage */
static void php_sdl_renderer_free_storage(zend_object *object)
{
	php_sdl_renderer_t *rt = php_sdl_renderer_from(object);

	zend_object_std_dtor(&rt->std);
	efree(rt);
} /* }}} */

/* {{{ proto Renderer Renderer::__construct(Window window[, int index = -1[, int flags = 0]]) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_renderer___construct_info, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, window, SDL\\Window, 0)
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Renderer, __construct) 
{
	php_sdl_renderer_t *rt = php_sdl_renderer_fetch(getThis());
	php_sdl_window_t *wt;

	zval *window;
	zend_long index = -1;
	zend_long flags = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O|ll", &window, sdlWindow_ce, &index, &flags) != SUCCESS) {
		return;
	}

	wt = php_sdl_window_fetch(window);

	rt->renderer = SDL_CreateRenderer(wt->window, index, flags);

	if (rt->renderer == NULL) {
		php_sdl_error(SDL_GetError());
	}
} /* }}} */

/* {{{ php_sdl_renderer_methods[] */
const zend_function_entry php_sdl_renderer_methods[] = {
	PHP_ME(Renderer, __construct, php_sdl_renderer___construct_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

PHP_MINIT_FUNCTION(SDL_Renderer) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, SDL_NS, "Renderer", php_sdl_renderer_methods);
	sdlRenderer_ce = zend_register_internal_class_ex(&ce, NULL);
	sdlRenderer_ce->ce_flags |= ZEND_ACC_FINAL;
	sdlRenderer_ce->create_object = php_sdl_renderer_create;
	sdlRenderer_ce->serialize = zend_class_serialize_deny;
	sdlRenderer_ce->unserialize = zend_class_unserialize_deny;

	memcpy(&php_sdl_renderer_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_sdl_renderer_handlers.dtor_obj = php_sdl_renderer_dtor_storage;
	php_sdl_renderer_handlers.free_obj = php_sdl_renderer_free_storage;
	php_sdl_renderer_handlers.clone_obj = NULL;

	php_sdl_renderer_handlers.offset = XtOffsetOf(php_sdl_renderer_t, std);

	return SUCCESS;
} /* }}} */

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
