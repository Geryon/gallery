<?xml version="1.0" encoding="iso-8859-1"?>
<!-- $Id: common.xsl 3688 2003-08-19 02:05:15Z alindeman $ -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

<xsl:template match="*" mode="galleryweb.title.nochapter">
 <xsl:call-template name="substitute-markup">
  <xsl:with-param name="allow-anchors" select="0"/>
  <xsl:with-param name="template" select="'%t'"/>
 </xsl:call-template>
</xsl:template>

</xsl:stylesheet>
