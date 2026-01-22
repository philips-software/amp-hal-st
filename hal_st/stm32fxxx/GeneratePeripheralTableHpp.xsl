<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:mcu="http://mcd.rou.st.com/modules.php?name=mcu">
  <xsl:output method="text" omit-xml-declaration="yes" indent="no"/>
  <xsl:strip-space elements="*"/>

  <xsl:variable name="lowercase" select="'abcdefghijklmnopqrstuvwxyz'"/>
  <xsl:variable name="uppercase" select="'ABCDEFGHIJKLMNOPQRSTUVWXYZ'"/>

  <xsl:template match="/">
    <xsl:text>#ifndef HAL_PERIPHERAL_TABLE_HPP
#define HAL_PERIPHERAL_TABLE_HPP

#include "infra/util/MemoryRange.hpp"
#include DEVICE_HEADER

namespace hal
{</xsl:text>
    <xsl:apply-templates/>
    <xsl:text>}

#endif
</xsl:text>
  </xsl:template>

  <xsl:template match="peripheral">
    <xsl:text>
    </xsl:text>
    <xsl:choose>
      <xsl:when test="item">
        <xsl:text>#define HAS_PERIPHERAL_</xsl:text><xsl:value-of select="translate(@name, $lowercase, $uppercase)"/>
        <xsl:text>
<![CDATA[    extern const infra::MemoryRange<]]></xsl:text><xsl:value-of select="@type"/><xsl:text><![CDATA[ const> peripheral]]></xsl:text><xsl:value-of select="@name"/><xsl:text>;</xsl:text>
        <xsl:for-each select="interrupt">
          <xsl:text>
<![CDATA[    extern const infra::MemoryRange<IRQn_Type const> peripheral]]></xsl:text><xsl:value-of select="../@name"/><xsl:value-of select="@name"/><xsl:text>Irq;</xsl:text>
        </xsl:for-each>
        <xsl:text>
    void EnableClock</xsl:text><xsl:value-of select="@name"/><xsl:text>(std::size_t index);
    void DisableClock</xsl:text><xsl:value-of select="@name"/><xsl:text>(std::size_t index);</xsl:text>
        <xsl:choose>
          <xsl:when test="@name = 'Spi' or @name = 'Adc'">
            <xsl:text>
    void ResetPeripheral</xsl:text><xsl:value-of select="@name"/><xsl:text>(std::size_t index);</xsl:text>
          </xsl:when>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>#undef HAS_PERIPHERAL_</xsl:text><xsl:value-of select="translate(@name, $lowercase, $uppercase)"/>
      </xsl:otherwise>
    </xsl:choose>
    <xsl:text>
</xsl:text>
  </xsl:template>

</xsl:stylesheet>
