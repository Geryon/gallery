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
 * @package GalleryCore
 * @subpackage UserInterface
 * @author Jay Rossiter <cryptographite@users.sf.net>
 */

/**
 * This controller will handle the recovery of passwords that have
 * been lost or forgotten by the user.
 *
 * @package GalleryCore
 * @subpackage UserInterface
 *
 */

class UserRecoverPasswordAdminController extends GalleryController {

    /**
     * @see GalleryController::handleRequest
     */
    function handleRequest($form) {
	global $gallery;
	$platform =& $gallery->getPlatform();
	$session =& $gallery->getSession();

	$status = $error = $results = array();

	if (isset($form['action']['recover'])) {
	    $authFile = dirname(dirname(dirname(__FILE__))) . '/login.txt';
	    $authFromFile = $platform->is_readable($authFile) ?
		trim(implode('', $platform->file($authFile))) : '';

	    if ($session->get('core.UserRecoverPasswordAdminAuthKey') != $authFromFile) {
		$error[] = 'form[error][authString][incorrect]';
	    }

	    if (empty($error)) {
		if (empty($form['userName'])) {
		    $error[] = 'form[error][userName][missing]';
		}

		if (empty($form['password1']) || empty($form['password2'])) {
		    $error[] = 'form[error][password][missing]';
		} else if ($form['password1'] != $form['password2']) {
		    $error[] = 'form[error][password][mismatch]';
		}
	    }

	    if (empty($error)) {
		/*
		 * TODO - We need to be able to support a renamed administrator account
		 * or, possibly, the recovery of any administrator account.
		 */
		list ($ret, $user) = GalleryCoreApi::fetchUserByUsername($form['userName']);
		if ($ret && !($ret->getErrorCode() & ERROR_MISSING_OBJECT)) {
		    return array($ret->wrap(__FILE__, __LINE__, 'Unable to load admin user'), null);
		}

		if (isset($user)) {
		    list ($ret, $lockId) = GalleryCoreApi::acquireWriteLock($user->getId());
		    if ($ret) {
			return array($ret->wrap(__FILE__, __LINE__), null);
		    }

		    list ($ret, $user) = $user->refresh();
		    if ($ret) {
			return array($ret->wrap(__FILE__, __LINE__), null);
		    }

		    $user->changePassword($form['password1']);

		    $ret = $user->save();
		    if ($ret) {
			return array($ret->wrap(__FILE__, __LINE__), null);
		    }

		    $ret = GalleryCoreApi::releaseLocks($lockId);
		    if ($ret) {
			return array($ret->wrap(__FILE__, __LINE__), null);
		    }

		    $status['passwordRecovered'] = 1;

		    /*
		     * Return 0 is essential - it prevents core.UserLogin from sending the
		     * user back to the recovery page after logging in
		     */
		    $results['return'] = 0;
		    $subView = 'core.UserLogin';

		    $session->remove('core.UserRecoverPasswordAdminAuthKey');
		} else {
		    $error[] = 'form[error][userName][incorrect]';
		}
	    }
	} else if (isset($form['action']['cancel'])) {
	    $results['return'] = 1;
	}

	if (empty($subView)) {
	    $subView = 'core.UserRecoverPasswordAdmin';
	}

	if (empty($error) && !isset($form['action']['refresh'])) {
	    $results['redirect']['view'] = 'core.UserAdmin';
	    $results['redirect']['subView'] = $subView;

	} else  {
	    $results['delegate']['view'] = 'core.UserAdmin';
	    $results['delegate']['subView'] = 'core.UserRecoverPasswordAdmin';
	}

	$results['status'] = $status;
	$results['error'] = $error;

	return array(null, $results);
    }
}

/**
 * This view prompts for login information
 *
 * @package GalleryCore
 * @subpackage UserInterface
 *
 */
class UserRecoverPasswordAdminView extends GalleryView {

    /**
     * @see GalleryView::loadTemplate
     */
    function loadTemplate(&$template, &$form) {
	global $gallery;
	$platform =& $gallery->getPlatform();
	$session =& $gallery->getSession();

	$status = $error = array();
	$authFile = dirname(__FILE__) . '/../../login.txt';

	if ($form['formName'] != 'UserRecoverPasswordAdmin') {
	    $form['formName'] = 'UserRecoverPasswordAdmin';

	    /* Don't display the Authorization Incorrect error on first page load */
	    $status['firstLoad'] = 1;

	    /* Generate the auth string on the first visit to this view */
	    $this->_generateAuthString();
	}

	$authString = $session->get('core.UserRecoverPasswordAdminAuthKey');

	if (!$platform->file_exists($authFile)) {
	    $error['authFile']['missing'] = 1;
	} else if (!$platform->is_readable($authFile)) {
	    $error['authFile']['unreadable'] = 1;
	} else {
	    $authStringFromFile = trim(join('', $platform->file($authFile)));
	    if ($authStringFromFile == $authString) {
		$status['authString']['correct'] = 1;
	    } else {
		$error['authString']['incorrect'] = 1;
	    }
	}

	$status['userName'] = GalleryUtilities::getRequestVariables('userName');

	$UserRecoverPasswordAdmin['authString'] = $authString;
	$UserRecoverPasswordAdmin['error'] = $error;
	$UserRecoverPasswordAdmin['status'] = $status;
	$template->setVariable('UserRecoverPasswordAdmin', $UserRecoverPasswordAdmin);
	$template->setVariable('controller', 'core.UserRecoverPasswordAdmin');
	return array(null,
		     array('body' => 'modules/core/templates/UserRecoverPasswordAdmin.tpl'));
    }

    /**
     * Generate the authorization string used for login.txt
     * @access private
     */
    function _generateAuthString() {
	global $gallery;
	$session =& $gallery->getSession();

	for ($len=64, $rand='';
	     strlen($rand) < $len;
	     $rand .= chr(!mt_rand(0, 2) ? mt_rand(48, 57) :
			  (!mt_rand(0, 1) ? mt_rand(65, 90) :
			   mt_rand(97, 122))));
	$session->put('core.UserRecoverPasswordAdminAuthKey', md5($rand));
    }
}
?>
