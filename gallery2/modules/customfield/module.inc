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
 * @package CustomField
 * @author Alan Harder <alan.harder@sun.com>
 */

/**
 * Custom fields
 *
 * This module provides custom fields for Gallery items.
 *
 * @package CustomField
 */
class CustomFieldModule extends GalleryModule {

    function CustomFieldModule() {
	global $gallery;

	$this->setId('customfield');
	$this->setName($gallery->i18n('Custom Fields'));
	$this->setDescription($gallery->i18n('Create custom data fields for Gallery items'));
	$this->setVersion('1.0.3');
	$this->setGroup('data', $gallery->i18n('Extra Data'));
	$this->setCallbacks('registerEventListeners|getSiteAdminViews|getItemSummaries');
	$this->setRequiredCoreApi(array(7, 0));
	$this->setRequiredModuleApi(array(3, 0));
    }

    /**
     * @see GalleryModule::upgrade()
     */
    function upgrade($currentVersion) {
	global $gallery;

	if (!empty($currentVersion) && version_compare($currentVersion, '1.0.1', '<')) {
	    $storage =& $gallery->getStorage();
	    $ret = $storage->configureStore($this->getId(), array('CustomFieldMap:1.0'));
	    if ($ret) {
		return $ret->wrap(__FILE__, __LINE__);
	    }
	}

	return null;
    }

    /**
     * @see GalleryModule::registerEventListeners()
     */
    function registerEventListeners() {
	GalleryCoreApi::requireOnce('modules/customfield/classes/CustomFieldHelper.class');
	GalleryCoreApi::registerEventListener('GalleryEntity::delete', new CustomFieldHelper());
    }

    /**
     * @see GalleryModule::getSiteAdminViews()
     */
    function getSiteAdminViews() {
	return array(null,
		     array(array('name' => $this->translate('Custom Fields'),
				 'view' => 'customfield.CustomFieldSiteAdmin')));
    }

    /**
     * @see GalleryModule::getItemSummaries()
     */
    function getItemSummaries($items, $permissions) {
	GalleryCoreApi::requireOnce('modules/customfield/classes/CustomFieldHelper.class');
	GalleryCoreApi::requireOnce('lib/smarty_plugins/modifier.markup.php');
	$summaries = array();

	list ($ret, $data) = CustomFieldHelper::fetchFieldValues($items, 'summary');
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null);
	}

	foreach ($data as $itemId => $fields) {
	    $list = array();
	    foreach ($fields as $field => $value) {
		$list[] = $field . ': ' . smarty_modifier_markup($value);
	    }
	    if (!empty($list)) {
		$summaries[$itemId] = implode("<br/>\n", $list);
	    }
	}
	return array(null, $summaries);
    }

    /**
     * @see GalleryModule::performFactoryRegistrations()
     */
    function performFactoryRegistrations() {
	/* Register our item edit plugin */
	$ret = GalleryCoreApi::registerFactoryImplementation(
	    'ItemEditPlugin', 'CustomFieldItemEdit', 'CustomFieldItemEdit',
	    'modules/customfield/CustomFieldItemEdit.inc', 'customfield', null);
	if ($ret) {
	    return $ret->wrap(__FILE__, __LINE__);
	}

	/* Register interface implementation */
	$ret = GalleryCoreApi::registerFactoryImplementation(
	    'CustomFieldInterface_1_0', 'CustomFieldHelper', 'CustomField',
	    'modules/customfield/classes/CustomFieldHelper.class', 'customfield', null);
	if ($ret) {
	    return $ret->wrap(__FILE__, __LINE__);
	}

	/* Register search implementation */
	$ret = GalleryCoreApi::registerFactoryImplementation(
	    'GallerySearchInterface_1_0', 'CustomFieldSearch', 'CustomField',
	    'modules/customfield/classes/CustomFieldSearch.class', 'customfield', null);
	if ($ret) {
	    return $ret->wrap(__FILE__, __LINE__);
	}

	return null;
    }
}
?>
