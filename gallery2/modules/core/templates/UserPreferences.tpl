{*
 * $Revision: 9810 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div class="gbBlock gcBackground1">
  <h2> {g->text text="Account Settings"} </h2>
</div>

{if isset($status.saved)}
<div class="gbBlock"><h2 class="giSuccess">
  {g->text text="Account settings saved successfully"}
</h2></div>
{/if}

<div class="gbBlock">
  <div>
    <h4> {g->text text="Username"} </h4>
    <p class="giDescription">
      {$user.userName}
    </p>
  </div>

  <div>
    <h4> {g->text text="Full Name"} </h4>
    <input type="text" name="{g->formVar var="form[fullName]"}" value="{$form.fullName}"/>
  </div>

  <div>
    <h4>
      {g->text text="E-mail Address"}
      <span class="giSubtitle">
      {if !isset($UserAdmin.isSiteAdmin)}
	{g->text text="(required)"}
      {else}
	{g->text text="(suggested)"}
      {/if}
      </span>
    </h4>

    <input type="text" name="{g->formVar var="form[email]"}" value="{$form.email}"/>

    {if isset($form.error.email.missing)}
    <div class="giError">
      {g->text text="You must enter an email address"}
    </div>
    {/if}
    {if isset($form.error.email.invalid)}
    <div class="giError">
      {g->text text="Invalid email address"}
    </div>
    {/if}
  </div>

  {if $UserPreferences.translationsSupported}
  <div>
    <h4> {g->text text="Language"} </h4>

    <select name="{g->formVar var="form[language]"}">
      {html_options options=$UserPreferences.languageList selected=$form.language}
    </select>
  </div>
  {/if}
</div>

<div class="gbBlock gcBackground1">
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][save]"}" value="{g->text text="Save"}"/>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][undo]"}" value="{g->text text="Reset"}"/>
  <input type="submit" class="inputTypeSubmit"
   name="{g->formVar var="form[action][cancel]"}" value="{g->text text="Cancel"}"/>
</div>
