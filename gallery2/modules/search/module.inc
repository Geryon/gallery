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
 * @package Search
 * @author Bharat Mediratta <bharat@menalto.com>
 */

/**
 * Search Module
 *
 * This module provides support for searching the Gallery
 *
 * @package Search
 */
class SearchModule extends GalleryModule {

    function SearchModule() {
	global $gallery;

	$this->setId('search');
	$this->setName($gallery->i18n('Search'));
	$this->setDescription($gallery->i18n('Search your Gallery'));
	$this->setVersion('1.0.1');
	$this->setGroup('gallery', $gallery->i18n('Gallery'));
	$this->setCallbacks('');
	$this->setRequiredCoreApi(array(7, 0));
	$this->setRequiredModuleApi(array(3, 0));
    }
}
?>
