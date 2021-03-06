<?php
/*
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Id: MainPage.inc 12540 2006-01-10 04:44:30Z mindless $
 */

/**
 * Controller for the initial page displaying link to available actions.
 *
 * @author Jozef Selesi <selesi at gmail dot com>
 *
 * @version $Revision: 12540 $ $Date: 2006-01-09 23:44:30 -0500 (Mon, 09 Jan 2006) $
 * @package RepositoryTools
 * @subpackage Controllers
 * @module MainPageController
 */
class MainPageControllerAndView extends RepositoryControllerAndView {

    /**
     * Displays the available actions in the repository tools.
     *
     * @return object GalleryStatus a status code
     */
     function showAvailableActions() {
	global $gallery;
	$data = array();
	$urlGenerator =& $gallery->getUrlGenerator();

	/* Make sure logged in user is an administrator. */
	list ($ret, $data['isSiteAdmin']) = GalleryCoreApi::isUserInSiteAdminGroup();
	if ($ret) {
	    print $ret->getAsHtml();
	    return;
	}

	/* Create links for repository actions. */
	$data['packagePluginLink'] = $urlGenerator->generateUrl(array(
	    'controller' => 'PackagePlugin', 'action' => 'showAvailablePlugins'));
	$data['packageAllPlugins'] = $urlGenerator->generateUrl(array(
	    'controller' => 'PackagePlugin', 'action' => 'packagePlugins'));
	$data['generateIndex'] = $urlGenerator->generateUrl(array(
	    'controller' => 'IndexGenerator', 'action' => 'generate'));
	$data['browseRepository'] = $urlGenerator->generateUrl(array(
	    'controller' => 'RepositoryManager', 'action' => 'browse'));

	$ret = $this->showView('MainPage', $data);
	if ($ret) {
	    return $ret->wrap(__FILE__, __LINE__);
	}
	return null;
    }
}
?>