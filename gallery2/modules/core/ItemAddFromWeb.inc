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
 * @version $Revision: 12823 $ $Date: 2006-01-29 08:39:38 -0500 (Sun, 29 Jan 2006) $
 * @package GalleryCore
 * @subpackage UserInterface
 * @author Bharat Mediratta <bharat@menalto.com>
 */

/**
 * This controller will handle the addition of an item as a children to
 * another item.
 *
 * @package GalleryCore
 * @subpackage UserInterface
 */
class ItemAddFromWeb extends ItemAddPlugin {

    /**
     * @see ItemAddPlugin::handleRequest
     */
    function handleRequest($form, &$item) {
	global $gallery;

	$status = array();
	$error = array();
	
	if (isset($form['action']['findFilesFromWebPage'])) {
	    /* Delegate back to the same view */
	} else if (isset($form['action']['addFromWebPage']) && empty($form['webPageUrls'])) {
	    GalleryUtilities::putRequestVariable('form[error][webPage][nothingSelected]', 1);
	} else if (isset($form['action']['addFromWebPage'])) {
	    $platform =& $gallery->getPlatform();
	    /* Pass referer checking */
	    if (!empty($form['webPage'])) {
		$extraHeaders =
		    array('Referer' => str_replace('&amp;', '&', $form['webPage']));
	    }
	    foreach (array_keys($form['webPageUrls']) as $url) {
		/* Copy the file locally */
		$tmpDir = $gallery->getConfig('data.gallery.tmp');
		$tmpFile = $platform->tempnam($tmpDir, 'add');

		$successfullyCopied = false;
		$url = GalleryUtilities::htmlEntityDecode($url);
		if (empty($extraHeaders)) {
		    $extraHeaders = array('Referer' => str_replace('&amp;', '&', $url));
		}
		list ($successfullyCopied, $response, $headers) =
		    GalleryCoreApi::fetchWebFile($url, $tmpFile, $extraHeaders);
		if ($successfullyCopied) {
		    /* Add it */
		    list ($ret, $mimeType, $fileName) = $this->_getMimeType($url, $headers, true);
		    if ($ret) {
			return array($ret->wrap(__FILE__, __LINE__), null, null);
		    }

		    list ($base) = GalleryUtilities::getFileNameComponents($fileName);
		    $title = (isset($form['set']['title']) && $form['set']['title']) ? $base : '';
		    $summary = (isset($form['set']['summary'])
			     && $form['set']['summary']) ? $base : '';
		    $description = (isset($form['set']['description'])
				 && $form['set']['description']) ? $base : '';

		    list ($ret, $newItem) = GalleryCoreApi::addItemToAlbum($tmpFile,
									   $fileName,
									   $title,
									   $summary,
									   $description,
									   $mimeType,
									   $item->getId());
		    if ($ret) {
			return array($ret->wrap(__FILE__, __LINE__), null, null);
		    }

		    $status['addedFiles'][] = array('fileName' => $url,
						    'id' => $newItem->getId(),
						    'warnings' => array());
		}
		@$platform->unlink($tmpFile);
	    }
	}

	return array(null, $error, $status);
    }

    /**
     * @see ItemAdd:loadTemplate
     */
    function loadTemplate(&$template, &$form, $item) {
	global $gallery;

	if ($form['formName'] != 'ItemAddFromWeb') {
	    /* First time around, load the form with item data */
	    $form['webPage'] = '';
	    $form['formName'] = 'ItemAddFromWeb';
	}

	if (isset($form['action']['findFilesFromWebPage'])) {
	    /* Download the web page then parse it to get the file list */
	    if (empty($form['webPage'])) {
		$form['error']['webPage']['missing'] = 1;
	    }
	    if (empty($form['error'])) {
		$baseUrlComponents = parse_url($form['webPage']);
		if (empty($baseUrlComponents['scheme']) || $baseUrlComponents['scheme'] != 'http') {
		    $form['error']['webPage']['invalid'] = 1;
		}
	    }
	    if (empty($form['error'])) {
		list ($successful, $buffer, $response, $headers, $actualUrl) =
		    GalleryCoreApi::fetchWebPage($form['webPage']);
		if (empty($buffer) || !$successful) {
		    $form['error']['webPage']['unavailable'] = 1;
		}
	    }
	    if (empty($form['error'])) {
		if (isset($actualUrl) && $actualUrl != $form['webPage']) {
		    $form['webPage'] = $actualUrl;
		    $baseUrlComponents = parse_url($form['webPage']);
		}

		/* Add path to the recent path list */
		$session =& $gallery->getSession();
		$recentUrls = $session->get('core.view.ItemAddFromWeb.recentUrls');
		$recentUrls[$form['webPage']] = 1;
		$session->put('core.view.ItemAddFromWeb.recentUrls', $recentUrls);

		$form['webPageUrls'] = array();
		if (isset($headers['Content-Type']) &&
			strncmp($headers['Content-Type'], 'text', 4)) {
		    /*
		     * If the buffer does not contain text(/html) then just offer
		     * to add this url, rather than parsing the contents.
		     */
		    list ($ret, $typeName) = $this->_getTypeName($form['webPage'], null, $headers);
		    if ($ret) {
			return array($ret->wrap(__FILE__, __LINE__), null, null);
		    }
		    if (!empty($typeName)) {
			$form['webPageUrls'][] = array('url' => $form['webPage'],
						       'itemType' => $typeName);
		    }
		} else {
		    /*
		     * Parse the buffer.  We match:
		     *  href="foo bar"    href='foo bar'   href=foo
		     *  src="foo bar"     src='foo bar'    src=foo
		     */
		    preg_match_all('/(src|href)\s*=\s*(?:\"(.*?)\"|\'(.*?)\'|([^\'\"\> ]*))/i',
				   $buffer, $matches);

		    $seenBefore = array();
		    for ($i = 0; $i < count($matches[1]); $i++) {
			$url = !empty($matches[2][$i]) ? $matches[2][$i]
				: (!empty($matches[3][$i]) ? $matches[3][$i]
				: $matches[4][$i]);

			/* Avoid XSS by eliminating any HTML from the url */
			$url = str_replace(array('<', '>'), array('&lt;', '&gt;'), $url);

			if (empty($url) || isset($seenBefore[$url])) {
			    continue;
			}
			$seenBefore[$url] = 1;

			/*
			 * Some sites (slashdot) have images that start with //
			 * and this confuses Gallery.  Prepend the base scheme.
			 */
			if (!strncmp($url, '//', 2)) {
			    $url = $baseUrlComponents['scheme'] . ':' . $url;
			}

			/* Convert urls without scheme/host to full urls */
			if (!preg_match('/^\w+:/', $url)) {
			    $tmp = $baseUrlComponents['scheme'] . '://'
				 . $baseUrlComponents['host'];
			    if (!empty($baseUrlComponents['port'])) {
				$tmp .= ':' . $baseUrlComponents['port'];
			    }
			    if ($url[0] == '/') {
				/* Absolute path; just prepend scheme/host:port */
				$url = $tmp . $url;
			    } else {
				/*
				 * Relative path; the current url might be one of:
				 *   http://example.com/path/to/file.html
				 *   http://example.com/path/to/
				 *
				 * If it's a directory, it should have a trailing slash at
				 * this point.  Either way, we want the base path to be:
				 *   http://example.com/path/to/
				 */
				if (isset($baseUrlComponents['path'])) {
				    $basePath = $baseUrlComponents['path'];
				} else {
				    $basePath = '/';
				}
				if (!preg_match('/\/$/', $basePath)) {
				    $basePath = dirname($basePath);
				    if ($basePath != '/') {
					$basePath .= '/';
				    }
				}
				$tmp .= $basePath;
				$url = $tmp . $url;
			    }
			}

			list ($ret, $typeName) = $this->_getTypeName($url, $matches[1][$i]);
			if ($ret) {
			    return array($ret->wrap(__FILE__, __LINE__), null, null);
			}

			if (!empty($typeName)) {
			    $form['webPageUrls'][] = array('url' => $url, 'itemType' => $typeName);
			}
		    }
		}
		if (empty($form['webPageUrls'])) {
		    /* Didn't find any urls to add */
		    $form['error']['webPage']['noUrlsFound'] = 1;
		}
	    }
	}

	if (!isset($form['set'])) {
	    $form['set'] = array('title' => 1, 'summary' => 0, 'description' => 0);
	}

	$ItemAddFromWeb = array(
	    'webPageUrlCount' => isset($form['webPageUrls']) ? count($form['webPageUrls']) : 0);

	$session =& $gallery->getSession();
	$recentUrls = $session->get('core.view.ItemAddFromWeb.recentUrls');
	if (!isset($recentUrls)) {
	    $recentUrls = array();
	}
	$ItemAddFromWeb['recentUrls'] = array_keys($recentUrls);

	$template->setVariable('ItemAddFromWeb', $ItemAddFromWeb);
	return array(null,
		     'modules/core/templates/ItemAddFromWeb.tpl',
		     'modules_core');
    }

    /**
     * Analyze url and available data to find mime type.  Check:
     *  1. Parse url for file extension and map this to a mime type.
     *  2. Check for Content-Disposition http header than includes
     *     a filename and map this extension to a mime type.
     *  3. Check for Content-Type http header and use that mime type.
     *
     * @param string url
     * @param array (optional) http response headers
     * @param boolean (optional) true to also figure out filename
     * @return array object GalleryStatus a status object
     *               string mime type
     *               string filename or null if not requested
     * @access private
     */
    function _getMimeType($url, $headers=null, $getFileName=false) {
	$urlComponents = parse_url($url);

	$fileName = null;
	if ($getFileName) {
	    if (!empty($headers['Content-Disposition'])
		    && preg_match('/filename="(.*?)"/', $headers['Content-Disposition'], $match)) {
		$fileName = $match[1];
	    } else if (!empty($urlComponents['path'])) {
		$fileName = basename($urlComponents['path']);
	    } else {
		$fileName = basename($url);
	    }
	}

	if (!empty($urlComponents['path'])) {
	    list ($ret, $mimeType) = GalleryCoreApi::getMimeType($urlComponents['path']);
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null, null);
	    }
	    if (!empty($mimeType) && $mimeType != 'application/unknown') {
		return array(null, $mimeType, $fileName);
	    }
	}

	if (!empty($urlComponents['query'])) {
	    /*
	     * Try again with end of the query string -- just in case.  Some sites, like
	     * Google's image search feature, put the file name at the end of the query string.
	     */
	    list ($ret, $mimeType) = GalleryCoreApi::getMimeType($urlComponents['query']);
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null, null);
	    }
	    if (!empty($mimeType) && $mimeType != 'application/unknown') {
		return array(null, $mimeType, $fileName);
	    }
	}

	if (!empty($headers['Content-Disposition'])
		&& preg_match('/filename="(.*?)"/', $headers['Content-Disposition'], $match)) {
	    list ($ret, $mimeType) = GalleryCoreApi::getMimeType($match[1]);
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null, null);
	    }
	    if (!empty($mimeType) && $mimeType != 'application/unknown') {
		return array(null, $mimeType, $fileName);
	    }
	}

	if (!empty($headers['Content-Type'])) {
	    return array(null, $headers['Content-Type'], $fileName);
	}

	return array(null, null, $fileName);
    }

    /**
     * Analyze url and available data to find item type.
     *
     * @param string url
     * @param string (optional) html attribute this url was parsed from
     * @param array (optional) http response headers
     * @return array object GalleryStatus a status object
     *               string type name
     * @access private
     */
    function _getTypeName($url, $htmlAttribute=null, $headers=null) {
	if (isset($htmlAttribute) && strtolower($htmlAttribute) == 'src') {
	    /* For <img> src attribute just assume "Photo" and don't examine mime type */
	    list ($ret, $module) = GalleryCoreApi::loadPlugin('module', 'core');
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null);
	    }
	    return array(null, $module->translate('Photo'));
	} else {
	    list ($ret, $mimeType) = $this->_getMimeType($url, $headers);
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null);
	    }
	    if (empty($mimeType)) {
		return array(null, null);
	    }

	    static $mimeTypeNameMap = array();
	    if (!isset($mimeTypeNameMap[$mimeType])) {
		list ($ret, $mimeTypeItem) = GalleryCoreApi::newItemByMimeType($mimeType);
		if ($ret) {
		    return array($ret->wrap(__FILE__, __LINE__), null);
		}
		$mimeTypeItem->setMimeType($mimeType);
		$mimeTypeNameMap[$mimeType] = $mimeTypeItem->itemTypeName();
	    }
	    return array(null, $mimeTypeNameMap[$mimeType][0]);
	}
    }

    /**
     * @see ItemAddPlugin::getTitle
     */
    function getTitle() {
	list ($ret, $module) = GalleryCoreApi::loadPlugin('module', 'core');
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null);
	}

	return array(null, $module->translate('From Web Page'));
    }
}
?>
