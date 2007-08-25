<?php

/**
 * PEAR::Net_9P_Socket_Client
 *
 * This class provides low-level client side socket
 * functionality for use by Net_9P.
 *
 * @category  Net
 * @package   Net_9P
 * @version   1.0
 * @author    Anant Narayanan <anant@php.net>
 * @copyright 2007 Anant Narayanan <anant@php.net>
 * @license   http://www.gnu.org/copyleft/lesser.html  LGPL License 2.1
 */

class Net_9P_Socket_Client
{
    // {{{ properties
    
    /**
     * Current file descriptor
     *
     * @var resource
     * @access private
     */
    private $socket;

    /**
     * Indicates if socket is connected
     *
     * @var boolean
     * @access protected
     */
    protected $connected;

    /**
     * Current socket protocol
     *
     * @var String
     * @access protected
     */
    protected $protocol;

    /**
     * Current socket host
     *
     * @var String
     * @access protected
     */
    protected $host;

    /**
     * Current socket port
     * Will be NULL if protocol is unix
     *
     * @var int
     * @access protected
     */
    protected $port;

    // }}}
    // {{{ __construct()

    /**
     * Create a new client-side socket
     *
     * @access  public
     * @param   String  $protocol   One of 'tcp' or 'unix'
     * @param   String  $host       Hostname or IP address of server (for TCP) or
     *                              absolute filename (for unix)
     * @param   int     $port       Port number. Defaults to 564, ignored
     *                              for 'unix' protocol
     * @return  False if connection failed.
     */
    public function __construct($protocol, $host, $port = 564)
    {
        $this->connected = false;
        switch ($protocol) {
            case 'tcp':     $this->socket = stream_socket_client('tcp://'.$hostname.':'.$port);
                            break;
            case 'unix':    $this->socket = stream_socket_client('unix://'.$hostname);
                            break;
            default:        return false;
        }

        if (!$this->socket)
            return false;

        $this->connected = true;
    }

    // }}}
    // {{{ __destruct()

    /**
     * Class destructor
     *
     * @access  private
     */
    private function __destruct()
    {
        if ($this->connected) {
            socket_close($this->socket);
            $this->connected = false;
        }
    }

    // }}}
    // {{{ close()
    
    /**
     * Closes the socket connection
     *
     * @access  private
     */
    private function close()
    {
        if ($this->connected) {
            socket_close($this->socket);
            $this->connected = false;
        }
    }

    // }}}
    // {{{ write()

    /**
     * Write data onto the socket
     *
     * @access  private
     * @param   String  $data       Data to write
     * @return  Number of bytes actually written
     */
    private function write($data)
    {
        return socket_write($this->socket, $data, strlen($data);
    }

    // }}}
    // {{{ read()
    
    /**
     * Read data from the socket
     *
     * @access  private
     * @param   int     $bytes      Number of bytes to read
     * @return  Bytes read
     */
    private function read($bytes)
    {
        return socket_read($this->socket, $bytes);
    }

    // }}}
}

/* vim: set expandtab tabstop=4 shiftwidth=4 softtabstop=4: */

