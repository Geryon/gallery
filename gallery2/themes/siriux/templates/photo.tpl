{*
 * $Revision: 12894 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}
{if !empty($theme.imageViews)}
{assign var="image" value=$theme.imageViews[$theme.imageViewsIndex]}
{/if}

<h2>{$theme.item.title|markup}</h2>

{if !empty($theme.imageViews)}
  {capture name="fallback"}
    <a href="{g->url arg1="view=core.DownloadItem" arg2="itemId=`$theme.item.id`"
		     forceFullUrl=true forceSessionId=true}">
      {g->text text="Download %s" arg1=$theme.sourceImage.itemTypeName.1}
    </a>
  {/capture}

  {if ($image.viewInline)}
    <div class="gallery-photo">
      {g->image item=$theme.item image=$image fallback=$smarty.capture.fallback usemap=#prevnext}
    </div>
  {else}
    {$smarty.capture.fallback}
  {/if}
{else}
  {g->text text="There is nothing to view for this item."}
{/if}

{* Navigation image map *}
<map name="prevnext">
{if isset($theme.navigator.back)}
  <area shape="rect" coords="0,0,{math equation="round(x/2-1)" x=$image.width},{$image.height}"
   href="{g->url params=$theme.navigator.back.urlParams}"/>
{/if}
{if isset($theme.navigator.next)}
  <area shape="rect" coords="{math equation="round(x/2)"
				   x=$image.width},0,{$image.width},{$image.height}"
   href="{g->url params=$theme.navigator.next.urlParams}"/>
{/if}
</map>


<br style="clear: both;" />


{* Navigator *}
{if !empty($theme.navigator)}
  {g->callback type="core.LoadPeers" item=$theme.item windowSize=1}
  {g->block type="core.Navigator" navigator=$theme.navigator prefix="&laquo; " suffix=" &raquo;"
      currentItem=$block.core.LoadPeers.thisPeerIndex totalItems=$block.core.LoadPeers.peerCount}
{/if}

<hr/>

{* Description *}
{if !empty($theme.item.description) && ($theme.item.description != $theme.item.title)}
  <p>{$theme.item.description|markup}</p>
{/if}

{* Download *}
{if !empty($theme.sourceImage) &&
    (count($theme.imageViews) > 1 || $theme.sourceImage.mimeType != $theme.item.mimeType)}
 <p>
  <a href="{g->url arg1="view=core.DownloadItem" arg2="itemId=`$theme.item.id`"}">
    {if $theme.sourceImage.mimeType != $theme.item.mimeType}
      {g->text text="Download %s in original format" arg1=$theme.sourceImage.itemTypeName.1}
    {else}
      {g->text text="Download %s" arg1=$theme.sourceImage.itemTypeName.1}
    {/if}
    {if !empty($theme.sourceImage.width)}
      {g->text text="(%dx%d)" arg1=$theme.sourceImage.width arg2=$theme.sourceImage.height}
    {/if}
  </a>
</p>
{/if}

{* Show any other album blocks (comments, exif etc) *}
{foreach from=$theme.params.photoBlocks item=block}
  {g->block type=$block.0 params=$block.1}
{/foreach}

{* Guest preview mode *}
{g->block type="core.GuestPreview" class="gbBlock"}

{* Our emergency edit link, if the user all blocks containing edit links *}
{g->block type="core.EmergencyEditItemLink" class="gbBlock" checkPhotoBlocks=true}

