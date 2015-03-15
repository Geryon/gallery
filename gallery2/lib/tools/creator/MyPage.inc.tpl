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
 * @package {$ucModuleId}
 * @subpackage UserInterface
 * @author {$authorFullName}
 */

/**
 * Handle input from our sample page
 *
 * @package {$ucModuleId}
 * @subpackage UserInterface
 *
 */
class MyPageController extends GalleryController {ldelim}
    /**
     * @see GalleryController::handleRequest()
     */
    function handleRequest($form) {ldelim}
	global $gallery;

	$itemId = GalleryUtilities::getRequestVariables('itemId');

	$redirect = array();
	$status = array();
	$error = array();
	if (isset($form['action']['save'])) {ldelim}
	    $ret = GalleryCoreApi::removeMapEntry('{$mapName}', array('itemId' => $itemId));
	    if ($ret) {ldelim}
	        return array($ret->wrap(__FILE__, __LINE__), null);
	    {rdelim}

	    $ret = GalleryCoreApi::addMapEntry(
                '{$mapName}',
                array('itemId' => $itemId, 'itemValue' => $form['value']));
	    if ($ret) {ldelim}
	        return array($ret->wrap(__FILE__, __LINE__), null);
	    {rdelim}

	    /* Send the user to a confirmation page, for now */
	    $redirect['view'] = '{$moduleId}.MyPage';
	    $redirect['itemId'] = (int)$itemId;
	    $status['added'] = 1;
	{rdelim}

	$results['status'] = $status;
	$results['error'] = $error;
	$results['redirect'] = $redirect;

	return array(null, $results);
    {rdelim}
{rdelim}

/**
 * This is a sample page generated by the Gallery 2 module creator.
 *
 * @package {$ucModuleId}
 * @subpackage UserInterface
 *
 */
class MyPageView extends GalleryView {ldelim}

    /**
     * @see GalleryView::loadTemplate
     */
    function loadTemplate(&$template, &$form) {ldelim}
	/* Load our item */
	list ($ret, $item) = $this->_getItem();
	if ($ret) {ldelim}
	    return array($ret->wrap(__FILE__, __LINE__), null);
	{rdelim}

	$MyPage = array();
	$MyPage['item'] = (array)$item;
	GalleryCoreApi::requireOnce('modules/{$moduleId}/classes/MyPageHelper.class');
	list ($ret, $MyPage['value']) = MyPageHelper::getItemValue($item->getId());
	if ($ret) {ldelim}
	    return array($ret->wrap(__FILE__, __LINE__), null);
	{rdelim}

	$template->setVariable('MyPage', $MyPage);

	return array(null, array('body' => 'modules/{$moduleId}/templates/MyPage.tpl'));
    {rdelim}
{rdelim}
?>