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
 * $Id: IndexGenerator.inc 12540 2006-01-10 04:44:30Z mindless $
 */

GalleryCoreApi::requireOnce('modules/core/classes/GalleryRepositoryUtilities.class');
GalleryCoreApi::requireOnce('lib/tools/repository/classes/RepositoryObject.class');
GalleryCoreApi::requireOnce('lib/tools/repository/classes/RepositoryIndex.class');

/**
 * Controller for the plugin packaging page.
 *
 * @author Jozef Selesi <selesi at gmail dot com>
 *
 * @version $Revision: 12540 $ $Date: 2006-01-09 23:44:30 -0500 (Mon, 09 Jan 2006) $
 * @package RepositoryTools
 * @subpackage Controllers
 * @module IndexGeneratorController
 */
class IndexGeneratorControllerAndView extends RepositoryControllerAndView {

    /**
     * Regenerates the repository index.
     *
     * @return object GalleryStatus a status code
     */
    function generate() {
	global $gallery;
	$index = new RepositoryIndex();
	$urlGenerator =& $gallery->getUrlGenerator();

	$repositoryDir = $gallery->getConfig('repository.path');

	$ret = $index->init($repositoryDir);
	if ($ret) {
	    return $ret->wrap(__FILE__, __LINE__);
	}

	list ($ret, $pluginCounts) = $index->generate();
	if ($ret) {
	    return $ret->wrap(__FILE__, __LINE__);
	}

	$viewData = array('moduleCount' => $pluginCounts['modules'],
			  'themeCount' => $pluginCounts['themes'],
			  'outputDir' => $repositoryDir,
			  'browseRepositoryLink' => $urlGenerator->generateUrl(
			      array('controller' => 'RepositoryManager', 'action' => 'browse')));

	$ret = $this->showView('IndexGenerationResults', $viewData);
	if ($ret) {
	    return $ret->wrap(__FILE__, __LINE__);
	}

	return null;
    }
}
?>
