JS9P.Angled = function() {
	
	var socket;

	function _init(host, port) {
		JS9P.Base.setBase64(false);
		socket = JS9P.Socket.init();
		socket.open(host, port);

		return true;
	}

	function _doTransaction(msg) {
		try {
			socket.write(msg);
		} catch (e) {
			AngledLog(e);
			return false;
		}
		//AngledLog("Written: " + JS9P.Base.decodeMessage(msg));

		try {
			var smsg = socket.read(4);
		} catch (e) {
			AngledLog(e);
			return false;
		}
		
		var size = JS9P.Base.decodeRaw(smsg, 4);
		AngledLog("Read size: " + size);
		try {
			var rmsg = socket.read(size - 4);
		} catch(e) {
			AngledLog(e);
			return false;
		}
		AngledLog("Read rest: " + String(size - 4));
		return JS9P.Base.decodeMessage(smsg + rmsg);
	}
	
	return {
		initialize: function(host, port) {
			if (_init(host, port))
				return this;
		},

		close: function() {
			socket.close();
			return true;
		},

		version: function() {
			var msg = JS9P.Base.encodeMessage(JS9P.Base.constants["NOTAG"], "Tversion", [16*1024, JS9P.Base.constants["VERSION"]]);
			return _doTransaction(msg);
		},

		attach: function(tag, fid, afid, uname, aname) {
			var msg = JS9P.Base.encodeMessage(tag, "Tattach", [fid, afid, uname, aname]);
			return _doTransaction(msg);
		},

		clunk: function(tag, fid) {
			var msg = JS9P.Base.encodeMessage(tag, "Tclunk", [fid]);
			return _doTransaction(msg);
		},

		flush: function(tag, oldtag) {
			var msg = JS9P.Base.encodeMessage(tag, "Tflush", [oldtag]);
			return _doTransaction(msg);
		},

		open: function(tag, fid, mode) {
			var msg = JS9P.Base.encodeMessage(tag, "Topen", [fid, mode]);
			return _doTransaction(msg);
		},

		create: function(tag, fid, name, perm, mode) {
			var msg = JS9P.Base.encodeMessage(tag, "Tcreate", [fid, name, perm, mode]);
			return _doTransaction(msg);
		},

		read: function(tag, fid, offset, count) {
			var msg = JS9P.Base.encodeMessage(tag, "Tread", [fid, offset, count]);
			return _doTransaction(msg);
		},

		write: function(tag, fid, offset, data) {
			var msg = JS9P.Base.encodeMessage(tag, "Twrite", [fid, offset, data]);
			return _doTransaction(msg);
		},

		remove: function(tag, fid) {
			var msg = JS9P.Base.encodeMessage(tag, "Tremove", [fid]);
			return _doTransaction(msg);
		},

		stat: function(tag, fid) {
			var msg = JS9P.Base.encodeMessage(tag, "Tstat", [fid]);
			return _doTransaction(msg);
		},

		wstat: function(tag, fid, stat) {
			if ((stat.length != 11) || (stat[2].length != 3)) {
				return false;
			} else {
				var msg = JS9P.Base.encodeMessage(tag, "Twstat", [stat]);
				return _doTransaction(msg);
			}
		},

		walk: function(tag, fid, newfid, names) {
			var msg = JS9P.Base.encodeMessage(tag, "Twalk", [[fid, newfid, names]]);
			return _doTransaction(msg);
		}
	}

} ();
