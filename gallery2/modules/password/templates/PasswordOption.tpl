{*
 * $Revision: 12783 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock">
  <h3> {g->text text="Password Protection"} </h3>
  <p class="giDescription">
    {g->text text="Assign a password that is required for guests or other users without view permission. Ensure guests have permission to view resizes and/or original items before assigning a password."}
  </p>

  {if ($form.PasswordOption.hasPassword)}
    <p class="giDescription">
      {g->text text="This item is already password protected."}
    </p>
    <input type="checkbox" id="RemovePassword_cb"
    	   name="{g->formVar var="form[PasswordOption][remove]"}"
	   onclick="passwordFieldToggle(this.checked)"/>
    <label for="RemovePassword_cb">
      {g->text text="Remove Password"}
    </label>
    <script type="text/javascript">{literal}
      // <![CDATA[
      function passwordFieldToggle(off) {
	document.getElementById('Password1_tb').disabled = off;
	document.getElementById('Password2_tb').disabled = off;
      }
      // ]]>
    </script>{/literal}
  {/if}

  <table class="gbDataTable"><tr>
    <td><label for="Password1_tb">
      {if ($form.PasswordOption.hasPassword)}
	{g->text text="New Password:"}
      {else}
	{g->text text="Password:"}
      {/if}
    </label></td>
    <td>
      <input type="password" size="20" id="Password1_tb"
	     name="{g->formVar var="form[PasswordOption][password1]"}"/>
    </td>
  </tr><tr>
    <td><label for="Password2_tb">
      {g->text text="Confirm Password:"}
    </label></td>
    <td>
      <input type="password" size="20" id="Password2_tb"
	     name="{g->formVar var="form[PasswordOption][password2]"}"/>
    </td>
  </tr></table>

  {if isset($form.error.PasswordOption.mismatch)}
   <div class="giError">
     {g->text text="The passwords you entered did not match"}
   </div>
  {/if}
</div>
