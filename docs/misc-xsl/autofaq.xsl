<?xml version="1.0" encoding="utf-8"?>
<!-- $Id: autofaq.xsl 4460 2003-10-14 05:59:21Z bharat $ -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                version="1.0">

  <xsl:output method="text"/>

  <xsl:template match="/book">
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="chapter" priority="0.9">
    <xsl:for-each select="sect1/sect2/qandaset/qandadiv/qandaentry">
      <xsl:value-of select="../../../@id"/> | <xsl:value-of select="@id"/> | <xsl:value-of select="normalize-space(question/para)"/> | <xsl:value-of select="normalize-space(answer/para)"/> |
    </xsl:for-each>
    
    <xsl:for-each select="sect1/qandaset/qandaentry">
      <xsl:value-of select="../../@id"/> | <xsl:value-of select="@id"/> | <xsl:value-of select="normalize-space(question/para)"/> | <xsl:value-of select="normalize-space(answer/para)"/> |
    </xsl:for-each>
  </xsl:template>
  <xsl:template match="*" priority="0.1"/>
</xsl:stylesheet>
