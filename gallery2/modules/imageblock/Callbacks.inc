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
 * @author Bharat Mediratta <bharat@menalto.com>
 */
class ImageBlockCallbacks {
    /**
     */
    function callback($params, &$smarty, $callback, $userId=null) {
	GalleryCoreApi::requireOnce('modules/imageblock/classes/ImageBlockHelper.class');

	switch($callback) {
	case 'LoadImageBlock':
	    /* Pick up our overrides */
	    if (!$params['useDefaults']) {
		/*
		 * If we're overriding the defaults, then overlay our changes on top of the
		 * defaults and pass in a complete 'show' parameter.
		 */
		list ($ret, $moduleParams) =
		    GalleryCoreApi::fetchAllPluginParameters('module', 'imageblock');
		if ($ret) {
		    return $ret->wrap(__FILE__, __LINE__);
		}

		$showMap = array_flip(explode('|', $moduleParams['show']));
		foreach (array('showHeading' => 'heading',
			       'showTitle' => 'title', 'showViews' => 'views',
			       'showOwner' => 'owner', 'showDate' => 'date')
			 as $blockVarName => $showMapName) {
		    if (empty($params[$blockVarName])) {
			unset($showMap[$showMapName]);
		    } else {
			$showMap[$showMapName] = 1;
		    }
		}

		$params['show'] = implode('|', array_keys($showMap));
	    }

	    $vars =& $smarty->_tpl_vars;
	    if (isset($vars['theme']['actingUserId'])) {
		/* Support guest-preview mode */
		$params['userId'] = $vars['theme']['actingUserId'];
	    }

	    /*
	     * ImageBlockHelper expects a GalleryTemplate, but Smarty callbacks don't provide
	     * one, so create a temporary wrapper.
	     */
	    $template = new GalleryTemplate(dirname(__FILE__), false);
	    $template->_smarty =& $smarty;

	    /* Remove null parameters */
	    foreach (array_keys($params) as $key) {
		if (!isset($params[$key])) {
		    unset($params[$key]);
		}
	    }

	    $ret = ImageBlockHelper::loadImageBlocks($template, $params);
	    if ($ret) {
		return $ret->wrap(__FILE__, __LINE__);
	    }

	    return null;
	}

	return GalleryCoreApi::error(ERROR_BAD_PARAMETER, __FILE__, __LINE__);
    }
}
?>
