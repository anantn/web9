<?php

/**
 * PEAR::Net_9P_Qid
 *
 * This class represents a Qid, used in the
 * 9P Protocol.
 *
 * @category  Net
 * @package   Net_9P
 * @version   1.0
 * @author    Anant Narayanan <anant@php.net>
 * @copyright 2007 Anant Narayanan <anant@php.net>
 * @license   http://www.gnu.org/copyleft/lesser.html  LGPL License 2.1
 */

class Net_9P_Qid
{
    // {{{ properties
    
    /**
     * Qid type
     *
     * @var int
     * @access public
     */
    public $qType;

    /**
     * Qid version for a given path
     *
     * @var int
     * @access public
     */
    public $version;

    /**
     * Server's unique identification for the associated file
     *
     * @var int
     * @access public
     */
    public $path;

    // }}}
    // {{{ __construct()

    /**
     * Sets the class properties
     *
     * @access  public
     * @param   int     $type       Qid type
     * @param   int     $version    Qid version for a given path
     * @param   int     $path       Server's unique identification for the associated file
     */
    public function __construct($type, $version, $path)
    {
        $this->qType = $type;
        $this->version = $version;
        $this->path = $path;
    }

    // }}}
}

/* vim: set expandtab tabstop=4 shiftwidth=4 softtabstop=4: */
