JS9P.Angled = function() {
	
	var socket;

	function _init(host, port) {
		socket = JS9P.Socket.init();
		socket.open(host, port);
	}

	function _version() {
		var msg = JS9P.Base.encodeMessage(JS9P.Base.constants["NOTAG"], "Tversion", false, [16*1024, JS9P.Base.constants["VERSION"]]);
		for (var i = 0; i < msg.length; i++) {
			socket.write(msg[i]);
		}

		var size = socket.read(4);
		//var ret = socket.read(JS9P.Base.decodeRaw(size, 4) - 4);

		AngledLog("Bytes Written!");
		//AngledLog(JS9P.Base.decodeRaw(size, 4));
		
		socket.close();
		//return JS9P.Base.decodeMessage(size + ret);
		return JS9P.Base.decodeRaw(size);
	}

	return {
		initialize: function(host, port) {
			_init(host, port);
			return _version();			
		}
	}

} ();
