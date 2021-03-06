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
 * @version $Revision: 12836 $ $Date: 2006-01-30 17:52:15 -0500 (Mon, 30 Jan 2006) $
 * @package Captcha
 * @author Stefan Ioachim <stefanioachim@gmail.com>
 * @author Bharat Mediratta <bharat@menalto.com>
 * @author Alan Harder <alan.harder@sun.com>
 */

GalleryCoreApi::requireOnce('modules/core/classes/GalleryValidationPlugin.class');

/**
 * Implement ValidationPlugin to present the HTML for the captcha image
 * and an input box for the user to type in the correct value.
 *
 * @package Captcha
 * @subpackage Classes
 */
class CaptchaValidationPlugin extends GalleryValidationPlugin {

    /**
     * @see GalleryValidationPlugin::performValidation
     */
    function performValidation(&$form, $options=array()) {
	global $gallery;
	$session =& $gallery->getSession();

	list ($ret, $useCaptcha, $failedAttempts) = $this->_shouldValidate($options);
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null, null);
	}

	$error = array();
	$success = true;
	if ($useCaptcha) {
	    $code = $session->get('captcha.key');

	    if (empty($form['CaptchaValidationPlugin']['word'])) {
		$error[] = 'form[error][CaptchaValidationPlugin][missing]';
		$success = false;
	    } else if ($form['CaptchaValidationPlugin']['word'] != $code) {
		$error[] = 'form[error][CaptchaValidationPlugin][invalid]';
		$success = false;
	    }
	}

	if (isset($options['pass']) && !$options['pass']) {
	    /* Increment failure count either in db or session */
	    if (!empty($options['key'])) {
		$ret = GalleryCoreApi::setPluginParameter(
			'module', 'captcha', 'failCount-' . $options['key'], $failedAttempts + 1);
		if ($ret) {
		    return array($ret->wrap(__FILE__, __LINE__), null, null);
		}
	    } else {
		$session->put('captcha.failedAttempts', $failedAttempts + 1);
	    }
	} else if (isset($options['pass']) && $options['pass']
		&& $success && $failedAttempts > 0 && !empty($options['key'])) {
	    $ret = GalleryCoreApi::removePluginParameter(
		    'module', 'captcha', 'failCount-' . $options['key']);
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null, null);
	    }
	}

	return array(null, $error, $success);
    }

    /**
     * @see GalleryValidationPlugin::loadTemplate
     */
    function loadTemplate(&$form, $options=array()) {
	list ($ret, $useCaptcha) = $this->_shouldValidate($options);
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null, null);
	}

	if ($useCaptcha) {
	    /* Generate a new code */
	    global $gallery;
	    list ($usec, $sec) = explode(' ', microtime());
	    srand((float)$sec + ((float)$usec * 100000));
	    $random_num = rand();
	    $datekey = date('H i s');
	    $rcode = hexdec(md5(GalleryUtilities::getServerVar('HTTP_USER_AGENT')
				. $random_num . $datekey));
	    $code = substr($rcode, 2, 6);
	    $session =& $gallery->getSession();
	    $session->put('captcha.key', $code);

	    return array(null,
			 'modules/captcha/templates/CaptchaValidationPlugin.tpl',
			 'modules_captcha');
	}

	return array(null, null, null);
    }

    /**
     * Determine if captcha validation should be enforced
     *
     * @param array options
     * @return array object GalleryStatus a status code
     *               boolean true to validate
     *               int current failure count, if applicable
     * @access private
     */
    function _shouldValidate($options) {
	$useCaptcha = false;
	$failedAttempts = 0;
	$securityLevel = empty($options['level']) ? 'HIGH' : $options['level'];

	switch ($securityLevel) {
	case 'HIGH':
	    /* Always require captcha to be enabled */
	    $useCaptcha = true;
	    break;

	case 'MEDIUM':
	case 'LOW':
	    /*
	     * Use the captcha when the number of failed attempts exceeds the module's
	     * failedAttemptThreshold parameter.  Track failed attempts in db via a given
	     * unique key (MEDIUM level) or in the session (LOW level).
	     */
	    list ($ret, $failedAttemptThreshold) = GalleryCoreApi::getPluginParameter(
		    'module', 'captcha', 'failedAttemptThreshold');
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null, null);
	    }

	    if ($securityLevel == 'LOW') {
		global $gallery;
		$session =& $gallery->getSession();
		$failedAttempts = $session->get('captcha.failedAttempts');
	    } else if (!empty($options['key'])) {
		list ($ret, $failedAttempts) = GalleryCoreApi::getPluginParameter(
			'module', 'captcha', 'failCount-' . $options['key']);
		if ($ret) {
		    return array($ret->wrap(__FILE__, __LINE__), null, null);
		}
	    }

	    if ($failedAttempts > $failedAttemptThreshold) {
		$useCaptcha = true;
	    }
	    break;

	default:
	    /* Bad Parameter */
	    return array(GalleryCoreApi::error(ERROR_BAD_PARAMETER, __FILE__, __LINE__),
			 null, null);
	}

	return array(null, $useCaptcha, (int)$failedAttempts);
    }
}
?>
