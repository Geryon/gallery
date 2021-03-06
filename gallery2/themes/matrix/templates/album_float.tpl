{*
 * $Revision: 12859 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}

<table width="100%" cellspacing="0" cellpadding="0">
  <tr valign="top">
    {if !empty($theme.params.sidebarBlocks)}
    <td id="gsSidebarCol">
      {g->theme include="sidebar.tpl"}
    </td>
    {/if}
    <td>
      <div id="gsContent">
        <div class="gbBlock gcBackground1">
          <table style="width: 100%">
            <tr>
              <td>
                {if !empty($theme.item.title)}
                <h2> {$theme.item.title|markup} </h2>
                {/if}
                {if !empty($theme.item.description)}
                <p class="giDescription">
                  {$theme.item.description|markup}
                </p>
                {/if}
              </td>
              <td style="width: 30%">
                {g->block type="core.ItemInfo"
                          item=$theme.item
                          showDate=true
                          showSize=true
                          showOwner=true
                          class="giInfo"}
              </td>
            </tr>
          </table>
        </div>

        {if !empty($theme.navigator)}
        <div class="gbBlock gcBackground2 gbNavigator">
          {g->block type="core.Navigator" navigator=$theme.navigator reverseOrder=true}
        </div>
        {/if}


        {if !count($theme.children)}
        <div class="gbBlock giDescription gbEmptyAlbum">
          <h3 class="emptyAlbum">
	    {g->text text="This album is empty."}
	    {if isset($theme.permissions.core_addDataItem)}
	    <br/>
              <a href="{g->url arg1="view=core.ItemAdmin" arg2="subView=core.ItemAdd" arg3="itemId=`$theme.item.id`"}"> {g->text text="Add a photo!"} </a>
	    {/if}
          </h3>
        </div>
        {else}
		
        <div class="gbBlock">
            <div id="gsThumbMatrix">	
                {foreach from=$theme.children item=child}
                <div class="{if $child.canContainChildren}giAlbumCell gcBackground1{else}giItemCell{/if}" style="width: {$theme.params.columnWidth}px; height: {$theme.params.rowHeight}px;">

                {if ($child.canContainChildren || $child.entityType == 'GalleryLinkItem')}
                    {assign var=frameType value="albumFrame"}
                {else}
                    {assign var=frameType value="itemFrame"}
                {/if}
                
                {if isset($theme.params.$frameType) && isset($child.thumbnail)}
                    {g->container type="imageframe.ImageFrame" frame=$theme.params.$frameType width=$child.thumbnail.width height=$child.thumbnail.height}
                    <a href="{g->url arg1="view=core.ShowItem" arg2="itemId=`$child.id`"}">{g->image id="%ID%" item=$child image=$child.thumbnail class="%CLASS% giThumbnail"}</a>
                    {/g->container}
                {elseif isset($child.thumbnail)}
                    <a href="{g->url arg1="view=core.ShowItem" arg2="itemId=`$child.id`"}">{g->image item=$child image=$child.thumbnail class="giThumbnail"}</a>
                {else}
                    <a href="{g->url arg1="view=core.ShowItem" arg2="itemId=`$child.id`"}" class="giMissingThumbnail">{g->text text="no thumbnail"}</a>
                {/if}

                {g->block type="core.ItemLinks" item=$child links=$child.itemLinks}

                {if !empty($child.title)}
                <p class="giTitle">
                    {if $child.canContainChildren}
                    {g->text text="Album: %s" arg1=$child.title|markup}
                    {else}
                    {$child.title|markup}
                    {/if}
                </p>
                {/if}

                {if !empty($child.summary)}
                <p class="giDescription">{$child.summary|markup|entitytruncate:256}</p>
                {/if}

                {if ($child.canContainChildren && $theme.params.showAlbumOwner) || (!$child.canContainChildren && $theme.params.showImageOwner)}
                    {assign var="showOwner" value=true}
                {else}
                    {assign var="showOwner" value=false}
                {/if}
                
                {g->block type="core.ItemInfo"
                                item=$child
                                showDate=true
                                showOwner=$showOwner
                                showSize=true
                                showViewCount=true
                                showSummaries=true
                                class="giInfo"}
                </div>
                {/foreach}
            </div>
        </div>
		
        {/if}

        {if !empty($theme.navigator)}
        <div class="gbBlock gcBackground2 gbNavigator">
          {g->block type="core.Navigator" navigator=$theme.navigator reverseOrder=true}
        </div>
        {/if}

        {if !empty($theme.jumpRange)}
        <div id="gsPages" class="gbBlock gcBackground1">
          {g->block type="core.Pager"}
        </div>
        {/if}

        {g->block type="core.GuestPreview" class="gbBlock"}

        {* Our emergency edit link, if the user all blocks containing edit links *}
		{g->block type="core.EmergencyEditItemLink" class="gbBlock"
                  checkSidebarBlocks=true
                  checkAlbumBlocks=true}

        {* Show any other album blocks (comments, etc) *}
        {foreach from=$theme.params.albumBlocks item=block}
          {g->block type=$block.0 params=$block.1}
        {/foreach}
      </div>
    </td>
  </tr>
</table>
