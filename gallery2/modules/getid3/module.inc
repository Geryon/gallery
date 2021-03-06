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
 * @version $Revision: 12732 $ $Date: 2006-01-22 15:03:13 -0500 (Sun, 22 Jan 2006) $
 * @package Getid3
 * @author Don Willingham <donwillingham@users.sf.net>
 */

/**
 * getID3() ID3 metadata Module
 *
 * This module provides the getID3 metadata toolkit for Gallery
 *
 * @package Getid3
 */
class Getid3Module extends GalleryModule {

    function Getid3Module() {
	global $gallery;

	$this->setId('getid3');
	$this->setName($gallery->i18n('Getid3'));
	$this->setDescription($gallery->i18n('A toolkit for getting id3 tag information'));
	$this->setVersion('0.9.0');
	$this->setGroup('data', $gallery->i18n('Extra Data'));
	$this->setCallbacks('getSiteAdminViews');
	$this->setRequiredCoreApi(array(7, 1));
	$this->setRequiredModuleApi(array(3, 0));
    }

    /**
     * @see GalleryModule::upgrade()
     */
    function upgrade($currentVersion) {
	global $gallery;

	if (!isset($currentVersion)) {
	    /* Initial install */
	    GalleryCoreApi::requireOnce('modules/getid3/classes/Getid3Helper.class');
	    $ret = Getid3Helper::setDefaultProperties(GETID3_SUMMARY);
	    if ($ret) {
	        return $ret->wrap(__FILE__, __LINE__);
	    }

	    $ret = Getid3Helper::setDefaultProperties(GETID3_DETAILED);
	    if ($ret) {
	        return $ret->wrap(__FILE__, __LINE__);
	    }

	} else {
	    switch ($currentVersion) {
	    case '0.0.1':
	    case '0.9.0':
	    case '0.9.9':
	    case '1.0.0':
	        break;
	    default:
	        return GalleryCoreApi::error(ERROR_BAD_PLUGIN, __FILE__, __LINE__,
	                                    sprintf('Unknown module version %s', $currentVersion));
	    }
	}

	return null;
    }

    /**
     * @see GalleryModule::performFactoryRegistrations()
     */
    function performFactoryRegistrations() {
	/* Register our graphics class with the factory */
	$ret = GalleryCoreApi::registerFactoryImplementation(
	    'Getid3Interface_1_0', 'Getid3Extractor', 'Getid3',
	    'modules/getid3/classes/Getid3Extractor.class', 'getid3', null);
	if ($ret) {
	    return $ret->wrap(__FILE__, __LINE__);
	}

	$ret = GalleryCoreApi::registerFactoryImplementation(
	    'GalleryToolkit', 'Getid3Toolkit', 'Getid3',
	    'modules/getid3/classes/Getid3Toolkit.class', 'getid3', null);
	if ($ret) {
	    return $ret->wrap(__FILE__, __LINE__);
	}

	$ret = GalleryCoreApi::registerFactoryImplementation('ItemAddOption',
	    'Getid3DescriptionOption', 'Getid3DescriptionOption',
	    'modules/getid3/Getid3DescriptionOption.inc', 'getid3', null);
	if ($ret) {
	    return $ret->wrap(__FILE__, __LINE__);
	}


	return null;
    }

    /**
     * @see GalleryModule::activate
     */
    function activate($postActivationEvent=true) {
	global $gallery;
	$ret = GalleryCoreApi::registerToolkitProperty(
	        'Getid3',
	        array('video/x-msvideo'),
	        'originationTimestamp',
	        'int',
	        $gallery->i18n('Get the origination timestamp'));
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null);
	}


	list ($ret, $redirect) = parent::activate($postActivationEvent);
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null);
	}

	return array(null, $redirect);
    }

    /**
     * @see GalleryModule::deactivate()
     */
    function deactivate() {
	list ($ret, $redirect) = parent::deactivate();
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null);
	}

	/* Unregister all of our properties and operations */
	$ret = GalleryCoreApi::unregisterToolkit('Getid3');
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null);
	}

	return array(null, $redirect);
    }

    /**
     * @see GalleryModule::getSiteAdminViews()
     */
    function getSiteAdminViews() {
	global $gallery;

	return array(null,
	             array(array('name' => $this->translate('Getid3'),
	                         'view' => 'getid3:AdminGetid3')));
    }
}
?>
