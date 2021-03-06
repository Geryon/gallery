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
 * @version $Revision: 12707 $ $Date: 2006-01-21 12:58:48 -0500 (Sat, 21 Jan 2006) $
 * @package SizeLimit
 * @author Felix Rabinovich <felix@rabinovich.org>
 */

/**
 * This module sets size limit for the picture
 *
 * @package SizeLimit
 */
class SizeLimitModule extends GalleryModule {

    function SizeLimitModule() {
	global $gallery;

	$this->setId('sizelimit');
	$this->setName($gallery->i18n('Size Limit'));
	$this->setDescription($gallery->i18n('Define picture size limit'));
	$this->setVersion('1.0.1');
	$this->setGroup('display', $gallery->i18n('Display'));
	$this->setCallbacks('registerEventListeners');
	$this->setRequiredCoreApi(array(7, 0));
	$this->setRequiredModuleApi(array(3, 0));
    }

    /**
     * @see GalleryModule::registerEventListeners()
     */
    function registerEventListeners() {
	GalleryCoreApi::registerEventListener('GalleryEntity::save',
	    new SizelimitModule(), true);   /* true=disableForUnitTest */
    }

    /**
     * @see GalleryModule::performFactoryRegistrations()
     */
    function performFactoryRegistrations() {
	$ret = GalleryCoreApi::registerFactoryImplementation(
	    'ItemEditOption', 'SizeLimitOption', 'SizeLimitOption',
	    'modules/sizelimit/SizeLimitOption.inc', 'sizelimit', array('ItemEditAlbum'));
	if ($ret) {
	    return $ret->wrap(__FILE__, __LINE__);
	}

	$ret = GalleryCoreApi::registerFactoryImplementation(
	    'ItemAddOption', 'SetSizeOption', 'SetSizeOption',
	    'modules/sizelimit/SetSizeOption.inc', 'sizelimit', null);
	if ($ret) {
	    return $ret->wrap(__FILE__, __LINE__);
	}

	return null;
    }

    /**
     * Event handler for GalleryEntity::save event
     * Copy dimension / filesize limits when the album is created
     *
     * @param object GalleryEvent the event
     * @return object GalleryStatus a status code
     */
    function handleEvent($event) {
	if ($event->getEventName() == 'GalleryEntity::save') {
	    $album = $event->getEntity();
	    if (GalleryUtilities::isA($album, 'GalleryAlbumItem') &&
		    $album->testPersistentFlag(STORAGE_FLAG_NEWLY_CREATED)) {
		$parentId = (int)$album->getParentId();
		list ($ret, $params) = GalleryCoreApi::fetchAllPluginParameters(
		    'module', 'sizelimit', $parentId);
		if ($ret) {
		    return array($ret->wrap(__FILE__, __LINE__), null);
		}
		foreach ($params as $param => $value) {
		    $ret = GalleryCoreApi::setPluginParameter('module', 'sizelimit',
			$param, $value, $album->getId());
		    if ($ret) {
			return array($ret->wrap(__FILE__, __LINE__), null);
		    }
		}
	    }
	}
	return array(null, null);
    }
}
?>
