{*
 * $Revision: 10972 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
<table border="0" cellspacing="0" cellpadding="0">
<tr valign="top"><td id="gsSidebarCol">
  <div id="gsSidebar" class="gcBorder1">
    {* Show the sidebar blocks chosen for this theme *}
    {foreach from=$theme.params.sidebarBlocks item=block}
      {g->block type=$block.0 params=$block.1 class="gbBlock"}
    {/foreach}
  </div>
</td><td id="gsContent">

<noscript><p class="giError">
  {g->text text="Warning: This site requires javascript."}
</p></noscript>

<div style="display: none">
{foreach from=$theme.children key=i item=it}
  {if isset($it.image)}
    {if isset($it.renderItem)}
      <a id="img_{$it.imageIndex}" href="{g->url arg1="view=core.ShowItem"
       arg2="itemId=`$it.id`" arg3="renderId=`$it.image.id`"}"></a>
    {else}
      <a id="img_{$it.imageIndex}" href="{g->url arg1="view=core.DownloadItem"
       arg2="itemId=`$it.image.id`" arg3="serialNumber=`$it.image.serialNumber`"}"></a>
    {/if}
    <span id="title_{$it.imageIndex}">{$it.title|markup}</span>
  {/if}
{/foreach}
</div>

<div id="image" class="gcBackground1" onclick="ui_vis('image',0)">
  <div class="gbBlock gcBackground2">
    <p id="title" class="giTitle"></p>
  </div>
  <div id="image_view"></div>
</div>

<div class="gbBlock gcBackground2">
  <p class="giTitle">{$theme.item.title|markup}</p>
</div>

<div class="gbBlock">
{if isset($theme.param.bgSerialNumber)}
  <table id="tile" style="background-image:url({g->url arg1="view=core.DownloadItem" arg2="itemId=`$theme.param.backgroundId`" arg3="serialNumber=`$theme.param.bgSerialNumber`"})" cellspacing="0">
  {section name=row loop=$theme.map}
   <tr>
   {section name=col loop=$theme.map[row]}
    <td>
    {assign var="id" value=$theme.map[row][col]}
    {if $id>0}
      {assign var="it" value=$theme.itemMap[$id]}
      <a href="#" onclick="image_show({$it.imageIndex});return false">
	{g->image item=$it image=$it.thumbnail class=thumb}
      </a>
    {else}
      <div class="emptyTile"></div>
    {/if}
    </td>
   {/section}
  </tr>
  {/section}
  </table>
  {g->block type="core.GuestPreview" class="gbBlock"}
{else}
  {capture name="link"}<a href="{g->url arg1="view=core.ItemAdmin" arg2="subView=core.ItemEdit"
   arg3="editPlugin=ItemEditTheme" arg4="itemId=`$theme.item.id`" arg5="return=true"}">{/capture}
  {g->text text="The theme has not been %sconfigured%s." arg1=$smarty.capture.link arg2="</a>"}
{/if}
</div>
</td></tr></table>

<script type="text/javascript">app_init();</script>
