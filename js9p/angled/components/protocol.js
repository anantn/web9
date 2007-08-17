const NINEP_SCHEME = "ninep"
const NINEP_PROTOCOL_NAME = "The 9P File Protocol" 
const NINEP_PROTOCOL_CONTRACTID = "@mozilla.org/network/protocol;1?name=ninep";
const NINEP_PROTOCOL_CID = Components.ID("6F16F5C0-AAC0-4A43-A800-EA33354B8CCC");

const SIMPLEURI_CONTRACTID = "@mozilla.org/network/simple-uri;1";
const IOSERVICE_CONTRACTID = "@mozilla.org/network/io-service;1";
const IOCHANNEL_CONTRACTID	= "@mozilla.org/network/stream-io-channel;1";

const nsISupports = Components.interfaces.nsISupports;
const nsIIOService = Components.interfaces.nsIIOService;
const nsIProtocolHandler = Components.interfaces.nsIProtocolHandler;
const nsIURI = Components.interfaces.nsIURI;

function AngledImport(obj) {
	var loader = Components.classes["@mozilla.org/moz/jssubscript-loader;1"].getService(Components.interfaces.mozIJSSubScriptLoader);
	loader.loadSubScript("file://"+__LOCATION__.parent.path+"/sockets.js", obj);
	loader.loadSubScript("file://"+__LOCATION__.parent.path+"/js9p.js", obj);
}
AngledImport(this);

function AngledLog(msg) {
	var consoleService = Components.classes["@mozilla.org/consoleservice;1"].getService(Components.interfaces.nsIConsoleService);
	consoleService.logStringMessage(msg);
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

		var x = new Socket();
		var ios = Components.classes[IOSERVICE_CONTRACTID].getService(nsIIOService);
		var uri = ios.newURI("file:///Users/anant/Documents/Resumes/me/resume.html", null, null);
		var chan = ios.newChannelFromURI(uri);
		return chan;
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
