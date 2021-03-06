{*
 * $Revision: 11130 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<div id="gsSidebar" class="gcBorder1">

  {* System Links *}
  <div class="gbBlock">
    {g->block type="core.SystemLinks"
	      order="core.SiteAdmin core.YourAccount core.Login core.Logout"
	      othersAt=4}
  </div>

  {* Breadcrumb *}
  {if !empty($theme.parents)}
  <div class="gbBlock">
    <h3> {g->text text="Navigation"} </h3>
    <ul>
    {foreach from=$theme.parents item=parent}
      <li>
	&raquo;
	<a href="{g->url arg1="view=core.ShowItem" arg2="itemId=`$parent.id`"}">
	  {$parent.title|default:$parent.pathComponent|markup:strip}
	</a>
      </li>
    {/foreach}
    </ul>
  </div>
  {/if}

  {* Album links *}
  {if !empty($theme.itemLinks)}
  <div class="gbBlock">
    <h3> {g->text text="Album"} </h3>
    <select onchange="{literal}if (this.value) { var u=this.value; this.options[0].selected=1; location.href=u; }{/literal}" style="margin-left: 1em">
      <option label="{g->text text="&laquo; actions &raquo;"}" value="">
	{g->text text="&laquo; actions &raquo;"}
      </option>
      {foreach from=$theme.itemLinks item=link}
	<option label="{$link.text}" value="{g->url params=$link.params}">{$link.text}</option>
      {/foreach}
    </select>
  </div>
  {/if}

  {* Item links *}
  <div id="photoActions" class="gbBlock" style="display: none">
    <h3> {g->text text="Photo"} </h3>
    <select id="linkList" onchange="{literal}if (this.value) { var u=this.value; this.options[0].selected=1; location.href=u; }{/literal}" style="margin-left: 1em">
      <option label="{g->text text="&laquo; actions &raquo;"}" value="">
	{g->text text="&laquo; actions &raquo;"}
      </option>
    </select>
  </div>

  {* Slideshow options *}
  <div class="gbBlock">
    <h3> {g->text text="Slideshow&nbsp;Options"} </h3>
    <ul><li>
      {g->text text="Delay"}:&nbsp;
      <select id="slide_delay" onchange="slide_setdelay(this.value)">
	<option value="3">{g->text text="3 seconds"}</option>
	<option selected="selected" value="5">{g->text text="5 seconds"}</option>
	<option value="7">{g->text text="7 seconds"}</option>
	<option value="10">{g->text text="10 seconds"}</option>
	<option value="15">{g->text text="15 seconds"}</option>
	<option value="20">{g->text text="20 seconds"}</option>
      </select>
      <br/>
      {g->text text="Direction"}:&nbsp;
      <select id="slide_order" onchange="slide_setorder(this.value)">
	<option selected="selected" value="1">{g->text text="forward"}</option>
	<option value="-1">{g->text text="reverse"}</option>
	<option value="0">{g->text text="random"}</option>
      </select>
    </li></ul>
  </div>

  {* Slider options *}
  <div class="gbBlock">
    <h3> {g->text text="Image&nbsp;Bar"} </h3>
    <ul><li>
      <a href="" onclick="options_onoff();thumbs_horizvert();return false">
	{g->text text="Horizontal/Vertical"}
      </a>
    </li></ul>
  </div>

  {* Show the sidebar blocks chosen for this theme *}
  {foreach from=$theme.params.sidebarBlocks item=block}
    {g->block type=$block.0 params=$block.1 class="gbBlock"}
  {/foreach}
</div>
