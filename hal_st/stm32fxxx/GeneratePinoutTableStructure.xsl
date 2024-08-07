<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:mcu="http://mcd.rou.st.com/modules.php?name=mcu">
  <xsl:param name="gpio-document"/>
  <xsl:param name="mcu-document"/>
  <xsl:output method="xml" indent="yes"/>
  <xsl:strip-space elements="*"/>

  <xsl:variable name="gpio-document-no-spaces">
    <xsl:call-template name="string-replace-all">
      <xsl:with-param name="text" select="$gpio-document"/>
      <xsl:with-param name="replace" select="' '"/>
      <xsl:with-param name="by" select="'&#37;20'"/>
    </xsl:call-template>
  </xsl:variable>

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
    <table_structure>
      <peripherals>
        <xsl:apply-templates/>
      </peripherals>
      <xsl:call-template name="generate_default_table"/>
      <xsl:call-template name="generate_analog_default_table"/>
    </table_structure>
  </xsl:template>

  <xsl:template match="peripheral">
    <peripheral>
      <xsl:attribute name="name">
        <xsl:value-of select="@name"/>
      </xsl:attribute>
      <xsl:apply-templates/>
    </peripheral>
  </xsl:template>

  <xsl:template match="pin">
    <pin>
      <xsl:attribute name="name">
        <xsl:value-of select="@name"/>
      </xsl:attribute>
      <xsl:choose>
        <xsl:when test="@drive != ''">
          <xsl:attribute name="drive">
            <xsl:value-of select="@drive"/>
          </xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="drive">Default</xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>

      <xsl:choose>
        <xsl:when test="@speed != ''">
          <xsl:attribute name="speed">
            <xsl:value-of select="@speed"/>
          </xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="speed">Default</xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>

      <xsl:choose>
        <xsl:when test="@weak-pull != ''">
          <xsl:attribute name="weak-pull">
            <xsl:value-of select="@weak-pull"/>
          </xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="weak-pull">Default</xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>

      <xsl:apply-templates/>
    </pin>
  </xsl:template>

  <xsl:template match="input">
    <xsl:choose>
      <xsl:when test="contains(@name, ':')">
        <xsl:apply-templates select="document($gpio-document-no-spaces)/*/mcu:GPIO_Pin/mcu:PinSignal">
          <xsl:with-param name="prefix">
            <xsl:value-of select="substring-before(@name, ':')"/>
          </xsl:with-param>
          <xsl:with-param name="postfix">
            <xsl:value-of select="substring-after(@name, ':')"/>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:when>
      <xsl:otherwise>
        <xsl:apply-templates select="document($gpio-document-no-spaces)/*/mcu:GPIO_Pin/mcu:PinSignal">
          <xsl:with-param name="prefix">
            <xsl:value-of select="@name"/>
          </xsl:with-param>
          <xsl:with-param name="postfix"/>
        </xsl:apply-templates>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="mcu:PinSignal">
    <xsl:param name="prefix"/>
    <xsl:param name="postfix"/>

    <xsl:if test="starts-with(@Name, $prefix) and ($postfix = substring(@Name, string-length(@Name) - string-length($postfix) + 1)) and (substring(../mcu:SpecificParameter[@Name='GPIO_Pin'], 10))">
      <xsl:variable name="pin_signal">
        <xsl:value-of select="@Name"/>
      </xsl:variable>
      <xsl:variable name="mcu_has_pin">
        <!-- Lookup mcu file to check if this pin exists and supports this signal, as ip file is too generic for a mcu family -->
        <xsl:for-each select="document($mcu-document-no-spaces)/mcu:Mcu/mcu:Pin[contains(@Name, ../@Name)]">
          <xsl:for-each select="mcu:Signal[starts-with(@Name, $pin_signal)]">
            <xsl:value-of select="$pin_signal"/>
          </xsl:for-each>
        </xsl:for-each>
      </xsl:variable>

      <xsl:if test="$mcu_has_pin != ''">
        <gpio_pin>
          <xsl:attribute name="port">
            <xsl:value-of select="substring(../@PortName, 2)"/>
          </xsl:attribute>
          <xsl:attribute name="pin-index">
            <xsl:value-of select="substring(../mcu:SpecificParameter[@Name='GPIO_Pin'], 10)"/>
          </xsl:attribute>
          <xsl:choose>
            <xsl:when test="$postfix != '' and concat($prefix, $postfix) != @Name">
              <xsl:attribute name="peripheral-index">
                <xsl:value-of select="substring(@Name, string-length($prefix) + 1, string-length(@Name) - string-length($prefix) - string-length($postfix))"/>
              </xsl:attribute>
            </xsl:when>
            <xsl:otherwise>
              <xsl:attribute name="peripheral-index">0</xsl:attribute>
            </xsl:otherwise>
          </xsl:choose>
          <xsl:attribute name="alternate-function">
            <xsl:value-of select="mcu:SpecificParameter[@Name='GPIO_AF']"/>
          </xsl:attribute>
        </gpio_pin>
      </xsl:if>
    </xsl:if>
  </xsl:template>

  <xsl:template match="peripheral" mode="get_table_names">
    <name>
      <xsl:value-of select="@name"/>
    </name>
  </xsl:template>

  <xsl:template name="generate_default_table">
    <default_table>
      <size>
        <xsl:value-of select="count(peripheral)"/>
      </size>
      <names>
        <xsl:apply-templates mode="get_table_names"/>
      </names>
    </default_table>
  </xsl:template>

  <xsl:template match="mcu:Signal" mode="analog_default_table">
    <xsl:if test="starts-with(@Name, 'ADC') and contains(@Name, '_IN')">
      <analog_pin>
        <xsl:attribute name="instance">
          <xsl:value-of select="substring(@Name, 4, 1)"/>
        </xsl:attribute>
        <xsl:attribute name="type">adc</xsl:attribute>
        <xsl:attribute name="port">
          <xsl:value-of select="substring(../@Name, 2, 1)"/>
        </xsl:attribute>
        <xsl:attribute name="pin-index">
          <xsl:value-of select="substring-before(concat(translate(substring(../@Name, 3), '-', '/'), '/'), '/')"/>
        </xsl:attribute>
        <xsl:choose>
          <xsl:when test="substring(@Name, 8, 1)='P' or substring(@Name, 8, 1)='N'">
            <xsl:attribute name="channel">
              <xsl:value-of select="substring(@Name, 9)"/>
            </xsl:attribute>
            <xsl:attribute name="channelType">
              <xsl:value-of select="substring(@Name, 8, 1)"/>
            </xsl:attribute>
          </xsl:when>
          <xsl:otherwise>
            <xsl:attribute name="channel">
              <xsl:value-of select="substring(@Name, 8)"/>
            </xsl:attribute>
          </xsl:otherwise>
        </xsl:choose>
      </analog_pin>
    </xsl:if>
    <xsl:if test="starts-with(@Name, 'DAC') and contains(@Name, '_OUT')">
      <analog_pin>
        <xsl:choose>
          <xsl:when test="substring(@Name, 4, 1)='_'">
            <xsl:attribute name="instance">
              <xsl:value-of select="1"/>
            </xsl:attribute>
          </xsl:when>
          <xsl:otherwise>
            <xsl:attribute name="instance">
              <xsl:value-of select="substring(@Name, 4, 1)"/>
            </xsl:attribute>
          </xsl:otherwise>
        </xsl:choose>
        <xsl:attribute name="type">dac</xsl:attribute>
        <xsl:attribute name="port">
          <xsl:value-of select="substring(../@Name, 2, 1)"/>
        </xsl:attribute>
        <xsl:attribute name="pin-index">
          <xsl:value-of select="substring-before(concat(translate(substring(../@Name, 3), '-', '/'), '/'), '/')"/>
        </xsl:attribute>
        <xsl:attribute name="channel">
          <xsl:value-of select="substring(@Name, string-length(@Name))"/>
        </xsl:attribute>
      </analog_pin>
    </xsl:if>
  </xsl:template>

  <xsl:template name="generate_analog_default_table">
    <analog_default_table>
      <xsl:apply-templates select="document($mcu-document-no-spaces)/mcu:Mcu/mcu:Pin" mode="analog_default_table"/>
    </analog_default_table>
  </xsl:template>
</xsl:stylesheet>
