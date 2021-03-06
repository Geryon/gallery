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
 * @package Comment
 * @author Johannes Schmittat <Johannes@Schmittat.com>
 * @author Alan Harder <alan.harder@sun.com>
 */

/**
 * @package Comment
 * @subpackage UserInterface
 */
class CommentSiteAdminController extends GalleryController {

    /**
     * @see GalleryController::handleRequest
     */
    function handleRequest($form) {
	$ret = GalleryCoreApi::assertUserIsSiteAdministrator();
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null);
	}

	$status = $error = array();
	if (isset($form['action']['save'])) {
	   if (!isset($form['show']) || !is_numeric($form['show'])) {
		$error[] = 'form[error][show]';
	    }

	    if (empty($error)) {
		$ret = GalleryCoreApi::setPluginParameter('module', 'comment',
							  'comments.show', $form['show']);
		if ($ret) {
		    return array($ret->wrap(__FILE__, __LINE__), null);
		}
		foreach (array('captcha', 'latest') as $key) {
		    $ret = GalleryCoreApi::setPluginParameter('module', 'comment',
			'comments.' . $key, (isset($form[$key]) && $form[$key]) ? 1 : 0);
		    if ($ret) {
			return array($ret->wrap(__FILE__, __LINE__), null);
		    }
		}
		$status['saved'] = 1;
	    }
	} /* else $form['action']['reset'] */

	/* Figure out where to redirect upon success */
	$method = empty($error) ? 'redirect' : 'delegate';
	$results[$method]['view'] = 'core.SiteAdmin';
	$results[$method]['subView'] = 'comment.CommentSiteAdmin';
	$results['status'] = $status;
	$results['error'] = $error;

	return array(null, $results);
    }
}

/**
 * @package Comment
 * @subpackage UserInterface
 */
class CommentSiteAdminView extends GalleryView {

    /**
     * @see GalleryView::loadTemplate
     */
    function loadTemplate(&$template, &$form) {
	$ret = GalleryCoreApi::assertUserIsSiteAdministrator();
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null);
	}

	if ($form['formName'] != 'CommentSiteAdmin') {
	    $form['formName'] = 'CommentSiteAdmin';
	    foreach (array('latest', 'show', 'captcha') as $key) {
		list ($ret, $value) =
		    GalleryCoreApi::getPluginParameter('module', 'comment', 'comments.' . $key);
		if ($ret) {
		    return array($ret->wrap(__FILE__, __LINE__), null);
		}
		$form[$key] = $value;
	    }
	} else {
	    foreach (array('latest', 'captcha') as $key) {
		if (!isset($form[$key])) {
		    $form[$key] = false;
		}
	    }
	}

	list ($ret, $pluginInstances) =
	    GalleryCoreApi::getAllFactoryImplementationIds('GalleryValidationPlugin');
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null);
	}
	$form['canValidate'] = !empty($pluginInstances);

	$template->setVariable('controller', 'comment.CommentSiteAdmin');
	return array(null,
		     array('body' => 'modules/comment/templates/CommentSiteAdmin.tpl'));
    }
}
?>
