<?php
/**
 * File: $Id: image.php 12865 2006-02-05 04:51:55Z andy_st $
 * 
 * Gallery2 imageblock wrapper
 * 
 * @package Xaraya eXtensible Management System
 * @copyright (C) 2003 by the Xaraya Development Team.
 * @license GPL {@link http://www.gnu.org/licenses/gpl.html}
 * @link http://www.xaraya.com
 *
 * @subpackage gallery2
 * @author Andy Staudacher 
 */
 
// Load the xarGallery2Helper class
include_once(dirname(__FILE__) .'/../xargallery2helper.php');

/**
 * initialise block
 */
function gallery2_imageblock_init()
{
  return true;
} 

/**
 * get information on block
 */
function gallery2_imageblock_info()
{ 
  return array('text_type' => 'image',
	       'module' => 'gallery2',
	       'text_type_long' => xarML('Show an image from G2'),
	       'allow_multiple' => true,
	       'form_content' => false,
	       'form_refresh' => false,
	       'show_preview' => false);
} 

/**
 * display block
 */
function gallery2_imageblock_display($blockinfo)
{ 
  if (!xarSecurityCheck('ReadGallery2', 0, 'Block', $blockinfo['title'])) {return;}

  // first check if the module has been configured
  if(!xarGallery2Helper::isConfigured()) {
    return;
  }
  
  // init G2 if not already done so
  if (!xarGallery2Helper::init(false, true)) {
    return;
  }

  // Get variables from content block.
  // Content is a serialized array for legacy support, but will be
  // an array (not serialized) once all blocks have been converted.
  if (!is_array($blockinfo['content'])) {
    $vars = @unserialize($blockinfo['content']);
  } else {
    $vars = $blockinfo['content'];
  }
  
  // Defaults
  if (!isset($vars['ibtype']) || empty($vars['ibtype'])) {
    $vars['ibtype'] = 0;
  } 
  if (!isset($vars['ibshowtitle']) || empty($vars['ibshowtitle'])) {
    $vars['ibshowtitle'] = 0;
  }
  if (!isset($vars['ibshowdate']) || empty($vars['ibshowdate'])) {
    $vars['ibshowdate'] = 0;
  }
  if (!isset($vars['ibshowviews']) || empty($vars['ibshowviews'])) {
    $vars['ibshowviews'] = 0;
  }
  if (!isset($vars['ibshowowner']) || empty($vars['ibshowowner'])) {
    $vars['ibshowowner'] = 0;
  } 
  

  // params for G2
  $params = array();

  switch($vars['ibtype']) {
  case 0:
    $params['blocks'] = 'randomImage';
    break;
  case 1:
    $params['blocks'] = 'recentImage';
    break;
  case 2:
    $params['blocks'] = 'viewedImage';
    break;
  case 3:
    $params['blocks'] = 'randomAlbum';
    break;
  case 4:
    $params['blocks'] = 'recentAlbum';
    break;
  case 5:
    $params['blocks'] = 'viewedAlbum';
    break;
  default:
    $params['blocks'] = 'randomImage';
  }
    
  $params['show'] = array();
  if ($vars['ibshowtitle']) {
    $params['show'][] = 'title';
  }
  if ($vars['ibshowdate']) {
    $params['show'][] = 'date';
  }
  if ($vars['ibshowviews']) {
    $params['show'][] = 'views';
  }
  if ($vars['ibshowowner']) {
    $params['show'][] = 'owner';
  } 

  if (count($params['show']) > 0) {
    $params['show'] = implode('|', $params['show']);
  } else {
    $params['show'] = 'none';
  }

  if (isset($vars['ibheading']) && $vars['ibheading']) {
    $params['heading'] = 1;
  }

  // render and get the imageblock html
  list ($ret, $blockinfo['content']) = GalleryEmbed::getImageBlock($params);
  if (!empty($ret)) {
    $msg = xarML('G2 did not return a success status upon an imageblock request. Here is the error message from G2: <br /> [#(1)]', $ret->getAsHtml());
    xarErrorSet(XAR_SYSTEM_EXCEPTION, 'FUNCTION_FAILED', new SystemException($msg));
    return;
  }

  xarGallery2Helper::done(); // register shutdown function _done()

  return $blockinfo;
} 

?>