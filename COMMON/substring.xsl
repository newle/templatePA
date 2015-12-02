<?xml version="1.0" encoding="gbk"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template name="substring-after-last">
  <xsl:param name="string" />
  <xsl:param name="delimiter" />
  <xsl:choose>
    <xsl:when test="contains($string, $delimiter)">
      <xsl:call-template name="substring-after-last">
        <xsl:with-param name="string" select="substring-after($string, $delimiter)" />
        <xsl:with-param name="delimiter" select="$delimiter" />
      </xsl:call-template>
    </xsl:when>
    <xsl:otherwise>
      <xsl:value-of select="$string" />
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template name="substring-before-last">
  <xsl:param name="string" />
  <xsl:param name="delimiter" />
  <xsl:variable name="last">
  <xsl:call-template name="substring-after-last">
    <xsl:with-param name="string" select="$string" />
    <xsl:with-param name="delimiter" select="$delimiter" />
  </xsl:call-template>
  </xsl:variable>
  <xsl:value-of select="substring-before($string, concat($delimiter, $last))" />
</xsl:template>

</xsl:stylesheet>
