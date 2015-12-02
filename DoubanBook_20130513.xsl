<?xml version="1.0" encoding="gbk"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
 <xsl:import href ="./COMMON/split.xsl"/>
 <xsl:output method="xml" encoding="gbk"/>
 <xsl:param name="template"/>
 <xsl:param name="oldtemplate"/>
 <xsl:param name="url"/>

 <xsl:strip-space elements="title name alais" />
 <xsl:preserve-space elements="image" />

 <xsl:template match="/">
  <display>
   <template><xsl:value-of select="$template"/></template>
   <template-old><xsl:value-of select="$oldtemplate"/></template-old>
   <url><xsl:value-of select="$url"/></url>
   <type>MOVIEHOME</type>
	 <xsl:apply-templates />
	</display>
 </xsl:template>

 <xsl:template match="//title">
  <title>
   <xsl:value-of select="normalize-space(.)"/>
  </title>
 </xsl:template>


 <xsl:template match="//h1//span">
  <name>
   <xsl:value-of select="normalize-space(.)"/>
  </name>
 </xsl:template>

 <xsl:template match="//div[@class='subject clearfix']//div[@id='info']">
 	<image>
		<xsl:value-of select="//div[@id='mainpic']//img/@src"/>
	</image>
 	<xsl:for-each select="./span">
 		<xsl:if test="contains(., ' 作者')">
		<authorset>
			<xsl:for-each select=".//a" >
				<author>
					<text><xsl:value-of select="normalize-space(.)"/></text>
					<url><xsl:value-of select="concat('http://movie.douban.com',./@href)"/></url>
				</author>
			</xsl:for-each>
		</authorset>
		</xsl:if>
		<xsl:if test="contains(., '出版社:')">
		 	<publisher>
		  	<text><xsl:value-of select="following-sibling::text()[1]"/></text>
				<url><xsl:value-of select="concat('http://movie.douban.com',./@href)"/></url>
		  </publisher>
		</xsl:if>
		<xsl:if test="contains(., '原作名:')">
		 	<originname>
		  	<xsl:value-of select="normalize-space(following-sibling::text()[1])"/>
		  </originname>
		</xsl:if>
		<xsl:if test="contains(., '页数:')">
			<pagenum>
			  <xsl:value-of select='following-sibling::text()[1]'/>
			</pagenum>
		</xsl:if>
		<xsl:if test="contains(., '定价:')">
			<price>
				<xsl:value-of select='following-sibling::text()[1]'/>
			</price>
		</xsl:if>
		<xsl:if test="contains(., '出版年:')">
			<initialreleasedate>
				<xsl:value-of select='normalize-space(following-sibling::text()[1])'/>
			</initialreleasedate>
		</xsl:if>
		<xsl:if test="contains(., 'ISBN:')">
		  <isbn>
			  <xsl:value-of select='normalize-space(following-sibling::text()[1])'/>
			</isbn>
		</xsl:if>
	</xsl:for-each>
 </xsl:template>

 <xsl:template match="//div[@id='interest_sectl']">
	<score>
	  <xsl:value-of select=".//p[@class='rating_self clearfix']//strong[@class='ll rating_num']"/>
  </score>
  <star>
	  <xsl:variable name="star" select=".//p[@class='rating_self clearfix']//span[contains(./@class, 'bigstar')]/@class" />
	  <xsl:value-of select="concat(substring($star, 11, 1), '.', substring($star, 12, 1))"/>
  </star>
  <rate>
    <xsl:value-of select=".//span[@property='v:votes']"/>
  </rate>
 </xsl:template>
	
	<xsl:template match="//div[@id='buyinfo-printed']">
		<productset>
			<xsl:for-each select=".//ul//a[@target='_blank']">
				<product>
					<text><xsl:value-of select=".//span"/></text>
					<url><xsl:value-of select="./@href"/></url>
				</product>
			</xsl:for-each>
		</productset>
	</xsl:template>
 
 <xsl:template match="//div[@class='related_info']">
 	 <synopsis>
	 	<xsl:value-of select=".//div[@id='link-report']//div[@class='intro']"/>
	 </synopsis>
	 <tagset>
		<xsl:for-each select=".//div[@id='db-tags-section']//div[@class='indent']//a">
			<tag>
				<text><xsl:value-of select="."/></text>
				<url><xsl:value-of select="./@href"/></url>
			</tag>
		</xsl:for-each>
	</tagset>
	<recset>
		<xsl:for-each select=".//div[@id='db-rec-section']//div[@class='content clearfix']//dl">
			<rec>
				<text><xsl:value-of select=".//dd//a/text()"/></text>
				<url><xsl:value-of select=".//dd//a/@href"/></url>
				<img><xsl:value-of select=".//dt//img/@src"/></img>
			</rec>
		</xsl:for-each>
	</recset>
 	<commentset>
	  <xsl:for-each select=".//div[@id='wt_0']//div[@class='ctsh']">
	    <comment>
	      <text><xsl:value-of select=".//div[@class='clst']//div[@class='review-short']//span"/></text>
	      <url><xsl:value-of select=".//div[@class='nlst']//a/@href"/></url>
	      <title><xsl:value-of select=".//div[@class='nlst']//a/@title"/></title>
				<xsl:variable name="star" select=".//span[contains(./@class, 'allstar')]/@class" />
				<score>
				  <xsl:if test="$star">
				   <xsl:value-of select="concat(substring($star, 8, 1), '.', substring($star, 9, 1))"/>
				  </xsl:if>
				</score>
				<time><xsl:value-of select=".//div[@class='pl clearfix']//span[@class='fleft']"/></time>
	    </comment>
	  </xsl:for-each>
	</commentset>
 </xsl:template> 
 
 <xsl:template match="text()|@*"/>

</xsl:stylesheet>
