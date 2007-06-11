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

/* {{{ Interface definitions */

/* {{{ Interface IxpFcallData */

static zend_class_entry * IxpFcallData_ce_ptr = NULL;

static zend_function_entry IxpFcallData_methods[] = {
	{ NULL, NULL, NULL }
};

static void interface_init_IxpFcallData(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFcallData", IxpFcallData_methods);
	IxpFcallData_ce_ptr = zend_register_internal_interface(&ce TSRMLS_CC);
}

/* }}} Class IxpFcallData */

/* {{{ Interface IxpCallbacks */

static zend_class_entry * IxpCallbacks_ce_ptr = NULL;

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

/* }}} Class IxpCallbacks */

/* {{{ Interface IxpServerCallbacks */

static zend_class_entry * IxpServerCallbacks_ce_ptr = NULL;

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

/* }}} Class IxpServerCallbacks */

/* }}} Interface definitions*/

/* {{{ Class definitions */

/* {{{ Class IxpClient */

static zend_class_entry * IxpClient_ce_ptr = NULL;

/* {{{ Methods */


/* {{{ proto void construct(string path)
   */
PHP_METHOD(IxpClient, __construct)
{
	zend_class_entry * _this_ce;
	zval * _this_zval;
	const char * path = NULL;
	int path_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &path, &path_len) == FAILURE) { 
		return;
	}

	_this_zval = getThis();
	_this_ce = Z_OBJCE_P(_this_zval);

	php_error(E_WARNING, "__construct: not yet implemented"); RETURN_FALSE;

}
/* }}} __construct */



/* {{{ proto void unmount()
   */
PHP_METHOD(IxpClient, unmount)
{
	zend_class_entry * _this_ce;
	zval * _this_zval = NULL;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &_this_zval, IxpClient_ce_ptr) == FAILURE) {
		return;
	}

	_this_ce = Z_OBJCE_P(_this_zval);

	php_error(E_WARNING, "unmount: not yet implemented"); RETURN_FALSE;

}
/* }}} unmount */



/* {{{ proto object create(string name [, string permissions [, string mode]])
   */
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



/* {{{ proto object open(string name [, string mode])
   */
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



/* {{{ proto int remove(string path)
   */
PHP_METHOD(IxpClient, remove)
{
	zend_class_entry * _this_ce;
	zval * _this_zval = NULL;
	const char * path = NULL;
	int path_len = 0;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &_this_zval, IxpClient_ce_ptr, &path, &path_len) == FAILURE) {
		return;
	}

	_this_ce = Z_OBJCE_P(_this_zval);

	php_error(E_WARNING, "remove: not yet implemented"); RETURN_FALSE;

	RETURN_LONG(0);
}
/* }}} remove */



/* {{{ proto object stat(string path)
   */
PHP_METHOD(IxpClient, stat)
{
	zend_class_entry * _this_ce;
	zval * _this_zval = NULL;
	const char * path = NULL;
	int path_len = 0;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &_this_zval, IxpClient_ce_ptr, &path, &path_len) == FAILURE) {
		return;
	}

	_this_ce = Z_OBJCE_P(_this_zval);

	php_error(E_WARNING, "stat: not yet implemented"); RETURN_FALSE;

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

/* }}} Methods */

static void class_init_IxpClient(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpClient", IxpClient_methods);
	IxpClient_ce_ptr = zend_register_internal_class(&ce);

	/* {{{ Property registration */

	zend_declare_property_long(IxpClient_ce_ptr, 
		"mSize", 5, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpClient_ce_ptr, 
		"lastFid", 7, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_null(IxpClient_ce_ptr, 
		"fd", 2, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_null(IxpClient_ce_ptr, 
		"freeFid", 7, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_null(IxpClient_ce_ptr, 
		"message", 7, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	/* }}} Property registration */

}

/* }}} Class IxpClient */

/* {{{ Class IxpCFid */

static zend_class_entry * IxpCFid_ce_ptr = NULL;

/* {{{ Methods */


/* {{{ proto int read(string buffer [, int count])
   */
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



/* {{{ proto int write(string buffer)
   */
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



/* {{{ proto int close()
   */
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

/* }}} Methods */

static void class_init_IxpCFid(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpCFid", IxpCFid_methods);
	IxpCFid_ce_ptr = zend_register_internal_class(&ce);

	/* {{{ Property registration */

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

	/* }}} Property registration */

}

/* }}} Class IxpCFid */

/* {{{ Class IxpStat */

static zend_class_entry * IxpStat_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpStat_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpStat(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpStat", IxpStat_methods);
	IxpStat_ce_ptr = zend_register_internal_class(&ce);

	/* {{{ Property registration */

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

	/* }}} Property registration */

}

/* }}} Class IxpStat */

/* {{{ Class IxpQid */

static zend_class_entry * IxpQid_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpQid_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpQid(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpQid", IxpQid_methods);
	IxpQid_ce_ptr = zend_register_internal_class(&ce);

	/* {{{ Property registration */

	zend_declare_property_long(IxpQid_ce_ptr, 
		"type", 4, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpQid_ce_ptr, 
		"version", 7, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_string(IxpQid_ce_ptr, 
		"path", 4, "", 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	/* }}} Property registration */

}

/* }}} Class IxpQid */

/* {{{ Class IxpConn */

static zend_class_entry * IxpConn_ce_ptr = NULL;

/* {{{ Methods */


/* {{{ proto void serve()
   */
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

/* }}} Methods */

static void class_init_IxpConn(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpConn", IxpConn_methods);
	IxpConn_ce_ptr = zend_register_internal_class(&ce);

	/* {{{ Property registration */

	zend_declare_property_long(IxpConn_ce_ptr, 
		"closed", 6, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_null(IxpConn_ce_ptr, 
		"running", 7, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_null(IxpConn_ce_ptr, 
		"fd", 2, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	/* }}} Property registration */

}

/* }}} Class IxpConn */

/* {{{ Class IxpFid */

static zend_class_entry * IxpFid_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpFid_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpFid(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFid", IxpFid_methods);
	IxpFid_ce_ptr = zend_register_internal_class(&ce);

	/* {{{ Property registration */

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

	/* }}} Property registration */

}

/* }}} Class IxpFid */

/* {{{ Class IxpRequest */

static zend_class_entry * IxpRequest_ce_ptr = NULL;

/* {{{ Methods */


/* {{{ proto void respond(string error)
   */
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

/* }}} Methods */

static void class_init_IxpRequest(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpRequest", IxpRequest_methods);
	IxpRequest_ce_ptr = zend_register_internal_class(&ce);

	/* {{{ Property registration */

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

	/* }}} Property registration */

}

/* }}} Class IxpRequest */

/* {{{ Class IxpFcall */

static zend_class_entry * IxpFcall_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpFcall_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpFcall(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFcall", IxpFcall_methods);
	IxpFcall_ce_ptr = zend_register_internal_class(&ce);

	/* {{{ Property registration */

	zend_declare_property_long(IxpFcall_ce_ptr, 
		"type", 4, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFcall_ce_ptr, 
		"tag", 3, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_null(IxpFcall_ce_ptr, 
		"data", 4, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	/* }}} Property registration */

}

/* }}} Class IxpFcall */

/* {{{ Class IxpFcallDataTVersion */

static zend_class_entry * IxpFcallDataTVersion_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpFcallDataTVersion_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpFcallDataTVersion(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFcallDataTVersion", IxpFcallDataTVersion_methods);
	IxpFcallDataTVersion_ce_ptr = zend_register_internal_class_ex(&ce, NULL, "IxpFcallData" TSRMLS_CC);

	/* {{{ Property registration */

	zend_declare_property_long(IxpFcallDataTVersion_ce_ptr, 
		"msize", 5, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_string(IxpFcallDataTVersion_ce_ptr, 
		"version", 7, "", 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	/* }}} Property registration */

}

/* }}} Class IxpFcallDataTVersion */

/* {{{ Class IxpFcallDataTauth */

static zend_class_entry * IxpFcallDataTauth_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpFcallDataTauth_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpFcallDataTauth(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFcallDataTauth", IxpFcallDataTauth_methods);
	IxpFcallDataTauth_ce_ptr = zend_register_internal_class_ex(&ce, NULL, "IxpFcallData" TSRMLS_CC);

	/* {{{ Property registration */

	zend_declare_property_long(IxpFcallDataTauth_ce_ptr, 
		"afid", 4, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_string(IxpFcallDataTauth_ce_ptr, 
		"uname", 5, "", 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_string(IxpFcallDataTauth_ce_ptr, 
		"aname", 5, "", 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	/* }}} Property registration */

}

/* }}} Class IxpFcallDataTauth */

/* {{{ Class IxpFcallDataRauth */

static zend_class_entry * IxpFcallDataRauth_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpFcallDataRauth_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpFcallDataRauth(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFcallDataRauth", IxpFcallDataRauth_methods);
	IxpFcallDataRauth_ce_ptr = zend_register_internal_class_ex(&ce, NULL, "IxpFcallData" TSRMLS_CC);

	/* {{{ Property registration */

	zend_declare_property_null(IxpFcallDataRauth_ce_ptr, 
		"aqid", 4, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	/* }}} Property registration */

}

/* }}} Class IxpFcallDataRauth */

/* {{{ Class IxpFcallDataRerror */

static zend_class_entry * IxpFcallDataRerror_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpFcallDataRerror_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpFcallDataRerror(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFcallDataRerror", IxpFcallDataRerror_methods);
	IxpFcallDataRerror_ce_ptr = zend_register_internal_class_ex(&ce, NULL, "IxpFcallData" TSRMLS_CC);

	/* {{{ Property registration */

	zend_declare_property_string(IxpFcallDataRerror_ce_ptr, 
		"ename", 5, "", 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	/* }}} Property registration */

}

/* }}} Class IxpFcallDataRerror */

/* {{{ Class IxpFcallDataTflush */

static zend_class_entry * IxpFcallDataTflush_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpFcallDataTflush_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpFcallDataTflush(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFcallDataTflush", IxpFcallDataTflush_methods);
	IxpFcallDataTflush_ce_ptr = zend_register_internal_class_ex(&ce, NULL, "IxpFcallData" TSRMLS_CC);

	/* {{{ Property registration */

	zend_declare_property_long(IxpFcallDataTflush_ce_ptr, 
		"oldtag", 6, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	/* }}} Property registration */

}

/* }}} Class IxpFcallDataTflush */

/* {{{ Class IxpFcallDataTattach */

static zend_class_entry * IxpFcallDataTattach_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpFcallDataTattach_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpFcallDataTattach(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFcallDataTattach", IxpFcallDataTattach_methods);
	IxpFcallDataTattach_ce_ptr = zend_register_internal_class_ex(&ce, NULL, "IxpFcallData" TSRMLS_CC);

	/* {{{ Property registration */

	zend_declare_property_long(IxpFcallDataTattach_ce_ptr, 
		"afid", 4, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_string(IxpFcallDataTattach_ce_ptr, 
		"uname", 5, "", 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_string(IxpFcallDataTattach_ce_ptr, 
		"aname", 5, "", 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	/* }}} Property registration */

}

/* }}} Class IxpFcallDataTattach */

/* {{{ Class IxpFcallDataTwalk */

static zend_class_entry * IxpFcallDataTwalk_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpFcallDataTwalk_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpFcallDataTwalk(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFcallDataTwalk", IxpFcallDataTwalk_methods);
	IxpFcallDataTwalk_ce_ptr = zend_register_internal_class_ex(&ce, NULL, "IxpFcallData" TSRMLS_CC);

	/* {{{ Property registration */

	zend_declare_property_long(IxpFcallDataTwalk_ce_ptr, 
		"fid", 3, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFcallDataTwalk_ce_ptr, 
		"newfid", 6, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFcallDataTwalk_ce_ptr, 
		"nwname", 6, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_string(IxpFcallDataTwalk_ce_ptr, 
		"wname", 5, "", 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	/* }}} Property registration */

}

/* }}} Class IxpFcallDataTwalk */

/* {{{ Class IxpFcallDataRwalk */

static zend_class_entry * IxpFcallDataRwalk_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpFcallDataRwalk_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpFcallDataRwalk(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFcallDataRwalk", IxpFcallDataRwalk_methods);
	IxpFcallDataRwalk_ce_ptr = zend_register_internal_class_ex(&ce, NULL, "IxpFcallData" TSRMLS_CC);

	/* {{{ Property registration */

	zend_declare_property_long(IxpFcallDataRwalk_ce_ptr, 
		"nwqid", 5, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_null(IxpFcallDataRwalk_ce_ptr, 
		"wqid", 4, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	/* }}} Property registration */

}

/* }}} Class IxpFcallDataRwalk */

/* {{{ Class IxpFcallDataTopen */

static zend_class_entry * IxpFcallDataTopen_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpFcallDataTopen_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpFcallDataTopen(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFcallDataTopen", IxpFcallDataTopen_methods);
	IxpFcallDataTopen_ce_ptr = zend_register_internal_class_ex(&ce, NULL, "IxpFcallData" TSRMLS_CC);

	/* {{{ Property registration */

	zend_declare_property_long(IxpFcallDataTopen_ce_ptr, 
		"fid", 3, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFcallDataTopen_ce_ptr, 
		"mode", 4, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	/* }}} Property registration */

}

/* }}} Class IxpFcallDataTopen */

/* {{{ Class IxpFcallDataRopen */

static zend_class_entry * IxpFcallDataRopen_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpFcallDataRopen_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpFcallDataRopen(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFcallDataRopen", IxpFcallDataRopen_methods);
	IxpFcallDataRopen_ce_ptr = zend_register_internal_class_ex(&ce, NULL, "IxpFcallData" TSRMLS_CC);

	/* {{{ Property registration */

	zend_declare_property_null(IxpFcallDataRopen_ce_ptr, 
		"qid", 3, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFcallDataRopen_ce_ptr, 
		"iounit", 6, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	/* }}} Property registration */

}

/* }}} Class IxpFcallDataRopen */

/* {{{ Class IxpFcallDataTcreate */

static zend_class_entry * IxpFcallDataTcreate_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpFcallDataTcreate_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpFcallDataTcreate(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFcallDataTcreate", IxpFcallDataTcreate_methods);
	IxpFcallDataTcreate_ce_ptr = zend_register_internal_class_ex(&ce, NULL, "IxpFcallData" TSRMLS_CC);

	/* {{{ Property registration */

	zend_declare_property_long(IxpFcallDataTcreate_ce_ptr, 
		"fid", 3, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFcallDataTcreate_ce_ptr, 
		"perm", 4, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFcallDataTcreate_ce_ptr, 
		"mode", 4, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_string(IxpFcallDataTcreate_ce_ptr, 
		"name", 4, "", 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	/* }}} Property registration */

}

/* }}} Class IxpFcallDataTcreate */

/* {{{ Class IxpFcallDataRcreate */

static zend_class_entry * IxpFcallDataRcreate_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpFcallDataRcreate_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpFcallDataRcreate(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFcallDataRcreate", IxpFcallDataRcreate_methods);
	IxpFcallDataRcreate_ce_ptr = zend_register_internal_class_ex(&ce, NULL, "IxpFcallData" TSRMLS_CC);

	/* {{{ Property registration */

	zend_declare_property_null(IxpFcallDataRcreate_ce_ptr, 
		"qid", 3, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFcallDataRcreate_ce_ptr, 
		"iounit", 6, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	/* }}} Property registration */

}

/* }}} Class IxpFcallDataRcreate */

/* {{{ Class IxpFcallDataTread */

static zend_class_entry * IxpFcallDataTread_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpFcallDataTread_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpFcallDataTread(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFcallDataTread", IxpFcallDataTread_methods);
	IxpFcallDataTread_ce_ptr = zend_register_internal_class_ex(&ce, NULL, "IxpFcallData" TSRMLS_CC);

	/* {{{ Property registration */

	zend_declare_property_long(IxpFcallDataTread_ce_ptr, 
		"fid", 3, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFcallDataTread_ce_ptr, 
		"offset", 6, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFcallDataTread_ce_ptr, 
		"count", 5, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	/* }}} Property registration */

}

/* }}} Class IxpFcallDataTread */

/* {{{ Class IxpFcallDataRread */

static zend_class_entry * IxpFcallDataRread_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpFcallDataRread_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpFcallDataRread(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFcallDataRread", IxpFcallDataRread_methods);
	IxpFcallDataRread_ce_ptr = zend_register_internal_class_ex(&ce, NULL, "IxpFcallData" TSRMLS_CC);

	/* {{{ Property registration */

	zend_declare_property_long(IxpFcallDataRread_ce_ptr, 
		"count", 5, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_string(IxpFcallDataRread_ce_ptr, 
		"data", 4, "", 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	/* }}} Property registration */

}

/* }}} Class IxpFcallDataRread */

/* {{{ Class IxpFcallDataTwrite */

static zend_class_entry * IxpFcallDataTwrite_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpFcallDataTwrite_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpFcallDataTwrite(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFcallDataTwrite", IxpFcallDataTwrite_methods);
	IxpFcallDataTwrite_ce_ptr = zend_register_internal_class_ex(&ce, NULL, "IxpFcallData" TSRMLS_CC);

	/* {{{ Property registration */

	zend_declare_property_long(IxpFcallDataTwrite_ce_ptr, 
		"fid", 3, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFcallDataTwrite_ce_ptr, 
		"offset", 6, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_long(IxpFcallDataTwrite_ce_ptr, 
		"count", 5, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_string(IxpFcallDataTwrite_ce_ptr, 
		"data", 4, "", 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	/* }}} Property registration */

}

/* }}} Class IxpFcallDataTwrite */

/* {{{ Class IxpFcallDataRwrite */

static zend_class_entry * IxpFcallDataRwrite_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpFcallDataRwrite_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpFcallDataRwrite(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFcallDataRwrite", IxpFcallDataRwrite_methods);
	IxpFcallDataRwrite_ce_ptr = zend_register_internal_class_ex(&ce, NULL, "IxpFcallData" TSRMLS_CC);

	/* {{{ Property registration */

	zend_declare_property_long(IxpFcallDataRwrite_ce_ptr, 
		"count", 5, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	/* }}} Property registration */

}

/* }}} Class IxpFcallDataRwrite */

/* {{{ Class IxpFcallDataTclunk */

static zend_class_entry * IxpFcallDataTclunk_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpFcallDataTclunk_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpFcallDataTclunk(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFcallDataTclunk", IxpFcallDataTclunk_methods);
	IxpFcallDataTclunk_ce_ptr = zend_register_internal_class_ex(&ce, NULL, "IxpFcallData" TSRMLS_CC);

	/* {{{ Property registration */

	zend_declare_property_long(IxpFcallDataTclunk_ce_ptr, 
		"fid", 3, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	/* }}} Property registration */

}

/* }}} Class IxpFcallDataTclunk */

/* {{{ Class IxpFcallDataRclunk */

static zend_class_entry * IxpFcallDataRclunk_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpFcallDataRclunk_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpFcallDataRclunk(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFcallDataRclunk", IxpFcallDataRclunk_methods);
	IxpFcallDataRclunk_ce_ptr = zend_register_internal_class_ex(&ce, NULL, "IxpFcallData" TSRMLS_CC);
}

/* }}} Class IxpFcallDataRclunk */

/* {{{ Class IxpFcallDataTremove */

static zend_class_entry * IxpFcallDataTremove_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpFcallDataTremove_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpFcallDataTremove(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFcallDataTremove", IxpFcallDataTremove_methods);
	IxpFcallDataTremove_ce_ptr = zend_register_internal_class_ex(&ce, NULL, "IxpFcallData" TSRMLS_CC);

	/* {{{ Property registration */

	zend_declare_property_long(IxpFcallDataTremove_ce_ptr, 
		"fid", 3, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	/* }}} Property registration */

}

/* }}} Class IxpFcallDataTremove */

/* {{{ Class IxpFcallDataRremove */

static zend_class_entry * IxpFcallDataRremove_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpFcallDataRremove_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpFcallDataRremove(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFcallDataRremove", IxpFcallDataRremove_methods);
	IxpFcallDataRremove_ce_ptr = zend_register_internal_class_ex(&ce, NULL, "IxpFcallData" TSRMLS_CC);
}

/* }}} Class IxpFcallDataRremove */

/* {{{ Class IxpFcallDataTstat */

static zend_class_entry * IxpFcallDataTstat_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpFcallDataTstat_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpFcallDataTstat(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFcallDataTstat", IxpFcallDataTstat_methods);
	IxpFcallDataTstat_ce_ptr = zend_register_internal_class_ex(&ce, NULL, "IxpFcallData" TSRMLS_CC);

	/* {{{ Property registration */

	zend_declare_property_long(IxpFcallDataTstat_ce_ptr, 
		"fid", 3, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	/* }}} Property registration */

}

/* }}} Class IxpFcallDataTstat */

/* {{{ Class IxpFcallDataRstat */

static zend_class_entry * IxpFcallDataRstat_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpFcallDataRstat_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpFcallDataRstat(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFcallDataRstat", IxpFcallDataRstat_methods);
	IxpFcallDataRstat_ce_ptr = zend_register_internal_class_ex(&ce, NULL, "IxpFcallData" TSRMLS_CC);

	/* {{{ Property registration */

	zend_declare_property_long(IxpFcallDataRstat_ce_ptr, 
		"nstat", 5, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_string(IxpFcallDataRstat_ce_ptr, 
		"stat", 4, "", 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	/* }}} Property registration */

}

/* }}} Class IxpFcallDataRstat */

/* {{{ Class IxpFcallDataTwstat */

static zend_class_entry * IxpFcallDataTwstat_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpFcallDataTwstat_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpFcallDataTwstat(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFcallDataTwstat", IxpFcallDataTwstat_methods);
	IxpFcallDataTwstat_ce_ptr = zend_register_internal_class_ex(&ce, NULL, "IxpFcallData" TSRMLS_CC);

	/* {{{ Property registration */

	zend_declare_property_long(IxpFcallDataTwstat_ce_ptr, 
		"nstat", 5, 0, 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	zend_declare_property_string(IxpFcallDataTwstat_ce_ptr, 
		"stat", 4, "", 
		ZEND_ACC_PUBLIC TSRMLS_DC);

	/* }}} Property registration */

}

/* }}} Class IxpFcallDataTwstat */

/* {{{ Class IxpFcallDataRwstat */

static zend_class_entry * IxpFcallDataRwstat_ce_ptr = NULL;

/* {{{ Methods */

static zend_function_entry IxpFcallDataRwstat_methods[] = {
	{ NULL, NULL, NULL }
};

/* }}} Methods */

static void class_init_IxpFcallDataRwstat(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpFcallDataRwstat", IxpFcallDataRwstat_methods);
	IxpFcallDataRwstat_ce_ptr = zend_register_internal_class_ex(&ce, NULL, "IxpFcallData" TSRMLS_CC);
}

/* }}} Class IxpFcallDataRwstat */

/* {{{ Class IxpServer */

static zend_class_entry * IxpServer_ce_ptr = NULL;

/* {{{ Methods */


/* {{{ proto void construct(string address, object IxpCallbacks[, object IxpServerCallbacks])
   */
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



/* {{{ proto void start()
   */
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



/* {{{ proto void stop()
   */
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

/* }}} Methods */

static void class_init_IxpServer(void)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "IxpServer", IxpServer_methods);
	IxpServer_ce_ptr = zend_register_internal_class(&ce);
}

/* }}} Class IxpServer */

/* }}} Class definitions*/

/* {{{ ixp_functions[] */
function_entry ixp_functions[] = {
	PHP_MALIAS(IxpClient, no, create, NULL, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL }
};
/* }}} */


/* {{{ ixp_module_entry
 */
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
	class_init_IxpFcallDataTVersion();
	class_init_IxpFcallDataTauth();
	class_init_IxpFcallDataRauth();
	class_init_IxpFcallDataRerror();
	class_init_IxpFcallDataTflush();
	class_init_IxpFcallDataTattach();
	class_init_IxpFcallDataTwalk();
	class_init_IxpFcallDataRwalk();
	class_init_IxpFcallDataTopen();
	class_init_IxpFcallDataRopen();
	class_init_IxpFcallDataTcreate();
	class_init_IxpFcallDataRcreate();
	class_init_IxpFcallDataTread();
	class_init_IxpFcallDataRread();
	class_init_IxpFcallDataTwrite();
	class_init_IxpFcallDataRwrite();
	class_init_IxpFcallDataTclunk();
	class_init_IxpFcallDataRclunk();
	class_init_IxpFcallDataTremove();
	class_init_IxpFcallDataRremove();
	class_init_IxpFcallDataTstat();
	class_init_IxpFcallDataRstat();
	class_init_IxpFcallDataTwstat();
	class_init_IxpFcallDataRwstat();
	class_init_IxpServer();
	interface_init_IxpFcallData();
	interface_init_IxpCallbacks();
	interface_init_IxpServerCallbacks();

	/* add your stuff here */

	return SUCCESS;
}
/* }}} */


/* {{{ PHP_MSHUTDOWN_FUNCTION */
PHP_MSHUTDOWN_FUNCTION(ixp)
{

	/* add your stuff here */

	return SUCCESS;
}
/* }}} */


/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(ixp)
{
	/* add your stuff here */

	return SUCCESS;
}
/* }}} */


/* {{{ PHP_RSHUTDOWN_FUNCTION */
PHP_RSHUTDOWN_FUNCTION(ixp)
{
	/* add your stuff here */

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
	/* add your stuff here */

}
/* }}} */

#endif /* HAVE_IXP */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
