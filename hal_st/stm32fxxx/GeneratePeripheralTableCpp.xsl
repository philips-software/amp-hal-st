<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:mcu="http://mcd.rou.st.com/modules.php?name=mcu">
  <xsl:output method="text" omit-xml-declaration="yes" indent="no"/>
  <xsl:strip-space elements="*"/>

  <xsl:variable name="lowercase" select="'abcdefghijklmnopqrstuvwxyz'"/>
  <xsl:variable name="uppercase" select="'ABCDEFGHIJKLMNOPQRSTUVWXYZ'"/>

  <xsl:template match="/">
    <xsl:text>#include "PeripheralTable.hpp"
#include &lt;array&gt;
#include &lt;cstdlib&gt;

namespace hal
{
</xsl:text>
    <xsl:apply-templates/>
    <xsl:text>}
</xsl:text>
  </xsl:template>

  <xsl:template match="peripheral">
    <xsl:variable name="max">
      <xsl:for-each select="item">
        <xsl:sort select="@position" data-type="number" order="descending"/>
        <xsl:if test="position() = 1">
          <xsl:value-of select="@position"/>
        </xsl:if>
      </xsl:for-each>
    </xsl:variable>
    <xsl:variable name="base">
      <xsl:choose>
        <xsl:when test="@base">
            <xsl:value-of select="@base"/>
        </xsl:when>
        <xsl:otherwise>
            <xsl:text></xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    <xsl:if test="$max != ''">
        <xsl:text><![CDATA[    constexpr std::array<unsigned int, ]]></xsl:text><xsl:value-of select="$max"/><xsl:text><![CDATA[> peripheral]]></xsl:text><xsl:value-of select="@name"/><xsl:text><![CDATA[Array =]]>
    {{
</xsl:text>
      <xsl:if test="$max != ''">
        <xsl:call-template name="enumerate-items">
          <xsl:with-param name="index" select="1"/>
          <xsl:with-param name="max" select="$max"/>
          <xsl:with-param name="base" select="$base"/>
        </xsl:call-template>
      </xsl:if>
      <xsl:text>    }};

    <![CDATA[const infra::MemoryRange<]]></xsl:text>
    <xsl:value-of select="@type"/>
    <xsl:text><![CDATA[ const> peripheral]]></xsl:text>
    <xsl:value-of select="@name"/>
    <xsl:text><![CDATA[ = infra::ReinterpretCastMemoryRange<]]></xsl:text>
    <xsl:value-of select="@type"/>
    <xsl:text><![CDATA[ const>(infra::MakeRange(peripheral]]></xsl:text>
    <xsl:value-of select="@name"/>
    <xsl:text>Array</xsl:text>
    <xsl:text><![CDATA[))]]></xsl:text>
    <xsl:text>;

</xsl:text>
      <xsl:for-each select="interrupt">
        <xsl:text>    <![CDATA[constexpr std::array<IRQn_Type const, ]]></xsl:text>
        <xsl:choose>
          <xsl:when test="$max != ''">
            <xsl:value-of select="$max"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>0</xsl:text>
          </xsl:otherwise>
        </xsl:choose>
        <xsl:text><![CDATA[> peripheral]]></xsl:text><xsl:value-of select="../@name"/><xsl:value-of select="@name"/><xsl:text>IrqArray =
    {{
</xsl:text>
        <xsl:if test="$max != ''">
          <xsl:call-template name="enumerate-items-irq">
            <xsl:with-param name="index" select="1"/>
            <xsl:with-param name="max" select="$max"/>
          </xsl:call-template>
        </xsl:if>
        <xsl:text>    }};

    <![CDATA[constexpr infra::MemoryRange<IRQn_Type const> peripheral]]></xsl:text><xsl:value-of select="../@name"/><xsl:value-of select="@name"/><xsl:text>Irq = peripheral</xsl:text><xsl:value-of select="../@name"/><xsl:value-of select="@name"/><xsl:text>IrqArray;

</xsl:text>
      </xsl:for-each>
      <xsl:text>    void EnableClock</xsl:text><xsl:value-of select="@name"/><xsl:text>(std::size_t index)
    {
        switch (index)
        {
</xsl:text>
      <xsl:if test="$max != ''">
        <xsl:call-template name="enable-clock">
          <xsl:with-param name="index" select="1"/>
          <xsl:with-param name="max" select="$max"/>
        </xsl:call-template>
      </xsl:if>
      <xsl:text>            default: std::abort();
        }
    }

    void DisableClock</xsl:text><xsl:value-of select="@name"/><xsl:text>(std::size_t index)
    {
        switch (index)
        {
</xsl:text>
      <xsl:if test="$max != ''">
        <xsl:call-template name="disable-clock">
          <xsl:with-param name="index" select="1"/>
          <xsl:with-param name="max" select="$max"/>
        </xsl:call-template>
      </xsl:if>
      <xsl:text>            default: std::abort();
        }
    }
</xsl:text>

      <xsl:choose>
        <xsl:when test="@name = 'Spi' or @name = 'Adc'">
          <xsl:text>
    void ResetPeripheral</xsl:text>
          <xsl:value-of select="@name"/>
          <xsl:text>(std::size_t index)
    {
      switch (index)
      {
</xsl:text>
          <xsl:if test="$max != ''">
            <xsl:call-template name="reset-peripheral">
              <xsl:with-param name="index" select="1"/>
              <xsl:with-param name="max" select="$max"/>
            </xsl:call-template>
          </xsl:if>
        <xsl:text>            default: std::abort();
        }
    }

</xsl:text>
        </xsl:when>
      </xsl:choose>

    </xsl:if>
  </xsl:template>

  <xsl:template name="enumerate-items">
    <xsl:param name="index"/>
    <xsl:param name="max"/>
    <xsl:param name="base"/>
    <xsl:text>        </xsl:text>
    <xsl:choose>
      <xsl:when test="item[@position=$index] and $base != ''">
        <xsl:value-of select="$base"/>
      </xsl:when>
      <xsl:when test="item[@position=$index] and $base = ''">
        <xsl:value-of select="item[@position=$index]/@name"/>
        <xsl:text>_BASE</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>0</xsl:text>
      </xsl:otherwise>
    </xsl:choose>
    <xsl:if test="$index != $max">
      <xsl:text>,</xsl:text>
    </xsl:if>
    <xsl:text>
</xsl:text>
    <xsl:if test="$index != $max and $base = ''">
      <xsl:call-template name="enumerate-items">
        <xsl:with-param name="index" select="$index + 1"/>
        <xsl:with-param name="max" select="$max"/>
        <xsl:with-param name="base" select="$base"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <xsl:template name="enumerate-items-irq">
    <xsl:param name="index"/>
    <xsl:param name="max"/>
    <xsl:text>        </xsl:text>
    <xsl:choose>
      <xsl:when test="../item[@position=$index]">
        <xsl:value-of select="../item[@position=$index]/@name"/><xsl:value-of select="@postfix"/><xsl:text>_IRQn</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>(IRQn_Type const)255</xsl:text>
      </xsl:otherwise>
    </xsl:choose>
    <xsl:if test="$index != $max">
      <xsl:text>,</xsl:text>
    </xsl:if>
    <xsl:text>
</xsl:text>
    <xsl:if test="$index != $max">
      <xsl:call-template name="enumerate-items-irq">
        <xsl:with-param name="index" select="$index + 1"/>
        <xsl:with-param name="max" select="$max"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <xsl:template name="enable-clock">
    <xsl:param name="index"/>
    <xsl:param name="max"/>
    <xsl:if test="item[@position=$index]">
      <xsl:text>            case </xsl:text><xsl:value-of select="$index - 1"/><xsl:text>: </xsl:text>
      <xsl:choose>
        <xsl:when test="item[@position=$index]/@clock-enable">
          <xsl:value-of select="item[@position=$index]/@clock-enable"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>__HAL_RCC_</xsl:text><xsl:value-of select="item[@position=$index]/@name"/><xsl:text>_CLK_ENABLE</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
      <xsl:text>(); break;
</xsl:text>
    </xsl:if>
    <xsl:if test="$index != $max">
      <xsl:call-template name="enable-clock">
        <xsl:with-param name="index" select="$index + 1"/>
        <xsl:with-param name="max" select="$max"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <xsl:template name="disable-clock">
    <xsl:param name="index"/>
    <xsl:param name="max"/>
    <xsl:if test="item[@position=$index]">
      <xsl:text>            case </xsl:text><xsl:value-of select="$index - 1"/><xsl:text>: </xsl:text>
      <xsl:choose>
        <xsl:when test="item[@position=$index]/@clock-enable">
          <xsl:value-of select="substring(item[@position=$index]/@clock-enable, 1, string-length(item[@position=$index]/@clock-enable) - 7)"/><xsl:text>_DISABLE</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>__HAL_RCC_</xsl:text><xsl:value-of select="item[@position=$index]/@name"/><xsl:text>_CLK_DISABLE</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
      <xsl:text>(); break;
</xsl:text>
    </xsl:if>
    <xsl:if test="$index != $max">
      <xsl:call-template name="disable-clock">
        <xsl:with-param name="index" select="$index + 1"/>
        <xsl:with-param name="max" select="$max"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <xsl:template name="strip-trailing-digits">
    <xsl:param name="value"/>
    <xsl:variable name="length" select="string-length($value)"/>
    <xsl:choose>
      <xsl:when test="$length &gt; 0 and contains('0123456789', substring($value, $length, 1))">
        <xsl:call-template name="strip-trailing-digits">
          <xsl:with-param name="value" select="substring($value, 1, $length - 1)"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$value"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="reset-peripheral">
    <xsl:param name="index"/>
    <xsl:param name="max"/>
    <xsl:if test="item[@position=$index]">
      <xsl:text>            case </xsl:text>
      <xsl:value-of select="$index - 1"/>
      <xsl:text>: </xsl:text>
      <xsl:choose>
        <xsl:when test="item[@position=$index]/@clock-enable">
          <xsl:value-of select="substring(item[@position=$index]/@clock-enable, 1, string-length(item[@position=$index]/@clock-enable) - 11)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>__HAL_RCC_</xsl:text>
          <xsl:choose>
            <xsl:when test="contains(item[@position=$index]/@name, 'ADC')">
              <xsl:call-template name="strip-trailing-digits">
                <xsl:with-param name="value" select="item[@position=$index]/@name"/>
              </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="item[@position=$index]/@name"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:otherwise>
      </xsl:choose>
      <xsl:text>_FORCE_RESET(); </xsl:text>
      <xsl:choose>
        <xsl:when test="item[@position=$index]/@clock-enable">
          <xsl:value-of select="substring(item[@position=$index]/@clock-enable, 1, string-length(item[@position=$index]/@clock-enable) - 11)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>__HAL_RCC_</xsl:text>
          <xsl:choose>
            <xsl:when test="contains(item[@position=$index]/@name, 'ADC')">
              <xsl:call-template name="strip-trailing-digits">
                <xsl:with-param name="value" select="item[@position=$index]/@name"/>
              </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="item[@position=$index]/@name"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:otherwise>
      </xsl:choose>
      <xsl:text>_RELEASE_RESET(); break;
</xsl:text>
    </xsl:if>
    <xsl:if test="$index != $max">
      <xsl:call-template name="reset-peripheral">
        <xsl:with-param name="index" select="$index + 1"/>
        <xsl:with-param name="max" select="$max"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

</xsl:stylesheet>
