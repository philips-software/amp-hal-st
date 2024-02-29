<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="6.3">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="2" name="Route2" color="1" fill="3" visible="no" active="no"/>
<layer number="3" name="Route3" color="4" fill="3" visible="no" active="no"/>
<layer number="4" name="Route4" color="1" fill="4" visible="no" active="no"/>
<layer number="5" name="Route5" color="4" fill="4" visible="no" active="no"/>
<layer number="6" name="Route6" color="1" fill="8" visible="no" active="no"/>
<layer number="7" name="Route7" color="4" fill="8" visible="no" active="no"/>
<layer number="8" name="Route8" color="1" fill="2" visible="no" active="no"/>
<layer number="9" name="Route9" color="4" fill="2" visible="no" active="no"/>
<layer number="10" name="Route10" color="1" fill="7" visible="no" active="no"/>
<layer number="11" name="Route11" color="4" fill="7" visible="no" active="no"/>
<layer number="12" name="Route12" color="1" fill="5" visible="no" active="no"/>
<layer number="13" name="Route13" color="4" fill="5" visible="no" active="no"/>
<layer number="14" name="Route14" color="1" fill="6" visible="no" active="no"/>
<layer number="15" name="Route15" color="4" fill="6" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="header">
<packages>
<package name="NUCLEO-767">
<wire x1="-21.59" y1="18.415" x2="-21.59" y2="17.145" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="17.145" x2="-22.225" y2="16.51" width="0.1524" layer="21"/>
<wire x1="-22.225" y1="16.51" x2="-21.59" y2="15.875" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="15.875" x2="-21.59" y2="14.605" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="14.605" x2="-22.225" y2="13.97" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="18.415" x2="-22.225" y2="19.05" width="0.1524" layer="21"/>
<wire x1="-22.225" y1="13.97" x2="-21.59" y2="13.335" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="13.335" x2="-21.59" y2="12.065" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="12.065" x2="-22.225" y2="11.43" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="10.795" x2="-21.59" y2="9.525" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="9.525" x2="-22.225" y2="8.89" width="0.1524" layer="21"/>
<wire x1="-22.225" y1="8.89" x2="-21.59" y2="8.255" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="8.255" x2="-21.59" y2="6.985" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="6.985" x2="-22.225" y2="6.35" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="10.795" x2="-22.225" y2="11.43" width="0.1524" layer="21"/>
<wire x1="-22.225" y1="6.35" x2="-21.59" y2="5.715" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="5.715" x2="-21.59" y2="4.445" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="4.445" x2="-22.225" y2="3.81" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="16.51" x2="-26.67" y2="17.145" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="13.97" x2="-26.67" y2="14.605" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="14.605" x2="-26.67" y2="15.875" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="15.875" x2="-26.035" y2="16.51" width="0.1524" layer="21"/>
<wire x1="-22.225" y1="19.05" x2="-26.035" y2="19.05" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="19.05" x2="-26.67" y2="18.415" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="17.145" x2="-26.67" y2="18.415" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="11.43" x2="-26.67" y2="12.065" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="12.065" x2="-26.67" y2="13.335" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="13.335" x2="-26.035" y2="13.97" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="8.89" x2="-26.67" y2="9.525" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="6.35" x2="-26.67" y2="6.985" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="6.985" x2="-26.67" y2="8.255" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="8.255" x2="-26.035" y2="8.89" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="11.43" x2="-26.67" y2="10.795" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="9.525" x2="-26.67" y2="10.795" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="3.81" x2="-26.67" y2="4.445" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="4.445" x2="-26.67" y2="5.715" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="5.715" x2="-26.035" y2="6.35" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="1.905" x2="-22.225" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="3.175" x2="-21.59" y2="1.905" width="0.1524" layer="21"/>
<wire x1="-22.225" y1="3.81" x2="-21.59" y2="3.175" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="3.175" x2="-26.035" y2="3.81" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="1.905" x2="-26.67" y2="3.175" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="1.27" x2="-26.67" y2="1.905" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="0.635" x2="-21.59" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-0.635" x2="-22.225" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="0.635" x2="-22.225" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="-1.27" x2="-26.67" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="1.27" x2="-26.67" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="-0.635" x2="-26.67" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-22.225" y1="-1.27" x2="-26.035" y2="-1.27" width="0.1524" layer="21"/>
<pad name="NC1" x="-25.4" y="17.78" drill="1" shape="octagon" rot="R270" first="yes"/>
<pad name="IOREF" x="-25.4" y="15.24" drill="1" rot="R270"/>
<pad name="RESET" x="-25.4" y="12.7" drill="1" rot="R270"/>
<pad name="+3V3" x="-25.4" y="10.16" drill="1" rot="R270"/>
<pad name="+5V" x="-25.4" y="7.62" drill="1" rot="R270"/>
<pad name="GND1" x="-25.4" y="5.08" drill="1" rot="R270"/>
<pad name="PC8" x="-22.86" y="17.78" drill="1" rot="R270"/>
<pad name="PC9" x="-22.86" y="15.24" drill="1" rot="R270"/>
<pad name="PC10" x="-22.86" y="12.7" drill="1" rot="R270"/>
<pad name="PC11" x="-22.86" y="10.16" drill="1" rot="R270"/>
<pad name="PC12" x="-22.86" y="7.62" drill="1" rot="R270"/>
<pad name="PD2" x="-22.86" y="5.08" drill="1" rot="R270"/>
<pad name="GND2" x="-25.4" y="2.54" drill="1" rot="R270"/>
<pad name="PG2" x="-22.86" y="2.54" drill="1" rot="R270"/>
<pad name="VIN" x="-25.4" y="0" drill="1" rot="R270"/>
<pad name="PG3" x="-22.86" y="0" drill="1" rot="R270"/>
<wire x1="-21.59" y1="-4.445" x2="-21.59" y2="-5.715" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-5.715" x2="-22.225" y2="-6.35" width="0.1524" layer="21"/>
<wire x1="-22.225" y1="-6.35" x2="-21.59" y2="-6.985" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-6.985" x2="-21.59" y2="-8.255" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-8.255" x2="-22.225" y2="-8.89" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-4.445" x2="-22.225" y2="-3.81" width="0.1524" layer="21"/>
<wire x1="-22.225" y1="-8.89" x2="-21.59" y2="-9.525" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-9.525" x2="-21.59" y2="-10.795" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-10.795" x2="-22.225" y2="-11.43" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-12.065" x2="-21.59" y2="-13.335" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-13.335" x2="-22.225" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="-22.225" y1="-13.97" x2="-21.59" y2="-14.605" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-14.605" x2="-21.59" y2="-15.875" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-15.875" x2="-22.225" y2="-16.51" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-12.065" x2="-22.225" y2="-11.43" width="0.1524" layer="21"/>
<wire x1="-22.225" y1="-16.51" x2="-21.59" y2="-17.145" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-17.145" x2="-21.59" y2="-18.415" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-18.415" x2="-22.225" y2="-19.05" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="-6.35" x2="-26.67" y2="-5.715" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="-8.89" x2="-26.67" y2="-8.255" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="-8.255" x2="-26.67" y2="-6.985" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="-6.985" x2="-26.035" y2="-6.35" width="0.1524" layer="21"/>
<wire x1="-22.225" y1="-3.81" x2="-26.035" y2="-3.81" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="-3.81" x2="-26.67" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="-5.715" x2="-26.67" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="-11.43" x2="-26.67" y2="-10.795" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="-10.795" x2="-26.67" y2="-9.525" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="-9.525" x2="-26.035" y2="-8.89" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="-13.97" x2="-26.67" y2="-13.335" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="-16.51" x2="-26.67" y2="-15.875" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="-15.875" x2="-26.67" y2="-14.605" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="-14.605" x2="-26.035" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="-11.43" x2="-26.67" y2="-12.065" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="-13.335" x2="-26.67" y2="-12.065" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="-19.05" x2="-26.67" y2="-18.415" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="-18.415" x2="-26.67" y2="-17.145" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="-17.145" x2="-26.035" y2="-16.51" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-20.955" x2="-22.225" y2="-21.59" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-19.685" x2="-21.59" y2="-20.955" width="0.1524" layer="21"/>
<wire x1="-22.225" y1="-19.05" x2="-21.59" y2="-19.685" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="-19.685" x2="-26.035" y2="-19.05" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="-20.955" x2="-26.67" y2="-19.685" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="-21.59" x2="-26.67" y2="-20.955" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-22.225" x2="-21.59" y2="-23.495" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-23.495" x2="-22.225" y2="-24.13" width="0.1524" layer="21"/>
<wire x1="-22.225" y1="-24.13" x2="-21.59" y2="-24.765" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-24.765" x2="-21.59" y2="-26.035" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-26.035" x2="-22.225" y2="-26.67" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-22.225" x2="-22.225" y2="-21.59" width="0.1524" layer="21"/>
<wire x1="-22.225" y1="-26.67" x2="-21.59" y2="-27.305" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-27.305" x2="-21.59" y2="-28.575" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-28.575" x2="-22.225" y2="-29.21" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-29.845" x2="-21.59" y2="-31.115" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-31.115" x2="-22.225" y2="-31.75" width="0.1524" layer="21"/>
<wire x1="-22.225" y1="-31.75" x2="-21.59" y2="-32.385" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-32.385" x2="-21.59" y2="-33.655" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-33.655" x2="-22.225" y2="-34.29" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-29.845" x2="-22.225" y2="-29.21" width="0.1524" layer="21"/>
<wire x1="-22.225" y1="-34.29" x2="-21.59" y2="-34.925" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-34.925" x2="-21.59" y2="-36.195" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-36.195" x2="-22.225" y2="-36.83" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="-24.13" x2="-26.67" y2="-23.495" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="-26.67" x2="-26.67" y2="-26.035" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="-26.035" x2="-26.67" y2="-24.765" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="-24.765" x2="-26.035" y2="-24.13" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="-21.59" x2="-26.67" y2="-22.225" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="-23.495" x2="-26.67" y2="-22.225" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="-29.21" x2="-26.67" y2="-28.575" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="-28.575" x2="-26.67" y2="-27.305" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="-27.305" x2="-26.035" y2="-26.67" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="-31.75" x2="-26.67" y2="-31.115" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="-34.29" x2="-26.67" y2="-33.655" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="-33.655" x2="-26.67" y2="-32.385" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="-32.385" x2="-26.035" y2="-31.75" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="-29.21" x2="-26.67" y2="-29.845" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="-31.115" x2="-26.67" y2="-29.845" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="-36.83" x2="-26.67" y2="-36.195" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="-36.195" x2="-26.67" y2="-34.925" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="-34.925" x2="-26.035" y2="-34.29" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-38.735" x2="-22.225" y2="-39.37" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-37.465" x2="-21.59" y2="-38.735" width="0.1524" layer="21"/>
<wire x1="-22.225" y1="-36.83" x2="-21.59" y2="-37.465" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="-37.465" x2="-26.035" y2="-36.83" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="-38.735" x2="-26.67" y2="-37.465" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="-39.37" x2="-26.67" y2="-38.735" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-40.005" x2="-21.59" y2="-41.275" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-41.275" x2="-22.225" y2="-41.91" width="0.1524" layer="21"/>
<wire x1="-21.59" y1="-40.005" x2="-22.225" y2="-39.37" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="-41.91" x2="-26.67" y2="-41.275" width="0.1524" layer="21"/>
<wire x1="-22.225" y1="-41.91" x2="-26.035" y2="-41.91" width="0.1524" layer="21"/>
<wire x1="-26.035" y1="-39.37" x2="-26.67" y2="-40.005" width="0.1524" layer="21"/>
<wire x1="-26.67" y1="-41.275" x2="-26.67" y2="-40.005" width="0.1524" layer="21"/>
<pad name="PA3" x="-25.4" y="-5.08" drill="1" shape="octagon" rot="R270" first="yes"/>
<pad name="PC0_ADC123IN10" x="-25.4" y="-7.62" drill="1" rot="R270"/>
<pad name="PC3_ADC123IN13" x="-25.4" y="-10.16" drill="1" rot="R270"/>
<pad name="PF3_ADC3IN9" x="-25.4" y="-12.7" drill="1" rot="R270"/>
<pad name="PF5_ADC3IN15" x="-25.4" y="-15.24" drill="1" rot="R270"/>
<pad name="PF10" x="-25.4" y="-17.78" drill="1" rot="R270"/>
<pad name="PD7" x="-22.86" y="-5.08" drill="1" rot="R270"/>
<pad name="PD6" x="-22.86" y="-7.62" drill="1" rot="R270"/>
<pad name="PD5" x="-22.86" y="-10.16" drill="1" rot="R270"/>
<pad name="PD4" x="-22.86" y="-12.7" drill="1" rot="R270"/>
<pad name="PD3" x="-22.86" y="-15.24" drill="1" rot="R270"/>
<pad name="GND4" x="-22.86" y="-17.78" drill="1" rot="R270"/>
<pad name="NC2" x="-25.4" y="-20.32" drill="1" rot="R270"/>
<pad name="PE2_SPI4SCK" x="-22.86" y="-20.32" drill="1" rot="R270"/>
<pad name="PA7_RMIIRXDV" x="-25.4" y="-22.86" drill="1" rot="R270"/>
<pad name="PF2" x="-25.4" y="-25.4" drill="1" rot="R270"/>
<pad name="PF1_I2C2SCL" x="-25.4" y="-27.94" drill="1" rot="R270"/>
<pad name="PF0_I2C2SDA" x="-25.4" y="-30.48" drill="1" rot="R270"/>
<pad name="GND3" x="-25.4" y="-33.02" drill="1" rot="R270"/>
<pad name="PD0" x="-25.4" y="-35.56" drill="1" rot="R270"/>
<pad name="PE4" x="-22.86" y="-22.86" drill="1" rot="R270"/>
<pad name="PE5_SPI4MISO" x="-22.86" y="-25.4" drill="1" rot="R270"/>
<pad name="PE6_SPI4MOSI" x="-22.86" y="-27.94" drill="1" rot="R270"/>
<pad name="PE3" x="-22.86" y="-30.48" drill="1" rot="R270"/>
<pad name="PF8_SPI5MISO" x="-22.86" y="-33.02" drill="1" rot="R270"/>
<pad name="PF7_SPI5SCK" x="-22.86" y="-35.56" drill="1" rot="R270"/>
<pad name="PD1" x="-25.4" y="-38.1" drill="1" rot="R270"/>
<pad name="PF9_SPI5MOSI" x="-22.86" y="-38.1" drill="1" rot="R270"/>
<pad name="PG0" x="-25.4" y="-40.64" drill="1" rot="R270"/>
<pad name="PG1" x="-22.86" y="-40.64" drill="1" rot="R270"/>
<wire x1="24.13" y1="27.305" x2="24.13" y2="26.035" width="0.1524" layer="21"/>
<wire x1="24.13" y1="26.035" x2="23.495" y2="25.4" width="0.1524" layer="21"/>
<wire x1="23.495" y1="25.4" x2="24.13" y2="24.765" width="0.1524" layer="21"/>
<wire x1="24.13" y1="24.765" x2="24.13" y2="23.495" width="0.1524" layer="21"/>
<wire x1="24.13" y1="23.495" x2="23.495" y2="22.86" width="0.1524" layer="21"/>
<wire x1="24.13" y1="27.305" x2="23.495" y2="27.94" width="0.1524" layer="21"/>
<wire x1="23.495" y1="22.86" x2="24.13" y2="22.225" width="0.1524" layer="21"/>
<wire x1="24.13" y1="22.225" x2="24.13" y2="20.955" width="0.1524" layer="21"/>
<wire x1="24.13" y1="20.955" x2="23.495" y2="20.32" width="0.1524" layer="21"/>
<wire x1="24.13" y1="19.685" x2="24.13" y2="18.415" width="0.1524" layer="21"/>
<wire x1="24.13" y1="18.415" x2="23.495" y2="17.78" width="0.1524" layer="21"/>
<wire x1="23.495" y1="17.78" x2="24.13" y2="17.145" width="0.1524" layer="21"/>
<wire x1="24.13" y1="17.145" x2="24.13" y2="15.875" width="0.1524" layer="21"/>
<wire x1="24.13" y1="15.875" x2="23.495" y2="15.24" width="0.1524" layer="21"/>
<wire x1="24.13" y1="19.685" x2="23.495" y2="20.32" width="0.1524" layer="21"/>
<wire x1="23.495" y1="15.24" x2="24.13" y2="14.605" width="0.1524" layer="21"/>
<wire x1="24.13" y1="14.605" x2="24.13" y2="13.335" width="0.1524" layer="21"/>
<wire x1="24.13" y1="13.335" x2="23.495" y2="12.7" width="0.1524" layer="21"/>
<wire x1="19.685" y1="25.4" x2="19.05" y2="26.035" width="0.1524" layer="21"/>
<wire x1="19.685" y1="22.86" x2="19.05" y2="23.495" width="0.1524" layer="21"/>
<wire x1="19.05" y1="23.495" x2="19.05" y2="24.765" width="0.1524" layer="21"/>
<wire x1="19.05" y1="24.765" x2="19.685" y2="25.4" width="0.1524" layer="21"/>
<wire x1="23.495" y1="27.94" x2="19.685" y2="27.94" width="0.1524" layer="21"/>
<wire x1="19.685" y1="27.94" x2="19.05" y2="27.305" width="0.1524" layer="21"/>
<wire x1="19.05" y1="26.035" x2="19.05" y2="27.305" width="0.1524" layer="21"/>
<wire x1="19.685" y1="20.32" x2="19.05" y2="20.955" width="0.1524" layer="21"/>
<wire x1="19.05" y1="20.955" x2="19.05" y2="22.225" width="0.1524" layer="21"/>
<wire x1="19.05" y1="22.225" x2="19.685" y2="22.86" width="0.1524" layer="21"/>
<wire x1="19.685" y1="17.78" x2="19.05" y2="18.415" width="0.1524" layer="21"/>
<wire x1="19.685" y1="15.24" x2="19.05" y2="15.875" width="0.1524" layer="21"/>
<wire x1="19.05" y1="15.875" x2="19.05" y2="17.145" width="0.1524" layer="21"/>
<wire x1="19.05" y1="17.145" x2="19.685" y2="17.78" width="0.1524" layer="21"/>
<wire x1="19.685" y1="20.32" x2="19.05" y2="19.685" width="0.1524" layer="21"/>
<wire x1="19.05" y1="18.415" x2="19.05" y2="19.685" width="0.1524" layer="21"/>
<wire x1="19.685" y1="12.7" x2="19.05" y2="13.335" width="0.1524" layer="21"/>
<wire x1="19.05" y1="13.335" x2="19.05" y2="14.605" width="0.1524" layer="21"/>
<wire x1="19.05" y1="14.605" x2="19.685" y2="15.24" width="0.1524" layer="21"/>
<wire x1="24.13" y1="10.795" x2="23.495" y2="10.16" width="0.1524" layer="21"/>
<wire x1="24.13" y1="12.065" x2="24.13" y2="10.795" width="0.1524" layer="21"/>
<wire x1="23.495" y1="12.7" x2="24.13" y2="12.065" width="0.1524" layer="21"/>
<wire x1="19.05" y1="12.065" x2="19.685" y2="12.7" width="0.1524" layer="21"/>
<wire x1="19.05" y1="10.795" x2="19.05" y2="12.065" width="0.1524" layer="21"/>
<wire x1="19.685" y1="10.16" x2="19.05" y2="10.795" width="0.1524" layer="21"/>
<wire x1="24.13" y1="9.525" x2="24.13" y2="8.255" width="0.1524" layer="21"/>
<wire x1="24.13" y1="8.255" x2="23.495" y2="7.62" width="0.1524" layer="21"/>
<wire x1="23.495" y1="7.62" x2="24.13" y2="6.985" width="0.1524" layer="21"/>
<wire x1="24.13" y1="6.985" x2="24.13" y2="5.715" width="0.1524" layer="21"/>
<wire x1="24.13" y1="5.715" x2="23.495" y2="5.08" width="0.1524" layer="21"/>
<wire x1="24.13" y1="9.525" x2="23.495" y2="10.16" width="0.1524" layer="21"/>
<wire x1="23.495" y1="5.08" x2="24.13" y2="4.445" width="0.1524" layer="21"/>
<wire x1="24.13" y1="4.445" x2="24.13" y2="3.175" width="0.1524" layer="21"/>
<wire x1="24.13" y1="3.175" x2="23.495" y2="2.54" width="0.1524" layer="21"/>
<wire x1="19.685" y1="7.62" x2="19.05" y2="8.255" width="0.1524" layer="21"/>
<wire x1="19.685" y1="5.08" x2="19.05" y2="5.715" width="0.1524" layer="21"/>
<wire x1="19.05" y1="5.715" x2="19.05" y2="6.985" width="0.1524" layer="21"/>
<wire x1="19.05" y1="6.985" x2="19.685" y2="7.62" width="0.1524" layer="21"/>
<wire x1="19.685" y1="10.16" x2="19.05" y2="9.525" width="0.1524" layer="21"/>
<wire x1="19.05" y1="8.255" x2="19.05" y2="9.525" width="0.1524" layer="21"/>
<wire x1="19.685" y1="2.54" x2="19.05" y2="3.175" width="0.1524" layer="21"/>
<wire x1="19.05" y1="3.175" x2="19.05" y2="4.445" width="0.1524" layer="21"/>
<wire x1="19.05" y1="4.445" x2="19.685" y2="5.08" width="0.1524" layer="21"/>
<wire x1="23.495" y1="2.54" x2="19.685" y2="2.54" width="0.1524" layer="21"/>
<pad name="PC6" x="20.32" y="26.67" drill="1" shape="octagon" rot="R270" first="yes"/>
<pad name="PB15" x="20.32" y="24.13" drill="1" rot="R270"/>
<pad name="PB13" x="20.32" y="21.59" drill="1" rot="R270"/>
<pad name="PB12" x="20.32" y="19.05" drill="1" rot="R270"/>
<pad name="PA15" x="20.32" y="16.51" drill="1" rot="R270"/>
<pad name="PC7" x="20.32" y="13.97" drill="1" rot="R270"/>
<pad name="PB8_I2C1SCL" x="22.86" y="26.67" drill="1" rot="R270"/>
<pad name="PB9_I2C1SDA" x="22.86" y="24.13" drill="1" rot="R270"/>
<pad name="AVDD" x="22.86" y="21.59" drill="1" rot="R270"/>
<pad name="GND5" x="22.86" y="19.05" drill="1" rot="R270"/>
<pad name="PA5" x="22.86" y="16.51" drill="1" rot="R270"/>
<pad name="PA6" x="22.86" y="13.97" drill="1" rot="R270"/>
<pad name="PB5_SPI6MOSI" x="20.32" y="11.43" drill="1" rot="R270"/>
<pad name="PA7_RMIIRXDV2" x="22.86" y="11.43" drill="1" rot="R270"/>
<pad name="PB3_SPI6SCK" x="20.32" y="8.89" drill="1" rot="R270"/>
<pad name="PA4_SPI6NSS" x="20.32" y="6.35" drill="1" rot="R270"/>
<pad name="PB4_SPI6MISO" x="20.32" y="3.81" drill="1" rot="R270"/>
<pad name="PD14" x="22.86" y="8.89" drill="1" rot="R270"/>
<pad name="PD15" x="22.86" y="6.35" drill="1" rot="R270"/>
<pad name="PF12" x="22.86" y="3.81" drill="1" rot="R270"/>
<wire x1="24.13" y1="0.635" x2="24.13" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-0.635" x2="23.495" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="23.495" y1="-1.27" x2="24.13" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-1.905" x2="24.13" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-3.175" x2="23.495" y2="-3.81" width="0.1524" layer="21"/>
<wire x1="24.13" y1="0.635" x2="23.495" y2="1.27" width="0.1524" layer="21"/>
<wire x1="23.495" y1="-3.81" x2="24.13" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-4.445" x2="24.13" y2="-5.715" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-5.715" x2="23.495" y2="-6.35" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-6.985" x2="24.13" y2="-8.255" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-8.255" x2="23.495" y2="-8.89" width="0.1524" layer="21"/>
<wire x1="23.495" y1="-8.89" x2="24.13" y2="-9.525" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-9.525" x2="24.13" y2="-10.795" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-10.795" x2="23.495" y2="-11.43" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-6.985" x2="23.495" y2="-6.35" width="0.1524" layer="21"/>
<wire x1="23.495" y1="-11.43" x2="24.13" y2="-12.065" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-12.065" x2="24.13" y2="-13.335" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-13.335" x2="23.495" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="19.685" y1="-1.27" x2="19.05" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="19.685" y1="-3.81" x2="19.05" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-3.175" x2="19.05" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-1.905" x2="19.685" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="23.495" y1="1.27" x2="19.685" y2="1.27" width="0.1524" layer="21"/>
<wire x1="19.685" y1="1.27" x2="19.05" y2="0.635" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-0.635" x2="19.05" y2="0.635" width="0.1524" layer="21"/>
<wire x1="19.685" y1="-6.35" x2="19.05" y2="-5.715" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-5.715" x2="19.05" y2="-4.445" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-4.445" x2="19.685" y2="-3.81" width="0.1524" layer="21"/>
<wire x1="19.685" y1="-8.89" x2="19.05" y2="-8.255" width="0.1524" layer="21"/>
<wire x1="19.685" y1="-11.43" x2="19.05" y2="-10.795" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-10.795" x2="19.05" y2="-9.525" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-9.525" x2="19.685" y2="-8.89" width="0.1524" layer="21"/>
<wire x1="19.685" y1="-6.35" x2="19.05" y2="-6.985" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-8.255" x2="19.05" y2="-6.985" width="0.1524" layer="21"/>
<wire x1="19.685" y1="-13.97" x2="19.05" y2="-13.335" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-13.335" x2="19.05" y2="-12.065" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-12.065" x2="19.685" y2="-11.43" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-15.875" x2="23.495" y2="-16.51" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-14.605" x2="24.13" y2="-15.875" width="0.1524" layer="21"/>
<wire x1="23.495" y1="-13.97" x2="24.13" y2="-14.605" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-14.605" x2="19.685" y2="-13.97" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-15.875" x2="19.05" y2="-14.605" width="0.1524" layer="21"/>
<wire x1="19.685" y1="-16.51" x2="19.05" y2="-15.875" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-17.145" x2="24.13" y2="-18.415" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-18.415" x2="23.495" y2="-19.05" width="0.1524" layer="21"/>
<wire x1="23.495" y1="-19.05" x2="24.13" y2="-19.685" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-19.685" x2="24.13" y2="-20.955" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-20.955" x2="23.495" y2="-21.59" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-17.145" x2="23.495" y2="-16.51" width="0.1524" layer="21"/>
<wire x1="23.495" y1="-21.59" x2="24.13" y2="-22.225" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-22.225" x2="24.13" y2="-23.495" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-23.495" x2="23.495" y2="-24.13" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-24.765" x2="24.13" y2="-26.035" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-26.035" x2="23.495" y2="-26.67" width="0.1524" layer="21"/>
<wire x1="23.495" y1="-26.67" x2="24.13" y2="-27.305" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-27.305" x2="24.13" y2="-28.575" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-28.575" x2="23.495" y2="-29.21" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-24.765" x2="23.495" y2="-24.13" width="0.1524" layer="21"/>
<wire x1="23.495" y1="-29.21" x2="24.13" y2="-29.845" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-29.845" x2="24.13" y2="-31.115" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-31.115" x2="23.495" y2="-31.75" width="0.1524" layer="21"/>
<wire x1="19.685" y1="-19.05" x2="19.05" y2="-18.415" width="0.1524" layer="21"/>
<wire x1="19.685" y1="-21.59" x2="19.05" y2="-20.955" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-20.955" x2="19.05" y2="-19.685" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-19.685" x2="19.685" y2="-19.05" width="0.1524" layer="21"/>
<wire x1="19.685" y1="-16.51" x2="19.05" y2="-17.145" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-18.415" x2="19.05" y2="-17.145" width="0.1524" layer="21"/>
<wire x1="19.685" y1="-24.13" x2="19.05" y2="-23.495" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-23.495" x2="19.05" y2="-22.225" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-22.225" x2="19.685" y2="-21.59" width="0.1524" layer="21"/>
<wire x1="19.685" y1="-26.67" x2="19.05" y2="-26.035" width="0.1524" layer="21"/>
<wire x1="19.685" y1="-29.21" x2="19.05" y2="-28.575" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-28.575" x2="19.05" y2="-27.305" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-27.305" x2="19.685" y2="-26.67" width="0.1524" layer="21"/>
<wire x1="19.685" y1="-24.13" x2="19.05" y2="-24.765" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-26.035" x2="19.05" y2="-24.765" width="0.1524" layer="21"/>
<wire x1="19.685" y1="-31.75" x2="19.05" y2="-31.115" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-31.115" x2="19.05" y2="-29.845" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-29.845" x2="19.685" y2="-29.21" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-33.655" x2="23.495" y2="-34.29" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-32.385" x2="24.13" y2="-33.655" width="0.1524" layer="21"/>
<wire x1="23.495" y1="-31.75" x2="24.13" y2="-32.385" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-32.385" x2="19.685" y2="-31.75" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-33.655" x2="19.05" y2="-32.385" width="0.1524" layer="21"/>
<wire x1="19.685" y1="-34.29" x2="19.05" y2="-33.655" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-34.925" x2="24.13" y2="-36.195" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-36.195" x2="23.495" y2="-36.83" width="0.1524" layer="21"/>
<wire x1="23.495" y1="-36.83" x2="24.13" y2="-37.465" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-37.465" x2="24.13" y2="-38.735" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-38.735" x2="23.495" y2="-39.37" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-34.925" x2="23.495" y2="-34.29" width="0.1524" layer="21"/>
<wire x1="23.495" y1="-39.37" x2="24.13" y2="-40.005" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-40.005" x2="24.13" y2="-41.275" width="0.1524" layer="21"/>
<wire x1="24.13" y1="-41.275" x2="23.495" y2="-41.91" width="0.1524" layer="21"/>
<wire x1="19.685" y1="-36.83" x2="19.05" y2="-36.195" width="0.1524" layer="21"/>
<wire x1="19.685" y1="-39.37" x2="19.05" y2="-38.735" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-38.735" x2="19.05" y2="-37.465" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-37.465" x2="19.685" y2="-36.83" width="0.1524" layer="21"/>
<wire x1="19.685" y1="-34.29" x2="19.05" y2="-34.925" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-36.195" x2="19.05" y2="-34.925" width="0.1524" layer="21"/>
<wire x1="19.685" y1="-41.91" x2="19.05" y2="-41.275" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-41.275" x2="19.05" y2="-40.005" width="0.1524" layer="21"/>
<wire x1="19.05" y1="-40.005" x2="19.685" y2="-39.37" width="0.1524" layer="21"/>
<wire x1="23.495" y1="-41.91" x2="19.685" y2="-41.91" width="0.1524" layer="21"/>
<pad name="AVDD2" x="20.32" y="0" drill="1" shape="octagon" rot="R270" first="yes"/>
<pad name="AGND2" x="20.32" y="-2.54" drill="1" rot="R270"/>
<pad name="GND6" x="20.32" y="-5.08" drill="1" rot="R270"/>
<pad name="PB1" x="20.32" y="-7.62" drill="1" rot="R270"/>
<pad name="PC2" x="20.32" y="-10.16" drill="1" rot="R270"/>
<pad name="PF4" x="20.32" y="-12.7" drill="1" rot="R270"/>
<pad name="PF13" x="22.86" y="0" drill="1" rot="R270"/>
<pad name="PE9" x="22.86" y="-2.54" drill="1" rot="R270"/>
<pad name="PE11" x="22.86" y="-5.08" drill="1" rot="R270"/>
<pad name="PF14_I2C4SCL" x="22.86" y="-7.62" drill="1" rot="R270"/>
<pad name="PE13" x="22.86" y="-10.16" drill="1" rot="R270"/>
<pad name="PF15_I2C4SDA" x="22.86" y="-12.7" drill="1" rot="R270"/>
<pad name="PB6_QSPICS" x="20.32" y="-15.24" drill="1" rot="R270"/>
<pad name="PG14_USART6TX" x="22.86" y="-15.24" drill="1" rot="R270"/>
<pad name="PB2_QSPICLK" x="20.32" y="-17.78" drill="1" rot="R270"/>
<pad name="GND7" x="20.32" y="-20.32" drill="1" rot="R270"/>
<pad name="PD13_QSPIIO3" x="20.32" y="-22.86" drill="1" rot="R270"/>
<pad name="PD12_QSPIIO1" x="20.32" y="-25.4" drill="1" rot="R270"/>
<pad name="PD11_QSPIIO0" x="20.32" y="-27.94" drill="1" rot="R270"/>
<pad name="PE2_QSPIIO2" x="20.32" y="-30.48" drill="1" rot="R270"/>
<pad name="PG9_USART6RX" x="22.86" y="-17.78" drill="1" rot="R270"/>
<pad name="PE8_USART7TX" x="22.86" y="-20.32" drill="1" rot="R270"/>
<pad name="PE7_USART7RX" x="22.86" y="-22.86" drill="1" rot="R270"/>
<pad name="GND9" x="22.86" y="-25.4" drill="1" rot="R270"/>
<pad name="PE10" x="22.86" y="-27.94" drill="1" rot="R270"/>
<pad name="PE12" x="22.86" y="-30.48" drill="1" rot="R270"/>
<pad name="GND8" x="20.32" y="-33.02" drill="1" rot="R270"/>
<pad name="PE14" x="22.86" y="-33.02" drill="1" rot="R270"/>
<pad name="PA0" x="20.32" y="-35.56" drill="1" rot="R270"/>
<pad name="PB0_LED1" x="20.32" y="-38.1" drill="1" rot="R270"/>
<pad name="PE0" x="20.32" y="-40.64" drill="1" rot="R270"/>
<pad name="PE15" x="22.86" y="-35.56" drill="1" rot="R270"/>
<pad name="PB10" x="22.86" y="-38.1" drill="1" rot="R270"/>
<pad name="PB11" x="22.86" y="-40.64" drill="1" rot="R270"/>
<wire x1="35.56" y1="-74.93" x2="-35.56" y2="-74.93" width="0.127" layer="21"/>
<wire x1="35.56" y1="-74.93" x2="35.56" y2="58.42" width="0.127" layer="21"/>
<wire x1="-35.56" y1="-74.93" x2="-35.56" y2="58.42" width="0.127" layer="21"/>
<wire x1="35.56" y1="58.42" x2="-35.56" y2="58.42" width="0.127" layer="21"/>
</package>
<package name="CONN6_125-GF-A-0190_GCT">
<smd name="B12" x="0" y="0" dx="0.8128" dy="1.1938" layer="1"/>
<smd name="B9" x="1.229996875" y="0" dx="0.762" dy="1.1938" layer="1"/>
<smd name="A5" x="2.249996875" y="0" dx="0.7112" dy="1.1938" layer="1"/>
<smd name="B5" x="3.24999375" y="0" dx="0.7112" dy="1.1938" layer="1"/>
<smd name="A9" x="4.269990625" y="0" dx="0.762" dy="1.1938" layer="1"/>
<smd name="A12" x="5.4999875" y="0" dx="0.8128" dy="1.1938" layer="1"/>
<pad name="10" x="-1.5748" y="-3.81" drill="0.508" diameter="1"/>
<wire x1="7.0612" y1="0.5588" x2="-1.7272" y2="0.5588" width="0.1524" layer="47"/>
<wire x1="-1.8542" y1="-6.5278" x2="7.3406" y2="-6.5278" width="0.1524" layer="21"/>
<wire x1="7.3406" y1="-6.5278" x2="7.3406" y2="-0.1016" width="0.1524" layer="21"/>
<wire x1="-1.8542" y1="-0.1016" x2="-1.8542" y2="-6.5278" width="0.1524" layer="21"/>
<wire x1="-1.7272" y1="-6.4008" x2="7.2136" y2="-6.4008" width="0.1524" layer="51"/>
<wire x1="7.2136" y1="-6.4008" x2="7.2136" y2="0.5588" width="0.1524" layer="51"/>
<wire x1="7.2136" y1="0.5588" x2="-1.7272" y2="0.5588" width="0.1524" layer="51"/>
<wire x1="-1.7272" y1="0.5588" x2="-1.7272" y2="-6.4008" width="0.1524" layer="51"/>
<text x="-0.5334" y="2.4638" size="1.27" layer="27" ratio="6" rot="SR0">&gt;Name</text>
<pad name="7" x="-1.5748" y="0" drill="0.508" diameter="1"/>
<pad name="8" x="7.0612" y="0" drill="0.508" diameter="1"/>
<pad name="9" x="7.0612" y="-3.81" drill="0.508" diameter="1"/>
</package>
<package name="HEADER-FEMALE-2-2">
<wire x1="-2.54" y1="2.54" x2="-2.54" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-2.54" x2="-0.254" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-0.254" y1="-2.54" x2="0" y2="-2.286" width="0.1524" layer="21"/>
<wire x1="0" y1="2.286" x2="-0.254" y2="2.54" width="0.1524" layer="21"/>
<wire x1="-0.254" y1="2.54" x2="-2.54" y2="2.54" width="0.1524" layer="21"/>
<wire x1="0" y1="-2.286" x2="0.254" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="0.254" y1="2.54" x2="0" y2="2.286" width="0.1524" layer="21"/>
<wire x1="2.54" y1="2.54" x2="2.54" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="0.254" y1="-2.54" x2="2.54" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="2.54" y1="2.54" x2="0.254" y2="2.54" width="0.1524" layer="21"/>
<pad name="1" x="-1.27" y="-1.27" drill="1" shape="octagon" rot="R90" first="yes"/>
<pad name="2" x="-1.27" y="1.27" drill="1" rot="R90"/>
<pad name="3" x="1.27" y="-1.27" drill="1" rot="R90"/>
<pad name="4" x="1.27" y="1.27" drill="1" rot="R90"/>
</package>
</packages>
<symbols>
<symbol name="HEADER-MALE-15-2">
<wire x1="-3.81" y1="22.86" x2="3.81" y2="22.86" width="0.4064" layer="94"/>
<wire x1="-1.27" y1="15.24" x2="-2.54" y2="15.24" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="17.78" x2="-2.54" y2="17.78" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="20.32" x2="-2.54" y2="20.32" width="0.6096" layer="94"/>
<wire x1="2.54" y1="15.24" x2="1.27" y2="15.24" width="0.6096" layer="94"/>
<wire x1="2.54" y1="17.78" x2="1.27" y2="17.78" width="0.6096" layer="94"/>
<wire x1="2.54" y1="20.32" x2="1.27" y2="20.32" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="10.16" x2="-2.54" y2="10.16" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="12.7" x2="-2.54" y2="12.7" width="0.6096" layer="94"/>
<wire x1="2.54" y1="10.16" x2="1.27" y2="10.16" width="0.6096" layer="94"/>
<wire x1="2.54" y1="12.7" x2="1.27" y2="12.7" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="2.54" x2="-2.54" y2="2.54" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="5.08" x2="-2.54" y2="5.08" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="7.62" x2="-2.54" y2="7.62" width="0.6096" layer="94"/>
<wire x1="2.54" y1="2.54" x2="1.27" y2="2.54" width="0.6096" layer="94"/>
<wire x1="2.54" y1="5.08" x2="1.27" y2="5.08" width="0.6096" layer="94"/>
<wire x1="2.54" y1="7.62" x2="1.27" y2="7.62" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-2.54" x2="-2.54" y2="-2.54" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="0" x2="-2.54" y2="0" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-2.54" x2="1.27" y2="-2.54" width="0.6096" layer="94"/>
<wire x1="2.54" y1="0" x2="1.27" y2="0" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-10.16" x2="-2.54" y2="-10.16" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-7.62" x2="-2.54" y2="-7.62" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-5.08" x2="-2.54" y2="-5.08" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-10.16" x2="1.27" y2="-10.16" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-7.62" x2="1.27" y2="-7.62" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-5.08" x2="1.27" y2="-5.08" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-15.24" x2="-2.54" y2="-15.24" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-12.7" x2="-2.54" y2="-12.7" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-15.24" x2="1.27" y2="-15.24" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-12.7" x2="1.27" y2="-12.7" width="0.6096" layer="94"/>
<wire x1="3.81" y1="-17.78" x2="3.81" y2="22.86" width="0.4064" layer="94"/>
<wire x1="-3.81" y1="22.86" x2="-3.81" y2="-17.78" width="0.4064" layer="94"/>
<wire x1="3.81" y1="-17.78" x2="-3.81" y2="-17.78" width="0.4064" layer="94"/>
<pin name="1" x="-7.62" y="20.32" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="3" x="-7.62" y="17.78" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="5" x="-7.62" y="15.24" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="2" x="7.62" y="20.32" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="4" x="7.62" y="17.78" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="6" x="7.62" y="15.24" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="7" x="-7.62" y="12.7" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="9" x="-7.62" y="10.16" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="8" x="7.62" y="12.7" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="10" x="7.62" y="10.16" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="11" x="-7.62" y="7.62" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="13" x="-7.62" y="5.08" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="15" x="-7.62" y="2.54" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="12" x="7.62" y="7.62" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="14" x="7.62" y="5.08" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="16" x="7.62" y="2.54" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="17" x="-7.62" y="0" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="19" x="-7.62" y="-2.54" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="18" x="7.62" y="0" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="20" x="7.62" y="-2.54" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="21" x="-7.62" y="-5.08" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="23" x="-7.62" y="-7.62" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="25" x="-7.62" y="-10.16" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="22" x="7.62" y="-5.08" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="24" x="7.62" y="-7.62" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="26" x="7.62" y="-10.16" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="27" x="-7.62" y="-12.7" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="29" x="-7.62" y="-15.24" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="28" x="7.62" y="-12.7" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="30" x="7.62" y="-15.24" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
</symbol>
<symbol name="HEADER-MALE-10-2">
<wire x1="-3.81" y1="12.7" x2="3.81" y2="12.7" width="0.4064" layer="94"/>
<wire x1="-1.27" y1="5.08" x2="-2.54" y2="5.08" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="7.62" x2="-2.54" y2="7.62" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="10.16" x2="-2.54" y2="10.16" width="0.6096" layer="94"/>
<wire x1="2.54" y1="5.08" x2="1.27" y2="5.08" width="0.6096" layer="94"/>
<wire x1="2.54" y1="7.62" x2="1.27" y2="7.62" width="0.6096" layer="94"/>
<wire x1="2.54" y1="10.16" x2="1.27" y2="10.16" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="0" x2="-2.54" y2="0" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="2.54" x2="-2.54" y2="2.54" width="0.6096" layer="94"/>
<wire x1="2.54" y1="0" x2="1.27" y2="0" width="0.6096" layer="94"/>
<wire x1="2.54" y1="2.54" x2="1.27" y2="2.54" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-7.62" x2="-2.54" y2="-7.62" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-5.08" x2="-2.54" y2="-5.08" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-2.54" x2="-2.54" y2="-2.54" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-7.62" x2="1.27" y2="-7.62" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-5.08" x2="1.27" y2="-5.08" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-2.54" x2="1.27" y2="-2.54" width="0.6096" layer="94"/>
<wire x1="3.81" y1="-15.24" x2="3.81" y2="12.7" width="0.4064" layer="94"/>
<wire x1="-3.81" y1="12.7" x2="-3.81" y2="-15.24" width="0.4064" layer="94"/>
<wire x1="3.81" y1="-15.24" x2="-3.81" y2="-15.24" width="0.4064" layer="94"/>
<pin name="1" x="-7.62" y="10.16" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="3" x="-7.62" y="7.62" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="5" x="-7.62" y="5.08" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="2" x="7.62" y="10.16" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="4" x="7.62" y="7.62" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="6" x="7.62" y="5.08" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="7" x="-7.62" y="2.54" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="9" x="-7.62" y="0" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="8" x="7.62" y="2.54" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="10" x="7.62" y="0" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="11" x="-7.62" y="-2.54" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="13" x="-7.62" y="-5.08" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="15" x="-7.62" y="-7.62" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="12" x="7.62" y="-2.54" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="14" x="7.62" y="-5.08" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="16" x="7.62" y="-7.62" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="17" x="-7.62" y="-10.16" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="18" x="7.62" y="-10.16" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="19" x="-7.62" y="-12.7" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="20" x="7.62" y="-12.7" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<wire x1="2.54" y1="-10.16" x2="1.27" y2="-10.16" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-12.7" x2="1.27" y2="-12.7" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-10.16" x2="-2.54" y2="-10.16" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-12.7" x2="-2.54" y2="-12.7" width="0.6096" layer="94"/>
</symbol>
<symbol name="HEADER-MALE-17-2">
<wire x1="-3.81" y1="25.4" x2="3.81" y2="25.4" width="0.4064" layer="94"/>
<wire x1="-1.27" y1="17.78" x2="-2.54" y2="17.78" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="20.32" x2="-2.54" y2="20.32" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="22.86" x2="-2.54" y2="22.86" width="0.6096" layer="94"/>
<wire x1="2.54" y1="17.78" x2="1.27" y2="17.78" width="0.6096" layer="94"/>
<wire x1="2.54" y1="20.32" x2="1.27" y2="20.32" width="0.6096" layer="94"/>
<wire x1="2.54" y1="22.86" x2="1.27" y2="22.86" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="12.7" x2="-2.54" y2="12.7" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="15.24" x2="-2.54" y2="15.24" width="0.6096" layer="94"/>
<wire x1="2.54" y1="12.7" x2="1.27" y2="12.7" width="0.6096" layer="94"/>
<wire x1="2.54" y1="15.24" x2="1.27" y2="15.24" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="5.08" x2="-2.54" y2="5.08" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="7.62" x2="-2.54" y2="7.62" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="10.16" x2="-2.54" y2="10.16" width="0.6096" layer="94"/>
<wire x1="2.54" y1="5.08" x2="1.27" y2="5.08" width="0.6096" layer="94"/>
<wire x1="2.54" y1="7.62" x2="1.27" y2="7.62" width="0.6096" layer="94"/>
<wire x1="2.54" y1="10.16" x2="1.27" y2="10.16" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="0" x2="-2.54" y2="0" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="2.54" x2="-2.54" y2="2.54" width="0.6096" layer="94"/>
<wire x1="2.54" y1="0" x2="1.27" y2="0" width="0.6096" layer="94"/>
<wire x1="2.54" y1="2.54" x2="1.27" y2="2.54" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-7.62" x2="-2.54" y2="-7.62" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-5.08" x2="-2.54" y2="-5.08" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-2.54" x2="-2.54" y2="-2.54" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-7.62" x2="1.27" y2="-7.62" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-5.08" x2="1.27" y2="-5.08" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-2.54" x2="1.27" y2="-2.54" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-12.7" x2="-2.54" y2="-12.7" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-10.16" x2="-2.54" y2="-10.16" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-12.7" x2="1.27" y2="-12.7" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-10.16" x2="1.27" y2="-10.16" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-17.78" x2="-2.54" y2="-17.78" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-15.24" x2="-2.54" y2="-15.24" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-17.78" x2="1.27" y2="-17.78" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-15.24" x2="1.27" y2="-15.24" width="0.6096" layer="94"/>
<wire x1="3.81" y1="-20.32" x2="3.81" y2="25.4" width="0.4064" layer="94"/>
<wire x1="-3.81" y1="25.4" x2="-3.81" y2="-20.32" width="0.4064" layer="94"/>
<wire x1="3.81" y1="-20.32" x2="-3.81" y2="-20.32" width="0.4064" layer="94"/>
<pin name="1" x="-7.62" y="22.86" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="3" x="-7.62" y="20.32" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="5" x="-7.62" y="17.78" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="2" x="7.62" y="22.86" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="4" x="7.62" y="20.32" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="6" x="7.62" y="17.78" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="7" x="-7.62" y="15.24" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="9" x="-7.62" y="12.7" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="8" x="7.62" y="15.24" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="10" x="7.62" y="12.7" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="11" x="-7.62" y="10.16" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="13" x="-7.62" y="7.62" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="15" x="-7.62" y="5.08" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="12" x="7.62" y="10.16" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="14" x="7.62" y="7.62" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="16" x="7.62" y="5.08" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="17" x="-7.62" y="2.54" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="19" x="-7.62" y="0" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="18" x="7.62" y="2.54" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="20" x="7.62" y="0" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="21" x="-7.62" y="-2.54" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="23" x="-7.62" y="-5.08" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="25" x="-7.62" y="-7.62" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="22" x="7.62" y="-2.54" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="24" x="7.62" y="-5.08" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="26" x="7.62" y="-7.62" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="27" x="-7.62" y="-10.16" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="29" x="-7.62" y="-12.7" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="28" x="7.62" y="-10.16" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="30" x="7.62" y="-12.7" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="31" x="-7.62" y="-15.24" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="33" x="-7.62" y="-17.78" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="32" x="7.62" y="-15.24" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="34" x="7.62" y="-17.78" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
</symbol>
<symbol name="HEADER-MALE-8-2">
<wire x1="-3.81" y1="10.16" x2="3.81" y2="10.16" width="0.4064" layer="94"/>
<wire x1="-1.27" y1="2.54" x2="-2.54" y2="2.54" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="5.08" x2="-2.54" y2="5.08" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="7.62" x2="-2.54" y2="7.62" width="0.6096" layer="94"/>
<wire x1="2.54" y1="2.54" x2="1.27" y2="2.54" width="0.6096" layer="94"/>
<wire x1="2.54" y1="5.08" x2="1.27" y2="5.08" width="0.6096" layer="94"/>
<wire x1="2.54" y1="7.62" x2="1.27" y2="7.62" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-2.54" x2="-2.54" y2="-2.54" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="0" x2="-2.54" y2="0" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-2.54" x2="1.27" y2="-2.54" width="0.6096" layer="94"/>
<wire x1="2.54" y1="0" x2="1.27" y2="0" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-10.16" x2="-2.54" y2="-10.16" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-7.62" x2="-2.54" y2="-7.62" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-5.08" x2="-2.54" y2="-5.08" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-10.16" x2="1.27" y2="-10.16" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-7.62" x2="1.27" y2="-7.62" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-5.08" x2="1.27" y2="-5.08" width="0.6096" layer="94"/>
<wire x1="3.81" y1="-12.7" x2="3.81" y2="10.16" width="0.4064" layer="94"/>
<wire x1="-3.81" y1="10.16" x2="-3.81" y2="-12.7" width="0.4064" layer="94"/>
<wire x1="3.81" y1="-12.7" x2="-3.81" y2="-12.7" width="0.4064" layer="94"/>
<pin name="1" x="-7.62" y="7.62" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="3" x="-7.62" y="5.08" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="5" x="-7.62" y="2.54" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="2" x="7.62" y="7.62" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="4" x="7.62" y="5.08" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="6" x="7.62" y="2.54" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="7" x="-7.62" y="0" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="9" x="-7.62" y="-2.54" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="8" x="7.62" y="0" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="10" x="7.62" y="-2.54" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="11" x="-7.62" y="-5.08" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="13" x="-7.62" y="-7.62" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="15" x="-7.62" y="-10.16" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="12" x="7.62" y="-5.08" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="14" x="7.62" y="-7.62" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="16" x="7.62" y="-10.16" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
</symbol>
<symbol name="CONN6_125-GF-A-0190">
<pin name="B12" x="2.54" y="0" length="middle" direction="pas"/>
<pin name="B9" x="2.54" y="-2.54" length="middle" direction="pas"/>
<pin name="A5" x="2.54" y="-5.08" length="middle" direction="pas"/>
<pin name="B5" x="2.54" y="-7.62" length="middle" direction="pas"/>
<pin name="A9" x="2.54" y="-10.16" length="middle" direction="pas"/>
<pin name="A12" x="2.54" y="-12.7" length="middle" direction="pas"/>
<pin name="8" x="2.54" y="-17.78" length="middle" direction="pas"/>
<pin name="7" x="2.54" y="-15.24" length="middle" direction="pas"/>
<pin name="9" x="2.54" y="-20.32" length="middle" direction="pas"/>
<pin name="10" x="2.54" y="-22.86" length="middle" direction="pas"/>
<wire x1="7.62" y1="5.08" x2="7.62" y2="-15.24" width="0.1524" layer="94"/>
<wire x1="7.62" y1="-15.24" x2="7.62" y2="-17.78" width="0.1524" layer="94"/>
<wire x1="7.62" y1="-17.78" x2="7.62" y2="-20.32" width="0.1524" layer="94"/>
<wire x1="7.62" y1="-20.32" x2="7.62" y2="-22.86" width="0.1524" layer="94"/>
<wire x1="7.62" y1="-22.86" x2="7.62" y2="-27.94" width="0.1524" layer="94"/>
<wire x1="7.62" y1="-27.94" x2="33.02" y2="-27.94" width="0.1524" layer="94"/>
<wire x1="33.02" y1="-27.94" x2="33.02" y2="5.08" width="0.1524" layer="94"/>
<wire x1="33.02" y1="5.08" x2="7.62" y2="5.08" width="0.1524" layer="94"/>
<text x="15.5956" y="9.1186" size="2.0828" layer="95" ratio="6" rot="SR0">&gt;Name</text>
<text x="14.9606" y="6.5786" size="2.0828" layer="96" ratio="6" rot="SR0">&gt;Value</text>
<wire x1="7.62" y1="-15.24" x2="12.7" y2="-15.24" width="0.254" layer="94"/>
<wire x1="12.7" y1="-15.24" x2="12.7" y2="-17.78" width="0.254" layer="94"/>
<wire x1="12.7" y1="-17.78" x2="12.7" y2="-20.32" width="0.254" layer="94"/>
<wire x1="12.7" y1="-20.32" x2="12.7" y2="-22.86" width="0.254" layer="94"/>
<wire x1="12.7" y1="-22.86" x2="12.7" y2="-25.4" width="0.254" layer="94"/>
<wire x1="12.7" y1="-25.4" x2="10.16" y2="-25.4" width="0.254" layer="94"/>
<wire x1="15.24" y1="-25.4" x2="12.7" y2="-25.4" width="0.254" layer="94"/>
<wire x1="7.62" y1="-22.86" x2="12.7" y2="-22.86" width="0.254" layer="94"/>
<wire x1="7.62" y1="-20.32" x2="12.7" y2="-20.32" width="0.254" layer="94"/>
<wire x1="7.62" y1="-17.78" x2="12.7" y2="-17.78" width="0.254" layer="94"/>
</symbol>
<symbol name="HEADER-FEMALE-2-2">
<wire x1="3.81" y1="-5.08" x2="-3.81" y2="-5.08" width="0.4064" layer="94"/>
<wire x1="-1.905" y1="-0.635" x2="-1.905" y2="0.635" width="0.254" layer="94" curve="-180" cap="flat"/>
<wire x1="-1.905" y1="-3.175" x2="-1.905" y2="-1.905" width="0.254" layer="94" curve="-180" cap="flat"/>
<wire x1="1.905" y1="0.635" x2="1.905" y2="-0.635" width="0.254" layer="94" curve="-180" cap="flat"/>
<wire x1="1.905" y1="-1.905" x2="1.905" y2="-3.175" width="0.254" layer="94" curve="-180" cap="flat"/>
<wire x1="-3.81" y1="2.54" x2="-3.81" y2="-5.08" width="0.4064" layer="94"/>
<wire x1="3.81" y1="-5.08" x2="3.81" y2="2.54" width="0.4064" layer="94"/>
<wire x1="-3.81" y1="2.54" x2="3.81" y2="2.54" width="0.4064" layer="94"/>
<pin name="1" x="-7.62" y="-2.54" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="3" x="-7.62" y="0" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="2" x="7.62" y="-2.54" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="4" x="7.62" y="0" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="NUCLEO-767">
<gates>
<gate name="G$2" symbol="HEADER-MALE-15-2" x="-38.1" y="-20.32" addlevel="always"/>
<gate name="G$3" symbol="HEADER-MALE-10-2" x="0" y="17.78" addlevel="always"/>
<gate name="G$4" symbol="HEADER-MALE-17-2" x="0" y="-22.86" addlevel="always"/>
<gate name="G$1" symbol="HEADER-MALE-8-2" x="-38.1" y="20.32" addlevel="always"/>
</gates>
<devices>
<device name="" package="NUCLEO-767">
<connects>
<connect gate="G$1" pin="1" pad="NC1"/>
<connect gate="G$1" pin="10" pad="PC12"/>
<connect gate="G$1" pin="11" pad="GND1"/>
<connect gate="G$1" pin="12" pad="PD2"/>
<connect gate="G$1" pin="13" pad="GND2"/>
<connect gate="G$1" pin="14" pad="PG2"/>
<connect gate="G$1" pin="15" pad="VIN"/>
<connect gate="G$1" pin="16" pad="PG3"/>
<connect gate="G$1" pin="2" pad="PC8"/>
<connect gate="G$1" pin="3" pad="IOREF"/>
<connect gate="G$1" pin="4" pad="PC9"/>
<connect gate="G$1" pin="5" pad="RESET"/>
<connect gate="G$1" pin="6" pad="PC10"/>
<connect gate="G$1" pin="7" pad="+3V3"/>
<connect gate="G$1" pin="8" pad="PC11"/>
<connect gate="G$1" pin="9" pad="+5V"/>
<connect gate="G$2" pin="1" pad="PA3"/>
<connect gate="G$2" pin="10" pad="PD3"/>
<connect gate="G$2" pin="11" pad="PF10"/>
<connect gate="G$2" pin="12" pad="GND4"/>
<connect gate="G$2" pin="13" pad="NC2"/>
<connect gate="G$2" pin="14" pad="PE2_SPI4SCK"/>
<connect gate="G$2" pin="15" pad="PA7_RMIIRXDV"/>
<connect gate="G$2" pin="16" pad="PE4"/>
<connect gate="G$2" pin="17" pad="PF2"/>
<connect gate="G$2" pin="18" pad="PE5_SPI4MISO"/>
<connect gate="G$2" pin="19" pad="PF1_I2C2SCL"/>
<connect gate="G$2" pin="2" pad="PD7"/>
<connect gate="G$2" pin="20" pad="PE6_SPI4MOSI"/>
<connect gate="G$2" pin="21" pad="PF0_I2C2SDA"/>
<connect gate="G$2" pin="22" pad="PE3"/>
<connect gate="G$2" pin="23" pad="GND3"/>
<connect gate="G$2" pin="24" pad="PF8_SPI5MISO"/>
<connect gate="G$2" pin="25" pad="PD0"/>
<connect gate="G$2" pin="26" pad="PF7_SPI5SCK"/>
<connect gate="G$2" pin="27" pad="PD1"/>
<connect gate="G$2" pin="28" pad="PF9_SPI5MOSI"/>
<connect gate="G$2" pin="29" pad="PG0"/>
<connect gate="G$2" pin="3" pad="PC0_ADC123IN10"/>
<connect gate="G$2" pin="30" pad="PG1"/>
<connect gate="G$2" pin="4" pad="PD6"/>
<connect gate="G$2" pin="5" pad="PC3_ADC123IN13"/>
<connect gate="G$2" pin="6" pad="PD5"/>
<connect gate="G$2" pin="7" pad="PF3_ADC3IN9"/>
<connect gate="G$2" pin="8" pad="PD4"/>
<connect gate="G$2" pin="9" pad="PF5_ADC3IN15"/>
<connect gate="G$3" pin="1" pad="PC6"/>
<connect gate="G$3" pin="10" pad="PA5"/>
<connect gate="G$3" pin="11" pad="PC7"/>
<connect gate="G$3" pin="12" pad="PA6"/>
<connect gate="G$3" pin="13" pad="PB5_SPI6MOSI"/>
<connect gate="G$3" pin="14" pad="PA7_RMIIRXDV2"/>
<connect gate="G$3" pin="15" pad="PB3_SPI6SCK"/>
<connect gate="G$3" pin="16" pad="PD14"/>
<connect gate="G$3" pin="17" pad="PA4_SPI6NSS"/>
<connect gate="G$3" pin="18" pad="PD15"/>
<connect gate="G$3" pin="19" pad="PB4_SPI6MISO"/>
<connect gate="G$3" pin="2" pad="PB8_I2C1SCL"/>
<connect gate="G$3" pin="20" pad="PF12"/>
<connect gate="G$3" pin="3" pad="PB15"/>
<connect gate="G$3" pin="4" pad="PB9_I2C1SDA"/>
<connect gate="G$3" pin="5" pad="PB13"/>
<connect gate="G$3" pin="6" pad="AVDD"/>
<connect gate="G$3" pin="7" pad="PB12"/>
<connect gate="G$3" pin="8" pad="GND5"/>
<connect gate="G$3" pin="9" pad="PA15"/>
<connect gate="G$4" pin="1" pad="AVDD2"/>
<connect gate="G$4" pin="10" pad="PE13"/>
<connect gate="G$4" pin="11" pad="PF4"/>
<connect gate="G$4" pin="12" pad="PF15_I2C4SDA"/>
<connect gate="G$4" pin="13" pad="PB6_QSPICS"/>
<connect gate="G$4" pin="14" pad="PG14_USART6TX"/>
<connect gate="G$4" pin="15" pad="PB2_QSPICLK"/>
<connect gate="G$4" pin="16" pad="PG9_USART6RX"/>
<connect gate="G$4" pin="17" pad="GND7"/>
<connect gate="G$4" pin="18" pad="PE8_USART7TX"/>
<connect gate="G$4" pin="19" pad="PD13_QSPIIO3"/>
<connect gate="G$4" pin="2" pad="PF13"/>
<connect gate="G$4" pin="20" pad="PE7_USART7RX"/>
<connect gate="G$4" pin="21" pad="PD12_QSPIIO1"/>
<connect gate="G$4" pin="22" pad="GND8"/>
<connect gate="G$4" pin="23" pad="PD11_QSPIIO0"/>
<connect gate="G$4" pin="24" pad="PE10"/>
<connect gate="G$4" pin="25" pad="PE2_QSPIIO2"/>
<connect gate="G$4" pin="26" pad="PE12"/>
<connect gate="G$4" pin="27" pad="GND9"/>
<connect gate="G$4" pin="28" pad="PE14"/>
<connect gate="G$4" pin="29" pad="PA0"/>
<connect gate="G$4" pin="3" pad="AGND2"/>
<connect gate="G$4" pin="30" pad="PE15"/>
<connect gate="G$4" pin="31" pad="PB0_LED1"/>
<connect gate="G$4" pin="32" pad="PB10"/>
<connect gate="G$4" pin="33" pad="PE0"/>
<connect gate="G$4" pin="34" pad="PB11"/>
<connect gate="G$4" pin="4" pad="PE9"/>
<connect gate="G$4" pin="5" pad="GND6"/>
<connect gate="G$4" pin="6" pad="PE11"/>
<connect gate="G$4" pin="7" pad="PB1"/>
<connect gate="G$4" pin="8" pad="PF14_I2C4SCL"/>
<connect gate="G$4" pin="9" pad="PC2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="USB4125-GF-A-0190" prefix="J">
<gates>
<gate name="A" symbol="CONN6_125-GF-A-0190" x="0" y="0"/>
</gates>
<devices>
<device name="" package="CONN6_125-GF-A-0190_GCT">
<connects>
<connect gate="A" pin="10" pad="10"/>
<connect gate="A" pin="7" pad="7"/>
<connect gate="A" pin="8" pad="8"/>
<connect gate="A" pin="9" pad="9"/>
<connect gate="A" pin="A12" pad="A12"/>
<connect gate="A" pin="A5" pad="A5"/>
<connect gate="A" pin="A9" pad="A9"/>
<connect gate="A" pin="B12" pad="B12"/>
<connect gate="A" pin="B5" pad="B5"/>
<connect gate="A" pin="B9" pad="B9"/>
</connects>
<technologies>
<technology name="">
<attribute name="COPYRIGHT" value="Copyright (C) 2023 Ultra Librarian. All rights reserved." constant="no"/>
<attribute name="MANUFACTURER_PART_NUMBER" value="USB4125-GF-A-0190" constant="no"/>
<attribute name="MFR_NAME" value="Global Connector Technology" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="HEADER-FEMALE-2-2">
<gates>
<gate name="G$1" symbol="HEADER-FEMALE-2-2" x="0" y="0"/>
</gates>
<devices>
<device name="" package="HEADER-FEMALE-2-2">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="diode">
<packages>
<package name="DO35-3">
<wire x1="2.54" y1="0" x2="2.032" y2="0" width="0.6096" layer="21"/>
<wire x1="-5.08" y1="0" x2="-4.572" y2="0" width="0.6096" layer="21"/>
<wire x1="-2.54" y1="1.27" x2="-2.54" y2="-1.27" width="0.254" layer="21"/>
<wire x1="-2.54" y1="-1.27" x2="0" y2="0" width="0.254" layer="21"/>
<wire x1="0" y1="0" x2="-2.54" y2="1.27" width="0.254" layer="21"/>
<wire x1="0" y1="1.27" x2="0" y2="-1.27" width="0.254" layer="21"/>
<pad name="A" x="-5.08" y="0" drill="0.8"/>
<pad name="C" x="2.54" y="0" drill="0.8"/>
<text x="-3.048" y="1.524" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-3.1496" y="-2.794" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="0" y1="-0.3048" x2="2.0066" y2="0.3048" layer="21"/>
<rectangle x1="-4.5466" y1="-0.3048" x2="-2.54" y2="0.3048" layer="21"/>
</package>
<package name="DO35-1">
<wire x1="-1.27" y1="1.27" x2="-1.27" y2="0" width="0.254" layer="21"/>
<wire x1="-1.27" y1="0" x2="-1.27" y2="-1.27" width="0.254" layer="21"/>
<wire x1="-1.27" y1="-1.27" x2="1.27" y2="0" width="0.254" layer="21"/>
<wire x1="1.27" y1="0" x2="-1.27" y2="1.27" width="0.254" layer="21"/>
<wire x1="1.27" y1="1.27" x2="1.27" y2="0" width="0.254" layer="21"/>
<wire x1="1.27" y1="0" x2="1.27" y2="-1.27" width="0.254" layer="21"/>
<wire x1="1.27" y1="0" x2="2.54" y2="0" width="0.127" layer="21"/>
<wire x1="-1.27" y1="0" x2="-2.54" y2="0" width="0.127" layer="21"/>
<pad name="A" x="-1.27" y="0" drill="0.8"/>
<pad name="C" x="1.27" y="0" drill="0.8"/>
<text x="-1.778" y="1.524" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-1.8796" y="-2.794" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="DO35-2">
<wire x1="2.54" y1="0" x2="2.032" y2="0" width="0.6096" layer="21"/>
<wire x1="-2.54" y1="0" x2="-2.032" y2="0" width="0.6096" layer="21"/>
<wire x1="-1.27" y1="1.27" x2="-1.27" y2="-1.27" width="0.254" layer="21"/>
<wire x1="-1.27" y1="-1.27" x2="1.27" y2="0" width="0.254" layer="21"/>
<wire x1="1.27" y1="0" x2="-1.27" y2="1.27" width="0.254" layer="21"/>
<wire x1="1.27" y1="1.27" x2="1.27" y2="-1.27" width="0.254" layer="21"/>
<pad name="A" x="-2.54" y="0" drill="0.8"/>
<pad name="C" x="2.54" y="0" drill="0.8"/>
<text x="-1.778" y="1.524" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-1.8796" y="-2.794" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="1.27" y1="-0.3048" x2="2.032" y2="0.3048" layer="21"/>
<rectangle x1="-2.032" y1="-0.3048" x2="-1.27" y2="0.3048" layer="21"/>
</package>
<package name="MINIMELF">
<description>&lt;b&gt;Mini Melf Diode&lt;/b&gt;</description>
<wire x1="1.3208" y1="0.7874" x2="-1.3208" y2="0.7874" width="0.1524" layer="51"/>
<wire x1="1.3208" y1="-0.7874" x2="-1.3208" y2="-0.7874" width="0.1524" layer="51"/>
<wire x1="0.5" y1="0.5" x2="-0.5" y2="0" width="0.2032" layer="21"/>
<wire x1="-0.5" y1="0" x2="0.5" y2="-0.5" width="0.2032" layer="21"/>
<wire x1="0.5" y1="-0.5" x2="0.5" y2="0.5" width="0.2032" layer="21"/>
<smd name="C" x="-1.7" y="0" dx="1.4" dy="1.8" layer="1"/>
<smd name="A" x="1.7" y="0" dx="1.4" dy="1.8" layer="1"/>
<text x="-1.651" y="1.143" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.651" y="-2.413" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-1.8542" y1="-0.8636" x2="-1.2954" y2="0.8636" layer="51"/>
<rectangle x1="1.2954" y1="-0.8636" x2="1.8542" y2="0.8636" layer="51"/>
<rectangle x1="-0.8636" y1="-0.7874" x2="-0.254" y2="0.7874" layer="21"/>
</package>
</packages>
<symbols>
<symbol name="DIODE">
<wire x1="-1.27" y1="-1.27" x2="1.27" y2="0" width="0.254" layer="94"/>
<wire x1="1.27" y1="0" x2="-1.27" y2="1.27" width="0.254" layer="94"/>
<wire x1="1.27" y1="1.27" x2="1.27" y2="0" width="0.254" layer="94"/>
<wire x1="-1.27" y1="1.27" x2="-1.27" y2="0" width="0.254" layer="94"/>
<wire x1="-1.27" y1="0" x2="-1.27" y2="-1.27" width="0.254" layer="94"/>
<wire x1="1.27" y1="0" x2="1.27" y2="-1.27" width="0.254" layer="94"/>
<wire x1="-2.54" y1="0" x2="-1.27" y2="0" width="0.1524" layer="94"/>
<wire x1="1.27" y1="0" x2="2.54" y2="0" width="0.1524" layer="94"/>
<text x="-3.81" y="1.7526" size="1.778" layer="95">&gt;NAME</text>
<text x="-3.81" y="-3.81" size="1.778" layer="96">&gt;VALUE</text>
<pin name="A" x="-2.54" y="0" visible="off" length="point" direction="pas"/>
<pin name="C" x="2.54" y="0" visible="off" length="point" direction="pas" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="1N4148" prefix="D">
<gates>
<gate name="G$1" symbol="DIODE" x="0" y="0"/>
</gates>
<devices>
<device name="D3" package="DO35-3">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="D1" package="DO35-1">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="D2" package="DO35-2">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="" package="MINIMELF">
<connects>
<connect gate="G$1" pin="A" pad="A"/>
<connect gate="G$1" pin="C" pad="C"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="passive">
<description>1uF  5.3 4.1&lt;br&gt;
4.7uF 5.3&lt;br&gt;
10uF 5.3 6.5&lt;br&gt;
22uF 5.3&lt;br&gt;
47uF 6.5&lt;br&gt;
100uF 6.5 8.2&lt;br&gt;
220uF 10.3&lt;br&gt;
330uF 8.3&lt;br&gt;
470uF 8.3&lt;br&gt;
680uF 10.2&lt;br&gt;
1000uF 10.2&lt;br&gt;
2200uF 16.2&lt;br&gt;</description>
<packages>
<package name="R1V">
<wire x1="-1.27" y1="0" x2="1.016" y2="0" width="0.6096" layer="51"/>
<circle x="-1.27" y="0" radius="1.27" width="0.1016" layer="21"/>
<circle x="-1.27" y="0" radius="1.016" width="0.1524" layer="51"/>
<pad name="1" x="-1.27" y="0" drill="0.6" first="yes"/>
<pad name="2" x="1.27" y="0" drill="0.6"/>
<text x="0.127" y="0.889" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="0.127" y="-2.159" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="R2V">
<wire x1="-2.54" y1="0" x2="2.286" y2="0" width="0.6096" layer="51"/>
<circle x="-2.54" y="0" radius="1.27" width="0.1016" layer="21"/>
<circle x="-2.54" y="0" radius="1.016" width="0.1524" layer="51"/>
<pad name="1" x="-2.54" y="0" drill="0.6" first="yes"/>
<pad name="2" x="2.54" y="0" drill="0.6"/>
<text x="-1.143" y="0.889" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-1.143" y="-2.159" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="R3V">
<wire x1="-2.54" y1="0" x2="4.826" y2="0" width="0.6096" layer="51"/>
<circle x="-2.54" y="0" radius="1.27" width="0.1016" layer="21"/>
<circle x="-2.54" y="0" radius="1.016" width="0.1524" layer="51"/>
<pad name="1" x="-2.54" y="0" drill="0.6" first="yes"/>
<pad name="2" x="5.08" y="0" drill="0.6"/>
<text x="-1.143" y="0.889" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-1.143" y="-2.159" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="R4">
<wire x1="5.08" y1="0" x2="4.064" y2="0" width="0.6096" layer="51"/>
<wire x1="-5.08" y1="0" x2="-4.064" y2="0" width="0.6096" layer="51"/>
<wire x1="-3.175" y1="0.889" x2="-2.921" y2="1.143" width="0.1524" layer="21" curve="-90"/>
<wire x1="-3.175" y1="-0.889" x2="-2.921" y2="-1.143" width="0.1524" layer="21" curve="90"/>
<wire x1="2.921" y1="-1.143" x2="3.175" y2="-0.889" width="0.1524" layer="21" curve="90"/>
<wire x1="2.921" y1="1.143" x2="3.175" y2="0.889" width="0.1524" layer="21" curve="-90"/>
<wire x1="-3.175" y1="-0.889" x2="-3.175" y2="0.889" width="0.1524" layer="21"/>
<wire x1="-2.921" y1="1.143" x2="-2.54" y2="1.143" width="0.1524" layer="21"/>
<wire x1="-2.413" y1="1.016" x2="-2.54" y2="1.143" width="0.1524" layer="21"/>
<wire x1="-2.921" y1="-1.143" x2="-2.54" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="-2.413" y1="-1.016" x2="-2.54" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="2.413" y1="1.016" x2="2.54" y2="1.143" width="0.1524" layer="21"/>
<wire x1="2.413" y1="1.016" x2="-2.413" y2="1.016" width="0.1524" layer="21"/>
<wire x1="2.413" y1="-1.016" x2="2.54" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="2.413" y1="-1.016" x2="-2.413" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="2.921" y1="1.143" x2="2.54" y2="1.143" width="0.1524" layer="21"/>
<wire x1="2.921" y1="-1.143" x2="2.54" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="3.175" y1="-0.889" x2="3.175" y2="0.889" width="0.1524" layer="21"/>
<pad name="1" x="-5.08" y="0" drill="0.6" first="yes"/>
<pad name="2" x="5.08" y="0" drill="0.6"/>
<text x="-3.048" y="1.524" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.2606" y="-0.635" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="3.175" y1="-0.3048" x2="4.0386" y2="0.3048" layer="21"/>
<rectangle x1="-4.0386" y1="-0.3048" x2="-3.175" y2="0.3048" layer="21"/>
</package>
<package name="R1.5V">
<wire x1="-1.27" y1="0" x2="2.286" y2="0" width="0.6096" layer="51"/>
<circle x="-1.27" y="0" radius="1.27" width="0.1016" layer="21"/>
<circle x="-1.27" y="0" radius="1.016" width="0.1524" layer="51"/>
<pad name="1" x="-1.27" y="0" drill="0.6" first="yes"/>
<pad name="2" x="2.54" y="0" drill="0.6"/>
<text x="0.127" y="0.889" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="0.127" y="-2.159" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
</package>
<package name="0805">
<wire x1="-0.3175" y1="0.5588" x2="0.9525" y2="0.5588" width="0.1524" layer="21"/>
<wire x1="-0.3175" y1="-0.5588" x2="0.9525" y2="-0.5588" width="0.1524" layer="21"/>
<wire x1="-1.4288" y1="0.8064" x2="2.0638" y2="0.8064" width="0.0508" layer="39"/>
<wire x1="2.0638" y1="0.8064" x2="2.0638" y2="-0.8064" width="0.0508" layer="39"/>
<wire x1="2.0638" y1="-0.8064" x2="-1.4288" y2="-0.8064" width="0.0508" layer="39"/>
<wire x1="-1.4288" y1="-0.8064" x2="-1.4288" y2="0.8064" width="0.0508" layer="39"/>
<smd name="1" x="-0.635" y="0" dx="0.889" dy="1.3208" layer="1"/>
<smd name="2" x="1.27" y="0" dx="0.889" dy="1.3208" layer="1"/>
<text x="-1.397" y="1.016" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.397" y="-2.286" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="0.9525" y1="-0.635" x2="1.3335" y2="0.635" layer="51" rot="R180"/>
<rectangle x1="-0.6985" y1="-0.635" x2="-0.3175" y2="0.635" layer="51"/>
</package>
<package name="R1206">
<wire x1="0.9525" y1="-0.8128" x2="-0.9652" y2="-0.8128" width="0.1524" layer="51"/>
<wire x1="0.9525" y1="0.8128" x2="-0.9652" y2="0.8128" width="0.1524" layer="51"/>
<wire x1="-2.473" y1="0.983" x2="2.473" y2="0.983" width="0.0508" layer="39"/>
<wire x1="2.473" y1="0.983" x2="2.473" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="2.473" y1="-0.983" x2="-2.473" y2="-0.983" width="0.0508" layer="39"/>
<wire x1="-2.473" y1="-0.983" x2="-2.473" y2="0.983" width="0.0508" layer="39"/>
<smd name="2" x="1.422" y="0" dx="1.6" dy="1.803" layer="1"/>
<smd name="1" x="-1.422" y="0" dx="1.6" dy="1.803" layer="1"/>
<text x="-1.397" y="1.143" size="1.27" layer="25">&gt;NAME</text>
<text x="-1.397" y="-2.413" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-1.6891" y1="-0.8763" x2="-0.9525" y2="0.8763" layer="51"/>
<rectangle x1="0.9525" y1="-0.8763" x2="1.6891" y2="0.8763" layer="51"/>
<rectangle x1="-0.3" y1="-0.7" x2="0.3" y2="0.7" layer="35"/>
</package>
<package name="R6">
<wire x1="7.62" y1="0" x2="6.604" y2="0" width="0.6096" layer="51"/>
<wire x1="-7.62" y1="0" x2="-6.604" y2="0" width="0.6096" layer="51"/>
<wire x1="-4.445" y1="1.524" x2="-4.191" y2="1.778" width="0.1524" layer="21" curve="-90"/>
<wire x1="-4.445" y1="-1.524" x2="-4.191" y2="-1.778" width="0.1524" layer="21" curve="90"/>
<wire x1="4.191" y1="-1.778" x2="4.445" y2="-1.524" width="0.1524" layer="21" curve="90"/>
<wire x1="4.191" y1="1.778" x2="4.445" y2="1.524" width="0.1524" layer="21" curve="-90"/>
<wire x1="-4.445" y1="-1.524" x2="-4.445" y2="1.524" width="0.1524" layer="21"/>
<wire x1="-4.191" y1="1.778" x2="-3.81" y2="1.778" width="0.1524" layer="21"/>
<wire x1="-3.683" y1="1.651" x2="-3.81" y2="1.778" width="0.1524" layer="21"/>
<wire x1="-4.191" y1="-1.778" x2="-3.81" y2="-1.778" width="0.1524" layer="21"/>
<wire x1="-3.683" y1="-1.651" x2="-3.81" y2="-1.778" width="0.1524" layer="21"/>
<wire x1="3.683" y1="1.651" x2="3.81" y2="1.778" width="0.1524" layer="21"/>
<wire x1="3.683" y1="1.651" x2="-3.683" y2="1.651" width="0.1524" layer="21"/>
<wire x1="3.683" y1="-1.651" x2="3.81" y2="-1.778" width="0.1524" layer="21"/>
<wire x1="3.683" y1="-1.651" x2="-3.683" y2="-1.651" width="0.1524" layer="21"/>
<wire x1="4.191" y1="1.778" x2="3.81" y2="1.778" width="0.1524" layer="21"/>
<wire x1="4.191" y1="-1.778" x2="3.81" y2="-1.778" width="0.1524" layer="21"/>
<wire x1="4.445" y1="-1.524" x2="4.445" y2="1.524" width="0.1524" layer="21"/>
<pad name="1" x="-7.62" y="0" drill="1" first="yes"/>
<pad name="2" x="7.62" y="0" drill="1"/>
<text x="-4.318" y="2.159" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-3.5306" y="-0.635" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="4.445" y1="-0.3048" x2="6.5786" y2="0.3048" layer="21"/>
<rectangle x1="-6.5786" y1="-0.3048" x2="-4.445" y2="0.3048" layer="21"/>
</package>
<package name="R7">
<wire x1="10.16" y1="0" x2="9.144" y2="0" width="0.6096" layer="51"/>
<wire x1="-7.62" y1="0" x2="-6.604" y2="0" width="0.6096" layer="51"/>
<wire x1="-4.445" y1="1.524" x2="-4.191" y2="1.778" width="0.1524" layer="21" curve="-90"/>
<wire x1="-4.445" y1="-1.524" x2="-4.191" y2="-1.778" width="0.1524" layer="21" curve="90"/>
<wire x1="6.731" y1="-1.778" x2="6.985" y2="-1.524" width="0.1524" layer="21" curve="90"/>
<wire x1="6.731" y1="1.778" x2="6.985" y2="1.524" width="0.1524" layer="21" curve="-90"/>
<wire x1="-4.445" y1="-1.524" x2="-4.445" y2="1.524" width="0.1524" layer="21"/>
<wire x1="-4.191" y1="1.778" x2="-3.81" y2="1.778" width="0.1524" layer="21"/>
<wire x1="-3.683" y1="1.651" x2="-3.81" y2="1.778" width="0.1524" layer="21"/>
<wire x1="-4.191" y1="-1.778" x2="-3.81" y2="-1.778" width="0.1524" layer="21"/>
<wire x1="-3.683" y1="-1.651" x2="-3.81" y2="-1.778" width="0.1524" layer="21"/>
<wire x1="6.223" y1="1.651" x2="6.35" y2="1.778" width="0.1524" layer="21"/>
<wire x1="6.223" y1="1.651" x2="-3.683" y2="1.651" width="0.1524" layer="21"/>
<wire x1="6.223" y1="-1.651" x2="6.35" y2="-1.778" width="0.1524" layer="21"/>
<wire x1="6.223" y1="-1.651" x2="-3.683" y2="-1.651" width="0.1524" layer="21"/>
<wire x1="6.731" y1="1.778" x2="6.35" y2="1.778" width="0.1524" layer="21"/>
<wire x1="6.731" y1="-1.778" x2="6.35" y2="-1.778" width="0.1524" layer="21"/>
<wire x1="6.985" y1="-1.524" x2="6.985" y2="1.524" width="0.1524" layer="21"/>
<pad name="1" x="-7.62" y="0" drill="1" first="yes"/>
<pad name="2" x="10.16" y="0" drill="1"/>
<text x="-4.318" y="2.159" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-3.5306" y="-0.635" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="6.985" y1="-0.3048" x2="9.1186" y2="0.3048" layer="21"/>
<rectangle x1="-6.5786" y1="-0.3048" x2="-4.445" y2="0.3048" layer="21"/>
</package>
<package name="ARRAY0603X4">
<wire x1="1.55" y1="0.75" x2="1.55" y2="-0.75" width="0.1016" layer="51"/>
<wire x1="-1.55" y1="-0.75" x2="-1.55" y2="0.75" width="0.1016" layer="51"/>
<wire x1="-1.55" y1="-0.15" x2="-1.55" y2="0.15" width="0.1016" layer="21"/>
<wire x1="1.55" y1="0.15" x2="1.55" y2="-0.15" width="0.1016" layer="21"/>
<wire x1="-1.55" y1="0.75" x2="-1.025" y2="0.75" width="0.1016" layer="51"/>
<wire x1="-0.575" y1="0.75" x2="-0.225" y2="0.75" width="0.1016" layer="51"/>
<wire x1="0.225" y1="0.75" x2="0.575" y2="0.75" width="0.1016" layer="51"/>
<wire x1="1.025" y1="0.75" x2="1.55" y2="0.75" width="0.1016" layer="51"/>
<wire x1="0.575" y1="0.75" x2="0.575" y2="0.45" width="0.1016" layer="51"/>
<wire x1="0.575" y1="0.45" x2="1.025" y2="0.45" width="0.1016" layer="51"/>
<wire x1="1.025" y1="0.45" x2="1.025" y2="0.75" width="0.1016" layer="51"/>
<wire x1="-1.025" y1="0.75" x2="-1.025" y2="0.45" width="0.1016" layer="51"/>
<wire x1="-1.025" y1="0.45" x2="-0.575" y2="0.45" width="0.1016" layer="51"/>
<wire x1="-0.575" y1="0.45" x2="-0.575" y2="0.75" width="0.1016" layer="51"/>
<wire x1="-0.225" y1="0.75" x2="-0.225" y2="0.45" width="0.1016" layer="51"/>
<wire x1="-0.225" y1="0.45" x2="0.225" y2="0.45" width="0.1016" layer="51"/>
<wire x1="0.225" y1="0.45" x2="0.225" y2="0.75" width="0.1016" layer="51"/>
<wire x1="1.55" y1="-0.75" x2="1.025" y2="-0.75" width="0.1016" layer="51"/>
<wire x1="0.575" y1="-0.75" x2="0.225" y2="-0.75" width="0.1016" layer="51"/>
<wire x1="-0.225" y1="-0.75" x2="-0.575" y2="-0.75" width="0.1016" layer="51"/>
<wire x1="-1.025" y1="-0.75" x2="-1.55" y2="-0.75" width="0.1016" layer="51"/>
<wire x1="-0.575" y1="-0.75" x2="-0.575" y2="-0.45" width="0.1016" layer="51"/>
<wire x1="-0.575" y1="-0.45" x2="-1.025" y2="-0.45" width="0.1016" layer="51"/>
<wire x1="-1.025" y1="-0.45" x2="-1.025" y2="-0.75" width="0.1016" layer="51"/>
<wire x1="1.025" y1="-0.75" x2="1.025" y2="-0.45" width="0.1016" layer="51"/>
<wire x1="1.025" y1="-0.45" x2="0.575" y2="-0.45" width="0.1016" layer="51"/>
<wire x1="0.575" y1="-0.45" x2="0.575" y2="-0.75" width="0.1016" layer="51"/>
<wire x1="0.225" y1="-0.75" x2="0.225" y2="-0.45" width="0.1016" layer="51"/>
<wire x1="0.225" y1="-0.45" x2="-0.225" y2="-0.45" width="0.1016" layer="51"/>
<wire x1="-0.225" y1="-0.45" x2="-0.225" y2="-0.75" width="0.1016" layer="51"/>
<smd name="1" x="-1.27" y="-0.75" dx="0.6" dy="0.8" layer="1"/>
<smd name="2" x="-0.4" y="-0.75" dx="0.5" dy="0.8" layer="1"/>
<smd name="3" x="0.4" y="-0.75" dx="0.5" dy="0.8" layer="1"/>
<smd name="4" x="1.27" y="-0.75" dx="0.6" dy="0.8" layer="1"/>
<smd name="5" x="1.27" y="0.75" dx="0.6" dy="0.8" layer="1" rot="R180"/>
<smd name="6" x="0.4" y="0.75" dx="0.5" dy="0.8" layer="1" rot="R180"/>
<smd name="7" x="-0.4" y="0.75" dx="0.5" dy="0.8" layer="1" rot="R180"/>
<smd name="8" x="-1.27" y="0.75" dx="0.6" dy="0.8" layer="1" rot="R180"/>
<text x="-1.9725" y="1.27" size="1.27" layer="25" font="vector">&gt;NAME</text>
<text x="-1.9725" y="-2.54" size="1.27" layer="27" font="vector">&gt;VALUE</text>
<rectangle x1="-1.74625" y1="-0.15875" x2="1.74625" y2="0.15875" layer="41"/>
</package>
</packages>
<symbols>
<symbol name="RESISTOR">
<wire x1="-2.54" y1="-0.889" x2="2.54" y2="-0.889" width="0.254" layer="94"/>
<wire x1="2.54" y1="0.889" x2="-2.54" y2="0.889" width="0.254" layer="94"/>
<wire x1="2.54" y1="-0.889" x2="2.54" y2="0.889" width="0.254" layer="94"/>
<wire x1="-2.54" y1="-0.889" x2="-2.54" y2="0.889" width="0.254" layer="94"/>
<text x="-3.81" y="1.4986" size="1.778" layer="95">&gt;NAME</text>
<text x="-3.81" y="-3.302" size="1.778" layer="96">&gt;VALUE</text>
<pin name="2" x="5.08" y="0" visible="off" length="short" direction="pas" swaplevel="1" rot="R180"/>
<pin name="1" x="-5.08" y="0" visible="off" length="short" direction="pas" swaplevel="1"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="RESISTOR" prefix="R" uservalue="yes">
<gates>
<gate name="G$1" symbol="RESISTOR" x="0" y="0"/>
</gates>
<devices>
<device name="1V" package="R1V">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="2V" package="R2V">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="3V" package="R3V">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="4" package="R4">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="1.5V" package="R1.5V">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="R0805" package="0805">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="R1206" package="R1206">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="6" package="R6">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="7" package="R7">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="ARRAY-4RESISTOR" prefix="R" uservalue="yes">
<gates>
<gate name="A" symbol="RESISTOR" x="0" y="10.16" swaplevel="1"/>
<gate name="B" symbol="RESISTOR" x="0" y="2.54" swaplevel="1"/>
<gate name="C" symbol="RESISTOR" x="0" y="-5.08" swaplevel="1"/>
<gate name="D" symbol="RESISTOR" x="0" y="-12.7" swaplevel="1"/>
</gates>
<devices>
<device name="" package="ARRAY0603X4">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="8"/>
<connect gate="B" pin="1" pad="2"/>
<connect gate="B" pin="2" pad="7"/>
<connect gate="C" pin="1" pad="3"/>
<connect gate="C" pin="2" pad="6"/>
<connect gate="D" pin="1" pad="4"/>
<connect gate="D" pin="2" pad="5"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="supply">
<packages>
</packages>
<symbols>
<symbol name="+3.3V">
<wire x1="1.27" y1="0.635" x2="0" y2="2.54" width="0.254" layer="94"/>
<wire x1="0" y1="2.54" x2="-1.27" y2="0.635" width="0.254" layer="94"/>
<text x="0" y="2.54" size="1.778" layer="97">+3.3V</text>
<pin name="+3.3V" x="0" y="0" visible="off" length="short" direction="sup" rot="R90"/>
</symbol>
<symbol name="0V">
<wire x1="-1.905" y1="-2.54" x2="1.905" y2="-2.54" width="0.254" layer="94"/>
<pin name="0V" x="0" y="0" visible="off" length="short" direction="sup" rot="R270"/>
</symbol>
<symbol name="+5V">
<wire x1="1.27" y1="0.635" x2="0" y2="2.54" width="0.254" layer="94"/>
<wire x1="0" y1="2.54" x2="-1.27" y2="0.635" width="0.254" layer="94"/>
<text x="0" y="2.54" size="1.778" layer="97">+5V</text>
<pin name="+5V" x="0" y="0" visible="off" length="short" direction="sup" rot="R90"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="+3.3V" prefix="P">
<gates>
<gate name="G$1" symbol="+3.3V" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="0V" prefix="P">
<gates>
<gate name="G$1" symbol="0V" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="+5V" prefix="P">
<gates>
<gate name="G$1" symbol="+5V" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="U$1" library="header" deviceset="NUCLEO-767" device=""/>
<part name="U$2" library="header" deviceset="NUCLEO-767" device=""/>
<part name="D1" library="diode" deviceset="1N4148" device=""/>
<part name="D2" library="diode" deviceset="1N4148" device=""/>
<part name="R1" library="passive" deviceset="RESISTOR" device="R0805" value="3k3"/>
<part name="P1" library="supply" deviceset="+3.3V" device=""/>
<part name="P2" library="supply" deviceset="0V" device=""/>
<part name="R2" library="passive" deviceset="RESISTOR" device="R0805" value="2k2"/>
<part name="R3" library="passive" deviceset="RESISTOR" device="R0805" value="2k2"/>
<part name="P3" library="supply" deviceset="+3.3V" device=""/>
<part name="P4" library="supply" deviceset="0V" device=""/>
<part name="R4" library="passive" deviceset="ARRAY-4RESISTOR" device="" value="0"/>
<part name="R5" library="passive" deviceset="ARRAY-4RESISTOR" device="" value="0"/>
<part name="J1" library="header" deviceset="USB4125-GF-A-0190" device=""/>
<part name="P5" library="supply" deviceset="0V" device=""/>
<part name="P6" library="supply" deviceset="+3.3V" device=""/>
<part name="P7" library="supply" deviceset="+5V" device=""/>
<part name="R6" library="passive" deviceset="RESISTOR" device="R0805" value="5k1"/>
<part name="R7" library="passive" deviceset="RESISTOR" device="R0805" value="5k1"/>
<part name="P8" library="supply" deviceset="0V" device=""/>
<part name="P9" library="supply" deviceset="0V" device=""/>
<part name="U$4" library="header" deviceset="HEADER-FEMALE-2-2" device=""/>
<part name="U$5" library="header" deviceset="HEADER-FEMALE-2-2" device=""/>
<part name="P10" library="supply" deviceset="+5V" device=""/>
<part name="P11" library="supply" deviceset="+5V" device=""/>
</parts>
<sheets>
<sheet>
<plain>
<text x="43.18" y="119.38" size="1.778" layer="97" rot="R90">ECHO from Postmaster</text>
</plain>
<instances>
<instance part="U$1" gate="G$2" x="27.94" y="45.72"/>
<instance part="U$1" gate="G$3" x="66.04" y="83.82"/>
<instance part="U$1" gate="G$4" x="66.04" y="43.18"/>
<instance part="U$1" gate="G$1" x="27.94" y="86.36"/>
<instance part="U$2" gate="G$2" x="180.34" y="45.72"/>
<instance part="U$2" gate="G$3" x="218.44" y="83.82"/>
<instance part="U$2" gate="G$4" x="218.44" y="43.18"/>
<instance part="U$2" gate="G$1" x="180.34" y="86.36"/>
<instance part="D1" gate="G$1" x="167.64" y="33.02"/>
<instance part="D2" gate="G$1" x="15.24" y="33.02"/>
<instance part="R1" gate="G$1" x="116.84" y="20.32" rot="R90"/>
<instance part="P1" gate="G$1" x="116.84" y="27.94"/>
<instance part="P2" gate="G$1" x="17.78" y="73.66"/>
<instance part="R2" gate="G$1" x="121.92" y="114.3" rot="R90"/>
<instance part="R3" gate="G$1" x="132.08" y="114.3" rot="R90"/>
<instance part="P3" gate="G$1" x="127" y="124.46"/>
<instance part="P4" gate="G$1" x="170.18" y="76.2"/>
<instance part="R4" gate="A" x="236.22" y="83.82"/>
<instance part="R4" gate="B" x="236.22" y="81.28"/>
<instance part="R4" gate="C" x="236.22" y="78.74"/>
<instance part="R4" gate="D" x="236.22" y="76.2"/>
<instance part="R5" gate="A" x="205.74" y="78.74"/>
<instance part="R5" gate="B" x="205.74" y="76.2"/>
<instance part="R5" gate="C" x="205.74" y="73.66"/>
<instance part="R5" gate="D" x="205.74" y="71.12"/>
<instance part="J1" gate="A" x="-7.62" y="144.78"/>
<instance part="P5" gate="G$1" x="-7.62" y="119.38"/>
<instance part="P6" gate="G$1" x="7.62" y="86.36"/>
<instance part="P7" gate="G$1" x="-15.24" y="147.32"/>
<instance part="R6" gate="G$1" x="-22.86" y="127" rot="R90"/>
<instance part="R7" gate="G$1" x="-33.02" y="127" rot="R90"/>
<instance part="P8" gate="G$1" x="-33.02" y="119.38"/>
<instance part="P9" gate="G$1" x="-22.86" y="119.38"/>
<instance part="U$4" gate="G$1" x="137.16" y="81.28"/>
<instance part="U$5" gate="G$1" x="-12.7" y="81.28"/>
<instance part="P10" gate="G$1" x="-2.54" y="83.82"/>
<instance part="P11" gate="G$1" x="147.32" y="83.82"/>
</instances>
<busses>
<bus name="SPI_CS,SPI_CLK,SPI_MISO,SPI_MOSI">
<segment>
<wire x1="259.08" y1="78.74" x2="259.08" y2="139.7" width="0.762" layer="92"/>
<wire x1="259.08" y1="139.7" x2="195.58" y2="139.7" width="0.762" layer="92"/>
<wire x1="195.58" y1="139.7" x2="50.8" y2="139.7" width="0.762" layer="92"/>
<wire x1="50.8" y1="139.7" x2="50.8" y2="73.66" width="0.762" layer="92"/>
<wire x1="195.58" y1="139.7" x2="195.58" y2="73.66" width="0.762" layer="92"/>
</segment>
</bus>
<bus name="UART_TX,UART_RX,UART_RTR,UART_CTS">
<segment>
<wire x1="259.08" y1="53.34" x2="259.08" y2="0" width="0.762" layer="92"/>
<wire x1="259.08" y1="0" x2="198.12" y2="0" width="0.762" layer="92"/>
<wire x1="198.12" y1="0" x2="45.72" y2="0" width="0.762" layer="92"/>
<wire x1="45.72" y1="0" x2="45.72" y2="66.04" width="0.762" layer="92"/>
<wire x1="198.12" y1="0" x2="198.12" y2="66.04" width="0.762" layer="92"/>
</segment>
</bus>
<bus name="ECHO_TX,ECHO_RX">
<segment>
<wire x1="48.26" y1="83.82" x2="48.26" y2="149.86" width="0.762" layer="92"/>
<wire x1="48.26" y1="149.86" x2="193.04" y2="149.86" width="0.762" layer="92"/>
<wire x1="193.04" y1="149.86" x2="193.04" y2="83.82" width="0.762" layer="92"/>
</segment>
</bus>
<bus name="GPIO_IN,GPIO_OUT">
<segment>
<wire x1="40.64" y1="40.64" x2="40.64" y2="-5.08" width="0.762" layer="92"/>
<wire x1="40.64" y1="-5.08" x2="195.58" y2="-5.08" width="0.762" layer="92"/>
<wire x1="195.58" y1="-5.08" x2="195.58" y2="40.64" width="0.762" layer="92"/>
</segment>
</bus>
</busses>
<nets>
<net name="N$1" class="0">
<segment>
<wire x1="10.16" y1="12.7" x2="116.84" y2="12.7" width="0.1524" layer="91"/>
<wire x1="116.84" y1="12.7" x2="162.56" y2="12.7" width="0.1524" layer="91"/>
<wire x1="162.56" y1="12.7" x2="162.56" y2="33.02" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$2" pin="25"/>
<wire x1="162.56" y1="33.02" x2="162.56" y2="35.56" width="0.1524" layer="91"/>
<wire x1="162.56" y1="35.56" x2="172.72" y2="35.56" width="0.1524" layer="91"/>
<pinref part="D1" gate="G$1" pin="A"/>
<wire x1="162.56" y1="33.02" x2="165.1" y2="33.02" width="0.1524" layer="91"/>
<junction x="162.56" y="33.02"/>
<pinref part="U$1" gate="G$2" pin="25"/>
<wire x1="20.32" y1="35.56" x2="10.16" y2="35.56" width="0.1524" layer="91"/>
<wire x1="10.16" y1="35.56" x2="10.16" y2="33.02" width="0.1524" layer="91"/>
<pinref part="D2" gate="G$1" pin="A"/>
<wire x1="10.16" y1="33.02" x2="10.16" y2="12.7" width="0.1524" layer="91"/>
<wire x1="12.7" y1="33.02" x2="10.16" y2="33.02" width="0.1524" layer="91"/>
<junction x="10.16" y="33.02"/>
<pinref part="R1" gate="G$1" pin="1"/>
<wire x1="116.84" y1="15.24" x2="116.84" y2="12.7" width="0.1524" layer="91"/>
<junction x="116.84" y="12.7"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<pinref part="U$1" gate="G$2" pin="27"/>
<pinref part="D2" gate="G$1" pin="C"/>
<wire x1="17.78" y1="33.02" x2="20.32" y2="33.02" width="0.1524" layer="91"/>
</segment>
</net>
<net name="+3.3V" class="0">
<segment>
<pinref part="P1" gate="G$1" pin="+3.3V"/>
<pinref part="R1" gate="G$1" pin="2"/>
<wire x1="116.84" y1="27.94" x2="116.84" y2="25.4" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="R2" gate="G$1" pin="2"/>
<wire x1="121.92" y1="119.38" x2="121.92" y2="121.92" width="0.1524" layer="91"/>
<wire x1="121.92" y1="121.92" x2="127" y2="121.92" width="0.1524" layer="91"/>
<pinref part="R3" gate="G$1" pin="2"/>
<wire x1="127" y1="121.92" x2="132.08" y2="121.92" width="0.1524" layer="91"/>
<wire x1="132.08" y1="121.92" x2="132.08" y2="119.38" width="0.1524" layer="91"/>
<pinref part="P3" gate="G$1" pin="+3.3V"/>
<wire x1="127" y1="121.92" x2="127" y2="124.46" width="0.1524" layer="91"/>
<junction x="127" y="121.92"/>
</segment>
<segment>
<pinref part="P6" gate="G$1" pin="+3.3V"/>
<pinref part="U$1" gate="G$1" pin="7"/>
<wire x1="7.62" y1="86.36" x2="20.32" y2="86.36" width="0.1524" layer="91"/>
</segment>
</net>
<net name="0V" class="0">
<segment>
<pinref part="P2" gate="G$1" pin="0V"/>
<wire x1="17.78" y1="81.28" x2="17.78" y2="78.74" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="11"/>
<wire x1="17.78" y1="78.74" x2="17.78" y2="73.66" width="0.1524" layer="91"/>
<wire x1="17.78" y1="81.28" x2="20.32" y2="81.28" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="13"/>
<wire x1="17.78" y1="78.74" x2="20.32" y2="78.74" width="0.1524" layer="91"/>
<junction x="17.78" y="78.74"/>
</segment>
<segment>
<pinref part="U$2" gate="G$1" pin="11"/>
<wire x1="172.72" y1="81.28" x2="170.18" y2="81.28" width="0.1524" layer="91"/>
<wire x1="170.18" y1="81.28" x2="170.18" y2="78.74" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$1" pin="13"/>
<wire x1="170.18" y1="78.74" x2="172.72" y2="78.74" width="0.1524" layer="91"/>
<wire x1="170.18" y1="78.74" x2="170.18" y2="76.2" width="0.1524" layer="91"/>
<junction x="170.18" y="78.74"/>
<pinref part="P4" gate="G$1" pin="0V"/>
</segment>
<segment>
<pinref part="J1" gate="A" pin="7"/>
<wire x1="-5.08" y1="129.54" x2="-7.62" y2="129.54" width="0.1524" layer="91"/>
<wire x1="-7.62" y1="129.54" x2="-7.62" y2="127" width="0.1524" layer="91"/>
<pinref part="J1" gate="A" pin="10"/>
<wire x1="-7.62" y1="127" x2="-7.62" y2="124.46" width="0.1524" layer="91"/>
<wire x1="-7.62" y1="124.46" x2="-7.62" y2="121.92" width="0.1524" layer="91"/>
<wire x1="-7.62" y1="121.92" x2="-7.62" y2="119.38" width="0.1524" layer="91"/>
<wire x1="-5.08" y1="121.92" x2="-7.62" y2="121.92" width="0.1524" layer="91"/>
<junction x="-7.62" y="121.92"/>
<pinref part="J1" gate="A" pin="9"/>
<wire x1="-5.08" y1="124.46" x2="-7.62" y2="124.46" width="0.1524" layer="91"/>
<junction x="-7.62" y="124.46"/>
<pinref part="J1" gate="A" pin="8"/>
<wire x1="-5.08" y1="127" x2="-7.62" y2="127" width="0.1524" layer="91"/>
<junction x="-7.62" y="127"/>
<pinref part="P5" gate="G$1" pin="0V"/>
<pinref part="J1" gate="A" pin="A12"/>
<wire x1="-5.08" y1="132.08" x2="-7.62" y2="132.08" width="0.1524" layer="91"/>
<wire x1="-7.62" y1="132.08" x2="-7.62" y2="129.54" width="0.1524" layer="91"/>
<junction x="-7.62" y="129.54"/>
<pinref part="J1" gate="A" pin="B12"/>
<wire x1="-5.08" y1="144.78" x2="-7.62" y2="144.78" width="0.1524" layer="91"/>
<wire x1="-7.62" y1="144.78" x2="-7.62" y2="132.08" width="0.1524" layer="91"/>
<junction x="-7.62" y="132.08"/>
<wire x1="-33.02" y1="121.92" x2="-33.02" y2="119.38" width="0.1524" layer="91"/>
<pinref part="P8" gate="G$1" pin="0V"/>
<pinref part="R7" gate="G$1" pin="1"/>
</segment>
<segment>
<pinref part="P9" gate="G$1" pin="0V"/>
<pinref part="R6" gate="G$1" pin="1"/>
<wire x1="-22.86" y1="119.38" x2="-22.86" y2="121.92" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$4" class="0">
<segment>
<pinref part="U$2" gate="G$3" pin="2"/>
<wire x1="226.06" y1="93.98" x2="241.3" y2="93.98" width="0.1524" layer="91"/>
<wire x1="241.3" y1="93.98" x2="241.3" y2="106.68" width="0.1524" layer="91"/>
<wire x1="241.3" y1="106.68" x2="121.92" y2="106.68" width="0.1524" layer="91"/>
<wire x1="121.92" y1="106.68" x2="93.98" y2="106.68" width="0.1524" layer="91"/>
<wire x1="93.98" y1="106.68" x2="93.98" y2="93.98" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$3" pin="2"/>
<wire x1="93.98" y1="93.98" x2="73.66" y2="93.98" width="0.1524" layer="91"/>
<pinref part="R2" gate="G$1" pin="1"/>
<wire x1="121.92" y1="109.22" x2="121.92" y2="106.68" width="0.1524" layer="91"/>
<junction x="121.92" y="106.68"/>
</segment>
</net>
<net name="N$5" class="0">
<segment>
<pinref part="U$1" gate="G$3" pin="4"/>
<wire x1="73.66" y1="91.44" x2="96.52" y2="91.44" width="0.1524" layer="91"/>
<wire x1="96.52" y1="91.44" x2="96.52" y2="104.14" width="0.1524" layer="91"/>
<wire x1="96.52" y1="104.14" x2="132.08" y2="104.14" width="0.1524" layer="91"/>
<wire x1="132.08" y1="104.14" x2="243.84" y2="104.14" width="0.1524" layer="91"/>
<wire x1="243.84" y1="104.14" x2="243.84" y2="91.44" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$3" pin="4"/>
<wire x1="243.84" y1="91.44" x2="226.06" y2="91.44" width="0.1524" layer="91"/>
<pinref part="R3" gate="G$1" pin="1"/>
<wire x1="132.08" y1="109.22" x2="132.08" y2="104.14" width="0.1524" layer="91"/>
<junction x="132.08" y="104.14"/>
</segment>
</net>
<net name="SPI_CS" class="0">
<segment>
<wire x1="241.3" y1="76.2" x2="256.54" y2="76.2" width="0.1524" layer="91"/>
<wire x1="259.08" y1="78.74" x2="256.54" y2="76.2" width="0.1524" layer="91"/>
<label x="243.84" y="76.2" size="1.778" layer="95"/>
<pinref part="R4" gate="D" pin="2"/>
</segment>
<segment>
<wire x1="50.8" y1="76.2" x2="53.34" y2="73.66" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$3" pin="17"/>
<wire x1="53.34" y1="73.66" x2="58.42" y2="73.66" width="0.1524" layer="91"/>
</segment>
<segment>
<wire x1="195.58" y1="76.2" x2="198.12" y2="73.66" width="0.1524" layer="91"/>
<pinref part="R5" gate="C" pin="1"/>
<wire x1="198.12" y1="73.66" x2="200.66" y2="73.66" width="0.1524" layer="91"/>
</segment>
</net>
<net name="SPI_MOSI" class="0">
<segment>
<wire x1="259.08" y1="81.28" x2="256.54" y2="78.74" width="0.1524" layer="91"/>
<wire x1="256.54" y1="78.74" x2="241.3" y2="78.74" width="0.1524" layer="91"/>
<label x="243.84" y="78.74" size="1.778" layer="95"/>
<pinref part="R4" gate="C" pin="2"/>
</segment>
<segment>
<wire x1="50.8" y1="81.28" x2="53.34" y2="78.74" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$3" pin="13"/>
<wire x1="53.34" y1="78.74" x2="58.42" y2="78.74" width="0.1524" layer="91"/>
</segment>
<segment>
<wire x1="195.58" y1="81.28" x2="198.12" y2="78.74" width="0.1524" layer="91"/>
<pinref part="R5" gate="A" pin="1"/>
<wire x1="198.12" y1="78.74" x2="200.66" y2="78.74" width="0.1524" layer="91"/>
</segment>
</net>
<net name="SPI_MISO" class="0">
<segment>
<wire x1="259.08" y1="83.82" x2="256.54" y2="81.28" width="0.1524" layer="91"/>
<wire x1="256.54" y1="81.28" x2="241.3" y2="81.28" width="0.1524" layer="91"/>
<label x="243.84" y="81.28" size="1.778" layer="95"/>
<pinref part="R4" gate="B" pin="2"/>
</segment>
<segment>
<wire x1="50.8" y1="73.66" x2="53.34" y2="71.12" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$3" pin="19"/>
<wire x1="53.34" y1="71.12" x2="58.42" y2="71.12" width="0.1524" layer="91"/>
</segment>
<segment>
<wire x1="195.58" y1="73.66" x2="198.12" y2="71.12" width="0.1524" layer="91"/>
<pinref part="R5" gate="D" pin="1"/>
<wire x1="198.12" y1="71.12" x2="200.66" y2="71.12" width="0.1524" layer="91"/>
</segment>
</net>
<net name="SPI_CLK" class="0">
<segment>
<wire x1="259.08" y1="86.36" x2="256.54" y2="83.82" width="0.1524" layer="91"/>
<wire x1="256.54" y1="83.82" x2="241.3" y2="83.82" width="0.1524" layer="91"/>
<label x="243.84" y="83.82" size="1.778" layer="95"/>
<pinref part="R4" gate="A" pin="2"/>
</segment>
<segment>
<wire x1="50.8" y1="78.74" x2="53.34" y2="76.2" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$3" pin="15"/>
<wire x1="53.34" y1="76.2" x2="58.42" y2="76.2" width="0.1524" layer="91"/>
</segment>
<segment>
<wire x1="195.58" y1="78.74" x2="198.12" y2="76.2" width="0.1524" layer="91"/>
<pinref part="R5" gate="B" pin="1"/>
<wire x1="198.12" y1="76.2" x2="200.66" y2="76.2" width="0.1524" layer="91"/>
</segment>
</net>
<net name="UART_TX" class="0">
<segment>
<wire x1="259.08" y1="53.34" x2="256.54" y2="50.8" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$4" pin="14"/>
<wire x1="256.54" y1="50.8" x2="226.06" y2="50.8" width="0.1524" layer="91"/>
</segment>
<segment>
<wire x1="45.72" y1="66.04" x2="43.18" y2="63.5" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$2" pin="4"/>
<wire x1="43.18" y1="63.5" x2="35.56" y2="63.5" width="0.1524" layer="91"/>
</segment>
<segment>
<wire x1="198.12" y1="63.5" x2="195.58" y2="60.96" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$2" pin="6"/>
<wire x1="195.58" y1="60.96" x2="187.96" y2="60.96" width="0.1524" layer="91"/>
</segment>
</net>
<net name="UART_RX" class="0">
<segment>
<wire x1="259.08" y1="50.8" x2="256.54" y2="48.26" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$4" pin="16"/>
<wire x1="256.54" y1="48.26" x2="226.06" y2="48.26" width="0.1524" layer="91"/>
</segment>
<segment>
<wire x1="45.72" y1="63.5" x2="43.18" y2="60.96" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$2" pin="6"/>
<wire x1="43.18" y1="60.96" x2="35.56" y2="60.96" width="0.1524" layer="91"/>
</segment>
<segment>
<wire x1="198.12" y1="66.04" x2="195.58" y2="63.5" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$2" pin="4"/>
<wire x1="195.58" y1="63.5" x2="187.96" y2="63.5" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$6" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="6"/>
<wire x1="35.56" y1="88.9" x2="40.64" y2="88.9" width="0.1524" layer="91"/>
<wire x1="40.64" y1="88.9" x2="40.64" y2="116.84" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$7" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="8"/>
<wire x1="35.56" y1="86.36" x2="43.18" y2="86.36" width="0.1524" layer="91"/>
<wire x1="43.18" y1="86.36" x2="43.18" y2="116.84" width="0.1524" layer="91"/>
</segment>
</net>
<net name="UART_CTS" class="0">
<segment>
<wire x1="45.72" y1="60.96" x2="43.18" y2="58.42" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$2" pin="8"/>
<wire x1="43.18" y1="58.42" x2="35.56" y2="58.42" width="0.1524" layer="91"/>
</segment>
<segment>
<wire x1="198.12" y1="58.42" x2="195.58" y2="55.88" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$2" pin="10"/>
<wire x1="195.58" y1="55.88" x2="187.96" y2="55.88" width="0.1524" layer="91"/>
</segment>
</net>
<net name="UART_RTR" class="0">
<segment>
<wire x1="45.72" y1="58.42" x2="43.18" y2="55.88" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$2" pin="10"/>
<wire x1="43.18" y1="55.88" x2="35.56" y2="55.88" width="0.1524" layer="91"/>
</segment>
<segment>
<wire x1="198.12" y1="60.96" x2="195.58" y2="58.42" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$2" pin="8"/>
<wire x1="195.58" y1="58.42" x2="187.96" y2="58.42" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$2" class="0">
<segment>
<pinref part="D1" gate="G$1" pin="C"/>
<pinref part="U$2" gate="G$2" pin="27"/>
<wire x1="170.18" y1="33.02" x2="172.72" y2="33.02" width="0.1524" layer="91"/>
</segment>
</net>
<net name="ECHO_RX" class="0">
<segment>
<wire x1="48.26" y1="86.36" x2="45.72" y2="83.82" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="10"/>
<wire x1="45.72" y1="83.82" x2="35.56" y2="83.82" width="0.1524" layer="91"/>
</segment>
<segment>
<wire x1="193.04" y1="83.82" x2="190.5" y2="81.28" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$1" pin="12"/>
<wire x1="190.5" y1="81.28" x2="187.96" y2="81.28" width="0.1524" layer="91"/>
</segment>
</net>
<net name="ECHO_TX" class="0">
<segment>
<wire x1="48.26" y1="83.82" x2="45.72" y2="81.28" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="12"/>
<wire x1="45.72" y1="81.28" x2="35.56" y2="81.28" width="0.1524" layer="91"/>
</segment>
<segment>
<wire x1="193.04" y1="86.36" x2="190.5" y2="83.82" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$1" pin="10"/>
<wire x1="190.5" y1="83.82" x2="187.96" y2="83.82" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$8" class="0">
<segment>
<pinref part="U$1" gate="G$2" pin="20"/>
<wire x1="35.56" y1="43.18" x2="43.18" y2="43.18" width="0.1524" layer="91"/>
<wire x1="43.18" y1="43.18" x2="43.18" y2="7.62" width="0.1524" layer="91"/>
<wire x1="43.18" y1="7.62" x2="157.48" y2="7.62" width="0.1524" layer="91"/>
<wire x1="157.48" y1="7.62" x2="157.48" y2="88.9" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$1" pin="5"/>
<wire x1="157.48" y1="88.9" x2="172.72" y2="88.9" width="0.1524" layer="91"/>
</segment>
</net>
<net name="GPIO_OUT" class="0">
<segment>
<wire x1="40.64" y1="40.64" x2="38.1" y2="38.1" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$2" pin="24"/>
<wire x1="38.1" y1="38.1" x2="35.56" y2="38.1" width="0.1524" layer="91"/>
</segment>
<segment>
<wire x1="195.58" y1="40.64" x2="193.04" y2="38.1" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$2" pin="24"/>
<wire x1="193.04" y1="38.1" x2="187.96" y2="38.1" width="0.1524" layer="91"/>
</segment>
</net>
<net name="GPIO_IN" class="0">
<segment>
<wire x1="40.64" y1="38.1" x2="38.1" y2="35.56" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$2" pin="26"/>
<wire x1="38.1" y1="35.56" x2="35.56" y2="35.56" width="0.1524" layer="91"/>
</segment>
<segment>
<wire x1="195.58" y1="38.1" x2="193.04" y2="35.56" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$2" pin="26"/>
<wire x1="193.04" y1="35.56" x2="187.96" y2="35.56" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$9" class="0">
<segment>
<pinref part="R4" gate="D" pin="1"/>
<pinref part="U$2" gate="G$3" pin="16"/>
<wire x1="226.06" y1="76.2" x2="231.14" y2="76.2" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$10" class="0">
<segment>
<pinref part="U$2" gate="G$3" pin="14"/>
<pinref part="R4" gate="C" pin="1"/>
<wire x1="231.14" y1="78.74" x2="226.06" y2="78.74" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$11" class="0">
<segment>
<pinref part="U$2" gate="G$3" pin="12"/>
<pinref part="R4" gate="B" pin="1"/>
<wire x1="231.14" y1="81.28" x2="226.06" y2="81.28" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$12" class="0">
<segment>
<pinref part="U$2" gate="G$3" pin="10"/>
<pinref part="R4" gate="A" pin="1"/>
<wire x1="231.14" y1="83.82" x2="226.06" y2="83.82" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$13" class="0">
<segment>
<pinref part="U$2" gate="G$3" pin="13"/>
<pinref part="R5" gate="A" pin="2"/>
</segment>
</net>
<net name="N$14" class="0">
<segment>
<pinref part="U$2" gate="G$3" pin="15"/>
<pinref part="R5" gate="B" pin="2"/>
</segment>
</net>
<net name="N$15" class="0">
<segment>
<pinref part="U$2" gate="G$3" pin="17"/>
<pinref part="R5" gate="C" pin="2"/>
</segment>
</net>
<net name="N$16" class="0">
<segment>
<pinref part="U$2" gate="G$3" pin="19"/>
<pinref part="R5" gate="D" pin="2"/>
</segment>
</net>
<net name="+5V" class="0">
<segment>
<pinref part="P7" gate="G$1" pin="+5V"/>
<wire x1="-15.24" y1="147.32" x2="-15.24" y2="142.24" width="0.1524" layer="91"/>
<pinref part="J1" gate="A" pin="B9"/>
<wire x1="-15.24" y1="142.24" x2="-5.08" y2="142.24" width="0.1524" layer="91"/>
<pinref part="J1" gate="A" pin="A9"/>
<wire x1="-5.08" y1="134.62" x2="-15.24" y2="134.62" width="0.1524" layer="91"/>
<wire x1="-15.24" y1="134.62" x2="-15.24" y2="142.24" width="0.1524" layer="91"/>
<junction x="-15.24" y="142.24"/>
</segment>
<segment>
<pinref part="U$4" gate="G$1" pin="4"/>
<wire x1="144.78" y1="81.28" x2="147.32" y2="81.28" width="0.1524" layer="91"/>
<pinref part="P11" gate="G$1" pin="+5V"/>
<wire x1="147.32" y1="81.28" x2="147.32" y2="83.82" width="0.1524" layer="91"/>
</segment>
<segment>
<pinref part="U$5" gate="G$1" pin="4"/>
<wire x1="-5.08" y1="81.28" x2="-2.54" y2="81.28" width="0.1524" layer="91"/>
<pinref part="P10" gate="G$1" pin="+5V"/>
<wire x1="-2.54" y1="81.28" x2="-2.54" y2="83.82" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$17" class="0">
<segment>
<pinref part="R6" gate="G$1" pin="2"/>
<wire x1="-22.86" y1="132.08" x2="-22.86" y2="137.16" width="0.1524" layer="91"/>
<pinref part="J1" gate="A" pin="B5"/>
<wire x1="-22.86" y1="137.16" x2="-5.08" y2="137.16" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$18" class="0">
<segment>
<pinref part="J1" gate="A" pin="A5"/>
<wire x1="-5.08" y1="139.7" x2="-33.02" y2="139.7" width="0.1524" layer="91"/>
<pinref part="R7" gate="G$1" pin="2"/>
<wire x1="-33.02" y1="139.7" x2="-33.02" y2="132.08" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$21" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="1"/>
<wire x1="129.54" y1="78.74" x2="111.76" y2="78.74" width="0.1524" layer="91"/>
<wire x1="111.76" y1="78.74" x2="111.76" y2="83.82" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$3" pin="10"/>
<wire x1="111.76" y1="83.82" x2="73.66" y2="83.82" width="0.1524" layer="91"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
