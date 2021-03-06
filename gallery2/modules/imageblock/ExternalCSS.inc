<?php
/*
 * $RCSfile$
 *
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2006 Bharat Mediratta
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA  02110-1301, USA.
 */
/**
 * @version $Revision: 12540 $ $Date: 2006-01-09 23:44:30 -0500 (Mon, 09 Jan 2006) $
 * @package ImageBlock
 * @author Alan Harder <alan.harder@sun.com>
 */

/**
 * Generate CSS required if image frames are used with external image block.
 *
 * @package ImageBlock
 * @subpackage UserInterface
 */
class ExternalCSSController extends GalleryController {

    /**
     * @see GalleryController::handleRequest
     */
    function handleRequest($form) {
	list ($ret, $imageframe) = GalleryCoreApi::newFactoryInstance('ImageFrameInterface_1_1');
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null);
	}
	if (isset($imageframe)) {
	    $frames = GalleryUtilities::getRequestVariables('frames');
	    if (empty($frames)) {
		$frames = array();
		foreach (array('albumFrame', 'itemFrame') as $key) {
		    list ($ret, $frame) =
			GalleryCoreApi::getPluginParameter('module', 'imageblock', $key);
		    if ($ret) {
			return array($ret->wrap(__FILE__, __LINE__), null);
		    }
		    if (!empty($frame)) {
			$frames[] = $frame;
		    }
		}
		if (!empty($frames)) {
		    GalleryUtilities::putRequestVariable('frames', implode('|', $frames));
		}
	    }
	}
	return array(null,
		     array('delegate' => array('view' =>
				empty($frames) ? 'imageblock.ExternalCSS' : 'imageframe.CSS'),
			   'status' => array(), 'error' => array()));
    }
}

/**
 * @package ImageBlock
 * @subpackage UserInterface
 */
class ExternalCSSView extends GalleryView {

    /**
     * @see GalleryView::isImmediate
     */
    function isImmediate() {
	return true;
    }

    /**
     * @see GalleryView::renderImmediate
     */
    function renderImmediate($status, $error) {
	header('Content-type: text/html');
	header('Expires: ' . GalleryUtilities::getHttpDate(time() + 2592000));
	print '<style type="text/css"> img.giThumbnail { border-style: none; } </style>';
	print "\n";
	return null;
    }
}
?>
