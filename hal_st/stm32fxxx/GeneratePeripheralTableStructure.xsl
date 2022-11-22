<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:mcu="http://mcd.rou.st.com/modules.php?name=mcu">
  <xsl:param name="mcu-document"/>
  <xsl:output method="xml" indent="yes"/>
  <xsl:strip-space elements="*"/>

  <xsl:variable name="mcu-document-no-spaces">
    <xsl:call-template name="string-replace-all">
      <xsl:with-param name="text" select="$mcu-document"/>
      <xsl:with-param name="replace" select="' '"/>
      <xsl:with-param name="by" select="'&#37;20'"/>
    </xsl:call-template>
  </xsl:variable>

  <xsl:template name="string-replace-all">
    <xsl:param name="text"/>
    <xsl:param name="replace"/>
    <xsl:param name="by"/>
    <xsl:choose>
      <xsl:when test="$text = '' or $replace = '' or not($replace)">
        <!-- Prevent this routine from hanging -->
        <xsl:value-of select="$text"/>
      </xsl:when>
      <xsl:when test="contains($text, $replace)">
        <xsl:value-of select="substring-before($text,$replace)"/>
        <xsl:value-of select="$by"/>
        <xsl:call-template name="string-replace-all">
          <xsl:with-param name="text" select="substring-after($text,$replace)"/>
          <xsl:with-param name="replace" select="$replace"/>
          <xsl:with-param name="by" select="$by"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$text"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
  <xsl:template match="peripherals">
    <peripherals>
      <xsl:apply-templates/>
    </peripherals>
  </xsl:template>

  <xsl:template match="peripheral">
    <peripheral>
      <xsl:attribute name="name"><xsl:value-of select="@name"/></xsl:attribute>
      <xsl:attribute name="type"><xsl:value-of select="@type"/></xsl:attribute>
      <xsl:choose>
        <xsl:when test="@base">
          <xsl:attribute name="base"><xsl:value-of select="@base"/></xsl:attribute>
        </xsl:when>
      </xsl:choose>
      <xsl:apply-templates>
        <xsl:with-param name="prefix"><xsl:value-of select="@prefix"/></xsl:with-param>
      </xsl:apply-templates>
    </peripheral>
  </xsl:template>

  <xsl:template match="item">
    <xsl:param name="prefix"/>
    <xsl:apply-templates select="document($mcu-document-no-spaces)/mcu:Mcu/mcu:IP">
      <xsl:with-param name="ip-name"><xsl:value-of select="@name"/></xsl:with-param>
      <xsl:with-param name="prefix"><xsl:value-of select="$prefix"/></xsl:with-param>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template match="mcu:IP">
    <xsl:param name="ip-name"/>
    <xsl:param name="prefix"/>
    <xsl:if test="@Name = $ip-name">
      <item>
        <xsl:attribute name="name"><xsl:value-of select="@InstanceName"/></xsl:attribute>
        <xsl:choose>
          <xsl:when test="$prefix = '' and @InstanceName = @Name">
            <xsl:attribute name="position">1</xsl:attribute>
          </xsl:when>
          <xsl:when test="$prefix = '' and @InstanceName != @Name">
            <xsl:attribute name="position"><xsl:value-of select="substring(@InstanceName, string-length(@Name) + 1)"/></xsl:attribute>
          </xsl:when>
          <xsl:otherwise>
            <xsl:attribute name="position"><xsl:value-of select="substring(@InstanceName, string-length($prefix) + 1)"/></xsl:attribute>
          </xsl:otherwise>
        </xsl:choose>
        <xsl:if test="@ClockEnableMode">
          <xsl:attribute name="clock-enable">
            <xsl:call-template name="string-replace-all">
              <xsl:with-param name="text" select="@ClockEnableMode"/>
              <xsl:with-param name="replace" select="';'"/>
              <xsl:with-param name="by" select="'();'"/>
            </xsl:call-template>
          </xsl:attribute>
        </xsl:if>
      </item>
    </xsl:if>
  </xsl:template>

  <xsl:template match="interrupt">
    <xsl:copy>
      <xsl:copy-of select="@*"/>
    </xsl:copy>
  </xsl:template>
  
</xsl:stylesheet>
