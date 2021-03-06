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
 * @package Rearrange
 * @subpackage UserInterface
 * @author Alan Harder <alan.harder@sun.com>
 */

/**
 * This controller will handle rearranging the order of all album items at once.
 *
 * @package Rearrange
 * @subpackage UserInterface
 *
 */
class RearrangeItemsController extends GalleryController {

    /**
     * Move the selected items into the destination album, if all the
     * permissions are set correctly.
     */
    function handleRequest($form) {
	$itemId = GalleryUtilities::getRequestVariables('itemId');

	/* Check permissions */
	$ret = GalleryCoreApi::assertHasItemPermission($itemId, 'core.edit');
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null);
	}

	$status = array();
	$error = array();
	if (isset($form['action']['save'])) {
	    $list = explode(',', $form['list']);

	    list ($ret, $item) = GalleryCoreApi::loadEntitiesById($itemId);
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null);
	    }
	    list ($ret, $childIds) = GalleryCoreApi::fetchChildItemIds($item);
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null);
	    }
	    /* Verify valid input */
	    if (count($list) != count($childIds)) {
		return array(GalleryCoreApi::error(ERROR_BAD_PARAMETER, __FILE__, __LINE__), null);
	    }

	    list ($ret, $orderWeights) = GalleryCoreApi::fetchItemOrderWeights($childIds);
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null);
	    }

	    foreach ($list as $newIndex => $oldIndex) {
		if ($newIndex == $oldIndex) {
		    continue;
		}

		$ret = GalleryCoreApi::setItemOrderWeight($childIds[$oldIndex],
							  $orderWeights[$childIds[$newIndex]]);
		if ($ret) {
		    return array($ret->wrap(__FILE__, __LINE__), null);
		}
	    }

	    $event = GalleryCoreApi::newEvent('Gallery::ItemOrder');
	    $event->setData($itemId);
	    list ($ret) = GalleryCoreApi::postEvent($event);
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null);
	    }

	    $status['saved'] = 1;
	} /* else $form['action']['reset'] */

	$results['redirect'] = array('view' => 'core.ItemAdmin',
				     'subView' => 'rearrange.RearrangeItems',
				     'itemId' => $itemId);
	$results['status'] = $status;
	$results['error'] = $error;

	return array(null, $results);
    }
}

/**
 * This view will display an interface to reorder all album items at once.
 *
 * @package Rearrange
 * @subpackage UserInterface
 */
class RearrangeItemsView extends GalleryView {

    /**
     * @see GalleryView::loadTemplate
     */
    function loadTemplate(&$template, &$form) {
	$itemId = GalleryUtilities::getRequestVariables('itemId');

	/* Check permissions */
	$ret = GalleryCoreApi::assertHasItemPermission($itemId, 'core.edit');
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null);
	}

	if ($form['formName'] != 'RearrangeItems') {
	    $form['formName'] = 'RearrangeItems';
	}

	list ($ret, $item) = GalleryCoreApi::loadEntitiesById($itemId);
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null);
	}

	$RearrangeItems = array();
	$orderBy = $item->getOrderBy();
	if (empty($orderBy)) {
	    list ($ret, $orderBy) =
		GalleryCoreApi::getPluginParameter('module', 'core', 'default.orderBy');
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null);
	    }
	}
	if ($orderBy != 'orderWeight') {
	    $RearrangeItems['automaticOrderMessage'] = 1;
	} else {
	    list ($ret, $childIds) = GalleryCoreApi::fetchChildItemIds($item);
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null);
	    }
	    list ($ret, $childItems) = GalleryCoreApi::loadEntitiesById($childIds);
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null);
	    }
	    list ($ret, $thumbnails) = GalleryCoreApi::fetchThumbnailsByItemIds($childIds);
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null);
	    }

	    $children = array();
	    $maxWidth = $maxHeight = 0;
	    foreach ($childItems as $child) {
		$childId = $child->getId();
		$child = (array)$child;
		if (isset($thumbnails[$childId])) {
		    $thumb = $thumbnails[$childId];
		    if (!($thumb->getWidth() && $thumb->getHeight())) {
			list ($ret, $thumb) =
			    GalleryCoreApi::rebuildDerivativeCache($thumb->getId());
			if ($ret) {
			    return array($ret->wrap(__FILE__, __LINE__), null);
			}
		    }
		    $child['thumbnail'] = (array)$thumb;
		    list ($w, $h) = GalleryUtilities::shrinkDimensionsToFit(
			$thumb->getWidth(), $thumb->getHeight(), 100);
		    $maxWidth = max($w, $maxWidth);
		    $maxHeight = max($h, $maxHeight);
		}
		$children[] = $child;
	    }
	    $RearrangeItems['children'] = $children;
	    $RearrangeItems['count'] = count($children);
	    $RearrangeItems['maxWidth'] = $maxWidth + 5;
	    $RearrangeItems['maxHeight'] = $maxHeight + 5;

	    /* See if the album's theme has "rows" and/or "columns" */
	    list ($ret, $themeId) = GalleryCoreApi::fetchThemeId($item);
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null);
	    }
	    list ($ret, $theme) = GalleryCoreApi::loadPlugin('theme', $themeId);
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null);
	    }
	    list ($ret, $params) = $theme->fetchParameters($item->getId());
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null);
	    }
	    $RearrangeItems['rows'] = empty($params['rows']) ? 0 : $params['rows'];
	    $RearrangeItems['columns'] = empty($params['columns']) ? 0 : $params['columns'];
	}

	$template->setVariable('RearrangeItems', $RearrangeItems);
	$template->setVariable('controller', 'rearrange.RearrangeItems');
	$template->head('modules/rearrange/templates/Header.tpl');
	return array(null,
		     array('body' => 'modules/rearrange/templates/RearrangeItems.tpl'));
    }

    /**
     * @see GalleryView::getViewDescription()
     */
    function getViewDescription() {
	list ($ret, $module) = GalleryCoreApi::loadPlugin('module', 'rearrange');
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null);
	}
	return array(null, $module->translate('rearrange items'));
    }
}
?>
