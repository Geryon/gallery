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
 * @version $Revision: 12868 $ $Date: 2006-02-05 05:44:36 -0500 (Sun, 05 Feb 2006) $
 * @package SelfRegistration
 * @subpackage UserInterface
 * @author Sebastian Eichner <mailsp@sebastian-eichner.de>
 */

/**
 * This controller will handle the registration of a user
 *
 * @package SelfRegistration
 * @subpackage UserInterface
 */
class UserSelfRegistrationController extends GalleryController {

    /**
     * @see GalleryController::handleRequest
     */
    function handleRequest($form) {
	GalleryCoreApi::requireOnce(
	    'modules/register/classes/GalleryPendingUserHelper.class');
	global $gallery;

	$results = $status = $error = array();
	if (isset($form['action']['cancel'])) {

	    /* Go back to the AdminUsers view */
	    $redirect['view'] = 'core.UserAdmin';

	} else if (isset($form['action']['create'])) {

	    /* Validate inputs */
	    foreach (array('userName', 'email', 'password1', 'password2', 'fullName') as $key) {
		if (empty($form[$key])) {
		    $error[] = 'form[error][' . $key . '][missing]';
		}
	    }
	    if (!empty($form['email']) && !GalleryUtilities::isValidEmailString($form['email'])) {
		$error[] = 'form[error][email][invalid]';
	    }
	    if (!empty($form['password1']) && !empty($form['password2']) &&
			$form['password1'] != $form['password2']) {
		$error[] = 'form[error][password2][mismatch]';
	    }

	    if (isset($this->_pluginInstances)) {
		$pluginInstances = $this->_pluginInstances;
	    } else {
		/* Get all the login plugins */
		list ($ret, $pluginInstances) =
		    GalleryCoreApi::getAllFactoryImplementationIds('GalleryValidationPlugin');
		if ($ret) {
		    return array($ret->wrap(__FILE__, __LINE__), null);
		}

		foreach (array_keys($pluginInstances) as $pluginId) {
		    list ($ret, $pluginInstances[$pluginId]) =
			GalleryCoreApi::newFactoryInstanceById('GalleryValidationPlugin',
							       $pluginId);
		    if ($ret) {
			return array($ret->wrap(__FILE__, __LINE__), null);
		    }
		}
	    }

	    /* Let each plugin do its verification */
	    foreach ($pluginInstances as $plugin) {
		list ($ret, $pluginErrors, $continue) = $plugin->performValidation($form);
		if ($ret) {
		    return array($ret->wrap(__FILE__, __LINE__), null);
		}

		$error = array_merge($error, $pluginErrors);
		if (!$continue) {
		    break;
		}
	    }

	    /* If all the right fields are in place then go ahead and create the user */
	    if (empty($error)) {
		list ($ret, $user) =
		    GalleryCoreApi::newFactoryInstance('GalleryEntity', 'GalleryPendingUser');
		if ($ret) {
		    return array($ret->wrap(__FILE__, __LINE__), null);
		}

		if (!isset($user)) {
		    return array(GalleryCoreApi::error(ERROR_MISSING_OBJECT, __FILE__, __LINE__),
				 null);
		}

		$ret = $user->create($form['userName']);
		if ($ret) {
		    if (!($ret->getErrorCode() & ERROR_COLLISION)) {
			return array($ret->wrap(__FILE__, __LINE__), null);
		    }

		    /* Set our error status and fall back to the view */
		    $error[] = 'form[error][userName][exists]';
		} else {
		    $user->setEmail($form['email']);
		    $user->setFullName($form['fullName']);
		    $user->changePassword(GalleryUtilities::htmlEntityDecode($form['password1']));

		    $ret = $user->save();
		    if ($ret) {
			return array($ret->wrap(__FILE__, __LINE__), null);
		    }

		    /*
		     * send a confirmation email if activated in module settings
		     * otherwise create the "real" user right now
		     */
		    list($ret, $params) =
			GalleryCoreApi::fetchAllPluginParameters('module', 'register');
		    if ($ret) {
			return array($ret->wrap(__FILE__, __LINE__), null);
		    }

		    if ($params['confirmation'] == 'email') {
			/* send confirmation email */
			$ret = GalleryPendingUserHelper::sendConfirmationEmail($user);
			if ($ret) {
			    return array($ret->wrap(__FILE__, __LINE__), null);
			}
			$redirect['pending'] = true;
			$redirect['sentConfirmationEmail'] = true;
		    } else if ($params['confirmation'] == 'auto') {
			/* turn the pending user into a real user right now */
			$ret = GalleryPendingUserHelper::createGalleryUser($user);
			if ($ret) {
			    return array($ret->wrap(__FILE__, __LINE__), null);
			}
		    } else { /* confirmation == 'admin' */
			$redirect['pending'] = true;
		    }

		    if ($params['emailadmins']) {
			$ret = GalleryPendingUserHelper::sendAdminEmail($user);
			if ($ret) {
			    return array($ret->wrap(__FILE__, __LINE__), null);
			}
		    }

		    /* Request a redirect to the confirmation screen */
		    $redirect['view'] = 'core.UserAdmin';
		    $redirect['subView'] = 'register.SelfRegistrationSuccess';
		}
	    }
	}

	if (!empty($redirect)) {
	    $results['redirect'] = $redirect;
	} else {
	    $results['delegate']['view'] = 'core.UserAdmin';
	    $results['delegate']['subView'] = 'register.UserSelfRegistration';
	}
	$results['status'] = $status;
	$results['error'] = $error;

	return array(null, $results);
    }

    /**
     * Tests can use this method to hardwire a specific set of plugin instances to use.
     * This avoids situations where some of the option instances will do unpredictable
     * things and derail the tests.
     *
     * @param array of GalleryValidationPlugin
     */
    function setPluginInstances($pluginInstances) {
	$this->_pluginInstances = $pluginInstances;
    }

}

/**
 * This view will prompt for data to create a new user
 *
 * @package SelfRegistration
 * @subpackage UserInterface
 */
class UserSelfRegistrationView extends GalleryView {

    /**
     * @see GalleryView::loadTemplate
     */
    function loadTemplate(&$template, &$form) {
	global $gallery;

	if ($form['formName'] != 'UserSelfRegistration') {
	    /* First time around, set our defaults here. */
	    $form['userName'] = '';
	    $form['email'] = '';
	    $form['fullName'] = '';
	    $form['formName'] = 'UserSelfRegistration';
	}

	$UserSelfRegistration = array();

	/* Get all the login plugins */
	list ($ret, $allPluginIds) =
	    GalleryCoreApi::getAllFactoryImplementationIds('GalleryValidationPlugin');
	if ($ret) {
	    return array($ret->wrap(__FILE__, __LINE__), null);
	}

	/* Let each plugin load its template data */
	$UserSelfRegistration['plugins'] = array();
	foreach (array_keys($allPluginIds) as $pluginId) {
	    list ($ret, $plugin) =
		GalleryCoreApi::newFactoryInstanceById('GalleryValidationPlugin', $pluginId);
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null);
	    }

	    list ($ret, $data['file'], $data['l10Domain']) = $plugin->loadTemplate($form);
	    if ($ret) {
		return array($ret->wrap(__FILE__, __LINE__), null);
	    }

	    if (isset($data['file'])) {
		$UserSelfRegistration['plugins'][] = $data;
	    }
	}

	$template->setVariable('UserSelfRegistration', $UserSelfRegistration);
	$template->setVariable('controller', 'register.UserSelfRegistration');

	return array(null,
		     array('body' => 'modules/register/templates/UserSelfRegistration.tpl'));
    }
}
?>
