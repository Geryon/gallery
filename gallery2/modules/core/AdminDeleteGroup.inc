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
 * @package GalleryCore
 * @subpackage UserInterface
 * @author Bharat Mediratta <bharat@menalto.com>
 */

/**
 * This controller will handle the deletion of a group
 *
 * @package GalleryCore
 * @subpackage UserInterface
 *
 */
class AdminDeleteGroupController extends GalleryController {

    /**
     * @see GalleryController::handleRequest
     */
    function handleRequest($form) {
	$ret = GalleryCoreApi::assertUserIsSiteAdministrator();
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null);
	}

	$results = $status = $error = array();
	$groupId = GalleryUtilities::getRequestVariables('groupId');

	if (isset($form['action']['cancel'])) {

	    /* Go back to the AdminGroups view */
	    $redirect['view'] = 'core.SiteAdmin';
	    $redirect['subView'] = 'core.AdminGroups';

	} else if (isset($form['action']['delete'])) {

	    /* Only allow users to delete GROUP_NORMAL groups. */
	    list ($ret, $group) = GalleryCoreApi::loadEntitiesById($groupId);
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null);
	    }

	    $ret = GalleryCoreApi::deleteEntityById($group->getId());
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null);
	    }
	    
	    /* Request a redirect to the confirmation screen */
	    $redirect['view'] = 'core.SiteAdmin';
	    $redirect['subView'] = 'core.AdminGroups';
	    $status['deletedGroup'] = $group->getGroupName();
	}

	if (!empty($redirect)) {
	    $results['redirect'] = $redirect;
	} else {
	    $results['delegate']['view'] = 'core.SiteAdmin';
	    $results['delegate']['subView'] = 'core.AdminDeleteGroup';
	}
	$results['status'] = $status;
	$results['error'] = $error;

	return array(null, $results);
    }
}

/**
 * This view will prompt for confirmation ot delete a group
 *
 * @package GalleryCore
 * @subpackage UserInterface
 */
class AdminDeleteGroupView extends GalleryView {

    /**
     * @see GalleryView::loadTemplate
     */
    function loadTemplate(&$template, &$form) {
	$ret = GalleryCoreApi::assertUserIsSiteAdministrator();
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null);
	}

	$groupId = GalleryUtilities::getRequestVariables('groupId');
	list ($ret, $group) = GalleryCoreApi::loadEntitiesById($groupId);
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null);
	}

        if ($form['formName'] != 'AdminDeleteGroup') {
	    /* First time around initialize our form */
	    $form['groupName'] = $group->getGroupName();
	    $form['formName'] = 'AdminDeleteGroup';
	}

	$AdminDeleteGroup = array();
	$AdminDeleteGroup['group'] = (array)$group;

	/* Render the HTML body */
	$template->setVariable('AdminDeleteGroup', $AdminDeleteGroup);
	$template->setVariable('controller', 'core.AdminDeleteGroup');

	return array(null,
		     array('body' => 'modules/core/templates/AdminDeleteGroup.tpl'));
    }
}
?>
