/* Globals */
var NINEP_SCHEME = "ninep"
var NINEP_PROTOCOL_NAME = "The 9P File Protocol" 
var NINEP_PROTOCOL_CONTRACTID = "@mozilla.org/network/protocol;1?name=ninep";
var NINEP_PROTOCOL_CID = Components.ID("6F16F5C0-AAC0-4A43-A800-EA33354B8CCC");

var SIMPLEURI_CONTRACTID = "@mozilla.org/network/simple-uri;1";
var IOSERVICE_CONTRACTID = "@mozilla.org/network/io-service;1";
var IOCHANNEL_CONTRACTID	= "@mozilla.org/network/stream-io-channel;1";

var nsISupports = Components.interfaces.nsISupports;
var nsIIOService = Components.interfaces.nsIIOService;
var nsIProtocolHandler = Components.interfaces.nsIProtocolHandler;
var nsIURI = Components.interfaces.nsIURI;

function AngledLog(msg, error) {
	var consoleService = Components.classes["@mozilla.org/consoleservice;1"].getService(Components.interfaces.nsIConsoleService);
	if (error) {
		consoleService.logStringError(msg);
	} else {
		consoleService.logStringMessage(msg);
	}
}

function AngledImport(obj) {
	var loader = Components.classes["@mozilla.org/moz/jssubscript-loader;1"].getService(Components.interfaces.mozIJSSubScriptLoader);
	loader.loadSubScript("file://"+__LOCATION__.parent.path+"/js9p.js", obj);
	loader.loadSubScript("file://"+__LOCATION__.parent.path+"/sockets.js", obj);
	loader.loadSubScript("file://"+__LOCATION__.parent.path+"/angled.js", obj);
}
AngledImport(this);
AngledLog("Scripts Imported");

function AngledDefaultChannel() {
	var ios = Components.classes[IOSERVICE_CONTRACTID].getService(nsIIOService);
	var uri = ios.newURI("chrome://angled/content/angled.png", null, null);
	var chan = ios.newChannelFromURI(uri);
	return chan;
}

function AngledProtocol() {

}

AngledProtocol.prototype =
{
	QueryInterface: function(iid) {
		if ((!iid.equals(nsIProtocolHandler)) && (!iid.equals(nsISupports))) {
			throw Components.results.NS_ERROR_NO_INTERFACE;
		}
		return this;
	},

	scheme: NINEP_SCHEME,
	defaultPort: 564,
	protocolFlags: nsIProtocolHandler.URI_NORELATIVE | nsIProtocolHandler.URI_NOAUTH,
  
	allowPort: function(port, scheme) {
		return false;
	},

	newURI: function(spec, charset, baseURI) {
		var uri = Components.classes[SIMPLEURI_CONTRACTID].createInstance(nsIURI);
		uri.spec = spec;
		return uri;
	},

	newChannel: function(aURI) {
		var angledURI = aURI.spec.substr((aURI.spec.indexOf("://") + "://".length));

		var host = '';
		var port = 564;
		var splitURI = angledURI.split('/');
		var header = splitURI[0].split('!');

		switch (header.length) {
			case 1: break;
			case 2: host = header[0]; port = header[1]; break;
			case 3:
				if (header[0] != 'tcp') {
					AngledLog("Angled supports only the TCP transport!", true);
					return AngledDeafaultChannel();
				} else {
					host = header[1];
					port = header[2];
				} break;
			default:
				return AngledDefaultChannel();
		}
		
		if ((splitURI[-1] == '/') || (splitURI[-1] == ''))
			return AngledDefaultChannel();

		var jObj = JS9P.Angled.initialize(host, port);
		JS9P.Angled.version();
		JS9P.Angled.attach(1, 10, JS9P.Base.constants['NOFID'], 'angled', '');
		JS9P.Angled.walk(2, 10, 11, ['/', 'kix']);
		var stat = JS9P.Angled.stat(3, 11);
		AngledLog(stat[-5]);
		//JS9P.Angled.open(4, 11);
		//AngledLog(JS9P.Angled.read(5, 11, 0, stat[-5]));
		//JS9P.Angled.clunk(5, 11, JS9P.Base.constants['OREAD']);
		jObj.close();
		return AngledDefaultChannel();
	}
};

var AngledProtocolFactory = {};

AngledProtocolFactory.createInstance = function(outer, iid) {
	if(outer !== null) {
		throw Components.results.NS_ERROR_NO_AGGREGATION;
	}

	if((!iid.equals(nsIProtocolHandler)) && (!iid.equals(nsISupports))) {
        throw Components.results.NS_ERROR_INVALID_ARG;
	}

    return new AngledProtocol();
};

var AngledModule = {};

AngledModule.registerSelf = function(compMgr, fileSpec, location, type) {
	compMgr = compMgr.QueryInterface(Components.interfaces.nsIComponentRegistrar);
	
	compMgr.registerFactoryLocation(NINEP_PROTOCOL_CID,
										NINEP_PROTOCOL_NAME,
											NINEP_PROTOCOL_CONTRACTID,
												fileSpec, location, type);
};

AngledModule.unregisterSelf = function(compMgr, fileSpec, location) {
    compMgr = compMgr.QueryInterface(Components.interfaces.nsIComponentRegistrar);
    
	compMgr.unregisterFactoryLocation(NINEP_PROTOCOL_CID, fileSpec);
};

AngledModule.getClassObject = function(compMgr, cid, iid) {
	if (!cid.equals(NINEP_PROTOCOL_CID)) {
		throw Components.results.NS_ERROR_NO_INTERFACE;
	}

	if (!iid.equals(Components.interfaces.nsIFactory)) {
		throw Components.results.NS_ERROR_NOT_IMPLEMENTED;
	}
    
	return AngledProtocolFactory;
};

AngledModule.canUnload = function(compMgr) {
	return true;
};

function NSGetModule(compMgr, fileSpec) {
	return AngledModule;
}
