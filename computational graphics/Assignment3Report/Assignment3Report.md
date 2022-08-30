
# 2.
## using the example code and updating it to fit my code:
## managed to load each mesh model opened into the gpu using the opengl api.
![image](https://user-images.githubusercontent.com/75092336/111061605-e966c580-84ac-11eb-8eb0-70daea263270.png)
![image](https://user-images.githubusercontent.com/75092336/111061613-f683b480-84ac-11eb-81cc-3fd64bb6ccc2.png)
# 3.
## the matrices were passed to the shaders as uniform from the cpu (render.cpp):
![image](https://user-images.githubusercontent.com/75092336/111061653-29c64380-84ad-11eb-8e41-fa6350b45ca5.png)
# 4.
## where fColor was unchanged (out vec3 fColor) in the fragment shader.
![image](https://user-images.githubusercontent.com/75092336/111061681-419dc780-84ad-11eb-8e86-738d88790a6c.png)
# 5.
![image](https://user-images.githubusercontent.com/75092336/111061699-5a0de200-84ad-11eb-8abe-23e2168562fc.png)
![image](https://user-images.githubusercontent.com/75092336/111061729-66923a80-84ad-11eb-873b-8bda699faf2d.png)

# 6. picked cow.obj to demo the shading.
## code:
![image](https://user-images.githubusercontent.com/75092336/111061490-54fc6300-84ac-11eb-9c77-e627ca963b01.png)
![image](https://user-images.githubusercontent.com/75092336/111061496-5c237100-84ac-11eb-954b-26032c93044f.png)
![image](https://user-images.githubusercontent.com/75092336/111061741-790c7400-84ad-11eb-9046-b2c91f4f633a.png)

![image](https://user-images.githubusercontent.com/75092336/111061257-1c0fbe80-84ab-11eb-9f06-17f2c28d1868.png)
## colors of the lighting changed.
## ambient: green, diffuse:red, specular: green.
![image](https://user-images.githubusercontent.com/75092336/111061300-482b3f80-84ab-11eb-9c55-8d0e650cd22e.png)
## light position changed to 2,1,0 (was 0,0,0 initially)
![image](https://user-images.githubusercontent.com/75092336/111061357-a0624180-84ab-11eb-81c5-4820e7336cc9.png)
## material color lights changed same as before
![image](https://user-images.githubusercontent.com/75092336/111061372-b7a12f00-84ab-11eb-84dd-9743b09d2e4c.png)
## to demo light rotation the cow was rotated to face the camera, and switched light's position -
## to 10,0,0
![image](https://user-images.githubusercontent.com/75092336/111061407-f1723580-84ab-11eb-9285-a22fb20114e1.png)
## rotated light  by ~180 degrees
![image](https://user-images.githubusercontent.com/75092336/111061418-02bb4200-84ac-11eb-88d9-7f8b0bd95d4c.png)
# can see that it rotated from right side to the left side.
## camera rotation by 75 degrees
## the light rotates with the model when camera is rotated.
![image](https://user-images.githubusercontent.com/75092336/111061452-27171e80-84ac-11eb-946d-ea5553021ec4.png)
# 7. toon shading.
## used this website for reference https://www.lighthouse3d.com/tutorials/glsl-12-tutorial/toon-shader-version-ii/
## code: 
![image](https://user-images.githubusercontent.com/75092336/111061921-68a8c900-84ae-11eb-9e2a-c00a5d1fc373.png)
## toon bunny.obj
![image](https://user-images.githubusercontent.com/75092336/111061902-5169db80-84ae-11eb-8014-d56b63866e3c.png)
