<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:mcu="http://mcd.rou.st.com/modules.php?name=mcu">
  <xsl:output method="text" omit-xml-declaration="yes" indent="no"/>
  <xsl:strip-space elements="*"/>

  <xsl:template match="/">
    <xsl:text>#ifndef HAL_PINOUT_TABLE_DEFAULT_HPP
#define HAL_PINOUT_TABLE_DEFAULT_HPP

#include "hal_st/stm32fxxx/GpioStm.hpp"
#include "infra/util/MemoryRange.hpp"

namespace hal
{
    <![CDATA[extern const infra::MemoryRange<const infra::MemoryRange<const GpioStm::PinoutTable>> pinoutTableDefaultStm;]]>
    <![CDATA[extern const infra::MemoryRange<const GpioStm::AnalogPinPosition> analogTableDefaultStm;]]>
</xsl:text>
    <xsl:apply-templates/>
    <xsl:text>
}

#endif
</xsl:text>
  </xsl:template>

  <xsl:template match="peripheral">
    <xsl:text>
<![CDATA[    extern const infra::MemoryRange<const GpioStm::PinoutTable> pinoutTable]]></xsl:text><xsl:value-of select="@name"/><xsl:text>;</xsl:text>
  </xsl:template>

  <xsl:template match="default_table"/>

</xsl:stylesheet>
