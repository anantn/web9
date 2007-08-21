JS9P.Angled = function() {
	
	var socket;

	function _init(host, port) {
		JS9P.Base.setBase64(false);
		socket = JS9P.Socket.init();
		socket.open(host, port);
	}

	function _version() {
		var msg = JS9P.Base.encodeMessage(JS9P.Base.constants["NOTAG"], "Tversion", [16*1024, JS9P.Base.constants["VERSION"]]);
		socket.write(msg);
		AngledLog("Written");
		
		try {
			var size = socket.read(4);
		} catch (e) {
			AngledLog("Exception " + e);
		}
		//AngledLog("Read" + size);
		//AngledLog(JS9P.Base.decodeRaw(size, 4));
		
		socket.close();
		//return JS9P.Base.decodeMessage(size + ret);
		//return JS9P.Base.decodeRaw(size);
		return msg.length;
	}

	return {
		initialize: function(host, port) {
			_init(host, port);
			return _version();			
		}
	}

} ();
