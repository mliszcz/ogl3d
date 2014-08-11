ogl3d
=====

### building application

1. clone repository

1. *make*, using *makefile* from *Debug* directory  
   `ogl3d $ make -C ./Debug all`
   
1. run from *ogl3d* directory  
   `ogl3d $ ./Debug/ogl3d`   
   **NOTE** current working directory must be *ogl3d*. cause resource paths are relative to this directory

### controls

* w/s/a/d, arrows - move car

* LMB + mouse drag - move camera

* mouse wheel up/down - zoom in/out

* ESC - exit

### third-party stuff

1. glm  
   http://glm.g-truc.net
   
1. gli (commented-out gli/core/save_dds.inl:142)  
   http://gli.g-truc.net
   
1. tiny-obj-loader (changed into header-only)  
   https://github.com/syoyo/tinyobjloader
   
1. car model 
   http://www.turbosquid.com/3d-models/mustang-gt-blend-free/645200
   
1. asphalt texture  
   http://artbaggage.com/business-calendars-bundle/  
   http://artbaggage.com/wp-content/gallery/asphalt_road_patterns/asphalt_9_512x512.png
   
1. per-pixel fog equations  
   http://www.ozone3d.net/tutorials/glsl_fog/p04.php

   

