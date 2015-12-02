<?xml version="1.0" encoding="gbk"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template name="output-tokens">
  <xsl:param name="content" />
  <xsl:param name="seperator" />
  <xsl:param name="tagname" />
  <xsl:variable name="newlist" select="concat(normalize-space($content), $seperator)" />
  <xsl:variable name="first" select="substring-before($newlist, $seperator)" />
  <xsl:variable name="remaining" select="substring-after($newlist, $seperator)" />
  <xsl:if test="$first != ''">
  <xsl:element name="{$tagname}"> 
  <text><xsl:value-of select="$first"/></text>
  </xsl:element>
  </xsl:if>
  <xsl:if test="substring-before($remaining, $seperator) != ''">
   <xsl:call-template name="output-tokens">
    <xsl:with-param name="content" select="$remaining" />
    <xsl:with-param name="seperator" select="$seperator" />
    <xsl:with-param name="tagname" select="$tagname" />
   </xsl:call-template>
  </xsl:if>
</xsl:template>

<xsl:template name="output-tokens-index">
  <xsl:param name="content" />
  <xsl:param name="seperator" />
  <xsl:param name="target" />
  <xsl:param name="index" />
  <xsl:variable name="newlist" select="concat(normalize-space($content), $seperator)" />
  <xsl:variable name="first" select="substring-before($newlist, $seperator)" />
  <xsl:variable name="remaining" select="substring-after($newlist, $seperator)" />
  <xsl:variable name="index" select="$index+1" />
  <xsl:if test="$first != '' and $first = $target">
  <text><xsl:value-of select="$index"/></text>
  </xsl:if>
  <xsl:if test="substring-before($remaining, $seperator) != ''">
   <xsl:call-template name="output-tokens-index">
    <xsl:with-param name="content" select="$remaining" />
    <xsl:with-param name="seperator" select="$seperator" />
    <xsl:with-param name="target" select="$target" />
    <xsl:with-param name="index" select="$index" />
   </xsl:call-template>
  </xsl:if>
</xsl:template>

<xsl:template name="output-tokens-target">
  <xsl:param name="content" />
  <xsl:param name="seperator" />
  <xsl:param name="target-index" />
  <xsl:param name="index" />
  <xsl:variable name="newlist" select="concat(normalize-space($content), $seperator)" />
  <xsl:variable name="first" select="substring-before($newlist, $seperator)" />
  <xsl:variable name="remaining" select="substring-after($newlist, $seperator)" />
  <xsl:variable name="index" select="$index+1" />
  <xsl:if test="$index = $target-index">
  <text><xsl:value-of select="$first"/></text>
  </xsl:if>
  <xsl:if test="substring-before($remaining, $seperator) != ''">
   <xsl:call-template name="output-tokens-target">
    <xsl:with-param name="content" select="$remaining" />
    <xsl:with-param name="seperator" select="$seperator" />
    <xsl:with-param name="target-index" select="$target-index" />
    <xsl:with-param name="index" select="$index" />
   </xsl:call-template>
  </xsl:if>
</xsl:template>

</xsl:stylesheet>
