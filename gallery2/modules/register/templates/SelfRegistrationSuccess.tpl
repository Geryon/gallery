{*
 * $Revision: 12868 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Registration successful"} </h2>
</div>

<div class="gbBlock">
{if $SelfRegistrationSuccess.pending}
  <h2 class="giTitle">
    {g->text text="Your registration was successful."}
  </h2>
  <p class="giDescription">
    {if $SelfRegistrationSuccess.sentConfirmationEmail}
      {g->text text="You will shortly receive an email containing a link. You have to click this link to confirm and activate your account.  This procedure is necessary to prevent account abuse."}
    {else}
      {g->text text="Your registration will be processed and your account activated soon."}
    {/if}
  </p>
{else}
  <h2 class="giTitle">
      {g->text text="Your registration was successful and your account has been activated."}
  </h2>
  <p class="giDescription">
      {capture name=login}
      <a href="{g->url arg1="view=core.UserAdmin" arg2="subView=core.UserLogin" forceFullUrl=true}">
	{g->text text="login"}
      </a>
      {/capture}

      {g->text text="You can now %s to your account with your username and password."
	       arg1=$smarty.capture.login}
  </p>
{/if}
</div>
