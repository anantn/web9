<?php

/**
 * PEAR::Net_9P_Base_Stat
 *
 * This class represents a Stat, used in the
 * 9P Protocol.
 *
 * @category  Net
 * @package   Net_9P
 * @version   1.0
 * @author    Anant Narayanan <anant@php.net>
 * @copyright 2007 Anant Narayanan <anant@php.net>
 * @license   http://www.gnu.org/copyleft/lesser.html  LGPL License 2.1
 */

class Net_9P_Base_Stat
{
    // {{{ properties
    
    /**
     * sType: For kernel use
     *
     * @var int
     * @access public
     */
    public $sType;

    /**
     * dev: For kernel use
     *
     * @var int
     * @access public
     */
    public $dev;

    /**
     * Qid associated with the stat
     *
     * @var Net_9P_Qid
     * @access public
     */
    public $qid;

    /**
     * File Mode
     *
     * @var int
     * @access public
     */
    public $mode;

    /**
     * Last access time
     *
     * @var int
     * @access public
     */
    public $atime;

    /**
     * Last modification time
     *
     * @var int
     * @access public
     */
    public $mtime;

    /**
     * Length of file in bytes
     *
     * @var int
     * @access public
     */
    public $length;

    /**
     * File name
     * Is '/' if file is root of 9P server
     *
     * @var String
     * @access public
     */
    public $sName;

    /**
     * Owner name
     *
     * @var String
     * @access public
     */
    public $uid;

    /**
     * Group name
     *
     * @var String
     * @access public
     */
    public $gid;

    /**
     * Name of user who last modified the file 
     *
     * @var String
     * @access public
     */
    public $muid;

    // }}}
    // {{{ __construct()

    /**
     * Sets the class properties
     *
     * @access  public
     * @param   int         $type       sType: For kernel use
     * @param   int         $dev        dev: For kernel use
     * @param   Net_9P_Qid  $qid        Qid associated with the stat
     * @param   int         $mode       File mode
     * @param   int         $atime      Last access time
     * @param   int         $mtime      Last modification time
     * @param   int         $length     Length of file in bytes
     * @param   String      $name       Name of file
     * @param   String      $uid        Owner name
     * @param   String      $gid        Group name
     * @param   String      $muid       Name of user who last modified the file
     */
    public function __construct($type, $dev, $qid, $mode, $atime, $mtime, $length, $name, $uid, $gid, $muid)
    {
        $this->sType = $type;
        $this->dev = $dev;
        $this->qid = $qid;
        $this->mode = $mode;
        $this->atime = $atime;
        $this->mtime = $mtime;
        $this->length = $length;
        $this->sName = $name;
        $this->uid = $uid;
        $this->gid = $gid;
        $this->muid = $muid;
    }

    // }}}
}

/* vim: set expandtab tabstop=4 shiftwidth=4 softtabstop=4: */

