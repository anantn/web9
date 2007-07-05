/*
   +----------------------------------------------------------------------+
   | This library is free software; you can redistribute it and/or        |
   | modify it under the terms of the GNU Lesser General Public           |
   | License as published by the Free Software Foundation; either         |
   | version 2.1 of the License, or (at your option) any later version.   | 
   |                                                                      |
   | This library is distributed in the hope that it will be useful,      |
   | but WITHOUT ANY WARRANTY; without even the implied warranty of       |
   | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU    |
   | Lesser General Public License for more details.                      | 
   |                                                                      |
   | You should have received a copy of the GNU Lesser General Public     |
   | License in the file LICENSE along with this library;                 |
   | if not, write to the                                                 | 
   |                                                                      |
   |   Free Software Foundation, Inc.,                                    |
   |   59 Temple Place, Suite 330,                                        |
   |   Boston, MA  02111-1307  USA                                        |
   +----------------------------------------------------------------------+
   | Authors: Anant Narayanan <anant@php.net>                             |
   +----------------------------------------------------------------------+
*/

/* $ Id: $ */ 

#include "php_ixp.h"

#if HAVE_IXP

/* {{{ Class entries */
static zend_class_entry * IxpCallbacks_ce_ptr = NULL;
static zend_class_entry * IxpServerCallbacks_ce_ptr = NULL;
static zend_class_entry * IxpClient_ce_ptr = NULL;
static zend_class_entry * IxpCFid_ce_ptr = NULL;
static zend_class_entry * IxpStat_ce_ptr = NULL;
static zend_class_entry * IxpQid_ce_ptr = NULL;
static zend_class_entry * IxpConn_ce_ptr = NULL;
static zend_class_entry * IxpFid_ce_ptr = NULL;
static zend_class_entry * IxpRequest_ce_ptr = NULL;
static zend_class_entry * IxpFcall_ce_ptr = NULL;
static zend_class_entry * IxpServer_ce_ptr = NULL;
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(ixp)
{
	class_init_IxpClient();
	class_init_IxpCFid();
	class_init_IxpStat();
	class_init_IxpQid();
	class_init_IxpConn();
	class_init_IxpFid();
	class_init_IxpRequest();
	class_init_IxpFcall();
	class_init_IxpServer();
	interface_init_IxpCallbacks();
	interface_init_IxpServerCallbacks();

	return SUCCESS;
}
/* }}} */


/* {{{ Helper functions */
static zval * object_instantiate(zend_class_entry *ce, zval *object TSRMLS_DC)
{
	if (!object)
		ALLOC_ZVAL(object);

	Z_TYPE_P(object) = IS_OBJECT;
	object_init_ex(object, ce);
	object->refcount = 1;
	object->is_ref = 0;
	return object;
}

/* }}} */

/* {{{ Interface IxpCallbacks */
static zend_function_entry IxpCallbacks_methods[] = {
	ZEND_FENTRY(attach, NULL, IxpCallbacks__attach_args, ZEND_ACC_ABSTRACT |  ZEND_ACC_INTERFACE | ZEND_ACC_PUBLIC)
	ZEND_FENTRY(clunk, NULL, IxpCallbacks__clunk_args, ZEND_ACC_ABSTRACT |  ZEND_ACC_INTERFACE | ZEND_ACC_PUBLIC)
	ZEND_FENTRY(create, NULL, IxpCallbacks__create_args, ZEND_ACC_ABSTRACT |  ZEND_ACC_INTERFACE | ZEND_ACC_PUBLIC)
	ZEND_FENTRY(flush, NULL, IxpCallbacks__flush_args, ZEND_ACC_ABSTRACT |  ZEND_ACC_INTERFACE | ZEND_ACC_PUBLIC)
	ZEND_FENTRY(open, NULL, IxpCallbacks__open_args, ZEND_ACC_ABSTRACT |  ZEND_ACC_INTERFACE | ZEND_ACC_PUBLIC)
	ZEND_FENTRY(read, NULL, IxpCallbacks__read_args, ZEND_ACC_ABSTRACT |  ZEND_ACC_INTERFACE | ZEND_ACC_PUBLIC)
	ZEND_FENTRY(remove, NULL, IxpCallbacks__remove_args, ZEND_ACC_ABSTRACT |  ZEND_ACC_INTERFACE | ZEND_ACC_PUBLIC)
	ZEND_FENTRY(stat, NULL, IxpCallbacks__stat_args, ZEND_ACC_ABSTRACT |  ZEND_ACC_INTERFACE | ZEND_ACC_PUBLIC)
	ZEND_FENTRY(walk, NULL, IxpCallbacks__walk_args, ZEND_ACC_ABSTRACT |  ZEND_ACC_INTERFACE | ZEND_ACC_PUBLIC)
	ZEND_FENTRY(write, NULL, IxpCallbacks__write_args, ZEND_ACC_ABSTRACT |  ZEND_ACC_INTERFACE | ZEND_ACC_PUBLIC)
	ZEND_FENTRY(freefid, NULL, IxpCallbacks__freefid_args, ZEND_ACC_ABSTRACT |  ZEND_ACC_INTERFACE | ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};

static void interface_init_IxpCallbacks(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpCallbacks", IxpCallbacks_methods);
	IxpCallbacks_ce_ptr = zend_register_internal_interface(&ce TSRMLS_CC);
}
/* }}} Interface IxpCallbacks */

/* {{{ Interface IxpServerCallbacks */
static zend_function_entry IxpServerCallbacks_methods[] = {
	ZEND_FENTRY(read, NULL, IxpServerCallbacks__read_args, ZEND_ACC_ABSTRACT |  ZEND_ACC_INTERFACE | ZEND_ACC_PUBLIC)
	ZEND_FENTRY(close, NULL, IxpServerCallbacks__close_args, ZEND_ACC_ABSTRACT |  ZEND_ACC_INTERFACE | ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};

static void interface_init_IxpServerCallbacks(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpServerCallbacks", IxpServerCallbacks_methods);
	IxpServerCallbacks_ce_ptr = zend_register_internal_interface(&ce TSRMLS_CC);
}
/* }}} Interface IxpServerCallbacks */


/* {{{ Class IxpClient */

static zend_object_handlers PHP_IxpClient_handlers;

static void PHP_IxpClient_object_free(void *object TSRMLS_DC)
{
	PHP_IxpClient *intern = (PHP_IxpClient *)object;

	zend_hash_destroy(intern->obj.properties);
	FREE_HASHTABLE(intern->obj.properties);

	efree(object);
}

static zend_object_value PHP_IxpClient_object_new_ex(zend_class_entry *class_type, PHP_IxpClient **object TSRMLS_DC)
{
	zend_object_value retval;
	PHP_IxpClient *intern;
	zval *tmp;

	intern = malloc(sizeof(PHP_IxpClient));
	memset(intern, 0, sizeof(PHP_IxpClient));
	intern->obj.ce = class_type;
	*object = intern;

	ALLOC_HASHTABLE(intern->obj.properties);
	zend_hash_init(intern->obj.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
	zend_hash_copy(intern->obj.properties, &class_type->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));

	retval.handle = zend_objects_store_put(intern, NULL, (zend_objects_free_object_storage_t) PHP_IxpClient_object_free, NULL TSRMLS_CC);
	retval.handlers = &PHP_IxpClient_handlers;
	return retval;
}

static zend_object_value PHP_IxpClient_object_new(zend_class_entry *class_type TSRMLS_DC)
{
	PHP_IxpClient *tmp;
	return PHP_IxpClient_object_new_ex(class_type, &tmp TSRMLS_DC);
}

/* {{{ proto void construct(string path) */
PHP_METHOD(IxpClient, __construct)
{
	zend_class_entry * _this_ce;
	zval * _this_zval = NULL;
	IxpClient *client;

	char * path = NULL;
	int path_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &path, &path_len) == FAILURE) { 
		return;
	}

	client = ixp_mount(path);
	if (client == nil) {
		php_error(E_WARNING, ixp_errbuf());
		RETURN_FALSE;
	}

	_this_zval = getThis();
    _this_ce = Z_OBJCE_P(_this_zval);
	PHP_IxpClient *object = FETCH_IxpClient(_this_zval);
	object->ptr = client;

	PROP_SET_LONG(mSize, client->msize);
	PROP_SET_LONG(lastFid, client->lastfid);
}
/* }}} __construct */

/* {{{ proto void unmount() */
PHP_METHOD(IxpClient, unmount)
{
	IxpClient *client;

	if(ZEND_NUM_ARGS() != 0)
        WRONG_PARAM_COUNT;
		
	PHP_IxpClient *object = FETCH_IxpClient(this_ptr);
	ixp_unmount(object->ptr);
	RETURN_TRUE;
}
/* }}} unmount */

/* {{{ proto object create(string name [, string permissions [, string mode]]) */
PHP_METHOD(IxpClient, create)
{
	zend_class_entry * _this_ce;
	zval * _this_zval = NULL;
	const char * name = NULL;
	int name_len = 0;
	const char * permissions = NULL;
	int permissions_len = 0;
	const char * mode = NULL;
	int mode_len = 0;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os|ss", &_this_zval, IxpClient_ce_ptr, &name, &name_len, &permissions, &permissions_len, &mode, &mode_len) == FAILURE) {
		return;
	}

	_this_ce = Z_OBJCE_P(_this_zval);

	php_error(E_WARNING, "create: not yet implemented"); RETURN_FALSE;

	object_init(return_value);
}
/* }}} create */

/* {{{ proto object open(string name [, string mode]) */
PHP_METHOD(IxpClient, open)
{
	zend_class_entry * _this_ce;
	zval * _this_zval = NULL;
	const char * name = NULL;
	int name_len = 0;
	const char * mode = NULL;
	int mode_len = 0;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os|s", &_this_zval, IxpClient_ce_ptr, &name, &name_len, &mode, &mode_len) == FAILURE) {
		return;
	}

	_this_ce = Z_OBJCE_P(_this_zval);

	php_error(E_WARNING, "open: not yet implemented"); RETURN_FALSE;

	object_init(return_value);
}
/* }}} open */

/* {{{ proto bool remove(string path) */
PHP_METHOD(IxpClient, remove)
{
	char * path = NULL;
	int path_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &path, &path_len) == FAILURE) {
		return;
	}

	PHP_IxpClient *object = FETCH_IxpClient(this_ptr);

	if (ixp_remove(object->ptr, path) == 0) {
		php_error(E_WARNING, ixp_errbuf());
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
/* }}} remove */

/* {{{ proto object stat(string path) */
PHP_METHOD(IxpClient, stat)
{
	IxpStat *stat;
	char * path = NULL;
	int path_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &path, &path_len) == FAILURE) {
		return;
	}

	PHP_IxpClient *object = FETCH_IxpClient(this_ptr);

	stat = ixp_stat(object->ptr, path);
	if (stat == nil) {
		php_error(E_WARNING, ixp_errbuf());
		RETURN_FALSE;
	}

	object_init(return_value);
}
/* }}} stat */

static zend_function_entry IxpClient_methods[] = {
	PHP_ME(IxpClient, __construct, NULL, /**/ZEND_ACC_PUBLIC)
	PHP_ME(IxpClient, unmount, NULL, /**/ZEND_ACC_PUBLIC)
	PHP_ME(IxpClient, create, IxpClient__create_args, /**/ZEND_ACC_PUBLIC)
	PHP_ME(IxpClient, open, IxpClient__open_args, /**/ZEND_ACC_PUBLIC)
	PHP_ME(IxpClient, remove, IxpClient__remove_args, /**/ZEND_ACC_PUBLIC)
	PHP_ME(IxpClient, stat, IxpClient__stat_args, /**/ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};

static void class_init_IxpClient(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpClient", IxpClient_methods);
	IxpClient_ce_ptr = zend_register_internal_class(&ce);
	IxpClient_ce_ptr->create_object = PHP_IxpClient_object_new;
	memcpy(&PHP_IxpClient_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	PHP_IxpClient_handlers.clone_obj = NULL;

	zend_declare_property_long(IxpClient_ce_ptr, "mSize", sizeof("mSize")-1, 0, ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_long(IxpClient_ce_ptr, "lastFid", sizeof("lastFid")-1, 0, ZEND_ACC_PUBLIC TSRMLS_CC);
}

/* }}} Class IxpClient */


/* {{{ Class IxpCFid */

/* {{{ proto int read(string buffer [, int count]) */
PHP_METHOD(IxpCFid, read)
{
	zend_class_entry * _this_ce;
	zval * _this_zval = NULL;
	const char * buffer = NULL;
	int buffer_len = 0;
	long count = 0;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os|l", &_this_zval, IxpCFid_ce_ptr, &buffer, &buffer_len, &count) == FAILURE) {
		return;
	}

	_this_ce = Z_OBJCE_P(_this_zval);

	php_error(E_WARNING, "read: not yet implemented"); RETURN_FALSE;

	RETURN_LONG(0);
}
/* }}} read */

/* {{{ proto int write(string buffer) */
PHP_METHOD(IxpCFid, write)
{
	zend_class_entry * _this_ce;
	zval * _this_zval = NULL;
	const char * buffer = NULL;
	int buffer_len = 0;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &_this_zval, IxpCFid_ce_ptr, &buffer, &buffer_len) == FAILURE) {
		return;
	}

	_this_ce = Z_OBJCE_P(_this_zval);

	php_error(E_WARNING, "write: not yet implemented"); RETURN_FALSE;

	RETURN_LONG(0);
}
/* }}} write */

/* {{{ proto int close() */
PHP_METHOD(IxpCFid, close)
{
	zend_class_entry * _this_ce;
	zval * _this_zval = NULL;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &_this_zval, IxpCFid_ce_ptr) == FAILURE) {
		return;
	}

	_this_ce = Z_OBJCE_P(_this_zval);

	php_error(E_WARNING, "close: not yet implemented"); RETURN_FALSE;

	RETURN_LONG(0);
}
/* }}} close */

static zend_function_entry IxpCFid_methods[] = {
	PHP_ME(IxpCFid, read, IxpCFid__read_args, /**/ZEND_ACC_PUBLIC)
	PHP_ME(IxpCFid, write, IxpCFid__write_args, /**/ZEND_ACC_PUBLIC)
	PHP_ME(IxpCFid, close, NULL, /**/ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};

static void class_init_IxpCFid(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpCFid", IxpCFid_methods);
	IxpCFid_ce_ptr = zend_register_internal_class(&ce);

	zend_declare_property_long(IxpCFid_ce_ptr, 
		"fid", 3, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpCFid_ce_ptr, 
		"mode", 4, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpCFid_ce_ptr, 
		"open", 4, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpCFid_ce_ptr, 
		"iounit", 6, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpCFid_ce_ptr, 
		"offset", 6, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_null(IxpCFid_ce_ptr, 
		"qid", 3, 
		ZEND_ACC_PUBLIC TSRMLS_DC);
}
/* }}} Class IxpCFid */


/* {{{ Class IxpStat */

static zend_function_entry IxpStat_methods[] = {
	{ NULL, NULL, NULL }
};

static void class_init_IxpStat(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpStat", IxpStat_methods);
	IxpStat_ce_ptr = zend_register_internal_class(&ce);

	zend_declare_property_long(IxpStat_ce_ptr, 
		"type", 4, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpStat_ce_ptr, 
		"device", 6, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpStat_ce_ptr, 
		"mode", 4, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpStat_ce_ptr, 
		"aTime", 5, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpStat_ce_ptr, 
		"mTime", 5, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpStat_ce_ptr, 
		"length", 6, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_string(IxpStat_ce_ptr, 
		"name", 4, "", 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_string(IxpStat_ce_ptr, 
		"uid", 3, "", 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_string(IxpStat_ce_ptr, 
		"gid", 3, "", 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_string(IxpStat_ce_ptr, 
		"muid", 4, "", 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_null(IxpStat_ce_ptr, 
		"qid", 3, 
		ZEND_ACC_PUBLIC TSRMLS_DC);
}
/* }}} Class IxpStat */


/* {{{ Class IxpQid */
static zend_function_entry IxpQid_methods[] = {
	{ NULL, NULL, NULL }
};

static void class_init_IxpQid(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpQid", IxpQid_methods);
	IxpQid_ce_ptr = zend_register_internal_class(&ce);

	zend_declare_property_long(IxpQid_ce_ptr, 
		"type", 4, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpQid_ce_ptr, 
		"version", 7, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_string(IxpQid_ce_ptr, 
		"path", 4, "", 
		ZEND_ACC_PUBLIC TSRMLS_DC);
}
/* }}} Class IxpQid */


/* {{{ Class IxpConn */

/* {{{ proto void serve() */
PHP_METHOD(IxpConn, serve)
{
	zend_class_entry * _this_ce;
	zval * _this_zval = NULL;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &_this_zval, IxpConn_ce_ptr) == FAILURE) {
		return;
	}

	_this_ce = Z_OBJCE_P(_this_zval);

	php_error(E_WARNING, "serve: not yet implemented"); RETURN_FALSE;

}
/* }}} serve */

static zend_function_entry IxpConn_methods[] = {
	PHP_ME(IxpConn, serve, NULL, /**/ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};

static void class_init_IxpConn(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpConn", IxpConn_methods);
	IxpConn_ce_ptr = zend_register_internal_class(&ce);

	zend_declare_property_long(IxpConn_ce_ptr, 
		"closed", 6, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_null(IxpConn_ce_ptr, 
		"running", 7, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_null(IxpConn_ce_ptr, 
		"fd", 2, 
		ZEND_ACC_PUBLIC TSRMLS_DC);
}
/* }}} Class IxpConn */


/* {{{ Class IxpFid */
static zend_function_entry IxpFid_methods[] = {
	{ NULL, NULL, NULL }
};

static void class_init_IxpFid(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFid", IxpFid_methods);
	IxpFid_ce_ptr = zend_register_internal_class(&ce);

	zend_declare_property_null(IxpFid_ce_ptr, 
		"qid", 3, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_string(IxpFid_ce_ptr, 
		"uid", 3, "", 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFid_ce_ptr, 
		"fid", 3, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFid_ce_ptr, 
		"omode", 5, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);
}
/* }}} Class IxpFid */


/* {{{ Class IxpRequest */

/* {{{ proto void respond(string error) */
PHP_METHOD(IxpRequest, respond)
{
	zend_class_entry * _this_ce;
	zval * _this_zval = NULL;
	const char * error = NULL;
	int error_len = 0;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &_this_zval, IxpRequest_ce_ptr, &error, &error_len) == FAILURE) {
		return;
	}

	_this_ce = Z_OBJCE_P(_this_zval);

	php_error(E_WARNING, "respond: not yet implemented"); RETURN_FALSE;

}
/* }}} respond */

static zend_function_entry IxpRequest_methods[] = {
	PHP_ME(IxpRequest, respond, IxpRequest__respond_args, /**/ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};

static void class_init_IxpRequest(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpRequest", IxpRequest_methods);
	IxpRequest_ce_ptr = zend_register_internal_class(&ce);

	zend_declare_property_null(IxpRequest_ce_ptr, 
		"fid", 3, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_null(IxpRequest_ce_ptr, 
		"ifcall", 6, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_null(IxpRequest_ce_ptr, 
		"ofcall", 6, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_null(IxpRequest_ce_ptr, 
		"newfid", 6, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_null(IxpRequest_ce_ptr, 
		"oldreq", 6, 
		ZEND_ACC_PUBLIC TSRMLS_DC);
}
/* }}} Class IxpRequest */


/* {{{ Class IxpFcall */
static zend_function_entry IxpFcall_methods[] = {
	{ NULL, NULL, NULL }
};

static void class_init_IxpFcall(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFcall", IxpFcall_methods);
	IxpFcall_ce_ptr = zend_register_internal_class(&ce);

	zend_declare_property_long(IxpFcall_ce_ptr, 
		"type", 4, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFcall_ce_ptr, 
		"tag", 3, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFcall_ce_ptr, 
		"fid", 3, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFcall_ce_ptr, 
		"msize", 5, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_string(IxpFcall_ce_ptr, 
		"version", 7, "", 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFcall_ce_ptr, 
		"oldtag", 6, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_string(IxpFcall_ce_ptr, 
		"ename", 5, "", 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_null(IxpFcall_ce_ptr, 
		"qid", 3, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFcall_ce_ptr, 
		"iounit", 6, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_null(IxpFcall_ce_ptr, 
		"aqid", 4, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFcall_ce_ptr, 
		"afid", 4, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_string(IxpFcall_ce_ptr, 
		"uname", 5, "", 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_string(IxpFcall_ce_ptr, 
		"aname", 5, "", 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFcall_ce_ptr, 
		"perm", 4, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_string(IxpFcall_ce_ptr, 
		"name", 4, "", 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFcall_ce_ptr, 
		"mode", 4, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFcall_ce_ptr, 
		"newfid", 6, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFcall_ce_ptr, 
		"nwname", 6, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_string(IxpFcall_ce_ptr, 
		"wname", 5, "", 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFcall_ce_ptr, 
		"nwqid", 5, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_null(IxpFcall_ce_ptr, 
		"wqid", 4, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFcall_ce_ptr, 
		"offset", 6, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFcall_ce_ptr, 
		"count", 5, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_string(IxpFcall_ce_ptr, 
		"data", 4, "", 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFcall_ce_ptr, 
		"nstat", 5, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_string(IxpFcall_ce_ptr, 
		"stat", 4, "", 
		ZEND_ACC_PUBLIC TSRMLS_DC);
}

/* }}} Class IxpFcall */


/* {{{ Class IxpServer */

/* {{{ proto void construct(string address, object IxpCallbacks[, object IxpServerCallbacks]) */
PHP_METHOD(IxpServer, __construct)
{
	zend_class_entry * _this_ce;
	zval * _this_zval;
	const char * address = NULL;
	int address_len = 0;
	zval * IxpCallbacks = NULL;
	zval * IxpServerCallbacks = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "so|o", &address, &address_len, &IxpCallbacks, &IxpServerCallbacks) == FAILURE) { 
		return;
	}

	_this_zval = getThis();
	_this_ce = Z_OBJCE_P(_this_zval);

	php_error(E_WARNING, "__construct: not yet implemented"); RETURN_FALSE;

}
/* }}} __construct */

/* {{{ proto void start() */
PHP_METHOD(IxpServer, start)
{
	zend_class_entry * _this_ce;
	zval * _this_zval = NULL;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &_this_zval, IxpServer_ce_ptr) == FAILURE) {
		return;
	}

	_this_ce = Z_OBJCE_P(_this_zval);

	php_error(E_WARNING, "start: not yet implemented"); RETURN_FALSE;

}
/* }}} start */

/* {{{ proto void stop() */
PHP_METHOD(IxpServer, stop)
{
	zend_class_entry * _this_ce;
	zval * _this_zval = NULL;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &_this_zval, IxpServer_ce_ptr) == FAILURE) {
		return;
	}

	_this_ce = Z_OBJCE_P(_this_zval);

	php_error(E_WARNING, "stop: not yet implemented"); RETURN_FALSE;

}
/* }}} stop */

static zend_function_entry IxpServer_methods[] = {
	PHP_ME(IxpServer, __construct, IxpServer____construct_args, /**/ZEND_ACC_PUBLIC)
	PHP_ME(IxpServer, start, NULL, /**/ZEND_ACC_PUBLIC)
	PHP_ME(IxpServer, stop, NULL, /**/ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};

static void class_init_IxpServer(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpServer", IxpServer_methods);
	IxpServer_ce_ptr = zend_register_internal_class(&ce);
}
/* }}} Class IxpServer */

/* {{{ ixp_functions[] */
function_entry ixp_functions[] = {
	PHP_MALIAS(IxpClient, no, create, NULL, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};
/* }}} */


/* {{{ ixp_module_entry */
zend_module_entry ixp_module_entry = {
	STANDARD_MODULE_HEADER,
	"ixp",
	ixp_functions,
	PHP_MINIT(ixp),     /* Replace with NULL if there is nothing to do at php startup   */ 
	PHP_MSHUTDOWN(ixp), /* Replace with NULL if there is nothing to do at php shutdown  */
	PHP_RINIT(ixp),     /* Replace with NULL if there is nothing to do at request start */
	PHP_RSHUTDOWN(ixp), /* Replace with NULL if there is nothing to do at request end   */
	PHP_MINFO(ixp),
	"0.0.1", 
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_IXP
ZEND_GET_MODULE(ixp)
#endif

/* {{{ PHP_MSHUTDOWN_FUNCTION */
PHP_MSHUTDOWN_FUNCTION(ixp)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(ixp)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION */
PHP_RSHUTDOWN_FUNCTION(ixp)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(ixp)
{
	php_info_print_box_start(0);
	php_printf("<p>PHP bindings to the 9P protocol</p>\n");
	php_printf("<p>Version 0.0.1devel (2007-06-11)</p>\n");
	php_printf("<p><b>Authors:</b></p>\n");
	php_printf("<p>Anant Narayanan &lt;anant@php.net&gt; (lead)</p>\n");
	php_info_print_box_end();
}
/* }}} */

#endif /* HAVE_IXP */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 
 * vim<600: noet sw=4 ts=4
 */
