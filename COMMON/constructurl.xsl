<?xml version="1.0" encoding="gbk"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template name="construct-full-url">
  <xsl:param name="url" />
  <xsl:param name="link" />
  <xsl:choose>
    <xsl:when test="starts-with($link, '/')">
      <xsl:value-of select="concat(concat('http://', substring-before(substring-after($url, 'http://'), '/')), $link)" />
    </xsl:when>
    <xsl:otherwise>
      <xsl:value-of select="$link" />
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

</xsl:stylesheet>
