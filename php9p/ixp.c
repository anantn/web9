/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2007 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header,v 1.16.2.1.2.1 2007/01/01 19:32:09 iliaa Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_ixp.h"

/* If you declare any globals in php_ixp.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(ixp)
*/

/* True global resources - no need for thread safety here */
static int le_ixp;

extern zend_class_entry *zend_ce_iterator;
PHP_METHOD(IXPClient, __construct);

static
ZEND_BEGIN_ARG_INFO(arginfo_IXPClient_construct, 0)
	ZEND_ARG_INFO(0, address_or_fd)
ZEND_END_ARG_INFO();

static zend_function_entry IXPClient_functions[] = {
	PHP_ME(IXPClient, __construct, arginfo_IXPClient_construct, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

static zend_object_handlers IXPClient_handlers;
static zend_class_entry *IXPClient_ce;

typedef struct _IXPClient_object {
	zend_object	std;
	struct IxpClient *obj;
} IXPClient_object;

/* {{{ IXPClient_object_free */
static void IXPClient_object_free(void *object TSRMLS_DC)
{
	IXPClient_object *intern = (IXPClient_object *)object;
	
	zend_hash_destroy(intern->std.properties);
	FREE_HASHTABLE(intern->std.properties);

	if (intern->obj) {
		ixp_unmount(intern->obj);
	}

	efree(object);
}
/* }}} */

/* {{{ IXPClient_object_new_ex */
static zend_object_value IXPClient_object_new_ex(zend_class_entry *class_type, _9pClient_object **obj TSRMLS_DC)
{
	zend_object_value retval;
	IXPClient_object *intern;
	zval *tmp;

	intern = emalloc(sizeof(IXPClient_object));
	memset(intern, 0, sizeof(IXPClient_object));
	intern->std.ce = class_type;
	*obj = intern;

	ALLOC_HASHTABLE(intern->std.properties);
	zend_hash_init(intern->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
	zend_hash_copy(intern->std.properties, &class_type->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));

	retval.handle = zend_objects_store_put(intern, NULL, (zend_objects_free_object_storage_t) IXPClient_object_free, NULL TSRMLS_CC);
	retval.handlers = &IXPClient_handlers;
	return retval;
}
/* }}} */

/* {{{ IXPClient_object_new */
static zend_object_value IXPClient_object_new(zend_class_entry *class_type TSRMLS_DC)
{
	IXPClient_object *tmp;
	return IXPClient_object_new_ex(class_type, &tmp TSRMLS_CC);
}
/* }}} */

/* {{{ IXPClient_get_ce */
#if MBO_0
static zend_class_entry *IXPClient_get_ce(zval *object TSRMLS_DC)
{
	return IXPClient_ce;
}
#endif
/* }}} */

#if PHP_MAJOR_VERSION >= 6 | (PHP_MAJOR_VERSION == 5 & PHP_MINOR_VERSION >= 2)
#define EXTSRM TSRMLS_C
#else
#define EXTSRM
#endif

/* {{{ proto void IXPClient::__construct(string path) */
PHP_METHOD(IXPClient, __construct)
{
	zval *object = getThis();
	IXPClient_object *intern;
	char *path;
	long len;

	php_set_error_handling(EH_THROW, zend_exception_get_default(EXTSRM) TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &path, &len) == FAILURE) {
		php_set_error_handling(EH_NORMAL, NULL TSRMLS_CC);
		return;
	}

	intern = (IXPClient_object*)zend_object_store_get_object(object TSRMLS_CC);
	intern->obj = ixp_mount(path);
}
/* }}} */


/* {{{ ixp_functions[]
 *
 * Every user visible function must have an entry in ixp_functions[].
 */
zend_function_entry ixp_functions[] = {
	PHP_FE(confirm_ixp_compiled,	NULL)		/* For testing, remove later. */
	{NULL, NULL, NULL}	/* Must be the last line in ixp_functions[] */
};
/* }}} */

/* {{{ ixp_module_entry
 */
zend_module_entry ixp_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"ixp",
	ixp_functions,
	PHP_MINIT(ixp),
	PHP_MSHUTDOWN(ixp),
	PHP_RINIT(ixp),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(ixp),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(ixp),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_IXP
ZEND_GET_MODULE(ixp)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("ixp.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_ixp_globals, ixp_globals)
    STD_PHP_INI_ENTRY("ixp.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_ixp_globals, ixp_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_ixp_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_ixp_init_globals(zend_ixp_globals *ixp_globals)
{
	ixp_globals->global_value = 0;
	ixp_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(ixp)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/

	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "IXPClient", IXPClient_functions);
	IXPClient_ce = zend_register_internal_class(&ce TSRMLS_CC);
	IXPClient_ce->create_object = IXPClient_object_new;
	zend_class_implements(IXPClient_ce TSRMLS_CC, 1, zend_ce_iterator);
	memcpy(&IXPClient_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(ixp)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(ixp)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(ixp)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(ixp)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "ixp support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_ixp_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_ixp_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char string[256];

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = sprintf(string, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "ixp", arg);
	RETURN_STRINGL(string, len, 1);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
