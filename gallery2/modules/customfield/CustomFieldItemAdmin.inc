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
 * @version $Revision: 12631 $ $Date: 2006-01-16 20:44:12 -0500 (Mon, 16 Jan 2006) $
 * @package CustomField
 * @subpackage UserInterface
 * @author Alan Harder <alan.harder@sun.com>
 */

GalleryCoreApi::requireOnce('modules/customfield/classes/CustomFieldHelper.class');

/**
 * This controller handles moving between admin modes in the Item Edit plugin,
 * by setting or clearning a session variable.  It also handles activation of
 * album-specific settings (copy in global settings) or reverting to global
 * settings for an album (clear album settings, remove field values).
 *
 * @package CustomField
 * @subpackage UserInterface
 */
class CustomFieldItemAdminController extends GalleryController {

    /**
     * @see GalleryController::handleRequest
     */
    function handleRequest() {
	global $gallery;
	$session =& $gallery->getSession();

	list ($adminMode, $itemId) = GalleryUtilities::getRequestVariables('cfAdmin', 'itemId');
	if (!empty($adminMode) && !empty($itemId)) {
	    $ret = GalleryCoreApi::assertHasItemPermission($itemId, 'core.edit');
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null);
	    }

	    /* Activate album-specific settings: */
	    if ($adminMode == 2) {
		/* Copy in global settings */
		list ($ret, $globalParam) = CustomFieldHelper::loadParameters();
		if ($ret) {
		    return array($ret->wrap(__FILE__, __LINE__), null);
		}
		$ret = CustomFieldHelper::saveParameters($globalParam, $itemId);
		if ($ret) {
		    return array($ret->wrap(__FILE__, __LINE__), null);
		}
		/* Mark setId on existing field values */
		list ($ret, $searchResults) = $gallery->search(
		    'SELECT [ChildEntity::id] FROM [ChildEntity] WHERE [ChildEntity::parentId]=?',
		    array($itemId));
		if ($ret) {
		    return array($ret->wrap(__FILE__, __LINE__), null);
		}
		$idList = array($itemId);
		while ($rec = $searchResults->nextResult()) {
		    $idList[] = $rec[0];
		}
		$ret = GalleryCoreApi::updateMapEntry(
		    'CustomFieldMap',
		    array('itemId' => $idList),
		    array('setId' => $itemId) );
		if ($ret) {
		    return array($ret->wrap(__FILE__, __LINE__), null);
		}
	    }

	    /* Revert to global settings: */
	    if ($adminMode == -2) {
		/* Remove album-specific settings */
		foreach (array('common', 'album', 'photo') as $set) {
		    $ret = GalleryCoreApi::removePluginParameter(
					   'module', 'customfield', $set, $itemId);
		}
		/* Clear setId for existing field values that can be retained */
		list ($ret, $globalParam) = CustomFieldHelper::loadParameters();
		if ($ret) {
		    return array($ret->wrap(__FILE__, __LINE__), null);
		}
		foreach (array('common' => 0, 'album' => 1, 'photo' => 2) as $set => $setType) {
		    $fieldList = array();
		    foreach ($globalParam[$set] as $it) {
			$fieldList[] = $it['field'];
		    }
		    if (empty($fieldList)) {
			continue;
		    }
		    $match = array('setId' => $itemId, 'field' => $fieldList);
		    if ($setType > 0) {
			$match['setType'] = $setType;
		    }
		    $ret = GalleryCoreApi::updateMapEntry(
			'CustomFieldMap', $match, array('setId' => 0));
		    if ($ret) {
			return array($ret->wrap(__FILE__, __LINE__), null);
		    }
		}
		/* Remove remaining field values */
		$ret = GalleryCoreApi::removeMapEntry(
		    'CustomFieldMap', array('setId' => $itemId));
		if ($ret) {
		    return array($ret->wrap(__FILE__, __LINE__), null);
		}
	    }

	    /* Set admin mode in session */
	    if ($adminMode > 0) {
		$session->put(CUSTOM_FIELD_SESSION_KEY, $itemId);
	    } else {
		$session->remove(CUSTOM_FIELD_SESSION_KEY);
	    }
	}

	$results['return'] = 1;
	$results['status'] = array();
	$results['error'] = array();

	return array(null, $results);
    }
}
?>
