#ifndef HAVE_PHP_SDL_WINDOW
#define HAVE_PHP_SDL_WINDOW

#include "php.h"
#include "zend_interfaces.h"

#ifdef _WIN32
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
#endif

#include "./common.h"
#include "./exceptions.h"
#include "./window.h"
#include "./surface.h"

static zend_object_handlers php_sdl_window_handlers;

zend_class_entry *sdlWindow_ce;

/* {{{ php_sdl_window_create */
static zend_object* php_sdl_window_create(zend_class_entry *ce)
{
	php_sdl_window_t *wt = 
		(php_sdl_window_t*) ecalloc(1, sizeof(php_sdl_window_t) + zend_object_properties_size(ce));

	zend_object_std_init(&wt->std, ce);
	object_properties_init(&wt->std, ce);
	wt->std.handlers = &php_sdl_window_handlers;

	return &wt->std;
} /* }}} */

/* {{{ php_sdl_window_dtor_storage */
static void php_sdl_window_dtor_storage(zend_object *object)
{
	php_sdl_window_t *wt = php_sdl_window_from(object);

	SDL_DestroyWindow(wt->window);
} /* }}} */

/* {{{ php_sdl_window_free_storage */
static void php_sdl_window_free_storage(zend_object *object)
{
	php_sdl_window_t *wt = php_sdl_window_from(object);

	zend_object_std_dtor(&wt->std);
	efree(wt);
} /* }}} */

/* {{{ proto Window Window::__construct(string title, int x, int y, int w, int h[, int flags = 0]) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_window___construct_info, 0, 0, 5)
	ZEND_ARG_TYPE_INFO(0, title, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, x, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, w, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, h, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Window, __construct)
{
	php_sdl_window_t *wt = php_sdl_window_fetch(getThis());

	zend_string *title;
	zend_long x;
	zend_long y;
	zend_long w;
	zend_long h;
	zend_long flags = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "Sllll|l", &title, &x, &y, &w, &h, &flags) != SUCCESS) {
		return;
	}

	wt->window = SDL_CreateWindow(ZSTR_VAL(title), x, y, w, h, flags);

	if (wt->window == NULL) {
		php_sdl_error(SDL_GetError());
	}
} /* }}} */

/* {{{ proto int Window::getID() */
PHP_METHOD(Window, getID)
{
	php_sdl_window_t *wt = php_sdl_window_fetch(getThis());
	RETURN_LONG(SDL_GetWindowID(wt->window));
} /* }}} */

/* {{{ proto int Window::getPixelFormat() */
PHP_METHOD(Window, getPixelFormat)
{
	php_sdl_window_t *wt = php_sdl_window_fetch(getThis());
	int retval = SDL_GetWindowPixelFormat(wt->window);
	if (retval == SDL_PIXELFORMAT_UNKNOWN) {
		php_sdl_error(SDL_GetError());
	}
	RETURN_LONG(retval);
} /* }}} */

/* {{{ proto Window Window::findFromID(int id) */
ZEND_BEGIN_ARG_INFO_EX(php_sdl_window_findFromID_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, id, IS_LONG, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(Window, findFromID)
{
	php_sdl_window_t *wt;
	
	zend_long id;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &id) != SUCCESS) {
		return;
	}
	
	object_init_ex(return_value, sdlWindow_ce);
	wt = php_sdl_window_fetch(return_value);

	wt->window = SDL_GetWindowFromID(id);
	if (wt->window == NULL) {
		zval_ptr_dtor(return_value);
		php_sdl_error(SDL_GetError());
	}
} /* }}} */

/* {{{ proto array Window::getSize() */
PHP_METHOD(Window, getSize)
{
	php_sdl_window_t *wt = php_sdl_window_fetch(getThis());

	int w;
	int h;

	SDL_GetWindowSize(wt->window, &w, &h);

	object_init(return_value);
	add_property_long(return_value, "w", w);
	add_property_long(return_value, "h", h);
} /* }}} */

/* {{{ proto array Window::getPosition() */
PHP_METHOD(Window, getPosition)
{
	php_sdl_window_t *wt = php_sdl_window_fetch(getThis());

	int x;
	int y;

	SDL_GetWindowPosition(wt->window, &x, &y);

	object_init(return_value);
	add_property_long(return_value, "x", x);
	add_property_long(return_value, "y", y);
} /* }}} */

/* {{{ proto array Window::getSurface() */
PHP_METHOD(Window, getSurface)
{
	php_sdl_window_t *wt = php_sdl_window_fetch(getThis());

	SDL_Surface *screen = SDL_GetWindowSurface(wt->window);
	if (screen == NULL) {
		php_sdl_error(SDL_GetError());
	}

	object_init_ex(return_value, sdlSurface_ce);
	php_sdl_surface_t *st = php_sdl_surface_fetch(return_value);
	st->surface = screen;
} /* }}} */

/* {{{ php_sdl_window_methods */
const zend_function_entry php_sdl_window_methods[] = {
	PHP_ME(Window, __construct, php_sdl_window___construct_info, ZEND_ACC_CTOR|ZEND_ACC_PUBLIC)
	PHP_ME(Window, findFromID, php_sdl_window_findFromID_info, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Window, getID, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Window, getPixelFormat, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Window, getSize, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Window, getPosition, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Window, getSurface, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

PHP_MINIT_FUNCTION(SDL_Window) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, SDL_NS, "Window", php_sdl_window_methods);
	sdlWindow_ce = zend_register_internal_class_ex(&ce, NULL);
	sdlWindow_ce->ce_flags |= ZEND_ACC_FINAL;
	sdlWindow_ce->create_object = php_sdl_window_create;
	sdlWindow_ce->serialize = zend_class_serialize_deny;
	sdlWindow_ce->unserialize = zend_class_unserialize_deny;

	memcpy(&php_sdl_window_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_sdl_window_handlers.free_obj = php_sdl_window_free_storage;
	php_sdl_window_handlers.dtor_obj = php_sdl_window_dtor_storage;
	php_sdl_window_handlers.clone_obj = NULL;

	php_sdl_window_handlers.offset = XtOffsetOf(php_sdl_window_t, std);

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
