
----------------------
-- Structura složek --
----------------------

- PV112
   - libs                  .. složka, ve které budeme mít knihovny, co budeme používat
      - GL
	  - glm
	  - IL
      - *.lib, *.dll
      - readme.txt
   - cv1                   .. složka s prvním cvièením
      - cv1.sln            .. solution Visual Studia
      - cv1                .. další složka cv1
         - cv1.vcxproj, cv1.vcxproj.filters, cv1.vcxproj.user
         - *.h, *.cpp      .. zdrojové kódy cvièení
         - *.glsl          .. kódy shaderù
   - cv2
      - ... stejnì jako cv1
   - cv3
      - ... stejnì jako cv1
   - ...

--------------
-- FreeGLUT --
--------------

web pages: http://freeglut.sourceforge.net
download (MSVC Package): http://www.transmissionzero.co.uk/software/freeglut-devel/

Nebo stáhnìte a zkompilujte zdrojový kód.

----------
-- GLEW --
----------

web pages: http://glew.sourceforge.net/
download: https://sourceforge.net/projects/glew/files/glew/1.13.0/glew-1.13.0-win32.zip/download

Nebo stáhnìte a zkompilujte zdrojový kód.

---------
-- GLM --
---------

web pages: http://glm.g-truc.net/
download: https://github.com/g-truc/glm/releases/download/0.9.7.3/glm-0.9.7.3.zip

-----------
-- DevIL --
-----------

web pages: http://openil.sourceforge.net/
download: http://downloads.sourceforge.net/openil/DevIL-SDK-x86-1.7.8.zip
