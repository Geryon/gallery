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
 * @version $Revision: 12632 $ $Date: 2006-01-16 20:52:05 -0500 (Mon, 16 Jan 2006) $
 * @package Thumbnail
 * @author Alan Harder <alan.harder@sun.com>
 */

/**
 * @package Thumbnail
 * @subpackage UserInterface
 */
class ThumbnailSiteAdminController extends GalleryController {

    /**
     * @see GalleryController::handleRequest
     */
    function handleRequest($form) {
	GalleryCoreApi::requireOnce('modules/thumbnail/classes/ThumbnailHelper.class');

	$ret = GalleryCoreApi::assertUserIsSiteAdministrator();
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null);
	}

	$status = '';
	if (isset($form['action']['add'])) {
	    if (empty($form['mimeType'])) {
		return array(null, $this->_exitStatus('mime_error'));
	    }
	    list ($ret, $mimeMap) = ThumbnailHelper::fetchToolkitSupport();
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null);
	    }
	    if (isset($mimeMap[$form['mimeType']])) {
		return array(null, $this->_exitStatus('mime_duplicate'));
	    }
	}

	if (isset($form['action']['add']) && $form['image']=='new') {
	    if (empty($form['tmp_name'][1])) {
		return array(null, $this->_exitStatus('file_error'));
	    }
	    list ($ret) = ThumbnailHelper::addItem(
		$form['mimeType'], null, $form['name'][1], $form['tmp_name'][1], $form['type'][1]);
	    if ($ret && ($ret->getErrorCode() & ERROR_UNSUPPORTED_FILE_TYPE)) {
		$status = 'imagemime_error';
	    } else if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null);
	    } else {
		$status = 'add';
	    }
	} else if (isset($form['action']['add'])) { /* $form['image']=='old' */
	    $ret = ThumbnailHelper::updateItem($form['oldimage'], $form['mimeType'], true);
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null);
	    }
	    $status = 'addMime';
	} else if (isset($form['action']['delete']) && isset($form['delete']['itemId'])) {
	    if (isset($form['delete']['mimeType'])) {
		$ret = ThumbnailHelper::updateItem(
		    $form['delete']['itemId'], $form['delete']['mimeType'], false);
		if ($ret) {
		    return array($ret->wrap(__FILE__, __LINE__), null);
		}
		$status = 'deleteMime';
	    } else {
		list ($ret, $item) = GalleryCoreApi::loadEntitiesById($form['delete']['itemId']);
		if ($ret) {
		    return array($ret->wrap(__FILE__, __LINE__), null);
		}
		$itemMimeTypesList = $item->getItemMimeTypesList();
		$ret = GalleryCoreApi::deleteEntityById($form['delete']['itemId']);
		if ($ret) {
		    return array($ret->wrap(__FILE__, __LINE__), null);
		}
		$ret = GalleryCoreApi::unregisterToolkitOperation('Thumbnail',
				       'convert-to-image/jpeg', $itemMimeTypesList);
		if ($ret) {
		    return array($ret->wrap(__FILE__, __LINE__), null);
		}
		$status = 'delete';
	    }
	}

	return array(null, $this->_exitStatus($status));
    }

    function _exitStatus($status) {
	$results['redirect']['view'] = 'core.SiteAdmin';
	$results['redirect']['subView'] = 'thumbnail.ThumbnailSiteAdmin';
	$results['status'] = array($status => 1);
	$results['error'] = array();
	return $results;
   }
}

/**
 * @package Thumbnail
 * @subpackage UserInterface
 */
class ThumbnailSiteAdminView extends GalleryView {

    /**
     * @see GalleryView::loadTemplate
     */
    function loadTemplate(&$template, &$form) {
	GalleryCoreApi::requireOnce('modules/thumbnail/classes/ThumbnailHelper.class');

	$ret = GalleryCoreApi::assertUserIsSiteAdministrator();
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null);
	}

	if ($form['formName'] != 'ThumbnailSiteAdmin') {
	    $form['formName'] = 'ThumbnailSiteAdmin';
	}

	list ($ret, $form['mimeMap']) = ThumbnailHelper::getMimeTypeMap();
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null);
	}

	list ($ret, $list) = ThumbnailHelper::fetchThumbnails();
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null);
	}
	$form['list'] = array();
	if (!empty($list)) {
	    list ($ret, $thumbs) = GalleryCoreApi::loadEntitiesById(array_keys($list));
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null);
	    }
	    foreach ($thumbs as $thumb) {
		$data = (array)$thumb;
		$data['itemMimeTypesList'] = $thumb->getItemMimeTypesList();
		$form['list'][] = $data;
	    }
	}

	list ($ret, $toolkitMime, $form['badMime']) = ThumbnailHelper::fetchToolkitSupport(false);
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null);
	}
	unset($toolkitMime['Thumbnail']);
	$form['operationSupport'] = $toolkitMime;

	/* Set the form's encoding type since we're uploading binary files */
	if ($template->hasVariable('SiteAdmin')) {
	    $SiteAdmin =& $template->getVariableByReference('SiteAdmin');
	    $SiteAdmin['enctype'] = 'multipart/form-data';
	} else {
	    $SiteAdmin['enctype'] = 'multipart/form-data';
	    $template->setVariable('SiteAdmin', $SiteAdmin);
	}

	$template->setVariable('controller', 'thumbnail.ThumbnailSiteAdmin');
	return array(null,
		     array('body' => 'modules/thumbnail/templates/ThumbnailSiteAdmin.tpl'));
    }
}
?>
