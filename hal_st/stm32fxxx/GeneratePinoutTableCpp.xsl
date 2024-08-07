<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:mcu="http://mcd.rou.st.com/modules.php?name=mcu">
  <xsl:output method="text" omit-xml-declaration="yes" indent="no"/>
  <xsl:strip-space elements="*"/>

  <xsl:variable name="lowercase" select="'abcdefghijklmnopqrstuvwxyz'"/>
  <xsl:variable name="uppercase" select="'ABCDEFGHIJKLMNOPQRSTUVWXYZ'"/>

  <xsl:template match="/">
    <xsl:text>#include "generated/stm32fxxx/PinoutTableDefault.hpp"

namespace hal
{
</xsl:text>
    <xsl:apply-templates/>
    <xsl:text>
}
</xsl:text>
  </xsl:template>

  <xsl:template match="peripheral">
    <xsl:apply-templates mode="list-pins"/>
    <xsl:text><![CDATA[    constexpr std::array<const GpioStm::PinoutTable, ]]></xsl:text>
    <xsl:value-of select="count(pin[*])"/>
    <xsl:text><![CDATA[> pinoutTable]]></xsl:text>
    <xsl:value-of select="@name"/>
    <xsl:text><![CDATA[Array =]]>
    {{
</xsl:text>
    <xsl:apply-templates mode="list-config"/>
    <xsl:text>    }};

</xsl:text>
    <xsl:text><![CDATA[    constexpr infra::MemoryRange<const GpioStm::PinoutTable> pinoutTable]]></xsl:text>
    <xsl:value-of select="@name"/>
    <xsl:text><![CDATA[ = pinoutTable]]></xsl:text>
    <xsl:value-of select="@name"/>
    <xsl:text><![CDATA[Array;]]>

</xsl:text>
  </xsl:template>

  <xsl:template match="pin" mode="list-pins">
    <xsl:if test="count(*) != 0">
      <xsl:text><![CDATA[    constexpr std::array<const GpioStm::PinPosition, ]]></xsl:text>
      <xsl:value-of select="count(gpio_pin)"/>
      <xsl:text><![CDATA[> pinoutTable]]></xsl:text>
      <xsl:value-of select="../@name"/>
      <xsl:value-of select="@name"/>
      <xsl:text><![CDATA[Pins =]]>
    {{
</xsl:text>
      <xsl:apply-templates>
        <xsl:sort select="@peripheral-index" data-type="number"/>
        <xsl:sort select="@port"/>
        <xsl:sort select="@pin-index" data-type="number"/>
      </xsl:apply-templates>
      <xsl:if test="count(gpio_pin) mod 4 != 0">
        <xsl:text>
</xsl:text>
      </xsl:if>
      <xsl:text>    }};

</xsl:text>
    </xsl:if>
  </xsl:template>

  <xsl:template match="gpio_pin">
    <xsl:if test="position() mod 4 = 1">
      <xsl:text>       </xsl:text>
    </xsl:if>
    <xsl:text> { </xsl:text>
    <xsl:value-of select="@peripheral-index"/>
    <xsl:text>, Port::</xsl:text>
    <xsl:value-of select="@port"/>
    <xsl:text>, </xsl:text>
    <xsl:value-of select="@pin-index"/>
    <xsl:text>, </xsl:text>
    <xsl:value-of select="@alternate-function"/>
    <xsl:text> }</xsl:text>
    <xsl:if test="position() != last()">
      <xsl:text>,</xsl:text>
    </xsl:if>
    <xsl:if test="position() mod 4 = 0">
      <xsl:text>
</xsl:text>
    </xsl:if>
  </xsl:template>

  <xsl:template match="pin" mode="list-config">
    <xsl:if test="count(*) != 0">
      <xsl:text>        {
</xsl:text>
      <xsl:text>            PinConfigTypeStm::</xsl:text>
      <xsl:value-of select="concat(translate(substring(../@name, 1, 1), $uppercase, $lowercase), substring(../@name, 2))"/>
      <xsl:value-of select="@name"/>
      <xsl:text>,
</xsl:text>
      <xsl:text>            pinoutTable</xsl:text>
      <xsl:value-of select="../@name"/>
      <xsl:value-of select="@name"/>
      <xsl:text>Pins,
            Drive::</xsl:text>
      <xsl:value-of select="@drive"/>
      <xsl:text>,
            Speed::</xsl:text>
      <xsl:value-of select="@speed"/>
      <xsl:text>,
            WeakPull::</xsl:text>
      <xsl:value-of select="@weak-pull"/>
      <xsl:text>
</xsl:text>
      <xsl:choose>
        <xsl:when test="position() != last()">
          <xsl:text>        },
</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>        }
</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:if>
  </xsl:template>

  <xsl:template match="default_table">
    <xsl:text><![CDATA[    constexpr std::array<const infra::MemoryRange<const GpioStm::PinoutTable>, ]]></xsl:text>
    <xsl:value-of select="size"/>
    <xsl:text><![CDATA[> pinoutTableDefaultStmArray = { ]]></xsl:text>
    <xsl:for-each select="names/*">
      <xsl:text>pinoutTable</xsl:text>
      <xsl:value-of select="."/>
      <xsl:if test="position() != last()">
        <xsl:text>, </xsl:text>
      </xsl:if>
    </xsl:for-each>
    <xsl:text> };
</xsl:text>
    <xsl:text><![CDATA[    constexpr infra::MemoryRange<const infra::MemoryRange<const GpioStm::PinoutTable>> pinoutTableDefaultStm = pinoutTableDefaultStmArray;]]>
</xsl:text>
  </xsl:template>

  <xsl:template match="analog_pin">
    <xsl:text>        { </xsl:text>
    <xsl:value-of select="@instance"/>
    <xsl:text>, Type::</xsl:text>
    <xsl:value-of select="@type"/>
    <xsl:text>, Port::</xsl:text>
    <xsl:value-of select="@port"/>
    <xsl:text>, </xsl:text>
    <xsl:value-of select="@pin-index"/>
    <xsl:text>, </xsl:text>
    <xsl:value-of select="@channel"/>
    <xsl:if test="@channelType">
      <xsl:text>, AdcChannelType::</xsl:text>
      <xsl:value-of select="@channelType"/>
    </xsl:if>
    <xsl:text> },
    </xsl:text>
  </xsl:template>

  <xsl:template match="analog_default_table">
    <xsl:text><![CDATA[    constexpr std::array<const GpioStm::AnalogPinPosition, ]]></xsl:text>
    <xsl:value-of select="count(*)"/>
    <xsl:text><![CDATA[> analogTableDefaultStmArray =]]>
    {{
</xsl:text>
    <xsl:apply-templates>
      <xsl:sort select="@port"/>
      <xsl:sort select="@pin-index" data-type="number"/>
    </xsl:apply-templates>
    <xsl:text>    }};
</xsl:text>
    <xsl:text><![CDATA[    constexpr infra::MemoryRange<const GpioStm::AnalogPinPosition> analogTableDefaultStm = analogTableDefaultStmArray;]]></xsl:text>
  </xsl:template>

</xsl:stylesheet>
