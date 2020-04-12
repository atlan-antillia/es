
<html>
<head>
<!--
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta name="description"
 content="ES++, es++,OZ++, oz++, OZ++1.0, OZ++2.0, OpenGL, Netlink++, Motif++,Class Library for Linux, Free C++ class library for Linux is available.">
 
<meta name="keywords"
         content="Es++; es++;OZ++; oz++; OZ++1.0; OZ++2.0; OpenGL;Netlink++; Motif++; Linux Class Library; C++ Class Library; Object Oriented Programming;C++">

<meta name="Author" content="Tom Atlan">
<meta http-equiv="Pragma:" content="no-cache">

-->


<style type="text/css">
body {
    color: #00; background: #FFF;
    margin-left:0; margin-right:0;
    margin-top:0;
	margin-bottom:0;
    font-family: arial,verdana,helvetica,tahoma,sans-serif; 
}
a:link { color: "#003344"}
a:hover {color:"red"}
a:visited { color: #72f; background:#fff}    

td.left a:link { color: "#003344"}
td.left a:hover {color:"red"}
td.left a:visited { color: #003344 ; background: #ddeeff}

td {
  word-wrap:break-word;
}
<!-- 
@media (max-width: 480px) {
  img{
    max-width: 95%;
    height: auto;
  }
}

</style>


<script type="text/javascript">
function confirmation(message, url) {
  if(window.confirm(message)){
    location.href = url + ".zip"; 
  } else{
      //window.alert('Canceled');
  }
}
</script>
-->

<!--
#00eecc }
 -->
<title>
Antillia.com
</title>
</head>

<body bgcolor="white" topmargin="0" leftmargin="0">
<table cellpadding="3"  border="0" width="100%" cellspacing="0" >
<tr align="left">
<td bgcolor="#003344" align="middle" background='../images/solarsystem.jpg' >

<font size=6 color ="white" >

&nbsp;&nbsp;Welcome to Software Laboratory of Antillia.com.<br>
&nbsp;&nbsp;&nbsp;&nbsp;Future Windows and Linux Programming<br>
</font>
</td>
</table>


<!-- Right Begin -->
<!--
================================================================================================
-->

<br clear="all">

 
<TABLE CELLSPACING="0" CELLPADDING="4" BORDER="0" WIDTH="100%">
<TR>
<TD COLSPAN="3" HEIGHT="20" VALIGN="MIDDLE" BGCOLOR="#003399">

<!--
<TD COLSPAN="3" HEIGHT="20" VALIGN="MIDDLE" BGCOLOR="darkgreen">
-->
<FONT size="5" 
COLOR="#ffffff" SIZE="-1">&nbsp;&nbsp;Es++ Class Library
</FONT></TD>
</TR></TABLE>

<!-- Top anchors begin -->

<table cellpadding="4"  border="0"  cellspacing="4"><tr>

<td class="left">
&nbsp;
<a href="../index.html">
<font size =2 color="navy">
<b> Home</b>
</font>
</a>
&nbsp;
</td>

<td  class ="left">
&nbsp;
<a href="http://www.antillia.com/es++/samples.html">
<font size=2 color="navy"><b>Es++ Samples
</b>
</font>
</a>
</td>
<td  class ="left">
&nbsp;
<a href="http://www.antillia.com/es++/classlist.html">
<font size=2 color="navy">
<b>Es++ ClassList
</b>
</font>
</a>
</td>
</tr>
</table>


<!-- Top anchors end -->

<!--
<font size = 2 color="red">
<b>
<li>25 Mar 2001:Added chapter 5.
</b>
</font>
-->
<font size = 3 color="black">
<hr noshade color ="gray">

<b>

<b>
<font size=2>
1 Download Es++<br>
</b>
</font>

<font size=2>
The latest Es++ library for Linux and sample programs is available. 
</font>

</b>
<br><br>
<!--
<b>
<li><a href="./es++2.0.3.zip">
Es++ Class Library 2.0.3 for Ubuntu 16.04 64 
</font>
</a><font size=2>(es++2.0.5.zip:</font><font color="red" size=2> #2018.08.21</font>)</li><br>
-->

<font color="red" size=2>

Download:<br>
<a href="http://www.antillia.com/es++/es++2.0.9.zip">
Es++ Class Library 2.0.9 for Ubuntu 18.04.02 (2020/02/12)</a>

</font>
<br>
</b>
</font>
<!--
<font size=2>
<li><a href="./es++1.0.2.zip">
Deprecated: Es++ Class Library 1.0.2 for Ubuntu 14.04 and OpenCV 2.8
</font>
</a><font size=2>(es++1.0.2.zip:</font><font color="red" size=2> #2017.07.26</font>)</li>
-->
<font color="black" size=2>

This libary contains both a set of C++ class header files and a lot of sample programs of Es++. <br>
</font>
<br>
<font size=2>
<b>
2 What is Es++? <br>
</b>
</font>

<font size=2>
<b>2.1 Es++ 2</b>
<br>

&nbsp;Es++ is an experimental and elementary C++ class library based on <a href="http://www.antillia.com/oz++/library.html">OZ++1.0</a>
 and <a href="https://www.gtkmm.org/en/">gtkmm C++ Interfaces for GTK+ and GNOME</a> GUI class library
for Linux.<br>
We provide a set of C++ class header files and a lot of sample programs for the libary.<br>
&nbsp;On Es++ we implemented all the member functions inside the C++ classes. 
This is not a conventional coding style of C++ of separating implementation and interface.
 But this coding style has one point which enables a rapid prototyping of writing and testing C++ prograqms quickly. 
 It may (or may not) be useful for various experimentation projects.
<br>
<br> 

&nbsp;We developed this Es++ 2 library by using g++ 7.3.0 on Ubuntu 18.04.02 LTS, but
 didn't test it on other g++ versions or Linux distributions.<br><br> 
$ g++ --version<br>
g++ (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0<br>
<br>

&nbsp;In the latest Es++ 2, we have modified some C++ classes including <a href="./classes/es++/PNGFile.html">PNGFile</a> to be compiled on the latest libpng1636
and opengl packages, and implemented <font color="red">OpenCV-4.2.0</font> GUI classes by using
 <a href="https://developer.gnome.org/gtkmm/stable/classGdk_1_1Pixbuf.html">Gdk::Pixbuf</a> class 
 and <a href="https://developer.gnome.org/gtkmm/stable/classGtk_1_1DrawingArea.html"> Gtk::DrawingArea</a> class,
 and a lot of elementary sample programs of <a href="http://opencv.org/releases.html">OpenCV-4.2.0.</a><br>
 <br>
Furthermore, we have modified <a href="./classes/opengl/OpenGLObject.html">OpenGLObject</a> class not to include "/usr/include/GL/glext.h", because
there are some typedef inconsistencis between "/usr/include/GL/glew.h" as shown below, e.g. :<br>
<br>
/usr/include/GL/glext.h:12098:92:‘void __glewGetFragmentMaterialfvSGIX(GLenum, GLenum, GLfloat*)’ <br>
/usr/include/GL/glew.h:19356:52: ‘void (* __glewGetFragmentMaterialfvSGIX)(GLenum, GLenum, const GLfloat*)’<br>
<br>

&nbsp;We have also implemented some C++ classes to support GUI for gtkmm-3.0 and added 
sample programs based on the Es++ 2 library.<br>
 <br>
<br>


<font size=2>
<b>
2.2 Development environment for Es++ 2<br>
</b>
</font>
<font size=2>

&nbsp;In Es++ 2, we have been using latest <a href="https://www.ubuntu.com/download/desktop">Ubuntu 18.04.02 LTS</a>,
and <a href="http://docs.opencv.org/4.0.1/">OpenCV-4.2.0</a> respectively.
In the version es++2.0.9, to use the legacy C constants and new APIs in <font color="red">OpenCV-4.2.0</font>, we have updated
<a href="./classes/opencv-4.0/OpenCVObject.html">OpenCVObject</a> and <a href="./classes/opencv-4.0/OpenCVImageConverter.html">OpenCVImageConverter</a> classes.
 Furthermore, we have modified the Makefiles not to use pkg-config(see <a href="https://github.com/opencv/opencv/issues/13154">4.0.0 does not produce pkgconfig file · Issue #13154 · opencv-opencv ·</a>).<br>
<br>To use the libary of OpenCV, we have installed <a href="https://launchpad.net/ubuntu/trusty/+package/libgtkmm-3.0-dev">gtkmm-3.3</a> library on the Ubuntu 16.04 system

In the version es++2.0.3, we have implemented some experimetal C++ classes to support OpenGL and its extention on gtkmm-3.0, 
which are based on <a href="http://freeglut.sourceforge.net/">freeglut</a>
and <a href="http://glew.sourceforge.net/">The OpenGL Extension Wrangler Library (GLEW)</a> libraries.
We did not use <a href="https://github.com/GNOME/gtkglextmm/blob/master/gtkglextmm.spec.in">gtkglextmm</a> library, because it is for gtkmm2.
<br><br>

We have added some development packages to the system in the following way, on the console.<br>
<br>

<table cellpadding="4"  width="98%" border=1 style='border-collapse: collapse;'>
<tr>
<td bgcolor='floralwhite'>
<font size=2>
1. Update and upgrade<br> 
sudo apt-get update<br>
sudo apt-get upgrade<br>
<br>
2. Install the latest g++ to support c++11<br>
sudo apt-get install g++<br>
<br>
3. Install the latest gtkmm-3.0<br>
sudo apt-get install libgtkmm-3.0-dev for gtkmm-3.0<br>
<br>
4. Download and install OpenCV-4.2.0<br>
 Download <a href="https://github.com/opencv/opencv/archive/4.2.0.zip">opencv-4.2.0.zip</a><br>
 Install it on the <a href="http://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html">Tutorial Linux install</a><br>
<br>
5. Download and install freeglut-3.0.0<br>
Download <a href="http://prdownloads.sourceforge.net/freeglut/freeglut-3.0.0.tar.gz?download">freeglut-3.0.0</a><br>
 and install it.<br>
 <br>
6. Download and install GLEW<br>
Download <a href="https://sourceforge.net/projects/glew/files/glew/2.1.0/">glew-2.1.0</a><br>
 and install it.<br>
 <br>

</font>
</td></tr>
</table>
<br>
<br>
<font size=2>
<b>
<br>
<font size=2 color="black">
<b>2.3 Es++ Features<br>
</font>
</b>
<font size=2 color="red">
<br>
 
 1. Es++ is basically single inheritance oriented.<br>
 2. Es++ classes have been implemented in C++ header files using inline member functions.<br>
 3. Es++ applications only have to include Es++ class header files, and they can be compiled without linking any OZ++ specific shared or static library.<br>
 4. Es++ will support multiple charsets of ASCII(char) and UNICODE(wchar_t).<br>
 
</font>
<br>

<font size=2>
<br>
<br>
<font size=3>

<hr noshade color = "gray">
<b>
2 How to install Es++ <br>
</b>
</b>
<font size=2 color="black">
You download the zip file es++.zip, and unzip it on your working directory. For example, 
by unzipping on a directory ~/work, you get the following directories:
<br>
<!--
<pre>
-->
<br><b>
2.1 Es++ 2<br>

In the latest version, we have removed obsolete gtkmm-2.0 and opencv-3.0 folders.

</b><br><br>

<b>~/work/es++/iclude </b><br>
, which includes non-gui classes, gtkmm-3.0, opencv-4.0, opengl, opengl2, openglarb, openglext C++ header files for Es++ 2.0.<br>
<br>

<b>~/work/es++/samples</b><br>
, which includes all NON-GUI sample program files for Es++ and build.sh shell script to compile them.
<br>
<br>
<b>~/work/es++/gtkmm-3.0</b><br>
, which includes some sample program files for gtkmm-3.0 and build.sh shell script to compile them.
<br>
<br>
<b>~/work/es++/netlink</b><br>
, which includes elementary sample program files for netlink and build.sh shell script to compile them.
<br>

<br>

<font size=2 color="red">
<b>[NEW] ~/work/es++/opencv-4.0</b><br>
, which includes elementary sample program files for <font color="red">opencv-4.2.0</font> and build.sh shell script to compile them.
</font>
<br>
<br>

<b>~/work/es++/opengl</b><br>
, which includes elementary sample program files for opengl and build.sh shell script to compile them.
<br>
<br>
<b>~/work/es++/opengl2</b><br>
, which includes elementary sample program files for opengl2 and build.sh shell script to compile them.
<br>
<br>

<br>


<!-- bottom anchors end -->
<!--
<font size= 2 >
<hr noshade color = "lightgray">
<b>
Last modified: 21 Aug. 2018 
</b>
</font>
-->

<!-- Right End -->

<!--
================================================================================================
-->

</td>

</tr>
</table>

<font size = 2 color = "navy" >
<hr noshade color="navy">
&nbsp;<b>Last modified: 12 Feb. 2020</b>
</font>
<br>
<font color = "navy" size ="2">
<b>
Copyright (c) 2019-2020  Antillia.com ALL RIGHTS RESERVED. 
</b>
</font>
</body>
</html>

